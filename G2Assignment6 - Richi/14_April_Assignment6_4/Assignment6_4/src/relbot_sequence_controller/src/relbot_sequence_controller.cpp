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

SteerRelbot::SteerRelbot() : Node("steer_relbot")
{
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

    // Search / rotation mode parameters
    declare_parameter<double>("target_timeout", 0.5);
    declare_parameter<double>("search_left_wheel_vel", 3.0);
    declare_parameter<double>("search_right_wheel_vel", -2.0);

    // First-order dynamics / Forward Euler parameters
    declare_parameter<double>("tau_linear", 1.0);
    declare_parameter<double>("tau_angular", 1.0);
    declare_parameter<double>("max_linear", 0.6);
    declare_parameter<double>("max_x_set", 2.0);
    declare_parameter<double>("max_theta_set", 3.14);

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

    get_parameter("target_timeout", target_timeout_);
    get_parameter("search_left_wheel_vel", search_left_wheel_vel_);
    get_parameter("search_right_wheel_vel", search_right_wheel_vel_);

    get_parameter("tau_linear", tau_linear_);
    get_parameter("tau_angular", tau_angular_);
    get_parameter("max_linear", max_linear_);
    get_parameter("max_x_set", max_x_set_);
    get_parameter("max_theta_set", max_theta_set_);

    create_topics();

    light_sub_ = this->create_subscription<geometry_msgs::msg::Point>(
        green_object_topic_, 10,
        std::bind(&SteerRelbot::light_callback, this, std::placeholders::_1)
    );

    timer_ = this->create_wall_timer(
        std::chrono::duration<double>(1.0 / setpoint_stream_),
        std::bind(&SteerRelbot::timer_callback, this)
    );

    // Initialize latest light position as invalid
    light_pos.x = -1.0;
    light_pos.y = -1.0;
    light_pos.z = 0.0;

    // Initialize output velocities
    left_velocity = 0.0;
    right_velocity = 0.0;

    // Target visibility state
    target_visible_ = false;
    last_seen_time_ = this->get_clock()->now();

    // Controller timing / integration state
    controller_initialized_ = false;
    last_control_time_ = this->get_clock()->now();
    x_set_ = 0.0;
    theta_set_ = 0.0;
}

void SteerRelbot::create_topics()
{
    left_wheel_topic_ = this->create_publisher<example_interfaces::msg::Float64>(
        "/input/left_motor/setpoint_vel", 1);

    right_wheel_topic_ = this->create_publisher<example_interfaces::msg::Float64>(
        "/input/right_motor/setpoint_vel", 1);
}

void SteerRelbot::light_callback(const geometry_msgs::msg::Point::SharedPtr msg)
{
    light_pos = *msg;

    // Mark target as visible only if message contains a valid detection
    if (light_pos.x >= 0.0 && light_pos.y >= 0.0 && light_pos.z > 0.0) {
        target_visible_ = true;
        last_seen_time_ = this->get_clock()->now();
    }
}

void SteerRelbot::calculate_velocity()
{
    auto now = this->get_clock()->now();

    // Initialize controller timing once
    if (!controller_initialized_) {
        last_control_time_ = now;
        controller_initialized_ = true;
    }

    double dt = (now - last_control_time_).seconds();
    last_control_time_ = now;

    // Safety against startup / timing glitches
    if (dt <= 0.0) {
        dt = 1.0 / setpoint_stream_;
    } else if (dt > 0.2) {
        dt = 0.2;
    }

    // -------------------------------------------------------------------------
    // SEARCH MODE: target lost or not seen recently
    // -------------------------------------------------------------------------
    if (!target_visible_ || (now - last_seen_time_).seconds() > target_timeout_) {
        target_visible_ = false;

        RCLCPP_WARN_THROTTLE(
            this->get_logger(),
            *this->get_clock(),
            1000,
            "Target lost, rotating to search"
        );

        // Reset internal integrated states so stale values do not remain
        x_set_ = 0.0;
        theta_set_ = 0.0;

        left_velocity = search_left_wheel_vel_;
        right_velocity = search_right_wheel_vel_;

        left_velocity = std::clamp(left_velocity, -max_wheel_vel_, max_wheel_vel_);
        right_velocity = std::clamp(right_velocity, -max_wheel_vel_, max_wheel_vel_);
        return;
    }

    // -------------------------------------------------------------------------
    // IMAGE-BASED ERRORS
    // -------------------------------------------------------------------------
    double center_x = image_width_ / 2.0;
    double offset_x = light_pos.x - center_x;

    // Angular error: only react outside the deadzone
    double angular_error = 0.0;
    if (std::fabs(offset_x) > deadzone_pixels_) {
        angular_error = -k_angular_ * offset_x;
    }

    // Linear error from apparent target size / area
    double linear_error = 0.0;

    if (light_pos.z < small_area_threshold_) {
        // Target is far -> move forward
        double normalized = (small_area_threshold_ - light_pos.z) / small_area_threshold_;
        normalized = std::clamp(normalized, 0.0, 1.0);
        linear_error = k_linear_ * normalized;
    } else if (light_pos.z > close_area_threshold_) {
        // Target is too close -> move backward
        double normalized = (light_pos.z / close_area_threshold_) - 1.0;
        normalized = std::clamp(normalized, 0.0, 1.0);
        linear_error = -k_linear_ * normalized;
    } else {
        // Inside acceptable range
        linear_error = 0.0;
    }

    // -------------------------------------------------------------------------
    // FIRST-ORDER DYNAMICS
    //
    // x_dot_set     = (1 / tau_linear_)  * linear_error
    // theta_dot_set = (1 / tau_angular_) * angular_error
    // -------------------------------------------------------------------------
    double x_dot_set = linear_error / tau_linear_;
    double theta_dot_set = angular_error / tau_angular_;

    // Clamp dynamic rates before integration
    x_dot_set = std::clamp(x_dot_set, -max_linear_, max_linear_);
    theta_dot_set = std::clamp(theta_dot_set, -max_angular_, max_angular_);

    // -------------------------------------------------------------------------
    // FORWARD EULER INTEGRATION
    //
    // x_set_(k+1)     = x_set_(k)     + x_dot_set * dt
    // theta_set_(k+1) = theta_set_(k) + theta_dot_set * dt
    // -------------------------------------------------------------------------
    x_set_ += x_dot_set * dt;
    theta_set_ += theta_dot_set * dt;

    // Anti-windup / state limiting
    x_set_ = std::clamp(x_set_, -max_x_set_, max_x_set_);
    theta_set_ = std::clamp(theta_set_, -max_theta_set_, max_theta_set_);

    // -------------------------------------------------------------------------
    // COMMAND SELECTION
    //
    // Use the first-order rates as commanded body velocities.
    // -------------------------------------------------------------------------
    double v = x_dot_set;
    double omega = theta_dot_set;

    // Final safety clamp
    v = std::clamp(v, -max_linear_, max_linear_);
    omega = std::clamp(omega, -max_angular_, max_angular_);

    // Differential-drive inverse kinematics
    left_velocity  = (v - (omega * wheel_base_width_ / 2.0)) / wheel_radius_;
    right_velocity = (v + (omega * wheel_base_width_ / 2.0)) / wheel_radius_;

    // Final wheel saturation
    left_velocity = std::clamp(left_velocity, -max_wheel_vel_, max_wheel_vel_);
    right_velocity = std::clamp(right_velocity, -max_wheel_vel_, max_wheel_vel_);
}

void SteerRelbot::timer_callback()
{
    calculate_velocity();

    example_interfaces::msg::Float64 left_wheel;
    left_wheel.data = left_velocity;

    example_interfaces::msg::Float64 right_wheel;
    right_wheel.data = right_velocity;

    left_wheel_topic_->publish(left_wheel);
    right_wheel_topic_->publish(right_wheel);
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SteerRelbot>());
    rclcpp::shutdown();
    return 0;
}