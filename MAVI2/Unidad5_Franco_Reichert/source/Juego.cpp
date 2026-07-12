#include "Juego.h"
#include <cmath>
//gravedad en m/s^2
static const float GRAVEDAD = 9.8f;
//duracion de EventDetected: 75 cuadros = 1.25 s a 60 fps
static const int CUADROS_EVENTO = 75;
Juego::Juego()
    : datosSuelo(TipoObjeto::Suelo)
{
    colorFondo = { 28, 32, 44, 255 };
    mundo = nullptr;  proyectil = nullptr;  objetivo = nullptr;
    zona = nullptr;
    estado = GameState::Waiting;
    acierto = false;  pasoPorZona = false;  cuadrosRespuesta = 0;
    aciertos = 0;  disparos = 0;
    anguloGrados = 45.0f;  potencia = 18.0f;
}
Juego::~Juego()
{
    LiberarEscena();
}
//loop principal: input -> simulacion y logica -> dibujo
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
        "TP Final Unidad 5 - Franco Reichert");
    SetTargetFPS(60);
    ConstruirEscena();
}
void Juego::ConstruirEscena()
{
    mundo = new b2World(b2Vec2(0.0f, GRAVEDAD));
    mundo->SetContactListener(&listener); //registro del observador
    //suelo, si el proyectil lo toca el tiro fallo
    b2BodyDef sueloDef;
    sueloDef.type = b2_staticBody;
    sueloDef.position.Set(Pix2M(anchoVentana / 2.0f), Pix2M(altoVentana - 15.0f));
    sueloDef.userData.pointer = reinterpret_cast<uintptr_t>(&datosSuelo);
    b2Body* suelo = mundo->CreateBody(&sueloDef);
    b2PolygonShape sueloShape;
    sueloShape.SetAsBox(Pix2M(anchoVentana / 2.0f), Pix2M(15.0f));
    suelo->CreateFixture(&sueloShape, 0.0f);
    zona = new ZonaSensor(mundo, 500.0f, 300.0f, 90.0f, 360.0f);
    objetivo = new Objetivo(mundo, 880.0f, 430.0f, 36.0f, 180.0f);
    estado = GameState::Waiting; //el sistema inicia en un estado definido
    listener.Reiniciar();
}
void Juego::LiberarEscena()
{
    delete proyectil; proyectil = nullptr;
    delete objetivo;  objetivo = nullptr;
    delete zona;      zona = nullptr;
    delete mundo;     mundo = nullptr; //libera los b2Body restantes (suelo incluido)
}
//transicion Finished -> Waiting, el marcador persiste
void Juego::ReiniciarRonda()
{
    delete proyectil; proyectil = nullptr;
    objetivo->Reiniciar();
    zona->EstablecerActiva(false);
    listener.Reiniciar();
    acierto = false;
    pasoPorZona = false;
    estado = GameState::Waiting;
}
//la entrada se filtra por estado: cada estado habilita sus acciones
void Juego::ProcesarInput()
{
    if (estado == GameState::Waiting)
    {
        if (IsKeyDown(KEY_UP))    anguloGrados += 60.0f * GetFrameTime();
        if (IsKeyDown(KEY_DOWN))  anguloGrados -= 60.0f * GetFrameTime();
        anguloGrados = fmaxf(anguloMin, fminf(anguloMax, anguloGrados));
        if (IsKeyDown(KEY_RIGHT)) potencia += 12.0f * GetFrameTime();
        if (IsKeyDown(KEY_LEFT))  potencia -= 12.0f * GetFrameTime();
        potencia = fmaxf(potenciaMin, fminf(potenciaMax, potencia));
        if (IsKeyPressed(KEY_SPACE)) DispararProyectil();
    }
    else if (estado == GameState::Finished)
    {
        //el reinicio solo esta permitido en Finished
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
    estado = GameState::Running; //accion del usuario -> Running
}
//unico punto donde avanza la simulacion, el estado decide cuando
void Juego::PasoFisico()
{
    mundo->Step(1.0f / 60.0f, 8, 3);
}
//flujo de la unidad: simulacion -> deteccion -> interpretacion -> reaccion
void Juego::Actualizar()
{
    switch (estado)
    {
    case GameState::Waiting:
        //la simulacion no avanza mientras se apunta
        break;
    case GameState::Running:
        PasoFisico();
        //el sensor no frena el tiro, solo lo registra (deteccion logica)
        zona->EstablecerActiva(listener.dentroDeZona);
        if (listener.dentroDeZona) pasoPorZona = true;
        //la logica interpreta las banderas que registro el listener
        if (listener.impactoObjetivo)
        {
            acierto = true;
            objetivo->MarcarImpactado();
            cuadrosRespuesta = CUADROS_EVENTO;
            estado = GameState::EventDetected; //evento -> cambio de estado
        }
        else if (listener.tocoSuelo || ProyectilFueraDePantalla())
        {
            acierto = false;
            cuadrosRespuesta = CUADROS_EVENTO;
            estado = GameState::EventDetected; //evento -> cambio de estado
        }
        break;
    case GameState::EventDetected:
        //la fisica sigue para ver el rebote, sin interpretar eventos nuevos
        PasoFisico();
        zona->EstablecerActiva(listener.dentroDeZona);
        cuadrosRespuesta--;
        if (cuadrosRespuesta <= 0)
        {
            if (acierto) aciertos++;
            estado = GameState::Finished;
        }
        break;
    case GameState::Finished:
        //sin simulacion activa, espera R
        break;
    }
}
bool Juego::ProyectilFueraDePantalla() const
{
    if (!proyectil) return false;
    b2Vec2 p = proyectil->ObtenerPosicionPix();
    return (p.x < -50 || p.x > anchoVentana + 50 || p.y > altoVentana + 50);
}
void Juego::Dibujar()
{
    BeginDrawing();
    ClearBackground(colorFondo);
    DrawRectangle(0, altoVentana - 30, anchoVentana, 30, Fade(DARKGREEN, 0.7f));
    if (estado == GameState::Waiting) DibujarTrayectoria();
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
    case GameState::Waiting:       return "WAITING - listo para disparar";
    case GameState::Running:       return "RUNNING - proyectil en movimiento";
    case GameState::EventDetected: return "EVENT DETECTED - procesando evento";
    case GameState::Finished:      return "FINISHED - resultado mostrado";
    }
    return "-";
}
//feedback visual asociado al estado
void Juego::DibujarUI()
{
    DrawRectangle(10, 10, 360, 92, Fade(BLACK, 0.55f));
    DrawText("estado del sistema", 22, 18, 16, RAYWHITE);
    Color col = SKYBLUE;
    if (estado == GameState::Running)       col = GOLD;
    if (estado == GameState::EventDetected) col = ORANGE;
    if (estado == GameState::Finished)      col = GREEN;
    DrawText(NombreEstado(), 22, 44, 14, col);
    DrawText(TextFormat("aciertos: %d   disparos: %d", aciertos, disparos),
        22, 64, 14, RAYWHITE);
    DrawText(TextFormat("zona activada: %d veces", listener.activacionesZona),
        22, 82, 14, listener.dentroDeZona ? SKYBLUE : GRAY);
    //panel de controles
    int pw = 360, ph = 84;
    int px = anchoVentana - pw - 10;
    int py = altoVentana - ph - 10;
    DrawRectangle(px, py, pw, ph, Fade(BLACK, 0.6f));
    DrawText("controles:", px + 12, py + 8, 14, YELLOW);
    DrawText(TextFormat("flechas arr/aba -> angulo  (%.0f)", anguloGrados),
        px + 12, py + 28, 12, RAYWHITE);
    DrawText(TextFormat("flechas izq/der -> potencia (%.0f m/s)", potencia),
        px + 12, py + 44, 12, RAYWHITE);
    DrawText("espacio = disparar    R = reiniciar (solo en FINISHED)",
        px + 12, py + 60, 12, RAYWHITE);
    //mensaje central segun el estado
    if (estado == GameState::EventDetected)
    {
        const char* msg = acierto ? "IMPACTO DETECTADO!" : "FALLO DETECTADO";
        int tw = MeasureText(msg, 40);
        DrawText(msg, anchoVentana / 2 - tw / 2, 20, 40, acierto ? GREEN : RED);
    }
    else if (estado == GameState::Finished)
    {
        const char* msg = acierto ? "RESULTADO: ACIERTO" : "RESULTADO: FALLO";
        int tw = MeasureText(msg, 40);
        DrawText(msg, anchoVentana / 2 - tw / 2, 20, 40, acierto ? GREEN : RED);
        const char* sub = TextFormat("paso por la zona: %s   |   presiona R para reiniciar",
            pasoPorZona ? "SI" : "NO");
        int tw2 = MeasureText(sub, 20);
        DrawText(sub, anchoVentana / 2 - tw2 / 2, 68, 20, RAYWHITE);
    }
}
bool Juego::DebeCerrar()
{
    return WindowShouldClose();
}