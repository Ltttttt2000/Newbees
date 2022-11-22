/*
This file is to read the value stored in EEPROM
*/
#include <EEPROM.h>

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

  delay(10000);
  Serial.println("Start print");

  PID object;
  while(address <= 65524){
    EEPROM.get(address, object);
    address = address + sizeof(object);
    Serial.print(address);
    Serial.print(":");
    Serial.println(object.e_l);
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

  // PID object;
  // EEPROM.get(address, object);
  // address = address + sizeof(object);
  // Serial.print(address);
  // Serial.print(":");
  // Serial.println(object.e_l);
  // Serial.print(",");
  // Serial.print(object.omegar);
  // Serial.print(",");
  // Serial.print(object.omegal);
  // Serial.print(",");
  // Serial.print(object.PID_r);
  // Serial.print(",");
  // Serial.println(object.PID_l);


}
