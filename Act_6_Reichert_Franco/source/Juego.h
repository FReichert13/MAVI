#ifndef JUEGO_H
#define JUEGO_H
#include "raylib.h"
#include "Jugador.h"
#include "Enemigo.h"
#include "EnemigoQueCae.h"
#include "EnemigoHorizontal.h"
#include "EnemigoQueRebota.h"
#include <vector>
enum class EstadoJuego
{
    MENU,
    JUGANDO,
    PAUSADO,
    FIN_JUEGO
};
class Juego
{
private:
    int anchoPantalla;
    int altoPantalla;
    const char* titulo;
    EstadoJuego estado;
    int puntaje;
    int vidas;
    int oleada;
    float temporizadorSpawn;
    float intervaloSpawn;
    Texture2D fondo;
    Jugador jugador;
    std::vector<Enemigo*> enemigos;
    int enemigosDestruidos;
    int enemigosEscapados;
    void CrearEnemigo();
    void VerificarColisiones();
    void ActualizarEnemigos(float deltaTiempo);
    void DibujarHUD();
    void DibujarMenu();
    void DibujarFinJuego();
    void LimpiarEnemigos();
public:
    Juego(int ancho, int alto, const char* tituloVentana);
    ~Juego();
    void Inicializar();
    void CargarRecursos();
    void DescargarRecursos();
    void Ejecutar();
    void ProcesarEntrada();
    void Actualizar();
    void Dibujar();
    void IniciarJuego();
    void PausarJuego();
    void ReiniciarJuego();
};
#endif 