#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Float32.h"
#include <sstream>
#include <termios.h>
#include <sensor_msgs/Joy.h>
#include <ros/ros.h>
#include "rosserial_server/serial_session.h"

using namespace ros;
using namespace std;

#define _SWITCH        100 /*START AUTONOMOUS MODE*/

int speed = 0;

int main(int argc, char **argv){
  ros::init(argc, argv, "test_drive"); //Initialize our node
  std::string port; //variable for our arduino port --usually /dev/ttyACM0: to see list of available ports use terminal command ls /dev/tty*
  ros::param::param<std::string>("~port", port, "/dev/ttyACM0"); //setting our port
  int baud; //variable for our baud rate --we must set the baud rate so the arduino and the host do not lose sync
  ros::param::param<int>("~baud", baud, 9600); //setting our baud
  boost::asio::io_service io_service; //necessary junk
  new rosserial_server::SerialSession(io_service, port, baud); //initializing our serial communication
  boost::thread(boost::bind(&boost::asio::io_service::run, &io_service));  //beginning our serial thread

  NodeHandle n;//subscriber node handler
  NodeHandle out; //publisher node handler

  Publisher chatter_left = out.advertise<std_msgs::Int16>("Left", 1);  //publish our drive commands
  Publisher chatter_right = out.advertise<std_msgs::Int16>("Right", 1);  //publish our drive commands

  Rate loop_rate(8); //loop rate in Hz

  ros::Time previous_update = ros::Time::now();
  int leftSpeed = 0;
  int rightSpeed = 0;

  /*Run until program is manually stopped*/
  while (ros::ok()){

    if ((ros::Time::now() - previous_update).toSec() > 2) {
      if (leftSpeed > 80) {  
        leftSpeed = 0;
        rightSpeed = 0;
      }
      else {
        leftSpeed += 10;
        rightSpeed += 10; 
      }
      previous_update = ros::Time::now();
    }

    std_msgs::Int16 left, right; //sending an 8 bit integer message
    left.data = leftSpeed; //populating our message with our command
    right.data = rightSpeed; //populating our message with our command
    
    ROS_INFO("PUBLISH Left: %d | Right: %d", left.data, right.data);

    chatter_left.publish(left); //publish our message
    chatter_right.publish(right);
    ros::spinOnce(); //and when that loop is over we'll do it all over again!
    loop_rate.sleep();
  }

  return 0;
}
