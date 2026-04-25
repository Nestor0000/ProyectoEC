// Curso 2025-2026

#include <nds.h> 		
#include <stdio.h>		
#include <stdlib.h>		
#include <unistd.h>		

/* Si queremos visualizar distintos fondos, aquí se debe incluir el fichero de cabecera (fichero .h) de cada fondo. Estos ficheros de cabecera se generan automáticamente durante la compilación */

#include "fondos.h"
#include "graficos.h"
#include "fondo1.h"
#include "fondo2.h"
#include "fondo3.h"
#include "fondo4.h"

/* Se elige el canal de DMA que se utilizará para copiar las imágenes en memoria */
static const int DMA_CHANNEL = 3;

/* Para cada fondo que se quiera visualizar hay que escribir un procedimiento como el siguiente */


void visualizarFondo1(){
    dmaCopyHalfWords(DMA_CHANNEL,
                    fondo1Bitmap,
                    (uint16 *)BG_BMP_RAM(0),
                    fondo1BitmapLen
    );
}

void visualizarFondo2(){
	dmaCopyHalfWords(DMA_CHANNEL,fondo2Bitmap,(uint16 *)BG_BMP_RAM(0),fondo1BitmapLen);
}
void visualizarFondo3(){
	dmaCopyHalfWords(DMA_CHANNEL,fondo3Bitmap,(uint16 *)BG_BMP_RAM(0),fondo3BitmapLen);
}
void visualizarFondo4(){
	dmaCopyHalfWords(DMA_CHANNEL,fondo4Bitmap,(uint16 *)BG_BMP_RAM(0),fondo4BitmapLen);
}

/***********************2025-2026*******************************/

