#ifndef ETIQUETAS_H
#define ETIQUETAS_H
//identidad logica de cada cuerpo: une el mundo fisico con la logica
enum class TipoObjeto
{
    Proyectil,
    Objetivo,     //blanco fisico
    ZonaSensor,   //region sensor
    Suelo
};
struct DatosCuerpo
{
    TipoObjeto tipo;
    DatosCuerpo(TipoObjeto t = TipoObjeto::Suelo) : tipo(t) {}
};
#endif