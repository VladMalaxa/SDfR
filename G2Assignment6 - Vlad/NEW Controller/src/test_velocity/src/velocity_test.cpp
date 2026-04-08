//==============================================================
// Filename    : velocity_test.cpp
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of the test code for the velocity controller of the RelBot. 
// The test sends velocity command value 1.0 to both left and right motors for 2 seconds.
//==============================================================

#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/float64.hpp"
#include <chrono>
#include <thread>

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    auto node = rclcpp::create_node("velocity_test");

    RCLCPP_INFO(node->get_logger(), "Starting velocity test");

    // Create publishers for left and right motors
    auto left_motor_pub = node->create_publisher<example_interfaces::msg::Float64>(
        "/input/left_motor/setpoint_vel", 1);
    
    auto right_motor_pub = node->create_publisher<example_interfaces::msg::Float64>(
        "/input/right_motor/setpoint_vel", 1);

    // Create velocity message with value 1.0
    auto velocity_msg = std::make_shared<example_interfaces::msg::Float64>();
    velocity_msg->data = 1.0;

    RCLCPP_INFO(node->get_logger(), "Publishing velocity 1.0 to both motors for 2 seconds");

    // Publish to both motors
    left_motor_pub->publish(*velocity_msg);
    right_motor_pub->publish(*velocity_msg);

    // Wait for 2 seconds
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Stop motors by sending 0
    velocity_msg->data = 0.0;
    RCLCPP_INFO(node->get_logger(), "Stopping motors");
    left_motor_pub->publish(*velocity_msg);
    right_motor_pub->publish(*velocity_msg);

    rclcpp::shutdown();
    return 0;
}
