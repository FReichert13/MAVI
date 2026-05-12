#include "Player.h"
Player::Player(Texture2D* tex, Vector2 pos) {
    texture = tex;
    position = pos;
    speed = 300.0f;   //velocidad constante mru
    lives = 3;
    score = 0;
    width = texture->width;
    height = texture->height;
}
void Player::update(float dt) {
    //pos final = pos inicial + velocidad * tiempo
    //multiplicar por dt hace que la velocidad sea independiente de los FPS
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) position.x += speed * dt;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) position.x -= speed * dt;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) position.y -= speed * dt;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) position.y += speed * dt;
    //limites de pantalla
    float sw = (float)GetScreenWidth();
    float sh = (float)GetScreenHeight();
    if (position.x < 0)             position.x = 0;
    if (position.y < 0)             position.y = 0;
    if (position.x > sw - width)    position.x = sw - width;
    if (position.y > sh - height)   position.y = sh - height;
}
void Player::draw() {
    DrawTexture(*texture, (int)position.x, (int)position.y, WHITE);
}
void Player::takeDamage() { lives--; }
void Player::addScore(int p) { score += p; }
void Player::reset(Vector2 pos) {
    position = pos;
    lives = 3;
    score = 0;
}
//rectangulo usado para colision con enemigos
Rectangle Player::getBounds() const {
    return { position.x, position.y, (float)width, (float)height };
}
//centro del sprite, colision circular
Vector2 Player::getCenter() const {
    return { position.x + width / 2.0f, position.y + height / 2.0f };
}