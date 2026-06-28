#include "Juego.h"
#include <cmath>
//fravedad en m/s^2
static const float GRAVEDAD = 9.8f;
Juego::Juego()
    : datosSuelo(TipoObjeto::Suelo)
{
    colorFondo = { 28, 32, 44, 255 };
    mundo = nullptr;  proyectil = nullptr;  objetivo = nullptr;
    zona = nullptr;   suelo = nullptr;
    estado = EstadoJuego::Esperando;
    aciertos = 0;  disparos = 0;
    anguloGrados = 45.0f;  potencia = 18.0f;
}
Juego::~Juego()
{
    LiberarEscena();
}
void Juego::Ejecutar()
{
    Inicializar();
    while (!DebeCerrar())
    {
        ProcesarInput();
        Actualizar();
        Dibujar();
    }
    CloseWindow();
}
void Juego::Inicializar()
{
    InitWindow(anchoVentana, altoVentana,
        "TP Unidad 4 - Franco Reichert");
    SetTargetFPS(60);
    ConstruirEscena();
}
void Juego::ConstruirEscena()
{
    mundo = new b2World(b2Vec2(0.0f, GRAVEDAD));
    mundo->SetContactListener(&listener); // registro del observador
    //suelo, si el proyectil lo toca, el tiro fallo
    b2BodyDef sueloDef;
    sueloDef.type = b2_staticBody;
    sueloDef.position.Set(Pix2M(anchoVentana / 2.0f), Pix2M(altoVentana - 15.0f));
    sueloDef.userData.pointer = reinterpret_cast<uintptr_t>(&datosSuelo);
    suelo = mundo->CreateBody(&sueloDef);
    b2PolygonShape sueloShape;
    sueloShape.SetAsBox(Pix2M(anchoVentana / 2.0f), Pix2M(15.0f));
    suelo->CreateFixture(&sueloShape, 0.0f);
    zona = new ZonaSensor(mundo, 500.0f, 300.0f, 90.0f, 360.0f);
    objetivo = new Objetivo(mundo, 880.0f, 430.0f, 36.0f, 180.0f);
    estado = EstadoJuego::Esperando;
    listener.Reiniciar();
}
void Juego::LiberarEscena()
{
    delete proyectil; proyectil = nullptr;
    delete objetivo;  objetivo = nullptr;
    delete zona;      zona = nullptr;
    delete mundo;     mundo = nullptr; //libera todos los b2Body
    suelo = nullptr;
}
void Juego::ReiniciarRonda()
{
    delete proyectil; proyectil = nullptr;
    objetivo->Reiniciar();
    zona->EstablecerActiva(false);
    listener.Reiniciar();
    estado = EstadoJuego::Esperando;
}
void Juego::ProcesarInput()
{
    if (estado == EstadoJuego::Esperando)
    {
        if (IsKeyDown(KEY_UP))    anguloGrados += 60.0f * GetFrameTime();
        if (IsKeyDown(KEY_DOWN))  anguloGrados -= 60.0f * GetFrameTime();
        anguloGrados = fmaxf(anguloMin, fminf(anguloMax, anguloGrados));
        if (IsKeyDown(KEY_RIGHT)) potencia += 12.0f * GetFrameTime();
        if (IsKeyDown(KEY_LEFT))  potencia -= 12.0f * GetFrameTime();
        potencia = fmaxf(potenciaMin, fminf(potenciaMax, potencia));
        if (IsKeyPressed(KEY_SPACE)) DispararProyectil();
    }
    else if (estado == EstadoJuego::EventoDetectado ||
        estado == EstadoJuego::Finalizado)
    {
        if (IsKeyPressed(KEY_R)) ReiniciarRonda();
    }
}
void Juego::DispararProyectil()
{
    float rad = anguloGrados * DEG2RAD;
    float vx = potencia * cosf(rad);
    float vy = -potencia * sinf(rad);
    proyectil = new Proyectil(mundo, xCanon, yCanon, radioProyectil, vx, vy);
    disparos++;
    estado = EstadoJuego::Disparado;
}
void Juego::Actualizar()
{
    if (mundo) mundo->Step(1.0f / 60.0f, 8, 3);
    zona->EstablecerActiva(listener.dentroDeZona);
    if (estado == EstadoJuego::Disparado && proyectil)
    {
        if (listener.impactoObjetivo) //evento esperado
        {
            objetivo->MarcarImpactado();
            aciertos++;
            estado = EstadoJuego::EventoDetectado;
        }
        else if (listener.tocoSuelo) //fallo
        {
            estado = EstadoJuego::Finalizado;
        }
        else
        {
            b2Vec2 p = proyectil->ObtenerPosicionPix(); //se fue de pantalla
            if (p.x < -50 || p.x > anchoVentana + 50 || p.y > altoVentana + 50)
                estado = EstadoJuego::Finalizado;
        }
    }
}
void Juego::Dibujar()
{
    BeginDrawing();
    ClearBackground(colorFondo);
    DrawRectangle(0, altoVentana - 30, anchoVentana, 30, Fade(DARKGREEN, 0.7f));
    if (estado == EstadoJuego::Esperando) DibujarTrayectoria();
    zona->Dibujar();
    objetivo->Dibujar();
    DibujarCanon();
    if (proyectil) proyectil->Dibujar();
    DibujarUI();
    EndDrawing();
}
void Juego::DibujarCanon()
{
    //soporte hasta el piso
    DrawRectangle((int)xCanon - 6, (int)yCanon, 12,
        (altoVentana - 30) - (int)yCanon, DARKGRAY);
    DrawCircle((int)xCanon, (int)yCanon, 16, GRAY);
    float rad = anguloGrados * DEG2RAD;
    float largo = 30.0f + (potencia / potenciaMax) * 35.0f;
    DrawLineEx({ xCanon, yCanon },
        { xCanon + cosf(rad) * largo, yCanon - sinf(rad) * largo },
        10.0f, MAROON);
}
//previsualiza el arco con las mismas ecuaciones de la fisica
void Juego::DibujarTrayectoria()
{
    float rad = anguloGrados * DEG2RAD;
    float vx = M2Pix(potencia * cosf(rad));
    float vy = M2Pix(-potencia * sinf(rad));
    float g = M2Pix(GRAVEDAD);
    for (int i = 1; i <= 30; i++)
    {
        float t = i * 0.04f;
        float x = xCanon + vx * t;
        float y = yCanon + vy * t + 0.5f * g * t * t;
        if (y > altoVentana || x > anchoVentana) break;
        DrawCircle((int)x, (int)y, 3, Fade(YELLOW, 0.55f));
    }
}
const char* Juego::NombreEstado() const
{
    switch (estado)
    {
    case EstadoJuego::Esperando:       return "esperando (apuntando)";
    case EstadoJuego::Disparado:       return "disparado (en vuelo)";
    case EstadoJuego::EventoDetectado: return "evento detectado (impacto)";
    case EstadoJuego::Finalizado:      return "finalizado (fallo)";
    }
    return "-";
}
void Juego::DibujarUI()
{
    DrawRectangle(10, 10, 360, 92, Fade(BLACK, 0.55f));
    DrawText("eventos y logica", 22, 18, 16, RAYWHITE);
    Color col = LIGHTGRAY;
    if (estado == EstadoJuego::EventoDetectado) col = GREEN;
    if (estado == EstadoJuego::Finalizado)      col = RED;
    if (estado == EstadoJuego::Disparado)       col = GOLD;
    DrawText(TextFormat("estado: %s", NombreEstado()), 22, 44, 14, col);
    DrawText(TextFormat("aciertos: %d   disparos: %d", aciertos, disparos),
        22, 64, 14, RAYWHITE);
    DrawText(TextFormat("zona activada: %d veces", listener.activacionesZona),
        22, 82, 14, listener.dentroDeZona ? SKYBLUE : GRAY);
    // Panel de control
    int pw = 360, ph = 84;
    int px = anchoVentana - pw - 10;
    int py = altoVentana - ph - 10;
    DrawRectangle(px, py, pw, ph, Fade(BLACK, 0.6f));
    DrawText("controles:", px + 12, py + 8, 14, YELLOW);
    DrawText(TextFormat("flecas arr/aba -> angulo  (%.0f)", anguloGrados),
        px + 12, py + 28, 12, RAYWHITE);
    DrawText(TextFormat("flechas izq/der -> potencia (%.0f m/s)", potencia),
        px + 12, py + 44, 12, RAYWHITE);
    DrawText("espacio = disparar    R = reiniciar", px + 12, py + 60, 12, RAYWHITE);
    if (estado == EstadoJuego::EventoDetectado ||
        estado == EstadoJuego::Finalizado)
    {
        bool acierto = (estado == EstadoJuego::EventoDetectado);
        const char* msg = acierto ? "DISTE EN EL BLANCO!" : "FALLASTE!";
        int tw = MeasureText(msg, 40);
        DrawText(msg, anchoVentana / 2 - tw / 2, 20, 40, acierto ? GREEN : RED);
        const char* sub = "presiona R para volver a tirar";
        int tw2 = MeasureText(sub, 20);
        DrawText(sub, anchoVentana / 2 - tw2 / 2, 68, 20, RAYWHITE);
    }
}
bool Juego::DebeCerrar()
{
    return WindowShouldClose();
}