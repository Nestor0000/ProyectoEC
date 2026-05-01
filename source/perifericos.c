// Curso 2025-2026

/*-------------------------------------
perifericos.c
-------------------------------------*/

#include <nds.h>
#include <stdio.h>
#include "definiciones.h"


int tecla; // Variable para guardar la tecla pulsada; valorar si es necesaria



int TeclaDetectada() 
{
	// Devuelve TRUE si detecta que se ha pulsado alguna tecla.
	if ((~TECLAS_DAT & 0x03ff)!=0) return 1;
	else return 0;

}

int TeclaPulsada() 
{

	// Devuelve el código de la tecla pulsada utilizando para ello las constantes definidas en 
	// definiciones.h:  A=0;B=1;SELECT=2;START=3;DERECHA=4;IZQUIERDA=5;
	// ARRIBA=6;ABAJO=7;R=8;L=9;
	// Poned código que sea comprensible al leer.

	int teclas = ~TECLAS_DAT & 0x03ff;
	if(teclas & (1<<0)) return 0; 
	if(teclas & (1<<1)) return 1;
	if(teclas & (1 <<2)) return 2;
	if(teclas & (1<<3)) return 3;
	if(teclas & (1<<4)) return 4;
	if(teclas & (1<<5)) return 5;
	if(teclas & (1<<6)) return 6;
	if(teclas & (1<<7)) return  7;
	if(teclas & (1<<8)) return 8;
	if(teclas & (1<<9)) return 9;
	return -1;

}



void ConfigurarTeclado(int Conf_Tec)
{
	// Configuración del teclado. Modificar su registro de control en base a los bits
        // activados en el parametro Conf_Tec
	//TECLAS_CNT=TECLAS_CNT|Conf_Tec;
	TECLAS_CNT=Conf_Tec;
}

void ConfigurarTemporizador(int Latch, int Conf_Tempo)
{
	// Configuración del temporizador. El latch es el valor del registro de datos del temporizador
        // Activar los bits del registro de control necesarios en base a los bits activados en el parámetro Conf_Tempo

	TIMER0_DAT=Latch;
	TIMER0_CNT=Conf_Tempo;
}

void HabilitarIntTeclado()
{
	// Habilitar las interrupciones del teclado
	// Para realizar esa operación, primero deshabilitar todas las interrupciones de forma general, realizar la operación, 
	// y después volver a habilitar las interrupciones de forma general 
	DeshabilitarInterrrupciones(); // IME=0;
        // ESCRIBID AQUÍ VUESTRO CÓDIGO
	IE=IE|0x0800;
	HabilitarInterrupciones(); // IME=1;
}

void InhibirIntTeclado()
{

	// Deshabilitar las interrupciones del teclado
	// Para realizar esa operación, primero deshabilitar todas las interrupciones de forma general, realizar la operación, 
	// y después volver a habilitar las interrupciones de forma general 

	DeshabilitarInterrrupciones(); // IME=0;
	// ESCRIBID AQUÍ VUESTRO CÓDIGO
	IE=IE&0XF7FF;
	HabilitarInterrupciones(); // IME=1;
}  

void HabilitarIntTempo()
{


	// Habilitar las interrupciones del temporizador (timer0)
	// Para realizar esa operación, primero deshabilitar todas las interrupciones de forma general, realizar la operación, 
	// y después volver a habilitar las interrupciones de forma general 
	DeshabilitarInterrrupciones(); // IME=0;
	// ESCRIBID AQUÍ VUESTRO CÓDIGO
	IE=IE|0X0004;
	HabilitarInterrupciones(); // IME=1;
}

void InhibirIntTempo()
{

	// Deshabilitar las interrupciones del temporizador (timer0)
	// Para realizar esa operación, primero deshabilitar todas las interrupciones de forma general, realizar la operación, 
	// y después volver a habilitar las interrupciones de forma general 
	DeshabilitarInterrrupciones(); // IME=0;
	// ESCRIBID AQUÍ VUESTRO CÓDIGO
	IE=IE&0XFFF7;
	HabilitarInterrupciones(); // IME=1;

}

void PonerEnMarchaTempo()
{
	// ESCRIBID AQUÍ VUESTRO CÓDIGO
	TIMER0_CNT = TIMER0_CNT|0X0040;
}

void PararTempo()
{
	// ESCRIBID AQUÍ VUESTRO CÓDIGO
	TIMER0_CNT=TIMER0_CNT&0XFFBF;
}

/***********************2025-2026*******************************/

