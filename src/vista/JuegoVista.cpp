#include "vista/JuegoVista.h"

#include <array>
#include <string>

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

namespace
{
bool cargarTexturaDesdeRutas(
    sf::Texture& textura,
    const std::string& rutaRelativa
)
{
    const std::array<std::string, 3> rutas = {
        rutaRelativa,
        "../../" + rutaRelativa,
        "../../../" + rutaRelativa
    };

    for (const std::string& ruta : rutas)
    {
        if (textura.loadFromFile(ruta))
        {
            textura.setSmooth(false);
            return true;
        }
    }

    return false;
}
}

JuegoVista::JuegoVista()
    : ventana(
          sf::VideoMode({920u, 624u}),
          "Bug Wars - Laboratorio de Programacion"
      ),
      fuenteCargada(false),
      texturaProfesorCargada(false),
      texturaEstudianteCargada(false),
      texturaBugBasicoCargada(false),
      texturaBugResistenteCargada(false),
      texturaBugResistenteDanadoCargada(false),
      texturaPisoCargada(false),
      texturaParedCargada(false),
      texturaPupitreCargada(false),
      texturaComputadoraCargada(false),
      texturaPizarronCargada(false),
      texturaEscritorioProfesorCargada(false),
      texturaBibliotecaCargada(false),
      texturaPuertaCargada(false),
      texturaVentanaCargada(false),
      texturaCargaCargada(false),
      texturaExplosionCargada(false),
      ultimaOleadaMostrada(0),
      mostrarAvisoOleada(false)
{
    ventana.setFramerateLimit(60);

    fuenteCargada =
        fuente.openFromFile(
            "C:/Windows/Fonts/arial.ttf"
        );

    if (!fuenteCargada)
    {
        fuenteCargada =
            fuente.openFromFile(
                "C:/Windows/Fonts/segoeui.ttf"
            );
    }

    texturaProfesorCargada =
        cargarTexturaDesdeRutas(
            texturaProfesor,
            "assets/sprites/jugadores/profesor.png"
        );

    texturaEstudianteCargada =
        cargarTexturaDesdeRutas(
            texturaEstudiante,
            "assets/sprites/jugadores/estudiante.png"
        );

    texturaBugBasicoCargada =
        cargarTexturaDesdeRutas(
            texturaBugBasico,
            "assets/sprites/enemigos/bug_basico.png"
        );

    texturaBugResistenteCargada =
        cargarTexturaDesdeRutas(
            texturaBugResistente,
            "assets/sprites/enemigos/bug_resistente.png"
        );

    texturaBugResistenteDanadoCargada =
        cargarTexturaDesdeRutas(
            texturaBugResistenteDanado,
            "assets/sprites/enemigos/bug_resistente_danado.png"
        );

    texturaPisoCargada =
        cargarTexturaDesdeRutas(
            texturaPiso,
            "assets/sprites/objetos/piso.png"
        );

    texturaParedCargada =
        cargarTexturaDesdeRutas(
            texturaPared,
            "assets/sprites/objetos/pared.png"
        );

    texturaPupitreCargada =
        cargarTexturaDesdeRutas(
            texturaPupitre,
            "assets/sprites/objetos/pupitre.png"
        );

    texturaComputadoraCargada =
        cargarTexturaDesdeRutas(
            texturaComputadora,
            "assets/sprites/objetos/computadora.png"
        );

    texturaPizarronCargada =
        cargarTexturaDesdeRutas(
            texturaPizarron,
            "assets/sprites/objetos/pizarron.png"
        );

    texturaEscritorioProfesorCargada =
        cargarTexturaDesdeRutas(
            texturaEscritorioProfesor,
            "assets/sprites/objetos/escritorio_profesor.png"
        );

    texturaBibliotecaCargada =
        cargarTexturaDesdeRutas(
            texturaBiblioteca,
            "assets/sprites/objetos/biblioteca.png"
        );

    texturaPuertaCargada =
        cargarTexturaDesdeRutas(
            texturaPuerta,
            "assets/sprites/objetos/puerta.png"
        );

    texturaVentanaCargada =
        cargarTexturaDesdeRutas(
            texturaVentana,
            "assets/sprites/objetos/ventana.png"
        );

    texturaCargaCargada =
        cargarTexturaDesdeRutas(
            texturaCarga,
            "assets/sprites/efectos/carga.png"
        );

    texturaExplosionCargada =
        cargarTexturaDesdeRutas(
            texturaExplosion,
            "assets/sprites/efectos/explosion.png"
        );
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

void JuegoVista::renderizar(
    const Partida& partida
)
{
    ventana.clear(sf::Color(24, 27, 32));

    if (partida.obtenerOleadaActual() !=
        ultimaOleadaMostrada)
    {
        ultimaOleadaMostrada =
            partida.obtenerOleadaActual();

        mostrarAvisoOleada = true;
        relojAvisoOleada.restart();
    }

    dibujarEscena(partida);

    if (partida.estaEnTransicionOleada() ||
        mostrarAvisoOleada)
    {
        dibujarAvisoOleada(partida);
    }

    if (mostrarAvisoOleada &&
        relojAvisoOleada.getElapsedTime().asSeconds() >=
            DURACION_AVISO_OLEADA)
    {
        mostrarAvisoOleada = false;
    }

    if (partida.obtenerEstado() !=
        EstadoPartida::Jugando)
    {
        dibujarPantallaFinal(partida);
    }

    ventana.display();
}

void JuegoVista::renderizarMenu()
{
    ventana.clear(sf::Color(24, 27, 32));

    sf::RectangleShape fondo({
        920.0f,
        ALTO_VENTANA
    });

    fondo.setFillColor(
        sf::Color(28, 32, 39)
    );

    ventana.draw(fondo);

    sf::RectangleShape cabecera({
        920.0f,
        130.0f
    });

    cabecera.setFillColor(
        sf::Color(48, 54, 65)
    );

    ventana.draw(cabecera);

    dibujarTextoCentrado(
        "BUG WARS",
        32.0f,
        48,
        sf::Color(245, 245, 245)
    );

    dibujarTextoCentrado(
        "Laboratorio de Programacion II",
        91.0f,
        22,
        sf::Color(175, 190, 210)
    );

    sf::RectangleShape opcionSolo({
        300.0f,
        170.0f
    });

    opcionSolo.setPosition({
        125.0f,
        205.0f
    });

    opcionSolo.setFillColor(
        sf::Color(55, 80, 115)
    );

    opcionSolo.setOutlineThickness(4.0f);
    opcionSolo.setOutlineColor(
        sf::Color(105, 165, 225)
    );

    ventana.draw(opcionSolo);

    sf::RectangleShape opcionDuo({
        300.0f,
        170.0f
    });

    opcionDuo.setPosition({
        495.0f,
        205.0f
    });

    opcionDuo.setFillColor(
        sf::Color(52, 100, 75)
    );

    opcionDuo.setOutlineThickness(4.0f);
    opcionDuo.setOutlineColor(
        sf::Color(95, 200, 135)
    );

    ventana.draw(opcionDuo);

    if (fuenteCargada)
    {
        sf::Text textoSolo(
            fuente,
            "1 - MODO SOLO",
            28
        );

        textoSolo.setFillColor(
            sf::Color::White
        );

        textoSolo.setPosition({
            166.0f,
            255.0f
        });

        ventana.draw(textoSolo);

        sf::Text detalleSolo(
            fuente,
            "Profesor: WASD + Espacio",
            18
        );

        detalleSolo.setFillColor(
            sf::Color(215, 225, 240)
        );

        detalleSolo.setPosition({
            155.0f,
            315.0f
        });

        ventana.draw(detalleSolo);

        sf::Text textoDuo(
            fuente,
            "2 - MODO DUO",
            28
        );

        textoDuo.setFillColor(
            sf::Color::White
        );

        textoDuo.setPosition({
            546.0f,
            255.0f
        });

        ventana.draw(textoDuo);

        sf::Text detalleDuo(
            fuente,
            "Profesor + Estudiante",
            18
        );

        detalleDuo.setFillColor(
            sf::Color(215, 235, 220)
        );

        detalleDuo.setPosition({
            548.0f,
            315.0f
        });

        ventana.draw(detalleDuo);
    }

    dibujarTextoCentrado(
        "P o ESC: pausar durante la partida",
        455.0f,
        20,
        sf::Color(205, 210, 220)
    );

    dibujarTextoCentrado(
        "ESC: salir",
        505.0f,
        18,
        sf::Color(155, 165, 180)
    );

    ventana.display();
}

void JuegoVista::renderizarPausa(
    const Partida& partida
)
{
    ventana.clear(sf::Color(24, 27, 32));

    dibujarEscena(partida);

    sf::RectangleShape fondoOscuro({
        920.0f,
        ALTO_VENTANA
    });

    fondoOscuro.setFillColor(
        sf::Color(8, 10, 14, 205)
    );

    ventana.draw(fondoOscuro);

    sf::RectangleShape panel({
        470.0f,
        280.0f
    });

    panel.setPosition({
        225.0f,
        170.0f
    });

    panel.setFillColor(
        sf::Color(35, 40, 49)
    );

    panel.setOutlineThickness(5.0f);
    panel.setOutlineColor(
        sf::Color(230, 190, 70)
    );

    ventana.draw(panel);

    dibujarTextoCentrado(
        "JUEGO PAUSADO",
        210.0f,
        38,
        sf::Color(245, 220, 110)
    );

    dibujarTextoCentrado(
        "P o ESC - Continuar",
        300.0f,
        24,
        sf::Color::White
    );

    dibujarTextoCentrado(
        "M - Volver al menu",
        350.0f,
        22,
        sf::Color(195, 205, 220)
    );

    ventana.display();
}

void JuegoVista::dibujarEscena(
    const Partida& partida
)
{
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
            sf::Color(70, 130, 180),
            texturaProfesorCargada
                ? &texturaProfesor
                : nullptr
        );
    }

    if (partida.esModoDuo() &&
        partida.obtenerEstudiante().estaActivo())
    {
        dibujarJugador(
            partida.obtenerEstudiante(),
            sf::Color(70, 180, 110),
            texturaEstudianteCargada
                ? &texturaEstudiante
                : nullptr
        );
    }

    dibujarHUD(partida);
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
    const sf::Color& color,
    const sf::Texture* textura
)
{
    if (textura != nullptr)
    {
        dibujarSpriteEnCelda(
            *textura,
            jugador.obtenerFila(),
            jugador.obtenerColumna(),
            1.55f,
            jugador.esInvulnerable()
                ? sf::Color(255, 235, 145)
                : sf::Color::White
        );

        return;
    }

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

    if (bugResistente == nullptr &&
        texturaBugBasicoCargada)
    {
        dibujarSpriteEnCelda(
            texturaBugBasico,
            enemigo.obtenerFila(),
            enemigo.obtenerColumna(),
            1.45f
        );

        return;
    }

    if (bugResistente != nullptr)
    {
        if (bugResistente->estaDanado() &&
            texturaBugResistenteDanadoCargada)
        {
            dibujarSpriteEnCelda(
                texturaBugResistenteDanado,
                enemigo.obtenerFila(),
                enemigo.obtenerColumna(),
                1.50f
            );

            return;
        }

        if (!bugResistente->estaDanado() &&
            texturaBugResistenteCargada)
        {
            dibujarSpriteEnCelda(
                texturaBugResistente,
                enemigo.obtenerFila(),
                enemigo.obtenerColumna(),
                1.50f
            );

            return;
        }
    }

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

void JuegoVista::dibujarSpriteEnCelda(
    const sf::Texture& textura,
    int fila,
    int columna,
    float proporcion,
    const sf::Color& color
)
{
    const sf::Vector2u tamanoTextura =
        textura.getSize();

    if (tamanoTextura.x == 0 ||
        tamanoTextura.y == 0)
    {
        return;
    }

    sf::Sprite sprite(textura);

    const float limite =
        TAMANO_CELDA * proporcion;

    const float escalaX =
        limite /
        static_cast<float>(tamanoTextura.x);

    const float escalaY =
        limite /
        static_cast<float>(tamanoTextura.y);

    const float escala =
        escalaX < escalaY
        ? escalaX
        : escalaY;

    sprite.setScale({
        escala,
        escala
    });

    sprite.setColor(color);

    const float anchoFinal =
        static_cast<float>(tamanoTextura.x) *
        escala;

    const float altoFinal =
        static_cast<float>(tamanoTextura.y) *
        escala;

    const float desplazamientoX =
        (TAMANO_CELDA - anchoFinal) / 2.0f;

    const float desplazamientoY =
        (TAMANO_CELDA - altoFinal) / 2.0f;

    sprite.setPosition({
        columna * TAMANO_CELDA +
            desplazamientoX,

        fila * TAMANO_CELDA +
            desplazamientoY
    });

    ventana.draw(sprite);
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
        sf::Color(30, 34, 41)
    );

    panel.setOutlineThickness(-3.0f);

    panel.setOutlineColor(
        sf::Color(72, 80, 92)
    );

    ventana.draw(panel);

    sf::RectangleShape cabecera({
        ANCHO_PANEL - 24.0f,
        112.0f
    });

    cabecera.setPosition({
        ANCHO_TABLERO + 12.0f,
        16.0f
    });

    cabecera.setFillColor(
        sf::Color(40, 45, 54)
    );

    cabecera.setOutlineThickness(2.0f);
    cabecera.setOutlineColor(
        sf::Color(72, 80, 92)
    );

    ventana.draw(cabecera);

    dibujarTextoCentradoEnArea(
        "OLEADA " +
            std::to_string(
                partida.obtenerOleadaActual()
            ) +
            " / " +
            std::to_string(
                Partida::TOTAL_OLEADAS
            ),
        ANCHO_TABLERO,
        ANCHO_PANEL,
        30.0f,
        20,
        sf::Color(245, 220, 105)
    );

    dibujarTextoCentradoEnArea(
        "BUGS: " +
            std::to_string(
                partida.obtenerBugsRestantes()
            ),
        ANCHO_TABLERO,
        ANCHO_PANEL,
        65.0f,
        19,
        sf::Color(235, 238, 245)
    );

    dibujarTextoCentradoEnArea(
        partida.esModoDuo()
            ? "MODO DUO"
            : "MODO SOLO",
        ANCHO_TABLERO,
        ANCHO_PANEL,
        96.0f,
        16,
        sf::Color(150, 185, 220)
    );

    if (partida.esModoDuo())
    {
        dibujarVidas(
            partida.obtenerProfesor(),
            "PROFESOR",
            165.0f,
            sf::Color(70, 130, 180)
        );

        sf::RectangleShape separador({
            ANCHO_PANEL - 32.0f,
            2.0f
        });

        separador.setPosition({
            ANCHO_TABLERO + 16.0f,
            318.0f
        });

        separador.setFillColor(
            sf::Color(72, 80, 92)
        );

        ventana.draw(separador);

        dibujarVidas(
            partida.obtenerEstudiante(),
            "ESTUDIANTE",
            350.0f,
            sf::Color(70, 180, 110)
        );
    }
    else
    {
        dibujarVidas(
            partida.obtenerProfesor(),
            "PROFESOR",
            235.0f,
            sf::Color(70, 130, 180)
        );
    }
}

void JuegoVista::dibujarVidas(
    const Jugador& jugador,
    const std::string& nombre,
    float posicionY,
    const sf::Color& color
)
{
    sf::RectangleShape identificador({
        150.0f,
        44.0f
    });

    identificador.setPosition({
        ANCHO_TABLERO + 25.0f,
        posicionY
    });

    identificador.setFillColor(color);
    identificador.setOutlineThickness(2.0f);

    identificador.setOutlineColor(
        sf::Color(235, 235, 235)
    );

    ventana.draw(identificador);

    dibujarTextoCentradoEnArea(
        nombre,
        ANCHO_TABLERO + 25.0f,
        150.0f,
        posicionY + 8.0f,
        18,
        sf::Color::White
    );

    for (int vida = 0; vida < 3; ++vida)
    {
        dibujarCorazon(
            ANCHO_TABLERO +
                37.0f +
                vida * 49.0f,
            posicionY + 72.0f,
            vida < jugador.obtenerVidas()
        );
    }
}

void JuegoVista::dibujarCorazon(
    float posicionX,
    float posicionY,
    bool activo
)
{
    const sf::Color colorRelleno =
        activo
        ? sf::Color(220, 65, 75)
        : sf::Color(72, 76, 84);

    const sf::Color colorBorde =
        activo
        ? sf::Color(255, 220, 225)
        : sf::Color(125, 130, 140);

    sf::CircleShape circuloIzquierdo(9.0f);
    circuloIzquierdo.setPosition({
        posicionX,
        posicionY
    });
    circuloIzquierdo.setFillColor(colorRelleno);
    circuloIzquierdo.setOutlineThickness(1.5f);
    circuloIzquierdo.setOutlineColor(colorBorde);

    sf::CircleShape circuloDerecho(9.0f);
    circuloDerecho.setPosition({
        posicionX + 12.0f,
        posicionY
    });
    circuloDerecho.setFillColor(colorRelleno);
    circuloDerecho.setOutlineThickness(1.5f);
    circuloDerecho.setOutlineColor(colorBorde);

    sf::ConvexShape punta(3);
    punta.setPoint(
        0,
        {0.0f, 0.0f}
    );
    punta.setPoint(
        1,
        {30.0f, 0.0f}
    );
    punta.setPoint(
        2,
        {15.0f, 22.0f}
    );
    punta.setPosition({
        posicionX,
        posicionY + 8.0f
    });
    punta.setFillColor(colorRelleno);
    punta.setOutlineThickness(1.5f);
    punta.setOutlineColor(colorBorde);

    ventana.draw(punta);
    ventana.draw(circuloIzquierdo);
    ventana.draw(circuloDerecho);
}

void JuegoVista::dibujarAvisoOleada(
    const Partida& partida
)
{
    sf::RectangleShape fondo({
        ANCHO_TABLERO,
        ALTO_VENTANA
    });

    fondo.setFillColor(
        sf::Color(10, 12, 16, 145)
    );

    ventana.draw(fondo);

    const float anchoCartel = 500.0f;
    const float altoCartel = 180.0f;
    const float posicionCartelX =
        (ANCHO_TABLERO - anchoCartel) / 2.0f;
    const float posicionCartelY = 220.0f;

    sf::RectangleShape cartel({
        anchoCartel,
        altoCartel
    });

    cartel.setPosition({
        posicionCartelX,
        posicionCartelY
    });

    cartel.setFillColor(
        sf::Color(35, 40, 49, 245)
    );

    cartel.setOutlineThickness(5.0f);
    cartel.setOutlineColor(
        sf::Color(235, 195, 70)
    );

    ventana.draw(cartel);

    if (partida.estaEnTransicionOleada())
    {
        dibujarTextoCentradoEnArea(
            "OLEADA SUPERADA",
            posicionCartelX,
            anchoCartel,
            posicionCartelY + 28.0f,
            32,
            sf::Color(105, 225, 145)
        );

        dibujarTextoCentradoEnArea(
            "PREPARATE PARA LA OLEADA " +
                std::to_string(
                    partida.obtenerOleadaActual()
                ),
            posicionCartelX,
            anchoCartel,
            posicionCartelY + 100.0f,
            21,
            sf::Color(245, 230, 145)
        );
    }
    else
    {
        dibujarTextoCentradoEnArea(
            "OLEADA " +
                std::to_string(
                    partida.obtenerOleadaActual()
                ),
            posicionCartelX,
            anchoCartel,
            posicionCartelY + 35.0f,
            36,
            sf::Color(245, 220, 105)
        );

        dibujarTextoCentradoEnArea(
            std::to_string(
                partida.obtenerBugsRestantes()
            ) +
                " BUGS POR ELIMINAR",
            posicionCartelX,
            anchoCartel,
            posicionCartelY + 108.0f,
            21,
            sf::Color::White
        );
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


void JuegoVista::dibujarTextoCentrado(
    const std::string& contenido,
    float posicionY,
    unsigned int tamano,
    const sf::Color& color
)
{
    if (!fuenteCargada)
    {
        return;
    }

    sf::Text texto(
        fuente,
        contenido,
        tamano
    );

    texto.setFillColor(color);

    const sf::FloatRect limites =
        texto.getLocalBounds();

    texto.setPosition({
        (920.0f - limites.size.x) / 2.0f -
            limites.position.x,
        posicionY
    });

    ventana.draw(texto);
}


void JuegoVista::dibujarTextoCentradoEnArea(
    const std::string& contenido,
    float inicioX,
    float anchoArea,
    float posicionY,
    unsigned int tamano,
    const sf::Color& color
)
{
    if (!fuenteCargada)
    {
        return;
    }

    sf::Text texto(
        fuente,
        contenido,
        tamano
    );

    texto.setFillColor(color);

    const sf::FloatRect limites =
        texto.getLocalBounds();

    texto.setPosition({
        inicioX +
            (anchoArea - limites.size.x) / 2.0f -
            limites.position.x,
        posicionY
    });

    ventana.draw(texto);
}
