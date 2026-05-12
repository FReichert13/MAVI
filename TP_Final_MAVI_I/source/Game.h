#pragma once
#include "raylib.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "HUD.h"
#include "SoundManager.h"
#include <vector>
//estados del juego
//menu-playing-gameover-playing o menu
enum GameState { MENU, PLAYING, GAMEOVER };
class Game {
public:
    Game();
    ~Game();
    void run();   //bucle principal del juego
private:
    static constexpr int SCREEN_WIDTH = 800;
    static constexpr int SCREEN_HEIGHT = 600;
    GameState state;
    int  menuOption;     //0 = jugar, 1 = salir
    bool shouldExit;     //se vuelve true si el jugador elige salir
    //texturas cargadas una sola vez al inicio
    Texture2D bgTex, asteroidTex, debrisTex, droneTex,
        playerTex, crystalTex, heartTex;
    Player* player;
    HUD* hud;
    SoundManager* sound;
    std::vector<Enemy*> enemies;   //punteros porque hay polimorfismo
    std::vector<Item>   items;     //objetos directos, no necesitan herencia
    //temporizadores para spawn periodico
    float enemySpawnTimer;
    float itemSpawnTimer;
    //metodos auxiliares
    void loadAssets();
    void unloadAssets();
    void update(float dt);
    void draw();
    void spawnEnemy();
    void spawnItem();
    void checkCollisions();
    void cleanInactive();
    void resetGame();
};