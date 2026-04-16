from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    image_topic = LaunchConfiguration('image_topic')
    robotmode = LaunchConfiguration('robotmode')
    max_speed_mps = LaunchConfiguration('max_speed_mps')
    max_speed_rads = LaunchConfiguration('max_speed_rads')

    detector_node = Node(
        package='assignment_6_2_2_ImagePositionDetector',
        executable='image_position_detector_node',
        name='image_position_detector_node',
        remappings=[('/image', image_topic)],
        output='screen'
    )

    controller_node = Node(
        package='relbot_sequence_controller',
        executable='relbot_sequence_controller',
        name='steer_relbot',
        output='screen'
    )

    adapter_node = Node(
        package='relbot_adapter',
        executable='relbot_adapter',
        name='relbot_adapter',
        parameters=[
            {'robotmode': robotmode},
            {'use_twist_cmd': False},
            {'max_speed_mps': max_speed_mps},
            {'max_speed_rads': max_speed_rads}
        ],
        output='screen'
    )

    xeno_bridge_node = Node(
        package='ros_xeno_bridge',
        executable='RosXenoBridge',
        name='RosXenoBridge',
        output='screen'
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            'image_topic',
            default_value='/image',
            description='Image topic published by the real camera'
        ),
        DeclareLaunchArgument(
            'robotmode',
            default_value='real',
            description='Relbot adapter robot mode: real or sim'
        ),
        DeclareLaunchArgument(
            'max_speed_mps',
            default_value='0.25',
            description='Maximum wheel speed in meters per second'
        ),
        DeclareLaunchArgument(
            'max_speed_rads',
            default_value='5.0',
            description='Maximum wheel speed in radians per second'
        ),
        detector_node,
        controller_node,
        adapter_node,
        xeno_bridge_node
    ])
