#include "modelo/Partida.h"

#include "modelo/BugBasico.h"
#include "modelo/BugResistente.h"

Partida::Partida()
    : tiempoTranscurrido(0.0f),
      estado(EstadoPartida::Jugando),
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
    crearEnemigosIniciales();
}

void Partida::actualizar(float deltaTiempo)
{
    if (deltaTiempo <= 0.0f ||
        estado != EstadoPartida::Jugando)
    {
        return;
    }

    tiempoTranscurrido += deltaTiempo;

    profesor.actualizar(deltaTiempo);
    estudiante.actualizar(deltaTiempo);

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
    crearEnemigosIniciales();
}

bool Partida::moverJugador(
    IdJugador idJugador,
    Direccion direccion
)
{
    if (estado != EstadoPartida::Jugando)
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
    if (estado != EstadoPartida::Jugando)
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

void Partida::crearEnemigosIniciales()
{
    enemigos.push_back(
        std::make_unique<BugBasico>(6, 7)
    );

    enemigos.push_back(
        std::make_unique<BugResistente>(8, 10)
    );
}

void Partida::comprobarEstadoFinal()
{
    if (!profesor.estaActivo() &&
        !estudiante.estaActivo())
    {
        estado = EstadoPartida::Derrota;
        return;
    }

    bool existeEnemigoActivo = false;

    for (const std::unique_ptr<Enemigo>& enemigo :
         enemigos)
    {
        if (enemigo->estaActivo())
        {
            existeEnemigoActivo = true;
            break;
        }
    }

    if (!existeEnemigoActivo)
    {
        estado = EstadoPartida::Victoria;
    }
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
    aplicarDanioSiCorresponde(estudiante);

    for (const std::unique_ptr<Enemigo>& enemigo :
         enemigos)
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
    for (const std::unique_ptr<Enemigo>& enemigo :
         enemigos)
    {
        if (!enemigo->estaActivo())
        {
            continue;
        }

        enemigo->actualizarEstado(deltaTiempo);

        BugBasico* bugBasico =
            dynamic_cast<BugBasico*>(
                enemigo.get()
            );

        if (bugBasico != nullptr)
        {
            bugBasico->actualizarConCargas(
                deltaTiempo,
                tablero,
                sistemaCargas,
                profesor,
                estudiante
            );
        }
        else
        {
            enemigo->actualizar(
                deltaTiempo,
                tablero,
                profesor,
                estudiante
            );
        }
    }
}

void Partida::resolverDanioPorContacto()
{
    aplicarDanioPorContacto(profesor);
    aplicarDanioPorContacto(estudiante);
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

    for (const std::unique_ptr<Enemigo>& enemigo :
         enemigos)
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
