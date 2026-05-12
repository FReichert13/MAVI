#pragma once
#include "raylib.h"
class Player {
public:
    Player(Texture2D* tex, Vector2 pos);
    void update(float dt);   //procesa input y actualiza posicion
    void draw();
    void takeDamage();              //resta una vida
    void addScore(int points);      //suma al puntaje
    void reset(Vector2 pos);        //reinicia vidas, puntaje y posicion
    Rectangle getBounds() const;    //caja AABB para colisiones con enemigos
    Vector2   getCenter() const;    //centro para colision circular con items
    int  getLives() const { return lives; }
    int  getScore() const { return score; }
    bool isAlive() const { return lives > 0; }
private:
    Texture2D* texture;     //puntero a la textura
    Vector2 position;
    float   speed;          //pixeles por segundo mru
    int     lives;
    int     score;
    int     width, height;  //tomados de la textura
};