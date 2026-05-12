#include "SoundManager.h"
SoundManager::SoundManager() {
    InitAudioDevice();   //se llama una sola vez en todo el programa
    //musica de fondo (archivo largo)
    music = LoadMusicStream("assets/fondo.mp3");
    //archivos cortos
    hitSfx = LoadSound("assets/vida.mp3");
    gameOverSfx = LoadSound("assets/gameover.mp3");
    pickupSfx = LoadSound("assets/estrella.mp3");
}
SoundManager::~SoundManager() {
    UnloadMusicStream(music);
    UnloadSound(hitSfx);
    UnloadSound(gameOverSfx);
    UnloadSound(pickupSfx);
    CloseAudioDevice();
}
void SoundManager::update() {
    UpdateMusicStream(music);
}
void SoundManager::playMusic() {
    PlayMusicStream(music);
}
void SoundManager::stopMusic() {
    StopMusicStream(music);
}
void SoundManager::playHit() {
    PlaySound(hitSfx);
}
void SoundManager::playGameOver() {
    PlaySound(gameOverSfx);
}
void SoundManager::playPickup() {
    PlaySound(pickupSfx);
}