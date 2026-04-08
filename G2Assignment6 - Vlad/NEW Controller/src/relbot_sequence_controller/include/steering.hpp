#ifndef STEER_RELBOT_HPP_
#define STEER_RELBOT_HPP_

// CPP library headers

// ROS Client Library CPP
#include "rclcpp/rclcpp.hpp"

// message types
#include "example_interfaces/msg/float64.hpp"
#include "geometry_msgs/msg/point.hpp"

class SteerRelbot : public rclcpp::Node
{
public:
    /**
     * @brief Construct a new steering object
     */
    SteerRelbot();

    const double DEFAULT_SETPOINT_STREAM = 30;  // How often the velocities are published per second

private:
    // Topics
    rclcpp::Publisher<example_interfaces::msg::Float64>::SharedPtr left_wheel_topic_;
    rclcpp::Publisher<example_interfaces::msg::Float64>::SharedPtr right_wheel_topic_;

    // Timer
    rclcpp::TimerBase::SharedPtr timer_;

    // Vision subscription
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr light_sub_;

    // attributes
    double left_velocity;
    double right_velocity;
    geometry_msgs::msg::Point light_pos;

    // methods
    void create_topics();
    void timer_callback();
    void calculate_velocity();
    void light_callback(const geometry_msgs::msg::Point::SharedPtr msg);
};

#endif /*STEER_RELBOT_HPP_*/