#pragma once
#include "raylib.h"
//EnemyLinear: MRU (velocidad constante)
//EnemyAccelerated: MRUA (gravedad, velocidad creciente)
//EnemyCircular: MCU (orbita alrededor de un punto)
//clase base
class Enemy {
public:
    Enemy(Texture2D* tex, Vector2 pos);
    virtual ~Enemy() = default;
    //las subclases definen como se mueve (polimorfismo)
    virtual void update(float dt) = 0;
    void draw();
    Rectangle getBounds() const;            
    bool isActive() const { return active; }
    void setActive(bool a) { active = a; }  //se desactiva al chocar o salir
protected:
    Texture2D* texture;
    Vector2 position;
    bool    active;          //si es false no se actualiza ni se dibuja
    int     width, height;
};
//MRU
class EnemyLinear : public Enemy {
public:
    EnemyLinear(Texture2D* tex, Vector2 pos, Vector2 vel);
    void update(float dt) override;
private:
    Vector2 velocity;        //const
};
//MRUA
class EnemyAccelerated : public Enemy {
public:
    EnemyAccelerated(Texture2D* tex, Vector2 pos);
    void update(float dt) override;
private:
    Vector2 velocity;        //cambia cada frame por la gravedad
    float   gravity;         //aceleracion constante hacia abajo
};
//MCU
class EnemyCircular : public Enemy {
public:
    EnemyCircular(Texture2D* tex, Vector2 center, float radius, float omega);
    void update(float dt) override;
private:
    Vector2 center;          //punto alrededor del cual orbita
    float   radius;          //distancia del centro
    float   omega;           //velocidad angular
    float   angle;           //angulo actual en la orbita
    float   lifetime;        //tiempo para desactivarse
};