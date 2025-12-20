#include "EnemigoQueCae.h"
EnemigoQueCae::EnemigoQueCae() : Enemigo()
{
    gravedad = 300.0f;   
    puntos = 15;         
}
EnemigoQueCae::EnemigoQueCae(Vector2 pos, float velocidadInicialY, float grav)
    : Enemigo(pos, { 0.0f, velocidadInicialY }, 30.0f, 15)
{
    gravedad = grav;
}
EnemigoQueCae::~EnemigoQueCae()
{
    DescargarSprite();
}
void EnemigoQueCae::Actualizar(float deltaTiempo)
{
    if (!activo) return;
    velocidad.y += gravedad * deltaTiempo;
    posicion.y += velocidad.y * deltaTiempo;
}
void EnemigoQueCae::Dibujar()
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
    DrawTexturePro(sprite, rectOrigen, rectDestino, origen, 0.0f, WHITE);
}