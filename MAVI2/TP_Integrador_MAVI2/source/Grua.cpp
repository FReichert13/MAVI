#include "Grua.h"
Grua::Grua(b2World* mundo, b2Body* rail, float xInicial, float yRail)
{
    this->mundo = mundo;
    anchoCar = 84.0f;  altoCar = 22.0f;
    anchoMast = 14.0f; altoMast = 70.0f;
    largoCable = 120.0f;
    //cuerpo dinamico que se desliza sobre el rail
    {
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position.Set(xInicial, yRail);
        carrito = mundo->CreateBody(&def);
        b2PolygonShape shape;
        shape.SetAsBox(anchoCar / 2.0f, altoCar / 2.0f);
        b2FixtureDef fix;
        fix.shape = &shape;
        fix.density = 4.0f;
        fix.friction = 0.5f;
        fix.restitution = 0.0f;
        carrito->CreateFixture(&fix);
    }
    //mastil que cuelga del carrito
    float yMast = yRail + altoCar / 2.0f + altoMast / 2.0f;
    {
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position.Set(xInicial, yMast);
        mastil = mundo->CreateBody(&def);
        b2PolygonShape shape;
        shape.SetAsBox(anchoMast / 2.0f, altoMast / 2.0f);
        b2FixtureDef fix;
        fix.shape = &shape;
        fix.density = 2.0f;
        fix.friction = 0.5f;
        fix.restitution = 0.0f;
        mastil->CreateFixture(&fix);
    }
    //el carrito solo se mueve en X, con limites de recorrido
    {
        b2PrismaticJointDef def;
        b2Vec2 ancla(xInicial, yRail);
        b2Vec2 eje(1.0f, 0.0f);
        def.Initialize(rail, carrito, ancla, eje);
        def.enableLimit = true;
        def.lowerTranslation = -400.0f;
        def.upperTranslation = 400.0f;
        mundo->CreateJoint(&def);
    }
    //el mastil queda rigido respecto del carrito
    {
        b2WeldJointDef def;
        b2Vec2 ancla(xInicial, yRail + altoCar / 2.0f);
        def.Initialize(carrito, mastil, ancla);
        mundo->CreateJoint(&def);
    }
}
void Grua::Mover(float velocidad)
{
    b2Vec2 v = carrito->GetLinearVelocity();
    carrito->SetLinearVelocity({ velocidad, v.y });
}
void Grua::Frenar()
{
    b2Vec2 v = carrito->GetLinearVelocity();
    carrito->SetLinearVelocity({ v.x * 0.8f, v.y });
}
b2Vec2 Grua::PosicionGancho() const
{
    //punto inferior del mastil, en coordenadas del mundo
    return mastil->GetWorldPoint({ 0.0f, altoMast / 2.0f });
}
b2Vec2 Grua::PuntoColgado() const
{
    b2Vec2 g = PosicionGancho();
    return { g.x, g.y + largoCable };
}
void Grua::Enganchar(b2Body* caja)
{
    b2Vec2 gancho = PosicionGancho();
    b2DistanceJointDef def;
    def.Initialize(mastil, caja, gancho, caja->GetPosition());
    cable = (b2DistanceJoint*)mundo->CreateJoint(&def);
}
void Grua::Soltar()
{
    if (cable)
    {
        mundo->DestroyJoint(cable);
        cable = nullptr;
    }
}
void Grua::Dibujar(Texture2D texCarrito, Texture2D texMastil, Texture2D texGancho) const
{
    b2Vec2 pCar = carrito->GetPosition();
    b2Vec2 pMast = mastil->GetPosition();
    //mastil
    {
        float w = anchoMast + 2.0f;
        float h = altoMast;
        Rectangle src = { 0.0f, 0.0f, (float)texMastil.width, (float)texMastil.height };
        Rectangle dst = { pMast.x, pMast.y, w, h };
        Vector2   org = { w / 2.0f, h / 2.0f };
        DrawTexturePro(texMastil, src, dst, org, 0.0f, WHITE);
    }
    {
        float w = 96.0f;
        float h = w * (float)texCarrito.height / (float)texCarrito.width; //mantiene proporcion
        Rectangle src = { 0.0f, 0.0f, (float)texCarrito.width, (float)texCarrito.height };
        Rectangle dst = { pCar.x, pCar.y + 16.0f, w, h };
        Vector2   org = { w / 2.0f, h / 2.0f };
        DrawTexturePro(texCarrito, src, dst, org, 0.0f, WHITE);
    }
    //gancho y cable hacia la caja colgada
    b2Vec2 g = PosicionGancho();
    float hGancho = 22.0f * (float)texGancho.height / (float)texGancho.width;
    {
        float w = 22.0f;
        Rectangle src = { 0.0f, 0.0f, (float)texGancho.width, (float)texGancho.height };
        Rectangle dst = { g.x, g.y + hGancho / 2.0f - 4.0f, w, hGancho };
        Vector2   org = { w / 2.0f, hGancho / 2.0f };
        DrawTexturePro(texGancho, src, dst, org, 0.0f, WHITE);
    }
    if (cable)
    {
        b2Vec2 puntaGancho = { g.x, g.y + hGancho * 0.62f };
        b2Vec2 pc = cable->GetBodyB()->GetPosition();
        DrawLineEx({ puntaGancho.x, puntaGancho.y }, { pc.x, pc.y }, 3.0f, DARKBROWN);
    }
}