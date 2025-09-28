#ifndef NDEBUG
#include <vld.h>
#endif
#include "raylib.h"
int main(void){
    InitWindow(800, 450, "Franco Andr�s Reichert");
    SetTargetFPS(60);
    Color fondo = GREEN;
    Color texto = BLACK;
    Color Line = RED;
    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(fondo);
        DrawText("Franco Andr�s Reichert", 255, 210, 30, texto);
        DrawLine(255, 245, 630, 245, Line);
        if (IsKeyPressed(KEY_SPACE)) {
            Line = BLACK;
            fondo = WHITE;
            texto = BLUE;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

