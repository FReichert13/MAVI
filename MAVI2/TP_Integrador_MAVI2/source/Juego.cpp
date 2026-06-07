#include "Juego.h"
#include <cmath>
//dibuja texto con una sombra oscura detras para que se lea mejor
static void TextoSombra(const char* t, int x, int y, int size, Color c)
{
    DrawText(t, x + 2, y + 2, size, Fade(BLACK, 0.6f));
    DrawText(t, x, y, size, c);
}
Juego::Juego()
{
    colorFondo = { 120, 160, 200, 255 };
    centrosPuente = { 280.0f, 640.0f, 1000.0f };
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
    DescargarTexturas();
    DescargarSonidos();
    CloseAudioDevice();
    CloseWindow();
}
void Juego::Inicializar()
{
    InitWindow(anchoVentana, altoVentana, "TP Integrador - Franco Reichert");
    InitAudioDevice();
    SetTargetFPS(60);
    CargarTexturas();
    CargarSonidos();
    ConstruirEscena();
}
void Juego::CargarTexturas()
{
    texFondo = LoadTexture("Assets/Fondo.png");
    texSuelo = LoadTexture("Assets/Suelo.png");
    texCarrito = LoadTexture("Assets/Carrito.png");
    texMastil = LoadTexture("Assets/Mastil.png");
    texGancho = LoadTexture("Assets/Gancho.png");
    texCaja = LoadTexture("Assets/Caja.png");
    texTablon = LoadTexture("Assets/Tablon.png");
    texTorre = LoadTexture("Assets/Torre.png");
}
void Juego::DescargarTexturas()
{
    UnloadTexture(texFondo);
    UnloadTexture(texSuelo);
    UnloadTexture(texCarrito);
    UnloadTexture(texMastil);
    UnloadTexture(texGancho);
    UnloadTexture(texCaja);
    UnloadTexture(texTablon);
    UnloadTexture(texTorre);
}
void Juego::CargarSonidos()
{
    sndSoltar = LoadSound("Assets/Soltar.mp3");
    sndColocada = LoadSound("Assets/Colocada.mp3");
    sndPerdida = LoadSound("Assets/Perdida.mp3");
    sndGanar = LoadSound("Assets/Ganar.mp3");
    sndPerder = LoadSound("Assets/Perder.mp3");
    //sonido ambiente que se reproduce en loop (viento)
    vientoMusica = LoadMusicStream("Assets/Viento.mp3");
    vientoMusica.looping = true;
    SetMusicVolume(vientoMusica, 0.30f); //bajo, de fondo
}
void Juego::DescargarSonidos()
{
    UnloadSound(sndSoltar);
    UnloadSound(sndColocada);
    UnloadSound(sndPerdida);
    UnloadSound(sndGanar);
    UnloadSound(sndPerder);
    StopMusicStream(vientoMusica);
    UnloadMusicStream(vientoMusica);
}
//crea todo el escenario, se llama al iniciar y al reiniciar
void Juego::ConstruirEscena()
{
    b2Vec2 gravedad(0.0f, 400.0f);
    mundo = new b2World(gravedad);
    mundo->SetContactListener(&listener);
    estado = EstadoJuego::Jugando;
    cajasDisponibles = totalCajas;
    puentesCompletos = 0;
    perdidasPrevias = 0;
    CrearEscenarioEstatico();
    CrearPuentes();
    CrearGrua();
    GenerarCajaColgante();
    CambiarViento();
    PlayMusicStream(vientoMusica); //arranca (o reinicia) el viento de fondo 
}
void Juego::CrearEscenarioEstatico()
{
    //suelo: si una caja lo toca, se considera perdida 
    etiquetaSuelo = { TipoCuerpo::Suelo, nullptr };
    b2BodyDef sueloDef;
    sueloDef.type = b2_staticBody;
    sueloDef.position.Set(anchoVentana / 2.0f, 830.0f);
    sueloDef.userData.pointer = reinterpret_cast<uintptr_t>(&etiquetaSuelo);
    b2Body* suelo = mundo->CreateBody(&sueloDef);
    b2PolygonShape sueloShape;
    sueloShape.SetAsBox(anchoVentana / 2.0f, 20.0f);
    suelo->CreateFixture(&sueloShape, 0.0f);
    b2BodyDef railDef;
    railDef.type = b2_staticBody;
    railDef.position.Set(anchoVentana / 2.0f, yRail);
    rail = mundo->CreateBody(&railDef);
}
void Juego::CrearPuentes()
{
    puentes.clear();
    puenteListo.clear();
    dibujoTorres.clear();
    const float yDeck = puenteDeckY;     
    const float halfSpan = puenteHalfSpan; 
    const float anchoTorre = 16.0f;
    const float altoTorre = 100.0f; 
    const float yTorre = yDeck + altoTorre / 2.0f - 10.0f; 
    for (int i = 0; i < (int)centrosPuente.size(); i++)
    {
        float cx = centrosPuente[i];
        float xIzq = cx - halfSpan;
        float xDer = cx + halfSpan;
        //torres estaticas (apoyos del puente)
        b2BodyDef tdef; tdef.type = b2_staticBody;
        b2PolygonShape tshape; tshape.SetAsBox(anchoTorre / 2.0f, altoTorre / 2.0f);
        tdef.position.Set(xIzq, yTorre);
        b2Body* torreIzq = mundo->CreateBody(&tdef);
        torreIzq->CreateFixture(&tshape, 0.0f);
        tdef.position.Set(xDer, yTorre);
        b2Body* torreDer = mundo->CreateBody(&tdef);
        torreDer->CreateFixture(&tshape, 0.0f);
        dibujoTorres.push_back({ xIzq - anchoTorre / 2.0f, yTorre - altoTorre / 2.0f, anchoTorre, altoTorre });
        dibujoTorres.push_back({ xDer - anchoTorre / 2.0f, yTorre - altoTorre / 2.0f, anchoTorre, altoTorre });
        b2Vec2 anclaIzq(xIzq + anchoTorre / 2.0f, yDeck);
        b2Vec2 anclaDer(xDer - anchoTorre / 2.0f, yDeck);
        puentes.push_back(new PuenteCorto(mundo, torreIzq, anclaIzq, torreDer, anclaDer, 3));
        puenteListo.push_back(false);
    }
}
void Juego::CrearGrua()
{
    grua = new Grua(mundo, rail, anchoVentana / 2.0f, yRail);
}
void Juego::GenerarCajaColgante()
{
    b2Vec2 pos = grua->PuntoColgado();
    Caja* c = new Caja(mundo, pos.x, pos.y, 34.0f, 34.0f);
    c->estado = EstadoCaja::Colgando;
    cajas.push_back(c);
    grua->Enganchar(c->Cuerpo());
    cajaColgando = c;
    cajasDisponibles--;
}
void Juego::ProcesarInput()
{
    if (IsKeyPressed(KEY_R)) { Reiniciar(); return; }
    if (estado != EstadoJuego::Jugando) return;
    bool izq = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    bool der = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
    if (der && !izq)      grua->Mover(250.0f);
    else if (izq && !der) grua->Mover(-250.0f);
    else                  grua->Frenar();
    //soltar la caja colgante
    if (IsKeyPressed(KEY_SPACE) && cajaColgando != nullptr)
    {
        grua->Soltar();
        PlaySound(sndSoltar);
        cajaColgando->estado = EstadoCaja::Libre;
        cajaColgando->tiempoEstable = 0.0f;
        cajaColgando = nullptr;
        if (cajasDisponibles > 0)
            GenerarCajaColgante();
    }
}
void Juego::Actualizar()
{
    UpdateMusicStream(vientoMusica); //mantiene vivo el loop del viento
    //la fisica solo avanza mientras se esta jugando
    if (estado == EstadoJuego::Jugando)
    {
        const float dt = 1.0f / 60.0f;
        vientoTimer -= dt;
        if (vientoTimer <= 0.0f) CambiarViento();
        AplicarViento();
        mundo->Step(1.0f / 60.0f, 8, 3);
        ActualizarLogica();
    }
}
void Juego::CambiarViento()
{
    int mag = GetRandomValue((int)vientoMin, (int)vientoMax);
    int dir = (GetRandomValue(0, 1) == 0) ? -1 : 1; //izquierda o derecha
    vientoAccel = (float)(mag * dir);
    vientoTimer = (float)GetRandomValue(3, 7); //cambia cada 3 a 7 segundos
}
void Juego::AplicarViento()
{
    //la fuerza se calcula proporcional a la masa
    for (Caja* c : cajas)
    {
        if (c->estado == EstadoCaja::Colocada || c->estado == EstadoCaja::Perdida) continue;
        b2Body* b = c->Cuerpo();
        b->ApplyForceToCenter({ vientoAccel * b->GetMass(), 0.0f }, true);
    }
    //el puente tambien se hamaca un poco con el viento
    for (PuenteCorto* p : puentes)
        p->AplicarFuerzaHorizontal(vientoAccel * factorVientoPuente);
}
int Juego::PuenteDebajo(b2Vec2 p) const
{
    for (int i = 0; i < (int)centrosPuente.size(); i++)
        if (fabsf(p.x - centrosPuente[i]) < puenteHalfSpan + 14.0f &&
            p.y > puenteDeckY - 60.0f && p.y < puenteDeckY + 45.0f)
            return i;
    return -1;
}
void Juego::ActualizarLogica()
{
    const float dt = 1.0f / 60.0f;
    int perdidasAhora = 0;
    for (Caja* c : cajas)
        if (c->estado == EstadoCaja::Perdida) perdidasAhora++;
    if (perdidasAhora > perdidasPrevias) PlaySound(sndPerdida);
    perdidasPrevias = perdidasAhora;
    for (Caja* c : cajas)
    {
        if (c->estado != EstadoCaja::Libre) { c->progreso = 0.0f; continue; }
        b2Vec2 p = c->Posicion();
        float  w = fabsf(c->Cuerpo()->GetAngularVelocity());
        int idx = PuenteDebajo(p);
        if (idx >= 0 && !puenteListo[idx])
        {
            float dx = p.x - c->refX, dy = p.y - c->refY;
            bool estable = (dx * dx + dy * dy < tolPosicion * tolPosicion) && (w < umbralAng);
            if (estable) c->tiempoEstable += dt;
            else { c->tiempoEstable = 0.0f; c->refX = p.x; c->refY = p.y; }
            c->progreso = c->tiempoEstable / tiempoNecesario;
            if (c->tiempoEstable >= tiempoNecesario)
            {
                c->estado = EstadoCaja::Colocada;
                puenteListo[idx] = true;
                puentesCompletos++;
                c->progreso = 0.0f;
                PlaySound(sndColocada);
            }
        }
        else
        {
            c->tiempoEstable = 0.0f;
            c->progreso = 0.0f;
            c->refX = p.x;
            c->refY = p.y;
        }
    }
    //condicion de victoria
    if (puentesCompletos >= objetivo)
    {
        estado = EstadoJuego::Ganaste;
        StopMusicStream(vientoMusica);
        PlaySound(sndGanar);
        return;
    }
    //condicion de derrota
    if (cajaColgando == nullptr && cajasDisponibles == 0)
    {
        bool enJuego = false;
        for (Caja* c : cajas)
        {
            if (c->estado != EstadoCaja::Libre) continue;
            if (c->Cuerpo()->GetLinearVelocity().Length() > 25.0f) { enJuego = true; break; } //todavia se mueve
            int idx = PuenteDebajo(c->Posicion());
            if (idx >= 0 && !puenteListo[idx]) { enJuego = true; break; }                     //a punto de contar
        }
        if (!enJuego)
        {
            estado = EstadoJuego::Perdiste;
            StopMusicStream(vientoMusica);
            PlaySound(sndPerder);
        }
    }
}
void Juego::Dibujar()
{
    BeginDrawing();
    ClearBackground(colorFondo);
    //fondo estirado a toda la ventana
    {
        Rectangle src = { 0.0f, 0.0f, (float)texFondo.width, (float)texFondo.height };
        Rectangle dst = { 0.0f, 0.0f, (float)anchoVentana, (float)altoVentana };
        DrawTexturePro(texFondo, src, dst, { 0.0f, 0.0f }, 0.0f, WHITE);
    }
    //suelo
    {
        float alturaSuelo = (float)altoVentana - 810.0f;
        float tileW = texSuelo.width * (alturaSuelo / texSuelo.height);
        Rectangle src = { 0.0f, 0.0f, (float)texSuelo.width, (float)texSuelo.height };
        for (float x = 0.0f; x < anchoVentana; x += tileW)
        {
            Rectangle dst = { x, 810.0f, tileW, alturaSuelo };
            DrawTexturePro(texSuelo, src, dst, { 0.0f, 0.0f }, 0.0f, WHITE);
        }
    }
    //rail
    float yR = yRail;
    DrawLineEx({ 120.0f, yR }, { (float)anchoVentana - 120.0f, yR }, 5.0f, Fade(BLACK, 0.65f));
    for (int x = 130; x < anchoVentana - 120; x += 28)
        DrawLineEx({ (float)x, yR - 6.0f }, { (float)x, yR + 6.0f }, 3.0f, Fade(BLACK, 0.45f));
    //torres
    {
        Rectangle src = { 0.0f, 0.0f, (float)texTorre.width, (float)texTorre.height };
        for (const Rectangle& r : dibujoTorres)
        {
            float w = 30.0f;
            float h = r.height + 6.0f; //se hunden apenas en el pasto
            Rectangle dst = { r.x + r.width / 2.0f - w / 2.0f, r.y, w, h };
            DrawTexturePro(texTorre, src, dst, { 0.0f, 0.0f }, 0.0f, WHITE);
        }
    }
    //puentes
    for (int i = 0; i < (int)puentes.size(); i++)
        puentes[i]->Dibujar(puenteListo[i], texTablon);
    //grua y cajas
    grua->Dibujar(texCarrito, texMastil, texGancho);
    for (Caja* c : cajas) c->Dibujar(texCaja);
    DibujarUI();
    EndDrawing();
}
void Juego::DibujarUI()
{
    //panel del marcador
    int usables = cajasDisponibles + (cajaColgando != nullptr ? 1 : 0);
    int wInfo = MeasureText("puentes completos: 0 / 3", 18) + 18;
    DrawRectangle(8, 6, wInfo, 52, Fade(BLACK, 0.5f));
    TextoSombra(TextFormat("puentes completos: %d / %d", puentesCompletos, objetivo), 18, 12, 18, YELLOW);
    TextoSombra(TextFormat("cajas restantes: %d", usables), 18, 34, 16, RAYWHITE);
    //objetivo del juego
    const char* obj = "Deja apoyada una caja en cada puente";
    int wObj = MeasureText(obj, 18);
    int xObj = anchoVentana / 2 - wObj / 2;
    DrawRectangle(xObj - 12, 6, wObj + 24, 34, Fade(BLACK, 0.5f));
    TextoSombra(obj, xObj, 12, 18, RAYWHITE);
    int cxw = anchoVentana - 130;           
    DrawRectangle(cxw - 62, 8, 124, 52, Fade(BLACK, 0.5f));
    const char* etiqueta = "Viento";
    int wTxt = MeasureText(etiqueta, 18);
    TextoSombra(etiqueta, cxw - wTxt / 2, 14, 18, RAYWHITE);
    int yArr = 44;
    int dir = (vientoAccel >= 0.0f) ? 1 : -1;
    int largo = (int)(12.0f + fabsf(vientoAccel));
    int xIni = cxw - dir * largo / 2;              
    int xFin = cxw + dir * largo / 2;             
    DrawLineEx({ (float)xIni, (float)yArr }, { (float)xFin, (float)yArr }, 5.0f, SKYBLUE);
    DrawLineEx({ (float)xFin, (float)yArr }, { (float)(xFin - dir * 12), (float)(yArr - 7) }, 5.0f, SKYBLUE);
    DrawLineEx({ (float)xFin, (float)yArr }, { (float)(xFin - dir * 12), (float)(yArr + 7) }, 5.0f, SKYBLUE);
    //panel de control
    int yC = altoVentana - 96;
    DrawRectangle(10, yC, 330, 86, Fade(BLACK, 0.55f));
    DrawText("controles:", 20, yC + 6, 14, YELLOW);
    DrawText("A / D  (o flechas): mover la grua", 20, yC + 26, 12, RAYWHITE);
    DrawText("Espacio: soltar la caja", 20, yC + 44, 12, RAYWHITE);
    DrawText("R: reiniciar", 20, yC + 62, 12, RAYWHITE);
    //mensajes de fin de partida
    if (estado == EstadoJuego::Ganaste)
    {
        DrawRectangle(0, altoVentana / 2 - 50, anchoVentana, 100, Fade(DARKGREEN, 0.85f));
        const char* t = "Ganaste!   -   R para jugar de nuevo";
        int w = MeasureText(t, 30);
        DrawText(t, anchoVentana / 2 - w / 2, altoVentana / 2 - 15, 30, RAYWHITE);
    }
    else if (estado == EstadoJuego::Perdiste)
    {
        DrawRectangle(0, altoVentana / 2 - 50, anchoVentana, 100, Fade(MAROON, 0.85f));
        const char* t = "Te quedaste sin cajas   -   R para reiniciar";
        int w = MeasureText(t, 28);
        DrawText(t, anchoVentana / 2 - w / 2, altoVentana / 2 - 14, 28, RAYWHITE);
    }
}
void Juego::Reiniciar()
{
    LiberarEscena();
    ConstruirEscena();
}
void Juego::LiberarEscena()
{
    for (Caja* c : cajas) delete c;
    cajas.clear();
    cajaColgando = nullptr;
    for (PuenteCorto* p : puentes) delete p;
    puentes.clear();
    puenteListo.clear();
    delete grua;
    grua = nullptr;
    dibujoTorres.clear();
    rail = nullptr;
    delete mundo;
    mundo = nullptr;
}
bool Juego::DebeCerrar()
{
    return WindowShouldClose();
}