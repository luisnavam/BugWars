
#pragma once

#include "modelo/Partida.h"
#include "vista/JuegoVista.h"

class JuegoControlador
{
public:
    JuegoControlador();

    void ejecutar();

private:
    Partida partida;
    JuegoVista vista;

    void procesarEventos();
};
