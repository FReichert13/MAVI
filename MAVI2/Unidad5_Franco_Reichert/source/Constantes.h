#ifndef CONSTANTES_H
#define CONSTANTES_H
const float PIXELS_POR_METRO = 30.0f;
inline float Pix2M(float px) { return px / PIXELS_POR_METRO; }
inline float M2Pix(float m) { return m * PIXELS_POR_METRO; }
#endif