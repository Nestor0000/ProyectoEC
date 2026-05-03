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
#include "entidades.h"

int tiempo;
Nave jugador;
Asteroide asteroides[MAX_ASTEROIDES];
int spawnasteroides_timer = 0;
int game_tick = 0;

void InitJuego() {
    InitAsteroides();

    jugador.x = 110;
    jugador.y = 96;
    jugador.orientacion_actual = SPR_NAVE_ARRIBA;
}

void InitAsteroides() {
    int i;
    for(i = 0; i < MAX_ASTEROIDES; i++) {
        asteroides[i].activo = 0;
    }
}

void SpawnAsteroide() {
    int i;
    for(i = 0; i < MAX_ASTEROIDES; i++) {

        if(asteroides[i].activo == 0) {

            int lado = rand() % 4;
            asteroides[i].lado = lado;
            int speed = 1;

            if(lado == 0) { // esto es el lado de arriba
                asteroides[i].x = rand() % 256;
                asteroides[i].y = 0;
                asteroides[i].vx = 0;
                asteroides[i].vy = speed;
            }

            else if(lado == 1) { // esto es el lado de abajo
                asteroides[i].x = rand() % 256;
                asteroides[i].y = 192;
                asteroides[i].vx = 0;
                asteroides[i].vy = -speed;
            }

            else if(lado == 2) { // esto es el lado de izquierda
                asteroides[i].x = 0;
                asteroides[i].y = rand() % 192;
                asteroides[i].vx = speed;
                asteroides[i].vy = 0;
            }

            else { // esto es el lado derecha
                asteroides[i].x = 256;
                asteroides[i].y = rand() % 192;
                asteroides[i].vx = -speed;
                asteroides[i].vy = 0;
            }

            asteroides[i].activo = 1;
            break;
        }
    }
}


void ActualizarAsteroides() {

    int i;
    for(i = 0; i < MAX_ASTEROIDES; i++) {

        if(asteroides[i].activo == 1) {

            // si se sale de pantalla → desactivar
            if(asteroides[i].x < -32 || asteroides[i].x > 256 ||
               asteroides[i].y < -32 || asteroides[i].y > 192) {

                // opcional: borrarlo visualmente antes de apagarlo
                BorrarAsteroide(asteroides[i], i);

                asteroides[i].activo = 0;
            }
        }
    }
}

void AparicionAsteroides() {
    int i;

    for(i = 0; i < MAX_ASTEROIDES; i++) {

        if(asteroides[i].activo) {

            // 1. borrar posición anterior
            BorrarAsteroide(asteroides[i], i);

            // 2. actualizar posición según lado
            if(asteroides[i].lado == 0) {
                asteroides[i].y += 1;
            }
            else if(asteroides[i].lado == 1) {
                asteroides[i].y -= 1;
            }
            else if(asteroides[i].lado == 2) {
                asteroides[i].x += 1;
            }
            else if(asteroides[i].lado == 3) {
                asteroides[i].x -= 1;
            }

            // 3. volver a dibujar en nueva posición
            MostrarAsteroide(asteroides[i], i);
            GuardarSpriteAsteroideMemoria();
        }
    }
}

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
    //ConfigurarTeclado(0xC001);
	ConfigurarTeclado(0x43FF);
    ConfigurarTemporizador(0xC000,0X00C0); //revisar parametros 
    EstablecerVectorInt();
    HabilitarIntTeclado();
    HabilitarIntTempo();
    irqEnable(IRQ_KEYS|IRQ_TIMER0);
    HabilitarInterrupciones();
    PonerEnMarchaTempo();
    irqDisable(IRQ_KEYS);

    InitJuego();
    visualizarFondo1();
    MostrarNave(jugador);
    while(1)
    {
        swiWaitForVBlank();
         game_tick++;
        if(cooldown_rotacion > 0){
        cooldown_rotacion--;

         

    }
        /*******************************EN LA 1.ACTIVIDAD *****************************************/
        //Si el estado es ESPERA: codificar aquí la encuesta del teclado, sacar por pantalla la tecla que se ha pulsado, y si se pulsa la tecla START cambiar de estado */

        if(ESTADO==GAME){
            spawnasteroides_timer++;
            if(spawnasteroides_timer > 25){
             SpawnAsteroide();
            spawnasteroides_timer = 0;
            }
            ActualizarAsteroides();
            AparicionAsteroides();
            GuardarSpriteAsteroideMemoria();

            tecla = TeclaPulsada();

            MostrarNave(jugador);

            if(tecla==DERECHA && jugador.x < 225){
                BorrarNave(jugador);
                jugador.x = jugador.x + 2;
                MostrarNave(jugador);
            } else if(tecla==IZQUIERDA && jugador.x > 0){
                BorrarNave(jugador);
                jugador.x = jugador.x - 2;
                MostrarNave(jugador);
            } else if (tecla==ARRIBA && jugador.y > 0){
                BorrarNave(jugador);
                jugador.y = jugador.y - 2;
                MostrarNave(jugador);
            } else if (tecla== ABAJO && jugador.y < 165){
                BorrarNave(jugador);
                jugador.y = jugador.y + 2;
                MostrarNave(jugador);
            }
               if(cooldown_rotacion == 0 && ((tecla==R && jugador.orientacion_actual == SPR_NAVE_ARRIBA) || (tecla==L && jugador.orientacion_actual== SPR_NAVE_ABAJO))){
                   BorrarNave(jugador);
                   jugador.orientacion_actual = SPR_NAVE_DERECHA;
                   MostrarNave(jugador);
                   GuardarSpritesMemoria(jugador.orientacion_actual);
                    cooldown_rotacion = 25;
            }
              else if(cooldown_rotacion == 0 && ((tecla==R && jugador.orientacion_actual == SPR_NAVE_DERECHA) || (tecla==L && jugador.orientacion_actual== SPR_NAVE_IZQUIERDA))){
                  BorrarNave(jugador);
                  jugador.orientacion_actual = SPR_NAVE_ABAJO;
                  MostrarNave(jugador);
                  GuardarSpritesMemoria(jugador.orientacion_actual);
                     cooldown_rotacion = 25;
              }
              else if(cooldown_rotacion == 0 && ((tecla==L && jugador.orientacion_actual == SPR_NAVE_DERECHA) || (tecla==R && jugador.orientacion_actual== SPR_NAVE_IZQUIERDA))) {
                  BorrarNave(jugador);
                  jugador.orientacion_actual = SPR_NAVE_ARRIBA;
                  MostrarNave(jugador);
                  GuardarSpritesMemoria(jugador.orientacion_actual);
                     cooldown_rotacion = 25;
              }
              else if(cooldown_rotacion == 0 && ((tecla==L && jugador.orientacion_actual == SPR_NAVE_ARRIBA) || (tecla==R && jugador.orientacion_actual== SPR_NAVE_ABAJO))) {
                BorrarNave(jugador);
                jugador.orientacion_actual = SPR_NAVE_IZQUIERDA;
                MostrarNave(jugador);
                GuardarSpritesMemoria(jugador.orientacion_actual);
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


