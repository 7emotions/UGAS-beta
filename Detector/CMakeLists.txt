# CMakeList.txt: Detector 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

# 将源代码添加到此项目的可执行文件。
add_executable (Detector "src/Detector.cpp" "Inc/Detector.h" "Inc/LightDescriptor.h" "src/LightDescriptor.cpp" "src/TinyXML.cpp" "Inc/TinyXML.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Detector PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。

target_include_directories(Detector PUBLIC Inc)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(Detector ${OpenCV_LIBS})

# add_custom_command(TARGET Detector PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:Detector>)