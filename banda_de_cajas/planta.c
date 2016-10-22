#include "planta.h"

// Actuador del brazo X
void runX(){
  int i;
  puts("    Actuando brazo X");
  for(i = 0; i < 500000; i++);
  puts("    Robot X terminado");
}

// Actuador del brazo Y
void runY(){
  int i;
  puts("   Actuando brazo Y");
  for(i = 0; i < 600000; i++);
  puts("   Robot Y terminado");
}

// Actuador del piston C
void runC(){
  int i;
  puts("   Actuando piston C");
  for(i = 0; i < 200000; i++);
  puts("   Piston C terminado");
}

// Actuador de la banda X
void runBanda(banda_t * banda){
  int i;
  puts("   Actuando un paso de la banda");
  for(i = banda->n-1; i > 0; i--){
    banda->casilla[i] = banda->casilla[i-1];
    banda->nivel_D[i] = banda->nivel_D[i-1]; 
  }
  banda->casilla[0] = 0;
  banda->nivel_D[0] = 0; 

  for(i = 0; i < 1000000; i++);
  puts("   Paso de la banda terminado");
}

// Lectura de sensores
void leerSensores(banda_t * banda, int * A, int *B, int *D){
  puts("    Leyendo sensores");
  *A = banda->casilla[0];
  *B = banda->casilla[banda->n - 1];
  *D = banda->nivel_D[banda->m] > banda->max_D;
  puts("    Sensores leidos");
}
