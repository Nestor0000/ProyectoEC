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
extern volatile int tiempo; // extern popr que es de juego.c y volatile por que puede cambiar por interrupciones
extern volatile int fondo_actual=1;

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
/*void RutAtencionTempo() esta es la funcion antigua
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
		if(tecla == A){
			iprintf("\x1b[1;1HISRteclaApulsada");
			InhibirIntTeclado();
			Disparo proyectil;
			proyectil.x = jugador.x;
			proyectil.y = jugador.y + 2;
			MostrarDisparo(proyectil, SPR_DISPARO_NAVE);
			HabilitarIntTeclado();
			//GuardarSpriteDisparoMemoria();
		}
	}
}
void RutAtencionTempo()
{ 
	tiempo++;
    if(tiempo>=2400)//20 segs segun chati
	{
		tiempo = 0;
		if(fondo_actual==1)
		{
			visualizarFondo2();
			fondo_actual = 2;
		}
		else if(fondo_actual==2)
		{
			visualizarFondo3();
			fondo_actual = 3;

		}
		else if(fondo_actual==3)
		{
			visualizarFondo4();
			fondo_actual = 4;
		}



	}
}
	





void EstablecerVectorInt()
{
// A COMPLETAR POR USTEDES
	//irqSet(IRQ_KEYS,RutAtencionTeclado);

	irqSet(IRQ_KEYS, RutAtencionTeclado);
	irqSet(IRQ_TIMER0,RutAtencionTempo);
}




/***********************2025-2026*******************************/

