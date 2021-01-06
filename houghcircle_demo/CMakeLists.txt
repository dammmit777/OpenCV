project(houghcircle_demo)

add_executable(houghcircle_demo main.cpp)
target_link_libraries(houghcircle_demo ${OpenCV_LIBS})