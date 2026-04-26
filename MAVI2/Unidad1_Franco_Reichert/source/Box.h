#pragma once
#include "raylib.h"
class b2World;
class b2Body;
class Box
{
public:
    Box(b2World& world,
        float x, float y,
        float width, float height,
        float angleRad,
        Color color);
    void Draw() const;
private:
    b2Body* m_body;
    float   m_width;
    float   m_height;
    Color   m_color;
};