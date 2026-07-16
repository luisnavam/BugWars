#include "modelo/Jugador.h"

Jugador::Jugador(
    const std::string& nombre,
    int filaInicial,
    int columnaInicial
)
    : nombre(nombre),
      fila(filaInicial),
      columna(columnaInicial),
      vidas(VIDAS_INICIALES),
      activo(true),
      invulnerabilidadRestante(0.0f)
{
}

const std::string& Jugador::obtenerNombre() const
{
    return nombre;
}

int Jugador::obtenerFila() const
{
    return fila;
}

int Jugador::obtenerColumna() const
{
    return columna;
}

int Jugador::obtenerVidas() const
{
    return vidas;
}

bool Jugador::estaActivo() const
{
    return activo;
}

bool Jugador::esInvulnerable() const
{
    return invulnerabilidadRestante > 0.0f;
}

void Jugador::cambiarPosicion(
    int nuevaFila,
    int nuevaColumna
)
{
    if (!activo)
    {
        return;
    }

    fila = nuevaFila;
    columna = nuevaColumna;
}

void Jugador::recibirDanio()
{
    if (!activo || esInvulnerable())
    {
        return;
    }

    --vidas;

    if (vidas <= 0)
    {
        vidas = 0;
        activo = false;
        return;
    }

    invulnerabilidadRestante =
        DURACION_INVULNERABILIDAD;
}

void Jugador::actualizar(float deltaTiempo)
{
    if (deltaTiempo <= 0.0f)
    {
        return;
    }

    if (invulnerabilidadRestante > 0.0f)
    {
        invulnerabilidadRestante -= deltaTiempo;

        if (invulnerabilidadRestante < 0.0f)
        {
            invulnerabilidadRestante = 0.0f;
        }
    }
}
