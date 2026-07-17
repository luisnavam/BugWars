
#include <tau/tau.h>

#include "modelo/SistemaCargas.h"
#include "modelo/Tablero.h"
#include "modelo/TipoCelda.h"
#include "modelo/IdJugador.h"

TEST(SistemaCargas, EstadoInicial)
{
    SistemaCargas sistema;

    CHECK(
        sistema.obtenerCargas().empty(),
        "El sistema debe comenzar sin cargas"
    );

    CHECK(
        sistema.obtenerExplosiones().empty(),
        "El sistema debe comenzar sin explosiones"
    );
}

TEST(SistemaCargas, ColocaUnaCarga)
{
    SistemaCargas sistema;

    const bool colocada = sistema.colocarCarga(
        5,
        5,
        IdJugador::Profesor
    );

    CHECK(
        colocada,
        "Debe permitir colocar una carga"
    );

    REQUIRE(
        sistema.obtenerCargas().size() == 1,
        "Debe existir una carga en el sistema"
    );

    const CargaCodigo& carga =
        sistema.obtenerCargas().front();

    CHECK(
        carga.obtenerFila() == 5,
        "La carga debe conservar la fila"
    );

    CHECK(
        carga.obtenerColumna() == 5,
        "La carga debe conservar la columna"
    );

    CHECK(
        carga.obtenerPropietario() ==
            IdJugador::Profesor,
        "La carga debe conservar su propietario"
    );

    CHECK(
        carga.obtenerAlcance() == 2,
        "La carga debe tener alcance dos"
    );
}

TEST(SistemaCargas, DetectaCargaEnUnaPosicion)
{
    SistemaCargas sistema;

    sistema.colocarCarga(
        5,
        5,
        IdJugador::Profesor
    );

    CHECK(
        sistema.existeCargaEn(5, 5),
        "Debe detectar la carga colocada"
    );

    CHECK(
        !sistema.existeCargaEn(5, 6),
        "No debe detectar una carga en otra posicion"
    );
}

TEST(SistemaCargas, DetectaCargaActivaDelJugador)
{
    SistemaCargas sistema;

    sistema.colocarCarga(
        5,
        5,
        IdJugador::Profesor
    );

    CHECK(
        sistema.jugadorTieneCargaActiva(
            IdJugador::Profesor
        ),
        "El profesor debe tener una carga activa"
    );

    CHECK(
        !sistema.jugadorTieneCargaActiva(
            IdJugador::Estudiante
        ),
        "El estudiante no debe tener una carga activa"
    );
}

TEST(SistemaCargas, NoPermiteDosCargasEnLaMismaPosicion)
{
    SistemaCargas sistema;

    REQUIRE(
        sistema.colocarCarga(
            5,
            5,
            IdJugador::Profesor
        ),
        "La primera carga debe colocarse"
    );

    const bool segundaColocada =
        sistema.colocarCarga(
            5,
            5,
            IdJugador::Estudiante
        );

    CHECK(
        !segundaColocada,
        "No debe permitir dos cargas en la misma posicion"
    );

    CHECK(
        sistema.obtenerCargas().size() == 1,
        "Debe continuar existiendo una sola carga"
    );
}

TEST(SistemaCargas, NoPermiteDosCargasDelMismoJugador)
{
    SistemaCargas sistema;

    REQUIRE(
        sistema.colocarCarga(
            5,
            5,
            IdJugador::Profesor
        ),
        "La primera carga debe colocarse"
    );

    const bool segundaColocada =
        sistema.colocarCarga(
            6,
            6,
            IdJugador::Profesor
        );

    CHECK(
        !segundaColocada,
        "Un jugador no debe tener dos cargas activas"
    );

    CHECK(
        sistema.obtenerCargas().size() == 1,
        "Debe continuar existiendo una sola carga"
    );
}

TEST(SistemaCargas, PermiteUnaCargaPorCadaJugador)
{
    SistemaCargas sistema;

    const bool cargaProfesor =
        sistema.colocarCarga(
            5,
            5,
            IdJugador::Profesor
        );

    const bool cargaEstudiante =
        sistema.colocarCarga(
            6,
            6,
            IdJugador::Estudiante
        );

    CHECK(
        cargaProfesor,
        "Debe permitir la carga del profesor"
    );

    CHECK(
        cargaEstudiante,
        "Debe permitir la carga del estudiante"
    );

    CHECK(
        sistema.obtenerCargas().size() == 2,
        "Debe haber una carga por cada jugador"
    );
}

TEST(SistemaCargas, CreaExplosionAlFinalizarLaCompilacion)
{
    SistemaCargas sistema;
    Tablero tablero;

    sistema.colocarCarga(
        5,
        5,
        IdJugador::Profesor
    );

    sistema.actualizar(
        2.49f,
        tablero
    );

    CHECK(
        sistema.obtenerCargas().size() == 1,
        "La carga debe seguir compilando"
    );

    CHECK(
        sistema.obtenerExplosiones().empty(),
        "Todavia no debe existir una explosion"
    );

    sistema.actualizar(
        0.01f,
        tablero
    );

    CHECK(
        sistema.obtenerCargas().empty(),
        "La carga ejecutada debe eliminarse"
    );

    REQUIRE(
        sistema.obtenerExplosiones().size() == 1,
        "Debe crearse una explosion"
    );

    CHECK(
        sistema.obtenerExplosiones()
            .front()
            .estaActiva(),
        "La explosion creada debe estar activa"
    );
}

TEST(SistemaCargas, ExplosionIncluyeLaPosicionCentral)
{
    SistemaCargas sistema;
    Tablero tablero;

    sistema.colocarCarga(
        5,
        5,
        IdJugador::Profesor
    );

    sistema.actualizar(
        2.49f,
        tablero
    );

    sistema.actualizar(
        0.01f,
        tablero
    );

    REQUIRE(
        !sistema.obtenerExplosiones().empty(),
        "Debe existir una explosion"
    );

    const Explosion& explosion =
        sistema.obtenerExplosiones().front();

    CHECK(
        explosion.afecta(5, 5),
        "La explosion debe afectar su posicion central"
    );
}

TEST(SistemaCargas, ExplosionSePropagaSegunElAlcance)
{
    SistemaCargas sistema;
    Tablero tablero;

    sistema.colocarCarga(
        5,
        5,
        IdJugador::Profesor
    );

    sistema.actualizar(
        2.49f,
        tablero
    );

    sistema.actualizar(
        0.01f,
        tablero
    );

    REQUIRE(
        !sistema.obtenerExplosiones().empty(),
        "Debe existir una explosion"
    );

    const Explosion& explosion =
        sistema.obtenerExplosiones().front();

    CHECK(
        explosion.afecta(3, 5),
        "Debe propagarse dos celdas hacia arriba"
    );

    CHECK(
        explosion.afecta(7, 5),
        "Debe propagarse dos celdas hacia abajo"
    );

    CHECK(
        explosion.afecta(5, 3),
        "Debe propagarse dos celdas hacia la izquierda"
    );

    CHECK(
        explosion.afecta(5, 7),
        "Debe propagarse dos celdas hacia la derecha"
    );

    CHECK(
        !explosion.afecta(2, 5),
        "No debe superar el alcance de dos celdas"
    );
}

TEST(SistemaCargas, ExplosionDestruyeUnPupitre)
{
    SistemaCargas sistema;
    Tablero tablero;

    REQUIRE(
        tablero.obtenerCelda(4, 3) ==
            TipoCelda::Pupitre,
        "La posicion debe comenzar como pupitre"
    );

    sistema.colocarCarga(
        4,
        2,
        IdJugador::Profesor
    );

    sistema.actualizar(
        2.49f,
        tablero
    );

    sistema.actualizar(
        0.01f,
        tablero
    );

    CHECK(
        tablero.obtenerCelda(4, 3) ==
            TipoCelda::Piso,
        "La explosion debe convertir el pupitre en piso"
    );

    REQUIRE(
        !sistema.obtenerExplosiones().empty(),
        "Debe existir una explosion"
    );

    const Explosion& explosion =
        sistema.obtenerExplosiones().front();

    CHECK(
        explosion.afecta(4, 3),
        "La explosion debe alcanzar el pupitre"
    );

    CHECK(
        !explosion.afecta(4, 4),
        "La propagacion debe detenerse al destruir el pupitre"
    );
}

TEST(SistemaCargas, EliminaExplosionCuandoFinaliza)
{
    SistemaCargas sistema;
    Tablero tablero;

    sistema.colocarCarga(
        5,
        5,
        IdJugador::Profesor
    );

    sistema.actualizar(
        2.49f,
        tablero
    );

    sistema.actualizar(
        0.01f,
        tablero
    );

    REQUIRE(
        sistema.obtenerExplosiones().size() == 1,
        "Debe existir una explosion activa"
    );

    sistema.actualizar(
        0.44f,
        tablero
    );

    CHECK(
        sistema.obtenerExplosiones().empty(),
        "La explosion finalizada debe eliminarse"
    );
}

TEST(SistemaCargas, JugadorPuedeColocarOtraCargaLuegoDeLaExplosion)
{
    SistemaCargas sistema;
    Tablero tablero;

    REQUIRE(
        sistema.colocarCarga(
            5,
            5,
            IdJugador::Profesor
        ),
        "La primera carga debe colocarse"
    );

    sistema.actualizar(
        2.49f,
        tablero
    );

    sistema.actualizar(
        0.01f,
        tablero
    );

    CHECK(
        !sistema.jugadorTieneCargaActiva(
            IdJugador::Profesor
        ),
        "El profesor ya no debe tener una carga activa"
    );

    const bool nuevaCarga =
        sistema.colocarCarga(
            6,
            6,
            IdJugador::Profesor
        );

    CHECK(
        nuevaCarga,
        "El jugador debe poder colocar una nueva carga"
    );
}
