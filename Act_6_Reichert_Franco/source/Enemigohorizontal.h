#ifndef ENEMIGO_HORIZONTAL_H
#define ENEMIGO_HORIZONTAL_H
#include "Enemigo.h"
class EnemigoHorizontal : public Enemigo
{
private:
    float amplitud;     
    float frecuencia;   
    float tiempo;       
    bool oscila;        
public:
    EnemigoHorizontal();
    EnemigoHorizontal(Vector2 pos, float velocidadX, bool oscilar = false);
    ~EnemigoHorizontal();
    void Actualizar(float deltaTiempo) override;
    void Dibujar() override;
};
#endif 