#include "raylib.h"
int main(void){
const int screenWidth = 800;
const int screenHeight = 600;
InitWindow(screenWidth, screenHeight, "Franco Andrés Reichert");
SetTargetFPS(60);
//Aqui cargo la textura
Texture2D textura = LoadTexture("assets/sprite.png");
//Aplico filtro bilineal
SetTextureFilter(textura, TEXTURE_FILTER_BILINEAR);
//Definicion de propiedades sprite 1
Vector2 posicionSprite1 = { 100.0f, 200.0f };
float escalaSprite1 = 0.5f;
float rotacionSprite1 = 0.0f;
Color colorSprite1 = RED;
//Propiedades sprite 2 (con la misma textura)
Vector2 posicionSprite2 = { 500.0f, 200.0f };
float escalaSprite2 = 0.3f;
float rotacionSprite2 = 45.0f;
Color colorSprite2 = { 255, 150, 150, 255 };
//variables del movimiento
float velocidadMovimiento = 100.0f;
float velocidadSprite2 = 50.0f;
float direccionSprite2 = 1.0f;
bool propiedadesIntercambiadas = false;
//Bucle principal del juego
while (!WindowShouldClose()){
//Movimiento sprite 1 con teclado
if (IsKeyDown(KEY_RIGHT))
posicionSprite1.x += velocidadMovimiento * GetFrameTime();
if (IsKeyDown(KEY_LEFT))
posicionSprite1.x -= velocidadMovimiento * GetFrameTime();
if (IsKeyDown(KEY_DOWN))
posicionSprite1.y += velocidadMovimiento * GetFrameTime();
if (IsKeyDown(KEY_UP))
posicionSprite1.y -= velocidadMovimiento * GetFrameTime();
//Movimiento automatico del Sprite 2 (horizontal con rebote)
posicionSprite2.x += velocidadSprite2 * direccionSprite2 * GetFrameTime();
//Rebote en los bordes (usando valores fijos estimados). Un poco de reciclado de la entrega anterior asi no se pierden los batilogos.
if (posicionSprite2.x > 700 || posicionSprite2.x < 0){
direccionSprite2 = -direccionSprite2;
}
//Intercambio de propiedades
if (IsKeyPressed(KEY_SPACE)){
propiedadesIntercambiadas = !propiedadesIntercambiadas;
if (propiedadesIntercambiadas){
//Intercambio de escalas
float tempEscala = escalaSprite1;
escalaSprite1 = escalaSprite2;
escalaSprite2 = tempEscala;
//Intercambio de rotaciones
float tempRotacion = rotacionSprite1;
rotacionSprite1 = rotacionSprite2;
rotacionSprite2 = tempRotacion;
//Intercambio de colores
Color tempColor = colorSprite1;
colorSprite1 = colorSprite2;
colorSprite2 = tempColor;
}
else{
//Restauracion de los valores originales
escalaSprite1 = 0.5f;
escalaSprite2 = 0.3f;
rotacionSprite1 = 0.0f;
rotacionSprite2 = 45.0f;
colorSprite1 = RED;
colorSprite2 = { 255, 150, 150, 255 };
}
}
BeginDrawing();
ClearBackground(DARKBLUE);
//Dibujo sprite 1 con DrawTextureEx
DrawTextureEx(textura, posicionSprite1, rotacionSprite1, escalaSprite1, colorSprite1);
//Dibujo sprite 2 con DrawTextureEx
DrawTextureEx(textura, posicionSprite2, rotacionSprite2, escalaSprite2, colorSprite2);
//Mostrar info del sprite 1
DrawText("SPRITE 1:", 10, 10, 20, YELLOW);
DrawText(TextFormat("Posicion: (%.1f, %.1f)", posicionSprite1.x, posicionSprite1.y), 10, 35, 15, WHITE);
DrawText(TextFormat("Escala: %.2f", escalaSprite1), 10, 55, 15, WHITE);
DrawText(TextFormat("Rotacion: %.1f grados", rotacionSprite1), 10, 75, 15, WHITE);
DrawText(TextFormat("Color: RGB(%d, %d, %d)", colorSprite1.r, colorSprite1.g, colorSprite1.b), 10, 95, 15, WHITE);
//Mostrar info del sprite 2
DrawText("SPRITE 2:", 10, 130, 20, YELLOW);
DrawText(TextFormat("Posicion: (%.1f, %.1f)", posicionSprite2.x, posicionSprite2.y), 10, 155, 15, WHITE);
DrawText(TextFormat("Escala: %.2f", escalaSprite2), 10, 175, 15, WHITE);
DrawText(TextFormat("Rotacion: %.1f grados", rotacionSprite2), 10, 195, 15, WHITE);
DrawText(TextFormat("Color: RGB(%d, %d, %d)", colorSprite2.r, colorSprite2.g, colorSprite2.b), 10, 215, 15, WHITE);
//Instrucciones del juego 
DrawText("Controles:", 10, 260, 18, LIME);
DrawText("Flechas: Mover sprite 1", 10, 285, 15, WHITE);
DrawText("Espacio: Intercambiar propiedades", 10, 305, 15, WHITE);
DrawText("Escape: Salir", 10, 325, 15, WHITE);
if (propiedadesIntercambiadas){
DrawText("¡Propiedades INTERCAMBIADAS!", screenWidth / 2 - 150, screenHeight - 40, 20, RED);
}
EndDrawing();
}
CloseWindow();
return 0;
}