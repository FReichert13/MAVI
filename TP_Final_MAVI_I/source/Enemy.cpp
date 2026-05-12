#include "Enemy.h"
#include <math.h>  
Enemy::Enemy(Texture2D* tex, Vector2 pos) {
    texture = tex;
    position = pos;
    active = true;
    width = texture->width;
    height = texture->height;
}
void Enemy::draw() {
    DrawTexture(*texture, (int)position.x, (int)position.y, WHITE);
}
//rctangulo usado para colisiones con el jugador
Rectangle Enemy::getBounds() const {
    return { position.x, position.y, (float)width, (float)height };
}
EnemyLinear::EnemyLinear(Texture2D* tex, Vector2 pos, Vector2 vel)
    : Enemy(tex, pos), velocity(vel) {
}
void EnemyLinear::update(float dt) {
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
    //si salio de la pantalla, se desactiva.
    if (position.x < -width || position.x > GetScreenWidth() ||
        position.y < -height || position.y > GetScreenHeight()) {
        active = false;
    }
}
EnemyAccelerated::EnemyAccelerated(Texture2D* tex, Vector2 pos)
    : Enemy(tex, pos), velocity({ 0, 0 }), gravity(350.0f) {
}
void EnemyAccelerated::update(float dt) {
    velocity.y += gravity * dt;     //la velocidad crece
    position.y += velocity.y * dt;  //la posicion baja dependiendo de esa velocidad

    if (position.y > GetScreenHeight()) active = false;
}
EnemyCircular::EnemyCircular(Texture2D* tex, Vector2 c, float r, float w)
    : Enemy(tex, { 0, 0 }), center(c), radius(r), omega(w),
    angle(0.0f), lifetime(0.0f) {
    //se calcula la posicion inicial centrando el sprite sobre la orbita
    position.x = center.x + radius * cosf(angle) - width / 2.0f;
    position.y = center.y + radius * sinf(angle) - height / 2.0f;
}
void EnemyCircular::update(float dt) {
    angle += omega * dt;
    position.x = center.x + radius * cosf(angle) - width / 2.0f;
    position.y = center.y + radius * sinf(angle) - height / 2.0f;
    //se elimina despues de 8 segundos para no acumularse muchos en la pantalla
    lifetime += dt;
    if (lifetime > 8.0f) active = false;
}