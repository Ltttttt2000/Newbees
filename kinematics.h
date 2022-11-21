#ifndef _KINEMATICS_H
#define _KINEMATICS_H

// Class to track robot position.
class Kinematics_c {
  public:
    float theta = 0;
    float y = 0;
    float x = 0;

    float distance;
    float radius = 1;
    
    float phi;                 
    float right_distance;             
    float left_distance;            
    float right_radians;               
    float left_radians;                

    long oldcounte0 = 0;
    long oldcounte1 = 0;
    int spin_error = 1;  
    int wheel_r = 16;  //mm      
    int l = 43;   // mm        
    float radpc = (2*PI)/358.3;  

    Kinematics_c() {
    } 

    void update() {      
      right_radians = (count_e0-oldcounte0)*radpc;                   
      left_radians = (count_e1-oldcounte1)*radpc;
      // update new count value
      oldcounte0 = count_e0;
      oldcounte1 = count_e1; 
         
      right_distance = right_radians*wheel_r;
      left_distance = left_radians*wheel_r;

      if (right_distance > -left_distance - spin_error || right_distance < -left_distance + spin_error) {
        //Spinning on the spot (with a sesnitivty of +- spin_error mm
        //i.e. it is said to be spinning if left_distance is the same as -right_distance +- the spin sensitivity
        //This is to avoid the program assuming it is moving in an arc when it is actually spinning on the spot
        basic_move(); 
        
      } else if (abs(right_distance) > abs(left_distance)) {
        //Turning in an arc to the right
        radius = ((2*l*left_distance) / abs(right_distance-left_distance)) + l;
        phi = (right_distance - left_distance)/(2*l); 
        arc_move();
                
      } else if (abs(right_distance) < abs(left_distance)) {
        //Turning in an arc to the left
        radius = ((2*l*right_distance) / abs(left_distance-right_distance)) + l;
        phi = (left_distance - right_distance)/(2*l); 
        arc_move();
               
      } else if (right_distance == left_distance) {
        //Straight forward or straight back
        basic_move();
      }
    }

    void arc_move() {
      y = y + radius*(-sin(theta) + sin(phi)*cos(theta) + sin(theta)*cos(phi));
      x = x + radius*(cos(theta) - cos(phi)*cos(theta) + sin(theta)*sin(phi)); 
      theta = theta + phi;
    }

    void basic_move() {
      distance = (right_distance + left_distance)/2;                  
      phi = (left_distance - right_distance)/(2*l);     

      y = y + distance*cos(theta);
      x = x + distance*sin(theta); 
      theta = theta + phi;
    }
    

};



#endif
