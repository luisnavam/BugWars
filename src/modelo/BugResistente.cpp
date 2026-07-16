#include "modelo/BugResistente.h"

BugResistente::BugResistente(
    int filaInicial,
    int columnaInicial
)
    : BugBasico(
          filaInicial,
          columnaInicial
      )
{
    puntosVida = 2;
}

bool BugResistente::estaDanado() const
{
    return estaActivo() &&
           obtenerPuntosVida() == 1;
}
