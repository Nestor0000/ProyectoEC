#ifndef ENTIDADES_H
#define ENTIDADES_H
#include "definiciones.h"
typedef struct {
	int offsetX,offsetY;
	int w,h;
}Hitbox;
typedef struct {
    int x,y;
    int orientacion_actual;
	Hitbox hitbox;
} Nave;

typedef struct {
    int x,y;
    int vx, vy;
    int activo;
    int velocidad;
    int lado;
    int indice;
	Hitbox hitbox;
} Asteroide;

typedef struct {
    int x,y;
    int recarga_balas;
} Orbe;


typedef struct {
    int x,y;
    int orientacion_actual;
	int activo;
    int indice;
	Hitbox hitbox;
} Disparo;

extern Nave jugador;
extern Asteroide asteroides[MAX_ASTEROIDES];

extern Disparo disparosNave[MAX_DISPAROS];
extern int contDisparos;
#endif