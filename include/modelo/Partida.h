#pragma once

#include <memory>
#include <vector>

#include "modelo/CargaCodigo.h"
#include "modelo/Direccion.h"
#include "modelo/Enemigo.h"
#include "modelo/Explosion.h"
#include "modelo/IdJugador.h"
#include "modelo/Jugador.h"
#include "modelo/SistemaCargas.h"
#include "modelo/Tablero.h"

class Partida
{
public:
    Partida();

    void actualizar(float deltaTiempo);

    bool moverJugador(
        IdJugador idJugador,
        Direccion direccion
    );

    bool colocarCarga(IdJugador idJugador);

    const Tablero& obtenerTablero() const;
    const Jugador& obtenerProfesor() const;
    const Jugador& obtenerEstudiante() const;

    const std::vector<CargaCodigo>&
    obtenerCargas() const;

    const std::vector<Explosion>&
    obtenerExplosiones() const;

    const std::vector<std::unique_ptr<Enemigo>>&
    obtenerEnemigos() const;

private:
    float tiempoTranscurrido;

    Tablero tablero;
    Jugador profesor;
    Jugador estudiante;
    SistemaCargas sistemaCargas;

    std::vector<std::unique_ptr<Enemigo>> enemigos;

    Jugador& obtenerJugador(IdJugador idJugador);

    bool posicionOcupadaPorOtroJugador(
        IdJugador idJugador,
        int fila,
        int columna
    ) const;

    void resolverDanioExplosiones();
    void aplicarDanioSiCorresponde(Jugador& jugador);

    void actualizarEnemigos(float deltaTiempo);
};
