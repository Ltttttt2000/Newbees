#ifndef _LINESENSOR_H
#define _LINESENSOR_H

#define LS_LEFT_PIN A0
#define LS_CENTRE_PIN A2
#define LS_RIGHT_PIN A3
#define EMIT 11
#define NB_LS_PINS 3
#define BEEP 6


class LineSensor_c{
   public:

    int sensor_output[NB_LS_PINS]; // the original sensor value
    float cal_sensor[NB_LS_PINS];  // the calbration sensor value
    unsigned long sensor_read[NB_LS_PINS];  // the sensor value to detect line
    float output[NB_LS_PINS];

    int timeout = 5000;
    int which;
    
   
    float scalar[3]; //Currently calibration only works for the front three
    float offset[3];
    int n = 100; //Number of datapoints for calibration

    int left_white[100];
    int centre_white[100];
    int right_white[100];
    int left_black[100];
    int centre_black[100];
    int right_black[100];

    unsigned long end_time[3];
    int ls_pin[NB_LS_PINS] = {LS_LEFT_PIN, LS_CENTRE_PIN, LS_RIGHT_PIN};

    LineSensor_c() {
      pinMode(EMIT,OUTPUT);
      digitalWrite(EMIT, HIGH);
    }

    void beep(int pitch) {
      for (int i = 0; i < 100; i++) {
        digitalWrite(BEEP, HIGH);
        delayMicroseconds(pitch);
        digitalWrite(BEEP, LOW);
        delayMicroseconds(pitch);
      }
    }
      
    float average(int myarray[100]) {//返回平均值
      int total = 0;
      for (int i = 0; i < (sizeof(myarray) / sizeof(myarray[0])); i++) {
        total += myarray[i];
      }
      return total/(sizeof(myarray) / sizeof(myarray[0]));
    }
        

    int minarray(int myarray[100]) {
      int min = myarray[0];
        for (int i = 0; i < (sizeof(myarray) / sizeof(myarray[0])); i++) {
          if (myarray[i] < min) {
            min = myarray[i];
          }
        }
      return min;   // 返回最小值
    }
    void make_calibration(){
      calibrate_white();
      calibrate_black();
    }
    void calibrate_white(){

      for (int i = 0; i < n; i++) {
        calibration();
        left_white[i] = sensor_output[0];
        centre_white[i] = sensor_output[1];
        right_white[i] = sensor_output[2];
      }

      beep(1000);
      delay(500);
    
      offset[0] = minarray(left_white);
      offset[1] = minarray(centre_white);
      offset[2] = minarray(right_white);
      // 三个传感器在白线上不同的值
      Serial.println(offset[0]);   // 532
      Serial.println(offset[1]);  // 468
      Serial.println(offset[2]);  // 700
    }

    void calibrate_black(){
      for (int i = 0; i < n; i++) {
        calibration();
        left_black[i] = sensor_output[0];
        centre_black[i] = sensor_output[1];
        right_black[i] = sensor_output[2];
      }
      //Derive the scaling factor from the reading range (difference between min and max readings)
      scalar[0] = 1.0000/(average(left_black)-average(left_white));
      scalar[1] = 1.0000/(average(centre_black)-average(centre_white));
      scalar[2] = 1.0000/(average(right_black)-average(right_white));
      Serial.println(average(left_black));  // 1368
      Serial.println(average(centre_black));  
      Serial.println(average(right_black));
      Serial.println(scalar[0]);
      Serial.println(scalar[1]);
      Serial.println(scalar[2]);      
      beep(1000);
    }

    void readsensors() { 
      bool done = false;
      unsigned long start_time = micros();

      for (which = 0; which < NB_LS_PINS; which++) {
        if (ls_pin[which] != 0) {
          pinMode(ls_pin[which], OUTPUT);
          digitalWrite(ls_pin[which], HIGH);
          pinMode(ls_pin[which], INPUT);
          end_time[which] = start_time;
        }
      }

      delayMicroseconds(10);

      while (done == false) {
        for (which = 0; which < NB_LS_PINS; which++) {
          if (digitalRead(ls_pin[which]) == 0 && end_time[which] == start_time) {
            end_time[which] = micros();
          }
        }

        if (end_time[0] != start_time && end_time[1] != start_time && end_time[2] != start_time) {
          done=true;
        }

        if ((micros()-start_time) >= timeout) {
          done=true;
        }
      }           

      output[0] = (end_time[0]-start_time - offset[0])*scalar[0];
      output[1] = (end_time[1]-start_time - offset[1])*scalar[1];
      output[2] = (end_time[2]-start_time - offset[2])*scalar[2];
          
               
    }
 
    void calibration() { 
      bool done = false;
      unsigned long start_time = micros();

      for (which = 0; which < NB_LS_PINS; which++) {
        if (ls_pin[which] != 0) {
          pinMode(ls_pin[which], OUTPUT);
          digitalWrite(ls_pin[which], HIGH);
          pinMode(ls_pin[which], INPUT);
          end_time[which] = start_time;
        }
      }

      delayMicroseconds(10);

      while (done == false) {
        for (which = 0; which < NB_LS_PINS; which++) {
          if (digitalRead(ls_pin[which]) == 0 && end_time[which] == start_time) {
            end_time[which] = micros();
          }
        }

        if (end_time[0] != start_time && end_time[1] != start_time && end_time[2] != start_time) {
          done=true;
        }

        if ((micros()-start_time) >= timeout) {
          done=true;
        }
      }

      sensor_output[0] = (end_time[0]-start_time);
      sensor_output[1] = (end_time[1]-start_time);
      sensor_output[2] = (end_time[2]-start_time);


      cal_sensor[0] = (sensor_output[0] - offset[0])*scalar[0];
      cal_sensor[1] = (sensor_output[1] - offset[1])*scalar[1];
      cal_sensor[2] = (sensor_output[2] - offset[2])*scalar[2];
           
    }
        

    //eline() returns a weighted line value for the front three sensors, this is useful for line following
    float findeline() {

      calibration();
      float total_activation = cal_sensor[0] + cal_sensor[1] + cal_sensor[2]; //Total of three forward sensor reading
      float w_L = (cal_sensor[0] + (cal_sensor[1]*0.5)) / total_activation; //Weight left side sensors
      float w_R = (cal_sensor[2] + (cal_sensor[1]*0.5)) / total_activation; //Weight right side sensors
      float eline = w_R - w_L; 
      return eline;
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

      Serial.print("line sensor: " );
      Serial.print( sensor_read[0]);
      Serial.print(",");
      Serial.print(sensor_read[1]);
      Serial.print(",");
      Serial.println(sensor_read[2]);
      delay(10);
    }

 };

 #endif
