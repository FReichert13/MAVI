#include "GruaMovil.h"
#include <cmath>
GruaMovil::GruaMovil(b2World* mundo, b2Body* rail,
    float xCarrito, float yCarrito,
    float anchoCarrito, float altoCarrito,
    float altoMastil,
    float anchoBrazo,
    float radioBola,
    float largoCuerda)
{
    anchoCar = anchoCarrito;
    altoCar = altoCarrito;
    anchoMast = 14.0f;
    altoMast = altoMastil;
    anchoBr = anchoBrazo;
    altoBr = 18.0f;
    radBola = radioBola;
    {
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position.Set(xCarrito, yCarrito);
        carrito = mundo->CreateBody(&def);
        b2PolygonShape shape;
        shape.SetAsBox(anchoCar / 2.0f, altoCar / 2.0f);
        b2FixtureDef fix;
        fix.shape = &shape;
        fix.density = 3.0f;
        fix.friction = 0.7f;
        fix.restitution = 0.1f;
        carrito->CreateFixture(&fix);
    }
    //mastil
    float yMastil = yCarrito - altoCar / 2.0f - altoMast / 2.0f;
    {
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position.Set(xCarrito, yMastil);
        mastil = mundo->CreateBody(&def);
        b2PolygonShape shape;
        shape.SetAsBox(anchoMast / 2.0f, altoMast / 2.0f);
        b2FixtureDef fix;
        fix.shape = &shape;
        fix.density = 2.0f;
        fix.friction = 0.5f;
        fix.restitution = 0.1f;
        mastil->CreateFixture(&fix);
    }
    //brazo
    float yTopeMastil = yMastil - altoMast / 2.0f;
    float xBrazo = xCarrito + anchoBr / 2.0f;
    float yBrazo = yTopeMastil;
    {
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position.Set(xBrazo, yBrazo);
        brazo = mundo->CreateBody(&def);
        b2PolygonShape shape;
        shape.SetAsBox(anchoBr / 2.0f, altoBr / 2.0f);
        b2FixtureDef fix;
        fix.shape = &shape;
        fix.density = 3.0f;
        fix.friction = 0.5f;
        fix.restitution = 0.1f;
        brazo->CreateFixture(&fix);
    }
    //bola de domolicion
    float xPuntaBrazo = xBrazo + anchoBr / 2.0f;
    float yBola = yBrazo + largoCuerda;
    {
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position.Set(xPuntaBrazo, yBola);
        bola = mundo->CreateBody(&def);
        b2CircleShape shape;
        shape.m_radius = radBola;
        b2FixtureDef fix;
        fix.shape = &shape;
        fix.density = 5.0f;
        fix.friction = 0.5f;
        fix.restitution = 0.3f;
        bola->CreateFixture(&fix);
    }
    //joints que controlan la grua
    {
        b2PrismaticJointDef def;
        b2Vec2 anclaRail(xCarrito, yCarrito);
        b2Vec2 eje(1.0f, 0.0f);
        def.Initialize(rail, carrito, anclaRail, eje);
        def.enableLimit = true;
        def.lowerTranslation = -180.0f;
        def.upperTranslation = 150.0f;
        mundo->CreateJoint(&def);
    }
    {
        b2WeldJointDef def;
        b2Vec2 anclaWeld(xCarrito, yCarrito - altoCar / 2.0f);
        def.Initialize(carrito, mastil, anclaWeld);
        mundo->CreateJoint(&def);
    }
    {
        b2RevoluteJointDef def;
        b2Vec2 anclaRev(xCarrito, yTopeMastil);
        def.Initialize(mastil, brazo, anclaRev);
        def.enableLimit = true;
        def.lowerAngle = -0.4f;
        def.upperAngle = 0.4f;
        mundo->CreateJoint(&def);
    }
    {
        b2DistanceJointDef def;
        b2Vec2 anclaPuntaBrazo(xPuntaBrazo, yBrazo);
        b2Vec2 anclaBola(xPuntaBrazo, yBola);
        def.Initialize(brazo, bola, anclaPuntaBrazo, anclaBola);
        mundo->CreateJoint(&def);
    }
}
void GruaMovil::MoverCarrito(float velocidad)
{
    b2Vec2 v = carrito->GetLinearVelocity();
    carrito->SetLinearVelocity({ velocidad, v.y });
}
void GruaMovil::FrenarCarrito()
{
    b2Vec2 v = carrito->GetLinearVelocity();
    carrito->SetLinearVelocity({ v.x * 0.85f, v.y });
}
void GruaMovil::AplicarImpulsoBola(float ix, float iy)
{
    bola->ApplyLinearImpulseToCenter({ ix, iy }, true);
}
void GruaMovil::Dibujar()
{
    //carrito
    b2Vec2 pCar = carrito->GetPosition();
    Rectangle rCar = { pCar.x, pCar.y, anchoCar, altoCar };
    Vector2   oCar = { anchoCar / 2.0f, altoCar / 2.0f };
    DrawRectanglePro(rCar, oCar, 0.0f, DARKBLUE);
    Rectangle rCarOut = { pCar.x - anchoCar / 2.0f, pCar.y - altoCar / 2.0f, anchoCar, altoCar };
    DrawRectangleLinesEx(rCarOut, 2.0f, BLACK);
    //ruedas decorativas
    DrawCircle((int)(pCar.x - anchoCar * 0.3f), (int)(pCar.y + altoCar * 0.4f), 8.0f, BLACK);
    DrawCircle((int)(pCar.x + anchoCar * 0.3f), (int)(pCar.y + altoCar * 0.4f), 8.0f, BLACK);
    b2Vec2 pMast = mastil->GetPosition();
    Rectangle rMast = { pMast.x, pMast.y, anchoMast, altoMast };
    Vector2   oMast = { anchoMast / 2.0f, altoMast / 2.0f };
    DrawRectanglePro(rMast, oMast, 0.0f, DARKGRAY);
    b2Vec2 pBr = brazo->GetPosition();
    float angBr = brazo->GetAngle();
    Rectangle rBr = { pBr.x, pBr.y, anchoBr, altoBr };
    Vector2   oBr = { anchoBr / 2.0f, altoBr / 2.0f };
    DrawRectanglePro(rBr, oBr, angBr * RAD2DEG, GRAY);
    //calculo de extremos del brazo 
    float dx = cosf(angBr) * anchoBr / 2.0f;
    float dy = sinf(angBr) * anchoBr / 2.0f;
    Vector2 puntaIzq = { pBr.x - dx, pBr.y - dy };  
    Vector2 puntaDer = { pBr.x + dx, pBr.y + dy };  
    //cuerda
    b2Vec2 pBola = bola->GetPosition();
    DrawLineEx(puntaDer, { pBola.x, pBola.y }, 3.0f, DARKBROWN);
    //bola
    DrawCircleV({ pBola.x, pBola.y }, radBola, MAROON);
    DrawCircleLines((int)pBola.x, (int)pBola.y, radBola, BLACK);
    int xWeld = (int)pCar.x;
    int yWeld = (int)(pCar.y - altoCar / 2.0f);
    DrawRectangle(xWeld - 6, yWeld - 6, 12, 12, GREEN);
    DrawRectangleLines(xWeld - 6, yWeld - 6, 12, 12, DARKGREEN);
    DrawCircleV(puntaIzq, 6.0f, RED);
    DrawCircleV(puntaDer, 5.0f, ORANGE);
}