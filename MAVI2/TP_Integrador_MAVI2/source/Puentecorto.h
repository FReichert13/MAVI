#pragma once
#include "raylib.h"
#include <box2d.h>
#include <vector>
class PuenteCorto
{
public:
    PuenteCorto(b2World* mundo,
        b2Body* torreIzq, b2Vec2 anclaIzq,
        b2Body* torreDer, b2Vec2 anclaDer,
        int numSegmentos);
    void Dibujar(bool completado, Texture2D texTablon) const;
    void AplicarFuerzaHorizontal(float accel); //viento sobre los segmentos
private:
    std::vector<b2Body*> segmentos;
    std::vector<b2Vec2>  puntosUnion;   //posiciones de los revolute joints
    float    anchoSeg;
    float    altoSeg;
};
