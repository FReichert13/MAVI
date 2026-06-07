#pragma once
//lo uso para identificar a que tipo de cuerpo del juego pertenece un b2Body
//guarda en el userdata del cuerpo para reconocerlo durante las colisiones
enum class TipoCuerpo { Suelo, Caja };
struct Etiqueta
{
    TipoCuerpo tipo;
    void* objeto; //puntero al objeto de juego (caja*) cuando aplica
};