#include "planta.h"

float A1[2] = {0.9048, 0};
float A2[2] = {0.0024, 0.9975};
float *sys_A[2] = { A1, A2 };
float sys_B[2] = {0.3807, 0.005};
float sys_C[2] = {0,1};
float sys_x[2] = {0,0};
ss_t sys = {2,sys_A,sys_B,sys_C,sys_x,0};

float adquisicion_temp(){
  return sys.x[1];
}

float adquisicion_flujo(){
  return sys.x[0];
}

void actuacion(float vo){
  sys_run(&sys, vo);
}
