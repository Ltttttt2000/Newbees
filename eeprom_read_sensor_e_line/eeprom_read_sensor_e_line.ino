#include "motor.h"
#include "linesensor.h"

#define black 900


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

  // all sensors in the white floor. just move forward.
  if(!lineDetected(left_sensor) && !lineDetected(centre_sensor) && !lineDetected(right_sensor)){
    Serial.println(" moving forward to find the line!");
    motors.setMotors(20,20);
  }

  // if any of the sensors detect the sensors.
  if(lineDetected(left_sensor) || lineDetected(centre_sensor) || lineDetected(right_sensor)){
    // delay(20);
    // turn pwm is scaled by e_line [-1.0, 1.0], e_line>0 turn left, e_line<0 turn right
    float speed = turn_pwm * (e_line);
    // Serial.println(speed);
    float straight_value = 0.1;
    // set the motor speed

    // if (e_line > straight_value) {//turn right
    //   motors.setMotors(0,speed);
    //   Serial.println("Turn right");
    // }
    
    // else if (e_line < -straight_value) {//turn left
    //   motors.setMotors(-speed,0);      
    //   Serial.println("Turn left");
    // }
    if(abs(e_line) > straight_value){
      motors.setMotors(-speed, speed);
      Serial.print(speed);
      Serial.println(-speed);
    }
    else {//move forward
      Serial.println("Move forward");
      motors.setMotors(20, 20);
    }
    
    delay(50);

  }



}
