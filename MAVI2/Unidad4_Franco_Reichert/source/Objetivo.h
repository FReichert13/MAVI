#ifndef OBJETIVO_H
#define OBJETIVO_H
#include <box2d.h>
#include "raylib.h"
#include "Etiquetas.h"
#include "Constantes.h"
class Objetivo
{
public:
    Objetivo(b2World* mundo, float cxPix, float cyPix, float anchoPix, float altoPix);
    ~Objetivo();
    void MarcarImpactado() { impactado = true; }
    void Reiniciar() { impactado = false; }
    void Dibujar() const;
private:
    b2Body* cuerpo;
    float       ancho, alto;   //en pixeles
    bool        impactado;
    DatosCuerpo datos;
};
#endif 