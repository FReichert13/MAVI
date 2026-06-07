#pragma once
#include "raylib.h"
#include <box2d.h>
class Grua
{
public:
    Grua(b2World* mundo, b2Body* rail, float xInicial, float yRail);
    void   Mover(float velocidad);   //mueve el carrito sobre el eje X
    void   Frenar();                 //frena suavemente el carrito
    void   Enganchar(b2Body* caja);  //crea el cable 
    void   Soltar();                 //destruye el cable
    b2Vec2 PosicionGancho() const;   //punto inferior del mastil
    b2Vec2 PuntoColgado() const;     //donde aparece una caja nueva colgada
    void   Dibujar(Texture2D texCarrito, Texture2D texMastil, Texture2D texGancho) const;
private:
    b2World* mundo;
    b2Body* carrito;
    b2Body* mastil;
    b2DistanceJoint* cable = nullptr;
    float            largoCable;
    float            anchoCar, altoCar;
    float            anchoMast, altoMast;
};