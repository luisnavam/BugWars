#pragma once

#include <array>

#include "modelo/TipoCelda.h"

class Tablero
{
public:
    static constexpr int FILAS = 13;
    static constexpr int COLUMNAS = 15;

    Tablero();

    TipoCelda obtenerCelda(int fila, int columna) const;

    bool estaDentro(int fila, int columna) const;
    bool esTransitable(int fila, int columna) const;

    bool bloqueaExplosion(int fila, int columna) const;
    bool esDestruible(int fila, int columna) const;

    bool destruirCelda(int fila, int columna);

private:
    std::array<
        std::array<TipoCelda, COLUMNAS>,
        FILAS
    > celdas;

    void inicializar();
};
