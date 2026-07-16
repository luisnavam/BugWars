
#include "modelo/BugBasico.h"

BugBasico::BugBasico(
    int filaInicial,
    int columnaInicial
)
    : Enemigo(
          filaInicial,
          columnaInicial,
          1
      )
{
}

void BugBasico::actualizar(float deltaTiempo)
{
    // El movimiento se implementará en el siguiente paso.
    (void)deltaTiempo;
}
