#ifndef PROYECTIL_H
#define PROYECTIL_H
#include "raylib.h"
class Proyectil
{
private:
    Vector2 posicion;       
    Vector2 velocidad;      
    float gravedad;         
    float radio;            
    bool activo;           
    Color color;            
    Vector2 posicionesEstela[10];
    int indiceEstela;
public:
    Proyectil();
    ~Proyectil();
    void Disparar(Vector2 posInicial, float angulo, float potencia);
    void Actualizar(float deltaTiempo);
    void Dibujar();
    Vector2 ObtenerPosicion() const;
    float ObtenerRadio() const;
    bool EstaActivo() const;
    void EstablecerActivo(bool valor);
    bool FueraDePantalla(int anchoPantalla, int altoPantalla);
};
#endif 