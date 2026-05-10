#pragma once
//como box2d trabaja en unidades fisicas (metros en este caso) si le pasasba pixeles necesitaba impulsos gigantes para moverlos por lo que defini una escala de 30px = 1 metro, para poder ver la difrencia enntre los impulsos en la pantalla
const float PIXELS_POR_METRO = 30.0f;
//helpers para convertir entre unidades
inline float Pix2M(float pixeles) { return pixeles / PIXELS_POR_METRO; }
inline float M2Pix(float metros) { return metros * PIXELS_POR_METRO; }