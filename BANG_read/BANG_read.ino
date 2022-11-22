/*
This file is to read the value stored in EEPROM
*/
#include <EEPROM.h>
struct SENSOR{
  unsigned long left;
  unsigned long centre;
  unsigned long right;
  float e_line;
};

struct PID{
  float e_l;
  float omegar;
  float omegal;
  float PID_r;
  float PID_l;
};

unsigned int address = 0;
void setup(){
  //IR LEDs were enabled

  Serial.begin(9600);
  Serial.println("****RESET***");

  PID object;
  while(address <= 65524){
    SENSOR get_sensor;
    EEPROM.get(address, get_sensor);
    address = address + sizeof(get_sensor);
    Serial.print(address);
    Serial.print(":");
    Serial.print(get_sensor.left);
    Serial.print(",");
    Serial.print(get_sensor.centre);
    Serial.print(",");
    Serial.print(get_sensor.right);
    Serial.print(",");
    Serial.println(get_sensor.e_line);
  }
}

int bytesToInt(byte* bytes,int size = 4) {
    int addr = bytes[0] & 0xFF;
    addr |= ((bytes[1] << 8) & 0xFF00);
    addr |= ((bytes[2] << 16) & 0xFF0000);
    addr |= ((bytes[3] << 24) & 0xFF000000);
    return addr;
}

void loop() {
  /*
    After storing the sensor value and e_line 
    the sensor value is left, centre, right
  */
  // SENSOR get_sensor;
  // EEPROM.get(address, get_sensor);
  // address = address + sizeof(get_sensor);

  // Serial.print(address);
  // Serial.print(":");
  // Serial.print(get_sensor.left);
  // Serial.print(",");
  // Serial.print(get_sensor.centre);
  // Serial.print(",");
  // Serial.print(get_sensor.right);
  // Serial.print(",");
  // Serial.println(get_sensor.e_line);

}
