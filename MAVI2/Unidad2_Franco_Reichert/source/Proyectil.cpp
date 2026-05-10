#include "Proyectil.h"
#include "Constantes.h"
Proyectil::Proyectil(b2World& mundoFisico, b2Vec2 posInicialPx, float rPx)
    : mundo(&mundoFisico),
    cuerpo(nullptr),
    radioPx(rPx),
    color(RED),
    lanzado(false)
{
    //px a metros para box2d
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(Pix2M(posInicialPx.x), Pix2M(posInicialPx.y));
    cuerpo = mundo->CreateBody(&def);
    b2CircleShape forma;
    forma.m_radius = Pix2M(radioPx);
    b2FixtureDef fixture;
    fixture.shape = &forma;
    fixture.density = 1.0f;
    fixture.friction = 0.3f;
    fixture.restitution = 0.4f;
    cuerpo->CreateFixture(&fixture);
}
Proyectil::~Proyectil()
{
    //destroyBody para sacar el cuerpo  del mundo fisico y no dejar en cada reset un cuerpo invisible en la simulacion
    if (mundo && cuerpo)
    {
        mundo->DestroyBody(cuerpo);
    }
}
void Proyectil::Lanzar(b2Vec2 impulso)
{
    cuerpo->ApplyLinearImpulseToCenter(impulso, true);
    lanzado = true;
}
Vector2 Proyectil::GetPosicionPx() const
{
    b2Vec2 posM = cuerpo->GetPosition();
    return { M2Pix(posM.x), M2Pix(posM.y) };
}
void Proyectil::Dibujar() const
{
    Vector2 posPx = GetPosicionPx();
    DrawCircleV(posPx, radioPx, color);
    DrawCircleLines((int)posPx.x, (int)posPx.y, radioPx, MAROON);
}