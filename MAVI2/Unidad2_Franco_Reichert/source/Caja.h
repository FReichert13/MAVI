#pragma once
#include <box2d.h>
#include "raylib.h"
class Caja
{
public:
    //ancho y alto van en pixles, despues los paso a metros para Box2D
    Caja(b2World& mundo, b2Vec2 posicionPx,
        float anchoPx = 50.0f, float altoPx = 50.0f);
    void Dibujar() const;
private:
    b2Body* cuerpo;
    float anchoPx;
    float altoPx;
    Color color;
};