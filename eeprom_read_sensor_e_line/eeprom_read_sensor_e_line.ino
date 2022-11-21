#include<EEPROM.h>
#include "motor.h"
#include "linesensor.h"

#define black 900

int address = 0;
Motors_c  motors;
LineSensor_c sensors;
float e_line;
// determine a proportional rotation power. scale a maximum PWM value by the e_line signal.
float turn_pwm = 150;  // the sensible max pwm for rotation

bool lineDetected(float sensor_value){
  if(sensor_value > black && sensor_value <2000){
    return true;
  }
  else{
    return false;
  }
}


void setup() {
  sensors.enableIRLEDs();
}

void loop() {
  sensors.doParallelSensorRead();
  float left_sensor = sensors.sensor_read[0];
  float centre_sensor = sensors.sensor_read[1];
  float right_sensor = sensors.sensor_read[2];
  // Serial.println(sensors.sensor_read[0]);
  float e_line = sensors.getLineError();
  Serial.println(e_line); //原本就是-0.08

  struct SENSOR{
    unsigned long left;
    unsigned long centre;
    unsigned long right;
    float e_line;
  };

  SENSOR sensor_value = {left_sensor, centre_sensor, right_sensor, e_line};

  address = address + sizeof(sensor_value);
  EEPROM.put(address, sensor_value);

  float speed = turn_pwm * (e_line);
  float straight_value = 0.1;
    if(abs(e_line) > straight_value){
      motors.setMotors(-speed, speed);
      Serial.print(speed);
      Serial.println(-speed);
    }
    else {//move forward
      Serial.println("Move forward");
      motors.setMotors(20, 20);
    }




}
