#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "planta.h"
#include "pid.h"

// Los handlers de los controladores
pid_handler_t pi_V, pi_T;
float r_temp = 80; // temperatura de referencia

// buffer de control
struct{
  pthread_mutex_t mutex;
  pthread_cond_t cond_temp;
  pthread_cond_t cond_flujo;
  int temp_status;
} buf;   // buffer

void * controlar_temp();
void * controlar_flujo();
void * controlar_planta();

int main(){
  pid_set(&pi_V, 4, 0, 0, 0.05);
  pid_set(&pi_T, 60, 3, 0, 1);

  // hilo para control de flujo, control de temperatura y simu Planta
  pthread_t hiloF, hiloT, hiloP;

  // initializo mutex
  if (pthread_mutex_init(&buf.mutex,NULL)) {
    printf("Error: no puede inicializar mutex\n");
    exit(1);
  }

  // initializo condition de variable
  if (pthread_cond_init(&buf.cond_temp,NULL)) {
    printf("Error: no puede initicializar condicion var\n");
    exit(1);
  }
  if (pthread_cond_init(&buf.cond_flujo,NULL)) {
    printf("Error: no puede initicializar condicion var\n");
    exit(1);
  }

  // atributo
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

  int i,j;
  for(i = 0; i < 30; i++){
    buf.temp_status = 1;
    pthread_create(&hiloT, &attr, controlar_temp, NULL);
    for(j = 0; j < 20; i++){
      pthread_create(&hiloF, &attr, controlar_flujo, NULL);
      pthread_create(&hiloP, &attr, controlar_planta, NULL);

      buf.temp_status = 0;

      pthread_join(hiloF, NULL);
      pthread_join(hiloP, NULL);
    }
    pthread_join(hiloF, NULL);
  }

  pthread_attr_destroy(&attr);
  pthread_exit(NULL);

  return 0;
}

void * controlar_temp(){
  puts("   Actualizando lazo de temperatura");
  pthread_mutex_lock(&buf.mutex);

  float temp = adquisicion_temp();
  pid_run(&pi_T, r_temp - temp);

  pthread_cond_signal(&buf.cond_temp);
  pthread_mutex_unlock(&buf.mutex);

  pthread_exit(NULL);
  puts("   Lazo de temperatura actualizado");
}

void * controlar_flujo(){
  puts("   Actualizando lazo de flujo");
  pthread_mutex_lock(&buf.mutex);
  if(buf.temp_status){
    pthread_cond_wait(&buf.cond_temp,&buf.mutex);
  }

  float flujo = adquisicion_flujo();
  pid_run(&pi_V, pi_T.u - flujo);

  pthread_cond_signal(&buf.cond_flujo);
  pthread_mutex_unlock(&buf.mutex);
  pthread_exit(NULL);
  puts("   Lazo de flujo actualizado");
}

void * controlar_planta(){
  puts("   Actuando sobre la planta");
  pthread_mutex_lock(&buf.mutex);
  pthread_cond_wait(&buf.cond_flujo,&buf.mutex);

  actuacion(pi_V.u);

  pthread_mutex_unlock(&buf.mutex);
  pthread_exit(NULL);
  puts("   Planta actuada");
}
