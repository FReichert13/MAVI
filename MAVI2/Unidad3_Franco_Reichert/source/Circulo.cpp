#include "Circulo.h"
#include <cmath>
Circulo::Circulo(b2World* mundo, float x, float y, float r, Color c)
{
    radio = r;
    color = c;
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(x, y);
    body = mundo->CreateBody(&def);
    b2CircleShape shape;
    shape.m_radius = r;
    b2FixtureDef fix;
    fix.shape = &shape;
    fix.density = 1.0f;
    fix.friction = 0.3f;
    fix.restitution = 0.5f;
    body->CreateFixture(&fix);
}
void Circulo::Dibujar()
{
    b2Vec2 p = body->GetPosition();
    float  ang = body->GetAngle();
    DrawCircleV({ p.x, p.y }, radio, color);
    DrawCircleLines((int)p.x, (int)p.y, radio, BROWN);
    Vector2 fin = { p.x + cosf(ang) * radio, p.y + sinf(ang) * radio };
    DrawLineEx({ p.x, p.y }, fin, 2.0f, MAROON);
}