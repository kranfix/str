#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "planta.h"

/* debug */
void debug();

/* Declaracion de los procesos de los actuadores */
void robotX();
void robotY();
void pistonC();

int caja[10] = {80,90,100,101,99,110,40,50,76,20}; // Valores para D

/* Declarando y definiendo handler banda */
#define N_banda 6
int banda_casilla[N_banda] = {0,0,0,0,0,0};
int banda_nivel_D[N_banda] = {0,0,0,0,0,0};
banda_t banda = {N_banda,3,100,banda_casilla,banda_nivel_D};

/* Declaracion de las variables compartidas */
struct{
  int n; // numero de cajas en la banda
  int N; // numero de cajas a entregar
  int nivel_D; // nidel D de la caja entrante
} buf; // buffer

/*struct{
  int N; // numero de iteraciones totales
  int i; // indice de la caja entregada
  int
  int * caja;
} ctrl = {16,caja};
*/
int main(){
  int i;
  int a,b,d,e,f; // variables tratadas como booleanas

  buf.n = 0;
  buf.N = 10;

  for(i = 0; i < 15; i++){
    printf("Tiempo de ejecucion: %d\n",i);
    leerSensores(&banda,&a,&b,&d);
    e = b | d; // banda bloqueada?
    buf.nivel_D = caja[i];
    if(!a && buf.N > 0){ // Si la banda no esta bloqueada
      robotX();
      debug();
    }
    if(b){
      robotY();
      debug();
    }
    if(d){
      pistonC();
      debug();
    }
    runBanda(&banda);
    debug();
  }
  return 0;
}

/* Definciones de los procesos de los actuadores */
void robotX(){
  if(buf.N > 0){
    runX(&banda,buf.nivel_D);
    buf.N--;
    buf.n++;
  }
}

void robotY(){
  runY(&banda);
  buf.n--;
}

void pistonC(){
  runC(&banda);
  buf.n--;
}

void debug(){
  printf("    %d\t%d\t%d\t%d\t%d\t%d\n",banda.casilla[0],
      banda.casilla[1],banda.casilla[2],banda.casilla[3],
      banda.casilla[4],banda.casilla[5]);
  printf("    %d\t%d\t%d\t%d\t%d\t%d\n",banda.nivel_D[0],
      banda.nivel_D[1],banda.nivel_D[2],banda.nivel_D[3],
      banda.nivel_D[4],banda.nivel_D[5]);
}
