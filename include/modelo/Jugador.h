#pragma once

#include <string>

class Jugador
{
public:
    Jugador(
        const std::string& nombre,
        int filaInicial,
        int columnaInicial
    );

    const std::string& obtenerNombre() const;

    int obtenerFila() const;
    int obtenerColumna() const;
    int obtenerVidas() const;

    bool estaActivo() const;
    bool esInvulnerable() const;

    void cambiarPosicion(int nuevaFila, int nuevaColumna);
    void recibirDanio();
    void actualizar(float deltaTiempo);

private:
    static constexpr int VIDAS_INICIALES = 3;
    static constexpr float DURACION_INVULNERABILIDAD = 1.5f;

    std::string nombre;

    int fila;
    int columna;
    int vidas;

    bool activo;
    float invulnerabilidadRestante;
};
