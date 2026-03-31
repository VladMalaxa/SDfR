#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

// ROS2 node that detects green objects in the subscribed image stream and saves the coordinates of the largest detected object to the console. The node subscribes to the "/image" topic, processes incoming images to identify green objects, and logs their coordinates if detected.
class GreenObjectDetectorNode : public rclcpp::Node {
public:
    GreenObjectDetectorNode() : Node("green_object_detector_node") {
        subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
            "/image", 10,
            std::bind(&GreenObjectDetectorNode::image_callback, this, std::placeholders::_1)
        );
    }

// Callback function, execute when a new image is received
private:
    void image_callback(const sensor_msgs::msg::Image::SharedPtr msg) {
        
        // Convert ROS image to OpenCV format
        cv_bridge::CvImagePtr cv_ptr;
        try {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        } catch (cv_bridge::Exception& e) {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
            return;
        }
        // Extract image and convert to HSV to help detect color better
        cv::Mat img = cv_ptr->image;
        cv::Mat hsv, mask;
        cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);


        // Chosen HSV range for green
        cv::Scalar lower_green(35, 100, 100);
        cv::Scalar upper_green(85, 255, 255);

        //Apply mask to highlight green objects and store countours
        cv::inRange(hsv, lower_green, upper_green, mask);
        std::vector<std::vector<cv::Point>> contours;

        //Find the largest contour and calculate the coordinates of its center
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        if (!contours.empty()) {
            auto max_it = std::max_element(contours.begin(), contours.end(),
                [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
                    return cv::contourArea(a) < cv::contourArea(b);
                });
            cv::Moments m = cv::moments(*max_it);
            if (m.m00 > 0) {
                int cx = static_cast<int>(m.m10 / m.m00);
                int cy = static_cast<int>(m.m01 / m.m00);
                RCLCPP_INFO(this->get_logger(), "Green object at (x=%d, y=%d)", cx, cy);
            }
        } else {
            RCLCPP_INFO(this->get_logger(), "No green object detected");
        }
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
};

//Main function to initialize ROS2 node and run the event loop to process images
int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GreenObjectDetectorNode>());
    rclcpp::shutdown();
    return 0;
}
