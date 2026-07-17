#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

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
    void renderizarMenu();
    void renderizarPausa(const Partida& partida);

private:
    sf::RenderWindow ventana;

    sf::Font fuente;
    bool fuenteCargada;

    sf::Texture texturaProfesor;
    sf::Texture texturaEstudiante;

    sf::Texture texturaBugBasico;
    sf::Texture texturaBugResistente;
    sf::Texture texturaBugResistenteDanado;

    sf::Texture texturaPiso;
    sf::Texture texturaPared;
    sf::Texture texturaPupitre;
    sf::Texture texturaComputadora;
    sf::Texture texturaPizarron;
    sf::Texture texturaEscritorioProfesor;
    sf::Texture texturaBiblioteca;
    sf::Texture texturaPuerta;
    sf::Texture texturaVentana;

    sf::Texture texturaCarga;
    sf::Texture texturaExplosion;

    bool texturaProfesorCargada;
    bool texturaEstudianteCargada;

    bool texturaBugBasicoCargada;
    bool texturaBugResistenteCargada;
    bool texturaBugResistenteDanadoCargada;

    bool texturaPisoCargada;
    bool texturaParedCargada;
    bool texturaPupitreCargada;
    bool texturaComputadoraCargada;
    bool texturaPizarronCargada;
    bool texturaEscritorioProfesorCargada;
    bool texturaBibliotecaCargada;
    bool texturaPuertaCargada;
    bool texturaVentanaCargada;

    bool texturaCargaCargada;
    bool texturaExplosionCargada;

    int ultimaOleadaMostrada;
    bool mostrarAvisoOleada;
    sf::Clock relojAvisoOleada;

    static constexpr float TAMANO_CELDA = 48.0f;
    static constexpr float ANCHO_TABLERO = 720.0f;
    static constexpr float ANCHO_PANEL = 200.0f;
    static constexpr float ALTO_VENTANA = 624.0f;
    static constexpr float DURACION_AVISO_OLEADA = 1.5f;

    void dibujarEscena(
        const Partida& partida
    );

    void dibujarTablero(
        const Partida& partida
    );

    void dibujarJugador(
        const Jugador& jugador,
        const sf::Color& color,
        const sf::Texture* textura
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
        const std::string& nombre,
        float posicionY,
        const sf::Color& color
    );

    void dibujarCorazon(
        float posicionX,
        float posicionY,
        bool activo
    );

    void dibujarAvisoOleada(
        const Partida& partida
    );

    void dibujarPantallaFinal(
        const Partida& partida
    );

    void dibujarSimboloVictoria();
    void dibujarSimboloDerrota();
    void dibujarIndicadorReinicio();

    void dibujarSpriteEnCelda(
        const sf::Texture& textura,
        int fila,
        int columna,
        float proporcion = 1.0f,
        const sf::Color& color = sf::Color::White
    );

    void dibujarTextoCentrado(
        const std::string& contenido,
        float posicionY,
        unsigned int tamano,
        const sf::Color& color
    );

    void dibujarTextoCentradoEnArea(
        const std::string& contenido,
        float inicioX,
        float anchoArea,
        float posicionY,
        unsigned int tamano,
        const sf::Color& color
    );
};
