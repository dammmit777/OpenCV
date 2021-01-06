project(threshold)

add_executable(threshold main.cpp)
target_link_libraries(threshold ${OpenCV_LIBS})