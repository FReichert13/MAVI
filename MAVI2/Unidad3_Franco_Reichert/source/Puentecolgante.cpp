#include "PuenteColgante.h"
PuenteColgante::PuenteColgante(b2World* mundo,
    b2Body* extremoIzq, b2Vec2 anclaIzq,
    b2Body* extremoDer, b2Vec2 anclaDer,
    int numSegmentos, Color c)
{
    color = c;
    altoSeg = 14.0f;
    anchoSeg = (anclaDer.x - anclaIzq.x) / (float)numSegmentos;
    b2Body* anterior = extremoIzq;
    b2Vec2  puntoUnion = anclaIzq;
    for (int i = 0; i < numSegmentos; i++)
    {
        float cx = anclaIzq.x + anchoSeg * ((float)i + 0.5f);
        float cy = anclaIzq.y;
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position.Set(cx, cy);
        b2Body* seg = mundo->CreateBody(&def);
        b2PolygonShape shape;
        shape.SetAsBox(anchoSeg / 2.0f - 2.0f, altoSeg / 2.0f);
        b2FixtureDef fix;
        fix.shape = &shape;
        fix.density = 0.8f;
        fix.friction = 0.6f;
        fix.restitution = 0.05f;
        seg->CreateFixture(&fix);
        //revolute jount con el cuerpo anterior
        b2RevoluteJointDef jdef;
        jdef.Initialize(anterior, seg, puntoUnion);
        mundo->CreateJoint(&jdef);
        anclas.push_back(puntoUnion);
        anterior = seg;
        puntoUnion.Set(anclaIzq.x + anchoSeg * (float)(i + 1), cy);
        segmentos.push_back(seg);
    }
    //ultimo revolute roint
    b2RevoluteJointDef jdef;
    jdef.Initialize(anterior, extremoDer, anclaDer);
    mundo->CreateJoint(&jdef);
    anclas.push_back(anclaDer);
}
void PuenteColgante::Dibujar()
{
    for (b2Body* seg : segmentos)
    {
        b2Vec2 p = seg->GetPosition();
        float  ang = seg->GetAngle() * RAD2DEG;
        Rectangle r = { p.x, p.y, anchoSeg, altoSeg };
        Vector2   o = { anchoSeg / 2.0f, altoSeg / 2.0f };
        DrawRectanglePro(r, o, ang, color);
    }
    //conexion de los revolute joints
    for (const b2Vec2& pt : anclas)
        DrawCircleV({ pt.x, pt.y }, 4.0f, RED);
}