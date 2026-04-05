#include <cmath>
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
        if (msg->x < 0 || msg->y < 0 || msg->z <= 0.0) {
            // No object, stop
            twist.linear.x = 0.0;
            twist.angular.z = 0.0;
        } else {
            // Parameters for control of the turtle, based on the camera for the webcam used in testing being 300 x 200
            // Change if needed for different camera size
            double image_width = 300.0;
            double center_x = image_width / 2.0;
            double gain_angular = 0.004;
            double deadzone = 25.0;
            double small_area_threshold = 8000.0;
            double close_area_threshold = 22000.0;
            double area = msg->z;
            double offset = msg->x - center_x;

            // Rotate only when the object is sufficiently off-center
            if (std::fabs(offset) > deadzone) {
                twist.angular.z = -gain_angular * offset;
                if (twist.angular.z > 0.6) twist.angular.z = 0.6;
                if (twist.angular.z < -0.6) twist.angular.z = -0.6;
            } else {
                twist.angular.z = 0.0;
            }

            // Move forward if the object is far (small green area)
            if (area < small_area_threshold) {
                double normalized = (small_area_threshold - area) / small_area_threshold;
                if (normalized > 1.0) normalized = 1.0;
                twist.linear.x = 0.5 * normalized;
            }
            // Move backward if the object is too close (large green area)
            else if (area > close_area_threshold) {
                double normalized = (area - close_area_threshold) / close_area_threshold;
                if (normalized > 1.0) normalized = 1.0;
                twist.linear.x = -0.4 * normalized;
            }
            // Otherwise stay in place and only rotate toward center if needed
            else {
                twist.linear.x = 0.0;
            }
        }
        publisher_->publish(twist);
    }
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
};

//Main function to run the event loop to process position messages and control turtlesim
int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TurtlesimControllerNode>());
    rclcpp::shutdown();
    return 0;
}