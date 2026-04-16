//==============================================================
// Filename    : relbot_sequence_controller.cpp
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of the sequence controller function used for the control of
// the RelBot. It receives the green object position from the vision system, computes
// a first-order setpoint control action, and publishes differential wheel velocities to
// the simulator or real robot.
//==============================================================

#include "steering.hpp"
#include <geometry_msgs/msg/point.hpp>
#include <cmath>
#include <algorithm>

SteerRelbot::SteerRelbot() : Node("steer_relbot") {
    RCLCPP_INFO(this->get_logger(), "Init");

    declare_parameter<std::string>("green_object_topic", "/green_object_position");
    declare_parameter<double>("image_width", 160.0);
    declare_parameter<double>("image_height", 160.0);
    declare_parameter<double>("k_linear", 0.6);
    declare_parameter<double>("k_angular", 0.004);
    declare_parameter<double>("max_angular", 0.8);
    declare_parameter<double>("max_wheel_vel", 20.0);
    declare_parameter<double>("small_area_threshold", 8000.0);
    declare_parameter<double>("close_area_threshold", 22000.0);
    declare_parameter<double>("wheel_base_width", 0.20);
    declare_parameter<double>("wheel_radius", 0.05);
    declare_parameter<double>("deadzone_pixels", 25.0);
    declare_parameter<double>("setpoint_stream", 30.0);

    // New parameters for search / rotation mode
    declare_parameter<double>("target_timeout", 0.5);
    declare_parameter<double>("search_left_wheel_vel", 3.0);
    declare_parameter<double>("search_right_wheel_vel", -2.0);

    get_parameter("green_object_topic", green_object_topic_);
    get_parameter("image_width", image_width_);
    get_parameter("image_height", image_height_);
    get_parameter("k_linear", k_linear_);
    get_parameter("k_angular", k_angular_);
    get_parameter("max_angular", max_angular_);
    get_parameter("max_wheel_vel", max_wheel_vel_);
    get_parameter("small_area_threshold", small_area_threshold_);
    get_parameter("close_area_threshold", close_area_threshold_);
    get_parameter("wheel_base_width", wheel_base_width_);
    get_parameter("wheel_radius", wheel_radius_);
    get_parameter("deadzone_pixels", deadzone_pixels_);
    get_parameter("setpoint_stream", setpoint_stream_);

    // New parameter reads
    get_parameter("target_timeout", target_timeout_);
    get_parameter("search_left_wheel_vel", search_left_wheel_vel_);
    get_parameter("search_right_wheel_vel", search_right_wheel_vel_);

    create_topics();

    light_sub_ = this->create_subscription<geometry_msgs::msg::Point>(
        green_object_topic_, 10,
        std::bind(&SteerRelbot::light_callback, this, std::placeholders::_1)
    );

    timer_ = this->create_wall_timer(
        std::chrono::duration<double>(1.0 / setpoint_stream_),
        std::bind(&SteerRelbot::timer_callback, this)
    );

    light_pos.x = -1.0;
    light_pos.y = -1.0;
    light_pos.z = 0.0;

    target_visible_ = false;
    last_seen_time_ = this->get_clock()->now();
}

void SteerRelbot::create_topics() {
    left_wheel_topic_ = this->create_publisher<example_interfaces::msg::Float64>(
        "/input/left_motor/setpoint_vel", 1);

    right_wheel_topic_ = this->create_publisher<example_interfaces::msg::Float64>(
        "/input/right_motor/setpoint_vel", 1);
}

void SteerRelbot::light_callback(const geometry_msgs::msg::Point::SharedPtr msg) {
    light_pos = *msg;

    // Mark target as visible only if message contains a valid detection
    if (light_pos.x >= 0.0 && light_pos.y >= 0.0 && light_pos.z > 0.0) {
        target_visible_ = true;
        last_seen_time_ = this->get_clock()->now();
    }
}

void SteerRelbot::calculate_velocity() {
    auto now = this->get_clock()->now();

    // If target was not seen recently, enter search mode
    if (!target_visible_ || (now - last_seen_time_).seconds() > target_timeout_) {
        target_visible_ = false;

        RCLCPP_WARN_THROTTLE(
            this->get_logger(),
            *this->get_clock(),
            1000,
            "Target lost, rotating to search"
        );

        left_velocity = search_left_wheel_vel_;
        right_velocity = search_right_wheel_vel_;

        left_velocity = std::clamp(left_velocity, -max_wheel_vel_, max_wheel_vel_);
        right_velocity = std::clamp(right_velocity, -max_wheel_vel_, max_wheel_vel_);
        return;
    }

    double center_x = image_width_ / 2.0;
    double offset_x = light_pos.x - center_x;

    double omega = 0.0;
    if (std::fabs(offset_x) > deadzone_pixels_) {
        omega = -k_angular_ * offset_x;
        omega = std::clamp(omega, -max_angular_, max_angular_);
    }

    double v = 0.0;
    if (light_pos.z < small_area_threshold_) {
        double normalized = (small_area_threshold_ - light_pos.z) / small_area_threshold_;
        normalized = std::clamp(normalized, 0.0, 1.0);
        v = k_linear_ * normalized;
    } else if (light_pos.z > close_area_threshold_) {
        double normalized = (light_pos.z / close_area_threshold_) - 1.0;
        normalized = std::clamp(normalized, 0.0, 1.0);
        v = -k_linear_ * normalized;
    }

    left_velocity  = (v - (omega * wheel_base_width_ / 2.0)) / wheel_radius_;
    right_velocity = (v + (omega * wheel_base_width_ / 2.0)) / wheel_radius_;

    left_velocity = std::clamp(left_velocity, -max_wheel_vel_, max_wheel_vel_);
    right_velocity = std::clamp(right_velocity, -max_wheel_vel_, max_wheel_vel_);
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