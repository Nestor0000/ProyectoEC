// Curso 2025-2026
extern u16* gfx;
extern u16* gfx2;
extern u16* gfx3;
extern u16* gfxSub;
typedef struct {
    int x,y;
} Nave;

extern void memoriaReserva();

extern void EstablecerPaletaPrincipal();
/* Para cada uno de los 256 valores que puede tomar un Píxel le da un color en la pantalla principal. El valor 0 es transparente y los valores sin definir, negros */


extern void EstablecerPaletaSecundaria();
/* Para cada uno de los 256 valores que puede tomar un Píxel le da un color en la pantalla secundaria. El valor 0 es transparente y los valores sin definir negros */


extern void GuardarSpritesMemoria();
/* Carga en memoria los Sprites dibujados */


extern void MostrarRombo(int indice, int x, int y);
/* Esta función dibuja un rombo en la posición de pantalla x,y. A cada rombo que queramos sacar en pantalla se le debe asignar un índice distinto que puede tomara valores entre 0 y 126 */


extern void BorrarRombo(int indice, int x, int y);
/* Esta función borra de la pantalla el rombo con el índice que se le indique como parámetro */


extern void MostrarRomboGrande(int indice, int x, int y);
/* Esta función dibuja un rombo grande en la posición de pantalla x,y. A cada rombo que queramos sacar en pantalla se le debe asignar un índice distinto que puede tomara valores entre 0 y 126 */


extern void BorrarRomboGrande(int indice, int x, int y);
/* Esta función borra de la pantalla el rombo grande con el índice que se le indique como parámetro */

extern void MostrarNave(int indice, int x, int y);
extern void BorrarNave(int indice, int x, int y);
/***********************2025-2026*******************************/

