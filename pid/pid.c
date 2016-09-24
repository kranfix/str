#include "pid.h"

void pid_set(pid_handler_t * p, float kp, float ki, float kd, float T){
  p->kp = kp + kd / T;
  p->ki = ki * T;
  p->kd = - kd / T;
  p->T = T;
  p->u = 0;
  p->ui = 0;
  p->u_max = 24;
  p->u_min = -p->u_max;
  p->e_last = 0;
}

void pid_run(pid_handler_t * p, float error){
  p->ui += p->ki * error;
  p->u = p->kp * error + p->ui + p->kd* p->e_last;

  if(p->u > p->u_max){
    p->ui = p->ui + p->u_max - p->u;
    p->u = p->u_max;
  } else if(p->u < p->u_min){
    p->ui = p->ui + p->u_min - p->u;
    p->u = p->u_min;
  }
}
