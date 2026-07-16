
#pragma once

class Posicion
{
public:
    Posicion(int fila, int columna)
        : fila(fila),
          columna(columna)
    {
    }

    int obtenerFila() const
    {
        return fila;
    }

    int obtenerColumna() const
    {
        return columna;
    }

private:
    int fila;
    int columna;
};
