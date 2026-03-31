This folder contains the files relevant to Assignment 5 of Software Development for Robotics and was created by group 'PPD 2',
This group is composed by:

Vlad Malaxa	-	s2726254
Ricardo Díaz	-	s3681548


Following the steps bellow will ensure the code is running and displaying the desired turtle movements. 

Change directory to your root workspace
cd <workspace name>

Then source by using the following commands when you open a new terminal:

source /opt/ros/humble/setup.bash
source install/setup.bash

When making changes to the sequence controller.cpp and before running, use the command:

colcon build

Lastly use the following to run the code:

ros2 launch relbot_launch relbot_sequence_controller.launch.py


In line 38, changing the "selection" int changes the desired trajectory used in the sequence controller:
selection = 1 : Straight line
selection = 2 : Circle
selection = 3 : Straight line followed by sharp 90 degree turn
selection = 4 : Square
any other value: Error message