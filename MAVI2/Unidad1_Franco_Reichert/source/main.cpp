#ifndef NDEBUG
#include <vld.h>
#endif
#include "raylib.h"
#include <box2d.h>
#include <vector>
#include "Box.h"
int main(void)
{
    //ventana inicial
    const int screenWidth = 1000;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Unidad 1 - Franco Reichert");
    SetTargetFPS(60);
    Color fondo = { 110, 100, 215, 255 };
    Color sueloColor = Fade(DARKGREEN, 0.7f);
    //mundo fisico
    b2Vec2 gravity(0.0f, 9.8f);
    b2World world(gravity);
    //suelo
    b2BodyDef groundDef;
    groundDef.type = b2_staticBody;
    groundDef.position.Set(screenWidth / 2.0f, screenHeight - 40.0f);
    b2Body* groundBody = world.CreateBody(&groundDef);
    b2PolygonShape groundShape;
    groundShape.SetAsBox(screenWidth / 2.0f, 20.0f);
    groundBody->CreateFixture(&groundShape, 0.0f);
    std::vector<Box> boxes;
    //punto donde aparecen las nuevas cajas
    const float spawnX = screenWidth / 2.0f;
    const float spawnY = 70.0f;
    //dinensiones del rectangulo
    const float boxWidth = 70.0f;
    const float boxHeight = 40.0f;
    //angulo creado para la proxima, se modifica con las flechas
    float pendingAngleRad = 0.0f;
    const float rotationStep = 0.04f;
    //bucle principal
    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_LEFT))   pendingAngleRad -= rotationStep;
        if (IsKeyDown(KEY_RIGHT))  pendingAngleRad += rotationStep;
        if (IsKeyPressed(KEY_SPACE))
        {
            boxes.emplace_back(world,
                spawnX, spawnY,
                boxWidth, boxHeight,
                pendingAngleRad,
                Fade(SKYBLUE, 0.95f));
        }
        world.Step(1.0f / 60.0f, 8, 3);
        BeginDrawing();
        ClearBackground(fondo);
        //suelo
        DrawRectangle(0, screenHeight - 60, screenWidth, 40, sueloColor);
        //cajas existentes
        for (const auto& box : boxes)
            box.Draw();
        //preview de la prox caja
        Rectangle previewRect = { spawnX, spawnY, boxWidth, boxHeight };
        Vector2   previewOrigin = { boxWidth / 2.0f, boxHeight / 2.0f };
        DrawRectanglePro(previewRect, previewOrigin,
            pendingAngleRad * RAD2DEG,
            Fade(YELLOW, 0.45f));
        //instrucciones
        DrawText("tecla espacio: crear caja", 20, 20, 20, RAYWHITE);
        DrawText("flechas IZQ/DER: rotar la proxima caja", 20, 45, 20, RAYWHITE);
        DrawText(TextFormat("angulo: %.1f cajas: %d",
            pendingAngleRad * RAD2DEG, (int)boxes.size()),
            20, 70, 20, GOLD);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}