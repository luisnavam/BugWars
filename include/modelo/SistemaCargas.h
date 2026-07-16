#pragma once

#include <vector>

#include "modelo/CargaCodigo.h"
#include "modelo/Explosion.h"
#include "modelo/IdJugador.h"

class Tablero;

class SistemaCargas
{
public:
    SistemaCargas();

    bool colocarCarga(
        int fila,
        int columna,
        IdJugador propietario
    );

    void actualizar(
        float deltaTiempo,
        Tablero& tablero
    );

    bool existeCargaEn(
        int fila,
        int columna
    ) const;

    bool jugadorTieneCargaActiva(
        IdJugador propietario
    ) const;

    const std::vector<CargaCodigo>&
    obtenerCargas() const;

    const std::vector<Explosion>&
    obtenerExplosiones() const;

private:
    static constexpr float TIEMPO_ACTIVACION =
        2.5f;

    static constexpr float DURACION_EXPLOSION =
        0.45f;

    static constexpr int ALCANCE = 2;

    std::vector<CargaCodigo> cargas;
    std::vector<Explosion> explosiones;

    std::vector<Posicion> calcularPropagacion(
        const CargaCodigo& carga,
        Tablero& tablero
    );

    void eliminarElementosFinalizados();
};
