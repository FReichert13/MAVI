#include "Juego.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
//constructor
Juego::Juego(int ancho, int alto, const char* tituloVentana)
{
    anchoPantalla = ancho;
    altoPantalla = alto;
    titulo = tituloVentana;
    estado = EstadoJuego::MENU;
    puntaje = 0;
    vidas = 10;
    oleada = 1;
    temporizadorSpawn = 0.0f;
    intervaloSpawn = 1.0f;
    enemigosDestruidos = 0;
    enemigosEscapados = 0;
    fondo = { 0 };
}
//destructor
Juego::~Juego()
{
    LimpiarEnemigos();
    DescargarRecursos();
}
void Juego::Inicializar()
{
    InitWindow(anchoPantalla, altoPantalla, titulo);
    SetTargetFPS(60);
    srand((unsigned int)time(NULL));
    CargarRecursos();
    jugador.Inicializar({ 80.0f, (float)altoPantalla - 80.0f });
}
void Juego::CargarRecursos()
{
    //fondo
    fondo = LoadTexture("Sprites/Sprite_fondo.png");
    //player
    jugador.CargarSprite("Sprites/Sprite1.png");
}
void Juego::DescargarRecursos()
{
    if (fondo.id != 0) UnloadTexture(fondo);
    jugador.DescargarSprite();
}
//loop principal
void Juego::Ejecutar()
{
    while (!WindowShouldClose())
    {
        ProcesarEntrada();
        Actualizar();
        Dibujar();
    }
    CloseWindow();
}
void Juego::ProcesarEntrada()
{
    switch (estado)
    {
    case EstadoJuego::MENU:
        if (IsKeyPressed(KEY_ENTER))
        {
            IniciarJuego();
        }
        if (IsKeyPressed(KEY_ESCAPE))
        {
        }
        break;

    case EstadoJuego::JUGANDO:
        jugador.ProcesarEntrada(); //input

        if (IsKeyPressed(KEY_P))
        {
            PausarJuego();
        }
        if (IsKeyPressed(KEY_ESCAPE))
        {
            estado = EstadoJuego::MENU;
        }
        break;

    case EstadoJuego::PAUSADO:
        if (IsKeyPressed(KEY_P))
        {
            estado = EstadoJuego::JUGANDO;
        }
        if (IsKeyPressed(KEY_ESCAPE))
        {
            estado = EstadoJuego::MENU;
        }
        break;

    case EstadoJuego::FIN_JUEGO:
        if (IsKeyPressed(KEY_ENTER))
        {
            ReiniciarJuego();
            IniciarJuego();
        }
        if (IsKeyPressed(KEY_ESCAPE))
        {
            estado = EstadoJuego::MENU;
        }
        break;
    }
}
void Juego::Actualizar()
{
    if (estado != EstadoJuego::JUGANDO) return;

    float deltaTiempo = GetFrameTime();
    jugador.Actualizar(deltaTiempo);
    ActualizarEnemigos(deltaTiempo);

    //aaparicion de los enemigos
    temporizadorSpawn += deltaTiempo;
    if (temporizadorSpawn >= intervaloSpawn)
    {
        CrearEnemigo();
        temporizadorSpawn = 0.0f;
        if (intervaloSpawn > 0.4f)
        {
            intervaloSpawn -= 0.03f;
        }
    }
    VerificarColisiones();
    if (vidas <= 0)
    {
        estado = EstadoJuego::FIN_JUEGO;
    }
    if (enemigosDestruidos > 0 && enemigosDestruidos % 10 == 0)
    {
        oleada = (enemigosDestruidos / 10) + 1;
    }
}
void Juego::ActualizarEnemigos(float deltaTiempo)
{
    for (auto it = enemigos.begin(); it != enemigos.end(); )
    {
        Enemigo* enemigo = *it;

        if (enemigo->EstaActivo())
        {
            enemigo->Actualizar(deltaTiempo);
            if (enemigo->FueraDePantalla(anchoPantalla, altoPantalla))
            {
                enemigo->EstablecerActivo(false);
                vidas--;           //-1 vida cuando te tocan
                enemigosEscapados++;
            }
        }
        if (!enemigo->EstaActivo())
        {
            delete enemigo;
            it = enemigos.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
void Juego::CrearEnemigo()
{
    int tipoEnemigo = rand() % 3;
    Enemigo* nuevoEnemigo = nullptr;

    switch (tipoEnemigo)
    {
    case 0:
    {
        float x = 150.0f + (float)(rand() % (anchoPantalla - 200));
        Vector2 pos = { x, -30.0f };
        float gravedad = 200.0f + (float)(rand() % 200);

        EnemigoQueCae* cayendo = new EnemigoQueCae(pos, 0.0f, gravedad);
        cayendo->CargarSprite("Sprites/Sprite2.png");
        nuevoEnemigo = cayendo;
        break;
    }

    case 1:
    {
        float y = 100.0f + (float)(rand() % (altoPantalla - 300));
        bool desdeIzquierda = (rand() % 2) == 0;
        float x = desdeIzquierda ? -30.0f : (float)anchoPantalla + 30.0f;
        float velocidadX = desdeIzquierda ? (150.0f + (float)(rand() % 100)) : -(150.0f + (float)(rand() % 100));

        Vector2 pos = { x, y };
        EnemigoHorizontal* horizontal = new EnemigoHorizontal(pos, velocidadX, (rand() % 2) == 0);
        horizontal->CargarSprite("Sprites/Sprite3.png");
        nuevoEnemigo = horizontal;
        break;
    }

    case 2:
    {
        float x = 100.0f + (float)(rand() % (anchoPantalla - 200));
        Vector2 pos = { x, 50.0f };
        float velX = -50.0f + (float)(rand() % 100);
        float velY = 50.0f + (float)(rand() % 100);
        Vector2 vel = { velX, velY };
        float gravedad = 300.0f + (float)(rand() % 200);

        EnemigoQueRebota* rebotando = new EnemigoQueRebota(pos, vel, gravedad, (float)altoPantalla - 50.0f);
        rebotando->CargarSprite("Sprites/Sprite4.png");
        nuevoEnemigo = rebotando;
        break;
    }
    }
    if (nuevoEnemigo != nullptr)
    {
        enemigos.push_back(nuevoEnemigo);
    }
}
void Juego::VerificarColisiones()
{
    Proyectil* proyectiles = jugador.ObtenerProyectiles();
    int maxProyectiles = jugador.ObtenerMaxProyectiles();
    for (int i = 0; i < maxProyectiles; i++)
    {
        if (!proyectiles[i].EstaActivo()) continue;

        Vector2 posProyectil = proyectiles[i].ObtenerPosicion();
        float radioProyectil = proyectiles[i].ObtenerRadio();

        for (Enemigo* enemigo : enemigos)
        {
            if (!enemigo->EstaActivo()) continue;

            Vector2 posEnemigo = enemigo->ObtenerPosicion();
            float radioEnemigo = enemigo->ObtenerRadio();
            float dx = posProyectil.x - posEnemigo.x;
            float dy = posProyectil.y - posEnemigo.y;
            float distancia = sqrtf(dx * dx + dy * dy);

            if (distancia < radioProyectil + radioEnemigo)
            {
                proyectiles[i].EstablecerActivo(false);
                enemigo->EstablecerActivo(false);
                puntaje += enemigo->ObtenerPuntos();
                enemigosDestruidos++;
            }
        }
    }
    Vector2 posJugador = jugador.ObtenerPosicion();
    float radioJugador = 40.0f; 

    for (Enemigo* enemigo : enemigos)
    {
        if (!enemigo->EstaActivo()) continue;

        Vector2 posEnemigo = enemigo->ObtenerPosicion();
        float radioEnemigo = enemigo->ObtenerRadio();
        float dx = posJugador.x - posEnemigo.x;
        float dy = posJugador.y - posEnemigo.y;
        float distancia = sqrtf(dx * dx + dy * dy);

        if (distancia < radioJugador + radioEnemigo)
        {
        //toque con el enemigo
            vidas = 0;
            return;
        }
    }
}
void Juego::Dibujar()
{
    BeginDrawing();
    ClearBackground(DARKGRAY);
    if (fondo.id != 0)
    {
        Rectangle rectOrigen = { 0.0f, 0.0f, (float)fondo.width, (float)fondo.height };
        Rectangle rectDestino = { 0.0f, 0.0f, (float)anchoPantalla, (float)altoPantalla };
        DrawTexturePro(fondo, rectOrigen, rectDestino, { 0, 0 }, 0.0f, WHITE);
    }
    switch (estado)
    {
    case EstadoJuego::MENU:
        DibujarMenu();
        break;

    case EstadoJuego::JUGANDO:
    case EstadoJuego::PAUSADO:
        for (Enemigo* enemigo : enemigos)
        {
            enemigo->Dibujar();
        }
        jugador.Dibujar();
        DibujarHUD();
        if (estado == EstadoJuego::PAUSADO)
        {
            DrawRectangle(0, 0, anchoPantalla, altoPantalla, Fade(BLACK, 0.5f));
            const char* textoPausa = "Pausa";
            int anchoTexto = MeasureText(textoPausa, 60);
            DrawText(textoPausa, (anchoPantalla - anchoTexto) / 2, altoPantalla / 2 - 30, 60, WHITE);
            DrawText("Presiona P para continuar", (anchoPantalla - MeasureText("Presiona P para continuar", 20)) / 2, altoPantalla / 2 + 50, 20, LIGHTGRAY);
        }
        break;

    case EstadoJuego::FIN_JUEGO:
        for (Enemigo* enemigo : enemigos)
        {
            enemigo->Dibujar();
        }
        jugador.Dibujar();
        DibujarFinJuego();
        break;
    }

    EndDrawing();
}
void Juego::DibujarHUD()
{
    DrawRectangle(10, 10, 200, 140, Fade(BLACK, 0.7f));
    DrawRectangleLines(10, 10, 200, 140, WHITE);
    //txt hud
    DrawText(TextFormat("PUNTAJE: %d", puntaje), 20, 20, 20, YELLOW);
    DrawText(TextFormat("ESCAPARON: %d", vidas), 20, 45, 20, RED); //enemigos que se escaparon, si llegas a 10 que se fueron perdes
    DrawText(TextFormat("OLEADA: %d", oleada), 20, 70, 20, GREEN);
    DrawText(TextFormat("ANGULO: %.1f", jugador.ObtenerAngulo()), 20, 95, 18, SKYBLUE);
    DrawText(TextFormat("POTENCIA: %.0f", jugador.ObtenerPotencia()), 20, 118, 18, ORANGE);
    //barra potencia 
    DrawRectangle(anchoPantalla - 40, 100, 20, 400, Fade(GRAY, 0.5f));
    float porcentajePotencia = (jugador.ObtenerPotencia() - 200.0f) / 600.0f;  
    int alturaBarra = (int)(400.0f * porcentajePotencia);
    DrawRectangle(anchoPantalla - 40, 500 - alturaBarra, 20, alturaBarra, ORANGE);
    DrawRectangleLines(anchoPantalla - 40, 100, 20, 400, WHITE);
    DrawText("POT", anchoPantalla - 45, 510, 15, WHITE);

    //controles
    DrawText("Controles:", anchoPantalla - 150, 20, 15, WHITE);
    DrawText("[ARRIBA/ABAJO] Angulo", anchoPantalla - 150, 40, 12, LIGHTGRAY);
    DrawText("[IZQ/DER] Potencia", anchoPantalla - 150, 55, 12, LIGHTGRAY);
    DrawText("[ESPACIO] Disparar", anchoPantalla - 150, 70, 12, LIGHTGRAY);
    DrawText("[P] Pausa", anchoPantalla - 150, 85, 12, LIGHTGRAY);
}
void Juego::DibujarMenu()
{
    //titulo del juego
    const char* textoTitulo = "Galaga con Fisica";
    int anchoTitulo = MeasureText(textoTitulo, 60);
    DrawText(textoTitulo, (anchoPantalla - anchoTitulo) / 2, 150, 60, YELLOW);
    //subtitu
    const char* subtitulo = "MRU, MRUV y Tiro Oblicuo";
    int anchoSub = MeasureText(subtitulo, 20);
    DrawText(subtitulo, (anchoPantalla - anchoSub) / 2, 220, 20, LIGHTGRAY);
    //instrucciones
    DrawText("PRESIONA ENTER PARA COMENZAR", (anchoPantalla - MeasureText("PRESIONA ENTER PARA COMENZAR", 25)) / 2, 350, 25, WHITE);
    //tipos de enemigos explicados
    DrawText("Tipos de enemigos:", 100, 420, 20, GREEN);
    DrawText("- Nave Roja: Cae con gravedad (MRUV)", 120, 450, 16, RED);
    DrawText("- Nave Azul: Se mueve horizontal (MRU)", 120, 475, 16, BLUE);
    DrawText("- Nave Verde: Rebota en el suelo (MRUV + Rebote)", 120, 500, 16, GREEN);
}
void Juego::DibujarFinJuego()
{
    DrawRectangle(0, 0, anchoPantalla, altoPantalla, Fade(BLACK, 0.8f));
    //txt Game Over
    const char* textoFinJuego = "FIN DEL JUEGO";
    int anchoTexto = MeasureText(textoFinJuego, 70);
    DrawText(textoFinJuego, (anchoPantalla - anchoTexto) / 2, 180, 70, RED);
    //stats
    DrawText(TextFormat("Puntaje Final: %d", puntaje), (anchoPantalla - MeasureText(TextFormat("Puntaje Final: %d", puntaje), 30)) / 2, 280, 30, YELLOW);
    DrawText(TextFormat("Enemigos Destruidos: %d", enemigosDestruidos), (anchoPantalla - MeasureText(TextFormat("Enemigos Destruidos: %d", enemigosDestruidos), 20)) / 2, 330, 20, GREEN);
    DrawText(TextFormat("Enemigos Escapados: %d", enemigosEscapados), (anchoPantalla - MeasureText(TextFormat("Enemigos Escapados: %d", enemigosEscapados), 20)) / 2, 360, 20, RED);
    DrawText(TextFormat("Oleada Alcanzada: %d", oleada), (anchoPantalla - MeasureText(TextFormat("Oleada Alcanzada: %d", oleada), 20)) / 2, 390, 20, SKYBLUE);
    //opciones
    DrawText("PRESIONA ENTER PARA REINICIAR", (anchoPantalla - MeasureText("PRESIONA ENTER PARA REINICIAR", 22)) / 2, 470, 22, WHITE);
    DrawText("PRESIONA ESC PARA MENU", (anchoPantalla - MeasureText("PRESIONA ESC PARA MENU", 18)) / 2, 510, 18, LIGHTGRAY);
}
void Juego::IniciarJuego()
{
    estado = EstadoJuego::JUGANDO;
    ReiniciarJuego();
}
void Juego::PausarJuego()
{
    if (estado == EstadoJuego::JUGANDO)
    {
        estado = EstadoJuego::PAUSADO;
    }
    else if (estado == EstadoJuego::PAUSADO)
    {
        estado = EstadoJuego::JUGANDO;
    }
}
void Juego::ReiniciarJuego()
{
    puntaje = 0;
    vidas = 10;
    oleada = 1;
    temporizadorSpawn = 0.0f;
    intervaloSpawn = 1.0f;
    enemigosDestruidos = 0;
    enemigosEscapados = 0;
    LimpiarEnemigos();
    jugador.Inicializar({ 80.0f, (float)altoPantalla - 80.0f });
}
void Juego::LimpiarEnemigos()
{
    for (Enemigo* enemigo : enemigos)
    {
        delete enemigo;
    }
    enemigos.clear();
}