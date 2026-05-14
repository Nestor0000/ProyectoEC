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

bool colisionDetectada;

#include <touch.h>
volatile int tiempo;
int contDisparos;
int teclaPulsada;
volatile int fondo_actual;
volatile int contador_orbe = 0;
volatile bool orbe_activo = false;
Orbe orbe;
bool orbe_recogido = false;


Nave jugador;
Disparo disparosNave[MAX_DISPAROS];
volatile int cooldown_disparo = 0;
Asteroide asteroides[MAX_ASTEROIDES];
int spawnasteroides_timer = 0;
int game_tick = 0;
int tempo_activado = 0;
int asteroides_inicializados = 0;
int colisiona(int x1,int y1, int w1, int h1, int x2, int y2, int w2, int h2){
    return (x1<x2+w2) && (x1+w1>x2)&&(y1<y2+h2)&&(y1+h1>y2);
}
int colisionaDisparoAsteroide(Disparo *d, Asteroide *a){
    return colisiona(d->x,d->y,d->hitbox.w,d->hitbox.h
        ,a->x,a->y,a->hitbox.w,a->hitbox.h);
}
int colisionaNaveAsteroide(Nave *n, Asteroide *a){
    return colisiona(n->x,n->y,n->hitbox.w,n->hitbox.h
        ,a->x,a->y,a->hitbox.w,a->hitbox.h);
}
int colisionaDisparoOrbe(Disparo *d, Orbe *o) {
	return colisiona(d->x, d->y, d->hitbox.w, d->hitbox.h, o->x, o->y, o->hitbox.w, o->hitbox.h);
}
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

            asteroides[i].hitbox.offsetX = 0;
            asteroides[i].hitbox.offsetY = 0;
            asteroides[i].hitbox.w = 16;
            asteroides[i].hitbox.h = 16;
            asteroides[i].activo = 1;
            break;
        }
    }
}
void Eliminar_Asteroides() {
// elimina los astereoides que se salen de la pantalla
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
            MostrarAsteroide(asteroides[i]);
            GuardarSpriteAsteroideMemoria();
        }
    }
}


void Spawn_Orbe () {
    orbe.recarga_balas = 20;
    if (!orbe_activo) {
            if (contador_orbe >= 600 + (rand() % 1200)) {
                 orbe.x = rand() % 220;
                 orbe.y = rand() % 180;
                 MostrarOrbe(orbe, SPR_ORBE);
                 GuardarSpriteOrbeMemoria();
                 orbe_activo = true;
				orbe.hitbox.offsetX = 0;
            	orbe.hitbox.offsetY = 0;
            	orbe.hitbox.w = 16;
            	orbe.hitbox.h = 16;
            }
    }
    if (orbe_recogido){
        BorrarOrbe(orbe, SPR_ORBE);
        orbe_activo = false;
        orbe_recogido = false;
        contador_orbe = 0;
    }

}

void juego() {
    
    // Definiciones de variables
    int i = 9;
    int tecla = 0;
    int j = 0; // se usara j como indice para recorrer bucles ya que i tiene otro uso
    ESTADO = MENU;
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
    inicializarFondos();
    jugador.x = 110;
    jugador.y = 96;
    jugador.orientacion_actual = SPR_NAVE_ARRIBA;
    fondo_actual = 0;
    srand(1);


    jugador.hitbox.offsetX = 0;
    jugador.hitbox.offsetY = 0;
    jugador.hitbox.w = 16;
    jugador.hitbox.h = 16;

    visualizarFondo1();
    fondo_actual=1;

	while(1)
    {
        Spawn_Orbe();
        swiWaitForVBlank();

        visualizarFondoMenu();
        touchRead(&PANT_DAT);
        if (PANT_DAT.px > 0 && PANT_DAT.py > 0)
        {
            ESTADO = GAME;
            iprintf("\x1b[2;1HPorky");
            visualizarFondo1();
        }

        /*******************************EN LA 1.ACTIVIDAD *****************************************/
        //Si el estado es ESPERA: codificar aquí la encuesta del teclado, sacar por pantalla la tecla que se ha pulsado, y si se pulsa la tecla START cambiar de estado */

        if(ESTADO==GAME){

            srand(jugador.x + jugador.y);

            if (tempo_activado == 0)
            {
                PonerEnMarchaTempo();
                tempo_activado = 1;
            }
            if(asteroides_inicializados == 0){
                InitAsteroides();
                asteroides_inicializados = 1;
            }
            if (fondo_actual == 0) {
            visualizarFondo1();
            fondo_actual=1;
            }

            MostrarNave(jugador);
            GuardarSpritesMemoria(jugador.orientacion_actual);
            //game_tick++;
            if(cooldown_rotacion > 0){
                cooldown_rotacion--;
            }
            spawnasteroides_timer++;
            if(spawnasteroides_timer > 25){
                SpawnAsteroide();
                spawnasteroides_timer = 0;
            }
            Eliminar_Asteroides();
            AparicionAsteroides();
            GuardarSpriteAsteroideMemoria();

            teclaPulsada=TeclaPulsada();
            if(teclaPulsada==DERECHA && jugador.x < 225){
                BorrarNave(jugador);
                jugador.x = jugador.x + 2;
                MostrarNave(jugador);

                colisionDetectada=false;
                for(j=0;j<MAX_ASTEROIDES && !colisionDetectada;j++){
                    if(asteroides[j].activo==1 && colisionaNaveAsteroide(&jugador,&asteroides[j])){
                        ESTADO=GAME_OVER;
                        fondo_actual = 5;

                        BorrarAsteroide(asteroides[j]);
                        asteroides[j].activo = 0;
                        colisionDetectada = true;
                    }
                }
                colisionDetectada=false;
            } else if(teclaPulsada==IZQUIERDA && jugador.x > 0){
                BorrarNave(jugador);
                jugador.x = jugador.x - 2;
                MostrarNave(jugador);

                colisionDetectada=false;
                for(j=0;j<MAX_ASTEROIDES && !colisionDetectada;j++){
                    if(asteroides[j].activo==1 && colisionaNaveAsteroide(&jugador,&asteroides[j])){
                        ESTADO=GAME_OVER;
                        fondo_actual = 5;

                        BorrarAsteroide(asteroides[j]);
                        asteroides[j].activo = 0;
                        colisionDetectada = true;
                    }
                }
                colisionDetectada=false;
            } else if (teclaPulsada==ARRIBA && jugador.y > 0){
                BorrarNave(jugador);
                jugador.y = jugador.y - 2;
                MostrarNave(jugador);
                colisionDetectada=false;
                for(j=0;j<MAX_ASTEROIDES && !colisionDetectada;j++){
                    if(asteroides[j].activo==1 && colisionaNaveAsteroide(&jugador,&asteroides[j])){
                        ESTADO=GAME_OVER;
                        fondo_actual = 5;

                        BorrarAsteroide(asteroides[j]);
                        asteroides[j].activo = 0;
                        colisionDetectada = true;
                    }
                }
                colisionDetectada=false;
            } else if (teclaPulsada== ABAJO && jugador.y < 165){
                BorrarNave(jugador);
                jugador.y = jugador.y + 2;
                MostrarNave(jugador);
                colisionDetectada=false;
                for(j=0;j<MAX_ASTEROIDES && !colisionDetectada;j++){
                    if(asteroides[j].activo==1 && colisionaNaveAsteroide(&jugador,&asteroides[j])){
                        ESTADO=GAME_OVER;
                        fondo_actual = 5;

                        BorrarAsteroide(asteroides[j]);
                        asteroides[j].activo = 0;
                        colisionDetectada = true;
                    }
                }
                colisionDetectada=false;
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
                colisionDetectada=false;
                Disparo *proyectil = &disparosNave[j];
                int k;
                if(proyectil->activo == ACTIVO && proyectil->orientacion_actual == SPR_NAVE_ARRIBA){
                    if(proyectil->y>0){
                        BorrarDisparo(proyectil);
                        proyectil->y = proyectil->y - 3;
                        MostrarDisparo(proyectil);

                        if(orbe_activo && colisionaDisparoOrbe(proyectil, &orbe)) {
                            orbe_recogido = true;
                            proyectil->activo = INACTIVO;
                            BorrarDisparo(proyectil);
                            contDisparos--;
                        }

                        for(k=0;k<MAX_ASTEROIDES && !colisionDetectada;k++){
                            if(asteroides[k].activo==1 && colisionaDisparoAsteroide(proyectil,&asteroides[k])){
                                BorrarDisparo(proyectil);
                                proyectil->activo = INACTIVO;
                                contDisparos--;

                                BorrarAsteroide(asteroides[k]);
                                asteroides[k].activo = 0;
                                colisionDetectada = true;
                            }
                        }
                        colisionDetectada=false;
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

                        if(orbe_activo && colisionaDisparoOrbe(proyectil, &orbe)) {
                            orbe_recogido = true;
                            proyectil->activo = INACTIVO;
                            BorrarDisparo(proyectil);
                            contDisparos--;
                        }
                        for(k=0;k<MAX_ASTEROIDES && !colisionDetectada;k++){
                            if(asteroides[k].activo==1 && colisionaDisparoAsteroide(proyectil,&asteroides[k])){
                                BorrarDisparo(proyectil);
                                proyectil->activo = INACTIVO;
                                contDisparos--;

                                BorrarAsteroide(asteroides[k]);
                                asteroides[k].activo = 0;
                                colisionDetectada = true;
                            }
                        }
                        colisionDetectada=false;
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

                        if(orbe_activo && colisionaDisparoOrbe(proyectil, &orbe)) {
                            orbe_recogido = true;
                            proyectil->activo = INACTIVO;
                            BorrarDisparo(proyectil);
                            contDisparos--;
                        }

                        for(k=0;k<MAX_ASTEROIDES && !colisionDetectada;k++){
                            if(asteroides[k].activo==1 && colisionaDisparoAsteroide(proyectil,&asteroides[k])){
                                BorrarDisparo(proyectil);
                                proyectil->activo = INACTIVO;
                                contDisparos--;

                                BorrarAsteroide(asteroides[k]);
                                asteroides[k].activo = 0;
                                colisionDetectada = true;
                            }
                        }
                        colisionDetectada=false;
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
                        if(orbe_activo && colisionaDisparoOrbe(proyectil, &orbe)) {
                            orbe_recogido = true;
                            proyectil->activo = INACTIVO;
                            BorrarDisparo(proyectil);
                            contDisparos--;
                        }
                        for(k=0;k<MAX_ASTEROIDES && !colisionDetectada;k++){
                            if(asteroides[k].activo==1 && colisionaDisparoAsteroide(proyectil,&asteroides[k])){
                                BorrarDisparo(proyectil);
                                proyectil->activo = INACTIVO;
                                contDisparos--;

                                BorrarAsteroide(asteroides[k]);
                                asteroides[k].activo = 0;
                                colisionDetectada = true;
                            }
                        }
                        colisionDetectada=false;
                    }else{
                        proyectil->activo =INACTIVO;
                        BorrarDisparo(proyectil);
                        contDisparos--;
                    }
                }
              }
            }

        }

        if(ESTADO==GAME_OVER){
            InhibirIntTempo();
            for(j = 0; j < MAX_ASTEROIDES; j++){
                if(asteroides[j].activo){
                     BorrarAsteroide( asteroides[j]);
                }
            }
            BorrarNave(jugador);
            for(j = 0; j <  MAX_DISPAROS; j++){
                Disparo *p = & disparosNave[j];
                if(p->activo == ACTIVO){
                    p->activo = INACTIVO;
                    BorrarDisparo(p);
                }
            }

           visualizarFondoGameOver();
        }
    }



    // Inhibir las interrupciones al final


/***********************2025-2026*******************************/


