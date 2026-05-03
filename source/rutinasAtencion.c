// Curso 2025-2026

/*-------------------------------------
rutinasAtencion.c
-------------------------------------*/

#include <nds.h>
#include <stdio.h>
#include "definiciones.h"
#include "perifericos.h"
#include "fondos.h"
#include "sprites.h"
#include "rutinasAtencion.h"

int ESTADO; // Para controlar el estado del autómata en que esté
int seg3;   // Para ver si pasan tres segundos
/*
void RutAtencionTeclado ()
{
if (ESTADO == CERRADA)
{	
	if (TeclaPulsada()==A)
	{
		ESTADO=ABIERTA;
		//visualizarPuertaAbierta();
		seg3=0;
		MostrarRombo(1, 5, 5);
		MostrarRomboGrande(2, 100, 100);
	}
}
}

 
*/
/*void RutAtencionTempo()
{
	static int tick=0;
	static int seg=0;
	

if (ESTADO!=ESPERA)
{
	tick++; 
	if (tick==5)
	{
		seg++;
		iprintf("\x1b[13;5HSegundos que han pasado=%d", seg);
		tick=0;
		/*if (ESTADO == ABIERTA)
		{
			seg3++;
			if (seg3==3)
			{
				//visualizarPuerta();
				seg3=0;
				ESTADO=CERRADA;
				BorrarRombo(1, 5, 5);
				BorrarRomboGrande(2, 100, 100);
			}
		}
				
	}
}
	
}*/

void RutAtencionTeclado() {
	iprintf("\x1b[1;1HISRejecutada");
	int tecla = TeclaPulsada();
	if (ESTADO == GAME) {
		if(tecla == B){
			iprintf("\x1b[1;1HISRteclaApulsada");
			InhibirIntTeclado();
			Disparo proyectil;
			if (jugador.orientacion_actual == 0) {
				proyectil.x = jugador.x;
				proyectil.y = jugador.y - 10;
			} else if (jugador.orientacion_actual == 1) {
				proyectil.x = jugador.x + 10;
				proyectil.y = jugador.y;
			} else if (jugador.orientacion_actual == 2) {
				proyectil.x = jugador.x;
				proyectil.y = jugador.y + 10;
			} else {
				proyectil.x = jugador.x - 10;
				proyectil.y = jugador.y;
			}
			MostrarDisparo(proyectil, SPR_DISPARO_NAVE);
			HabilitarIntTeclado();
			//GuardarSpriteDisparoMemoria();
		}
	}
}
void EstablecerVectorInt()
{
// A COMPLETAR POR USTEDES
	//irqSet(IRQ_KEYS,RutAtencionTeclado);

	irqSet(IRQ_KEYS, RutAtencionTeclado);
	//irqSet(IRQ_TIMER0,RutAtencionTempo);
}



/***********************2025-2026*******************************/

