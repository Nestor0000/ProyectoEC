#ifndef ENTIDADES_H
#define ENTIDADES_H
#include "definiciones.h"
typedef struct {
    int x,y;
    int orientacion_actual;
} Nave;

typedef struct {
    int x,y;
    int orientacion_actual;
	int activo;
    int indice;
} Disparo;
extern Nave jugador;
extern Disparo disparosNave[MAX_DISPAROS];
extern int contDisparos;
#endif