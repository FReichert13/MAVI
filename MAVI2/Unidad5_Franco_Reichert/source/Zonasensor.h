#ifndef ZONA_SENSOR_H
#define ZONA_SENSOR_H
#include <box2d.h>
#include "raylib.h"
#include "Etiquetas.h"
#include "Constantes.h"
//detecta entrada/salida del proyectil pero no afecta su movimiento
class ZonaSensor
{
public:
    ZonaSensor(b2World* mundo, float cxPix, float cyPix, float anchoPix, float altoPix);
    ~ZonaSensor();
    void EstablecerActiva(bool a) { activa = a; }
    void Dibujar() const;
private:
    b2Body* cuerpo;
    float       ancho, alto;   //en pixeles
    bool        activa;
    DatosCuerpo datos;
};
#endif