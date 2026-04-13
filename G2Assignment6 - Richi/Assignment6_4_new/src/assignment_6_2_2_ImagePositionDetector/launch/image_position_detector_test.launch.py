from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import EnvironmentVariable, LaunchConfiguration
from launch_ros.actions import Node
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    config = os.path.join(
        get_package_share_directory('cam2image_vm2ros'),
        'config',
        'cam2image.yaml'
    )

    socket_ip = LaunchConfiguration('socket_ip')

    cam_node = Node(
        package='cam2image_vm2ros',
        executable='cam2image',
        name='cam2image',
        parameters=[config, {'socket_ip': socket_ip}],
        output='screen'
    )

    detector_node = Node(
        package='assignment_6_2_2_ImagePositionDetector',
        executable='image_position_detector_node',
        name='image_position_detector_node',
        output='screen'
    )

    turtlesim_node = Node(
        package='turtlesim',
        executable='turtlesim_node',
        name='turtlesim_node',
        output='screen'
    )

    controller_node = Node(
        package='assignment_6_2_2_ImagePositionDetector',
        executable='turtlesim_controller_node',
        name='turtlesim_controller_node',
        output='screen'
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            'socket_ip',
            default_value=EnvironmentVariable('CAM2IMAGE_SOCKET_IP', default_value='172.22.128.1'),
            description='Remote host IP for cam2image host2vm server'
        ),
        turtlesim_node,
        cam_node,
        detector_node,
        controller_node
    ])
