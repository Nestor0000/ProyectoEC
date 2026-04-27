// Curso 2025-2026

/*---------------------------------------------------------------------------------
Este código se ha desarrollado basándose en el código de dovoto "Simple sprite demo" 
y en otro ejemplo de Jaeden Ameronen
---------------------------------------------------------------------------------*/


#include <nds.h> 		// Librería de la NDS
#include <stdio.h>		// Librería de entrada/salida estándar de C
#include <stdlib.h>		// Librería estándar de C para reserva de memoria y conversiones numéricas
#include <unistd.h>		// Librería para asegurar la compatibilidad entre sistemas operativos

// Librerías desarrolladas por nosotros para el proyecto
#include "sprites.h"
#include "juego.h"
#include "definiciones.h"
#include "perifericos.h"
#include "rutinasAtencion.h"
#include "fondos.h"
#include "sprites.h"

int tiempo;

void juego()
{
    // ------------------ INICIALIZACIÓN GRÁFICA ------------------
    videoSetMode(MODE_0_2D);
    oamInit(&oamMain, SpriteMapping_1D_32, false);

    memoriaReserva();
    EstablecerPaletaPrincipal();
    GuardarSpritesMemoria();

    // ------------------ VARIABLES ------------------
    int i = 9;

    // Debug por pantalla
    iprintf("\x1b[22;5HPrueba de escritura");
    iprintf("\x1b[23;5HValor=%d", i);

    // ------------------ INTERRUPCIONES ------------------
    irqInit();
    irqEnable(IRQ_VBLANK);

    ConfigurarTeclado(0xC001);
    ConfigurarTemporizador(0xC000,0X00C0);

    HabilitarIntTeclado();
    HabilitarIntTempo();

    irqEnable(IRQ_KEYS | IRQ_TIMER0);

    PonerEnMarchaTempo();

    // ------------------ FONDO ------------------
    visualizarFondo1();

    // ------------------ NAVE ------------------
    Nave jugador;
    jugador.x = 100;
    jugador.y = 80;

    // ------------------ BUCLE PRINCIPAL ------------------
    while(1)
    {
        swiWaitForVBlank();

        // Dibujar nave fija
        MostrarNave(SPR_NAVE_ARRIBA, jugador.x, jugador.y);
    }
}

/***********************2025-2026*******************************/


