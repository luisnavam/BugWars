#pragma once

#include "modelo/Partida.h"
#include "vista/JuegoVista.h"

class JuegoControlador
{
public:
    JuegoControlador();

    void ejecutar();

private:
    enum class EstadoAplicacion
    {
        Menu,
        Jugando,
        Pausado
    };

    Partida partida;
    JuegoVista vista;
    EstadoAplicacion estadoAplicacion;

    void procesarEventos();
    void iniciarPartida(ModoJuego modo);
};
