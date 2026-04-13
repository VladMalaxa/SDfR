from launch import LaunchDescription
from launch_ros.actions import Node
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    config = os.path.join(
        get_package_share_directory('cam2image_vm2ros'),
        'config',
        'cam2image.yaml'
    )

    cam_node = Node(
        package='cam2image_vm2ros',
        executable='cam2image',
        name='cam2image',
        parameters=[config],
        output='screen'
    )

    detector_node = Node(
        package='green_object_detector',
        executable='green_object_detector_node',
        name='green_object_detector_node',
        output='screen'
    )

    return LaunchDescription([
        cam_node,
        detector_node
    ])
