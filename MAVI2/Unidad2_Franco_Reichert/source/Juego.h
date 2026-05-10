#pragma once
#include <box2d.h>
#include "raylib.h"
#include <vector>
#include "Proyectil.h"
#include "Caja.h"
class Juego
{
public:
    Juego(int ancho = 1000, int alto = 600);
    ~Juego();
    void Ejecutar();
private:
    void Iniciar();
    void CrearLimites();
    void CrearMuroEstatico(float cxPx, float cyPx, float halfWPx, float halfHPx);
    void CrearEscena();
    void ProcesarInput();
    void Actualizar();
    void Dibujar();
    void DibujarHUDCarga();
    void Finalizar();
    int anchoVentana;
    int altoVentana;
    b2World* mundo;
    Proyectil* proyectil;
    std::vector<Caja> cajas;
    b2Vec2 impulsoMaximo;
    //acumuladar carga mientras apretas el espacio
    float potenciaCarga;
    //tiempo necesario para llegar al 100%
    float tiempoCargaTotal;
    //potencia minima
    float potenciaMinima;
    //porcentaje potencia
    int ultimaPotenciaDisparada;
};