//Include relevent class files
#include<EEPROM.h>
#include "motors.h"
#include "linesensor.h"
#include "encoders.h"
#include "kinematics.h"
#include "pid.h"

//Setup relevent classes
Motors_c motors;
PID_c pidl;
PID_c pidr;
PID_c pidtheta;
PID_c pidline;
Kinematics_c kinematics;
LineSensor_c sensors;

int address = 0;

//Time parameters
unsigned long current_time = millis();
unsigned long oldtime = 0;

//Kinematics parameters
long countnow_e0;
long countnow_e1;
long countold_e0 = 0;
long countold_e1 = 0;
float phir;                //radians right wheel turned per timestep
float phil;                //radians left wheel turned per timestep
float omegar;              //radians per second right wheel turned per timestep
float omegal;              //radians per second left wheel turned per timestep

//Wheel parameters
int wheel_r = 16;      //mm wheel radius 
int l = 43;                //mm distance between wheel centrelines
float cpr = 358.3;         //counts per rev from pololu documentation
float radpc = (2 * PI) / cpr; //radians per count


float PID_r;
float PID_l;
float PID_theta;
float PID_line;

float P_motor = 0.3;
float I_motor = 0.001;
float D_motor = 0;  

int state = 1;



void setup() {
  Serial.begin(9600);

  setupEncoder0();
  setupEncoder1();
  
  pidtheta.reset();
  pidr.reset();
  pidl.reset();

  sensors.make_calibration();
}

void loop() {
  float e_l = sensors.findeline(); //weighted error from left to right side
  updateomega();
  PID_line = pidline.update_pid(0, e_l, 30, I_motor, D_motor);
  PID_r = pidr.update_pid(4 + PID_line*4, omegar, P_motor, I_motor, D_motor);
  PID_l = pidl.update_pid(4 - PID_line*4, omegal, P_motor, I_motor, D_motor);
  motors.setMotors(PID_l, PID_r);

  // Record e_line, omegar, omegal,PID_r, PID_l in EEPROM.???
  struct SENSOR{
    float e_l;
    float omegar;
    float omegal;
    float PID_r;
    float PID_l;
  };

  SENSOR object = {e_l, omegar, omegal, PID_r, PID_l};

  address = address + sizeof(object);
  EEPROM.put(address, object);

}

void updateomega() {
  countnow_e0 = count_e0;
  countnow_e1 = count_e1;
  unsigned long timenow = millis();

  phir = (countnow_e0 - countold_e0) * radpc;
  phil = (countnow_e1 - countold_e1) * radpc;
  omegar = (phir / (timenow - oldtime)) * 1000;
  omegal = (phil / (timenow - oldtime)) * 1000;

  countold_e0 = countnow_e0;
  countold_e1 = countnow_e1;
  oldtime = timenow;
}

