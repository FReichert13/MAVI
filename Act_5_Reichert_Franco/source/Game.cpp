#include "Game.h"
#include <cstdlib>  
#include <ctime>    
#include <cmath>   
Game::Game(){
ballScale = 0.5f;
ballSpeed = INITIAL_SPEED;
ballWidth = 0.0f;
ballHeight = 0.0f;
isRunning = true;
ballPosition = { 0.0f, 0.0f };
ballVelocity = { 0.0f, 0.0f };
}
Game::~Game(){
UnloadTexture(ballTexture);
CloseWindow();
}
void Game::Init(){
InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tenis por Franco Reichert");
SetTargetFPS(60);
srand(static_cast<unsigned int>(time(nullptr))); //nros aleatorios
ballTexture = LoadTexture("source/Sprites/Sprite1.png");
if (ballTexture.id == 0){
ballTexture = LoadTexture("Sprites/Sprite1.png");
}
SetTextureFilter(ballTexture, TEXTURE_FILTER_BILINEAR);
ballWidth = ballTexture.width * ballScale;
ballHeight = ballTexture.height * ballScale;
//defino la pos inicial = centro de la pantalla
ballPosition.x = (SCREEN_WIDTH - ballWidth) / 2.0f;
ballPosition.y = (SCREEN_HEIGHT - ballHeight) / 2.0f;
//dir aleatoria cuando empieza
GenerateRandomDirection();
}
void Game::Run(){
while (!WindowShouldClose() && isRunning){
float deltaTime = GetFrameTime();
HandleEvents();
UpdateBall(deltaTime);
BeginDrawing();
DrawGame();
EndDrawing();
}
}
void Game::HandleEvents(){
//aumento de velocidad
if (IsKeyPressed(KEY_UP)){
ballSpeed += SPEED_INCREMENT;
if (ballSpeed > MAX_SPEED){
ballSpeed = MAX_SPEED;
}
float currentMagnitude = sqrtf(ballVelocity.x * ballVelocity.x +
ballVelocity.y * ballVelocity.y);
if (currentMagnitude > 0){
Vector2 direction;
direction.x = ballVelocity.x / currentMagnitude;
direction.y = ballVelocity.y / currentMagnitude;
ballVelocity.x = direction.x * ballSpeed;
ballVelocity.y = direction.y * ballSpeed;
}
}
//dismunucion de velocidad
if (IsKeyPressed(KEY_DOWN)){
ballSpeed -= SPEED_INCREMENT;
if (ballSpeed < MIN_SPEED){
ballSpeed = MIN_SPEED;
}
float currentMagnitude = sqrtf(ballVelocity.x * ballVelocity.x +
ballVelocity.y * ballVelocity.y);
if (currentMagnitude > 0){
Vector2 direction;
direction.x = ballVelocity.x / currentMagnitude;
direction.y = ballVelocity.y / currentMagnitude;
ballVelocity.x = direction.x * ballSpeed;
ballVelocity.y = direction.y * ballSpeed;
}
}
//reinicio
if (IsKeyPressed(KEY_R)){
ballPosition.x = (SCREEN_WIDTH - ballWidth) / 2.0f;
ballPosition.y = (SCREEN_HEIGHT - ballHeight) / 2.0f;
GenerateRandomDirection();
}
//salir del juego (esc)
if (IsKeyPressed(KEY_ESCAPE)){
isRunning = false;
}
}
void Game::UpdateBall(float deltaTime){
ballPosition.x += ballVelocity.x * deltaTime;
ballPosition.y += ballVelocity.y * deltaTime;
//detectar colisiones
if (ballPosition.x <= 0){
ballPosition.x = 0;
GenerateRandomDirection();
if (ballVelocity.x < 0) ballVelocity.x = -ballVelocity.x;
}
if (ballPosition.x + ballWidth >= SCREEN_WIDTH){
ballPosition.x = SCREEN_WIDTH - ballWidth;
GenerateRandomDirection();
if (ballVelocity.x > 0) ballVelocity.x = -ballVelocity.x;
}
if (ballPosition.y <= 0){
ballPosition.y = 0;
GenerateRandomDirection();
if (ballVelocity.y < 0) ballVelocity.y = -ballVelocity.y;
}
if (ballPosition.y + ballHeight >= SCREEN_HEIGHT){
ballPosition.y = SCREEN_HEIGHT - ballHeight;
 GenerateRandomDirection();
if (ballVelocity.y > 0) ballVelocity.y = -ballVelocity.y;
}
}
void Game::DrawGame()
{
ClearBackground(DARKBLUE);
Vector2 ballCenter;
ballCenter.x = ballPosition.x + (ballWidth / 2.0f);
ballCenter.y = ballPosition.y + (ballHeight / 2.0f);
Vector2 shadowPosition;
shadowPosition.x = ballCenter.x + 5.0f;
shadowPosition.y = ballCenter.y + 5.0f;
Color shadowColor = { 0, 0, 0, 80 }; 
DrawCircleV(shadowPosition, ballWidth / 2.0f, shadowColor);
DrawTextureEx(ballTexture, ballPosition, 0.0f, ballScale, WHITE);
DrawText("Wimbledon", 10, 10, 20, WHITE);
DrawText(TextFormat("Velocidad: %.0f px/s", ballSpeed), 10, 40, 18, LIGHTGRAY);
DrawText(TextFormat("FPS: %d", GetFPS()), SCREEN_WIDTH - 100, 10, 18, LIME);
//instrucciones
DrawText("Flecha arriba: + velocidad", 10, SCREEN_HEIGHT - 80, 16, YELLOW);
DrawText("Flecha abajo: - velocidad", 10, SCREEN_HEIGHT - 55, 16, YELLOW);
DrawText("R: Reiniciar", 10, SCREEN_HEIGHT - 30, 16, YELLOW);
float barWidth = 200.0f;
float barHeight = 15.0f;
float barX = SCREEN_WIDTH - barWidth - 20;
float barY = 40;
float fillPercent = (ballSpeed - MIN_SPEED) / (MAX_SPEED - MIN_SPEED);
DrawRectangle(static_cast<int>(barX), static_cast<int>(barY),
static_cast<int>(barWidth), static_cast<int>(barHeight), DARKGRAY);
DrawRectangle(static_cast<int>(barX), static_cast<int>(barY),
static_cast<int>(barWidth * fillPercent), static_cast<int>(barHeight), GREEN);
DrawText("Velocidad", static_cast<int>(barX), static_cast<int>(barY - 18), 14, WHITE);
}
//direcciones aleatoriasw
void Game::GenerateRandomDirection(){
float minAngle = 0.3f;  
float maxAngle = 1.27f;  
float angle = minAngle + (static_cast<float>(rand()) / RAND_MAX) * (maxAngle - minAngle);
float dirX = cosf(angle);
float dirY = sinf(angle);
if (rand() % 2 == 0) dirX = -dirX;
if (rand() % 2 == 0) dirY = -dirY;
ballVelocity.x = dirX * ballSpeed;
ballVelocity.y = dirY * ballSpeed;
}
bool Game::IsRunning() const{
return isRunning;
}