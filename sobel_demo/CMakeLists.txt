project(sobel_demo)

add_executable(sobel_demo main.cpp)
target_link_libraries(sobel_demo ${OpenCV_LIBS})