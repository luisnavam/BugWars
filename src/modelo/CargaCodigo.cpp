
#include "modelo/CargaCodigo.h"

CargaCodigo::CargaCodigo(
    int fila,
    int columna,
    IdJugador propietario,
    float tiempoActivacion,
    int alcance
)
    : fila(fila),
      columna(columna),
      propietario(propietario),
      tiempoRestante(tiempoActivacion),
      alcance(alcance),
      estado(EstadoCarga::Compilando)
{
}

void CargaCodigo::actualizar(float deltaTiempo)
{
    if (estado != EstadoCarga::Compilando ||
        deltaTiempo <= 0.0f)
    {
        return;
    }

    tiempoRestante -= deltaTiempo;

    if (tiempoRestante <= 0.0f)
    {
        tiempoRestante = 0.0f;
        estado = EstadoCarga::ListaParaEjecutar;
    }
}

void CargaCodigo::finalizar()
{
    estado = EstadoCarga::Finalizada;
}

int CargaCodigo::obtenerFila() const
{
    return fila;
}

int CargaCodigo::obtenerColumna() const
{
    return columna;
}

int CargaCodigo::obtenerAlcance() const
{
    return alcance;
}

IdJugador CargaCodigo::obtenerPropietario() const
{
    return propietario;
}

EstadoCarga CargaCodigo::obtenerEstado() const
{
    return estado;
}

bool CargaCodigo::debeEjecutarse() const
{
    return estado == EstadoCarga::ListaParaEjecutar;
}

bool CargaCodigo::estaFinalizada() const
{
    return estado == EstadoCarga::Finalizada;
}
