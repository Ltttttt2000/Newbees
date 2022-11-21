// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _LINESENSOR_H
#define _LINESENSOR_H

#define LS_LEFT_PIN A0
#define LS_CENTRE_PIN A2
#define LS_RIGHT_PIN A3
#define EMIT 11
#define NB_LS_PINS 3

// Class to operate the linesensor(s).
class LineSensor_c {
  public:
    int ls_pin[NB_LS_PINS] = {LS_LEFT_PIN, LS_CENTRE_PIN, LS_RIGHT_PIN};
    unsigned long sensor_read[NB_LS_PINS]; 
   
  
    // Constructor, must exist.
    LineSensor_c() {
      

    } 
    void enableIRLEDs(){
      pinMode(EMIT, OUTPUT);
      digitalWrite(EMIT, HIGH);
    }
    void disableIRLEDs(){
      pinMode(EMIT, OUTPUT);
      digitalWrite(EMIT, LOW);
    }

    float getLineError(){
      double e_line;
      float w_left;
      float w_right;

      // get sensor reading, may call an updateLineSensor() function from loop before hand
      float sum_gs;
      float gs_value[3];
      // float left = sensor_read[0] - 120;
      // float centre = sensor_read[1];
      // float right = sensor_read[2] - 236;
      float left = sensor_read[0];
      float centre = sensor_read[1];
      float right = sensor_read[2];
      sum_gs = left + centre + right;
      gs_value[0] = left / sum_gs;
      gs_value[1] = centre / sum_gs;
      gs_value[2] = right / sum_gs;
      // sum ground sensor activation

      // normalize individual sensor readings against sum
      // calculated error signal
      w_left = gs_value[0] + (gs_value[1]*0.5);
      w_right = gs_value[2] + (gs_value[1]*0.5);
      e_line = w_left - w_right;
      // bigger value means more black.
      // Serial.println(e_line);

      return e_line;
    }
    void chargeCapacitors(){
      // Charge capacitor by setting input pin
      // temporarily to output and HIGH
      pinMode(LS_LEFT_PIN,OUTPUT);
      pinMode( LS_RIGHT_PIN, OUTPUT );
      pinMode(LS_CENTRE_PIN,OUTPUT);
  
      digitalWrite( LS_LEFT_PIN, HIGH );
      digitalWrite( LS_RIGHT_PIN, HIGH );
      digitalWrite( LS_CENTRE_PIN, HIGH );
  

      // Tiny delay for capacitor to charge.
      delayMicroseconds(30);

      //  Turn input pin back to an input
      pinMode( LS_LEFT_PIN, INPUT );
      pinMode( LS_RIGHT_PIN, INPUT );
      pinMode( LS_CENTRE_PIN, INPUT );
    }
    void readLineSensor(){
      //Test single sensor.
      //Stay in a loop whilst the capacitor
      //is still registering as "HIGH".
      bool done = false;
      unsigned long timeout = 5000;
      unsigned long sensor_time;
      unsigned long start_time; 
  
      // pinMode(LS_LEFT_PIN,OUTPUT);
  
      // digitalWrite( LS_LEFT_PIN, HIGH );
      // pinMode(LS_LEFT_PIN,INPUT);
      chargeCapacitors();
      start_time = micros();
  
      while(done!=true){
        unsigned long current_time = micros();
          // Calculate elapsed time
        unsigned long elapsed_time; // t_elapsed
    
        elapsed_time = current_time - start_time;

        if(elapsed_time >= timeout){
          done = true;
          sensor_time = timeout;
        }
        if(digitalRead( LS_LEFT_PIN ) == LOW){
          sensor_time = elapsed_time;
          done = true;
        }
      }
      //Serial.println(start_time);
      // Serial.println(sensor_time);
    }


    void doParallelSensorRead(){
      int which;   // index through each sensor.
      // Places to store microsecond count
      unsigned long start_time;
      unsigned long timeout = 5000;
      // multiple sensors need multiple places to store the measurement result.
      // check how many sensors have finished reading. count down to zero.
      int remaining = NB_LS_PINS;
      unsigned long elapsed_time; // t_elapsed
      // unsigned long sensor_read[NB_LS_PINS]; 
      // must set default separately!!
      sensor_read[0] = 0;
      sensor_read[1] = 0;
      sensor_read[2] = 0;
    
      chargeCapacitors();
      start_time = micros();
      bool done = false;

      while(remaining>0){
        unsigned long current_time = micros();
          // Calculate elapsed time
        unsigned long elapsed_time; // t_elapsed

        elapsed_time = current_time - start_time;

        for(which = 0; which<NB_LS_PINS; which++){
          // the sensor has completed.
          if(digitalRead( ls_pin[which] ) == LOW){
            if(sensor_read[which] == 0){
              sensor_read[which] = elapsed_time;
              remaining = remaining - 1;
            }
          }     
        } 
        // Here, we make a final check to see if the 
        // whole read operation is taking too long.
        if(elapsed_time >= timeout){
          sensor_read[0] = timeout;
          sensor_read[1] = timeout;
          sensor_read[2] = timeout;
          remaining = remaining - 1;
        }
      }

      // Serial.print("line sensor: " );
      // Serial.print( sensor_read[0]);
      // Serial.print(",");
      // Serial.print(sensor_read[1]);
      // Serial.print(",");
      // Serial.println(sensor_read[2]);
      // delay(10);
    }


};



#endif
