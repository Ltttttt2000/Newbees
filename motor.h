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


    } 
    void setMotorDirection(char* L_DIR, char* R_DIR){
	    digitalWrite(L_DIR_PIN, L_DIR);
	    digitalWrite(R_DIR_PIN, R_DIR);
    }
    void setMotorSpeed(float L_PWM, float R_PWM){
	    analogWrite(L_PWM_PIN, L_PWM);
	    analogWrite(R_PWM_PIN, R_PWM);
    }


    setMotors(float left, float right){
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

    // Use this function to 
    // initialise the pins and 
    // state of your motor(s).
    void initialise() {
	    setMotorDirection(FWD, FWD);
	    setMotorSpeed(0, 0);
    }

    // functions

    void moving_forwards(){
	    setMotorDirection(FWD, FWD);
	    setMotorSpeed(20, 20);
    }
    void moving_backwards(){
	    setMotorDirection(REV, REV);
	    setMotorSpeed(20, 20);
    }
    void turn_left(){
      setMotorDirection(FWD, FWD);
      setMotorSpeed(0, 20);
    }
    void turn_right(){
      setMotorDirection(FWD, FWD);
      setMotorSpeed(20, 0);   
    }
    // 直角转弯
    void rotate_halfpi(){

    }
    void rotate_pi(){
      
    }
    void rorate_right(){
	    setMotorDirection(FWD, FWD);
	    setMotorSpeed(0, 20);
    }
    void rorate_left(){
	    setMotorDirection(FWD, FWD);
	    setMotorSpeed(20, 0);
    }

    void moving_arc(){
	    setMotorDirection(FWD, FWD);
	    setMotorSpeed(20, 15);
    }
    void rorating(){
	    setMotorDirection(FWD, REV);
	    setMotorSpeed(20, 20);
    }
    
};



#endif