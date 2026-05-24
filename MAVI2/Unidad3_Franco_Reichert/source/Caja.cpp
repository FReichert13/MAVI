#include "Caja.h"
Caja::Caja(b2World* mundo, float x, float y, float w, float h, Color c)
{
    ancho = w;
    alto = h;
    color = c;
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(x, y);
    body = mundo->CreateBody(&def);
    b2PolygonShape shape;
    shape.SetAsBox(w / 2.0f, h / 2.0f);
    b2FixtureDef fix;
    fix.shape = &shape;
    fix.density = 1.0f;
    fix.friction = 0.4f;
    fix.restitution = 0.2f;
    body->CreateFixture(&fix);
}
void Caja::Dibujar()
{
    b2Vec2 p = body->GetPosition();
    float  ang = body->GetAngle() * RAD2DEG;
    Rectangle r = { p.x, p.y, ancho, alto };
    Vector2   origen = { ancho / 2.0f, alto / 2.0f };
    DrawRectanglePro(r, origen, ang, color);
    Rectangle rOut = { p.x - ancho / 2.0f, p.y - alto / 2.0f, ancho, alto };
    DrawRectangleLinesEx(rOut, 1.0f, BLACK);
}