#pragma once

class Jugador;
class Tablero;

class Enemigo
{
public:
    Enemigo(
        int filaInicial,
        int columnaInicial,
        int puntosVidaIniciales
    );

    virtual ~Enemigo() = default;

    virtual void actualizar(
        float deltaTiempo,
        const Tablero& tablero,
        const Jugador& profesor,
        const Jugador& estudiante
    ) = 0;

    void actualizarEstado(float deltaTiempo);
    void recibirImpacto();

    int obtenerFila() const;
    int obtenerColumna() const;
    int obtenerPuntosVida() const;

    bool estaActivo() const;
    bool esInvulnerable() const;

protected:
    int fila;
    int columna;
    int puntosVida;
    bool activo;

    void cambiarPosicion(
        int nuevaFila,
        int nuevaColumna
    );

private:
    static constexpr float DURACION_INVULNERABILIDAD = 0.60f;

    float invulnerabilidadRestante;
};
