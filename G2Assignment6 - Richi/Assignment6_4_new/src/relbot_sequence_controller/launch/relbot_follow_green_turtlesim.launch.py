from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    relbot_simulator = Node(
        package='relbot_simulator',
        executable='relbot_simulator',
        name='relbot_simulator',
        output='screen'
    )

    detector_node = Node(
        package='assignment_6_2_2_ImagePositionDetector',
        executable='image_position_detector_node',
        name='image_position_detector_node',
        remappings=[('/image', '/output/moving_camera')],
        output='screen'
    )

    controller_node = Node(
        package='relbot_sequence_controller',
        executable='relbot_sequence_controller',
        name='steer_relbot',
        output='screen'
    )

    turtlesim_node = Node(
        package='turtlesim',
        executable='turtlesim_node',
        name='turtlesim',
        output='screen'
    )

    relbot2turtlesim_node = Node(
        package='relbot2turtlesim',
        executable='relbot2turtlesim',
        name='relbot2turtlesim',
        output='screen'
    )

    return LaunchDescription([
        relbot_simulator,
        detector_node,
        controller_node,
        turtlesim_node,
        relbot2turtlesim_node,
    ])
