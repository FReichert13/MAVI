#pragma once
#include "raylib.h"
class HUD {
public:
    HUD(Texture2D* heartTex);
    void draw(int lives, int score);
private:
    Texture2D* heartTex;   //textura del icono de vida
};