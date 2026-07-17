
#include <tau/tau.h>

#include <vector>

#include "modelo/Explosion.h"
#include "modelo/Posicion.h"

TEST(Explosion, EstadoInicial)
{
    std::vector<Posicion> posiciones = {
        Posicion(1, 1),
        Posicion(1, 2),
        Posicion(1, 3)
    };

    Explosion explosion(
        posiciones,
        1.0f
    );

    CHECK(
        explosion.estaActiva(),
        "La explosion debe comenzar activa"
    );

    CHECK(
        explosion.obtenerPosicionesAfectadas().size() == 3,
        "La explosion debe conservar las posiciones recibidas"
    );
}

TEST(Explosion, AfectaPosicionesIncluidas)
{
    std::vector<Posicion> posiciones = {
        Posicion(2, 2),
        Posicion(2, 3),
        Posicion(3, 2)
    };

    Explosion explosion(
        posiciones,
        1.0f
    );

    CHECK(
        explosion.afecta(2, 2),
        "Debe afectar la posicion 2,2"
    );

    CHECK(
        explosion.afecta(2, 3),
        "Debe afectar la posicion 2,3"
    );

    CHECK(
        explosion.afecta(3, 2),
        "Debe afectar la posicion 3,2"
    );
}

TEST(Explosion, NoAfectaPosicionesNoIncluidas)
{
    std::vector<Posicion> posiciones = {
        Posicion(1, 1),
        Posicion(1, 2)
    };

    Explosion explosion(
        posiciones,
        1.0f
    );

    CHECK(
        !explosion.afecta(5, 5),
        "No debe afectar una posicion que no fue incluida"
    );

    CHECK(
        !explosion.afecta(1, 3),
        "No debe afectar una columna diferente"
    );
}

TEST(Explosion, FinalizaAlCumplirseLaDuracion)
{
    std::vector<Posicion> posiciones = {
        Posicion(1, 1)
    };

    Explosion explosion(
        posiciones,
        1.0f
    );

    explosion.actualizar(1.0f);

    CHECK(
        !explosion.estaActiva(),
        "La explosion debe finalizar al cumplirse su duracion"
    );
}

TEST(Explosion, PermaneceActivaAntesDeCumplirseLaDuracion)
{
    std::vector<Posicion> posiciones = {
        Posicion(1, 1)
    };

    Explosion explosion(
        posiciones,
        1.0f
    );

    explosion.actualizar(0.5f);

    CHECK(
        explosion.estaActiva(),
        "La explosion debe seguir activa antes de finalizar"
    );
}

TEST(Explosion, NoActualizaConTiempoInvalido)
{
    std::vector<Posicion> posiciones = {
        Posicion(1, 1)
    };

    Explosion explosion(
        posiciones,
        1.0f
    );

    explosion.actualizar(0.0f);

    CHECK(
        explosion.estaActiva(),
        "Con delta igual a cero debe seguir activa"
    );

    explosion.actualizar(-1.0f);

    CHECK(
        explosion.estaActiva(),
        "Con delta negativo debe seguir activa"
    );
}
