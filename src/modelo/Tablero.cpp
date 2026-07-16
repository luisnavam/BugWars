#include "modelo/Tablero.h"

Tablero::Tablero()
{
    inicializar();
}

TipoCelda Tablero::obtenerCelda(
    int fila,
    int columna
) const
{
    return celdas.at(fila).at(columna);
}

bool Tablero::estaDentro(
    int fila,
    int columna
) const
{
    return fila >= 0 &&
           fila < FILAS &&
           columna >= 0 &&
           columna < COLUMNAS;
}

bool Tablero::esTransitable(
    int fila,
    int columna
) const
{
    if (!estaDentro(fila, columna))
    {
        return false;
    }

    const TipoCelda tipo =
        obtenerCelda(fila, columna);

    return tipo == TipoCelda::Piso ||
           tipo == TipoCelda::Puerta ||
           tipo == TipoCelda::SpawnProfesor ||
           tipo == TipoCelda::SpawnEstudiante;
}

bool Tablero::bloqueaExplosion(
    int fila,
    int columna
) const
{
    if (!estaDentro(fila, columna))
    {
        return true;
    }

    const TipoCelda tipo =
        obtenerCelda(fila, columna);

    return tipo == TipoCelda::Pared ||
           tipo == TipoCelda::Pizarron ||
           tipo == TipoCelda::EscritorioProfesor ||
           tipo == TipoCelda::Biblioteca ||
           tipo == TipoCelda::Ventana;
}

bool Tablero::esDestruible(
    int fila,
    int columna
) const
{
    if (!estaDentro(fila, columna))
    {
        return false;
    }

    const TipoCelda tipo =
        obtenerCelda(fila, columna);

    return tipo == TipoCelda::Pupitre ||
           tipo == TipoCelda::Computadora;
}

bool Tablero::destruirCelda(
    int fila,
    int columna
)
{
    if (!esDestruible(fila, columna))
    {
        return false;
    }

    celdas[fila][columna] = TipoCelda::Piso;
    return true;
}

void Tablero::inicializar()
{
    for (auto& fila : celdas)
    {
        fila.fill(TipoCelda::Piso);
    }

    for (int columna = 0;
         columna < COLUMNAS;
         ++columna)
    {
        celdas[0][columna] = TipoCelda::Pared;
        celdas[FILAS - 1][columna] =
            TipoCelda::Pared;
    }

    for (int fila = 0;
         fila < FILAS;
         ++fila)
    {
        celdas[fila][0] = TipoCelda::Pared;
        celdas[fila][COLUMNAS - 1] =
            TipoCelda::Pared;
    }

    for (int columna = 5;
         columna <= 9;
         ++columna)
    {
        celdas[0][columna] =
            TipoCelda::Pizarron;
    }

    celdas[3][0] = TipoCelda::Ventana;
    celdas[4][0] = TipoCelda::Ventana;
    celdas[8][0] = TipoCelda::Ventana;
    celdas[9][0] = TipoCelda::Ventana;

    celdas[6][COLUMNAS - 1] =
        TipoCelda::Puerta;

    celdas[2][6] =
        TipoCelda::EscritorioProfesor;
    celdas[2][7] =
        TipoCelda::EscritorioProfesor;
    celdas[2][8] =
        TipoCelda::EscritorioProfesor;

    celdas[1][11] = TipoCelda::Biblioteca;
    celdas[1][12] = TipoCelda::Biblioteca;
    celdas[10][1] = TipoCelda::Biblioteca;
    celdas[10][2] = TipoCelda::Biblioteca;

    const int filasPuestos[] = {4, 7, 10};
    const int columnasPuestos[] = {3, 6, 9, 12};

    for (const int fila : filasPuestos)
    {
        for (const int columna :
             columnasPuestos)
        {
            celdas[fila][columna] =
                TipoCelda::Pupitre;

            celdas[fila - 1][columna] =
                TipoCelda::Computadora;
        }
    }

    celdas[1][1] =
        TipoCelda::SpawnProfesor;
    celdas[1][2] = TipoCelda::Piso;
    celdas[2][1] = TipoCelda::Piso;

    celdas[FILAS - 2][COLUMNAS - 2] =
        TipoCelda::SpawnEstudiante;

    celdas[FILAS - 2][COLUMNAS - 3] =
        TipoCelda::Piso;

    celdas[FILAS - 3][COLUMNAS - 2] =
        TipoCelda::Piso;
}
