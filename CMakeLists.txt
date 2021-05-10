#----------------------------------------------------------------------
# CMakeList.txt: CMake Project

cmake_minimum_required (VERSION 3.8)

project ("DetectAndTrack")

# Look for OpenCV libraries

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OPENCV_LIB_DIR})

# Generate the executable
add_executable (${PROJECT_NAME} "src/main.cpp")


add_subdirectory(include)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} include)

