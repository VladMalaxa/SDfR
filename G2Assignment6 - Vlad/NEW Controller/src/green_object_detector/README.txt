This package detects green objects in camera images and outputs their position. Created by:
Vlad Malaxa	-	s2726254
Ricardo Díaz	-	s3681548

Usage:

- Build the workspace with `colcon build`.
- Source the workspace: `source install/setup.bash`.
-Open a different VS Code window that is not connected to WSL, open the folder containing videoserver.py.
-run the folllowing:
python videoserver.py

- Launch the test:
  ros2 launch green_object_detector green_object_detector_test.launch.py

Node info:
- Node name: green_object_detector_node
- Subscribed topic: /image (sensor_msgs/msg/Image)
- Output: Prints the centroid (x, y) of the largest green object detected in the image.
