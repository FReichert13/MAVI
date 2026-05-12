#pragma once
#include "raylib.h"
class SoundManager {
public:
    SoundManager();
    ~SoundManager();
    void update();         //mantiene la musica
    void playMusic();
    void stopMusic();
    void playHit();        //perder una vida
    void playGameOver();   //perder la partida
    void playPickup();     //juntar una estrellita
private:
    Music music;           
    Sound hitSfx;         
    Sound gameOverSfx;
    Sound pickupSfx;
};