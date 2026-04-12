cmake_minimum_required(VERSION 3.8)
project(assignment_6_2_2_ImagePositionDetector)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED COMPONENTS core highgui imgcodecs imgproc videoio)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)

add_executable(image_position_detector_node src/image_position_detector_node.cpp)
ament_target_dependencies(image_position_detector_node rclcpp sensor_msgs cv_bridge OpenCV geometry_msgs)

add_executable(turtlesim_controller_node src/turtlesim_controller_node.cpp)
ament_target_dependencies(turtlesim_controller_node rclcpp geometry_msgs turtlesim)

install(TARGETS image_position_detector_node turtlesim_controller_node DESTINATION lib/${PROJECT_NAME})

install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()