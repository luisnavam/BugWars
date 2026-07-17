
#include <tau/tau.h>

#include "modelo/CargaCodigo.h"

TEST(CargaCodigo, EstadoInicial)
{
    CargaCodigo carga(
        5,
        8,
        IdJugador::Profesor,
        3.0f,
        2
    );

    CHECK(
        carga.obtenerFila() == 5,
        "La fila inicial debe ser correcta"
    );

    CHECK(
        carga.obtenerColumna() == 8,
        "La columna inicial debe ser correcta"
    );

    CHECK(
        carga.obtenerAlcance() == 2,
        "El alcance inicial debe ser correcto"
    );

    CHECK(
        carga.obtenerPropietario() == IdJugador::Profesor,
        "Debe conservar el propietario"
    );

    CHECK(
        carga.obtenerEstado() == EstadoCarga::Compilando,
        "Debe comenzar compilando"
    );

    CHECK(
        !carga.debeEjecutarse(),
        "No debe ejecutarse inmediatamente"
    );

    CHECK(
        !carga.estaFinalizada(),
        "No debe comenzar finalizada"
    );
}

TEST(CargaCodigo, NoActualizaConTiempoInvalido)
{
    CargaCodigo carga(
        1,
        1,
        IdJugador::Profesor,
        2.0f,
        1
    );

    carga.actualizar(0.0f);

    CHECK(
        carga.obtenerEstado() == EstadoCarga::Compilando,
        "Con delta igual a cero no debe cambiar de estado"
    );

    carga.actualizar(-1.0f);

    CHECK(
        carga.obtenerEstado() == EstadoCarga::Compilando,
        "Con delta negativo no debe cambiar de estado"
    );
}

TEST(CargaCodigo, PasaAListaParaEjecutar)
{
    CargaCodigo carga(
        1,
        1,
        IdJugador::Profesor,
        2.0f,
        1
    );

    carga.actualizar(2.5f);

    CHECK(
        carga.obtenerEstado() == EstadoCarga::ListaParaEjecutar,
        "Debe pasar al estado ListaParaEjecutar"
    );

    CHECK(
        carga.debeEjecutarse(),
        "Debe indicar que ya puede ejecutarse"
    );
}

TEST(CargaCodigo, Finalizar)
{
    CargaCodigo carga(
        1,
        1,
        IdJugador::Profesor,
        2.0f,
        1
    );

    carga.finalizar();

    CHECK(
        carga.obtenerEstado() == EstadoCarga::Finalizada,
        "Debe pasar al estado Finalizada"
    );

    CHECK(
        carga.estaFinalizada(),
        "Debe indicar que esta finalizada"
    );
}

TEST(CargaCodigo, NoActualizaLuegoDeFinalizar)
{
    CargaCodigo carga(
        1,
        1,
        IdJugador::Profesor,
        2.0f,
        1
    );

    carga.finalizar();

    carga.actualizar(10.0f);

    CHECK(
        carga.obtenerEstado() == EstadoCarga::Finalizada,
        "Una carga finalizada no debe cambiar de estado"
    );
}
