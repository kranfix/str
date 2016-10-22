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

void runX(banda_t * banda, int nivel_D);
void runY(banda_t * banda);     // Actuador del brazo Y
void runC(banda_t * banda);     // Actuador del piston C
void runBanda(banda_t * banda); // Actuador de la banda

/* Sensores */
void leerSensores(banda_t * banda, int * A, int *B, int *D);

#endif
