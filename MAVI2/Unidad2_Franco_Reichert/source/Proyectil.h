#pragma once
#include <box2d.h>
#include "raylib.h"
class Proyectil
{
public:
    Proyectil(b2World& mundoFisico, b2Vec2 posicionInicialPx, float radioPx = 15.0f);
    ~Proyectil();
    void Lanzar(b2Vec2 impulso);
    void Dibujar() const;
    bool FueLanzado() const { return lanzado; }
    Vector2 GetPosicionPx() const;
private:
    b2World* mundo;
    b2Body* cuerpo;
    float radioPx;
    Color color;
    bool lanzado;
};