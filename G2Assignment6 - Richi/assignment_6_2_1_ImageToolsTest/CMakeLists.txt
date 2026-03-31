cmake_minimum_required(VERSION 3.8)
project(assignment_6_2_1_ImageToolsTest)

find_package(ament_cmake REQUIRED)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()