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
#include "juego.h"
#include "definiciones.h"
#include "perifericos.h"
#include "rutinasAtencion.h"
#include "fondos.h"

int tiempo;

void juego()
{
	// Definiciones de variables
	int i=9;
	int tecla=0;

	//ESTADO=ESPERA;
	
	// Escribe en la fila 22 columna 5 de la pantalla	
	iprintf("\x1b[22;5HPrueba de escritura");

/* Si se quiere visualizar el valor de una variable escribir %d dentro de las comillas y el nombre de la variable fuera de las comillas */
	iprintf("\x1b[23;5HPrueba de escritura con variable. Valor=%d", i);

	//******************************* EN LA 2.ACTIVIDAD ********************************//
        // LLAMADAS A REALIZAR (ORDEN RECOMENDADO):
	// Configurar el teclado.
	// Configurar el temporizador.
	// Establecer las rutinas de atención a interrupciones.
	// Habilitar las interrupciones del teclado.
	// Habilitar las interrupciones del temporizador.
	// Habilitar interrupciones.
	//******************************************************************************//
	irqInit();
	irqEnable(IRQ_VBLANK);
	ConfigurarTeclado(0xC001);
	ConfigurarTemporizador(0xC000,0X00C0);
	//EstablecerVectorInt();
	HabilitarIntTeclado();
	HabilitarIntTempo();
	irqEnable(IRQ_KEYS|IRQ_TIMER0);
	PonerEnMarchaTempo();
	

	visualizarFondo1();
	Nave jugador;
	jugador.x = 128;
	jugador.y = 96;
	MostrarNave(NAVE_ARRIBA, jugador.x, jugador.y);
	while(1)
	{	
		
      /*******************************EN LA 1.ACTIVIDAD *****************************************/
		 //Si el estado es ESPERA: codificar aquí la encuesta del teclado, sacar por pantalla la tecla que se ha pulsado, y si se pulsa la tecla START cambiar de estado */

		/*if(ESTADO==GAME){
			if(TeclaDetectada()==DERECHA){
				tecla=TeclaPulsada();
				if(tecla==START){
					ESTADO=CERRADA;
					visualizarFondo1();
	
				}
				iprintf("Tecla pulsada: %d", tecla);	
				while(TeclaDetectada()==1);		
			}
					
		}*/

	}

	// Inhibir las interrupciones al final
}

/***********************2025-2026*******************************/


