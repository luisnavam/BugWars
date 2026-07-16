#include "vista/JuegoVista.h"

#include "modelo/CargaCodigo.h"
#include "modelo/Enemigo.h"
#include "modelo/Explosion.h"
#include "modelo/Jugador.h"
#include "modelo/Partida.h"
#include "modelo/Posicion.h"
#include "modelo/Tablero.h"
#include "modelo/TipoCelda.h"

JuegoVista::JuegoVista()
    : ventana(
          sf::VideoMode({920u, 624u}),
          "Bug Wars - Laboratorio de Programacion"
      )
{
    ventana.setFramerateLimit(60);
}

bool JuegoVista::estaAbierta() const
{
    return ventana.isOpen();
}

std::optional<sf::Event> JuegoVista::obtenerEvento()
{
    return ventana.pollEvent();
}

void JuegoVista::cerrar()
{
    ventana.close();
}

void JuegoVista::renderizar(const Partida& partida)
{
    ventana.clear(sf::Color(24, 27, 32));

    dibujarTablero(partida);

    for (const CargaCodigo& carga : partida.obtenerCargas())
    {
        dibujarCarga(carga);
    }

    for (const Explosion& explosion : partida.obtenerExplosiones())
    {
        dibujarExplosion(explosion);
    }

    for (const auto& enemigo : partida.obtenerEnemigos())
    {
        if (enemigo->estaActivo())
        {
            dibujarEnemigo(*enemigo);
        }
    }

    if (partida.obtenerProfesor().estaActivo())
    {
        dibujarJugador(
            partida.obtenerProfesor(),
            sf::Color(70, 130, 180)
        );
    }

    if (partida.obtenerEstudiante().estaActivo())
    {
        dibujarJugador(
            partida.obtenerEstudiante(),
            sf::Color(70, 180, 110)
        );
    }

    dibujarHUD(partida);

    ventana.display();
}

void JuegoVista::dibujarTablero(const Partida& partida)
{
    const Tablero& tablero = partida.obtenerTablero();

    for (int fila = 0; fila < Tablero::FILAS; ++fila)
    {
        for (int columna = 0;
             columna < Tablero::COLUMNAS;
             ++columna)
        {
            const TipoCelda tipo =
                tablero.obtenerCelda(fila, columna);

            sf::RectangleShape celda({
                TAMANO_CELDA - 1.0f,
                TAMANO_CELDA - 1.0f
            });

            celda.setPosition({
                columna * TAMANO_CELDA,
                fila * TAMANO_CELDA
            });

            switch (tipo)
            {
                case TipoCelda::Piso:
                case TipoCelda::SpawnProfesor:
                case TipoCelda::SpawnEstudiante:
                    celda.setFillColor(
                        sf::Color(202, 187, 154)
                    );
                    break;

                case TipoCelda::Pared:
                    celda.setFillColor(
                        sf::Color(64, 70, 78)
                    );
                    break;

                case TipoCelda::Pupitre:
                    celda.setFillColor(
                        sf::Color(133, 88, 52)
                    );
                    break;

                case TipoCelda::Computadora:
                    celda.setFillColor(
                        sf::Color(55, 74, 86)
                    );
                    break;

                case TipoCelda::Pizarron:
                    celda.setFillColor(
                        sf::Color(34, 82, 65)
                    );
                    break;

                case TipoCelda::EscritorioProfesor:
                    celda.setFillColor(
                        sf::Color(112, 68, 42)
                    );
                    break;

                case TipoCelda::Biblioteca:
                    celda.setFillColor(
                        sf::Color(91, 55, 36)
                    );
                    break;

                case TipoCelda::Puerta:
                    celda.setFillColor(
                        sf::Color(169, 117, 65)
                    );
                    break;

                case TipoCelda::Ventana:
                    celda.setFillColor(
                        sf::Color(100, 174, 210)
                    );
                    break;
            }

            ventana.draw(celda);

            if (tipo == TipoCelda::Computadora)
            {
                sf::RectangleShape pantalla({
                    TAMANO_CELDA * 0.55f,
                    TAMANO_CELDA * 0.40f
                });

                pantalla.setFillColor(
                    sf::Color(76, 190, 210)
                );

                pantalla.setPosition({
                    columna * TAMANO_CELDA +
                        TAMANO_CELDA * 0.22f,

                    fila * TAMANO_CELDA +
                        TAMANO_CELDA * 0.20f
                });

                ventana.draw(pantalla);
            }
        }
    }
}

void JuegoVista::dibujarJugador(
    const Jugador& jugador,
    const sf::Color& color
)
{
    sf::CircleShape figura(TAMANO_CELDA * 0.30f);

    figura.setFillColor(color);
    figura.setOutlineThickness(2.0f);

    if (jugador.esInvulnerable())
    {
        figura.setOutlineColor(
            sf::Color(255, 230, 90)
        );
    }
    else
    {
        figura.setOutlineColor(
            sf::Color(235, 235, 235)
        );
    }

    const float diametro =
        figura.getRadius() * 2.0f;

    const float desplazamiento =
        (TAMANO_CELDA - diametro) / 2.0f;

    figura.setPosition({
        jugador.obtenerColumna() *
            TAMANO_CELDA + desplazamiento,

        jugador.obtenerFila() *
            TAMANO_CELDA + desplazamiento
    });

    ventana.draw(figura);
}

void JuegoVista::dibujarCarga(
    const CargaCodigo& carga
)
{
    sf::RectangleShape figura({
        TAMANO_CELDA * 0.55f,
        TAMANO_CELDA * 0.55f
    });

    figura.setFillColor(
        sf::Color(190, 55, 70)
    );

    figura.setOutlineThickness(3.0f);
    figura.setOutlineColor(
        sf::Color(255, 215, 90)
    );

    const float desplazamiento =
        TAMANO_CELDA * 0.225f;

    figura.setPosition({
        carga.obtenerColumna() *
            TAMANO_CELDA + desplazamiento,

        carga.obtenerFila() *
            TAMANO_CELDA + desplazamiento
    });

    ventana.draw(figura);
}

void JuegoVista::dibujarExplosion(
    const Explosion& explosion
)
{
    const sf::Color colorExplosion(
        255,
        145,
        35
    );

    for (const Posicion& posicion :
         explosion.obtenerPosicionesAfectadas())
    {
        sf::RectangleShape figura({
            TAMANO_CELDA - 6.0f,
            TAMANO_CELDA - 6.0f
        });

        figura.setFillColor(colorExplosion);
        figura.setOutlineThickness(2.0f);
        figura.setOutlineColor(
            sf::Color(255, 230, 90)
        );

        figura.setPosition({
            posicion.obtenerColumna() *
                TAMANO_CELDA + 3.0f,

            posicion.obtenerFila() *
                TAMANO_CELDA + 3.0f
        });

        ventana.draw(figura);
    }
}

void JuegoVista::dibujarEnemigo(
    const Enemigo& enemigo
)
{
    sf::RectangleShape cuerpo({
        TAMANO_CELDA * 0.55f,
        TAMANO_CELDA * 0.55f
    });

    cuerpo.setFillColor(
        sf::Color(145, 65, 180)
    );

    cuerpo.setOutlineThickness(3.0f);
    cuerpo.setOutlineColor(
        sf::Color(50, 20, 65)
    );

    const float desplazamiento =
        TAMANO_CELDA * 0.225f;

    cuerpo.setPosition({
        enemigo.obtenerColumna() *
            TAMANO_CELDA + desplazamiento,

        enemigo.obtenerFila() *
            TAMANO_CELDA + desplazamiento
    });

    ventana.draw(cuerpo);

    for (int ojo = 0; ojo < 2; ++ojo)
    {
        sf::CircleShape figuraOjo(4.0f);

        figuraOjo.setFillColor(
            sf::Color(245, 245, 245)
        );

        figuraOjo.setPosition({
            enemigo.obtenerColumna() *
                TAMANO_CELDA +
                15.0f +
                ojo * 13.0f,

            enemigo.obtenerFila() *
                TAMANO_CELDA +
                17.0f
        });

        ventana.draw(figuraOjo);
    }
}

void JuegoVista::dibujarHUD(const Partida& partida)
{
    sf::RectangleShape panel({
        ANCHO_PANEL,
        624.0f
    });

    panel.setPosition({
        ANCHO_TABLERO,
        0.0f
    });

    panel.setFillColor(
        sf::Color(35, 39, 46)
    );

    panel.setOutlineThickness(-3.0f);
    panel.setOutlineColor(
        sf::Color(70, 76, 88)
    );

    ventana.draw(panel);

    dibujarVidas(
        partida.obtenerProfesor(),
        100.0f,
        sf::Color(70, 130, 180)
    );

    dibujarVidas(
        partida.obtenerEstudiante(),
        330.0f,
        sf::Color(70, 180, 110)
    );
}

void JuegoVista::dibujarVidas(
    const Jugador& jugador,
    float posicionY,
    const sf::Color& color
)
{
    sf::RectangleShape identificador({
        120.0f,
        42.0f
    });

    identificador.setPosition({
        ANCHO_TABLERO + 40.0f,
        posicionY
    });

    identificador.setFillColor(color);
    identificador.setOutlineThickness(2.0f);
    identificador.setOutlineColor(
        sf::Color(235, 235, 235)
    );

    ventana.draw(identificador);

    for (int vida = 0; vida < 3; ++vida)
    {
        sf::CircleShape indicadorVida(14.0f);

        indicadorVida.setPosition({
            ANCHO_TABLERO + 38.0f +
                vida * 48.0f,

            posicionY + 70.0f
        });

        if (vida < jugador.obtenerVidas())
        {
            indicadorVida.setFillColor(
                sf::Color(220, 65, 75)
            );
        }
        else
        {
            indicadorVida.setFillColor(
                sf::Color(75, 78, 86)
            );
        }

        indicadorVida.setOutlineThickness(2.0f);
        indicadorVida.setOutlineColor(
            sf::Color(235, 235, 235)
        );

        ventana.draw(indicadorVida);
    }
}
