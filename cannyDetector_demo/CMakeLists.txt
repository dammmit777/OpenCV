project(cannyDetector_demo)

add_executable(cannyDetector_demo main.cpp)
target_link_libraries(cannyDetector_demo ${OpenCV_LIBS})