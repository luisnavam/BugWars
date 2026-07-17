
#include <tau/tau.h>

#include "modelo/Jugador.h"

TEST(Jugador, EstadoInicial)
{
    Jugador jugador(
        "Profesor",
        1,
        2
    );

    CHECK(
        jugador.obtenerNombre() == "Profesor",
        "El jugador debe conservar el nombre recibido"
    );

    CHECK(
        jugador.obtenerFila() == 1,
        "La fila inicial debe ser 1"
    );

    CHECK(
        jugador.obtenerColumna() == 2,
        "La columna inicial debe ser 2"
    );

    CHECK(
        jugador.obtenerVidas() == 3,
        "El jugador debe comenzar con 3 vidas"
    );

    CHECK(
        jugador.estaActivo(),
        "El jugador debe comenzar activo"
    );

    CHECK(
        !jugador.esInvulnerable(),
        "El jugador no debe comenzar invulnerable"
    );
}

TEST(Jugador, CambioDePosicion)
{
    Jugador jugador(
        "Estudiante",
        3,
        4
    );

    jugador.cambiarPosicion(
        5,
        6
    );

    CHECK(
        jugador.obtenerFila() == 5,
        "El jugador debe cambiar a la fila 5"
    );

    CHECK(
        jugador.obtenerColumna() == 6,
        "El jugador debe cambiar a la columna 6"
    );
}

TEST(Jugador, RecibeDanio)
{
    Jugador jugador(
        "Profesor",
        1,
        1
    );

    jugador.recibirDanio();

    CHECK(
        jugador.obtenerVidas() == 2,
        "El jugador debe perder una vida"
    );

    CHECK(
        jugador.esInvulnerable(),
        "El jugador debe quedar invulnerable"
    );
}

TEST(Jugador, NoRecibeDanioMientrasEsInvulnerable)
{
    Jugador jugador(
        "Profesor",
        1,
        1
    );

    jugador.recibirDanio();

    REQUIRE(
        jugador.obtenerVidas() == 2,
        "El primer danio debe reducir las vidas a 2"
    );

    jugador.recibirDanio();

    CHECK(
        jugador.obtenerVidas() == 2,
        "No debe perder otra vida durante la invulnerabilidad"
    );
}

TEST(Jugador, FinalizaInvulnerabilidad)
{
    Jugador jugador(
        "Profesor",
        1,
        1
    );

    jugador.recibirDanio();

    REQUIRE(
        jugador.esInvulnerable(),
        "Debe comenzar el periodo de invulnerabilidad"
    );

    jugador.actualizar(1.6f);

    CHECK(
        !jugador.esInvulnerable(),
        "La invulnerabilidad debe finalizar"
    );
}

TEST(Jugador, QuedaInactivoSinVidas)
{
    Jugador jugador(
        "Profesor",
        1,
        1
    );

    jugador.recibirDanio();
    jugador.actualizar(1.6f);

    jugador.recibirDanio();
    jugador.actualizar(1.6f);

    jugador.recibirDanio();

    CHECK(
        jugador.obtenerVidas() == 0,
        "El jugador debe quedar con cero vidas"
    );

    CHECK(
        !jugador.estaActivo(),
        "El jugador debe quedar inactivo"
    );
}
