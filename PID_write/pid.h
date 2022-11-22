#ifndef _PID_H
#define _PID_H

class PID_c {
  public:
    float P;
    float I;
    float D;

    unsigned long oldtime = 0;
    unsigned long newtime;
    unsigned long dt;
    float etdt;
    float error_old = 0;

    PID_c() {
    } 


    float update_pid(float demand, float measurement, float P_value, float I_value, float D_value) {
      float error = demand - measurement;
      float ediff;
      newtime = millis();            
      dt = newtime - oldtime;
      oldtime = newtime;
      
      etdt = etdt + error*dt;
      ediff = error - error_old;
      error_old = error;
      
      P = P_value * error;
      I = I_value * etdt;

      if (dt != 0) {
        D = D_value*(ediff/dt);
      }
      else {
        D = 0;
      }

      float pid = P + I + D;

      return pid;          
    }

    void reset() { 
      etdt = 0;
      oldtime = millis();
    }      
};



#endif
