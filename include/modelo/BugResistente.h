
#pragma once

#include "modelo/BugBasico.h"

class BugResistente : public BugBasico
{
public:
    BugResistente(
        int filaInicial,
        int columnaInicial
    );

    bool estaDanado() const;
};
