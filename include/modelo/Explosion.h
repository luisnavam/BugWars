#pragma once

#include <vector>

#include "modelo/Posicion.h"

class Explosion
{
public:
    Explosion(
        const std::vector<Posicion>& posicionesAfectadas,
        float duracion
    );

    void actualizar(float deltaTiempo);

    bool estaActiva() const;
    bool afecta(int fila, int columna) const;

    const std::vector<Posicion>&
    obtenerPosicionesAfectadas() const;

private:
    std::vector<Posicion> posicionesAfectadas;
    float tiempoRestante;
};
