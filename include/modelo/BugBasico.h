
#pragma once

#include "modelo/Enemigo.h"

class BugBasico : public Enemigo
{
public:
    BugBasico(
        int filaInicial,
        int columnaInicial
    );

    void actualizar(float deltaTiempo) override;
};
