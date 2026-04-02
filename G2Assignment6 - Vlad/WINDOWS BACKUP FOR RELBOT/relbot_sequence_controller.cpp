//==============================================================
// Filename    : relbot_sequence_controller.cpp
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of the sequence controller function used for the control of
// of the RelBot. Inside the SteerRelbot::calculate_velocity() function, the finite state machine for the turtle movements 
// is defined, with each value of the int "selection" (line 38) being tied to a specific trajectory.
//==============================================================

#include "steering.hpp"
#include <geometry_msgs/msg/point.hpp>

SteerRelbot::SteerRelbot() : Node("steer_relbot") {
    RCLCPP_INFO(this->get_logger(), "Init");

    create_topics();

    // Subscribe to vision system
    light_sub_ = this->create_subscription<geometry_msgs::msg::Point>(
        "/green_object/position", 10,
        std::bind(&SteerRelbot::light_callback, this, std::placeholders::_1)
    );

    timer_ = this->create_wall_timer(
        std::chrono::duration<double>(1/DEFAULT_SETPOINT_STREAM),
        std::bind(&SteerRelbot::timer_callback, this)
    );
}

void SteerRelbot::create_topics() {
    left_wheel_topic_ = this->create_publisher<example_interfaces::msg::Float64>(
        "/input/left_motor/setpoint_vel", 1);

    right_wheel_topic_ = this->create_publisher<example_interfaces::msg::Float64>(
        "/input/right_motor/setpoint_vel", 1);
}

// Store object position
void SteerRelbot::light_callback(const geometry_msgs::msg::Point::SharedPtr msg) {
    light_pos = *msg;
}

void SteerRelbot::calculate_velocity() {

    // Controller parameters (tune if needed)
    double k_linear = 5.0;
    double k_angular = 8.0;

    // Error (already normalized [-1,1])
    double error_x = -light_pos.y;   // forward/backward
    double error_theta = -light_pos.x; // left/right

    // Control law (first-order)
    double v = k_linear * error_x;
    double omega = k_angular * error_theta;

    // Convert to wheel velocities
    left_velocity  = v - omega;
    right_velocity = v + omega;

    // Safety clamp (very important)
    double max_vel = 10.0;
    left_velocity = std::max(std::min(left_velocity, max_vel), -max_vel);
    right_velocity = std::max(std::min(right_velocity, max_vel), -max_vel);
}

void SteerRelbot::timer_callback() {
    calculate_velocity();

    example_interfaces::msg::Float64 left_wheel;
    left_wheel.data = left_velocity;

    example_interfaces::msg::Float64 right_wheel;
    right_wheel.data = right_velocity;

    left_wheel_topic_->publish(left_wheel);
    right_wheel_topic_->publish(right_wheel);
}

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SteerRelbot>());
    rclcpp::shutdown();
    return 0;
}