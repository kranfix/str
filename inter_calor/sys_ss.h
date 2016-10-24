#ifndef SYS_SS_H
#define SYS_SS_H

typedef struct ss_t {
  int dim; // dimension - numero de estados
  float **A;
  float *B;
  float *C;
  float *x;
  float y;
} ss_t;

float sys_run(ss_t * p, float u);

#endif // SYS_SS_H
