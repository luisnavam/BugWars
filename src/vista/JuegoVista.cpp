#include "vista/JuegoVista.h"

#include "modelo/BugResistente.h"
#include "modelo/CargaCodigo.h"
#include "modelo/Enemigo.h"
#include "modelo/EstadoPartida.h"
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

    for (const CargaCodigo& carga :
         partida.obtenerCargas())
    {
        dibujarCarga(carga);
    }

    for (const Explosion& explosion :
         partida.obtenerExplosiones())
    {
        dibujarExplosion(explosion);
    }

    for (const auto& enemigo :
         partida.obtenerEnemigos())
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

    if (partida.obtenerEstado() !=
        EstadoPartida::Jugando)
    {
        dibujarPantallaFinal(partida);
    }

    ventana.display();
}

void JuegoVista::dibujarTablero(
    const Partida& partida
)
{
    const Tablero& tablero =
        partida.obtenerTablero();

    for (int fila = 0;
         fila < Tablero::FILAS;
         ++fila)
    {
        for (int columna = 0;
             columna < Tablero::COLUMNAS;
             ++columna)
        {
            const TipoCelda tipo =
                tablero.obtenerCelda(
                    fila,
                    columna
                );

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
    sf::CircleShape figura(
        TAMANO_CELDA * 0.30f
    );

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

        figura.setFillColor(
            colorExplosion
        );

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
    const BugResistente* bugResistente =
        dynamic_cast<const BugResistente*>(
            &enemigo
        );

    float proporcion = 0.55f;

    sf::Color colorCuerpo(
        145,
        65,
        180
    );

    sf::Color colorBorde(
        50,
        20,
        65
    );

    if (bugResistente != nullptr)
    {
        proporcion = 0.68f;

        if (bugResistente->estaDanado())
        {
            colorCuerpo =
                sf::Color(230, 125, 40);

            colorBorde =
                sf::Color(110, 50, 15);
        }
        else
        {
            colorCuerpo =
                sf::Color(175, 45, 55);

            colorBorde =
                sf::Color(80, 15, 20);
        }
    }

    sf::RectangleShape cuerpo({
        TAMANO_CELDA * proporcion,
        TAMANO_CELDA * proporcion
    });

    cuerpo.setFillColor(colorCuerpo);
    cuerpo.setOutlineThickness(3.0f);
    cuerpo.setOutlineColor(colorBorde);

    const float tamanoCuerpo =
        TAMANO_CELDA * proporcion;

    const float desplazamiento =
        (TAMANO_CELDA - tamanoCuerpo) /
        2.0f;

    cuerpo.setPosition({
        enemigo.obtenerColumna() *
            TAMANO_CELDA + desplazamiento,

        enemigo.obtenerFila() *
            TAMANO_CELDA + desplazamiento
    });

    ventana.draw(cuerpo);

    const float posicionBaseX =
        enemigo.obtenerColumna() *
        TAMANO_CELDA;

    const float posicionBaseY =
        enemigo.obtenerFila() *
        TAMANO_CELDA;

    for (int ojo = 0; ojo < 2; ++ojo)
    {
        sf::CircleShape figuraOjo(
            bugResistente != nullptr
                ? 5.0f
                : 4.0f
        );

        figuraOjo.setFillColor(
            sf::Color(245, 245, 245)
        );

        figuraOjo.setPosition({
            posicionBaseX +
                14.0f +
                ojo * 14.0f,

            posicionBaseY + 16.0f
        });

        ventana.draw(figuraOjo);
    }
}

void JuegoVista::dibujarHUD(
    const Partida& partida
)
{
    sf::RectangleShape panel({
        ANCHO_PANEL,
        ALTO_VENTANA
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
        sf::CircleShape indicadorVida(
            14.0f
        );

        indicadorVida.setPosition({
            ANCHO_TABLERO +
                38.0f +
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

        indicadorVida.setOutlineThickness(
            2.0f
        );

        indicadorVida.setOutlineColor(
            sf::Color(235, 235, 235)
        );

        ventana.draw(indicadorVida);
    }
}

void JuegoVista::dibujarPantallaFinal(
    const Partida& partida
)
{
    sf::RectangleShape fondoOscuro({
        920.0f,
        ALTO_VENTANA
    });

    fondoOscuro.setFillColor(
        sf::Color(10, 12, 16, 185)
    );

    ventana.draw(fondoOscuro);

    const bool esVictoria =
        partida.obtenerEstado() ==
        EstadoPartida::Victoria;

    const sf::Color colorPrincipal =
        esVictoria
        ? sf::Color(55, 180, 105)
        : sf::Color(205, 65, 75);

    sf::RectangleShape cartel({
        420.0f,
        310.0f
    });

    cartel.setPosition({
        250.0f,
        155.0f
    });

    cartel.setFillColor(
        sf::Color(32, 36, 43)
    );

    cartel.setOutlineThickness(6.0f);
    cartel.setOutlineColor(
        colorPrincipal
    );

    ventana.draw(cartel);

    sf::RectangleShape franjaSuperior({
        420.0f,
        62.0f
    });

    franjaSuperior.setPosition({
        250.0f,
        155.0f
    });

    franjaSuperior.setFillColor(
        colorPrincipal
    );

    ventana.draw(franjaSuperior);

    if (esVictoria)
    {
        dibujarSimboloVictoria();
    }
    else
    {
        dibujarSimboloDerrota();
    }

    dibujarIndicadorReinicio();
}

void JuegoVista::dibujarSimboloVictoria()
{
    sf::CircleShape circulo(58.0f);

    circulo.setPosition({
        402.0f,
        235.0f
    });

    circulo.setFillColor(
        sf::Color(40, 125, 75)
    );

    circulo.setOutlineThickness(5.0f);

    circulo.setOutlineColor(
        sf::Color(120, 240, 160)
    );

    ventana.draw(circulo);

    sf::RectangleShape tramoCorto({
        18.0f,
        55.0f
    });

    tramoCorto.setPosition({
        428.0f,
        285.0f
    });

    tramoCorto.setFillColor(
        sf::Color(245, 245, 245)
    );

    tramoCorto.setRotation(
        sf::degrees(-45.0f)
    );

    ventana.draw(tramoCorto);

    sf::RectangleShape tramoLargo({
        18.0f,
        95.0f
    });

    tramoLargo.setPosition({
        454.0f,
        309.0f
    });

    tramoLargo.setFillColor(
        sf::Color(245, 245, 245)
    );

    tramoLargo.setRotation(
        sf::degrees(-135.0f)
    );

    ventana.draw(tramoLargo);
}

void JuegoVista::dibujarSimboloDerrota()
{
    sf::CircleShape circulo(58.0f);

    circulo.setPosition({
        402.0f,
        235.0f
    });

    circulo.setFillColor(
        sf::Color(135, 40, 50)
    );

    circulo.setOutlineThickness(5.0f);

    circulo.setOutlineColor(
        sf::Color(245, 120, 130)
    );

    ventana.draw(circulo);

    sf::RectangleShape diagonalUno({
        18.0f,
        95.0f
    });

    diagonalUno.setPosition({
        425.0f,
        263.0f
    });

    diagonalUno.setFillColor(
        sf::Color(245, 245, 245)
    );

    diagonalUno.setRotation(
        sf::degrees(-45.0f)
    );

    ventana.draw(diagonalUno);

    sf::RectangleShape diagonalDos({
        18.0f,
        95.0f
    });

    diagonalDos.setPosition({
        492.0f,
        250.0f
    });

    diagonalDos.setFillColor(
        sf::Color(245, 245, 245)
    );

    diagonalDos.setRotation(
        sf::degrees(45.0f)
    );

    ventana.draw(diagonalDos);
}

void JuegoVista::dibujarIndicadorReinicio()
{
    sf::RectangleShape teclaEnter({
        150.0f,
        48.0f
    });

    teclaEnter.setPosition({
        385.0f,
        390.0f
    });

    teclaEnter.setFillColor(
        sf::Color(58, 64, 74)
    );

    teclaEnter.setOutlineThickness(3.0f);

    teclaEnter.setOutlineColor(
        sf::Color(210, 215, 225)
    );

    ventana.draw(teclaEnter);

    sf::RectangleShape lineaHorizontal({
        65.0f,
        8.0f
    });

    lineaHorizontal.setPosition({
        420.0f,
        409.0f
    });

    lineaHorizontal.setFillColor(
        sf::Color(235, 235, 235)
    );

    ventana.draw(lineaHorizontal);

    sf::RectangleShape lineaVertical({
        8.0f,
        24.0f
    });

    lineaVertical.setPosition({
        477.0f,
        393.0f
    });

    lineaVertical.setFillColor(
        sf::Color(235, 235, 235)
    );

    ventana.draw(lineaVertical);

    sf::RectangleShape puntaSuperior({
        8.0f,
        22.0f
    });

    puntaSuperior.setPosition({
        414.0f,
        405.0f
    });

    puntaSuperior.setFillColor(
        sf::Color(235, 235, 235)
    );

    puntaSuperior.setRotation(
        sf::degrees(45.0f)
    );

    ventana.draw(puntaSuperior);

    sf::RectangleShape puntaInferior({
        8.0f,
        22.0f
    });

    puntaInferior.setPosition({
        418.0f,
        414.0f
    });

    puntaInferior.setFillColor(
        sf::Color(235, 235, 235)
    );

    puntaInferior.setRotation(
        sf::degrees(135.0f)
    );

    ventana.draw(puntaInferior);
}
