#include "EnemigoHorizontal.h"
#include <cmath>
EnemigoHorizontal::EnemigoHorizontal() : Enemigo()
{
    amplitud = 0.0f;
    frecuencia = 0.0f;
    tiempo = 0.0f;
    oscila = false;
    puntos = 10;      
}
EnemigoHorizontal::EnemigoHorizontal(Vector2 pos, float velocidadX, bool oscilar)
    : Enemigo(pos, { velocidadX, 0.0f }, 25.0f, 10)
{
    amplitud = 50.0f;       
    frecuencia = 2.0f;      
    tiempo = 0.0f;
    oscila = oscilar;
}
EnemigoHorizontal::~EnemigoHorizontal()
{
    DescargarSprite();
}
void EnemigoHorizontal::Actualizar(float deltaTiempo)
{
    if (!activo) return;
    posicion.x += velocidad.x * deltaTiempo;
    if (oscila)
    {
        tiempo += deltaTiempo;
        posicion.y += amplitud * sinf(frecuencia * tiempo) * deltaTiempo;
    }
}
void EnemigoHorizontal::Dibujar()
{
    if (!activo) return;
    Rectangle rectOrigen = { 0.0f, 0.0f, (float)sprite.width, (float)sprite.height };
    Rectangle rectDestino = {
        posicion.x - radio,
        posicion.y - radio,
        radio * 2.0f,
        radio * 2.0f
    };
    Vector2 origen = { 0.0f, 0.0f };
    if (velocidad.x < 0)
    {
        rectOrigen.width = -rectOrigen.width;
    }
    DrawTexturePro(sprite, rectOrigen, rectDestino, origen, 0.0f, WHITE);
}