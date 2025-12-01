#ifndef JUEGO_H
#define JUEGO_H
#include "raylib.h"
#include "Jugador.h"
#include "Enemigo.h"
//posibles estados
enum EstadoJuego {
    MENU,
    JUGANDO,
    GANADO,
    PERDIDO
};
//clase principal
class Juego {
private:
    //configuracion de pantalla
    const int screenWidth;
    const int screenHeight;
    EstadoJuego estadoActual;
    Jugador* jugador;
    Enemigo* enemigo1;
    Enemigo* enemigo2;
    float groundLevel;
    Rectangle suelo;
    Rectangle plataforma1;
    Rectangle plataforma2;
    Rectangle plataforma3;
    Rectangle objetivo;
    Rectangle botonJugar;
    Texture2D fondoTexture;
    Texture2D objetivoTexture;
    bool fondoTextureLoaded;
    bool objetivoTextureLoaded;
public:
    Juego();
    ~Juego();
    void Inicializar();
    void CargarRecursos();
    void ProcesarInput();
    void Actualizar(float deltaTime);
    void Dibujar();
    void Reiniciar();
    bool DebeTerminar() const;
private:
    void DibujarMenu();
    void DibujarJuego();
    void DibujarPantallaPerdido();
    void DibujarPantallaGanado();
    void VerificarColisiones();
    bool MouseSobreRectangulo(Rectangle rect);
};
#endif