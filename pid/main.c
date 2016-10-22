/********************* Adquisicion.c **************************/
/**************************************************************/

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pid.h"
#include "planta.h"

/* variables globales y struct compartida entre threads */

pid_handler_t p;
planta_handler_t plant;

void *status;                 /* exit thread status */
void *hilo_pid(void *);       /* declaro prototipo  */
float simular_planta();       /* declaro prototipo */
void controlar();

// buffer para mutex y cond
struct {
  pthread_mutex_t mutex;      /* variable mutex */
  pthread_cond_t cond;        /* condition de variable mutex*/
  int status_pid;
  int status_planta;
} buffer;

#define N 5000

int r = 12; // referencia/setpoint para la planta

/***************** inicio del programa principal *************/
/*************************************************************/
int main() {
  // configuracion de la planta y del pid
  pid_set(&p, 2, 1, 0, 0.5);
  //planta_set();

  // status de pid y planta
  buffer.status_pid = 1; // asumimos que se hizo un calculo previo
  buffer.status_planta = 0; // asumimos que no se ha ejecutado

  // creacion del hilo de controlador
  pthread_t rd1;            /* identificador del hilo lectura */
  pthread_attr_t attr;      /* attributes */

  /* initializo mutex */
  if (pthread_mutex_init(&buffer.mutex,NULL)) {
    printf("Error: no puede inicializar mutex\n");
    exit(1);
  }
  /* initializo condition de variable */
  if (pthread_cond_init(&buffer.cond,NULL)) {
    printf("Error: no puede initicializar condicion var\n");
    exit(1);
  }
  /* initializo atributos de hilos */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE );
  //--------------------------------------------------------

  /* crea el hilo: Lectura-Impresion */
  if (pthread_create(&rd1,&attr,hilo_pid,NULL)) {
    printf("Error: no puede crear lectura\n");
     exit(1);
  }

  /* -------------------- 'main' es un hilo 1 ------------------------*/
  /* -------------  Este hilo realiza la adquisicion de datos --------*/
  printf(" \n"); 	printf(" \n"); 	printf(" \n");
  printf(" -----------------------------------------------------------\n");
  printf("  Los 2 hilos estan ejecutandose concurrentemente ahora.....\n");
  printf(" * EL hilo 0 simula un periodo de la planta *\n");
  printf(" * EL hilo 1 simula un periodo del controlador    *\n");
  printf(" -------------------------------------------------------------\n");
  printf(" \n"); 	printf(" \n");

  /*inicio de la adquisicion de las 5 muestras de datos cada 1 segundos*/
  for(int i = 0; i < N; i++) {
    sleep(1);
    printf("   (Hilo 0) -> Simulando planta :  ");
    float y = simular_planta();     /* escribe valor adquirido en buffer compartido*/
    printf("%f\n ",y);
   }

  /* -------------------------------------------------*/
  /* espera hasta que el hilo lectura/impresor termine */

  if (pthread_join(rd1,&status)) {
    printf("Error: no puede join lectura\n");
    exit(1);
  }
  printf("    !Termino Hilo2!....\n");
  printf(" \n"); 	printf(" \n");
  exit(0);
} // Fin del main
/*------------------------ hilo 2-----------------------------*/

/*------ Este hilo realiza la impresion de los datos-----------*/
void *hilo_pid(void *arg) {
  int c,y;
  int read_from_buffer();
   y=0;
  for(int i = 0; i < N; i++) {
     sleep(1);  			 /*retardo en la lectura e impresion*/
     controlar();   	 /*lee el valor del buffer compartido*/
     printf("  (Hilo 2) <- Valor leido del Buffer        **  : ");
     printf("%f\n",p.u);    	/*imprime valor del buffer*/
  }
}

/*-----------------------------------------------------------*/
float simular_planta() {
  pthread_mutex_lock(&buffer.mutex);    /*  lock mutex */
  if(buffer.status_pid){
    pthread_cond_wait(&buffer.cond,&buffer.mutex);
    buffer.status_pid = 0;
  }

  planta_run(&plant, p. u);

  buffer.status_planta = 1;

  /* despierta al hilo lectura*/
  pthread_cond_signal(&buffer.cond);
  pthread_mutex_unlock(&buffer.mutex);   /* unlock mutex */

  return plant.y;
}

/*-----------------------------------------------------------*/
void controlar() {
  pthread_mutex_lock(&buffer.mutex);     /* lock mutex */
  if(buffer.status_planta) {
    pthread_cond_wait(&buffer.cond,&buffer.mutex);
    buffer.status_planta = 0;
  }

  pid_run(&p,r-plant.y);
  buffer.status_pid = 1;

  /* despierta el hilo printerr */
  pthread_cond_signal(&buffer.cond);
  pthread_mutex_unlock(&buffer.mutex);  /* unlock mutex */
}
