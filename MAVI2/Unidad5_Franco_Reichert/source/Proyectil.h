#ifndef PROYECTIL_H
#define PROYECTIL_H
#include <box2d.h>
#include "raylib.h"
#include "Etiquetas.h"
#include "Constantes.h"
//cuerpo dinamico que dispara el usuario
class Proyectil
{
public:
    //posicion y radio en pixeles, velocidad de disparo en m/s
    Proyectil(b2World* mundo, float xPix, float yPix, float radioPix,
        float vx, float vy);
    ~Proyectil();
    void   Dibujar() const;
    b2Vec2 ObtenerPosicionPix() const;
private:
    b2World* mundo;
    b2Body* cuerpo;
    float       radioPix;
    DatosCuerpo datos;
};
#endif