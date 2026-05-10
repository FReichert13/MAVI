#include "Juego.h"
#include "Constantes.h"
Juego::Juego(int ancho, int alto)
    : anchoVentana(ancho),
    altoVentana(alto),
    mundo(nullptr),
    proyectil(nullptr),
    impulsoMaximo(15.0f, -22.0f),
    potenciaCarga(0.0f),
    tiempoCargaTotal(1.5f),
    potenciaMinima(0.05f),
    ultimaPotenciaDisparada(0)
{
}
Juego::~Juego()
{
    Finalizar();
}
void Juego::Ejecutar()
{
    Iniciar();
    CrearEscena();
    while (!WindowShouldClose())
    {
        ProcesarInput();
        Actualizar();
        Dibujar();
    }
}
void Juego::Iniciar()
{
    InitWindow(anchoVentana, altoVentana, "MAVI2 - Unidad2 - Franco Reichert");
    SetTargetFPS(60);
    //con el mundo esta a escala de metros, podemos usar la gravedad real de la tierra 9.8 m/s^2 
    b2Vec2 gravedad(0.0f, 9.8f);
    mundo = new b2World(gravedad);
}
void Juego::CrearMuroEstatico(float cxPx, float cyPx, float halfWPx, float halfHPx)
{
    b2BodyDef def;
    def.type = b2_staticBody;
    def.position.Set(Pix2M(cxPx), Pix2M(cyPx));
    b2Body* body = mundo->CreateBody(&def);
    b2PolygonShape forma;
    forma.SetAsBox(Pix2M(halfWPx), Pix2M(halfHPx));
    body->CreateFixture(&forma, 0.0f);
}
void Juego::CrearLimites()
{
    //suelo
    CrearMuroEstatico(anchoVentana / 2.0f, altoVentana - 40.0f,
        anchoVentana / 2.0f, 20.0f);
    //limites en techo y paredes
    CrearMuroEstatico(-10.0f, altoVentana / 2.0f,
        10.0f, altoVentana / 2.0f);
    CrearMuroEstatico(anchoVentana + 10.0f, altoVentana / 2.0f,
        10.0f, altoVentana / 2.0f);
    CrearMuroEstatico(anchoVentana / 2.0f, -10.0f,
        anchoVentana / 2.0f, 10.0f);
}
void Juego::CrearEscena()
{
    CrearLimites();
    cajas.reserve(4);
    for (int i = 0; i < 4; i++)
    {
        b2Vec2 pos(600.0f + i * 55.0f, 480.0f - i * 55.0f);
        cajas.emplace_back(*mundo, pos);
    }
    //proyectil cerca del suelo
    b2Vec2 posInicial(100.0f, altoVentana - 100.0f);
    proyectil = new Proyectil(*mundo, posInicial);
}
void Juego::ProcesarInput()
{
    //R: reinicia el proyectil 
    if (IsKeyPressed(KEY_R))
    {
        delete proyectil;   //el destructor del peoyectil saca el cuerpo del mundo
        b2Vec2 posInicial(100.0f, altoVentana - 100.0f);
        proyectil = new Proyectil(*mundo, posInicial);
        potenciaCarga = 0.0f;
        ultimaPotenciaDisparada = 0;
        return;
    }
    if (proyectil->FueLanzado()) return;
    //mientras apretas espacio, carga potencia
    if (IsKeyDown(KEY_SPACE))
    {
        potenciaCarga += GetFrameTime() / tiempoCargaTotal;
        if (potenciaCarga > 1.0f) potenciaCarga = 1.0f;
    }
    //al soltar espacio, tiramos el proyectil
    if (IsKeyReleased(KEY_SPACE))
    {
        float potenciaEfectiva =
            potenciaMinima + potenciaCarga * (1.0f - potenciaMinima);
        b2Vec2 impulso(
            impulsoMaximo.x * potenciaEfectiva,
            impulsoMaximo.y * potenciaEfectiva
        );
        proyectil->Lanzar(impulso);

        ultimaPotenciaDisparada = (int)(potenciaEfectiva * 100);
    }
}
void Juego::Actualizar()
{
    mundo->Step(1.0f / 60.0f, 8, 3);
}
void Juego::Dibujar()
{
    Color fondo = { 110, 100, 215, 255 };
    Color sueloColor = Fade(DARKGREEN, 0.7f);
    BeginDrawing();
    ClearBackground(fondo);
    DrawRectangle(0, altoVentana - 60, anchoVentana, 40, sueloColor);
    for (const auto& caja : cajas) caja.Dibujar();
    proyectil->Dibujar();
    DibujarHUDCarga();
    //instrucciones
    DrawRectangle(20, 20, 540, 100, Fade(BLACK, 0.4f));
    DrawText("lanzamiento por impulso", 35, 30, 24, RAYWHITE);
    DrawText("apreta espacio para cargar, soltala para tirar", 35, 64, 18, RAYWHITE);
    DrawText("tecla 'R': reiniciar", 35, 86, 18, RAYWHITE);
    //despues del disparo vemos la potencia del disparo
    if (proyectil->FueLanzado() && ultimaPotenciaDisparada > 0)
    {
        DrawRectangle(anchoVentana - 280, 20, 260, 50, Fade(BLACK, 0.5f));
        DrawText(TextFormat("disparo con %d%%", ultimaPotenciaDisparada),
            anchoVentana - 265, 35, 22, YELLOW);
    }
    EndDrawing();
}
void Juego::DibujarHUDCarga()
{
    if (proyectil->FueLanzado()) return;
    Vector2 pos = proyectil->GetPosicionPx();
    float potenciaEfectiva =
        potenciaMinima + potenciaCarga * (1.0f - potenciaMinima);
    //linea que apunta a 45 grados
    float largoFlecha = 30.0f + potenciaCarga * 100.0f;
    float grosorFlecha = 2.0f + potenciaCarga * 4.0f;
    Vector2 desde = { pos.x, pos.y };
    Vector2 hasta = {
        pos.x + largoFlecha * 0.7071f,   
        pos.y - largoFlecha * 0.7071f    
    };
    DrawLineEx(desde, hasta, grosorFlecha, YELLOW);
   //barra de potencia
    int barraAncho = 100;
    int barraAlto = 12;
    int barraX = (int)(pos.x - barraAncho / 2.0f);
    int barraY = (int)(pos.y - 55.0f);
    DrawRectangle(barraX, barraY, barraAncho, barraAlto, Fade(BLACK, 0.6f));
    int rellenoAncho = (int)(barraAncho * potenciaCarga);
    Color colorBarra = {
        (unsigned char)(255 * potenciaCarga),
        (unsigned char)(220 * (1.0f - potenciaCarga * 0.5f)),
        80,
        255
    };
    DrawRectangle(barraX, barraY, rellenoAncho, barraAlto, colorBarra);
    DrawRectangleLines(barraX, barraY, barraAncho, barraAlto, RAYWHITE);
    //porcentaje efectivo al lado de la barra
    DrawText(TextFormat("%d%%", (int)(potenciaEfectiva * 100)),
        barraX + barraAncho + 8, barraY - 2, 18, RAYWHITE);
}
void Juego::Finalizar()
{
    if (proyectil)
    {
        delete proyectil;
        proyectil = nullptr;
    }
    cajas.clear();
    if (mundo)
    {
        delete mundo;
        mundo = nullptr;
    }
    if (IsWindowReady())
    {
        CloseWindow();
    }
}