#include "modelo/Enemigo.h"

Enemigo::Enemigo(
    int filaInicial,
    int columnaInicial,
    int puntosVidaIniciales
)
    : fila(filaInicial),
      columna(columnaInicial),
      puntosVida(puntosVidaIniciales),
      activo(true),
      invulnerabilidadRestante(0.0f)
{
}

void Enemigo::actualizarEstado(float deltaTiempo)
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

void Enemigo::recibirImpacto()
{
    if (!activo || esInvulnerable())
    {
        return;
    }

    --puntosVida;

    if (puntosVida <= 0)
    {
        puntosVida = 0;
        activo = false;
        return;
    }

    invulnerabilidadRestante =
        DURACION_INVULNERABILIDAD;
}

int Enemigo::obtenerFila() const
{
    return fila;
}

int Enemigo::obtenerColumna() const
{
    return columna;
}

int Enemigo::obtenerPuntosVida() const
{
    return puntosVida;
}

bool Enemigo::estaActivo() const
{
    return activo;
}

bool Enemigo::esInvulnerable() const
{
    return invulnerabilidadRestante > 0.0f;
}

void Enemigo::cambiarPosicion(
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
