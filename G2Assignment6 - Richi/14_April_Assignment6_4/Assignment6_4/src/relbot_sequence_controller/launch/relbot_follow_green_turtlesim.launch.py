from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import EnvironmentVariable, LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    socket_ip = LaunchConfiguration('socket_ip')

    config = os.path.join(
        get_package_share_directory('cam2image_vm2ros'),
        'config',
        'cam2image.yaml'
    )

    cam_node = Node(
        package='cam2image_vm2ros',
        executable='cam2image',
        name='cam2image',
        parameters=[config, {'socket_ip': socket_ip}],
        output='screen'
    )

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
        DeclareLaunchArgument(
            'socket_ip',
            default_value=EnvironmentVariable('CAM2IMAGE_SOCKET_IP', default_value='172.23.128.1'),
            description='Remote host IP for cam2image host-to-VM server'
        ),
        cam_node,
        relbot_simulator,
        detector_node,
        controller_node,
        turtlesim_node,
        relbot2turtlesim_node,
    ])
