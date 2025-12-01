#include "Juego.h"
//constructor
Juego::Juego()
    : screenWidth(800)
    , screenHeight(600)
    , estadoActual(MENU)
    , jugador(nullptr)
    , enemigo1(nullptr)
    , enemigo2(nullptr)
    , groundLevel(550.0f)
    , suelo{ 0, 0, 0, 0 }
    , plataforma1{ 0, 0, 0, 0 }
    , plataforma2{ 0, 0, 0, 0 }
    , plataforma3{ 0, 0, 0, 0 }
    , objetivo{ 0, 0, 0, 0 }
    , botonJugar{ 0, 0, 0, 0 }
    , fondoTextureLoaded(false)
    , objetivoTextureLoaded(false)
{
}
//destructor
Juego::~Juego() {
    if (jugador != nullptr) {
        delete jugador;
    }
    if (enemigo1 != nullptr) {
        delete enemigo1;
    }
    if (enemigo2 != nullptr) {
        delete enemigo2;
    }
    if (fondoTextureLoaded) {
        UnloadTexture(fondoTexture);
    }
    if (objetivoTextureLoaded) {
        UnloadTexture(objetivoTexture);
    }
}
void Juego::Inicializar() {
    InitWindow(screenWidth, screenHeight, "TP Integrador: Reichert Franco");
    SetTargetFPS(60);
    //tamaño de los sprites
    float spriteSize = 80.0f;
    suelo = { 0, groundLevel, (float)screenWidth, 50 };
    plataforma1 = { 100, 450, 250, 25 };      
    plataforma2 = { 350, 340, 250, 25 };    
    plataforma3 = { 520, 220, 250, 25 };      
    objetivo = { 680, 130, 90, 90 };          
    botonJugar = { 300, 300, 200, 50 };
    jugador = new Jugador(50, 400, spriteSize, spriteSize);
    enemigo1 = new Enemigo(150, 370, spriteSize, spriteSize, 100, 270, 100.0f);
    enemigo2 = new Enemigo(400, 260, spriteSize, spriteSize, 350, 520, 80.0f);
    CargarRecursos();
}
void Juego::CargarRecursos() {
    fondoTexture = LoadTexture("Sprites/Fondo.png");
    if (fondoTexture.id != 0) {
        fondoTextureLoaded = true;
    }
    jugador->CargarTextura("Sprites/Sprite1.png");
    enemigo1->CargarTextura("Sprites/Sprite2.png");
    enemigo2->CargarTextura("Sprites/Sprite2.png");
    objetivoTexture = LoadTexture("Sprites/Sprite3.png");
    if (objetivoTexture.id != 0) {
        objetivoTextureLoaded = true;
    }
}
void Juego::ProcesarInput() {
    switch (estadoActual) {
    case MENU:
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (MouseSobreRectangulo(botonJugar)) {
                estadoActual = JUGANDO;
                Reiniciar();
            }
        }
        break;
    case JUGANDO:
        jugador->HandleInput();
        if (IsKeyPressed(KEY_R)) {
            Reiniciar();
        }
        break;
    case GANADO:
    case PERDIDO:
        if (IsKeyPressed(KEY_ENTER)) {
            estadoActual = MENU;
        }
        break;
    }
}
void Juego::Actualizar(float deltaTime) {
    if (estadoActual == JUGANDO) {
        jugador->Update(deltaTime, groundLevel);
        enemigo1->Update(deltaTime);
        enemigo2->Update(deltaTime);
        VerificarColisiones();
    }
}
void Juego::VerificarColisiones() {
    Rectangle jugadorRect = jugador->GetCollisionRect();
    Vector2 jugadorPos = jugador->GetPosition();
    Vector2 jugadorVel = jugador->GetVelocity();
    float jugadorHeight = jugadorRect.height;
    float jugadorWidth = jugadorRect.width;
    if (jugadorPos.y + jugadorHeight < groundLevel) {
        jugador->SetOnGround(false);
    }
    if (CheckCollisionRecs(jugadorRect, plataforma1)) {
        if (jugadorVel.y > 0) {
            float jugadorCentroX = jugadorPos.x + jugadorWidth / 2;
            if (jugadorCentroX >= plataforma1.x && jugadorCentroX <= plataforma1.x + plataforma1.width) {
                float jugadorBottom = jugadorPos.y + jugadorHeight;
                if (jugadorBottom <= plataforma1.y + 20) {
                    jugador->SetPosition(jugadorPos.x, plataforma1.y - jugadorHeight + 5);
                    jugador->SetOnGround(true);
                }
            }
        }
    }
    if (CheckCollisionRecs(jugadorRect, plataforma2)) {
        if (jugadorVel.y > 0) {
            float jugadorCentroX = jugadorPos.x + jugadorWidth / 2;
            if (jugadorCentroX >= plataforma2.x && jugadorCentroX <= plataforma2.x + plataforma2.width) {
                float jugadorBottom = jugadorPos.y + jugadorHeight;
                if (jugadorBottom <= plataforma2.y + 20) {
                    jugador->SetPosition(jugadorPos.x, plataforma2.y - jugadorHeight + 5);
                    jugador->SetOnGround(true);
                }
            }
        }
    }
    if (CheckCollisionRecs(jugadorRect, plataforma3)) {
        if (jugadorVel.y > 0) {
            float jugadorCentroX = jugadorPos.x + jugadorWidth / 2;
            if (jugadorCentroX >= plataforma3.x && jugadorCentroX <= plataforma3.x + plataforma3.width) {
                float jugadorBottom = jugadorPos.y + jugadorHeight;
                if (jugadorBottom <= plataforma3.y + 20) {
                    jugador->SetPosition(jugadorPos.x, plataforma3.y - jugadorHeight + 5);
                    jugador->SetOnGround(true);
                }
            }
        }
    }
    float margen = jugadorWidth * 0.2f;  
    Rectangle jugadorHitbox = {
        jugadorRect.x + margen,
        jugadorRect.y + margen,
        jugadorRect.width - margen * 2,
        jugadorRect.height - margen * 2
    };
    Rectangle enemigo1Rect = enemigo1->GetCollisionRect();
    float margenEnemigo = enemigo1Rect.width * 0.2f;
    Rectangle enemigo1Hitbox = {
        enemigo1Rect.x + margenEnemigo,
        enemigo1Rect.y + margenEnemigo,
        enemigo1Rect.width - margenEnemigo * 2,
        enemigo1Rect.height - margenEnemigo * 2
    };
    Rectangle enemigo2Rect = enemigo2->GetCollisionRect();
    Rectangle enemigo2Hitbox = {
        enemigo2Rect.x + margenEnemigo,
        enemigo2Rect.y + margenEnemigo,
        enemigo2Rect.width - margenEnemigo * 2,
        enemigo2Rect.height - margenEnemigo * 2
    };
    if (CheckCollisionRecs(jugadorHitbox, enemigo1Hitbox) ||
        CheckCollisionRecs(jugadorHitbox, enemigo2Hitbox)) {
        estadoActual = PERDIDO;
    }
    if (CheckCollisionRecs(jugadorHitbox, objetivo)) {
        estadoActual = GANADO;
    }
}
void Juego::Dibujar() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch (estadoActual) {
    case MENU:
        DibujarMenu();
        break;
    case JUGANDO:
        DibujarJuego();
        break;
    case GANADO:
        DibujarPantallaGanado();
        break;
    case PERDIDO:
        DibujarPantallaPerdido();
        break;
    }
    EndDrawing();
}
void Juego::DibujarMenu() {
    DrawText("Trabajo Práctico Integrador", 200, 100, 30, DARKBLUE);
    DrawText("By Franco Reichert", 220, 140, 20, DARKGRAY);
    Color btnColor = MouseSobreRectangulo(botonJugar) ? DARKGREEN : GREEN;
    DrawRectangleRec(botonJugar, btnColor);
    DrawText("JUGAR", 360, 315, 24, WHITE);
    DrawText("Controles:", 280, 380, 20, DARKBLUE);
    DrawText("Flechas: Mover", 280, 410, 16, BLACK);
    DrawText("Espacio: Saltar", 280, 430, 16, BLACK);
    DrawText("R: Reiniciar nivel", 280, 450, 16, BLACK);
    DrawText("1-4: Cambiar color del jugador", 280, 470, 16, BLACK);
    DrawText("No toques a los enemigos y lleva a Arthas hacia la Frostmoure!", 180, 520, 18, MAROON);
}
void Juego::DibujarJuego() {
    if (fondoTextureLoaded) {
        DrawTexturePro(fondoTexture,
            { 0, 0, (float)fondoTexture.width, (float)fondoTexture.height },
            { 0, 0, (float)screenWidth, (float)screenHeight },
            { 0, 0 }, 0.0f, WHITE);
    }
    else {
        DrawRectangle(0, 0, screenWidth, (int)groundLevel, SKYBLUE);
        DrawRectangleRec(suelo, DARKGREEN);
    }
    DrawRectangleRec(plataforma1, BROWN);
    DrawRectangleRec(plataforma2, BROWN);
    DrawRectangleRec(plataforma3, BROWN);
    enemigo1->Draw();
    enemigo2->Draw();
    if (objetivoTextureLoaded) {
        Rectangle source = { 0, 0, (float)objetivoTexture.width, (float)objetivoTexture.height };
        Rectangle dest = { objetivo.x, objetivo.y, objetivo.width, objetivo.height };
        DrawTexturePro(objetivoTexture, source, dest, { 0, 0 }, 0.0f, WHITE);
    }
    else {
        DrawRectangleRec(objetivo, GOLD);
        DrawText("META", (int)objetivo.x + 10, (int)objetivo.y + 30, 16, BLACK);
    }
    jugador->Draw();

    //coordenadas
    Vector2 pos = jugador->GetPosition();
    DrawText(TextFormat("Posicion: X=%.0f Y=%.0f", pos.x, pos.y), 10, 10, 16, BLACK);
    DrawText(TextFormat("Saltos: %d", jugador->GetContadorSaltos()), 10, 30, 16, BLACK);
    DrawText("R - Reiniciar | 1-4 Cambiar color", 10, 570, 14, DARKGRAY);
}
void Juego::DibujarPantallaGanado() {
    ClearBackground(DARKGREEN);
    DrawText("FELICIDADES!", 280, 200, 40, GOLD);
    DrawText("Arthas obtuvo la FROSTMOURE!", 220, 260, 24, WHITE);
    DrawText(TextFormat("Saltos utilizados: %d", jugador->GetContadorSaltos()), 260, 320, 20, YELLOW);
    DrawText("Presiona ENTER para volver al menu", 180, 400, 18, LIGHTGRAY);
}
//derrota
void Juego::DibujarPantallaPerdido() {
    ClearBackground(DARKGRAY);
    DrawText("GAME OVER", 280, 250, 40, RED);
    DrawText("Has sido asesinado por un elfo!", 240, 310, 20, WHITE);
    DrawText("Presiona enter para volver al menu", 180, 400, 18, LIGHTGRAY);
}
void Juego::Reiniciar() {
    jugador->Reset(50, 400);
}
bool Juego::DebeTerminar() const {
    return WindowShouldClose();
}
bool Juego::MouseSobreRectangulo(Rectangle rect) {
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, rect);
}