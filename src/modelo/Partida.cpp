#include "modelo/Partida.h"

#include "modelo/BugBasico.h"

Partida::Partida()
    : tiempoTranscurrido(0.0f),
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
    enemigos.push_back(
        std::make_unique<BugBasico>(6, 7)
    );
}

void Partida::actualizar(float deltaTiempo)
{
    if (deltaTiempo < 0.0f)
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

    actualizarEnemigos(deltaTiempo);
    resolverDanioExplosiones();
}

bool Partida::moverJugador(
    IdJugador idJugador,
    Direccion direccion
)
{
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

    return true;
}

bool Partida::colocarCarga(IdJugador idJugador)
{
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

void Partida::actualizarEnemigos(float deltaTiempo)
{
    for (const std::unique_ptr<Enemigo>& enemigo : enemigos)
    {
        if (enemigo->estaActivo())
        {
            enemigo->actualizar(deltaTiempo);
        }
    }
}
