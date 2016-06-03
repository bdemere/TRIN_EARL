#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Float32.h"
#include <sstream>
#include <termios.h>
#include <ros/ros.h>
#include "rosserial_server/serial_session.h"

using namespace ros;
using namespace std;

// constants
float speed_min = 0.5; // 0.5 m/s is equivalent to ~1 mph
float speed_max = 1.79; // 1.79 m/s is equivalent to ~ 4 mph
int voltage_min = 80; // voltage at which the robot moves at ~1 mph
int voltage_max = 200; // voltage at which the robot moves at ~4 mph

// store speed in m/s 
float leftWheel;
float rightWheel;

/* Linear mapping from speed to voltage signal to be sent to motors */
int speed_to_voltage(float speed) {
  // if speed more than maximum speed
  if(speed > speed_max) {
    speed = speed_max;
  }

  // calculate and return voltage
  return int(voltage_min + ((speed/(speed_max - speed_min)) * (voltage_max - voltage_min)));
}

/* Callback to store left speed data to global variable */
void leftCB(const std_msgs::Float32& msg) {
  leftWheel = msg.data;
}

/* Call back to store right speed data to global variable */
void rightCB(const std_msgs::Float32& msg) {
  rightWheel = msg.data; 
}

int main(int argc, char **argv){
  ros::init(argc, argv, "auto_drive"); // initialize node
  
  std::string port; // variable for arduino port -- usually /dev/ttyACM0: to see list of available ports use terminal command ls /dev/tty*
  ros::param::param<std::string>("~port", port, "/dev/ttyACM0"); // setting arduino port

  int baud; // variable for baud rate --we must set the baud rate so the arduino and the host do not lose sync
  ros::param::param<int>("~baud", baud, 9600); // setting baud rate

  boost::asio::io_service io_service; // necessary junk
  new rosserial_server::SerialSession(io_service, port, baud); // initializing serial communication
  boost::thread(boost::bind(&boost::asio::io_service::run, &io_service)); // beginning serial thread

  // setup publishers
  NodeHandle out; // node handler  
  Publisher chatter_left = out.advertise<std_msgs::Int16>("Left", 1);  // publish our drive commands
  Publisher chatter_right = out.advertise<std_msgs::Int16>("Right", 1);  // publish our drive commands

  // subscribe to speed (in m/s) topic from twist_to_motors differetial drive node
  ros::Subscriber<std_msgs::Float32> subL("lwheel_vtarget", &leftCB );
  ros::Subscriber<std_msgs::Float32> subR("rwheel_vtarget", &rightCB );

  Rate loop_rate(8); // loop rate in Hz

  // run until program is manually stopped
  while (ros::ok()){
    std_msgs::Int16 left, right; // voltage is an 8 bit integer message
    
    // convert speed data to voltage and populate message
    left.data = speed_to_voltage(leftWheel);
    right.data = speed_to_voltage(rightWheel);
    
    if(leftWheel != 0) // output command to a terminal window if there is anything to show
      ROS_INFO("PUBLISH Left: %d | Right: %d", left.data, right.data);

    // publish messages
    chatter_left.publish(left);
    chatter_right.publish(right);

    ros::spinOnce(); //and when that loop is over we'll do it all over again!
    loop_rate.sleep();
  }
  return 0;
}
