#pragma once
#include "raylib.h"
#include <box2d.h>
#include <vector>
class PuenteColgante
{
public:
    PuenteColgante(b2World* mundo,
        b2Body* extremoIzq, b2Vec2 anclaIzq,
        b2Body* extremoDer, b2Vec2 anclaDer,
        int numSegmentos, Color c);
    void Dibujar();
private:
    std::vector<b2Body*> segmentos;
    std::vector<b2Vec2>  anclas;     
    float anchoSeg;
    float altoSeg;
    Color color;
};