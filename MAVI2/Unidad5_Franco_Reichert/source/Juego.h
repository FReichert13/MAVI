#ifndef JUEGO_H
#define JUEGO_H
#include <box2d.h>
#include "raylib.h"
#include "Etiquetas.h"
#include "Constantes.h"
#include "ContactListener.h"
#include "Proyectil.h"
#include "Objetivo.h"
#include "ZonaSensor.h"
//estados del sistema: definen que puede ocurrir en cada momento
enum class GameState
{
    Waiting,        //listo para disparar, se apunta
    Running,        //proyectil en movimiento, la simulacion avanza
    EventDetected,  //evento detectado, se procesa la respuesta
    Finished        //resultado mostrado, espera reinicio
};
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
    bool DebeCerrar();
    void ConstruirEscena();
    void LiberarEscena();
    void ReiniciarRonda();
    void DispararProyectil();
    void PasoFisico();
    bool ProyectilFueraDePantalla() const;
    void DibujarTrayectoria();
    void DibujarCanon();
    const char* NombreEstado() const;
    const int anchoVentana = 1000;
    const int altoVentana = 600;
    Color colorFondo;
    b2World* mundo;
    ContactListener listener;
    Proyectil* proyectil;   //null mientras se apunta, se crea al disparar
    Objetivo* objetivo;
    ZonaSensor* zona;
    DatosCuerpo datosSuelo;
    GameState estado;
    bool acierto;           //interpretacion del ultimo evento
    bool pasoPorZona;       //el tiro atraveso la zona sensor
    int  cuadrosRespuesta;  //duracion de EventDetected en frames
    int  aciertos;
    int  disparos;
    //control del disparo
    float anguloGrados;   //0 = horizontal, 90 = vertical hacia arriba
    float potencia;       //velocidad inicial en m/s
    const float anguloMin = 5.0f, anguloMax = 85.0f;
    const float potenciaMin = 8.0f, potenciaMax = 32.0f;
    const float xCanon = 90.0f;
    const float yCanon = 430.0f;
    const float radioProyectil = 12.0f;
};
#endif