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
        int teclas;
        teclas = ~TECLAS_DAT & 0x03ff;
        
        if (teclas & 1) return 0;
        if (teclas & 2) return 1;
        if (teclas & 4) return 2;     
        if (teclas & 8) return 3;     
        if (teclas & 32) return 4;    
        if (teclas & 16) return 5;   
        if (teclas & 64) return 6;    
        if (teclas & 128) return 7;   
        if (teclas & 256) return 8;   
        if (teclas & 512) return 9;   

    return -1;  // si no se ha pulsado nada
}


void ConfigurarTeclado(int Conf_Tec)
{
	// Configuración del teclado. Modificar su registro de control en base a los bits
        // activados en el parametro Conf_Tec
	
}

void ConfigurarTemporizador(int Latch, int Conf_Tempo)
{
	// Configuración del temporizador. El latch es el valor del registro de datos del temporizador
        // Activar los bits del registro de control necesarios en base a los bits activados en el parámetro Conf_Tempo

	
}

void HabilitarIntTeclado()
{
	// Habilitar las interrupciones del teclado
	// Para realizar esa operación, primero deshabilitar todas las interrupciones de forma general, realizar la operación, 
	// y después volver a habilitar las interrupciones de forma general 
	DeshabilitarInterrrupciones(); // IME=0;
        // ESCRIBID AQUÍ VUESTRO CÓDIGO
	
	HabilitarInterrupciones(); // IME=1;
}

void InhibirIntTeclado()
{

	// Deshabilitar las interrupciones del teclado
	// Para realizar esa operación, primero deshabilitar todas las interrupciones de forma general, realizar la operación, 
	// y después volver a habilitar las interrupciones de forma general 

	DeshabilitarInterrrupciones(); // IME=0;
	// ESCRIBID AQUÍ VUESTRO CÓDIGO

	HabilitarInterrupciones(); // IME=1;
}  

void HabilitarIntTempo()
{


	// Habilitar las interrupciones del temporizador (timer0)
	// Para realizar esa operación, primero deshabilitar todas las interrupciones de forma general, realizar la operación, 
	// y después volver a habilitar las interrupciones de forma general 
	DeshabilitarInterrrupciones(); // IME=0;
	// ESCRIBID AQUÍ VUESTRO CÓDIGO
	
	HabilitarInterrupciones(); // IME=1;
}

void InhibirIntTempo()
{

	// Deshabilitar las interrupciones del temporizador (timer0)
	// Para realizar esa operación, primero deshabilitar todas las interrupciones de forma general, realizar la operación, 
	// y después volver a habilitar las interrupciones de forma general 
	DeshabilitarInterrrupciones(); // IME=0;
	// ESCRIBID AQUÍ VUESTRO CÓDIGO
	
	HabilitarInterrupciones(); // IME=1;

}

void PonerEnMarchaTempo()
{
	// ESCRIBID AQUÍ VUESTRO CÓDIGO
	
}

void PararTempo()
{
	// ESCRIBID AQUÍ VUESTRO CÓDIGO
	
}

/***********************2025-2026*******************************/

