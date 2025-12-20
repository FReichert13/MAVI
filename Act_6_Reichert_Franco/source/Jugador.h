#ifndef JUGADOR_H
#define JUGADOR_H
#include "raylib.h"
#include "Proyectil.h"
class Jugador
{
private:
    Vector2 posicion;          
    float angulo;               
    float potencia;             
    float anguloMinimo;        
    float anguloMaximo;        
    float potenciaMinima;       
    float potenciaMaxima;       
    float velocidadAngulo;      
    float velocidadPotencia;  
    Texture2D sprite;          
    float escalaSprite;         
    static const int MAX_PROYECTILES = 5;
    Proyectil proyectiles[MAX_PROYECTILES];
public:
    Jugador();
    ~Jugador();
    void Inicializar(Vector2 pos);
    void CargarSprite(const char* ruta);
    void DescargarSprite();
    void ProcesarEntrada();
    void Actualizar(float deltaTiempo);
    void Dibujar();
    void DibujarIndicadorApuntado();
    Vector2 ObtenerPosicion() const;
    float ObtenerAngulo() const;
    float ObtenerPotencia() const;
    Proyectil* ObtenerProyectiles();
    int ObtenerMaxProyectiles() const;
    void Disparar();
};
#endif 