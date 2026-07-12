#ifndef CONTACT_LISTENER_H
#define CONTACT_LISTENER_H
#include <box2d.h>
#include "Etiquetas.h"
class ContactListener : public b2ContactListener
{
public:
    bool impactoObjetivo = false;  //el proyectil toco el objetivo
    bool tocoSuelo = false;        //el proyectil cayo al suelo
    bool dentroDeZona = false;     //proyectil dentro del sensor
    int  activacionesZona = 0;
    void BeginContact(b2Contact* contacto) override;
    void EndContact(b2Contact* contacto) override;
    void Reiniciar();
private:
    DatosCuerpo* ObtenerDatos(b2Body* cuerpo);
    bool EsPar(DatosCuerpo* a, DatosCuerpo* b, TipoObjeto t1, TipoObjeto t2);
};
#endif