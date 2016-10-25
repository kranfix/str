#include <stdio.h>
#include <stdlib.h>
#include "planta.h"
#include "pid.h"

// Los handlers de los controladores
pid_handler_t pi_F, pi_T;
float r_temp = 80; // temperatura de referencia

void controlar_temp();
void controlar_flujo();
void controlar_planta();

int main(){
  pid_set(&pi_F, 4, 0, 0, 0.05);
  pid_set(&pi_T, 60, 3, 0, 1);

  int i,j;
  for(i = 0; i < 30; i++){
    controlar_temp();
    for(j = 0; j < 20; j++){
      controlar_flujo();
      controlar_planta();
    }
  }
  return 0;
}

void controlar_temp(){
  puts("   Actualizando lazo de temperatura");
  float temp = adquisicion_temp();
  pid_run(&pi_T, r_temp - temp);
  puts("   Lazo de temperatura actualizado");
}

void controlar_flujo(){
  puts("   Actualizando lazo de flujo");
  float flujo = adquisicion_flujo();
  pid_run(&pi_F, pi_T.u - flujo);
  puts("   Lazo de flujo actualizado");
}

void controlar_planta(){
  puts("   Actuando sobre la planta");
  actuacion(pi_F.u);
  puts("   Planta actuada");
}
