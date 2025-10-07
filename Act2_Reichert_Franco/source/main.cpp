#ifndef NDEBUG
#include <vld.h>
#endif
#include "raylib.h"
int main(void) {
const int anchoVentana = 1024;
const int altoVentana = 768;
InitWindow(anchoVentana, altoVentana, "Franco Andrés Reichert");
SetTargetFPS(60);
Color fondo = GREEN;
Color texto = BLACK;
Color Line = RED;
Vector2 posicionEsfera = { 512.0f, 384.0f };
Vector2 velocidadEsfera = { 200.0f, 150.0f };
float radioEsfera = 30.0f;
Color colorEsfera = { 255, 0, 0, 180 };
Color colores[4] = {
{255, 100, 100, 180},  
{100, 255, 100, 180}, 
{100, 100, 255, 180},  
{255, 255, 100, 180}   
};
int indiceColor = 0;
int rebotes = 0;
while (!WindowShouldClose()) {
float deltaTime = GetFrameTime();
posicionEsfera.x += velocidadEsfera.x * deltaTime;
posicionEsfera.y += velocidadEsfera.y * deltaTime;
if (posicionEsfera.x + radioEsfera >= anchoVentana) {
posicionEsfera.x = anchoVentana - radioEsfera;
velocidadEsfera.x = -velocidadEsfera.x;
indiceColor = (indiceColor + 1) % 4;
colorEsfera = colores[indiceColor];
rebotes++;
}
if (posicionEsfera.x - radioEsfera <= 0) {
posicionEsfera.x = radioEsfera;
velocidadEsfera.x = -velocidadEsfera.x;
indiceColor = (indiceColor + 1) % 4;
colorEsfera = colores[indiceColor];
rebotes++;
}
if (posicionEsfera.y + radioEsfera >= altoVentana) {
posicionEsfera.y = altoVentana - radioEsfera;
velocidadEsfera.y = -velocidadEsfera.y;
indiceColor = (indiceColor + 1) % 4;
colorEsfera = colores[indiceColor];
rebotes++;
}
if (posicionEsfera.y - radioEsfera <= 0) {
posicionEsfera.y = radioEsfera;
velocidadEsfera.y = -velocidadEsfera.y;
indiceColor = (indiceColor + 1) % 4;
colorEsfera = colores[indiceColor];
rebotes++;
}
if (IsKeyPressed(KEY_SPACE)) {
Line = BLACK;
fondo = WHITE;
texto = BLUE;
}
BeginDrawing();
ClearBackground(fondo);
DrawText("Franco Andrés Reichert", 255, 210, 30, texto);
DrawLine(255, 245, 630, 245, Line);
DrawCircle((int)posicionEsfera.x, (int)posicionEsfera.y, radioEsfera, colorEsfera);
DrawText(TextFormat("Resolucion: %dx%d", anchoVentana, altoVentana), 10, 10, 20, texto);
DrawText(TextFormat("Posicion: (%.1f, %.1f)", posicionEsfera.x, posicionEsfera.y), 10, 35, 20, texto);
DrawText(TextFormat("Rebotes: %d", rebotes), 10, 60, 20, texto);
EndDrawing();
}
CloseWindow();
return 0;
}