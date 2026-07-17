#pragma once

#include <memory>
#include <vector>

#include "modelo/CargaCodigo.h"
#include "modelo/Direccion.h"
#include "modelo/Enemigo.h"
#include "modelo/EstadoPartida.h"
#include "modelo/Explosion.h"
#include "modelo/IdJugador.h"
#include "modelo/Jugador.h"
#include "modelo/ModoJuego.h"
#include "modelo/SistemaCargas.h"
#include "modelo/Tablero.h"

class Partida
{
public:
    static constexpr int TOTAL_OLEADAS = 4;

    Partida();

    void iniciar(ModoJuego nuevoModo);
    void actualizar(float deltaTiempo);
    void reiniciar();

    bool moverJugador(
        IdJugador idJugador,
        Direccion direccion
    );

    bool colocarCarga(IdJugador idJugador);

    EstadoPartida obtenerEstado() const;
    ModoJuego obtenerModo() const;
    bool esModoDuo() const;

    int obtenerOleadaActual() const;
    int obtenerBugsRestantes() const;
    bool estaEnTransicionOleada() const;
    float obtenerTiempoTransicionOleada() const;

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
    static constexpr float DURACION_TRANSICION_OLEADA = 1.5f;

    float tiempoTranscurrido;
    EstadoPartida estado;
    ModoJuego modo;

    int oleadaActual;
    bool enTransicionOleada;
    float tiempoTransicionOleada;

    Tablero tablero;
    Jugador profesor;
    Jugador estudiante;
    SistemaCargas sistemaCargas;

    std::vector<std::unique_ptr<Enemigo>> enemigos;

    void crearOleadaActual();
    void crearEnemigosDeOleada(
        int cantidadBasicos,
        int cantidadResistentes
    );

    bool agregarBugBasicoEnPosicionLibre();
    bool agregarBugResistenteEnPosicionLibre();

    bool posicionDisponibleParaEnemigo(
        int fila,
        int columna
    ) const;

    bool hayEnemigosActivos() const;
    void comprobarEstadoFinal();
    void iniciarSiguienteOleada();

    Jugador& obtenerJugador(IdJugador idJugador);

    bool jugadorHabilitado(
        IdJugador idJugador
    ) const;

    bool posicionOcupadaPorOtroJugador(
        IdJugador idJugador,
        int fila,
        int columna
    ) const;

    void resolverDanioExplosiones();

    void aplicarDanioSiCorresponde(
        Jugador& jugador
    );

    void actualizarEnemigos(
        float deltaTiempo
    );

    void resolverDanioPorContacto();

    void aplicarDanioPorContacto(
        Jugador& jugador
    );
};
