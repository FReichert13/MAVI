#pragma once
#include <box2d.h>
#include "Etiqueta.h"
#include "Caja.h"
class ListenerColisiones : public b2ContactListener
{
public:
    void BeginContact(b2Contact* contacto) override;
private:
    static Etiqueta* etiquetaDe(b2Fixture* f);
};