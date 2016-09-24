#ifndef PID_H
#define PID_H

typedef struct pid_handler_t {
  float kp;
  float kd;
  float ki;
  float T;
  float ui;
  float u;
  float u_max;
  float u_min;
  float e_last;
} pid_handler_t;

void pid_set(pid_handler_t * p, float kp, float ki, float kd, float T);
void pid_run(pid_handler_t * p,float error);

#endif // PID_H
