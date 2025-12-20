#ifndef ENEMIGO_H
#define ENEMIGO_H
#include "raylib.h"
class Enemigo
{
protected:
    Vector2 posicion;       
    Vector2 velocidad;      
    float radio;          
    Texture2D sprite;      
    bool activo;            
    int puntos;          
public:
    Enemigo();
    Enemigo(Vector2 pos, Vector2 vel, float rad, int pts);
    virtual ~Enemigo();
    virtual void Actualizar(float deltaTiempo) = 0;
    virtual void Dibujar() = 0;
    void CargarSprite(const char* ruta);
    void DescargarSprite();
    Vector2 ObtenerPosicion() const;
    float ObtenerRadio() const;
    bool EstaActivo() const;
    int ObtenerPuntos() const;
    void EstablecerActivo(bool valor);
    virtual bool FueraDePantalla(int anchoPantalla, int altoPantalla);
};
#endif 