#ifndef GAME_H
#define GAME_H
#include "raylib.h"
class Game{
private:
static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 600;
static constexpr float INITIAL_SPEED = 200.0f;
static constexpr float SPEED_INCREMENT = 50.0f;
static constexpr float MIN_SPEED = 50.0f;
static constexpr float MAX_SPEED = 500.0f;
//propiedades de la pelotita
Texture2D ballTexture;     
Vector2 ballPosition;      
Vector2 ballVelocity;       
float ballScale;           
float ballSpeed;            
float ballWidth;           
float ballHeight;           
bool isRunning;             
void GenerateRandomDirection();
public:
//constructor y dsetructor
Game();
~Game();
//metodos publicos
void Init();               
void Run();                 
void HandleEvents();        
void UpdateBall(float deltaTime); 
void DrawGame();            
bool IsRunning() const;
};
#endif