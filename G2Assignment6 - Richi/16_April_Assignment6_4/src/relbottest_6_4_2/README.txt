This package provides wheel setpoints to the loopcontroller of the RELbot with testing in mind and was created by:
Vlad Malaxa	-	s2726254
Ricardo Díaz	-	s3681548

The node publishes setpoint velocities on:
/input/left_motor/setpoint_vel
/input/right_motor/setpoint_vel

Usage:

It is intended to run together with the provided loop controller and the relbot_adapter package.

>>Following the steps bellow will ensure the code is running and the tests are carried out:

-Start by connecting to host via ssh -X <group_name>@<relbot_IP>
-Build relbot_msgs & xrf2_msgs first, to avoid build errors:
    cd ~/<workspace_root>
    colcon build --packages-select relbot_msgs xrf2_msgs

-Source and build the rest of the project:
    source /opt/ros/humble/setup.bash
    source install/setup.bash
    colcon build

-Open 5 new cmd terminals and connect each of them to ssh via: 
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
ros2 launch relbottest_6_4_2 relbottest_6_4_2.launch.py

-Terminal 5:
cd ~/<workspace_root>
source install/setup.bash
ros2 topic pub --once /XenoCmd std_msgs/msg/Int32 "{data: 1}"

Parameters

- sequence_enabled (bool, default: true)
- setpoint_vel (double, default: 2.0)
- phase_duration (double, default: 4.0)
- publish_rate_hz (double, default: 10.0)
- test_mode (string, default: straight)

When sequence_enabled is true, the node goes through each of these steps:
1. left_only
2. right_only
3. straight
4. turn_cw
5. turn_ccw
6. stop

When sequence_enabled is false, the node publishes the mode selected by test_mode continuously.