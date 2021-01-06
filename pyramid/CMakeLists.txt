project(pyramid)

add_executable(pyramid main.cpp)
target_link_libraries(pyramid ${OpenCV_LIBS})