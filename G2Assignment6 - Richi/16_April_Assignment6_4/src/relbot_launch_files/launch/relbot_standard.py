from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess, TimerAction
from launch.substitutions import EnvironmentVariable, LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    

    ros_xeno_bridge_node = Node(
        package='ros_xeno_bridge',
        executable='RosXenoBridge',
        name='ros_xeno_bridge',
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
        ros_xeno_bridge_node,
        relbot_adapter_node,
    ])
