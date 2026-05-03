#ifndef ENTIDADES_H
#define ENTIDADES_H
#include "definiciones.h"

typedef struct {
    int x,y;
    int orientacion_actual;
} Nave;

typedef struct {
    int x,y;
    int vx, vy;
    int activo;
    int velocidad;
    int lado;
} Asteroide;



typedef struct {
    int x,y;
    int orientacion_actual;
} Disparo;

extern Nave jugador;
extern Asteroide asteroides[MAX_ASTEROIDES];

#endif