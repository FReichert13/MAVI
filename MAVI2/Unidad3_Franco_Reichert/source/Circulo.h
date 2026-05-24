#pragma once
#include "raylib.h"
#include <box2d.h>
//lo mismo que el cuerpo rectangular de la clase caja, pero circular
class Circulo
{
public:
    Circulo(b2World* mundo, float x, float y, float r, Color c);
    void Dibujar();
private:
    b2Body* body;
    float   radio;
    Color   color;
};