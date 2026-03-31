from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    detector_node = Node(
        package='green_object_detector',
        executable='green_object_detector_node',
        name='green_object_detector_node',
        remappings=[
            ('/image', '/moving_camera_output')  
        ],
        output='screen'
    )

    return LaunchDescription([
        detector_node
    ])
