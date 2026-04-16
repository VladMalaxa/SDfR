//==============================================================
// Filename    : relbot_sequence_controller.cpp
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of the green object detection function 
// it receives images from the "/image" topic, processes them to find the position of the largest 
// green object, and publishes the coordinates and area of that object to the
// "/green_object_position" topic. It also publishes a visualization image with the 
// detected object highlighted to the "/processed_image" topic.
//==============================================================

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <geometry_msgs/msg/point.hpp>

// The node subscribes to the "/image" topic first.
class ImagePositionDetectorNode : public rclcpp::Node {
public:
    ImagePositionDetectorNode() : Node("image_position_detector_node") {
        subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
            "/image", 10,
            std::bind(&ImagePositionDetectorNode::image_callback, this, std::placeholders::_1)
        );
        publisher_ = this->create_publisher<geometry_msgs::msg::Point>("/green_object_position", 10);
        image_publisher_ = this->create_publisher<sensor_msgs::msg::Image>("/processed_image", 10);
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

        //cv::Scalar lower_green(20, 50, 50);
        //cv::Scalar upper_green(100, 255, 255);

        //Apply mask to highlight green objects and store countours
        cv::inRange(hsv, lower_green, upper_green, mask);
        std::vector<std::vector<cv::Point>> contours;

        //Find the largest contour and calculate the coordinates of its center
        //Also calculate the total area of green
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        geometry_msgs::msg::Point point_msg;
        if (!contours.empty()) {
            auto max_it = std::max_element(contours.begin(), contours.end(),
                [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
                    return cv::contourArea(a) < cv::contourArea(b);
                });
            double area = cv::contourArea(*max_it);
            cv::Moments m = cv::moments(*max_it);
            if (m.m00 > 0) {
                int cx = static_cast<int>(m.m10 / m.m00);
                int cy = static_cast<int>(m.m01 / m.m00);
                point_msg.x = cx;
                point_msg.y = cy;
                point_msg.z = area;
                RCLCPP_INFO(this->get_logger(), "Green object at (x=%f, y=%f, area=%f)", point_msg.x, point_msg.y, point_msg.z);
            } 
            // If the contour has zero area, print for debuggging
            else {
                point_msg.x = -1.0;
                point_msg.y = -1.0;
                point_msg.z = 0.0;
                RCLCPP_INFO(this->get_logger(), "Detected contour has zero area");
            }
        } 
        // If no contours are found, publish a message with negative coordinates and zero area to indicate no object detected, print for debugging
        else {
            point_msg.x = -1.0;
            point_msg.y = -1.0;
            point_msg.z = 0.0;
            RCLCPP_INFO(this->get_logger(), "No green object detected");
        }
        publisher_->publish(point_msg);

        // Create visualization on the image
        cv::Mat vis_img = img.clone();
        
        // Draw center crosshair
        int center_x = img.cols / 2;
        int center_y = img.rows / 2;
        cv::line(vis_img, cv::Point(center_x - 20, center_y), cv::Point(center_x + 20, center_y), cv::Scalar(255, 255, 255), 2);
        cv::line(vis_img, cv::Point(center_x, center_y - 20), cv::Point(center_x, center_y + 20), cv::Scalar(255, 255, 255), 2);
        
        // Draw contour of the largest green object
        if (!contours.empty()) {
            auto max_it = std::max_element(contours.begin(), contours.end(),
                [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
                    return cv::contourArea(a) < cv::contourArea(b);
                });
            cv::drawContours(vis_img, std::vector<std::vector<cv::Point>>{*max_it}, -1, cv::Scalar(0, 255, 0), 2);
            
            // Draw center point of the object
            cv::Moments m = cv::moments(*max_it);
            if (m.m00 > 0) {
                int cx = static_cast<int>(m.m10 / m.m00);
                int cy = static_cast<int>(m.m01 / m.m00);
                cv::circle(vis_img, cv::Point(cx, cy), 5, cv::Scalar(0, 0, 255), -1);
            }
        }

        // Publish the processed image
        sensor_msgs::msg::Image::SharedPtr processed_msg = cv_bridge::CvImage(msg->header, "bgr8", vis_img).toImageMsg();
        image_publisher_->publish(*processed_msg);
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr publisher_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_publisher_;
};

//Main function to initialize ROS2 node and run the event loop to process images
int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ImagePositionDetectorNode>());
    rclcpp::shutdown();
    return 0;
}