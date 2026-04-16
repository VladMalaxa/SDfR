from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    turtlesim_node = Node(
        package='turtlesim',
        executable='turtlesim_node',
        name='turtlesim',
        output='screen'
    )

    relbot_controller = Node(
        package='relbot_sequence_controller',
        executable='relbot_sequence_controller',
        name='steer_relbot',
        output='screen'
    )

    return LaunchDescription([
        turtlesim_node,
        relbot_controller
    ])