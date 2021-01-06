project(laplace_demo)

add_executable(laplace_demo main.cpp)
target_link_libraries(laplace_demo ${OpenCV_LIBS})