#ifndef ENEMIGO_QUE_REBOTA_H
#define ENEMIGO_QUE_REBOTA_H
#include "Enemigo.h"
class EnemigoQueRebota : public Enemigo
{
private:
    float gravedad;         
    float factorRebote;     
    float nivelSuelo;       
    int contadorRebotes;    
    int maxRebotes;         
public:
    EnemigoQueRebota();
    EnemigoQueRebota(Vector2 pos, Vector2 velInicial, float grav, float suelo);
    ~EnemigoQueRebota();
    void Actualizar(float deltaTiempo) override;
    void Dibujar() override;
};
#endif 