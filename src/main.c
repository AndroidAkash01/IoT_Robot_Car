#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "Motor.h"
#include "Car.h"
#include "IR_Sensors.h"
#include "Servo.h"
#include <pins_arduino.h>
#include <stdio.h>

// #include <Arduino.h>
 



#define SCL_Pin PIN_A5  //Set the clock pin to A5
#define SDA_Pin A4  //Set data pin to A4

#define LEFT_MOTOR_DIR_PORT   PORTD
#define LEFT_MOTOR_DIR_PIN    PD2

#define RIGHT_MOTOR_DIR_PORT  PORTD
#define RIGHT_MOTOR_DIR_PIN   PD3


#define TXD   PD0
#define RXD   PD1


int THRESHOLD = 0;
 
 

int distance, distance_l, distance_r;


//For obstacle detection


char BLE_val;



int L_Initial_Value = 0;
int R_Initial_Value = 0;
int Initial_Difference = 0;


int speed_R = 100;
int speed_L = 100;



//#############################################################
int GradientRightEnd = 770;
int GradientMiddle   = 650;
int GradientLeftEnd  = 350;

int GradientOutsideTrack = 500;

//###############################################################

int carIsInTrack = 1;
int overtaking = 0;



 
void digitalWrite(uint8_t pin, uint8_t value);
void setup();
// void analogWriteRight(uint8_t value);
// void analogWriteLeft(uint8_t value);
void analogWrite(int pin, uint8_t value);
void tracking1();
void printGradient();
// void turnServo(int angle);
void detectLane();
void updateLaneOnObstacle(int obstacle_left ,int obstacle_front,int obstacle_right,int distance);
void switchLanes(char NewLane);
void keepOnTrack();
void scanForObstacle( int distance_limit);
void printNumber(char *str, int num); 
void move(int leftSpeed, int rightSpeed);
void MotorSpeed(int leftSpeed, int rightSpeed);
void printNumberSameLine(char *str, int num);
void KeepCarOnMyLane();
void stopCar();
void simpleGradientAlgorithm();
void simpleObstacleAvoidance();
void fastGradientAlgorithm();
void simpleGradientAlgorithm2();
// int get_distance();
int getCurrentOrientation();
void stopAfterLoops(int n);
void move_globally();


// static inline char uart_read_char(void) {
//     return UDR0;
// }

// static inline uint8_t uart_available(void) {
//     return (UCSR0A & (1 << RXC0)) != 0;
// }

void adc_init()
{
	ADMUX |= (1 << REFS0);   // Set reference voltage to AVCC
	// Set ADC prescaler to 128 (16MHz / 128 = 125kHz)
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	//ADCSRA |= (1 << ADIE); // Enable interrupt;
	ADCSRA |= (1 << ADEN);// | (1 << ADATE); // Enable ADC
	// DIDR0 |= (1 << ADC2D) | (1 << ADC1D) | (1 << ADC0D); // Disable digital buffer for ADC2
	DIDR0 |= (1 << ADC0D) | (1 << ADC3D) | (1 << ADC4D);

	// Start the first ADC conversion
	ADCSRA |= (1 << ADSC);
}

 

void setup() {
    // Initialize UART for debugging

    DDRD |= (1 << TXD);
    DDRD &= ~(1 << RXD);

      initUART();
      adc_init();
      init_IR_Sensors();
      initMotor();
      setupServo();
      uart_puts("Hello, World!!\n");
 
            // set_angle(90);

 
}

 
//function to read digital values
uint8_t digitalRead(uint8_t pin) {
   // Read the digital value from the pin
   return PIND & (1 << pin);
}

 
uint16_t analogRead(uint8_t pin) {
   // Read the analog value from the pin
   // Start the ADC conversion
   ADCSRA |= (1 << ADSC);
   // Wait for the conversion to complete
   while (ADCSRA & (1 << ADSC));
   // Return the ADC value
   return ADC;
}
  

int initial_L = 0;
int initial_R = 0;

int clockwise = 1;
int obstacleDistance = 100;
 
int main(void) {
 
   setup(); // Initialize the setup

   // Receive data from PC
  
   read_IR_SensorValues();
   L_Initial_Value = L_val;
   R_Initial_Value = R_val;
   Initial_Difference = R_Initial_Value - L_Initial_Value;
   detectLane();
   My_Lane = Lane;
   obstacle_front = get_distance();

   set_angle(90);

   car_Stop();
   clockwise = getCurrentOrientation();
 

// Lane = RIGHT;
initial_L = L_val;
initial_R = R_val;

while(1){

 


 

    while (uart_available()) {
        uint8_t c = uart_getc_blocking();
        if (c == '\a' || c == '\n') {
            stopCar();
            _delay_ms(2000);

        }
        else  {

        
              speed_R =   40;
              speed_L =   40;
              move_globally();
               

        }
    }

 
              // speed_R =   40;
              // speed_L =   40;
              // move_globally();
               




     

    read_IR_SensorValues();   


    // keepOnTrack();
 
    // uart_puts("Distance: ");
    // printNumber("Distance: ", Obstacle_Distance);
 
    // detectLane();

    // printGradient();  

    // simpleGradientAlgorithm();
 
 
    if(get_distance() > 30){    
        simpleGradientAlgorithm2();
    }else{
        stopCar();
    }

// print the distance to the obstacle
 printNumberSameLine("Distance : ", get_distance());


    // fastGradientAlgorithm();

    // simpleObstacleAvoidance();
 

    // scanForObstacle( 45);
    // printNumberSameLine("Obstacle Left: ",  obstacle_left);
    // printNumberSameLine(" Obstacle Front: ", obstacle_front);
    // printNumberSameLine(" Obstacle Right: ", obstacle_right);
    // uart_puts("\n");
 

}
 
	 
   return 0;  
}


int speed = 85;


void simpleObstacleAvoidance(){

     if(obstacle_front < 35 && obstacle_front > 2) {
        // Brake = 1;

        move(0,0);
        stopCar();
        overtaking = 1;

    }
    else{
      if(obstacle_front > 40){
           overtaking = 0;
      }
      else 
       if(Lane == LEFT && obstacle_right < 40){

            overtaking = 1;
            move(speed,0);

        }
        else if(Lane == RIGHT && obstacle_left < 40){

          overtaking = 1;
          move(0,speed);

        }

    }
 
}



void MotorSpeed(int leftSpeed, int rightSpeed) {
   PORTD |= (1 << left_ctrl);   
   PORTD |= (1 << right_ctrl);   
   LEFT_MOTOR_OCnx  = 155;
   RIGHT_MOTOR_OCnx = 155;
}


int getCurrentOrientation (){

    if(L_val > R_val){
       return 1; //Clockwise
    }else{
        return 0; //AntiClockwise
    }

}
 


void move(int leftSpeed, int rightSpeed) {

    if (leftSpeed >= 0) {
        PORTD |= (1 << left_ctrl);  // Set left motor to forward
    } else {
        PORTD &= ~(1 << left_ctrl); // Set left motor to reverse
    }

    if (rightSpeed >= 0) {
        PORTD |= (1 << right_ctrl);  // Set right motor to forward
    } else {
        PORTD &= ~(1 << right_ctrl); // Set right motor to reverse
    }

   
    OCR0A =    255 -   abs(leftSpeed);  // Adjusted Left motor PWM
    OCR0B =    255 -   abs(rightSpeed); // Adjusted Right motor PWM

 

}

void move_globally(){


  if(!overtaking){

    if (speed_L >= 0) {
        PORTD |= (1 << left_ctrl);  // Set left motor to forward
    } else {
        PORTD &= ~(1 << left_ctrl); // Set left motor to reverse
    }

    if (speed_R >= 0) {
        PORTD |= (1 << right_ctrl);  // Set right motor to forward
    } else {
        PORTD &= ~(1 << right_ctrl); // Set right motor to reverse
    }

   
    OCR0A =    255 -   abs(speed_L);  // Adjusted Left motor PWM
    OCR0B =    255 -   abs(speed_R); // Adjusted Right motor PWM

  }

 
}

 int loopCount = 0;


void simpleGradientAlgorithm2(){

        stopAfterLoops(2);  
         
        if(clockwise){

            if(L_val <= (initial_L - 22)) { 

            if(speed_L > 0){
              speed_L =  speed_L- 1;
            }else{
              speed_L = 0;
            }
              speed_R =   speed; 
            }  else
             if (R_val >= (initial_R + 22))
            {
              if(speed_R > 0){

              speed_R =   speed_R - 1;

              }else{
                  speed_R = 0;
                }
              speed_L =  speed;

            }else{
              speed_R =   speed;
              speed_L =   speed;
            }


        }else{
          //for anti clockwise
            if(R_val <= (initial_R - 6)) {
        if(speed_R > 0){
          speed_R =   speed_R - 6;
        }else{
          speed_R = 0;
        }
           speed_L =   speed;
       
        }  else if (L_val >= (initial_L + 6))
        {
          if(speed_L > 0){

          speed_L =   speed_L - 6;

          }else{
              speed_L = 0;
            }
          speed_R =  speed;
           

        }else{
          speed_R =   speed;
          speed_L =   speed;
         }
        }

        


           move_globally();

}
int overBlack = 0;

int L_overBlack = 0;
int R_overBlack = 0;

int L_overWhite = 0;
int R_overWhite = 0;

void stopAfterLoops( int n){

     if(loopCount >= n){
          stopCar();
          while(1);
        }

     
 
    if(L_val > 900){
      L_overBlack = 1;
    }

    if(R_val > 900){
      R_overBlack = 1;
    }
 
 
     if(L_overBlack == 1 && R_overBlack == 1){



        if(L_val < 700){
            L_overWhite = 1;
          }

          if(R_val < 700){
            R_overWhite = 1;
          }

          if(L_overWhite == 1 && R_overWhite == 1){
 
             _delay_ms(800);
 
            loopCount = loopCount + 1;
            L_overBlack = 0;
            R_overBlack = 0;
            L_overWhite = 0;
            R_overWhite = 0;

          }

     }
 
  

}

 
int turningAround = 0;

void simpleGradientAlgorithm() {

 if(turningAround){

    if(L_val < (R_val + 80)){
      turningAround = 0;
    }

        if(L_val <= Middle_Val) {
          speed_R =   speed - (L_val/2.5);
          speed_L =   speed;

        }else{
          speed_R =   speed;
          speed_L =   speed - (200);
        }
 }
 else{

 if(L_val <= GradientOutsideTrack && R_val <= GradientOutsideTrack) {
  // Reverse
          speed_L =  -180;
          speed_R =  -180;
   }else
      if(L_val > R_val) {

        // Turn complete around
        turningAround = 1;

      }else{
 
       if(L_val < (600) && L_val > (530)) {
            speed_R =  45;
        }
        else
        if(L_val < (530)){
            speed_R =   0;
        }else{
            speed_R =  speed;
        }

        if(R_val > (GradientRightEnd - 30) && R_val < (GradientRightEnd)) {
            speed_L =  45;
        }
        else
        if(R_val > (GradientRightEnd)) {
            speed_L =  0;
        }else{
            speed_L =  speed;
        }
  
      }

 }
 
   move_globally();

 
}


  
 
 


void stopCar() {
     move(0,0);
}

int switchingLane = 0;  //0 - No, 1 - Yes

void KeepCarOnMyLane(){

  //If my lane is not same as the lane, turn to the lane
  if(My_Lane != Lane) {

    switchingLane = 1;
   switchLanes(My_Lane);

  }else{
    //Keep the car on the lane

    //Keep the car on the My Lane (Between Middle and Right/Left edge)

if(Lane == LEFT) {

    if(L_val < GradientRightEnd) {
      speed_R = 0;
    } else   {
      speed_R = 100;
    }  

    if(R_val > GradientMiddle) {
      speed_L = 0;
    } else   {
      speed_L = 100;
    } 
  
}else{

    if(L_val < GradientMiddle) {
      speed_R = 0;
    } else   {
      speed_R = 100;
    }  

    if(R_val > GradientRightEnd) {
      speed_L = 0;
    } else   {
      speed_L = 100;
    }  

  }
 
 }

}



 

void printGradient(){

      char buffer[10];
      itoa(L_val, buffer, 10);
      uart_puts("L: ");
      uart_puts(buffer);
      //right sensor

      //print in red colour


    //print in red colour
       
      itoa(R_val, buffer, 10);
      uart_puts(" R: ");
      uart_puts(buffer);
      uart_puts("Initial R: ");
      itoa(R_Initial_Value, buffer, 10);
      uart_puts(buffer);
      uart_puts("Initial L: ");
      itoa(L_Initial_Value, buffer, 10);
      uart_puts(buffer);
 
      uart_puts("\n");




}


int ObstacleExist(int distance_limit) {
            if(get_distance() < distance_limit) {
            return 1;
            }
            else {
            return 0;
            }
}



void scanForObstacle( int distance_limit) {

  
      if (servoUpdateFlag == 1) {

        if(ServoDirection == 'R') {
          obstacle_left = get_distance();
          obstacle_right = 0;

        } else {
          obstacle_right = get_distance();
          obstacle_left = 0;
        }
            set_angle(90);   
            servoUpdateFlag = 0;
        } else if (servoUpdateFlag == 2) {
            obstacle_front = get_distance();
            set_angle((Lane == 'R') ? 180 : 0);
            servoUpdateFlag = 0;
        }

}

 
 
void tracking1() {
  int track_flag = 1;
  while (track_flag) {
 
    if (M_val > THRESHOLD) { //Black line detected in the middle
      if (L_val > THRESHOLD && R_val <= THRESHOLD) { //If a black line is detected on the left, but not on the right, turn left
        car_left();
      }
      else if (L_val <= THRESHOLD && R_val > THRESHOLD) { //Otherwise, if a black line is detected on the right and not on the left, turn right
        car_right();
      }
      else { //Otherwise, the car goes forward
        car_front();
      }
    }
    else { //no black lines detected in the middle
      if (L_val > THRESHOLD && R_val <= THRESHOLD) { //If a black line is detected on the left, but not on the right, turn left
        car_right();
      }
      else if (L_val <= THRESHOLD && R_val > THRESHOLD) { //Otherwise, if a black line is detected on the right and not on the left, turn right
        car_right();;
      }
      else { //Otherwise, stop
            // uart_puts(readPin(L_pin));

        car_Stop();
      }
    }
   //  BLE_val = Serial.read();
   //  if (BLE_val == 'S') { //When S is received, the car stops
   //    track_flag = 0;
   //    car_Stop();
   //  }
  }

 
}
 




 //function to print a number with a string
void printNumber(char *str, int num) {
   char buffer[10];
   itoa(num, buffer, 10);
   uart_puts(str);
   uart_puts(buffer);
   uart_puts("\n");
}

void printNumberSameLine(char *str, int num) {
   char buffer[10];
   itoa(num, buffer, 10);
   uart_puts(str);
   uart_puts(buffer);
}





void detectLane() {
        // servopulse(0);
      Middle_Val = (L_val + R_val) / 2;

      if(Middle_Val > GradientMiddle){
        Lane = RIGHT;
      }else{
        Lane = LEFT;
      }
}
 


// TO DO


void switchLanes(char NewLane) {
  if(NewLane == RIGHT ) {
     
    //drive to right 60 degrees, go forward till the L_val is greater than middle



   switchingLane = 0;

  }else{
     //drive to left 60 degrees, go forward till the R_val is lesser than middle
 


    switchingLane = 0;

  }

      //Check if its outside of track raise a flag
      // checkIfCarIsInTrack();


}




void checkIfCarIsInTrack() {
  if(L_val > GradientOutsideTrack && R_val > GradientOutsideTrack) {
    carIsInTrack = 1;
  }else{
    carIsInTrack = 0;
  }
}

