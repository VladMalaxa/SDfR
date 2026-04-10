This package detects green objects in camera images and outputs their position and was created by
Vlad Malaxa	-	s2726254
Ricardo Díaz	-	s3681548

Usage:

- Build the workspace with `colcon build`.
- Source the workspace: 
  source install/setup.bash
- Open a different VS Code window or terminal that can run your local webcam server. Open the folder containing `videoserver.py`.
- Run the video server:
  python videoserver.py

- Launch the test. If your professor's network uses a different reachable host IP for the video server, replace `172.23.128.1` with that address.
  ros2 launch assignment_6_2_2_ImagePositionDetector image_position_detector_test.launch.py socket_ip:=172.23.128.1

  Example if the video server runs on another address:
  ros2 launch assignment_6_2_2_ImagePositionDetector image_position_detector_test.launch.py socket_ip:=<VIDEO_SERVER_IP>

- To see processed image showing contour and center of green object and center of screen:
  open new terminal
  run: 
  ros2 run rqt_image_view rqt_image_view

Node info:
- Node name: image_position_detector_node
- Subscribed topic: /image (sensor_msgs/msg/Image)
- Output: Prints the centroid (x, y) of the largest green object detected in the image.

- Node name: turtlesim_controller_node
- Subscribed topic: /green_object_position (provided by image_position_detector_node)
- Output: Moves the turtlesim turtle to center the green object.
