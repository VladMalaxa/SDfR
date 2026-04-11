from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():

    # Get the path to cam2image.yaml file inside the package
    config = os.path.join(
        get_package_share_directory('cam2image_vm2ros'),
        'config',
        'cam2image.yaml'
    )

    socket_ip = LaunchConfiguration('socket_ip')

    # Node 1: Camera node (publishes images to a topic)
    cam_node = Node(
        package='cam2image_vm2ros',
        executable='cam2image',
        name='cam2image',
        parameters=[config, {'socket_ip': socket_ip}],
        output='screen'
    )

    # Node 2: Image viewer node (subscribes to image topic)
    show_node = Node(
        package='image_tools',
        executable='showimage',
        name='showimage_node',
        output='screen'
    )

    # Launch both nodes, first the camera node and then the image viewer node
    return LaunchDescription([
        DeclareLaunchArgument(
            'socket_ip',
            default_value='172.22.128.1',
            description='Remote host IP for cam2image host2vm server'
        ),
        cam_node,
        show_node
    ])