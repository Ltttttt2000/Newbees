// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _MOTORS_H
#define _MOTORS_H

#define FWD LOW
#define REV HIGH

// Class to operate the motor(s).
class Motors_c {
  public:
    int R_PWM_PIN = 9;      // LED connected to digital pin 9 Right motor PWM
    int L_PWM_PIN = 10;    // LED connected to digital pin 10 Left motor PWM
    int L_DIR_PIN = 16;   // left direction
    int R_DIR_PIN = 15;    // right direction

    // Constructor, must exist.
    Motors_c() {
      pinMode(L_PWM_PIN, OUTPUT);
      pinMode(L_DIR_PIN, OUTPUT);
      pinMode(R_PWM_PIN, OUTPUT);
      pinMode(R_DIR_PIN, OUTPUT);  
    } 

    void setMotorDirection(char* L_DIR, char* R_DIR){
	    digitalWrite(L_DIR_PIN, L_DIR);
	    digitalWrite(R_DIR_PIN, R_DIR);
    }
    void setMotorSpeed(float L_PWM, float R_PWM){
	    analogWrite(L_PWM_PIN, L_PWM);
	    analogWrite(R_PWM_PIN, R_PWM);
    }


    void setMotors(float left, float right){
      if(left<0 && right >=0){
        setMotorDirection(REV, FWD);
        setMotorSpeed(abs(left), right);
      }
      else if(left < 0 && right <0){
        setMotorDirection(REV, REV);
        setMotorSpeed(abs(left), abs(right));
      }
      else if(left >=0 && right < 0){
        setMotorDirection(FWD, REV);
        setMotorSpeed(abs(left), abs(right));        
      }
      else if(left >=0 && right >= 0){
        setMotorDirection(FWD, FWD);
        setMotorSpeed(abs(left), abs(right));        
      }
    }

    void stopMotors(){
      setMotors(0, 0);
    }

};



#endif