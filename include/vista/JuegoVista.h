#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

class CargaCodigo;
class Enemigo;
class Explosion;
class Jugador;
class Partida;

class JuegoVista
{
public:
    JuegoVista();

    bool estaAbierta() const;
    std::optional<sf::Event> obtenerEvento();
    void cerrar();

    void renderizar(const Partida& partida);

private:
    sf::RenderWindow ventana;

    static constexpr float TAMANO_CELDA = 48.0f;
    static constexpr float ANCHO_TABLERO = 720.0f;
    static constexpr float ANCHO_PANEL = 200.0f;
    static constexpr float ALTO_VENTANA = 624.0f;

    void dibujarTablero(
        const Partida& partida
    );

    void dibujarJugador(
        const Jugador& jugador,
        const sf::Color& color
    );

    void dibujarEnemigo(
        const Enemigo& enemigo
    );

    void dibujarCarga(
        const CargaCodigo& carga
    );

    void dibujarExplosion(
        const Explosion& explosion
    );

    void dibujarHUD(
        const Partida& partida
    );

    void dibujarVidas(
        const Jugador& jugador,
        float posicionY,
        const sf::Color& color
    );

    void dibujarPantallaFinal(
        const Partida& partida
    );

    void dibujarSimboloVictoria();
    void dibujarSimboloDerrota();
    void dibujarIndicadorReinicio();
};
