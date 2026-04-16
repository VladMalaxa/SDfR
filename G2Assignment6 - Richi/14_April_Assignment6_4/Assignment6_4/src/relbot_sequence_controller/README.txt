This folder contains the files relevant to 6.4 of Software Development for Robotics and was created by group 'PPD 2',
This group is composed by:

Vlad Malaxa 	-	s2726254
Ricardo Díaz	-	s3681548


>>Following the steps bellow will ensure the code is running and the relbot moves correctly:

-Start by connecting to host via ssh -X <group_name>@<relbot_IP>
-Build relbot_msgs & xrf2_msgs first, to avoid build errors:
    cd ~/<workspace_root>
    colcon build --packages-select relbot_msgs xrf2_msgs

-Source and build the rest of the project:
    source /opt/ros/humble/setup.bash
    source install/setup.bash
    colcon build

-Open 6 new cmd terminals and connect each of them to ssh via: 
    wsl ssh -X <group_name>@<relbot_IP>

-On each of the terminals, run the following. Some of these commands are time sensitive,
so running them should be done at a somewhat short time interval.

-Terminal 1:
cd ~/<workspace_root>
source install/setup.bash
sudo ./build/project_ppd2/project_ppd2

-Terminal 2: 
cd ~/<workspace_root>
source install/setup.bash
ros2 run ros_xeno_bridge RosXenoBridge

-Terminal 3:
cd ~/<workspace_root>
source install/setup.bash
ros2 run relbot_adapter relbot_adapter --ros-args -p robotmode:=real

-Terminal 4: 
cd ~/<workspace_root>
source install/setup.bash
ros2 run cam2image_vm2ros cam2image --ros-args --params-file src/cam2image_vm2ros/config/cam2image_relbot.yaml

-Terminal 5:
cd ~/<workspace_root>
source install/setup.bash
ros2 launch relbot_sequence_controller relbot_follow_green_real.launch.py

-Terminal 6:
cd ~/<workspace_root>
source install/setup.bash
ros2 topic pub --once /XenoCmd std_msgs/msg/Int32 "{data: 1}"