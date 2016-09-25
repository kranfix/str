#ifndef PLANTA_H
#define PLANTA_H

typedef struct planta_handler_t {
  int dim; // dimension - numero de estados
  float **A;
  float *B;
  float *C;
  float *x;
} planta_handler_t;

void planta_run(planta_handler_t * p, float u);

#endif // PLANTA_H
