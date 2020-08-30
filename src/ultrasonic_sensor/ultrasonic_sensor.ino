#include <HCSR04.h> 
#include <ros.h>
#include <std_msgs/Float32.h>

const int trigPin = 10;
const int echoPin = 9;

double distance;

std_msgs::Float32 radar_msg;
ros::Publisher distance_pub("/radar", &radar_msg);
ros::NodeHandle  nh;

void setup(){
  Serial.begin(57600);
  HCSR04.begin(trigPin, echoPin);
  
  nh.initNode();
  nh.advertise(distance_pub);
}

void loop(){
  double distance = hcsr04.measureDistanceCm();  
  radar_msg.data = distance;
  distance_pub.publish(&radar_msg);

  nh.spinOnce();
}
