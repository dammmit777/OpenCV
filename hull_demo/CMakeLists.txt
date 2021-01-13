project(hull_demo)

add_executable(hull_demo hull_demo.cpp)
target_link_libraries(hull_demo ${OpenCV_LIBS})