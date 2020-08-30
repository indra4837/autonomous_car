#include <L298NX2.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>

ros::NodeHandle  nh;

// left: motorA, right: motorB
L298NX2 motors(D10, D9, D8, D3, D4, D5);
int linearSpeed;
int angularSpeed;

void moveStraight(int linearSpeed) {
  motors.setSpeedA(linearSpeed);
  motors.setSpeedB(linearSpeed);
  motors.forward();
}

void turnLeft(int linearSpeed, int angularSpeed) {
  motors.setSpeedA(linearSpeed + angularSpeed);
  motors.setSpeedB(linearSpeed);
  motors.forward();
}

void turnRight(int linearSpeed, int angularSpeed) {
  motors.setSpeedA(linearSpeed);
  motors.setSpeedB(linearSpeed + angularSpeed);
  motors.forward();
}

void motorCb(const geometry_msgs::Twist& cmd){

  if (cmd.angular.z == 0) {
    if (cmd.linear.x == 0) {
      linearSpeed = 0;
      motors.stop();
    } else {
      linearSpeed = (cmd.linear.x > 255) ? 255 : cmd.linear.x;
      moveStraight(linearSpeed);
    }
  } else if (cmd.angular.z != 0) {
      angularSpeed = cmd.angular.z;
      linearSpeed = cmd.linear.x;
      if (angularSpeed > 0) turnRight(linearSpeed, angularSpeed);
      else turnLeft(linearSpeed, angularSpeed);     
  }
}

ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", motorCb);

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{
  nh.spinOnce();
  delay(1);
}
