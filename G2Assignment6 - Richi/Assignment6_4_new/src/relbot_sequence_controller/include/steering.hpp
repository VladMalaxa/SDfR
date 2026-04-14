#ifndef STEER_RELBOT_HPP_
#define STEER_RELBOT_HPP_

// ROS Client Library CPP
#include "rclcpp/rclcpp.hpp"

// message type for velocity
#include "example_interfaces/msg/float64.hpp"

// NEW: message type from vision system
#include "geometry_msgs/msg/point.hpp"

class SteerRelbot : public rclcpp::Node
{
public:
    /**
     * @brief Construct a new steering object
     */
    SteerRelbot();

private:
    // Publishers
    rclcpp::Publisher<example_interfaces::msg::Float64>::SharedPtr left_wheel_topic_;
    rclcpp::Publisher<example_interfaces::msg::Float64>::SharedPtr right_wheel_topic_;

    // Subscriber to vision system
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr light_sub_;

    // Timer
    rclcpp::TimerBase::SharedPtr timer_;

    // Attributes
    double left_velocity;
    double right_velocity;
    geometry_msgs::msg::Point light_pos;

    // Parameters
    std::string green_object_topic_;
    double image_width_;
    double image_height_;
    double k_linear_;
    double k_angular_;
    double max_angular_;
    double max_wheel_vel_;
    double small_area_threshold_;
    double close_area_threshold_;
    double wheel_base_width_;
    double wheel_radius_;
    double deadzone_pixels_;
    double setpoint_stream_;

    // Methods
    void create_topics();
    void timer_callback();
    void calculate_velocity();

    // Callback for vision input
    void light_callback(const geometry_msgs::msg::Point::SharedPtr msg);
};

#endif /*STEER_RELBOT_HPP_*/
