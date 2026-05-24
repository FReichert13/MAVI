#pragma once
#include "raylib.h"
#include <box2d.h>
class GruaMovil
{
public:
    GruaMovil(b2World* mundo, b2Body* rail,
        float xCarrito, float yCarrito,
        float anchoCarrito, float altoCarrito,
        float altoMastil,
        float anchoBrazo,
        float radioBola,
        float largoCuerda);
    void MoverCarrito(float velocidad);          
    void FrenarCarrito();
    void AplicarImpulsoBola(float ix, float iy); 
    void Dibujar();
private:
    b2Body* carrito;
    b2Body* mastil;
    b2Body* brazo;
    b2Body* bola;
    float anchoCar, altoCar;
    float anchoMast, altoMast;
    float anchoBr, altoBr;
    float radBola;
};