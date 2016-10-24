#include "sys_c.h"

float sys_run(planta_handler_t * p, float u){
  float y = 0;

  for(int i = 0; i < p->dim; i++){
    y += p->C[i] * p->x[i];
    float x = p->B[i] * u;

    for(int j = 0; j < p->dim; j++){
      x += p->A[i][j] * p->x[j];
    }
    p->x[i] = x;
  }
  p->y = y;
  return y;
}
