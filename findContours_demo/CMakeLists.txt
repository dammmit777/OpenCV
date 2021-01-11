project(findContours_demo)

add_executable(findContours_demo main.cpp)
target_link_libraries(findContours_demo ${OpenCV_LIBS})