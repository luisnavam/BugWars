#include "modelo/SistemaCargas.h"

#include <algorithm>

#include "modelo/Tablero.h"

SistemaCargas::SistemaCargas()
    : cargas(),
      explosiones()
{
}

bool SistemaCargas::colocarCarga(
    int fila,
    int columna,
    IdJugador propietario
)
{
    if (existeCargaEn(fila, columna) ||
        jugadorTieneCargaActiva(propietario))
    {
        return false;
    }

    cargas.emplace_back(
        fila,
        columna,
        propietario,
        TIEMPO_ACTIVACION,
        ALCANCE
    );

    return true;
}

void SistemaCargas::actualizar(
    float deltaTiempo,
    Tablero& tablero
)
{
    for (CargaCodigo& carga : cargas)
    {
        carga.actualizar(deltaTiempo);

        if (carga.debeEjecutarse())
        {
            const std::vector<Posicion>
                posiciones =
                    calcularPropagacion(
                        carga,
                        tablero
                    );

            explosiones.emplace_back(
                posiciones,
                DURACION_EXPLOSION
            );

            carga.finalizar();
        }
    }

    for (Explosion& explosion : explosiones)
    {
        explosion.actualizar(deltaTiempo);
    }

    eliminarElementosFinalizados();
}

bool SistemaCargas::existeCargaEn(
    int fila,
    int columna
) const
{
    for (const CargaCodigo& carga : cargas)
    {
        if (!carga.estaFinalizada() &&
            carga.obtenerFila() == fila &&
            carga.obtenerColumna() == columna)
        {
            return true;
        }
    }

    return false;
}

bool SistemaCargas::jugadorTieneCargaActiva(
    IdJugador propietario
) const
{
    for (const CargaCodigo& carga : cargas)
    {
        if (!carga.estaFinalizada() &&
            carga.obtenerPropietario() ==
                propietario)
        {
            return true;
        }
    }

    return false;
}

const std::vector<CargaCodigo>&
SistemaCargas::obtenerCargas() const
{
    return cargas;
}

const std::vector<Explosion>&
SistemaCargas::obtenerExplosiones() const
{
    return explosiones;
}

std::vector<Posicion>
SistemaCargas::calcularPropagacion(
    const CargaCodigo& carga,
    Tablero& tablero
)
{
    std::vector<Posicion> posiciones;

    const int filaCentro =
        carga.obtenerFila();

    const int columnaCentro =
        carga.obtenerColumna();

    posiciones.emplace_back(
        filaCentro,
        columnaCentro
    );

    const int direcciones[4][2] =
    {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    for (const auto& direccion : direcciones)
    {
        for (int distancia = 1;
             distancia <= carga.obtenerAlcance();
             ++distancia)
        {
            const int fila =
                filaCentro +
                direccion[0] * distancia;

            const int columna =
                columnaCentro +
                direccion[1] * distancia;

            if (!tablero.estaDentro(
                    fila,
                    columna
                ))
            {
                break;
            }

            if (tablero.bloqueaExplosion(
                    fila,
                    columna
                ))
            {
                break;
            }

            posiciones.emplace_back(
                fila,
                columna
            );

            if (tablero.esDestruible(
                    fila,
                    columna
                ))
            {
                tablero.destruirCelda(
                    fila,
                    columna
                );

                break;
            }
        }
    }

    return posiciones;
}

void SistemaCargas::eliminarElementosFinalizados()
{
    cargas.erase(
        std::remove_if(
            cargas.begin(),
            cargas.end(),
            [](const CargaCodigo& carga)
            {
                return carga.estaFinalizada();
            }
        ),
        cargas.end()
    );

    explosiones.erase(
        std::remove_if(
            explosiones.begin(),
            explosiones.end(),
            [](const Explosion& explosion)
            {
                return !explosion.estaActiva();
            }
        ),
        explosiones.end()
    );
}
