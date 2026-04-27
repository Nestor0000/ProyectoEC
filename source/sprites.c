// Curso 2025-2026

#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sprites.h"
#include "definiciones.h"

u16* gfxrombo;
u16* gfxromboGrande;
u16* gfx_nave;   // <-- CAMBIO IMPORTANTE (ANTES gfxnave)

/* Reservar memoria */
void memoriaReserva()
{
    gfxrombo = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
    gfxromboGrande = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
    gfx_nave = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
}

/* Paleta */
void EstablecerPaletaPrincipal() {
    SPRITE_PALETTE[1] = RGB15(31,0,0);
    SPRITE_PALETTE[2] = RGB15(0,31,0);
    SPRITE_PALETTE[3] = RGB15(0,0,31);
}

/* Sprites */
u8 NAVE_ARRIBA[1024] = { /* igual que el tuyo */ };
u8 rombo[256] = { /* igual */ };
u8 romboGrande[1024] = { /* igual */ };

/* Cargar sprites */
void GuardarSpritesMemoria()
{
    int i;

    for(i = 0; i < 16 * 16 / 2; i++)
        gfxrombo[i] = rombo[i*2] | (rombo[i*2+1] << 8);

    for(i = 0; i < 32 * 32 / 2; i++)
        gfxromboGrande[i] = romboGrande[i*2] | (romboGrande[i*2+1] << 8);

    for(i = 0; i < 32 * 32 / 2; i++)
        gfx_nave[i] = NAVE_ARRIBA[i*2] | (NAVE_ARRIBA[i*2+1] << 8);
}

/* Mostrar nave */
void MostrarNave(int indice, int x, int y)
{
    oamSet(&oamMain,
        indice,
        x, y,
        0,
        0,
        SpriteSize_32x32,
        SpriteColorFormat_256Color,
        gfx_nave,
        -1,
        false,
        false,
        false,
        false,
        false);

    oamUpdate(&oamMain);
}

void BorrarNave(int indice, int x, int y) {}