
#pragma once

#include "modelo/EstadoCarga.h"
#include "modelo/IdJugador.h"

class CargaCodigo
{
public:
    CargaCodigo(
        int fila,
        int columna,
        IdJugador propietario,
        float tiempoActivacion,
        int alcance
    );

    void actualizar(float deltaTiempo);
    void finalizar();

    int obtenerFila() const;
    int obtenerColumna() const;
    int obtenerAlcance() const;

    IdJugador obtenerPropietario() const;
    EstadoCarga obtenerEstado() const;

    bool debeEjecutarse() const;
    bool estaFinalizada() const;

private:
    int fila;
    int columna;

    IdJugador propietario;

    float tiempoRestante;
    int alcance;

    EstadoCarga estado;
};
