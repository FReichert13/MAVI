#ifndef JUGADOR_H
#define JUGADOR_H
#include "raylib.h"
enum EstadoJugador {
    EN_SUELO,
    SALTANDO,
    CAYENDO
};
class Jugador {
private:
    Vector2 position;       
    Vector2 velocity;      
    float width;            
    float height;          
    Color color;            
    EstadoJugador estado;   
    bool onGround;          
    Texture2D texture;      
    bool textureLoaded;    
    int contadorSaltos;    
    const float GRAVITY = 980.0f;
    const float JUMP_FORCE = -620.0f;
    const float MOVE_SPEED = 220.0f;
public:
    Jugador(float x, float y, float w, float h);
    ~Jugador();
    void CargarTextura(const char* rutaTextura);
    void Update(float deltaTime, float groundLevel);
    void Draw();
    void HandleInput();
    void Jump();
    void Reset(float x, float y);
    Vector2 GetPosition() const;
    Vector2 GetVelocity() const;
    Rectangle GetCollisionRect() const;
    bool IsOnGround() const;
    int GetContadorSaltos() const;
    void SetPosition(float x, float y);
    void SetOnGround(bool value);
};
#endif