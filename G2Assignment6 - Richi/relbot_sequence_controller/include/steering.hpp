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

    const double DEFAULT_SETPOINT_STREAM = 30;

private:
    // Publishers
    rclcpp::Publisher<example_interfaces::msg::Float64>::SharedPtr left_wheel_topic_;
    rclcpp::Publisher<example_interfaces::msg::Float64>::SharedPtr right_wheel_topic_;

    // NEW: Subscriber to vision system
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr light_sub_;

    // Timer
    rclcpp::TimerBase::SharedPtr timer_;

    // attributes
    double left_velocity;
    double right_velocity;

    // NEW: store detected object position
    geometry_msgs::msg::Point light_pos;

    // methods
    void create_topics();
    void timer_callback();
    void calculate_velocity();

    // NEW: callback for vision input
    void light_callback(const geometry_msgs::msg::Point::SharedPtr msg);
};

#endif /*STEER_RELBOT_HPP_*/