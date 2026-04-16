from launch import LaunchDescription
from launch.actions import ExecuteProcess, TimerAction
from launch_ros.actions import Node


def generate_launch_description():
    # Step 1:
    # sudo ./build/project_ppd2/project_ppd2
    project_ppd2_process = ExecuteProcess(
        cmd=['sudo', './build/project_ppd2/project_ppd2'],
        output='screen'
    )

    # Step 2:
    # ros2 run ros_xeno_bridge RosXenoBridge
    ros_xeno_bridge_node = Node(
        package='ros_xeno_bridge',
        executable='RosXenoBridge',
        name='ros_xeno_bridge',
        output='screen'
    )

    # Step 3:
    # ros2 run relbot_adapter relbot_adapter --ros-args -p robotmode:=real
    relbot_adapter_node = Node(
        package='relbot_adapter',
        executable='relbot_adapter',
        name='relbot_adapter',
        parameters=[{'robotmode': 'real'}],
        output='screen'
    )

    # Step 4:
    # ros2 run cam2image_vm2ros cam2image --ros-args --params-file src/cam2image_vm2ros/config/cam2image_relbot.yaml
    cam2image_node = Node(
        package='cam2image_vm2ros',
        executable='cam2image',
        name='cam2image',
        parameters=['src/cam2image_vm2ros/config/cam2image_relbot.yaml'],
        output='screen'
    )

    # Step 5:
    # ros2 launch relbot_sequence_controller relbot_follow_green_real.launch.py
    # Assuming this launch internally starts the sequence controller and detector,
    # here we directly launch the main controller node instead.
    # If your relbot_follow_green_real.launch.py also launches other required nodes,
    # those should be added here too.
    controller_node = Node(
        package='relbot_sequence_controller',
        executable='relbot_sequence_controller',
        name='steer_relbot',
        output='screen'
    )

    # Step 6:
    # ros2 topic pub --once /XenoCmd std_msgs/msg/int32 "{data: 1}"
    xeno_cmd_pub = ExecuteProcess(
        cmd=[
            'ros2', 'topic', 'pub', '--once',
            '/XenoCmd',
            'std_msgs/msg/int32',
            '{data: 1}'
        ],
        output='screen'
    )

    return LaunchDescription([
        project_ppd2_process,

        TimerAction(
            period=2.0,
            actions=[ros_xeno_bridge_node]
        ),

        TimerAction(
            period=4.0,
            actions=[relbot_adapter_node]
        ),

        TimerAction(
            period=6.0,
            actions=[cam2image_node]
        ),

        TimerAction(
            period=8.0,
            actions=[controller_node]
        ),

        TimerAction(
            period=10.0,
            actions=[xeno_cmd_pub]
        ),
    ])