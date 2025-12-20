#include "EnemigoQueRebota.h"
EnemigoQueRebota::EnemigoQueRebota() : Enemigo()
{
    gravedad = 400.0f;
    factorRebote = 0.7f;   
    nivelSuelo = 550.0f;   
    contadorRebotes = 0;
    maxRebotes = 5;
    puntos = 20;            
}
EnemigoQueRebota::EnemigoQueRebota(Vector2 pos, Vector2 velInicial, float grav, float suelo)
    : Enemigo(pos, velInicial, 28.0f, 20)
{
    gravedad = grav;
    factorRebote = 0.7f;
    nivelSuelo = suelo;
    contadorRebotes = 0;
    maxRebotes = 5;
}
EnemigoQueRebota::~EnemigoQueRebota()
{
    DescargarSprite();
}
void EnemigoQueRebota::Actualizar(float deltaTiempo)
{
    if (!activo) return;
    posicion.x += velocidad.x * deltaTiempo;
    velocidad.y += gravedad * deltaTiempo;
    posicion.y += velocidad.y * deltaTiempo;
    if (posicion.y + radio >= nivelSuelo)
    {
        posicion.y = nivelSuelo - radio;
        velocidad.y *= -factorRebote;
        velocidad.x *= 0.95f;
        contadorRebotes++;
        if (contadorRebotes >= maxRebotes)
        {
            activo = false;
        }
    }
}
void EnemigoQueRebota::Dibujar()
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