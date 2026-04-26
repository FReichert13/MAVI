#include "Box.h"
#include <box2d.h>
//constructor, para crear el cuerpo, su forma y su fixture dentro del mundo físico que se recibe por referencua
Box::Box(b2World& world,
    float x, float y,
    float width, float height,
    float angleRad,
    Color color)
    : m_body(nullptr), m_width(width), m_height(height), m_color(color)
{
    //definicion del cuerpo, dinámico, con posición y ángulo inicial.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    bodyDef.angle = angleRad;     //angulo inicial
    m_body = world.CreateBody(&bodyDef);
    //forma rectangular.
    b2PolygonShape shape;
    shape.SetAsBox(width / 2.0f, height / 2.0f);
    //fixture, asocia la forma al cuerpo y define las propiedades fisicas
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.4f;
    fixtureDef.restitution = 0.2f;
    m_body->CreateFixture(&fixtureDef);
}
//draw, dibuja la caja usando el estado actual del cuerpo físico.
void Box::Draw() const
{
    b2Vec2 pos = m_body->GetPosition();
    float  angleDeg = m_body->GetAngle() * RAD2DEG;
    Rectangle rect = { pos.x, pos.y, m_width, m_height };
    Vector2   origin = { m_width / 2.0f, m_height / 2.0f };
    DrawRectanglePro(rect, origin, angleDeg, m_color);
}