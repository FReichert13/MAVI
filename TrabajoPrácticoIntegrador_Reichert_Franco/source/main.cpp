#include "raylib.h"
#include "Juego.h"
int main() {
	//creando la instancia del juego
	Juego juego;
	juego.Inicializar();
	//loop principal
	while (!juego.DebeTerminar()) {
		float deltaTime = GetFrameTime();
		juego.ProcesarInput();
		juego.Actualizar(deltaTime);
		juego.Dibujar();
	}
	CloseWindow();
	return 0;
}