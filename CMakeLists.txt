﻿cmake_minimum_required(VERSION 3.12)
project(ugas-beta VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set compilation flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()



set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Set the output executable file name:
# When compiling in the ROS environment, the output executable file name will be the node name.
# Otherwise, the output file name will be the project name.
set(EXECUTABLE_NAME ugas_ros)
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(OpenCV 4.7 REQUIRED)
find_package(HikCameraSDK REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()



# Include project source directory
include_directories(${PROJECT_SOURCE_DIR}/src ${CONFIGURE_DIR_PATH})

# Recursively search for all source files under the 'src' folder and store them into UGAS_SOURCE variable
# Flag 'CONFIGURE_DEPENDS' asks cmake to detect GLOB result changes so no need to rerun cmake when adding a new source file.
file(GLOB_RECURSE UGAS_SOURCE CONFIGURE_DEPENDS
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.cc)

# Find ros packages & add source files to compilation
add_executable(${EXECUTABLE_NAME} ${UGAS_SOURCE})
ament_target_dependencies(${EXECUTABLE_NAME} rclcpp std_msgs)

target_include_directories(ugas_ros PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(ugas_ros PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

install(TARGETS ugas_ros
  DESTINATION lib/${PROJECT_NAME})


# Link libraries
target_link_libraries(${EXECUTABLE_NAME} ${OpenCV_LIBS} ${HikCameraSDK_LIB} ${OpenVINO_LIB} -lpthread)


ament_package()