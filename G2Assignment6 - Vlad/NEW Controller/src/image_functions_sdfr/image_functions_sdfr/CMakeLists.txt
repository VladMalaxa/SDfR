cmake_minimum_required(VERSION 3.8)
project(image_functions_sdfr)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_ros REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)

# Library
add_library(image_functions SHARED
  src/image_functions.cpp
)

target_compile_features(image_functions PUBLIC c_std_99 cxx_std_17)

target_include_directories(image_functions PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(
  image_functions
  rclcpp
  sensor_msgs
)

target_compile_definitions(image_functions PRIVATE "IMAGE_FUNCTIONS_BUILDING_LIBRARY")

# Install headers
install(
  DIRECTORY include/
  DESTINATION include
)

# Install library
install(
  TARGETS image_functions
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Export for other packages
ament_export_include_directories(include)
ament_export_libraries(image_functions)
ament_export_targets(export_${PROJECT_NAME})
ament_export_dependencies(sensor_msgs)

ament_package()