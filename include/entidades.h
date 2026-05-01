#ifndef ENTIDADES_H
#define ENTIDADES_H
typedef struct {
    int x,y;
    int orientacion_actual;
} Nave;

typedef struct {
    int x,y;
    int orientacion_actual;
} Disparo;
extern Nave jugador;
#endif