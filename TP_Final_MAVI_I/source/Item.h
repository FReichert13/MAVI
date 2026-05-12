#pragma once
#include "raylib.h"
class Item {
public:
    Item(Texture2D* tex, Vector2 pos);
    void draw();
    Vector2 getCenter() const;              //centro para colision circular
    float   getRadius() const { return radius; }
    bool    isActive()  const { return active; }
    void    collect() { active = false; }   //se llama al ser recolectada
private:
    Texture2D* texture;
    Vector2 position;
    bool    active;
    int     width, height;
    float   radius;     //radio aproximado a partir del tamaño del sprite
};