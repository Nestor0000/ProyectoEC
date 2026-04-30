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

int tiempo;

void juego() {
    
    // Definiciones de variables
    int i=9;
    int tecla=0;

    ESTADO=GAME;
    int cooldown_rotacion = 0;



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
    jugador.x = 110;
    jugador.y = 96;
    jugador.orientacion_actual = SPR_NAVE_ARRIBA;
    MostrarNave(jugador);
    while(1)
    {
        swiWaitForVBlank();

        if(cooldown_rotacion > 0){
        cooldown_rotacion--;
    }
        /*******************************EN LA 1.ACTIVIDAD *****************************************/
        //Si el estado es ESPERA: codificar aquí la encuesta del teclado, sacar por pantalla la tecla que se ha pulsado, y si se pulsa la tecla START cambiar de estado */

        if(ESTADO==GAME){
            if(TeclaPulsada()==DERECHA && jugador.x < 225){
                BorrarNave(jugador);
                //tecla=TeclaPulsada();
                jugador.x = jugador.x + 2;
                MostrarNave(jugador);
            } else if(TeclaPulsada()==IZQUIERDA && jugador.x > 0){
                BorrarNave(jugador);
                jugador.x = jugador.x - 2;
                MostrarNave(jugador);
            } else if (TeclaPulsada()==ARRIBA && jugador.y > 0){
                BorrarNave(jugador);
                jugador.y = jugador.y - 2;
                MostrarNave(jugador);
            } else if (TeclaPulsada()== ABAJO && jugador.y < 165){
                BorrarNave(jugador);
                jugador.y = jugador.y + 2;
                MostrarNave(jugador);
            }
               if(cooldown_rotacion == 0 && ((TeclaPulsada()==R && jugador.orientacion_actual == SPR_NAVE_ARRIBA) || (TeclaPulsada()==L && jugador.orientacion_actual== SPR_NAVE_ABAJO))){
                   BorrarNave(jugador);
                   jugador.orientacion_actual = SPR_NAVE_DERECHA;
                   MostrarNave(jugador);
                   GuardarSpritesMemoria(jugador.orientacion_actual);
                    cooldown_rotacion = 25;
            }
              if(cooldown_rotacion == 0 && ((TeclaPulsada()==R && jugador.orientacion_actual == SPR_NAVE_DERECHA) || (TeclaPulsada()==L && jugador.orientacion_actual== SPR_NAVE_IZQUIERDA))){
                  BorrarNave(jugador);
                  jugador.orientacion_actual = SPR_NAVE_ABAJO;
                  MostrarNave(jugador);
                  GuardarSpritesMemoria(jugador.orientacion_actual);
                     cooldown_rotacion = 25;
              }
              if(cooldown_rotacion == 0 && ((TeclaPulsada()==L && jugador.orientacion_actual == SPR_NAVE_DERECHA) || (TeclaPulsada()==R && jugador.orientacion_actual== SPR_NAVE_IZQUIERDA))) {
                  BorrarNave(jugador);
                  jugador.orientacion_actual = SPR_NAVE_ARRIBA;
                  MostrarNave(jugador);
                  GuardarSpritesMemoria(jugador.orientacion_actual);
                     cooldown_rotacion = 25;
              }
              if(cooldown_rotacion == 0 && ((TeclaPulsada()==L && jugador.orientacion_actual == SPR_NAVE_ARRIBA) || (TeclaPulsada()==R && jugador.orientacion_actual== SPR_NAVE_ABAJO))) {
                BorrarNave(orientacion_actual, jugador);
                orientacion_actual = SPR_NAVE_IZQUIERDA;
                MostrarNave(orientacion_actual, jugador);
                GuardarSpritesMemoria(orientacion_actual);
                cooldown_rotacion = 25;
              }

            



               /* if(tecla==START){
                    ESTADO=CERRADA;
                    visualizarFondo1();

                }
                iprintf("Tecla pulsada: %d", tecla);
                while(TeclaDetectada()==1);

                */
            } 

        }

    }



    // Inhibir las interrupciones al final


/***********************2025-2026*******************************/


