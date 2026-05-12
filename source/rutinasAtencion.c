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
#include "entidades.h"

int ESTADO; // Para controlar el estado del autómata en que esté
int seg3;   // Para ver si pasan tres segundos

// --- Variables externas de juego.c ---
extern volatile int tiempo; // extern popr que es de juego.c y volatile por que puede cambiar por interrupciones
extern volatile int fondo_actual;
extern volatile int cooldown_disparo;
extern int contDisparos;

// --- Variables de dificultad
extern int fase_actual;
extern int velocidad_fase;
extern int espera_spawn_fase;

void RutAtencionTeclado() {
	int tecla = TeclaPulsada();
	if(tecla==B && contDisparos <10 && cooldown_disparo == 0){
		InhibirIntTeclado();
		int i = 0;
		int k = 0; //Esto es para salir del bucle cuando se encuentre uno inactivo para poder usarlo
		Disparo *proyectil = NULL;
		while(i<10 && k==0){
			//para corregir
			if(disparosNave!=NULL &&disparosNave[i].activo == INACTIVO){
				proyectil= &disparosNave[i];
				k=1;
			}
			i++;
		}
		if(jugador.orientacion_actual == SPR_NAVE_ARRIBA){
			proyectil->x = jugador.x;
			proyectil->y=jugador.y-3;
			proyectil->orientacion_actual = SPR_NAVE_ARRIBA;
			contDisparos++;
		}
		else if(jugador.orientacion_actual ==SPR_NAVE_DERECHA){
			proyectil->x = jugador.x + 3;
			proyectil->y=jugador.y;
			proyectil->orientacion_actual = SPR_NAVE_DERECHA;
			contDisparos++;
		}
		else if(jugador.orientacion_actual ==SPR_NAVE_IZQUIERDA){
			proyectil->x = jugador.x-3;
			proyectil->y=jugador.y;
			proyectil->orientacion_actual = SPR_NAVE_IZQUIERDA;
			contDisparos++;
		}
		else if(jugador.orientacion_actual ==SPR_NAVE_ABAJO){
			proyectil->x = jugador.x;
			proyectil->y=jugador.y + 3;
			proyectil->orientacion_actual = SPR_NAVE_ABAJO;
			contDisparos++;
		}
		proyectil->activo = ACTIVO;
		proyectil->hitbox.offsetX=0;
		proyectil->hitbox.offsetY=0;
		proyectil->hitbox.w =4;
		proyectil->hitbox.h =4;
		
		MostrarDisparo(proyectil);
		cooldown_disparo=60;
		HabilitarIntTeclado();

	}

}
void RutAtencionTempo()
{
	tiempo++;
    if(tiempo>=2400)//20 segs segun chati
	{
		tiempo = 0;

		if (fase_actual < 4) {
			fase_actual++;
			//Tope de velocidad para que no se vuelva injugable
			if (velocidad_fase<3){
			velocidad_fase++;
			}
			// Cada vez hay menos tiempo entre spawns de asteroides  
			if (espera_spawn_fase > 10)
			{
				espera_spawn_fase -= 5;
			}
		}


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
	if(cooldown_disparo>0){
		cooldown_disparo--;
	}
}
void EstablecerVectorInt()
{
	irqSet(IRQ_KEYS, RutAtencionTeclado);
	irqSet(IRQ_TIMER0,RutAtencionTempo);
}



/***********************2025-2026*******************************/
