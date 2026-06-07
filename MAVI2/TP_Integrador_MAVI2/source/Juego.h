#pragma once
#include "raylib.h"
#include <box2d.h>
#include <vector>
#include "Etiqueta.h"
#include "Caja.h"
#include "PuenteCorto.h"
#include "Grua.h"
#include "ListenerColisiones.h"
enum class EstadoJuego { Jugando, Ganaste, Perdiste };
class Juego
{
public:
    Juego();
    ~Juego();
    void Ejecutar();
private:
    void Inicializar();
    void ConstruirEscena();
    void CrearEscenarioEstatico();
    void CrearPuentes();
    void CrearGrua();
    void GenerarCajaColgante();
    void CambiarViento();
    void AplicarViento();
    void ProcesarInput();
    void Actualizar();
    void ActualizarLogica();
    int  PuenteDebajo(b2Vec2 p) const;
    void Dibujar();
    void DibujarUI();
    void Reiniciar();
    void LiberarEscena();
    bool DebeCerrar();
    void CargarTexturas();
    void DescargarTexturas();
    void CargarSonidos();
    void DescargarSonidos();
    //ventana
    const int anchoVentana = 1280;
    const int altoVentana = 900;
    Color colorFondo;
    // texturas
    Texture2D texFondo, texSuelo, texCarrito, texMastil, texGancho, texCaja, texTablon, texTorre;
    // sonidos
    Sound sndSoltar, sndColocada, sndPerdida, sndGanar, sndPerder;
    Music vientoMusica; //sonido de viento en loop 
    // fisica
    b2World* mundo = nullptr;
    ListenerColisiones listener;
    //cuerpos estaticos
    b2Body* rail = nullptr;
    Etiqueta etiquetaSuelo;
    std::vector<Rectangle> dibujoTorres; //torres para dibujar
    //mecanismos
    Grua* grua = nullptr;
    std::vector<PuenteCorto*> puentes;
    std::vector<bool>         puenteListo;
    //cajas
    std::vector<Caja*> cajas;
    Caja* cajaColgando = nullptr;
    //estado de juego
    EstadoJuego estado = EstadoJuego::Jugando;
    int         cajasDisponibles = 0;
    int         puentesCompletos = 0;
    int         perdidasPrevias = 0; //para detectar cuando se pierde una caja nueva
    //viento
    float vientoAccel = 0.0f; //aceleracion lateral actual
    float vientoTimer = 0.0f; //seg hasta el proximo cambio
    const float vientoMin = 35.0f;        //intensidad minima
    const float vientoMax = 65.0f;        //intensidad maxima
    const float factorVientoPuente = 0.15f;//el puente apenas se mueve
    //parametros de juego
    const int   totalCajas = 5;     //cajas que se pueden usar en total
    const int   objetivo = 3;     //puentes a completar para ganar
    const float umbralAng = 0.9f; 
    const float tiempoNecesario = 1.6f;  //seg quieta sobre el puente para contar
    const float tolPosicion = 10.0f; //px que puede moverse y seguir contando como quieta
    const float yRail = 70.0f;
    const float puenteHalfSpan = 44.0f; //medio ancho del tablero
    const float puenteDeckY = 720.0f;//altura del tablero
    std::vector<float> centrosPuente;
};