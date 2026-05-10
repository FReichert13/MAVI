#include "Caja.h"
#include "Constantes.h"
Caja::Caja(b2World& mundo, b2Vec2 posicionPx, float wPx, float hPx)
    : cuerpo(nullptr),
    anchoPx(wPx),
    altoPx(hPx),
    color(Fade(SKYBLUE, 0.95f))
{
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(Pix2M(posicionPx.x), Pix2M(posicionPx.y));
    cuerpo = mundo.CreateBody(&def);
    b2PolygonShape forma;
    forma.SetAsBox(Pix2M(anchoPx / 2.0f), Pix2M(altoPx / 2.0f));
    b2FixtureDef fixture;
    fixture.shape = &forma;
    fixture.density = 1.0f;
    fixture.friction = 0.4f;
    fixture.restitution = 0.2f;
    cuerpo->CreateFixture(&fixture);
}
void Caja::Dibujar() const
{
    b2Vec2 posM = cuerpo->GetPosition();
    Vector2 posPx = { M2Pix(posM.x), M2Pix(posM.y) };
    float angulo = cuerpo->GetAngle() * RAD2DEG;

    Rectangle rect = {
        posPx.x - anchoPx / 2.0f,
        posPx.y - altoPx / 2.0f,
        anchoPx,
        altoPx
    };
    Vector2 origen = { anchoPx / 2.0f, altoPx / 2.0f };
    DrawRectanglePro(rect, origen, angulo, color);
    DrawRectangleLinesEx(rect, 2, DARKBLUE);
}