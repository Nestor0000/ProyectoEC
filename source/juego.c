// Curso 2025-2026

/*---------------------------------------------------------------------------------
Este código se ha desarrollado basándose en el código de dovoto "Simple sprite demo"
y en otro ejemplo de Jaeden Ameronen
---------------------------------------------------------------------------------*/

#include <nds.h>    // Librería de la NDS
#include <stdio.h>  // Librería de entrada/salida estándar de C
#include <stdlib.h> // Librería estándar de C para reserva de memoria y conversiones numéricas
#include <unistd.h> // Librería para asegurar la compatibilidad entre sistemas operativos

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
int fase_actual = 1;           // Empieza por defecto en la fase 1
int velocidad_fase = 1;        // Velocidad con la que inician los enemigos
int espera_spawn_fase = 25;    // Cada cuanto tardan en aparecer los enemigos (en ticks)
int spawnasteroides_timer = 0; // Para controlar el tiempo entre spawns de enemigos

// --- REGISTROS DE MOVIMIENTO Y DISPAROS ---
int contDisparos;
int teclaPulsada;
volatile int cooldown_disparo = 0;

// --- MISCELANEO / ESTADO ---
int tempo_activado = 0;
int asteroides_inicializados = 0;
volatile int fondo_actual;

// --- ENTIDADES IN-GAME ---
Nave jugador;
Disparo disparosNave[MAX_DISPAROS];
Asteroide asteroides[MAX_ASTEROIDES];

bool colisionDetectada;
volatile int contador_orbe = 0;
volatile bool orbe_activo = false;
Orbe orbe;
bool orbe_recogido = false;
//esta funcion detecta colisiones en base a coordenadas y tamaños de la hitbox, es generico, hay funciones mas especificas que son simplemente para que
//al desarrollador le sea mas comodo utilizar las funciones de colisiones al no tener que llamar a esto y solo tener que pasar las entidades necesarias
int colisiona(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    return (x1 < x2 + w2) && (x1 + w1 > x2) && (y1 < y2 + h2) && (y1 + h1 > y2);
}
//este es especifico entre disparo y asteroide, lo que en realidad esto hace es llamar a la funcion colisiona pero es  mas facil para el desarrollador usar esta funcion
int colisionaDisparoAsteroide(Disparo *d, Asteroide *a)
{
    return colisiona(d->x, d->y, d->hitbox.w, d->hitbox.h, a->x, a->y, a->hitbox.w, a->hitbox.h);
}
// lo mismo pero para entre nave y asteroide
int colisionaNaveAsteroide(Nave *n, Asteroide *a)
{
    return colisiona(n->x, n->y, n->hitbox.w, n->hitbox.h, a->x, a->y, a->hitbox.w, a->hitbox.h);
}
// lo mismo pero para entre disparo y orbe
int colisionaDisparoOrbe(Disparo *d, Orbe *o)
{
    return colisiona(d->x, d->y, d->hitbox.w, d->hitbox.h, o->x, o->y, o->hitbox.w, o->hitbox.h);
}
void InitAsteroides()
{
    int i;
    for (i = 0; i < MAX_ASTEROIDES; i++)
    {
        asteroides[i].activo = INACTIVO;
        asteroides[i].indice = i + 16;
    }
}
void SpawnAsteroide()
{
    int i;
    for (i = 0; i < MAX_ASTEROIDES; i++)
    {

        if (asteroides[i].activo == INACTIVO )
        {

            int lado = rand() % 4;
            asteroides[i].lado = lado;
            int speed = velocidad_fase;

            if (lado == 0)
            { // esto es el lado de arriba
                asteroides[i].x = rand() % 256;
                asteroides[i].y = 0;
                asteroides[i].vx = 0;
                asteroides[i].vy = speed;
            }

            else if (lado == 1)
            { // esto es el lado de abajo
                asteroides[i].x = rand() % 256;
                asteroides[i].y = 192;
                asteroides[i].vx = 0;
                asteroides[i].vy = -speed;
            }

            else if (lado == 2)
            { // esto es el lado de izquierda
                asteroides[i].x = 0;
                asteroides[i].y = rand() % 192;
                asteroides[i].vx = speed;
                asteroides[i].vy = 0;
            }

            else
            { // esto es el lado derecha
                asteroides[i].x = 256;
                asteroides[i].y = rand() % 192;
                asteroides[i].vx = -speed;
                asteroides[i].vy = 0;
            }

            asteroides[i].hitbox.offsetX = 0;
            asteroides[i].hitbox.offsetY = 0;
            asteroides[i].hitbox.w = 16;
            asteroides[i].hitbox.h = 16;
            asteroides[i].activo = ACTIVO;
            break;
        }
    }
}
void AparicionAsteroides()
{
    int i;

    for (i = 0; i < MAX_ASTEROIDES; i++)
    {

        if (asteroides[i].activo)
        {

            // 1. borrar posición anterior
            BorrarAsteroide(asteroides[i]);

            // 2. actualizar posición según lado
            if (asteroides[i].lado == 0)
            {
                asteroides[i].y += velocidad_fase;
            }
            else if (asteroides[i].lado == 1)
            {
                asteroides[i].y -= velocidad_fase;
            }
            else if (asteroides[i].lado == 2)
            {
                asteroides[i].x += velocidad_fase;
            }
            else if (asteroides[i].lado == 3)
            {
                asteroides[i].x -= velocidad_fase;
            }

            // 3. volver a dibujar en nueva posición
            MostrarAsteroide(asteroides[i]);
        }
    }
    GuardarSpriteAsteroideMemoria();
}
void Eliminar_Asteroides()
{
    // elimina los astereoides que se salen de la pantalla
    int i;
    for (i = 0; i < MAX_ASTEROIDES; i++)
    {

        if (asteroides[i].activo == ACTIVO)
        {

            // si se sale de pantalla → desactivar
            if (asteroides[i].x < -32 || asteroides[i].x > 256 ||
                asteroides[i].y < -32 || asteroides[i].y > 192)
            {

                // opcional: borrarlo visualmente antes de apagarlo
                BorrarAsteroide(asteroides[i]);

                asteroides[i].activo = INACTIVO;
            }
        }
    }
}
//esta funcion se llamara siempre ya que gestiona internamente el tema de eliminar y mostrar en funcion de sus condiciones
void Spawn_Orbe()
{
    orbe.recarga_balas = 20;
    if (!orbe_activo)
    {
        if (contador_orbe >= 600 + (rand() % 1200))
        {
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
    if (orbe_recogido)
    {
        BorrarOrbe(orbe, SPR_ORBE);
        orbe_activo = false;
        orbe_recogido = false;
        contador_orbe = 0;
    }
}
void juego()
{

    // Definiciones de variables
    int i = 0;
    int tecla = 0;
    ESTADO = MENU;
    int cooldown_rotacion = 0;

    for (i = 0; i < MAX_DISPAROS; i++)
    {
        disparosNave[i].indice = i + 5;
    }


    irqInit();
    irqEnable(IRQ_VBLANK);
    ConfigurarTeclado(0x43FF);
    ConfigurarTemporizador(61440, 0x0041);
    EstablecerVectorInt();
    HabilitarIntTeclado();
    HabilitarIntTempo();
    irqEnable(IRQ_KEYS | IRQ_TIMER0);
    HabilitarInterrupciones();
    inicializarFondos();
    jugador.x = 110;
    jugador.y = 96;
    jugador.orientacion_actual = SPR_NAVE_ARRIBA;
    fondo_actual = 0;

    jugador.hitbox.offsetX = 0;
    jugador.hitbox.offsetY = 0;
    jugador.hitbox.w = 16;
    jugador.hitbox.h = 16;

    while (1)
    {

        swiWaitForVBlank();
        if (ESTADO == MENU)
        {
            visualizarFondoMenu();
            fondo_actual = 0;
            touchRead(&PANT_DAT);

            if (PANT_DAT.px > 0 && PANT_DAT.py > 0 || TeclaPulsada() == ARRIBA)
            {
                ESTADO = GAME;
            }
        }

        /*******************************EN LA 1.ACTIVIDAD *****************************************/
        // Si el estado es ESPERA: codificar aquí la encuesta del teclado, sacar por pantalla la tecla que se ha pulsado, y si se pulsa la tecla START cambiar de estado */

        if (ESTADO == GAME)
        {
            swiWaitForVBlank();
            Spawn_Orbe();
            srand(TIMER0_DAT);

            if (tempo_activado == 0)
            {
                PonerEnMarchaTempo();
                tempo_activado = 1;
            }
            if (asteroides_inicializados == 0)
            {
                InitAsteroides();
                asteroides_inicializados = 1;
            }
            if (fondo_actual == 0)
            {
                visualizarFondo1();
                fondo_actual = 1;
            }

            MostrarNave(jugador);
            GuardarSpritesMemoria(jugador.orientacion_actual);
            if (cooldown_rotacion > 0)
            {
                cooldown_rotacion--;
            }
            spawnasteroides_timer++;
            if (spawnasteroides_timer > 25)
            {
                SpawnAsteroide();
                spawnasteroides_timer = 0;
            }
            Eliminar_Asteroides();
            AparicionAsteroides();
            GuardarSpriteAsteroideMemoria();

            teclaPulsada = TeclaPulsada();
            if (teclaPulsada == DERECHA && jugador.x < 225)
            {
                BorrarNave(jugador);
                jugador.x = jugador.x + 2;
                MostrarNave(jugador);

                colisionDetectada = false;
                for (i = 0; i < MAX_ASTEROIDES && !colisionDetectada; i++)
                {
                    if (asteroides[i].activo == ACTIVO && colisionaNaveAsteroide(&jugador, &asteroides[i]))
                    {
                        ESTADO = GAME_OVER;
                        fondo_actual = 5;
                        BorrarAsteroide(asteroides[i]);
                        asteroides[i].activo = INACTIVO;
                        colisionDetectada = true;
                    }
                }
                colisionDetectada = false;
            }
            else if (teclaPulsada == IZQUIERDA && jugador.x > 0)
            {
                BorrarNave(jugador);
                jugador.x = jugador.x - 2;
                MostrarNave(jugador);
                colisionDetectada = false;
                for (i = 0; i < MAX_ASTEROIDES && !colisionDetectada; i++)
                {
                    if (asteroides[i].activo == ACTIVO && colisionaNaveAsteroide(&jugador, &asteroides[i]))
                    {
                        ESTADO = GAME_OVER;
                        fondo_actual = 5;
                        BorrarAsteroide(asteroides[i]);
                        asteroides[i].activo =INACTIVO ;
                        colisionDetectada = true;
                    }
                }
                colisionDetectada = false;
            }
            else if (teclaPulsada == ARRIBA && jugador.y > 0)
            {
                BorrarNave(jugador);
                jugador.y = jugador.y - 2;
                MostrarNave(jugador);
                colisionDetectada = false;
                for (i = 0; i < MAX_ASTEROIDES && !colisionDetectada; i++)
                {
                    if (asteroides[i].activo == ACTIVO && colisionaNaveAsteroide(&jugador, &asteroides[i]))
                    {
                        ESTADO = GAME_OVER;
                        fondo_actual = 5;
                        BorrarAsteroide(asteroides[i]);
                        asteroides[i].activo = INACTIVO ;
                        colisionDetectada = true;
                    }
                }
                colisionDetectada = false;
            }
            else if (teclaPulsada == ABAJO && jugador.y < 165)
            {
                BorrarNave(jugador);
                jugador.y = jugador.y + 2;
                MostrarNave(jugador);
                colisionDetectada = false;
                for (i = 0; i < MAX_ASTEROIDES && !colisionDetectada; i++)
                {
                    if (asteroides[i].activo == ACTIVO && colisionaNaveAsteroide(&jugador, &asteroides[i]))
                    {
                        ESTADO = GAME_OVER;
                        fondo_actual = 5;
                        BorrarAsteroide(asteroides[i]);
                        asteroides[i].activo = INACTIVO ;
                        colisionDetectada = true;
                    }
                }
                colisionDetectada = false;
            }
            if (cooldown_rotacion == 0 && ((teclaPulsada == R && jugador.orientacion_actual == SPR_NAVE_ARRIBA) || (teclaPulsada == L && jugador.orientacion_actual == SPR_NAVE_ABAJO)))
            {
                BorrarNave(jugador);
                jugador.orientacion_actual = SPR_NAVE_DERECHA;
                MostrarNave(jugador);
                GuardarSpritesMemoria(jugador.orientacion_actual);
                cooldown_rotacion = 25;
            }
            else if (cooldown_rotacion == 0 && ((teclaPulsada == R && jugador.orientacion_actual == SPR_NAVE_DERECHA) || (teclaPulsada == L && jugador.orientacion_actual == SPR_NAVE_IZQUIERDA)))
            {
                BorrarNave(jugador);
                jugador.orientacion_actual = SPR_NAVE_ABAJO;
                MostrarNave(jugador);
                GuardarSpritesMemoria(jugador.orientacion_actual);
                cooldown_rotacion = 25;
            }
            else if (cooldown_rotacion == 0 && ((teclaPulsada == L && jugador.orientacion_actual == SPR_NAVE_DERECHA) || (teclaPulsada == R && jugador.orientacion_actual == SPR_NAVE_IZQUIERDA)))
            {
                BorrarNave(jugador);
                jugador.orientacion_actual = SPR_NAVE_ARRIBA;
                MostrarNave(jugador);
                GuardarSpritesMemoria(jugador.orientacion_actual);
                cooldown_rotacion = 25;
            }
            else if (cooldown_rotacion == 0 && ((teclaPulsada == L && jugador.orientacion_actual == SPR_NAVE_ARRIBA) || (teclaPulsada == R && jugador.orientacion_actual == SPR_NAVE_ABAJO)))
            {
                BorrarNave(jugador);
                jugador.orientacion_actual = SPR_NAVE_IZQUIERDA;
                MostrarNave(jugador);
                GuardarSpritesMemoria(jugador.orientacion_actual);
                cooldown_rotacion = 25;
            }

            for (i = 0; i < MAX_DISPAROS; i++)
            {
                colisionDetectada = false;
                Disparo *proyectil = &disparosNave[i];
                int j;
                if (proyectil->activo == ACTIVO && proyectil->orientacion_actual == SPR_NAVE_ARRIBA)
                {
                    if (proyectil->y > 0)
                    {
                        BorrarDisparo(proyectil);
                        proyectil->y = proyectil->y - 3;
                        MostrarDisparo(proyectil);

                        if (orbe_activo && colisionaDisparoOrbe(proyectil, &orbe))
                        {
                            orbe_recogido = true;
                            proyectil->activo = INACTIVO;
                            BorrarDisparo(proyectil);
                            contDisparos--;
                        }

                        for (j = 0; j < MAX_ASTEROIDES && !colisionDetectada; j++)
                        {
                            if (asteroides[j].activo == ACTIVO && colisionaDisparoAsteroide(proyectil, &asteroides[j]))
                            {
                                BorrarDisparo(proyectil);
                                proyectil->activo = INACTIVO;
                                contDisparos--;

                                BorrarAsteroide(asteroides[j]);
                                asteroides[j].activo = INACTIVO;
                                colisionDetectada = true;
                            }
                        }
                        colisionDetectada = false;
                    }
                    else
                    {
                        proyectil->activo = INACTIVO;
                        BorrarDisparo(proyectil);
                        contDisparos--;
                    }
                }
                else if (proyectil->activo == ACTIVO && proyectil->orientacion_actual == SPR_NAVE_DERECHA)
                {
                    if (proyectil->x < 225)
                    {
                        BorrarDisparo(proyectil);
                        proyectil->x = proyectil->x + 3;
                        MostrarDisparo(proyectil);

                        if (orbe_activo && colisionaDisparoOrbe(proyectil, &orbe))
                        {
                            orbe_recogido = true;
                            proyectil->activo = INACTIVO;
                            BorrarDisparo(proyectil);
                            contDisparos--;
                        }
                        for (j = 0; j < MAX_ASTEROIDES && !colisionDetectada; j++)
                        {
                            if (asteroides[j].activo == ACTIVO && colisionaDisparoAsteroide(proyectil, &asteroides[j]))
                            {
                                BorrarDisparo(proyectil);
                                proyectil->activo = INACTIVO;
                                contDisparos--;

                                BorrarAsteroide(asteroides[j]);
                                asteroides[j].activo = INACTIVO;
                                colisionDetectada = true;
                            }
                        }
                        colisionDetectada = false;
                    }
                    else
                    {
                        proyectil->activo = INACTIVO;
                        BorrarDisparo(proyectil);
                        contDisparos--;
                    }
                }
                else if (proyectil->activo == ACTIVO && proyectil->orientacion_actual == SPR_NAVE_ABAJO)
                {
                    if (proyectil->y < 165)
                    {
                        BorrarDisparo(proyectil);
                        proyectil->y = proyectil->y + 3;
                        MostrarDisparo(proyectil);

                        if (orbe_activo && colisionaDisparoOrbe(proyectil, &orbe))
                        {
                            orbe_recogido = true;
                            proyectil->activo = INACTIVO;
                            BorrarDisparo(proyectil);
                            contDisparos--;
                        }

                        for (j = 0; j < MAX_ASTEROIDES && !colisionDetectada; j++)
                        {
                            if (asteroides[j].activo == ACTIVO && colisionaDisparoAsteroide(proyectil, &asteroides[j]))
                            {
                                BorrarDisparo(proyectil);
                                proyectil->activo = INACTIVO;
                                contDisparos--;

                                BorrarAsteroide(asteroides[j]);
                                asteroides[j].activo = INACTIVO;
                                colisionDetectada = true;
                            }
                        }
                        colisionDetectada = false;
                    }
                    else
                    {
                        proyectil->activo = INACTIVO;
                        BorrarDisparo(proyectil);
                        contDisparos--;
                    }
                }
                else if (proyectil->activo == ACTIVO && proyectil->orientacion_actual == SPR_NAVE_IZQUIERDA)
                {
                    if (proyectil->x > 0)
                    {
                        BorrarDisparo(proyectil);
                        proyectil->x = proyectil->x - 3;
                        MostrarDisparo(proyectil);
                        if (orbe_activo && colisionaDisparoOrbe(proyectil, &orbe))
                        {
                            orbe_recogido = true;
                            proyectil->activo = INACTIVO;
                            BorrarDisparo(proyectil);
                            contDisparos--;
                        }
                        for (j = 0; j < MAX_ASTEROIDES && !colisionDetectada; j++)
                        {
                            if (asteroides[j].activo == ACTIVO && colisionaDisparoAsteroide(proyectil, &asteroides[j]))
                            {
                                BorrarDisparo(proyectil);
                                proyectil->activo = INACTIVO;
                                contDisparos--;

                                BorrarAsteroide(asteroides[j]);
                                asteroides[j].activo = INACTIVO;
                                colisionDetectada = true;
                            }
                        }
                        colisionDetectada = false;
                    }
                    else
                    {
                        proyectil->activo = INACTIVO;
                        BorrarDisparo(proyectil);
                        contDisparos--;
                    }
                }
            }
        }
        else if (ESTADO == GAME_OVER)
    {
        for (i = 0; i < MAX_ASTEROIDES; i++)
        {
            if (asteroides[i].activo)
            {
                BorrarAsteroide(asteroides[i]);
            }
        }
        BorrarNave(jugador);
        for (i = 0; i < MAX_DISPAROS; i++)
        {
            Disparo *p = &disparosNave[i];
            if (p->activo == ACTIVO)
            {
                p->activo = INACTIVO;
                BorrarDisparo(p);
            }
        }
        fondo_actual = 5;
        visualizarFondoGameOver();
    }
    }
    
}

// Inhibir las interrupciones al final

/***********************2025-2026*******************************/
