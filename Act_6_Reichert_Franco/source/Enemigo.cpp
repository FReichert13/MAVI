#include "Enemigo.h"
Enemigo::Enemigo()
{
    posicion = { 0.0f, 0.0f };
    velocidad = { 0.0f, 0.0f };
    radio = 20.0f;
    activo = true;
    puntos = 10;
    sprite = { 0 };
}
Enemigo::Enemigo(Vector2 pos, Vector2 vel, float rad, int pts)
{
    posicion = pos;
    velocidad = vel;
    radio = rad;
    activo = true;
    puntos = pts;
    sprite = { 0 };
}
Enemigo::~Enemigo()
{

}
void Enemigo::CargarSprite(const char* ruta)
{
    sprite = LoadTexture(ruta);
}
void Enemigo::DescargarSprite()
{
    if (sprite.id != 0)
    {
        UnloadTexture(sprite);
        sprite = { 0 };
    }
}
Vector2 Enemigo::ObtenerPosicion() const
{
    return posicion;
}
float Enemigo::ObtenerRadio() const
{
    return radio;
}
bool Enemigo::EstaActivo() const
{
    return activo;
}
int Enemigo::ObtenerPuntos() const
{
    return puntos;
}
void Enemigo::EstablecerActivo(bool valor)
{
    activo = valor;
}
bool Enemigo::FueraDePantalla(int anchoPantalla, int altoPantalla)
{
    return (posicion.x < -radio * 2 ||
        posicion.x > anchoPantalla + radio * 2 ||
        posicion.y < -radio * 2 ||
        posicion.y > altoPantalla + radio * 2);
}