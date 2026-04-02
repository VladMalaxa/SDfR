import os
import time
import pytest
import launch
import launch_ros.actions
import launch_testing

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image

from ament_index_python.packages import get_package_share_directory


@pytest.mark.launch_test
def generate_test_description():
    # ✅ Correct way to locate config file
    config_file = os.path.join(
        get_package_share_directory('cam2image_vm2ros'),
        'config',
        'cam2image.yaml'
    )

    # ✅ Camera node
    cam_node = launch_ros.actions.Node(
        package='cam2image_vm2ros',
        executable='cam2image',
        name='cam2image',
        parameters=[config_file],
        output='screen'
    )

    # ✅ Auto shutdown after 5 seconds (prevents hanging tests)
    shutdown_timer = launch.actions.TimerAction(
        period=5.0,
        actions=[launch.actions.EmitEvent(event=launch.events.Shutdown())]
    )

    return (
        launch.LaunchDescription([
            cam_node,
            launch_testing.actions.ReadyToTest(),
            shutdown_timer
        ]),
        {
            'cam_node': cam_node
        }
    )


# ✅ Test that node starts correctly
def test_node_starts(cam_node, proc_info):
    proc_info.assertWaitForStartup(process=cam_node, timeout=5)


# ✅ Test that images are actually published
def test_image_published():
    rclpy.init()
    node = Node('test_node')

    received_messages = []

    def callback(msg):
        received_messages.append(msg)

    # Subscribe to image topic
    subscription = node.create_subscription(
        Image,
        '/image',   # ⚠️ Change if your topic name is different
        callback,
        10
    )

    # Spin for a few seconds to collect messages
    start_time = time.time()
    while time.time() - start_time < 4:
        rclpy.spin_once(node, timeout_sec=0.1)

    node.destroy_node()
    rclpy.shutdown()

    # ✅ Actual assertion
    assert len(received_messages) > 0, "No images received on /image topic"


# ✅ Verify clean shutdown
@launch_testing.post_shutdown_test()
class TestShutdown:

    def test_exit_code(self, proc_info, cam_node):
        proc_info.assertWaitForShutdown(process=cam_node, timeout=5)