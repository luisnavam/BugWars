#include "modelo/BugBasico.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <random>

#include "modelo/Jugador.h"
#include "modelo/SistemaCargas.h"
#include "modelo/Tablero.h"

BugBasico::BugBasico(
    int filaInicial,
    int columnaInicial
)
    : Enemigo(
          filaInicial,
          columnaInicial,
          1
      ),
      tiempoHastaMovimiento(INTERVALO_MOVIMIENTO),
      generador(std::random_device{}())
{
}

void BugBasico::actualizar(
    float deltaTiempo,
    const Tablero& tablero,
    const Jugador& profesor,
    const Jugador& estudiante
)
{
    if (!activo || deltaTiempo <= 0.0f)
    {
        return;
    }

    tiempoHastaMovimiento -= deltaTiempo;

    if (tiempoHastaMovimiento > 0.0f)
    {
        return;
    }

    decidirMovimiento(
        tablero,
        nullptr,
        profesor,
        estudiante
    );

    tiempoHastaMovimiento =
        INTERVALO_MOVIMIENTO;
}

void BugBasico::actualizarConCargas(
    float deltaTiempo,
    const Tablero& tablero,
    const SistemaCargas& sistemaCargas,
    const Jugador& profesor,
    const Jugador& estudiante
)
{
    if (!activo || deltaTiempo <= 0.0f)
    {
        return;
    }

    tiempoHastaMovimiento -= deltaTiempo;

    if (tiempoHastaMovimiento > 0.0f)
    {
        return;
    }

    decidirMovimiento(
        tablero,
        &sistemaCargas,
        profesor,
        estudiante
    );

    tiempoHastaMovimiento =
        INTERVALO_MOVIMIENTO;
}

void BugBasico::decidirMovimiento(
    const Tablero& tablero,
    const SistemaCargas* sistemaCargas,
    const Jugador& profesor,
    const Jugador& estudiante
)
{
    const Jugador* objetivo =
        buscarJugadorMasCercano(
            profesor,
            estudiante
        );

    if (objetivo != nullptr &&
        calcularDistancia(*objetivo) <=
            DISTANCIA_DETECCION)
    {
        intentarPerseguir(
            *objetivo,
            tablero,
            sistemaCargas
        );

        return;
    }

    intentarMoverAleatoriamente(
        tablero,
        sistemaCargas
    );
}

void BugBasico::intentarPerseguir(
    const Jugador& objetivo,
    const Tablero& tablero,
    const SistemaCargas* sistemaCargas
)
{
    struct Desplazamiento
    {
        int fila;
        int columna;
    };

    std::array<Desplazamiento, 4> direcciones =
    {{
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    }};

    std::sort(
        direcciones.begin(),
        direcciones.end(),
        [this, &objetivo](
            const Desplazamiento& a,
            const Desplazamiento& b
        )
        {
            const int distanciaA =
                std::abs(
                    fila + a.fila -
                    objetivo.obtenerFila()
                ) +
                std::abs(
                    columna + a.columna -
                    objetivo.obtenerColumna()
                );

            const int distanciaB =
                std::abs(
                    fila + b.fila -
                    objetivo.obtenerFila()
                ) +
                std::abs(
                    columna + b.columna -
                    objetivo.obtenerColumna()
                );

            return distanciaA < distanciaB;
        }
    );

    for (const Desplazamiento& direccion :
         direcciones)
    {
        const int nuevaFila =
            fila + direccion.fila;

        const int nuevaColumna =
            columna + direccion.columna;

        if (posicionValida(
                nuevaFila,
                nuevaColumna,
                tablero,
                sistemaCargas
            ))
        {
            cambiarPosicion(
                nuevaFila,
                nuevaColumna
            );

            return;
        }
    }
}

void BugBasico::intentarMoverAleatoriamente(
    const Tablero& tablero,
    const SistemaCargas* sistemaCargas
)
{
    struct Desplazamiento
    {
        int fila;
        int columna;
    };

    std::array<Desplazamiento, 4> direcciones =
    {{
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    }};

    std::shuffle(
        direcciones.begin(),
        direcciones.end(),
        generador
    );

    for (const Desplazamiento& direccion :
         direcciones)
    {
        const int nuevaFila =
            fila + direccion.fila;

        const int nuevaColumna =
            columna + direccion.columna;

        if (posicionValida(
                nuevaFila,
                nuevaColumna,
                tablero,
                sistemaCargas
            ))
        {
            cambiarPosicion(
                nuevaFila,
                nuevaColumna
            );

            return;
        }
    }
}

bool BugBasico::posicionValida(
    int nuevaFila,
    int nuevaColumna,
    const Tablero& tablero,
    const SistemaCargas* sistemaCargas
) const
{
    if (!tablero.esTransitable(
            nuevaFila,
            nuevaColumna
        ))
    {
        return false;
    }

    if (sistemaCargas != nullptr &&
        sistemaCargas->existeCargaEn(
            nuevaFila,
            nuevaColumna
        ))
    {
        return false;
    }

    return true;
}

const Jugador* BugBasico::buscarJugadorMasCercano(
    const Jugador& profesor,
    const Jugador& estudiante
) const
{
    if (!profesor.estaActivo() &&
        !estudiante.estaActivo())
    {
        return nullptr;
    }

    if (!profesor.estaActivo())
    {
        return &estudiante;
    }

    if (!estudiante.estaActivo())
    {
        return &profesor;
    }

    const int distanciaProfesor =
        calcularDistancia(profesor);

    const int distanciaEstudiante =
        calcularDistancia(estudiante);

    if (distanciaProfesor <=
        distanciaEstudiante)
    {
        return &profesor;
    }

    return &estudiante;
}

int BugBasico::calcularDistancia(
    const Jugador& jugador
) const
{
    return std::abs(
               fila - jugador.obtenerFila()
           ) +
           std::abs(
               columna - jugador.obtenerColumna()
           );
}
