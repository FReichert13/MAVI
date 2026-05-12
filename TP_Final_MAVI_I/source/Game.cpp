#include "Game.h"
Game::Game() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Survival - Franco Reichert");
    SetTargetFPS(60);
    loadAssets();
    sound = new SoundManager();   //inicializa el audio
    //punto de arranque del jugador
    player = new Player(&playerTex, { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f });
    hud = new HUD(&heartTex);
    state = MENU;
    menuOption = 0;
    shouldExit = false;
    enemySpawnTimer = 0.0f;
    itemSpawnTimer = 0.0f;
}
Game::~Game() {
    //liberar enemigos y items
    for (Enemy* e : enemies) delete e;
    enemies.clear();
    items.clear();
    delete player;
    delete hud;
    delete sound;       //libera audio device antes de cerrar la ventana
    unloadAssets();
    CloseWindow();
}
//carga de las texturas
void Game::loadAssets() {
    bgTex = LoadTexture("assets/espacio.png");
    asteroidTex = LoadTexture("assets/asteroide.png");
    debrisTex = LoadTexture("assets/basura.png");
    droneTex = LoadTexture("assets/nave_enemiga.png");
    playerTex = LoadTexture("assets/nave_jugador.png");
    crystalTex = LoadTexture("assets/estrella.png");
    heartTex = LoadTexture("assets/vidas.png");
}
void Game::unloadAssets() {
    UnloadTexture(bgTex);
    UnloadTexture(asteroidTex);
    UnloadTexture(debrisTex);
    UnloadTexture(droneTex);
    UnloadTexture(playerTex);
    UnloadTexture(crystalTex);
    UnloadTexture(heartTex);
}
//bucle principal
void Game::run() {
    while (!WindowShouldClose() && !shouldExit) {
        update(GetFrameTime());

        BeginDrawing();
        draw();
        EndDrawing();
    }
}
void Game::update(float dt) {
    sound->update();   //la musica se actualiza siempre
    switch (state) {
    case MENU:
        //navegacion por el menu con flechas
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
            menuOption = (menuOption + 1) % 2;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            if (menuOption == 0) {            //jugar
                resetGame();
                sound->playMusic();
                state = PLAYING;
            }
            else {                          //salir
                shouldExit = true;
            }
        }
        break;
    case PLAYING: {
        player->update(dt);
        //solo actualizamos los enemigos activos
        for (Enemy* e : enemies) {
            if (e->isActive()) e->update(dt);
        }
        //spawn periodico por temporizador
        enemySpawnTimer += dt;
        if (enemySpawnTimer > 1.0f) { spawnEnemy(); enemySpawnTimer = 0.0f; }
        itemSpawnTimer += dt;
        if (itemSpawnTimer > 2.0f) { spawnItem();  itemSpawnTimer = 0.0f; }
        checkCollisions();
        cleanInactive();
        //game over es la unica condicion de fin del juego
        if (!player->isAlive()) {
            sound->stopMusic();
            sound->playGameOver();
            state = GAMEOVER;
        }
        break;
    }
    case GAMEOVER:
        if (IsKeyPressed(KEY_R)) {            //reiniciar
            resetGame();
            sound->playMusic();
            state = PLAYING;
        }
        if (IsKeyPressed(KEY_M)) {            //volver al menu
            state = MENU;
            menuOption = 0;
        }
        break;
    }
}
//dibuja la pantalla segun el estado actual
void Game::draw() {
    //fondo escalado a la pantalla completa
    DrawTexturePro(bgTex,
        { 0, 0, (float)bgTex.width, (float)bgTex.height },
        { 0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT },
        { 0, 0 }, 0.0f, WHITE);
    switch (state) {
    case MENU: {
        DrawText("Space Survival", SCREEN_WIDTH / 2 - 220, 150, 40, WHITE);
        //la opcion seleccionada se ve en amarillo
        Color cJugar = (menuOption == 0) ? YELLOW : WHITE;
        Color cSalir = (menuOption == 1) ? YELLOW : WHITE;
        DrawText("Jugar", SCREEN_WIDTH / 2 - 50, 300, 30, cJugar);
        DrawText("Salir", SCREEN_WIDTH / 2 - 50, 360, 30, cSalir);
        DrawText("flechas para elegir - enter para confirmar",
            SCREEN_WIDTH / 2 - 230, 500, 18, GRAY);
        break;
    }
    case PLAYING:
        for (Enemy* e : enemies) if (e->isActive()) e->draw();
        for (Item& it : items)   if (it.isActive()) it.draw();
        player->draw();
        hud->draw(player->getLives(), player->getScore());
        break;
    case GAMEOVER:
        DrawText("Game Over", SCREEN_WIDTH / 2 - 130, 200, 50, RED);
        DrawText(TextFormat("Puntaje final: %d", player->getScore()),
            SCREEN_WIDTH / 2 - 140, 280, 24, WHITE);
        DrawText("'R' para reiniciar", SCREEN_WIDTH / 2 - 110, 340, 20, WHITE);
        DrawText("'M' para volver al menu", SCREEN_WIDTH / 2 - 150, 380, 20, WHITE);
        break;
    }
}
//Spawn de enemigos SE elige al azar uno de los 3 tipos de movimiento
void Game::spawnEnemy() {
    if (enemies.size() >= 12) return;   //limite para no saturar la pantalla
    int type = GetRandomValue(0, 2);
    Enemy* e = nullptr;
    if (type == 0) {
        //ssteroide cruza horizontal a velocidad constante.
        float y = (float)GetRandomValue(50, SCREEN_HEIGHT - 100);
        bool fromLeft = GetRandomValue(0, 1) == 0;
        Vector2 pos = { fromLeft ? -60.0f : (float)SCREEN_WIDTH, y };
        Vector2 vel = { fromLeft ? 200.0f : -200.0f, 0.0f };
        e = new EnemyLinear(&asteroidTex, pos, vel);
    }
    else if (type == 1) {
        //basura cae desde arriba acelerando por gravedad
        float x = (float)GetRandomValue(20, SCREEN_WIDTH - 60);
        e = new EnemyAccelerated(&debrisTex, { x, -50.0f });
    }
    else {
        //nave enemiga orbita alrededor de un punto al azar
        Vector2 center = {
            (float)GetRandomValue(150, SCREEN_WIDTH - 150),
            (float)GetRandomValue(150, SCREEN_HEIGHT - 150)
        };
        float radius = (float)GetRandomValue(60, 120);
        float omega = 1.5f;   //velocidad angular
        e = new EnemyCircular(&droneTex, center, radius, omega);
    }
    enemies.push_back(e);
}
//crea una estrella en una posicion aleatoria de la pantalla
void Game::spawnItem() {
    Vector2 pos = {
        (float)GetRandomValue(20, SCREEN_WIDTH - 40),
        (float)GetRandomValue(20, SCREEN_HEIGHT - 40)
    };
    items.emplace_back(&crystalTex, pos);
}
//tipos de colisiones
void Game::checkCollisions() {
    Rectangle pBounds = player->getBounds();
    Vector2   pCenter = player->getCenter();
    float     pRadius = 20.0f;
    for (Enemy* e : enemies) {
        if (!e->isActive()) continue;
        if (CheckCollisionRecs(pBounds, e->getBounds())) {
            player->takeDamage();
            e->setActive(false);
            if (player->isAlive()) sound->playHit();
        }
    }
    for (Item& it : items) {
        if (!it.isActive()) continue;
        if (CheckCollisionCircles(pCenter, pRadius, it.getCenter(), it.getRadius())) {
            player->addScore(10);
            it.collect();
            sound->playPickup(); 
        }
    }
}
//elimina enemigos e items inactivos
void Game::cleanInactive() {
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (!(*it)->isActive()) { delete* it; it = enemies.erase(it); }
        else { ++it; }
    }
    for (auto it = items.begin(); it != items.end(); ) {
        if (!it->isActive()) it = items.erase(it);
        else                 ++it;
    }
}
//vuelve al estado inicial de una partida nueva
void Game::resetGame() {
    for (Enemy* e : enemies) delete e;
    enemies.clear();
    items.clear();
    player->reset({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f });
    enemySpawnTimer = 0.0f;
    itemSpawnTimer = 0.0f;
}