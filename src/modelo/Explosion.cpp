#include "modelo/Explosion.h"

Explosion::Explosion(
    const std::vector<Posicion>& posicionesAfectadas,
    float duracion
)
    : posicionesAfectadas(posicionesAfectadas),
      tiempoRestante(duracion)
{
}

void Explosion::actualizar(float deltaTiempo)
{
    if (deltaTiempo <= 0.0f)
    {
        return;
    }

    tiempoRestante -= deltaTiempo;

    if (tiempoRestante < 0.0f)
    {
        tiempoRestante = 0.0f;
    }
}

bool Explosion::estaActiva() const
{
    return tiempoRestante > 0.0f;
}

bool Explosion::afecta(int fila, int columna) const
{
    for (const Posicion& posicion : posicionesAfectadas)
    {
        if (posicion.obtenerFila() == fila &&
            posicion.obtenerColumna() == columna)
        {
            return true;
        }
    }

    return false;
}

const std::vector<Posicion>&
Explosion::obtenerPosicionesAfectadas() const
{
    return posicionesAfectadas;
}
