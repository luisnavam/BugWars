
#pragma once

class Enemigo
{
public:
    Enemigo(
        int filaInicial,
        int columnaInicial,
        int puntosVidaIniciales
    );

    virtual ~Enemigo() = default;

    virtual void actualizar(float deltaTiempo) = 0;

    void recibirImpacto();

    int obtenerFila() const;
    int obtenerColumna() const;
    int obtenerPuntosVida() const;

    bool estaActivo() const;

protected:
    int fila;
    int columna;
    int puntosVida;
    bool activo;
};
