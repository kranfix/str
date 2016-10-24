#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "planta.h"

/* debug */
void debug();

/* Declaracion de los procesos de los actuadores */
void * robotX();
void * robotY();
void * pistonC();

/* Valores que sensara D para cada caja */
int caja[10] = {80,90,100,101,99,110,40,50,76,20};

/* Declaracion de las variables compartidas */
struct{
  pthread_mutex_t mutex;
  int n;       // numero de cajas en la banda
  int N;       // numero de cajas a entregar
  int nivel_D; // nidel D de la caja entrante
} buf;   // buffer

/* Declarando y definiendo handler banda */
#define N_banda 6
int banda_casilla[N_banda] = {0,0,0,0,0,0};
int banda_nivel_D[N_banda] = {0,0,0,0,0,0};
banda_t banda = {N_banda,3,100,banda_casilla,banda_nivel_D};

int main(){
  int i;
  int a,b,d, e,f,g; // variables tratadas como booleanas

  pthread_t hiloX, hiloY, hiloC;

  // atributo
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

  buf.n = 0;
  buf.N = 10;

  for(i = 0; i < 15; i++){
    printf("Tiempo de ejecucion: %d\n",i);
    leerSensores(&banda,&a,&b,&d);
    buf.nivel_D = caja[i];
    e = !a & (buf.N > 0);
    f = b;
    g = d;

    if(e){
      pthread_create(&hiloX, &attr, robotX, NULL);
    }
    if(b){
      pthread_create(&hiloY, &attr, robotY, NULL);
    }
    if(d){
      pthread_create(&hiloC, &attr, pistonC, NULL);
    }

    if(e){
      pthread_join(hiloX, NULL);
    }
    if(f){
      pthread_join(hiloY, NULL);
    }
    if(g){
      pthread_join(hiloC, NULL);
    }
    debug();
    runBanda(&banda);
    debug();
  }

  pthread_attr_destroy(&attr);
  pthread_exit(NULL);

  return 0;
}


/* Definciones de los procesos de los actuadores */
void * robotX(){
  pthread_mutex_lock(&buf.mutex);

  if(buf.N > 0){
    runX(&banda,buf.nivel_D);
    buf.n++;
    buf.N--;
  }

  pthread_mutex_unlock(&buf.mutex);
  pthread_exit(NULL);
}

void * robotY(){
  pthread_mutex_lock(&buf.mutex);

  runY(&banda);
  buf.n--;

  pthread_mutex_unlock(&buf.mutex);
  pthread_exit(NULL);
}

void * pistonC(){
  pthread_mutex_lock(&buf.mutex);

  runC(&banda);
  buf.n--;

  pthread_mutex_unlock(&buf.mutex);
  pthread_exit(NULL);
}

void debug(){
  printf("    %d\t%d\t%d\t%d\t%d\t%d\n",banda.casilla[0],
      banda.casilla[1],banda.casilla[2],banda.casilla[3],
      banda.casilla[4],banda.casilla[5]);
  printf("    %d\t%d\t%d\t%d\t%d\t%d\n",banda.nivel_D[0],
      banda.nivel_D[1],banda.nivel_D[2],banda.nivel_D[3],
      banda.nivel_D[4],banda.nivel_D[5]);
}
