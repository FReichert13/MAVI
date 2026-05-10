#ifndef NDEBUG
#include <vld.h>
#endif
#include "Juego.h"
//funcion principal
int main(void)
{
    Juego juego(1000, 600);
    juego.Ejecutar();
    return 0;
}