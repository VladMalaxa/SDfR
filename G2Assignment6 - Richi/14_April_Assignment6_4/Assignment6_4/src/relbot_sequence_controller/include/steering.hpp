#ifndef STEER_RELBOT_HPP_
#define STEER_RELBOT_HPP_

// ROS Client Library CPP
#include "rclcpp/rclcpp.hpp"

// message type for velocity
#include "example_interfaces/msg/float64.hpp"

// message type from vision system
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

    // Output wheel velocities
    double left_velocity;
    double right_velocity;

    // Latest vision data
    geometry_msgs::msg::Point light_pos;

    // Search / rotation state
    bool target_visible_;
    rclcpp::Time last_seen_time_;

    // Controller timing / integration state
    bool controller_initialized_;
    rclcpp::Time last_control_time_;
    double x_set_;       // integrated forward/backward internal setpoint state
    double theta_set_;   // integrated angular internal setpoint state

    // Parameters
    std::string green_object_topic_;
    double image_width_;
    double image_height_;

    // Keep original gains for scaling the errors before first-order dynamics
    double k_linear_;
    double k_angular_;

    // Existing output limits
    double max_angular_;
    double max_wheel_vel_;
    double small_area_threshold_;
    double close_area_threshold_;
    double wheel_base_width_;
    double wheel_radius_;
    double deadzone_pixels_;
    double setpoint_stream_;

    // Search mode parameters
    double target_timeout_;
    double search_left_wheel_vel_;
    double search_right_wheel_vel_;

    // New first-order / Euler parameters
    double tau_linear_;
    double tau_angular_;
    double max_linear_;
    double max_x_set_;
    double max_theta_set_;

    // Methods
    void create_topics();
    void timer_callback();
    void calculate_velocity();

    // Callback for vision input
    void light_callback(const geometry_msgs::msg::Point::SharedPtr msg);
};

#endif /* STEER_RELBOT_HPP_ */