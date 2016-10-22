#ifndef PLANTA_ACTUADORES_H
#define PLANTA_ACTUADORES_H

#include <stdio.h>

/* Manipulador de la planta */

typedef struct {
  int n;         // numero de casillas
  int m;         // posicion del sensor D
  int max_D;     // Tope del sensor D
  int * casilla; // hay caja
  int * nivel_D; // para el sensor 
} banda_t;

void runY();     // Actuador del brazo Y
void runC();     // Actuador del piston C
void runBanda(); // Actuador de la banda X

/* Sensores */
void leerSensores(banda_t * banda, int * A, int *B, int *D);

#endif
