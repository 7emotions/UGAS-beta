cmake_minimum_required(VERSION 3.12)
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
set(EXECUTABLE_NAME ${PROJECT_NAME})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

# Find non-ros packages
find_package(OpenCV 4.7 REQUIRED)
find_package(HikCameraSDK REQUIRED)

# Include project source directory
include_directories(${PROJECT_SOURCE_DIR}/src ${CONFIGURE_DIR_PATH})

# Recursively search for all source files under the 'src' folder and store them into UGAS_SOURCE variable
# Flag 'CONFIGURE_DEPENDS' asks cmake to detect GLOB result changes so no need to rerun cmake when adding a new source file.
file(GLOB_RECURSE UGAS_SOURCE CONFIGURE_DEPENDS
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.cc)

# Find ros packages & add source files to compilation
add_executable(${EXECUTABLE_NAME} ${UGAS_SOURCE})

# Link libraries
target_link_libraries(${EXECUTABLE_NAME} ${OpenCV_LIBS} ${HikCameraSDK_LIB} ${OpenVINO_LIB} -lpthread)
