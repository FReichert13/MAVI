#include "ZonaSensor.h"
ZonaSensor::ZonaSensor(b2World* mundo, float cxPix, float cyPix, float anchoPix, float altoPix)
    : ancho(anchoPix), alto(altoPix), activa(false), datos(TipoObjeto::ZonaSensor)
{
    b2BodyDef def;
    def.type = b2_staticBody;
    def.position.Set(Pix2M(cxPix), Pix2M(cyPix));
    def.userData.pointer = reinterpret_cast<uintptr_t>(&datos);
    cuerpo = mundo->CreateBody(&def);
    b2PolygonShape forma;
    forma.SetAsBox(Pix2M(ancho / 2.0f), Pix2M(alto / 2.0f));
    b2FixtureDef fixture;
    fixture.shape = &forma;
    fixture.isSensor = true; //detecta contactos pero no bloquea
    cuerpo->CreateFixture(&fixture);
}
ZonaSensor::~ZonaSensor() {}
void ZonaSensor::Dibujar() const
{
    b2Vec2 m = cuerpo->GetPosition();
    int x = (int)(M2Pix(m.x) - ancho / 2.0f);
    int y = (int)(M2Pix(m.y) - alto / 2.0f);
    Color base = activa ? SKYBLUE : DARKBLUE;
    DrawRectangle(x, y, (int)ancho, (int)alto, Fade(base, activa ? 0.45f : 0.20f));
    DrawRectangleLines(x, y, (int)ancho, (int)alto, Fade(SKYBLUE, 0.8f));
    DrawText(activa ? "Activado" : "Desactivado", x + 6, y - 20, 14,
        activa ? SKYBLUE : GRAY);
}