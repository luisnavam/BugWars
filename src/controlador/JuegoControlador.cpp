#include "controlador/JuegoControlador.h"

#include "modelo/Direccion.h"
#include "modelo/IdJugador.h"

JuegoControlador::JuegoControlador()
    : partida(),
      vista()
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

        partida.actualizar(deltaTiempo);
        vista.renderizar(partida);
    }
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

        if (codigo == sf::Keyboard::Scancode::Escape)
        {
            vista.cerrar();
            return;
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
