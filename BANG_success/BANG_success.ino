#include<EEPROM.h>
#include "motor.h"
#include "linesensor.h"

#define black 900
// Change these two values!!
float turn_pwm = 90;  // the sensible max pwm for rotation
float init_speed = 23;


Motors_c  motors;
LineSensor_c sensors;
float e_line;
// determine a proportional rotation power. scale a maximum PWM value by the e_line signal.

unsigned long start_time = millis();
unsigned long current_time = millis();

struct SENSOR{
  unsigned long left;
  unsigned long centre;
  unsigned long right;
  float e_line;
  // unsigned long time;
};

bool lineDetected(float sensor_value){
  if(sensor_value > black && sensor_value <2000){
    return true;
  }
  else{
    return false;
  }
}

void resetAll(unsigned int eepromLength, byte value = 255){
  for(int address = 0; address < eepromLength; address++){
    EEPROM.write(address, 0);
    // Serial.print("Address ");
    // Serial.print(address);
    // Serial.print(": ");
    // Serial.println(data);
  }
}

void to_bytes(uint32_t val, uint8_t *bytes) {
  bytes[0] = (uint8_t) val;
  bytes[1] = (uint8_t) (val >> 8);
  bytes[2] = (uint8_t) (val >> 16);
  bytes[3] = (uint8_t) (val >> 24);
}
void setup() {
  sensors.enableIRLEDs();
  resetAll(EEPROM.length(), 0);
}

void loop() {
  sensors.doParallelSensorRead();
  float left_sensor = sensors.sensor_read[0];
  float centre_sensor = sensors.sensor_read[1];
  float right_sensor = sensors.sensor_read[2];
  // Serial.println(sensors.sensor_read[0]);
  float e_line = sensors.getLineError();
  // Serial.println(e_line); //原本就是-0.08
  

  float speed = turn_pwm * (e_line);
  float straight_value = 0.1;
    if(abs(e_line) > straight_value){
      motors.setMotors(-speed, speed);
    }
    else {//move forward
      // Serial.println("Move forward");
      motors.setMotors(init_speed, init_speed);
    }




}
