#include "ContactListener.h"
DatosCuerpo* ContactListener::ObtenerDatos(b2Body* cuerpo)
{
    uintptr_t ptr = cuerpo->GetUserData().pointer;
    return ptr ? reinterpret_cast<DatosCuerpo*>(ptr) : nullptr;
}
bool ContactListener::EsPar(DatosCuerpo* a, DatosCuerpo* b,
    TipoObjeto t1, TipoObjeto t2)
{
    if (!a || !b) return false;
    return (a->tipo == t1 && b->tipo == t2) ||
        (a->tipo == t2 && b->tipo == t1);
}
void ContactListener::BeginContact(b2Contact* contacto)
{
    b2Body* cuerpoA = contacto->GetFixtureA()->GetBody();
    b2Body* cuerpoB = contacto->GetFixtureB()->GetBody();
    DatosCuerpo* datosA = ObtenerDatos(cuerpoA);
    DatosCuerpo* datosB = ObtenerDatos(cuerpoB);
    if (EsPar(datosA, datosB, TipoObjeto::Proyectil, TipoObjeto::Objetivo))
        impactoObjetivo = true;
    if (EsPar(datosA, datosB, TipoObjeto::Proyectil, TipoObjeto::ZonaSensor))
    {
        dentroDeZona = true;
        activacionesZona++;
    }
    if (EsPar(datosA, datosB, TipoObjeto::Proyectil, TipoObjeto::Suelo))
        tocoSuelo = true;
}
void ContactListener::EndContact(b2Contact* contacto)
{
    //contacto es un intervalo
    b2Body* cuerpoA = contacto->GetFixtureA()->GetBody();
    b2Body* cuerpoB = contacto->GetFixtureB()->GetBody();
    DatosCuerpo* datosA = ObtenerDatos(cuerpoA);
    DatosCuerpo* datosB = ObtenerDatos(cuerpoB);
    if (EsPar(datosA, datosB, TipoObjeto::Proyectil, TipoObjeto::ZonaSensor))
        dentroDeZona = false;
}
void ContactListener::Reiniciar()
{
    impactoObjetivo = false;
    tocoSuelo = false;
    dentroDeZona = false;
    activacionesZona = 0;
}