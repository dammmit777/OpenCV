project(houghlines_demo)

add_executable(houghlines_demo main.cpp)
target_link_libraries(houghlines_demo ${OpenCV_LIBS})