#include "Proyectil.h"
Proyectil::Proyectil(b2World* mundoFisico, float xPix, float yPix, float radioPix,
    float vx, float vy)
    : mundo(mundoFisico), radioPix(radioPix), datos(TipoObjeto::Proyectil)
{
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(Pix2M(xPix), Pix2M(yPix));
    def.userData.pointer = reinterpret_cast<uintptr_t>(&datos);
    cuerpo = mundo->CreateBody(&def);
    b2CircleShape forma;
    forma.m_radius = Pix2M(radioPix);
    b2FixtureDef fixture;
    fixture.shape = &forma;
    fixture.density = 1.0f;
    fixture.friction = 0.3f;
    fixture.restitution = 0.25f;
    cuerpo->CreateFixture(&fixture);
    cuerpo->SetLinearVelocity(b2Vec2(vx, vy)); //velocidad inicial del tiro
}
//saca el cuerpo del mundo para que el reinicio deje la escena limpia
Proyectil::~Proyectil()
{
    mundo->DestroyBody(cuerpo);
}
b2Vec2 Proyectil::ObtenerPosicionPix() const
{
    b2Vec2 m = cuerpo->GetPosition();
    return b2Vec2(M2Pix(m.x), M2Pix(m.y));
}
void Proyectil::Dibujar() const
{
    b2Vec2 p = ObtenerPosicionPix();
    DrawCircle((int)p.x, (int)p.y, radioPix, ORANGE);
    DrawCircleLines((int)p.x, (int)p.y, radioPix, MAROON);
}