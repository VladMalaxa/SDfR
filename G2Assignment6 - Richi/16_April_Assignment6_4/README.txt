This project was created for the course Software Development for Robotics by:
Vlad Malaxa	-	s2726254
Ricardo Díaz	-	s3681548

Usage:

The following university provided folders and filesmust be present within the src folder of this project. 
Ideally the ones inside the provided file are kept to avoid any inconsistencies:

cam2image_vm2ros
cam2imageudptools
Image_functions
relbot_adapter
relbot_launch_files
relbot_msgs
relbot_simulator
relbot2turtlesim
RELbot-loopC
XRF2
changelog XRF2
RELbotSimple

- For assignment 6.2.2, open the README.txt present in the package assignment_6_2_2_ImagePositionDetector and follow 
the instructions present inside the README while using your laptop to run all steps, to ensure everything works as intended.

- To run assignment 6.3, do the following:

	-Every time you want to source, change directory to your root workspace
	    	cd <workspace name>

	-Then source by using the following commands when you open a new terminal:

	    	source /opt/ros/humble/setup.bash
		source install/setup.bash

	-When making changes to the sequence controller.cpp and before running, use the command:

		colcon build --packages-select assignment_6_2_2_ImagePositionDetector cam2image_vm2ros relbot_sequence_controller relbot_simulator relbot2turtlesim
	
	-Source again
	    	source /opt/ros/humble/setup.bash
		source install/setup.bash

	-First, open a new VS window and ensure it is disconnected from WSL.
	-Then run the following:

		python videoserver.py


	-Then open a new terminal in the original WSL terminal, source and use the following to run the code:

		ros2 launch relbot_sequence_controller relbot_follow_green_turtlesim.launch.py socket_ip:=172.23.128.1

- To run assignment 6.4, open the README.txt present in the package relbottest_6_4_2 and follow the instructions,
while connected to the RELbot (either via wifi or via Ethernet cable)

- To run assignment 6.5, open the README.txt present in the package relbot_sequence_controller and follow the instructions,
while connected to the RELbot (either via wifi or via Ethernet cable)