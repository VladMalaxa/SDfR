This package was created to show the testing of image_tools as specified by assignment 6.2.1, 
by running the standard showimage node of the standard image_tools ROS2 package. Created by:

Vlad Malaxa	-	s2726254
Ricardo Díaz	-	s3681548

Usage:

- Build the workspace with `colcon build`.
- Source the workspace: `source install/setup.bash`.
-Open a different VS Code window that is not connected to WSL, open the folder containing videoserver.py.
-run the folllowing:
python videoserver.py

- Launch the test:
ros2 launch assignment_6_2_1_ImageToolsTest cam2image_test.launch.py


