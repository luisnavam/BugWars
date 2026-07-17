
#include <tau/tau.h>

#include <stdexcept>

#include "modelo/Tablero.h"
#include "modelo/TipoCelda.h"

TEST(Tablero, DimensionesCorrectas)
{
    CHECK(
        Tablero::FILAS == 13,
        "El tablero debe tener 13 filas"
    );

    CHECK(
        Tablero::COLUMNAS == 15,
        "El tablero debe tener 15 columnas"
    );
}

TEST(Tablero, ValidaPosicionesDentroDelTablero)
{
    Tablero tablero;

    CHECK(
        tablero.estaDentro(0, 0),
        "La posicion 0,0 debe estar dentro"
    );

    CHECK(
        tablero.estaDentro(12, 14),
        "La ultima posicion debe estar dentro"
    );

    CHECK(
        !tablero.estaDentro(-1, 0),
        "Una fila negativa debe estar fuera"
    );

    CHECK(
        !tablero.estaDentro(0, -1),
        "Una columna negativa debe estar fuera"
    );

    CHECK(
        !tablero.estaDentro(13, 0),
        "Una fila mayor al limite debe estar fuera"
    );

    CHECK(
        !tablero.estaDentro(0, 15),
        "Una columna mayor al limite debe estar fuera"
    );
}

TEST(Tablero, BordesSonParedes)
{
    Tablero tablero;

    CHECK(
        tablero.obtenerCelda(0, 0) == TipoCelda::Pared,
        "La esquina superior izquierda debe ser pared"
    );

    CHECK(
        tablero.obtenerCelda(12, 14) == TipoCelda::Pared,
        "La esquina inferior derecha debe ser pared"
    );

    CHECK(
        tablero.obtenerCelda(5, 0) == TipoCelda::Pared,
        "El borde izquierdo debe ser pared"
    );

    CHECK(
        tablero.obtenerCelda(5, 14) == TipoCelda::Pared,
        "El borde derecho debe ser pared"
    );
}

TEST(Tablero, InicializaElementosEspeciales)
{
    Tablero tablero;

    CHECK(
        tablero.obtenerCelda(0, 5) == TipoCelda::Pizarron,
        "Debe existir el pizarron en la fila superior"
    );

    CHECK(
        tablero.obtenerCelda(3, 0) == TipoCelda::Ventana,
        "Debe existir una ventana en la posicion esperada"
    );

    CHECK(
        tablero.obtenerCelda(6, 14) == TipoCelda::Puerta,
        "Debe existir una puerta en el borde derecho"
    );

    CHECK(
        tablero.obtenerCelda(2, 7) ==
            TipoCelda::EscritorioProfesor,
        "Debe existir el escritorio del profesor"
    );

    CHECK(
        tablero.obtenerCelda(1, 11) ==
            TipoCelda::Biblioteca,
        "Debe existir una biblioteca"
    );
}

TEST(Tablero, InicializaPosicionesDeJugadores)
{
    Tablero tablero;

    CHECK(
        tablero.obtenerCelda(1, 1) ==
            TipoCelda::SpawnProfesor,
        "Debe existir el spawn del profesor"
    );

    CHECK(
        tablero.obtenerCelda(11, 13) ==
            TipoCelda::SpawnEstudiante,
        "Debe existir el spawn del estudiante"
    );

    CHECK(
        tablero.esTransitable(1, 1),
        "El spawn del profesor debe ser transitable"
    );

    CHECK(
        tablero.esTransitable(11, 13),
        "El spawn del estudiante debe ser transitable"
    );
}

TEST(Tablero, IdentificaCeldasTransitables)
{
    Tablero tablero;

    CHECK(
        tablero.esTransitable(1, 2),
        "Una celda de piso debe ser transitable"
    );

    CHECK(
        tablero.esTransitable(6, 14),
        "La puerta debe ser transitable"
    );

    CHECK(
        !tablero.esTransitable(0, 0),
        "Una pared no debe ser transitable"
    );

    CHECK(
        !tablero.esTransitable(4, 3),
        "Un pupitre no debe ser transitable"
    );

    CHECK(
        !tablero.esTransitable(-1, 0),
        "Una posicion fuera del tablero no debe ser transitable"
    );
}

TEST(Tablero, IdentificaBloqueosDeExplosion)
{
    Tablero tablero;

    CHECK(
        tablero.bloqueaExplosion(0, 0),
        "Una pared debe bloquear la explosion"
    );

    CHECK(
        tablero.bloqueaExplosion(0, 5),
        "El pizarron debe bloquear la explosion"
    );

    CHECK(
        tablero.bloqueaExplosion(2, 7),
        "El escritorio debe bloquear la explosion"
    );

    CHECK(
        tablero.bloqueaExplosion(1, 11),
        "La biblioteca debe bloquear la explosion"
    );

    CHECK(
        tablero.bloqueaExplosion(3, 0),
        "La ventana debe bloquear la explosion"
    );

    CHECK(
        !tablero.bloqueaExplosion(1, 2),
        "El piso no debe bloquear la explosion"
    );

    CHECK(
        tablero.bloqueaExplosion(-1, 0),
        "Una posicion fuera del tablero debe bloquear la explosion"
    );
}

TEST(Tablero, IdentificaCeldasDestruibles)
{
    Tablero tablero;

    CHECK(
        tablero.esDestruible(4, 3),
        "Un pupitre debe ser destruible"
    );

    CHECK(
        tablero.esDestruible(3, 3),
        "Una computadora debe ser destruible"
    );

    CHECK(
        !tablero.esDestruible(0, 0),
        "Una pared no debe ser destruible"
    );

    CHECK(
        !tablero.esDestruible(1, 2),
        "El piso no debe ser destruible"
    );

    CHECK(
        !tablero.esDestruible(-1, 0),
        "Una posicion fuera del tablero no debe ser destruible"
    );
}

TEST(Tablero, DestruyePupitre)
{
    Tablero tablero;

    REQUIRE(
        tablero.obtenerCelda(4, 3) ==
            TipoCelda::Pupitre,
        "La celda debe comenzar siendo un pupitre"
    );

    const bool destruida =
        tablero.destruirCelda(4, 3);

    CHECK(
        destruida,
        "La destruccion del pupitre debe devolver true"
    );

    CHECK(
        tablero.obtenerCelda(4, 3) ==
            TipoCelda::Piso,
        "El pupitre destruido debe convertirse en piso"
    );

    CHECK(
        tablero.esTransitable(4, 3),
        "La celda destruida debe quedar transitable"
    );
}

TEST(Tablero, DestruyeComputadora)
{
    Tablero tablero;

    REQUIRE(
        tablero.obtenerCelda(3, 3) ==
            TipoCelda::Computadora,
        "La celda debe comenzar siendo una computadora"
    );

    const bool destruida =
        tablero.destruirCelda(3, 3);

    CHECK(
        destruida,
        "La destruccion de la computadora debe devolver true"
    );

    CHECK(
        tablero.obtenerCelda(3, 3) ==
            TipoCelda::Piso,
        "La computadora destruida debe convertirse en piso"
    );
}

TEST(Tablero, NoDestruyeCeldaIndestructible)
{
    Tablero tablero;

    const bool destruida =
        tablero.destruirCelda(0, 0);

    CHECK(
        !destruida,
        "Una pared no debe poder destruirse"
    );

    CHECK(
        tablero.obtenerCelda(0, 0) ==
            TipoCelda::Pared,
        "La pared debe conservar su tipo"
    );
}

TEST(Tablero, NoDestruyePosicionFueraDelTablero)
{
    Tablero tablero;

    CHECK(
        !tablero.destruirCelda(-1, 0),
        "No debe destruir una posicion negativa"
    );

    CHECK(
        !tablero.destruirCelda(13, 15),
        "No debe destruir una posicion fuera del limite"
    );
}
