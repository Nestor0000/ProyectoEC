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

// --- RELOJES Y TIEMPO ---
volatile int tiempo = 0;
volatile int ticks_fase = 0;
int game_tick = 0;

// --- PARÁMETROS DE DIFICULTAD (Velocidad y cantidad de los enemigos, fases, etc) ---
int fase_actual = 1;                //Empieza por defecto en la fase 1
int velocidad_fase = 1;             //Velocidad con la que inician los enemigos
int espera_spawn_fase = 25;         //Cada cuanto tardan en aparecer los enemigos (en ticks)
int spawnasteroides_timer = 0;      //Para controlar el tiempo entre spawns de enemigos

// --- REGISTROS DE MOVIMIENTO Y DISPAROS ---
int contDisparos;
int teclaPulsada;
volatile int cooldown_disparo = 0;

// --- MISCELANEO ---
int tempo_activado = 0;
int asteroides_inicializados = 0;
volatile int fondo_actual = 1;

// --- ENTIDADES IN-GAME ---
Nave jugador;
Disparo disparosNave[MAX_DISPAROS];
Asteroide asteroides[MAX_ASTEROIDES];

void InitAsteroides() {
    int i;
    for(i = 0; i < MAX_ASTEROIDES; i++) {
        asteroides[i].activo = 0;
        asteroides[i].indice = i+16;
    }
}
void SpawnAsteroide() {
    int i;
    for(i = 0; i < MAX_ASTEROIDES; i++) {

        if(asteroides[i].activo == 0) {

            int lado = rand() % 4;
            asteroides[i].lado = lado;
            int speed = velocidad_fase;

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
                BorrarAsteroide(asteroides[i]);

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
            BorrarAsteroide(asteroides[i]);

            // 2. actualizar posición según lado
            if(asteroides[i].lado == 0) {
                asteroides[i].y += velocidad_fase;
            }
            else if(asteroides[i].lado == 1) {
                asteroides[i].y -= velocidad_fase;
            }
            else if(asteroides[i].lado == 2) {
                asteroides[i].x += velocidad_fase;
            }
            else if(asteroides[i].lado == 3) {
                asteroides[i].x -= velocidad_fase;
            }

            // 3. volver a dibujar en nueva posición
            MostrarAsteroide(asteroides[i]);
            //GuardarSpriteAsteroideMemoria();
        }
    }
    GuardarSpriteAsteroideMemoria();
}

void juego() {
    
    // Definiciones de variables
    int i=9;
    int tecla=0;
    int j = 0;//se usara j como indice para recorrer bucles ya que i tiene otro uso
    ESTADO=GAME;
    int cooldown_rotacion = 0;

    for(j=0;j<MAX_DISPAROS; j++){
        disparosNave[j].indice = j+5;
    }

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
    ConfigurarTemporizador(61440,0x0041);
    EstablecerVectorInt();
    HabilitarIntTeclado();
    HabilitarIntTempo();
    irqEnable(IRQ_KEYS|IRQ_TIMER0);
    HabilitarInterrupciones();


    jugador.x = 110;
    jugador.y = 96;
    jugador.orientacion_actual = SPR_NAVE_ARRIBA;





	while(1)
    {
        swiWaitForVBlank();


        /*******************************EN LA 1.ACTIVIDAD *****************************************/
        //Si el estado es ESPERA: codificar aquí la encuesta del teclado, sacar por pantalla la tecla que se ha pulsado, y si se pulsa la tecla START cambiar de estado */


        // --- INDICADORES DE DIFICULTAD PARA ASEGURAR FUNCIONAMIENTO (TEMPORAL)
        iprintf("\x1b[1;1H--- STATUS ---");
        iprintf("\x1b[2;1HFase actual: %d      ", fase_actual);
        iprintf("\x1b[3;1HVelocidad:   %d      ", velocidad_fase);
        iprintf("\x1b[4;1HSpawn Rate:  %d      ", espera_spawn_fase);

        if(ESTADO==GAME){
            if(tempo_activado == 0) {
                PonerEnMarchaTempo();
                tempo_activado = 1;
            }
            if(asteroides_inicializados == 0){
                InitAsteroides();
                asteroides_inicializados = 1;
            }
            visualizarFondo1();
            fondo_actual=1;
            MostrarNave(jugador);
            GuardarSpritesMemoria(jugador.orientacion_actual);
            //game_tick++;
            if(cooldown_rotacion > 0){
                cooldown_rotacion--;
            }
            spawnasteroides_timer++;
            if(spawnasteroides_timer > espera_spawn_fase){
                SpawnAsteroide();
                spawnasteroides_timer = 0;
            }
            ActualizarAsteroides();
            AparicionAsteroides();
            GuardarSpriteAsteroideMemoria();

            teclaPulsada=TeclaPulsada();
            if(teclaPulsada==DERECHA && jugador.x < 225){
                BorrarNave(jugador);
                jugador.x = jugador.x + 2;
                MostrarNave(jugador);
            } else if(teclaPulsada==IZQUIERDA && jugador.x > 0){
                BorrarNave(jugador);
                jugador.x = jugador.x - 2;
                MostrarNave(jugador);
            } else if (teclaPulsada==ARRIBA && jugador.y > 0){
                BorrarNave(jugador);
                jugador.y = jugador.y - 2;
                MostrarNave(jugador);
            } else if (teclaPulsada== ABAJO && jugador.y < 165){
                BorrarNave(jugador);
                jugador.y = jugador.y + 2;
                MostrarNave(jugador);
            }
               if(cooldown_rotacion == 0 && ((teclaPulsada==R && jugador.orientacion_actual == SPR_NAVE_ARRIBA) || (teclaPulsada==L && jugador.orientacion_actual== SPR_NAVE_ABAJO))){
                   BorrarNave(jugador);
                   jugador.orientacion_actual = SPR_NAVE_DERECHA;
                   MostrarNave(jugador);
                   GuardarSpritesMemoria(jugador.orientacion_actual);
                    cooldown_rotacion = 25;
            }
              else if(cooldown_rotacion == 0 && ((teclaPulsada==R && jugador.orientacion_actual == SPR_NAVE_DERECHA) || (teclaPulsada==L && jugador.orientacion_actual== SPR_NAVE_IZQUIERDA))){
                  BorrarNave(jugador);
                  jugador.orientacion_actual = SPR_NAVE_ABAJO;
                  MostrarNave(jugador);
                  GuardarSpritesMemoria(jugador.orientacion_actual);
                     cooldown_rotacion = 25;
              }
              else if(cooldown_rotacion == 0 && ((teclaPulsada==L && jugador.orientacion_actual == SPR_NAVE_DERECHA) || (teclaPulsada==R && jugador.orientacion_actual== SPR_NAVE_IZQUIERDA))) {
                  BorrarNave(jugador);
                  jugador.orientacion_actual = SPR_NAVE_ARRIBA;
                  MostrarNave(jugador);
                  GuardarSpritesMemoria(jugador.orientacion_actual);
                     cooldown_rotacion = 25;
              }
              else if(cooldown_rotacion == 0 && ((teclaPulsada==L && jugador.orientacion_actual == SPR_NAVE_ARRIBA) || (teclaPulsada==R && jugador.orientacion_actual== SPR_NAVE_ABAJO))) {
                BorrarNave(jugador);
                jugador.orientacion_actual = SPR_NAVE_IZQUIERDA;
                MostrarNave(jugador);
                GuardarSpritesMemoria(jugador.orientacion_actual);
                cooldown_rotacion = 25;
              }


              for (j = 0; j < MAX_DISPAROS; j++)
              {
                Disparo *proyectil = &disparosNave[j];
                if(proyectil->activo == ACTIVO && proyectil->orientacion_actual == SPR_NAVE_ARRIBA){
                    if(proyectil->y>0){
                        BorrarDisparo(proyectil);
                        proyectil->y = proyectil->y - 3;
                        MostrarDisparo(proyectil);
                    }
                    else{
                        proyectil->activo = INACTIVO;
                        BorrarDisparo(proyectil);
                        contDisparos--;
                    }
                }
                else if(proyectil->activo == ACTIVO && proyectil->orientacion_actual == SPR_NAVE_DERECHA){
                    if(proyectil->x<225){
                        BorrarDisparo(proyectil);
                        proyectil->x = proyectil->x + 3;
                        MostrarDisparo(proyectil);
                    }else{
                        proyectil->activo =INACTIVO;
                        BorrarDisparo(proyectil);
                        contDisparos--;
                    }
                }
                else if(proyectil->activo == ACTIVO && proyectil->orientacion_actual == SPR_NAVE_ABAJO){
                    if(proyectil->y < 165){
                        BorrarDisparo(proyectil);
                        proyectil->y = proyectil->y+3;
                        MostrarDisparo(proyectil);
                    }else{
                        proyectil->activo = INACTIVO;
                        BorrarDisparo(proyectil);
                        contDisparos--;
                    }
                }
                else if(proyectil->activo == ACTIVO && proyectil->orientacion_actual == SPR_NAVE_IZQUIERDA){
                    if(proyectil->x>0){
                        BorrarDisparo(proyectil);
                        proyectil->x = proyectil->x-3;
                        MostrarDisparo(proyectil);
                    }else{
                        proyectil->activo =INACTIVO;
                        BorrarDisparo(proyectil);
                        contDisparos--;
                    }
                }
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

