#pragma once
#include "raylib.h"
#include <box2d.h>
#include <vector>
#include "Caja.h"
#include "Circulo.h"
#include "PuenteColgante.h"
#include "GruaMovil.h"
class Juego
{
public:
    Juego();
    ~Juego();
    void Ejecutar();
private:
    void Inicializar();
    void ProcesarInput();
    void Actualizar();
    void Dibujar();
    void DibujarUI();
    void Liberar();
    bool DebeCerrar();
    void CrearEstructurasEstaticas();
    void CrearMecanismos();
    //ventana
    const int anchoVentana = 1000;
    const int altoVentana = 600;
    Color colorFondo;
    Color colorSuelo;
    //fisicas
    b2World* mundo;
    //cuerpos estaticos
    b2Body* rail;                 
    b2Body* anclaPuenteIzq;      
    b2Body* anclaPuenteDer;      
    //pos para los dibujos
    float xCarritoInic, yCarrito;
    float xAnclaPuenteIzq, xAnclaPuenteDer, yAnclaPuente;
    float anchoAncla, altoAncla;
    //mecanismos
    GruaMovil* grua;
    PuenteColgante* puente;
    //cuerpos dinamicos
    std::vector<Caja*>    cajas;
    std::vector<Circulo*> circulos;
};