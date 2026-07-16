
#include "modelo/Enemigo.h"

Enemigo::Enemigo(
    int filaInicial,
    int columnaInicial,
    int puntosVidaIniciales
)
    : fila(filaInicial),
      columna(columnaInicial),
      puntosVida(puntosVidaIniciales),
      activo(true)
{
}

void Enemigo::recibirImpacto()
{
    if (!activo)
    {
        return;
    }

    --puntosVida;

    if (puntosVida <= 0)
    {
        puntosVida = 0;
        activo = false;
    }
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
