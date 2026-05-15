// Curso 2025-2026
#include "entidades.h"
extern u16* gfx;
extern u16* gfx2;
extern u16* gfx3;
extern u16* gfxSub;


extern void memoriaReserva();

extern void EstablecerPaletaPrincipal();
/* Para cada uno de los 256 valores que puede tomar un Píxel le da un color en la pantalla principal. El valor 0 es transparente y los valores sin definir, negros */


extern void EstablecerPaletaSecundaria();
/* Para cada uno de los 256 valores que puede tomar un Píxel le da un color en la pantalla secundaria. El valor 0 es transparente y los valores sin definir negros */


extern void GuardarSpritesMemoria(int orientacion_nave);
/* Carga en memoria los Sprites dibujados */
extern void GuardarSpriteDisparoMemoria ();
extern void GuardarSpriteAsteroideMemoria ();
extern void GuardarSpriteOrbeMemoria();

/* Esta función borra de la pantalla el rombo grande con el índice que se le indique como parámetro */
extern void MostrarOrbe(Orbe orbe, int indice);
extern void BorrarOrbe(Orbe orbe, int indice);
extern void MostrarDisparo(Disparo *proyectil);
extern void BorrarDisparo(Disparo *proyectil);
extern void BorrarAsteroide(Asteroide enemigo);
extern void MostrarAsteroide(Asteroide enemigo);
extern void MostrarNave(Nave jugador);
extern void BorrarNave(Nave jugador);

extern void InitAsteroides();
extern void SpawnAsteroide();
extern void ActualizarAsteroides();
extern void AparicionAsteroides();
/***********************2025-2026*******************************/

