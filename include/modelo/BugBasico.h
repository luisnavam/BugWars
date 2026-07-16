#pragma once

#include <random>

#include "modelo/Enemigo.h"

class Jugador;
class SistemaCargas;

class BugBasico : public Enemigo
{
public:
    BugBasico(
        int filaInicial,
        int columnaInicial
    );

    void actualizar(
        float deltaTiempo,
        const Tablero& tablero,
        const Jugador& profesor,
        const Jugador& estudiante
    ) override;

    void actualizarConCargas(
        float deltaTiempo,
        const Tablero& tablero,
        const SistemaCargas& sistemaCargas,
        const Jugador& profesor,
        const Jugador& estudiante
    );

private:
    static constexpr float INTERVALO_MOVIMIENTO = 0.45f;
    static constexpr int DISTANCIA_DETECCION = 4;

    float tiempoHastaMovimiento;
    std::mt19937 generador;

    void decidirMovimiento(
        const Tablero& tablero,
        const SistemaCargas* sistemaCargas,
        const Jugador& profesor,
        const Jugador& estudiante
    );

    void intentarPerseguir(
        const Jugador& objetivo,
        const Tablero& tablero,
        const SistemaCargas* sistemaCargas
    );

    void intentarMoverAleatoriamente(
        const Tablero& tablero,
        const SistemaCargas* sistemaCargas
    );

    bool posicionValida(
        int nuevaFila,
        int nuevaColumna,
        const Tablero& tablero,
        const SistemaCargas* sistemaCargas
    ) const;

    const Jugador* buscarJugadorMasCercano(
        const Jugador& profesor,
        const Jugador& estudiante
    ) const;

    int calcularDistancia(
        const Jugador& jugador
    ) const;
};
