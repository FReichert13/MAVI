#include "Jugador.h"
#include <cmath>
#define DEG2RAD (PI / 180.0f)
Jugador::Jugador()
{
    posicion = { 80.0f, 520.0f };
    angulo = 45.0f;            
    potencia = 400.0f;         
    anguloMinimo = 0.0f;
    anguloMaximo = 90.0f;
    potenciaMinima = 200.0f;
    potenciaMaxima = 800.0f;
    velocidadAngulo = 60.0f;    
    velocidadPotencia = 200.0f; 
    sprite = { 0 };
    escalaSprite = 1.0f;
}

Jugador::~Jugador()
{
    DescargarSprite();
}
void Jugador::Inicializar(Vector2 pos)
{
    posicion = pos;
    angulo = 45.0f;
    potencia = 400.0f;
}
void Jugador::CargarSprite(const char* ruta)
{
    sprite = LoadTexture(ruta);
    if (sprite.id != 0)
    {
        escalaSprite = 80.0f / (float)sprite.width;  //escalar a 80 px
    }
}
void Jugador::DescargarSprite()
{
    if (sprite.id != 0)
    {
        UnloadTexture(sprite);
        sprite = { 0 };
    }
}
void Jugador::ProcesarEntrada()
{
    float deltaTiempo = GetFrameTime();
    if (IsKeyDown(KEY_UP))
    {
        angulo += velocidadAngulo * deltaTiempo;
        if (angulo > anguloMaximo) angulo = anguloMaximo;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        angulo -= velocidadAngulo * deltaTiempo;
        if (angulo < anguloMinimo) angulo = anguloMinimo;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        potencia += velocidadPotencia * deltaTiempo;
        if (potencia > potenciaMaxima) potencia = potenciaMaxima;
    }
    if (IsKeyDown(KEY_LEFT))
    {
        potencia -= velocidadPotencia * deltaTiempo;
        if (potencia < potenciaMinima) potencia = potenciaMinima;
    }
    if (IsKeyPressed(KEY_SPACE))
    {
        Disparar();
    }
}
void Jugador::Disparar()
{
    for (int i = 0; i < MAX_PROYECTILES; i++)
    {
        if (!proyectiles[i].EstaActivo())
        {
            Vector2 posDisparo = {
                posicion.x + 40.0f * cosf(angulo * DEG2RAD),
                posicion.y - 40.0f * sinf(angulo * DEG2RAD)
            };

            proyectiles[i].Disparar(posDisparo, angulo, potencia);
            break;
        }
    }
}
void Jugador::Actualizar(float deltaTiempo)
{
    for (int i = 0; i < MAX_PROYECTILES; i++)
    {
        if (proyectiles[i].EstaActivo())
        {
            proyectiles[i].Actualizar(deltaTiempo);
            if (proyectiles[i].FueraDePantalla(GetScreenWidth(), GetScreenHeight()))
            {
                proyectiles[i].EstablecerActivo(false);
            }
        }
    }
}
void Jugador::Dibujar()
{
    //sprite proyectiles
    for (int i = 0; i < MAX_PROYECTILES; i++)
    {
        proyectiles[i].Dibujar();
    }
    //sprite jugador
    Rectangle rectOrigen = { 0.0f, 0.0f, (float)sprite.width, (float)sprite.height };
    Rectangle rectDestino = {
        posicion.x - (sprite.width * escalaSprite) / 2.0f,
        posicion.y - (sprite.height * escalaSprite) / 2.0f,
        sprite.width * escalaSprite,
        sprite.height * escalaSprite
    };
    Vector2 origen = { 0.0f, 0.0f };
    DrawTexturePro(sprite, rectOrigen, rectDestino, origen, 0.0f, WHITE);
    DibujarIndicadorApuntado();
}
void Jugador::DibujarIndicadorApuntado()
{
    float longitudMira = 80.0f;
    Vector2 finMira = {
        posicion.x + longitudMira * cosf(angulo * DEG2RAD),
        posicion.y - longitudMira * sinf(angulo * DEG2RAD)
    };
    DrawLineEx(posicion, finMira, 3.0f, Fade(WHITE, 0.7f));
    //puntos de trayectoria aprox.
    float simPotencia = potencia;
    float simAngulo = angulo * DEG2RAD;
    Vector2 simPos = posicion;
    Vector2 simVel = { simPotencia * cosf(simAngulo), -simPotencia * sinf(simAngulo) };
    float simGravedad = 500.0f;
    float dt = 0.05f;

    for (int i = 0; i < 30; i++)
    {
        simVel.y += simGravedad * dt;
        simPos.x += simVel.x * dt;
        simPos.y += simVel.y * dt;

        if (simPos.y > GetScreenHeight() || simPos.x > GetScreenWidth())
            break;
        float alfa = 1.0f - (float)i / 30.0f;
        DrawCircleV(simPos, 3.0f, Fade(YELLOW, alfa * 0.5f));
    }
}
//getters
Vector2 Jugador::ObtenerPosicion() const
{
    return posicion;
}
float Jugador::ObtenerAngulo() const
{
    return angulo;
}
float Jugador::ObtenerPotencia() const
{
    return potencia;
}
Proyectil* Jugador::ObtenerProyectiles()
{
    return proyectiles;
}
int Jugador::ObtenerMaxProyectiles() const
{
    return MAX_PROYECTILES;
}