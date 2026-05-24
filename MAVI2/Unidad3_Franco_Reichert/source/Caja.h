#pragma once
#include "raylib.h"
#include <box2d.h>
//cuerpo (rectangulo) que cae libremente y choca contra la escena
class Caja
{
public:
    Caja(b2World* mundo, float x, float y, float w, float h, Color c);
    void Dibujar();
private:
    b2Body* body;
    float   ancho;
    float   alto;
    Color   color;
};