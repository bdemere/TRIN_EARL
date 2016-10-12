Please read the style guide for ROS to allow for some coherence in our project

http://wiki.ros.org/CppStyleGuide

=============================
*** Pre-reqs for compiling *** 
==============================
-Install Eigen library 
	1. Install tar.bz2 from website and follow INSTALL instruction)
	2. If Cmake error, find FindEigen3.cmake file and put in it appropriate place, then rename it back to FindEigen.cmake
	* (Look inside the cmake folder somewhere within the compiled library folder and paste 
	   the cmake files into the catkin folder of the package)
-Install orocos-bfl (sudo apt-get install ros-jade-navigation) 
-Install dependencies for openslam-gmapping (sudo apt-get install ros-jade-openslam-gmapping)
-Install popt.h (sudo apt-get install libpopt-dev)
-Install xbox drivers
 1.sudo apt-add-repository -y ppa:rael-gc/ubuntu-xboxdrv
 2. sudo apt-get update
 3. sudo apt-get install ubuntu-xboxdrv
 4. sudo apt-get install --install-recommends jstest* joystick xboxdrv
-Two-webcam setup
* (I don't know how I got it to work, but here are some things I tried...)
 1. sudo apt-get install v4l-utils
 2. sudo rmmod uvcvideo
 3. sudo modprobe uvcvideo quirks=128
 4. rosdep install(?) usb_cam
 5. sudo apt-get install ros-jade-usb-cam
 6. Add serial numbers to the rules, etc
 
  --
 sudo apt-get install ros-indigo-rosserial-arduino
 sudo apt-get install ros-indigo-rosserial
-- 
Fovis_ros consists of fovis_ros and libfovis ROS package

--


Install libuvc here : https://int80k.com/libuvc/doc/
Install libgps = sudo apt-get install libgps-dev
---
Warnings
--
-Don't install ubuntu-xboxdrv (install just xboxdrv)
----
Optional
---
-sudo apt-get install ros-jade-imu-tools
 *. source devel/setup.bash
 1. git clone (git directory)
 2. rosdep imu_tools 

-in .bashrc, put:
	source /opt/ros/jade/setup.bash
	source /catkin_ws/install/setup.bash
-Run /catkin_ws/devel/setup.bash for good measure


------------------------------------
Shortcuts 
-----------------------------------

Ctrl + K  - Indents files automatically (if setup so)

-------------------
Teleoperation
-------------------

roslaunch turtlebot_teleop keyboard_teleop.launch --screen

rosrun turtlesim turtle_teleop_key /turtle1/cmd_vel:=/mybot/cmd_vel
rosrun turtlebot_teleop turtlebot_teleop_key /turtlebot_teleop/cmd_vel:=/mybot/cmd_vel

-----------------------------
GPS
-----------------------------
rosrun nmea_navsat_driver nmea_serial_driver _port=/dev/ttyUSB0 

------------------------
Running the robot
------------------------

Start
------------------
Open terminal 
cd ~/catkin_ws
source devel/setup.bash


Run if file change 
------------------
catkin_make --pkg rosserial_server 


Man_drive location: ~/catkin_ws/src/rosserial/rosserial_server/src/man_drive_update.cpp

Run commands 
------------
roslaunch rosserial_server man_drive.launch
sudo xboxdrv --silent 
-== sudo rmmod xpad
password: Trin_EARL

----------------
ROS Commands
----------------

rostopic echo /topic_name
rostopic pub my_topic std_msgs/String "hello there"

-----------------------------
Transforms
----------------------------
rosrun tf tf_echo turtle1 turtle2
rosrun tf view_frames
rosrun tf tf_monitor turtle1 turtle2
rosrun rqt_graph rqt_graph

---------------------
Connecting to the Cameras
----------------------

ssh -X ubuntu@Jetson1
ssh -X ubuntu@Jetson2

source devel/setup.bash

roslaunch image_transport_package steam.launch

------------------------
Launching Phidgets
----------------------

roslaunch phidgets_imu imu.launch

----------------------
Changes to fix after pulling from Github
----------------------

----------
Launch file 
----------
ns  => stands for namespace to launch the node in

===========
Other command line commands
===========
lsusb - lists usb connections 
(can be used to check if Kinect is properly powered and connected, etc.)

==========
GPS
=========
rosrun nmea_navsat_driver nmea_serial_driver _port=/dev/ttyUSB0 

===========
Camera calibration
===========

Refer: http://wiki.ros.org/camera_calibration

The camera calibration file is stored in /.ros/camera_info <=required for rectification
 

 rosrun camera_calibration cameracalibrat.py --size 7x6 --square 0.0121 right:=/stereo/right/image_raw left:=/stereo/left/image_raw right_camera:=/stereo/right left_camera:=/stereo/left --approximate=0.1


Size (8x6) refers to the corners where four corners meet
Square refers to the size of the squares in meters

** Test image calibration routinely using the checkerboard and looking at the disparity image 

===========
Image view
===========
rosrun image_view stereo_view stereo:=stereo image:=image_rect _queue_size:=1000 _approximate_sync:=True

TODOS
==========
https://www.youtube.com/watch?v=LzyMQVSJvzY
=Forbot?? point distance?? 
=Seam time
