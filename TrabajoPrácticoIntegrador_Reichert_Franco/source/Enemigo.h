#ifndef ENEMIGO_H
#define ENEMIGO_H
#include "raylib.h"
class Enemigo {
private:
    Vector2 position;          
    float width;              
    float height;          
    float velocidad;            
    float limiteIzquierdo;      
    float limiteDerecho;        
    bool moviendoDerecha;       
    Texture2D texture;          
    bool textureLoaded;        
public:
    //constructor y destructor
    Enemigo(float x, float y, float w, float h, float limIzq, float limDer, float vel);
    ~Enemigo();
    //metodos 
    void CargarTextura(const char* rutaTextura);
    void Update(float deltaTime);
    void Draw();
    //getters
    Rectangle GetCollisionRect() const;
};
#endif