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
        turtlesim_node,
        cam_node,
        detector_node,
        controller_node
    ])