#include "Proyectil.h"
#include <cmath>
#define DEG2RAD (PI / 180.0f)
//implemento proyectil
//constructor
Proyectil::Proyectil()
{
    posicion = { 0.0f, 0.0f };
    velocidad = { 0.0f, 0.0f };
    gravedad = 500.0f;       //gravedad
    radio = 8.0f;
    activo = false;
    color = YELLOW;
    indiceEstela = 0;
    for (int i = 0; i < 10; i++)
    {
        posicionesEstela[i] = { -100.0f, -100.0f };
    }
}
//destructor
Proyectil::~Proyectil()
{
}
void Proyectil::Disparar(Vector2 posInicial, float angulo, float potencia)
{
    posicion = posInicial;
    float anguloRad = angulo * DEG2RAD;
    velocidad.x = potencia * cosf(anguloRad);
    velocidad.y = -potencia * sinf(anguloRad); 
    activo = true;
    indiceEstela = 0;
    for (int i = 0; i < 10; i++)
    {
        posicionesEstela[i] = posInicial;
    }
}
void Proyectil::Actualizar(float deltaTiempo)
{
    if (!activo) return;
    posicionesEstela[indiceEstela] = posicion;
    indiceEstela = (indiceEstela + 1) % 10;
    posicion.x += velocidad.x * deltaTiempo;
    velocidad.y += gravedad * deltaTiempo;
    posicion.y += velocidad.y * deltaTiempo;
}
void Proyectil::Dibujar()
{
    if (!activo) return;

    //dibujo estela con círculos que se desvanecen
    for (int i = 0; i < 10; i++)
    {
        int idx = (indiceEstela - i - 1 + 10) % 10;
        float alfa = 1.0f - (float)i / 10.0f;
        Color colorEstela = Fade(ORANGE, alfa * 0.5f);
        float radioEstela = radio * (1.0f - (float)i / 15.0f);
        DrawCircleV(posicionesEstela[idx], radioEstela, colorEstela);
    }
    //dibujo el proyectil principal
    DrawCircleV(posicion, radio, color);
    DrawCircleV(posicion, radio * 0.5f, WHITE); //brillo
}
//getters
Vector2 Proyectil::ObtenerPosicion() const
{
    return posicion;
}
float Proyectil::ObtenerRadio() const
{
    return radio;
}
bool Proyectil::EstaActivo() const
{
    return activo;
}
//setters
void Proyectil::EstablecerActivo(bool valor)
{
    activo = valor;
}
bool Proyectil::FueraDePantalla(int anchoPantalla, int altoPantalla)
{
    return (posicion.x < -radio ||
        posicion.x > anchoPantalla + radio ||
        posicion.y > altoPantalla + radio);
}