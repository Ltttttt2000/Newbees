#include<EEPROM.h>
#include "motor.h"
#include "linesensor.h"

#define black 900

// Push buttons connected to:
#define BUTTON_A_PIN  14
#define BUTTON_B_PIN  30

int address = 0;
// float f = 123.456f;  //Variable to store in EEPROM.

Motors_c  motors;
LineSensor_c sensors;
float e_line;
// determine a proportional rotation power. scale a maximum PWM value by the e_line signal.
float turn_pwm = 100;  // the sensible max pwm for rotation
unsigned long start_time = millis();
unsigned long current_time = millis();

// We will imagine this is data
// collected by the robot.
//创建结构体
struct SENSOR{
  float left_sensor;
  float centre_sensor;
  float right_sensor;
  float e_line;
};


void resetAll(unsigned int eepromLength, byte value = 255){
  for(int address = 0; address < eepromLength; address++){
    EEPROM.write(address, value);
  }
}


void setup() {
  pinMode( BUTTON_A_PIN, INPUT_PULLUP );
  pinMode( BUTTON_B_PIN, INPUT_PULLUP );
  sensors.enableIRLEDs();
  resetAll(EEPROM.length(), 0);
  Serial.begin(9600);
  delay(1000);
  Serial.println("***RESET***");
  // Setup our grid in volatile memory
  // as if it had been data collected
  // by the robot.
  // Wait for a button press to decide 
  // what to do.
  int a;
  int b;
  a = HIGH;
  b = HIGH;
  do {
    Serial.println("A = write, B = read");
    a = digitalRead( BUTTON_A_PIN);
    b = digitalRead( BUTTON_B_PIN);
  } while( a == HIGH && b == HIGH ); 
  // Saves our volatile grid into EEPROM.写入
  if( a == LOW ) {
    Serial.println("Writing new values to EEPROM");
    writeGridToEEPROM();
    while(1) {
      Serial.println("Write done, please reset");
      delay(1000);
    }
  } else {  // Recovers data from EEPROM.
    Serial.println("Reading old values from EEPROM");
    
  }

}

// writes a 2D grid into the 1D 
// EEPROM.  Note, readFromEEPROM
// uses a consistent method to 
// get the data back out.
void writeGridToEEPROM() {
  do{
  sensors.doParallelSensorRead();
  float left_sensor = sensors.sensor_read[0];
  float centre_sensor = sensors.sensor_read[1];
  float right_sensor = sensors.sensor_read[2];
  // Serial.println(sensors.sensor_read[0]);
  float e_line = sensors.getLineError();
  SENSOR sensor_value = {left_sensor, centre_sensor, right_sensor, e_line};
  Serial.println(sensor_value.left_sensor);
  Serial.println(sensor_value.centre_sensor);
  Serial.println(sensor_value.right_sensor);
  Serial.println(sensor_value.e_line);
  if(address>=EEPROM.length()){
    address = 0;
  }
  EEPROM.put(address, sensor_value);
  address = address + sizeof(sensor_value);
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
  }while(1);
}

// Serial prints the contents of 
// EEPROM
void readFromEEPROM() {
  int address=0;
    // Newline after print a row
    do{
    // byte value = EEPROM.read( address );
    SENSOR get_sensor;
    EEPROM.get(address, get_sensor);
    Serial.println( "Read custom object from EEPROM: " );
    Serial.print(address);
    Serial.print(":");
    Serial.println(get_sensor.left_sensor);
    Serial.println(get_sensor.centre_sensor);
    Serial.println(get_sensor.right_sensor);
    Serial.println(get_sensor.e_line);
    address = address + sizeof(get_sensor);
  }while(address <= 65524);
//  
    Serial.print("\n");
  }
 
void loop() {
  readFromEEPROM();
  delay(1000);
}