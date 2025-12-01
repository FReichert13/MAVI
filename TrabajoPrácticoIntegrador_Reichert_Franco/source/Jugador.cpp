#include "Jugador.h"
Jugador::Jugador(float x, float y, float w, float h) {
    position.x = x;
    position.y = y;
    width = w;
    height = h;
    velocity.x = 0.0f;
    velocity.y = 0.0f;
    color = BLUE;              
    estado = CAYENDO;           
    onGround = false;
    textureLoaded = false;
    contadorSaltos = 0;         //contador en 0
}
Jugador::~Jugador() {
    if (textureLoaded) {
        UnloadTexture(texture);
    }
}
void Jugador::CargarTextura(const char* rutaTextura) {
    texture = LoadTexture(rutaTextura);
    if (texture.id != 0) {
        textureLoaded = true;
    }
}
//actualizar la fisica del jugador: gravedad, posicion y limites
void Jugador::Update(float deltaTime, float groundLevel) {
    //gravedad
    velocity.y += GRAVITY * deltaTime;
    if (velocity.y > 0) {
        estado = CAYENDO;
    }
    else if (velocity.y < 0) {
        estado = SALTANDO;
    }
    //actualizar posicion
    position.y += velocity.y * deltaTime;
    position.x += velocity.x * deltaTime;
    //colision con el suelo
    if (position.y + height >= groundLevel) {
        position.y = groundLevel - height;
        velocity.y = 0.0f;
        onGround = true;
        estado = EN_SUELO;
    }
    //limites horizontales de la pantalla
    if (position.x < 0) {
        position.x = 0;
    }
    if (position.x + width > 800) {
        position.x = 800 - width;
    }
}
void Jugador::Draw() {
    if (textureLoaded) {
        Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
        Rectangle dest = { position.x, position.y, width, height };
        //color a la textura
        DrawTexturePro(texture, source, dest, { 0, 0 }, 0.0f, color);
    }
    else {
        Rectangle playerRect = { position.x, position.y, width, height };
        DrawRectangleRec(playerRect, color);
        DrawCircleV({ position.x + width * 0.3f, position.y + height * 0.3f }, 3, WHITE);
        DrawCircleV({ position.x + width * 0.7f, position.y + height * 0.3f }, 3, WHITE);
    }
}
void Jugador::HandleInput() {
    if (IsKeyDown(KEY_RIGHT)) {
        velocity.x = MOVE_SPEED;
    }
    else if (IsKeyDown(KEY_LEFT)) {
        velocity.x = -MOVE_SPEED;
    }
    else {
        velocity.x = 0.0f;
    }
    //salto
    if (IsKeyPressed(KEY_SPACE)) {
        Jump();
    }
    //cambios de color
    if (IsKeyPressed(KEY_ONE)) {
        color = BLUE;
    }
    if (IsKeyPressed(KEY_TWO)) {
        color = RED;
    }
    if (IsKeyPressed(KEY_THREE)) {
        color = GREEN;
    }
    if (IsKeyPressed(KEY_FOUR)) {
        color = YELLOW;
    }
}
void Jugador::Jump() {
    if (onGround) {
        velocity.y = JUMP_FORCE;
        estado = SALTANDO;
        onGround = false;
        contadorSaltos++;     
    }
}
//reiniciar pos inicial
void Jugador::Reset(float x, float y) {
    position.x = x;
    position.y = y;
    velocity.x = 0.0f;
    velocity.y = 0.0f;
    estado = CAYENDO;
    onGround = false;
    color = BLUE;
    contadorSaltos = 0;         //resetear contador de saltos
}
Vector2 Jugador::GetPosition() const {
    return position;
}
Vector2 Jugador::GetVelocity() const {
    return velocity;
}
Rectangle Jugador::GetCollisionRect() const {
    return { position.x, position.y, width, height };
}
bool Jugador::IsOnGround() const {
    return onGround;
}
int Jugador::GetContadorSaltos() const {
    return contadorSaltos;
}
void Jugador::SetPosition(float x, float y) {
    position.x = x;
    position.y = y;
}
void Jugador::SetOnGround(bool value) {
    onGround = value;
    if (value) {
        estado = EN_SUELO;
        velocity.y = 0.0f;
    }
}