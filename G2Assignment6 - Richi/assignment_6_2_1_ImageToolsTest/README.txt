This package was created to show the testing of image_tools as specified by assignment 6.2.1, 
by running the standard showimage node of the standard image_tools ROS2 package. Created by:

Vlad Malaxa	-	s2726254
Ricardo Díaz	-	s3681548

Usage:

- Build the workspace with `colcon build`.
- Source the workspace: `source install/setup.bash`.
- Open a different VS Code window that is not connected to WSL, open the folder containing videoserver.py.
- run the folllowing:
python videoserver.py

- Launch the test. If your network uses a different reachable host IP for the video server, replace `172.23.128.1` with that address.
    ros2 launch assignment_6_2_1_ImageToolsTest cam2image_test.launch.py socket_ip:=172.23.128.1

- To check what your required IP is, open a new command terminal (on windows, powershell) and run:
    ipconfig
- Check for the IPv4 address on your WSL.

- Example if the video server runs on another address:
    ros2 launch assignment_6_2_1_ImageToolsTest cam2image_test.launch.py socket_ip:=<VIDEO_SERVER_IP>



