// Curso 2025-2026
#include "entidades.h"

/*---------------------------------------------------------------------------------
juego.h
---------------------------------------------------------------------------------*/
int colisiona(int x1,int y1, int w1, int h1, int x2, int y2, int w2, int h2);
int colisionaDisparoAsteroide(Disparo *d, Asteroide *a);
int colisionaNaveAsteroide(Nave *n, Asteroide *a);
int colisionaDisparoOrbe(Disparo *d, Orbe *o);
void InitAsteroides();
void SpawnAsteroide();
void AparicionAsteroides();
void Eliminar_Asteroides();
void Spawn_Orbe();
void juego();

/***********************2025-2026*******************************/

