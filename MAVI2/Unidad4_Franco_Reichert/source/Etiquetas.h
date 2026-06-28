#ifndef ETIQUETAS_H
#define ETIQUETAS_H
//identidad logica de cada cuerpo,umion entre el mundo fisico y la logica del juego
enum class TipoObjeto
{
    Proyectil,
    Objetivo,     //blanco fisico
    ZonaSensor,   //region sensor 
    Suelo,
    Pared
};
struct DatosCuerpo
{
    TipoObjeto tipo;
    DatosCuerpo(TipoObjeto t = TipoObjeto::Pared) : tipo(t) {}
};
#endif 