#include "Caja.h"
Caja::Caja(b2World* mundo, float x, float y, float w, float h)
{
    ancho = w;
    alto = h;
    //la etiqueta permite reconocer este cuerpo durante las colisiones
    etiqueta = { TipoCuerpo::Caja, this };
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(x, y);
    def.linearDamping = 0.4f;   
    def.angularDamping = 0.4f;
    def.bullet = true;          
    def.userData.pointer = reinterpret_cast<uintptr_t>(&etiqueta);
    body = mundo->CreateBody(&def);
    b2PolygonShape shape;
    shape.SetAsBox(w / 2.0f, h / 2.0f);
    b2FixtureDef fix;
    fix.shape = &shape;
    fix.density = 0.9f;     
    fix.friction = 0.9f;    
    fix.restitution = 0.0f;  
    body->CreateFixture(&fix);
}
void Caja::Dibujar(Texture2D tex) const
{
    b2Vec2 p = body->GetPosition();
    float  ang = body->GetAngle() * RAD2DEG;
    Rectangle src = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
    Rectangle dst = { p.x, p.y, ancho, alto };
    Vector2   org = { ancho / 2.0f, alto / 2.0f };
    DrawTexturePro(tex, src, dst, org, ang, WHITE);
    //color segun el estado, para que el resultado sea claro
    if (estado == EstadoCaja::Colocada)
        DrawRectanglePro(dst, org, ang, Fade(GREEN, 0.6f)); //verde = bien colocada
    else if (estado == EstadoCaja::Perdida)
        DrawRectanglePro(dst, org, ang, Fade(RED, 0.55f));  //rojo = error
    //barra de progreso de estabilidad de la caja
    if (estado == EstadoCaja::Libre && progreso > 0.0f)
    {
        float prog = progreso > 1.0f ? 1.0f : progreso;
        float bw = ancho;
        int bx = (int)(p.x - bw / 2.0f);
        int by = (int)(p.y - alto / 2.0f - 10.0f);
        DrawRectangle(bx, by, (int)bw, 5, Fade(BLACK, 0.4f));
        DrawRectangle(bx, by, (int)(bw * prog), 5, LIME);
    }
}