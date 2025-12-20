#ifndef ENEMIGO_QUE_CAE_H
#define ENEMIGO_QUE_CAE_H
#include "Enemigo.h"
class EnemigoQueCae : public Enemigo
{
private:
    float gravedad;     
public:
    EnemigoQueCae();
    EnemigoQueCae(Vector2 pos, float velocidadInicialY, float grav);
    ~EnemigoQueCae();
    void Actualizar(float deltaTiempo) override;
    void Dibujar() override;
};
#endif 