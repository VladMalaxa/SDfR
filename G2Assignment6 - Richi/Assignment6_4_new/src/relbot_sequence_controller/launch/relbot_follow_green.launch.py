from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
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

    relbot_adapter_node = Node(
        package='relbot_adapter',
        executable='relbot_adapter',
        name='relbot_adapter',
        parameters=[{'robotmode': 'real'}],
        output='screen'
    )

    return LaunchDescription([
        detector_node,
        controller_node,
        relbot_adapter_node
    ])
