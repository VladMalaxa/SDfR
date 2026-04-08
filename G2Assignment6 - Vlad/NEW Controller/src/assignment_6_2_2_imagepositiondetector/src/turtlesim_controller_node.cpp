#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <geometry_msgs/msg/twist.hpp>

// ROS2 node that subscribes to green object position and controls turtlesim to keep it centered
class TurtlesimControllerNode : public rclcpp::Node {
public:
    TurtlesimControllerNode() : Node("turtlesim_controller_node") {
        subscription_ = this->create_subscription<geometry_msgs::msg::Point>(
            "/green_object_position", 10,
            std::bind(&TurtlesimControllerNode::position_callback, this, std::placeholders::_1)
        );
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
    }

private:
    void position_callback(const geometry_msgs::msg::Point::SharedPtr msg) {
        geometry_msgs::msg::Twist twist;
        if (msg->x < 0 || msg->y < 0) {
            // No object, stop
            twist.linear.x = 0.0;
            twist.angular.z = 0.0;
        } else {
            // Assume image 640x480, center 320,240
            double center_x = 150.0;
            double center_y = 100.0;
            double gain_angular = 0.005;
            double gain_linear = 0.005;
            twist.angular.z = -gain_angular * (msg->x - center_x);
            twist.linear.x = gain_linear * (center_y - msg->y);  // If object is below center, move forward to bring it up
        }
        publisher_->publish(twist);
    }
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TurtlesimControllerNode>());
    rclcpp::shutdown();
    return 0;
}