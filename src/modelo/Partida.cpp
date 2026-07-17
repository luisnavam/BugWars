#include "modelo/Partida.h"

#include "modelo/BugBasico.h"
#include "modelo/BugResistente.h"

Partida::Partida()
    : tiempoTranscurrido(0.0f),
      estado(EstadoPartida::Jugando),
      modo(ModoJuego::Duo),
      oleadaActual(1),
      enTransicionOleada(false),
      tiempoTransicionOleada(0.0f),
      tablero(),
      profesor("Profesor", 1, 1),
      estudiante(
          "Estudiante",
          Tablero::FILAS - 2,
          Tablero::COLUMNAS - 2
      ),
      sistemaCargas(),
      enemigos()
{
    crearOleadaActual();
}

void Partida::iniciar(
    ModoJuego nuevoModo
)
{
    modo = nuevoModo;
    reiniciar();
}

void Partida::actualizar(float deltaTiempo)
{
    if (deltaTiempo <= 0.0f ||
        estado != EstadoPartida::Jugando)
    {
        return;
    }

    if (enTransicionOleada)
    {
        tiempoTransicionOleada -= deltaTiempo;

        if (tiempoTransicionOleada <= 0.0f)
        {
            tiempoTransicionOleada = 0.0f;
            enTransicionOleada = false;
            crearOleadaActual();
        }

        return;
    }

    tiempoTranscurrido += deltaTiempo;

    profesor.actualizar(deltaTiempo);

    if (esModoDuo())
    {
        estudiante.actualizar(deltaTiempo);
    }

    sistemaCargas.actualizar(
        deltaTiempo,
        tablero
    );

    resolverDanioExplosiones();
    actualizarEnemigos(deltaTiempo);
    resolverDanioPorContacto();

    comprobarEstadoFinal();
}

void Partida::reiniciar()
{
    tiempoTranscurrido = 0.0f;
    estado = EstadoPartida::Jugando;

    oleadaActual = 1;
    enTransicionOleada = false;
    tiempoTransicionOleada = 0.0f;

    tablero = Tablero();

    profesor = Jugador(
        "Profesor",
        1,
        1
    );

    estudiante = Jugador(
        "Estudiante",
        Tablero::FILAS - 2,
        Tablero::COLUMNAS - 2
    );

    sistemaCargas = SistemaCargas();

    enemigos.clear();
    crearOleadaActual();
}

bool Partida::jugadorHabilitado(
    IdJugador idJugador
) const
{
    if (idJugador == IdJugador::Profesor)
    {
        return true;
    }

    return esModoDuo();
}

bool Partida::moverJugador(
    IdJugador idJugador,
    Direccion direccion
)
{
    if (estado != EstadoPartida::Jugando ||
        enTransicionOleada ||
        !jugadorHabilitado(idJugador))
    {
        return false;
    }

    Jugador& jugador = obtenerJugador(idJugador);

    if (!jugador.estaActivo())
    {
        return false;
    }

    int nuevaFila = jugador.obtenerFila();
    int nuevaColumna = jugador.obtenerColumna();

    switch (direccion)
    {
        case Direccion::Arriba:
            --nuevaFila;
            break;

        case Direccion::Abajo:
            ++nuevaFila;
            break;

        case Direccion::Izquierda:
            --nuevaColumna;
            break;

        case Direccion::Derecha:
            ++nuevaColumna;
            break;
    }

    if (!tablero.esTransitable(
            nuevaFila,
            nuevaColumna
        ))
    {
        return false;
    }

    if (posicionOcupadaPorOtroJugador(
            idJugador,
            nuevaFila,
            nuevaColumna
        ))
    {
        return false;
    }

    if (sistemaCargas.existeCargaEn(
            nuevaFila,
            nuevaColumna
        ))
    {
        return false;
    }

    jugador.cambiarPosicion(
        nuevaFila,
        nuevaColumna
    );

    resolverDanioPorContacto();
    comprobarEstadoFinal();

    return true;
}

bool Partida::colocarCarga(
    IdJugador idJugador
)
{
    if (estado != EstadoPartida::Jugando ||
        enTransicionOleada ||
        !jugadorHabilitado(idJugador))
    {
        return false;
    }

    const Jugador& jugador =
        idJugador == IdJugador::Profesor
        ? profesor
        : estudiante;

    if (!jugador.estaActivo())
    {
        return false;
    }

    return sistemaCargas.colocarCarga(
        jugador.obtenerFila(),
        jugador.obtenerColumna(),
        idJugador
    );
}

EstadoPartida Partida::obtenerEstado() const
{
    return estado;
}

ModoJuego Partida::obtenerModo() const
{
    return modo;
}

bool Partida::esModoDuo() const
{
    return modo == ModoJuego::Duo;
}

int Partida::obtenerOleadaActual() const
{
    return oleadaActual;
}

int Partida::obtenerBugsRestantes() const
{
    int cantidad = 0;

    for (const std::unique_ptr<Enemigo>& enemigo : enemigos)
    {
        if (enemigo->estaActivo())
        {
            ++cantidad;
        }
    }

    return cantidad;
}

bool Partida::estaEnTransicionOleada() const
{
    return enTransicionOleada;
}

float Partida::obtenerTiempoTransicionOleada() const
{
    return tiempoTransicionOleada;
}

const Tablero& Partida::obtenerTablero() const
{
    return tablero;
}

const Jugador& Partida::obtenerProfesor() const
{
    return profesor;
}

const Jugador& Partida::obtenerEstudiante() const
{
    return estudiante;
}

const std::vector<CargaCodigo>&
Partida::obtenerCargas() const
{
    return sistemaCargas.obtenerCargas();
}

const std::vector<Explosion>&
Partida::obtenerExplosiones() const
{
    return sistemaCargas.obtenerExplosiones();
}

const std::vector<std::unique_ptr<Enemigo>>&
Partida::obtenerEnemigos() const
{
    return enemigos;
}

void Partida::crearOleadaActual()
{
    enemigos.clear();

    switch (oleadaActual)
    {
        case 1:
            crearEnemigosDeOleada(2, 0);
            break;

        case 2:
            crearEnemigosDeOleada(2, 1);
            break;

        case 3:
            crearEnemigosDeOleada(3, 2);
            break;

        case 4:
            crearEnemigosDeOleada(4, 3);
            break;

        default:
            estado = EstadoPartida::Victoria;
            break;
    }
}

void Partida::crearEnemigosDeOleada(
    int cantidadBasicos,
    int cantidadResistentes
)
{
    for (int i = 0; i < cantidadBasicos; ++i)
    {
        agregarBugBasicoEnPosicionLibre();
    }

    for (int i = 0; i < cantidadResistentes; ++i)
    {
        agregarBugResistenteEnPosicionLibre();
    }
}

bool Partida::agregarBugBasicoEnPosicionLibre()
{
    for (int fila = 1; fila < Tablero::FILAS - 1; ++fila)
    {
        for (int columna = 1;
             columna < Tablero::COLUMNAS - 1;
             ++columna)
        {
            if (posicionDisponibleParaEnemigo(fila, columna))
            {
                enemigos.push_back(
                    std::make_unique<BugBasico>(fila, columna)
                );

                return true;
            }
        }
    }

    return false;
}

bool Partida::agregarBugResistenteEnPosicionLibre()
{
    for (int fila = Tablero::FILAS - 2; fila >= 1; --fila)
    {
        for (int columna = Tablero::COLUMNAS - 2;
             columna >= 1;
             --columna)
        {
            if (posicionDisponibleParaEnemigo(fila, columna))
            {
                enemigos.push_back(
                    std::make_unique<BugResistente>(fila, columna)
                );

                return true;
            }
        }
    }

    return false;
}

bool Partida::posicionDisponibleParaEnemigo(
    int fila,
    int columna
) const
{
    if (!tablero.esTransitable(fila, columna))
    {
        return false;
    }

    const int diferenciaFilaProfesor =
        fila > profesor.obtenerFila()
        ? fila - profesor.obtenerFila()
        : profesor.obtenerFila() - fila;

    const int diferenciaColumnaProfesor =
        columna > profesor.obtenerColumna()
        ? columna - profesor.obtenerColumna()
        : profesor.obtenerColumna() - columna;

    if (diferenciaFilaProfesor + diferenciaColumnaProfesor < 4)
    {
        return false;
    }

    if (esModoDuo())
    {
        const int diferenciaFilaEstudiante =
            fila > estudiante.obtenerFila()
            ? fila - estudiante.obtenerFila()
            : estudiante.obtenerFila() - fila;

        const int diferenciaColumnaEstudiante =
            columna > estudiante.obtenerColumna()
            ? columna - estudiante.obtenerColumna()
            : estudiante.obtenerColumna() - columna;

        if (diferenciaFilaEstudiante +
            diferenciaColumnaEstudiante < 4)
        {
            return false;
        }
    }

    for (const std::unique_ptr<Enemigo>& enemigo : enemigos)
    {
        if (enemigo->obtenerFila() == fila &&
            enemigo->obtenerColumna() == columna)
        {
            return false;
        }
    }

    return true;
}

bool Partida::hayEnemigosActivos() const
{
    for (const std::unique_ptr<Enemigo>& enemigo : enemigos)
    {
        if (enemigo->estaActivo())
        {
            return true;
        }
    }

    return false;
}

void Partida::comprobarEstadoFinal()
{
    const bool jugadoresDerrotados =
        esModoDuo()
        ? (!profesor.estaActivo() &&
           !estudiante.estaActivo())
        : !profesor.estaActivo();

    if (jugadoresDerrotados)
    {
        estado = EstadoPartida::Derrota;
        return;
    }

    if (hayEnemigosActivos())
    {
        return;
    }

    if (oleadaActual >= TOTAL_OLEADAS)
    {
        estado = EstadoPartida::Victoria;
        return;
    }

    iniciarSiguienteOleada();
}

void Partida::iniciarSiguienteOleada()
{
    ++oleadaActual;

    enemigos.clear();
    sistemaCargas = SistemaCargas();

    enTransicionOleada = true;
    tiempoTransicionOleada = DURACION_TRANSICION_OLEADA;
}

Jugador& Partida::obtenerJugador(
    IdJugador idJugador
)
{
    if (idJugador == IdJugador::Profesor)
    {
        return profesor;
    }

    return estudiante;
}

bool Partida::posicionOcupadaPorOtroJugador(
    IdJugador idJugador,
    int fila,
    int columna
) const
{
    if (!esModoDuo())
    {
        return false;
    }

    const Jugador& otroJugador =
        idJugador == IdJugador::Profesor
        ? estudiante
        : profesor;

    if (!otroJugador.estaActivo())
    {
        return false;
    }

    return otroJugador.obtenerFila() == fila &&
           otroJugador.obtenerColumna() == columna;
}

void Partida::resolverDanioExplosiones()
{
    aplicarDanioSiCorresponde(profesor);

    if (esModoDuo())
    {
        aplicarDanioSiCorresponde(estudiante);
    }

    for (const std::unique_ptr<Enemigo>& enemigo : enemigos)
    {
        if (!enemigo->estaActivo())
        {
            continue;
        }

        for (const Explosion& explosion :
             sistemaCargas.obtenerExplosiones())
        {
            if (explosion.afecta(
                    enemigo->obtenerFila(),
                    enemigo->obtenerColumna()
                ))
            {
                enemigo->recibirImpacto();
                break;
            }
        }
    }
}

void Partida::aplicarDanioSiCorresponde(
    Jugador& jugador
)
{
    if (!jugador.estaActivo() ||
        jugador.esInvulnerable())
    {
        return;
    }

    for (const Explosion& explosion :
         sistemaCargas.obtenerExplosiones())
    {
        if (explosion.afecta(
                jugador.obtenerFila(),
                jugador.obtenerColumna()
            ))
        {
            jugador.recibirDanio();
            return;
        }
    }
}

void Partida::actualizarEnemigos(
    float deltaTiempo
)
{
    for (const std::unique_ptr<Enemigo>& enemigo : enemigos)
    {
        if (!enemigo->estaActivo())
        {
            continue;
        }

        enemigo->actualizarEstado(deltaTiempo);

        BugBasico* bugBasico =
            dynamic_cast<BugBasico*>(enemigo.get());

        const Jugador& segundoObjetivo =
            esModoDuo()
            ? estudiante
            : profesor;

        if (bugBasico != nullptr)
        {
            bugBasico->actualizarConCargas(
                deltaTiempo,
                tablero,
                sistemaCargas,
                profesor,
                segundoObjetivo
            );
        }
        else
        {
            enemigo->actualizar(
                deltaTiempo,
                tablero,
                profesor,
                segundoObjetivo
            );
        }
    }
}

void Partida::resolverDanioPorContacto()
{
    aplicarDanioPorContacto(profesor);

    if (esModoDuo())
    {
        aplicarDanioPorContacto(estudiante);
    }
}

void Partida::aplicarDanioPorContacto(
    Jugador& jugador
)
{
    if (!jugador.estaActivo() ||
        jugador.esInvulnerable())
    {
        return;
    }

    for (const std::unique_ptr<Enemigo>& enemigo : enemigos)
    {
        if (!enemigo->estaActivo())
        {
            continue;
        }

        const bool mismaFila =
            enemigo->obtenerFila() ==
            jugador.obtenerFila();

        const bool mismaColumna =
            enemigo->obtenerColumna() ==
            jugador.obtenerColumna();

        if (mismaFila && mismaColumna)
        {
            jugador.recibirDanio();
            return;
        }
    }
}
