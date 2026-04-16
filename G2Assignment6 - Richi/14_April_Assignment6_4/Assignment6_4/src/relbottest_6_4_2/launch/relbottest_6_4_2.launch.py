from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='relbottest_6_4_2',
            executable='relbottest_6_4_2_node',
            name='relbottest_6_4_2',
            output='screen',
            parameters=[
                {
                    'sequence_enabled': True,
                    'setpoint_vel': 2.0,
                    'phase_duration': 4.0,
                    'publish_rate_hz': 10.0,
                    'test_mode': 'straight'
                }
            ]
        )
    ])
