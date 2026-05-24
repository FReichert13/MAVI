#include "Juego.h"
Juego::Juego()
{
    colorFondo = { 110, 100, 215, 255 };
    colorSuelo = Fade(DARKGREEN, 0.7f);
    mundo = nullptr;
    rail = anclaPuenteIzq = anclaPuenteDer = nullptr;
    grua = nullptr;
    puente = nullptr;
    xCarritoInic = 250.0f;
    yCarrito = 555.0f;
    xAnclaPuenteIzq = 600.0f;
    xAnclaPuenteDer = 920.0f;
    yAnclaPuente = 270.0f;
    anchoAncla = 30.0f;
    altoAncla = 40.0f;
}
Juego::~Juego()
{
    Liberar();
}
void Juego::Inicializar()
{
    InitWindow(anchoVentana, altoVentana,
        "Unidad 3 - Reichert Franco");
    SetTargetFPS(60);
    b2Vec2 gravedad(0.0f, 9.8f);
    mundo = new b2World(gravedad);
    CrearEstructurasEstaticas();
    CrearMecanismos();
}
void Juego::CrearEstructurasEstaticas()
{
    //suelo (para colisiones)
    b2BodyDef sueloDef;
    sueloDef.type = b2_staticBody;
    sueloDef.position.Set((float)anchoVentana / 2.0f, 585.0f);
    b2Body* suelo = mundo->CreateBody(&sueloDef);
    b2PolygonShape sueloShape;
    sueloShape.SetAsBox((float)anchoVentana / 2.0f, 15.0f);
    suelo->CreateFixture(&sueloShape, 0.0f);
    b2BodyDef railDef;
    railDef.type = b2_staticBody;
    railDef.position.Set((float)anchoVentana / 2.0f, yCarrito);
    rail = mundo->CreateBody(&railDef);
    b2PolygonShape ancShape;
    ancShape.SetAsBox(anchoAncla / 2.0f, altoAncla / 2.0f);
    b2BodyDef ancDef;
    ancDef.type = b2_staticBody;
    ancDef.position.Set(xAnclaPuenteIzq, yAnclaPuente);
    anclaPuenteIzq = mundo->CreateBody(&ancDef);
    anclaPuenteIzq->CreateFixture(&ancShape, 0.0f);
    ancDef.position.Set(xAnclaPuenteDer, yAnclaPuente);
    anclaPuenteDer = mundo->CreateBody(&ancDef);
    anclaPuenteDer->CreateFixture(&ancShape, 0.0f);
}
void Juego::CrearMecanismos()
{
    //grua movil
    grua = new GruaMovil(mundo, rail,
        xCarritoInic, yCarrito,
        130.0f, 30.0f,   //ancho y alto del carrito
        400.0f,          //alto del mastil
        160.0f,          //ancho del brazo
        22.0f,           //radio de la bola
        130.0f);         //largo de la cuerda
    //puente colgante
    float yPuente = yAnclaPuente + altoAncla / 2.0f - 8.0f;
    b2Vec2 anclaIzq(xAnclaPuenteIzq + anchoAncla / 2.0f, yPuente);
    b2Vec2 anclaDer(xAnclaPuenteDer - anchoAncla / 2.0f, yPuente);
    puente = new PuenteColgante(mundo,
        anclaPuenteIzq, anclaIzq,
        anclaPuenteDer, anclaDer,
        10,
        { 139, 90, 43, 255 });
}
void Juego::ProcesarInput()
{
    if (IsKeyDown(KEY_D)) grua->MoverCarrito(200.0f);
    else if (IsKeyDown(KEY_A)) grua->MoverCarrito(-200.0f);
    else                       grua->FrenarCarrito();
    if (IsKeyDown(KEY_RIGHT)) grua->AplicarImpulsoBola(7000.0f, 0.0f);
    if (IsKeyDown(KEY_LEFT))  grua->AplicarImpulsoBola(-7000.0f, 0.0f);
    //R=impulso de demolicion
    if (IsKeyPressed(KEY_R))  grua->AplicarImpulsoBola(800000.0f, -200000.0f);
    //espacio=soltar caja
    if (IsKeyPressed(KEY_SPACE))
    {
        Vector2 m = GetMousePosition();
        cajas.push_back(new Caja(mundo, m.x, m.y,
            32.0f, 32.0f,
            Fade(YELLOW, 0.95f)));
    }
    //click izq=soltar circulo
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 m = GetMousePosition();
        circulos.push_back(new Circulo(mundo, m.x, m.y, 14.0f,
            Fade(PINK, 0.95f)));
    }
}
void Juego::Actualizar()
{
    mundo->Step(1.0f / 60.0f, 8, 3);
}
void Juego::Dibujar()
{
    BeginDrawing();
    ClearBackground(colorFondo);
    DrawRectangle(0, altoVentana - 30, anchoVentana, 30, colorSuelo);
    int yRailLinea = (int)yCarrito + 18;
    DrawLine(50, yRailLinea, anchoVentana - 50, yRailLinea, Fade(BLACK, 0.5f));
    for (int x = 60; x < anchoVentana - 50; x += 30)
        DrawLine(x, yRailLinea - 5, x, yRailLinea + 5, Fade(BLACK, 0.5f));
    Rectangle rAi = { xAnclaPuenteIzq - anchoAncla / 2.0f,
                       yAnclaPuente - altoAncla / 2.0f,
                       anchoAncla, altoAncla };
    DrawRectangleRec(rAi, BROWN);
    DrawRectangleLinesEx(rAi, 2.0f, BLACK);
    Rectangle rAd = { xAnclaPuenteDer - anchoAncla / 2.0f,
                       yAnclaPuente - altoAncla / 2.0f,
                       anchoAncla, altoAncla };
    DrawRectangleRec(rAd, BROWN);
    DrawRectangleLinesEx(rAd, 2.0f, BLACK);
    //mecanismos
    puente->Dibujar();
    grua->Dibujar();
    for (Caja* c : cajas)    c->Dibujar();
    for (Circulo* c : circulos) c->Dibujar();
    DibujarUI();
    EndDrawing();
}

void Juego::DibujarUI()
{
    //identificacion de los joints
    DrawText("mecanismo: grua movil y puente colgante", 20, 38, 12, LIGHTGRAY);
    DrawText("joints utilizados:", 20, 60, 13, YELLOW);
    DrawText("1- prismatic: en el carrito el sobre rail", 20, 78, 12, SKYBLUE);
    DrawText("2- weld:en el mastil al carrito", 20, 94, 12, GREEN);
    DrawText("3- revolute: en el brazo y puente", 20, 110, 12, GOLD);
    DrawText("4- distance: en la cuerda de la bola", 20, 126, 12, ORANGE);
    //controles
    int yC = altoVentana - 130;
    DrawRectangle(10, yC, 300, 120, Fade(BLACK, 0.6f));
    DrawText("controles:", 20, yC + 8, 14, YELLOW);
    DrawText("a/d: mover toda la grua", 20, yC + 28, 12, RAYWHITE);
    DrawText("flechas: empujar la bola", 20, yC + 44, 12, RAYWHITE);
    DrawText("'r': impulso fuerte (demolicion)", 20, yC + 60, 12, RAYWHITE);
    DrawText("espacio: soltar caja en la pos del mouse", 20, yC + 76, 12, RAYWHITE);
    DrawText("click izq: soltar circulo en la pos del mouse", 20, yC + 92, 12, RAYWHITE);
    int total = (int)cajas.size() + (int)circulos.size();
    DrawText(TextFormat("cuerpos dinamicos sueltos: %d", total),
        anchoVentana - 290, 22, 16, RAYWHITE);
}
void Juego::Liberar()
{
    for (Caja* c : cajas)    delete c;
    for (Circulo* c : circulos) delete c;
    cajas.clear();
    circulos.clear();
    delete grua;   grua = nullptr;
    delete puente; puente = nullptr;
    //destructor de b2World libera todos los b2Body internos
    delete mundo;  mundo = nullptr;
}
bool Juego::DebeCerrar()
{
    return WindowShouldClose();
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