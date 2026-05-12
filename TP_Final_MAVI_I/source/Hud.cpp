#include "HUD.h"
HUD::HUD(Texture2D* tex) : heartTex(tex) {}
void HUD::draw(int lives, int score) {
    //un icono de vida por cada vida restante
    for (int i = 0; i < lives; i++) {
        DrawTexture(*heartTex, 10 + i * (heartTex->width + 5), 10, WHITE);
    }
    //puntaje debajo de las vidas
    DrawText(TextFormat("Puntaje: %d", score),
        10, 10 + heartTex->height + 10, 24, WHITE);
}