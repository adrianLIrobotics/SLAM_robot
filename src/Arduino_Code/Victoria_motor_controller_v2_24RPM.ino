//-------------------------SPECIFICATIONS------------------//
#ifndef ROBOT_SPECS_H
#define ROBOT_SPECS_H
#define wheel_diameter  0.0765   //m
#define wheel_width     0.01   //m
#define track_width     2.75   //m
#define pi              3.1415926
#define two_pi          6.2831853
#endif
//----------------------------ROS---------------------------//
//ROS headers
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
#include <ros.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <geometry_msgs/Twist.h>
#include <ros/time.h>
//------------------------MOTOR SHIELD---------------------//
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "Adafruit_PWMServoDriver.h"
double forward = 0;
double backward = 0;
double Stop = 0;

#define encoder0PinA  18
#define encoder0PinB  17
#define encoder1PinA  19
#define encoder1PinB  16
#define LOOPTIME        100   // PID loop time(ms)

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Select which 'port' M1, M2, M3 or M4. 
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
//-----------------------VARIABLES---------------------//
unsigned long lastMilli = 0;       // loop timing 
unsigned long lastMilliPrint = 0;
unsigned long lastMilliPub = 0;
volatile long count1 = 0;  
int rpm_req1 = 0;
int rpm_req2 = 0;
int rpm_act1 = 0;
int rpm_act2 = 0;
int direction1 = FORWARD;
int direction2 = FORWARD;
int PWM_val1 = 0;                  // (25% = 64; 50% = 127; 75% = 191; 100% = 255)
int PWM_val2 = 0;
boolean goingUp = false;
boolean goingDown = false;
//float Kp =   1;                    // PID proportional control Gain
//float Kd =   0.5; 
float   Kp = 1;
float   Kd = 0.7;

//--------------------------ROS NODE-------------------------//
ros::NodeHandle nh;

void handle_cmd( const geometry_msgs::Twist& cmd_msg) {
  float x = cmd_msg.linear.x;
  float z = cmd_msg.angular.z;
  //oooooooooooooooooooooooooooooooooooooooooooooooooooo//
  if (z == 0) {     // go straight
    if (x>0){
       rpm_req1 = 34;
       rpm_req2 = rpm_req1;
      }
     else if (x<0){
      rpm_req1 = -34;
      rpm_req2 = rpm_req1;
      }
     else if (x==0)
      rpm_req1 = 0;
      rpm_req2 = rpm_req1;  
  }

  else if ((z == 0) && (x==0)) {
      rpm_req1 = 0;
      rpm_req2 = 0;  
    }
  //oooooooooooooooooooooooooooooooooooooooooooooooooooo//
  else if (x == 0) {
    // convert rad/s to rpm
    if (z>0){
      rpm_req2 = 34;
      rpm_req1 = -34; 
      }
    else if (z<0){
      rpm_req2 = -34;
      rpm_req1 = 34; 
      }
  }
  //oooooooooooooooooooooooooooooooooooooooooooooooooooo//
   if (rpm_req1 >0) direction1 = BACKWARD;
   else if (rpm_req1 <0) direction1 = FORWARD;
   else if (rpm_req1 == 0) direction1 = RELEASE;

   if (rpm_req2 >0) direction2 = FORWARD;
   else if (rpm_req2 <0) direction2 = BACKWARD;
   else if (rpm_req2 == 0) direction2 = RELEASE;

   motor1->setSpeed(80);  
   motor2->setSpeed(86);
 
}
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", handle_cmd);
geometry_msgs::Vector3Stamped rpm_msg;
ros::Publisher rpm_pub("rpm", &rpm_msg);
ros::Time current_time;
ros::Time last_time;
//--------------------------VOID SETUP()---------------------//
void setup() {
AFMS.begin();  // create with the default frequency 1.6KHz
 nh.initNode();
 nh.getHardware()->setBaud(57600);
 nh.subscribe(sub);
 nh.advertise(rpm_pub);
}
 //----------------------VOID LOOP()-------------------------//
 void loop() {
 nh.spinOnce();
 unsigned long time = millis();

 motor1->run(direction1);// send PWM to motor
 motor2->run(direction2);
 
 if(time-lastMilliPub >= LOOPTIME) {
    publishRPM(time-lastMilliPub);
    lastMilliPub = time;
  }
 }
 //---------------------PUBLISH RPM---------------------------//
void publishRPM(unsigned long time) {
  rpm_msg.header.stamp = nh.now();
  rpm_msg.vector.x = rpm_req1;
  rpm_msg.vector.y = rpm_req2;
  rpm_msg.vector.z = double(time)/1000;
  rpm_pub.publish(&rpm_msg);
  nh.spinOnce();
}
