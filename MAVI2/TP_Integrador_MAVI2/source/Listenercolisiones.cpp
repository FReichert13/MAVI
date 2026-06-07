#include "ListenerColisiones.h"
Etiqueta * ListenerColisiones::etiquetaDe(b2Fixture * f)
{
    uintptr_t p = f->GetBody()->GetUserData().pointer;
    if (p == 0) return nullptr;
    return reinterpret_cast<Etiqueta*>(p);
}
void ListenerColisiones::BeginContact(b2Contact* contacto)
{
    Etiqueta* a = etiquetaDe(contacto->GetFixtureA());
    Etiqueta* b = etiquetaDe(contacto->GetFixtureB());
    if (!a || !b) return;
    Etiqueta* etCaja = nullptr;
    Etiqueta* otro = nullptr;
    if (a->tipo == TipoCuerpo::Caja) { etCaja = a; otro = b; }
    else if (b->tipo == TipoCuerpo::Caja) { etCaja = b; otro = a; }
    if (!etCaja) return;
    Caja* c = static_cast<Caja*>(etCaja->objeto);
    if (!c) return;
    //si la caja libre toca el suelo, se considera perdida 
    if (otro->tipo == TipoCuerpo::Suelo && c->estado == EstadoCaja::Libre)
        c->estado = EstadoCaja::Perdida;
}