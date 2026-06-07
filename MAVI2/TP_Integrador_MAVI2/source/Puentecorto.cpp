#include "PuenteCorto.h"
PuenteCorto::PuenteCorto(b2World* mundo,
    b2Body* torreIzq, b2Vec2 anclaIzq,
    b2Body* torreDer, b2Vec2 anclaDer,
    int numSegmentos)
{
    altoSeg = 12.0f;
    anchoSeg = (anclaDer.x - anclaIzq.x) / (float)numSegmentos;
    b2Body* anterior = torreIzq;   
    b2Vec2  union_ = anclaIzq;
    for (int i = 0; i < numSegmentos; i++)
    {
        float cx = anclaIzq.x + anchoSeg * ((float)i + 0.5f);
        float cy = anclaIzq.y;
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position.Set(cx, cy);
        b2Body* seg = mundo->CreateBody(&def);
        b2PolygonShape shape;
        shape.SetAsBox(anchoSeg / 2.0f - 1.0f, altoSeg / 2.0f);
        b2FixtureDef fix;
        fix.shape = &shape;
        fix.density = 3.0f;
        fix.friction = 0.9f;
        fix.restitution = 0.0f;
        seg->CreateFixture(&fix);
        b2RevoluteJointDef jdef;
        jdef.Initialize(anterior, seg, union_);
        mundo->CreateJoint(&jdef);
        puntosUnion.push_back(union_);
        anterior = seg;
        union_.Set(anclaIzq.x + anchoSeg * (float)(i + 1), cy);
        segmentos.push_back(seg);
    }
    b2RevoluteJointDef jdef;
    jdef.Initialize(anterior, torreDer, anclaDer);
    mundo->CreateJoint(&jdef);
    puntosUnion.push_back(anclaDer);
}
void PuenteCorto::AplicarFuerzaHorizontal(float accel)
{
    for (b2Body* seg : segmentos)
        seg->ApplyForceToCenter({ accel * seg->GetMass(), 0.0f }, true);
}
void PuenteCorto::Dibujar(bool completado, Texture2D texTablon) const
{
    Color tinte = completado ? Color{ 170, 255, 170, 255 } : WHITE;
    Rectangle src = { 0.0f, 0.0f, (float)texTablon.width, (float)texTablon.height };
    for (b2Body* seg : segmentos)
    {
        b2Vec2 p = seg->GetPosition();
        float  ang = seg->GetAngle() * RAD2DEG;
        float  w = anchoSeg + 2.0f;
        float  h = 18.0f;
        Rectangle dst = { p.x, p.y, w, h };
        Vector2   org = { w / 2.0f, h / 2.0f };
        DrawTexturePro(texTablon, src, dst, org, ang, tinte);
    }
    //marcadores de las uniones para que se vean las conexiones
    for (const b2Vec2& pt : puntosUnion)
        DrawCircleV({ pt.x, pt.y }, 2.5f, RED);
}