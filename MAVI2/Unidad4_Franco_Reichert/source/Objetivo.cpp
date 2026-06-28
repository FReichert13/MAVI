#include "Objetivo.h"
Objetivo::Objetivo(b2World* mundo, float cxPix, float cyPix, float anchoPix, float altoPix)
    : ancho(anchoPix), alto(altoPix), impactado(false), datos(TipoObjeto::Objetivo)
{
    b2BodyDef def;
    def.type = b2_staticBody;
    def.position.Set(Pix2M(cxPix), Pix2M(cyPix));
    def.userData.pointer = reinterpret_cast<uintptr_t>(&datos);
    cuerpo = mundo->CreateBody(&def);
    b2PolygonShape forma;
    forma.SetAsBox(Pix2M(ancho / 2.0f), Pix2M(alto / 2.0f));
    cuerpo->CreateFixture(&forma, 0.0f);
}
Objetivo::~Objetivo() {}
void Objetivo::Dibujar() const
{
    b2Vec2 m = cuerpo->GetPosition();
    int x = (int)(M2Pix(m.x) - ancho / 2.0f);
    int y = (int)(M2Pix(m.y) - alto / 2.0f);
    Color relleno = impactado ? GREEN : GRAY;
    DrawRectangle(x, y, (int)ancho, (int)alto, relleno);
    DrawRectangleLines(x, y, (int)ancho, (int)alto, BLACK);
    DrawCircle(x + (int)ancho / 2, y + (int)alto / 2, 8,
        impactado ? DARKGREEN : RED);
    DrawText("BLANCO", x - 6, y - 20, 14, BLACK);
}