#include "controlador/JuegoControlador.h"

#include "modelo/Direccion.h"
#include "modelo/EstadoPartida.h"
#include "modelo/IdJugador.h"
#include "modelo/ModoJuego.h"

JuegoControlador::JuegoControlador()
    : partida(),
      vista(),
      estadoAplicacion(EstadoAplicacion::Menu)
{
}

void JuegoControlador::ejecutar()
{
    sf::Clock reloj;

    while (vista.estaAbierta())
    {
        procesarEventos();

        if (!vista.estaAbierta())
        {
            break;
        }

        const float deltaTiempo =
            reloj.restart().asSeconds();

        if (estadoAplicacion ==
            EstadoAplicacion::Jugando)
        {
            partida.actualizar(deltaTiempo);
            vista.renderizar(partida);
        }
        else if (estadoAplicacion ==
                 EstadoAplicacion::Pausado)
        {
            vista.renderizarPausa(partida);
        }
        else
        {
            vista.renderizarMenu();
        }
    }
}

void JuegoControlador::iniciarPartida(
    ModoJuego modo
)
{
    partida.iniciar(modo);
    estadoAplicacion = EstadoAplicacion::Jugando;
}

void JuegoControlador::procesarEventos()
{
    while (const std::optional<sf::Event> evento =
               vista.obtenerEvento())
    {
        if (evento->is<sf::Event::Closed>())
        {
            vista.cerrar();
            return;
        }

        const auto* tecla =
            evento->getIf<sf::Event::KeyPressed>();

        if (tecla == nullptr)
        {
            continue;
        }

        const sf::Keyboard::Scancode codigo =
            tecla->scancode;

        if (estadoAplicacion ==
            EstadoAplicacion::Menu)
        {
            if (codigo ==
                    sf::Keyboard::Scancode::Num1 ||
                codigo ==
                    sf::Keyboard::Scancode::Numpad1)
            {
                iniciarPartida(ModoJuego::Solo);
            }
            else if (codigo ==
                         sf::Keyboard::Scancode::Num2 ||
                     codigo ==
                         sf::Keyboard::Scancode::Numpad2)
            {
                iniciarPartida(ModoJuego::Duo);
            }
            else if (codigo ==
                     sf::Keyboard::Scancode::Escape)
            {
                vista.cerrar();
            }

            continue;
        }

        if (estadoAplicacion ==
            EstadoAplicacion::Pausado)
        {
            if (codigo ==
                    sf::Keyboard::Scancode::P ||
                codigo ==
                    sf::Keyboard::Scancode::Escape)
            {
                estadoAplicacion =
                    EstadoAplicacion::Jugando;
            }
            else if (codigo ==
                     sf::Keyboard::Scancode::M)
            {
                estadoAplicacion =
                    EstadoAplicacion::Menu;
            }

            continue;
        }

        if (partida.obtenerEstado() !=
            EstadoPartida::Jugando)
        {
            if (codigo ==
                sf::Keyboard::Scancode::Enter)
            {
                partida.reiniciar();
            }
            else if (codigo ==
                         sf::Keyboard::Scancode::M ||
                     codigo ==
                         sf::Keyboard::Scancode::Escape)
            {
                estadoAplicacion =
                    EstadoAplicacion::Menu;
            }

            continue;
        }

        if (codigo ==
                sf::Keyboard::Scancode::P ||
            codigo ==
                sf::Keyboard::Scancode::Escape)
        {
            estadoAplicacion =
                EstadoAplicacion::Pausado;
            continue;
        }

        // Profesor: W, A, S, D y Espacio.
        if (codigo == sf::Keyboard::Scancode::W)
        {
            partida.moverJugador(
                IdJugador::Profesor,
                Direccion::Arriba
            );
        }
        else if (codigo == sf::Keyboard::Scancode::S)
        {
            partida.moverJugador(
                IdJugador::Profesor,
                Direccion::Abajo
            );
        }
        else if (codigo == sf::Keyboard::Scancode::A)
        {
            partida.moverJugador(
                IdJugador::Profesor,
                Direccion::Izquierda
            );
        }
        else if (codigo == sf::Keyboard::Scancode::D)
        {
            partida.moverJugador(
                IdJugador::Profesor,
                Direccion::Derecha
            );
        }
        else if (codigo == sf::Keyboard::Scancode::Space)
        {
            partida.colocarCarga(
                IdJugador::Profesor
            );
        }

        // Estudiante: flechas y Enter.
        else if (codigo == sf::Keyboard::Scancode::Up)
        {
            partida.moverJugador(
                IdJugador::Estudiante,
                Direccion::Arriba
            );
        }
        else if (codigo == sf::Keyboard::Scancode::Down)
        {
            partida.moverJugador(
                IdJugador::Estudiante,
                Direccion::Abajo
            );
        }
        else if (codigo == sf::Keyboard::Scancode::Left)
        {
            partida.moverJugador(
                IdJugador::Estudiante,
                Direccion::Izquierda
            );
        }
        else if (codigo == sf::Keyboard::Scancode::Right)
        {
            partida.moverJugador(
                IdJugador::Estudiante,
                Direccion::Derecha
            );
        }
        else if (codigo == sf::Keyboard::Scancode::Enter)
        {
            partida.colocarCarga(
                IdJugador::Estudiante
            );
        }
    }
}
