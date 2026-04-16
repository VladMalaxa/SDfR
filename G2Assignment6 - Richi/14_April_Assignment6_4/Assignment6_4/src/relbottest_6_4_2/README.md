# relbottest_6_4_2

A ROS 2 package to generate basic wheel setpoints for the RELbot loop controller.

This package provides a test node that publishes setpoint velocities on:
- `/input/left_motor/setpoint_vel`
- `/input/right_motor/setpoint_vel`

It is intended to run together with the provided loop controller and/or the `relbot_adapter` package.

## Build

```bash
colcon build --packages-select relbottest_6_4_2
```

## Run

Directly:

```bash
ros2 run relbottest_6_4_2 relbottest_6_4_2_node
```

Using the launch file:

```bash
ros2 launch relbottest_6_4_2 relbottest_6_4_2.launch.py
```

## Parameters

- `sequence_enabled` (bool, default: true)
- `setpoint_vel` (double, default: 2.0)
- `phase_duration` (double, default: 4.0)
- `publish_rate_hz` (double, default: 10.0)
- `test_mode` (string, default: `straight`)

When `sequence_enabled` is true, the node steps through:
1. left_only
2. right_only
3. straight
4. turn_cw
5. turn_ccw
6. stop

When `sequence_enabled` is false, the node publishes the mode selected by `test_mode` continuously.
