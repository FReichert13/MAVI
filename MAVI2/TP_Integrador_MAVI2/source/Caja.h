#pragma once
#include "raylib.h"
#include <box2d.h>
#include "Etiqueta.h"
//estado de una caja dentro del juego
enum class EstadoCaja { Colgando, Libre, Colocada, Perdida };
class Caja
{
public:
    Caja(b2World* mundo, float x, float y, float w, float h);
    void    Dibujar(Texture2D tex) const;
    b2Body* Cuerpo() const { return body; }
    b2Vec2  Posicion() const { return body->GetPosition(); }
    //estado de juego
    EstadoCaja estado = EstadoCaja::Colgando;
    float      tiempoEstable = 0.0f;  
    float      progreso = 0.0f;       
    float      refX = 0.0f, refY = 0.0f; 
private:
    b2Body* body;
    float    ancho, alto;
    Etiqueta etiqueta;
};