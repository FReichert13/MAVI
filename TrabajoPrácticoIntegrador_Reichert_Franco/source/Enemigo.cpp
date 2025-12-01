#include "Enemigo.h"
Enemigo::Enemigo(float x, float y, float w, float h, float limIzq, float limDer, float vel) {
    position.x = x;
    position.y = y;
    width = w;
    height = h;
    limiteIzquierdo = limIzq;
    limiteDerecho = limDer;
    velocidad = vel;
    moviendoDerecha = true;
    textureLoaded = false;
}
Enemigo::~Enemigo() {
    if (textureLoaded) {
        UnloadTexture(texture);
    }
}
void Enemigo::CargarTextura(const char* rutaTextura) {
    texture = LoadTexture(rutaTextura);
    if (texture.id != 0) {
        textureLoaded = true;
    }
}
//movimiento de patrulla del enemigo
void Enemigo::Update(float deltaTime) {
    if (moviendoDerecha) {
        position.x += velocidad * deltaTime;
        //verifico si llego al limite derecho
        if (position.x >= limiteDerecho) {
            position.x = limiteDerecho;
            moviendoDerecha = false;
        }
    }
    //movimiento hacia la izquierda
    else {
        position.x -= velocidad * deltaTime;
        //verifico si llego al limite izquierdo
        if (position.x <= limiteIzquierdo) {
            position.x = limiteIzquierdo;
            moviendoDerecha = true;
        }
    }
}
void Enemigo::Draw() {
    if (textureLoaded) {
        Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
        Rectangle dest = { position.x, position.y, width, height };
        DrawTexturePro(texture, source, dest, { 0, 0 }, 0.0f, WHITE);
    }
    else {
        Rectangle enemyRect = { position.x, position.y, width, height };
        DrawRectangleRec(enemyRect, RED);
        DrawText("!", (int)(position.x + width * 0.4f), (int)(position.y + height * 0.2f), 20, WHITE);
    }
}
Rectangle Enemigo::GetCollisionRect() const {
    return { position.x, position.y, width, height };
}