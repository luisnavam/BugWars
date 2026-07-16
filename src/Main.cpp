#include "controlador/JuegoControlador.h"

#include <cstdlib>
#include <exception>
#include <iostream>

int main()
{
    try
    {
        JuegoControlador controlador;
        controlador.ejecutar();

        return EXIT_SUCCESS;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Error inesperado: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
