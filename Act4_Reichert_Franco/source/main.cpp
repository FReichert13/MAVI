#include "raylib.h"
#include <cmath>
int main(){
//medidas de pantalla
const int screenWidth = 800;
const int screenHeight = 600; 
InitWindow(screenWidth, screenHeight, "Franco Andrés Reichert");
SetTargetFPS(60);
//iniciar audio
InitAudioDevice();
//cargo el sonido desde un mp3 en mi pc    
Sound jumpSound = LoadSound("Assets/jump.mp3");
//cargo las imagenes 
Texture2D sprite1 = LoadTexture("Assets/Sprite1.png");
Texture2D sprite2 = LoadTexture("Assets/Sprite2.png");
Vector2 playerPosition = { 100.0f, 300.0f };
Vector2 initialPosition = { 100.0f, 300.0f }; //reinicio 
float playerSpeed = 5.0f;
//variables para el salto
bool isJumping = false;
float jumpHeight = 150.0f;        
float jumpStartY = playerPosition.y;
float jumpProgress = 0.0f;
float jumpSpeed = 0.05f;
bool showMessage = false;
bool lightOn = true;  //para ver si la luz esta encendidad 
Color backgroundColor = WHITE;  
//interruptor de luz
Vector2 switchPosition = { 650.0f, 200.0f };  
float switchRadius = 20.0f;
while (!WindowShouldClose()){
//contro de movimiento
if (IsKeyDown(KEY_RIGHT)){
playerPosition.x += playerSpeed;
if (playerPosition.x > screenWidth - (float)sprite1.width)
playerPosition.x = screenWidth - (float)sprite1.width;
}
if (IsKeyDown(KEY_LEFT)){
playerPosition.x -= playerSpeed;
if (playerPosition.x < 0)
playerPosition.x = 0;
}
//salto 
if (IsKeyPressed(KEY_SPACE) && !isJumping){
 isJumping = true;
jumpStartY = playerPosition.y;
jumpProgress = 0.0f;
//reproducir sonido al saltar
PlaySound(jumpSound);
}
if (isJumping){
jumpProgress += jumpSpeed;
if (jumpProgress <= 0.5f){
playerPosition.y = jumpStartY - (jumpHeight * (jumpProgress * 2));
}
else{
playerPosition.y = jumpStartY - (jumpHeight * (2 - jumpProgress * 2));
}
if (jumpProgress >= 1.0f){
isJumping = false;
playerPosition.y = jumpStartY; 
}
}
//reiniciar posicion inicial
if (IsKeyPressed(KEY_R)){
playerPosition = initialPosition;
isJumping = false;
}
//mostrar mensaje oculto
if (IsKeyPressed(KEY_M)){           
showMessage = !showMessage;
}
Vector2 mousePosition = GetMousePosition();
if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
float distanceX = mousePosition.x - switchPosition.x;
float distanceY = mousePosition.y - switchPosition.y;
float distance = sqrtf(distanceX * distanceX + distanceY * distanceY);
if (distance <= switchRadius){
lightOn = !lightOn;
if (lightOn)
backgroundColor = WHITE;//luz prendida
else
backgroundColor = BLACK;//luz apagada
}
}
BeginDrawing();        
ClearBackground(backgroundColor);
//cambios de sprite segun luz on o off
if (lightOn){
DrawTexture(sprite1, (int)playerPosition.x, (int)playerPosition.y, WHITE);
}
else{
DrawTexture(sprite2, (int)playerPosition.x, (int)playerPosition.y, WHITE);
}
DrawCircle((int)switchPosition.x, (int)switchPosition.y, switchRadius, RED);
DrawCircleLines((int)switchPosition.x, (int)switchPosition.y, switchRadius, DARKGRAY);
Color textColor = lightOn ? BLACK : WHITE;
DrawText("INTERRUPTOR", (int)switchPosition.x - 60, (int)switchPosition.y - 60, 20, textColor);
//ver coordenadas
DrawText(TextFormat("Posición X: %.0f", playerPosition.x), 10, 10, 20, textColor);
DrawText(TextFormat("Posición Y: %.0f", playerPosition.y), 10, 35, 20, textColor);
//ver mensaje
if (showMessage){
DrawRectangle(50, 100, 700, 70, Fade(DARKGRAY, 0.85f));
DrawText("Ten cuidado con apretar el interruptor y apagar la luz", 70, 110, 20, RED);
DrawText("A Arthas no le gusta la oscuridad", 70, 140, 20, RED);
}
EndDrawing();
}
UnloadTexture(sprite1);
UnloadTexture(sprite2);
UnloadSound(jumpSound);
CloseAudioDevice();
CloseWindow();
return 0;
}