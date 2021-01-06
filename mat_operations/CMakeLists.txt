project(mat_operations)

add_executable(mat_operations main.cpp)
target_link_libraries(mat_operations ${OpenCV_LIBS})
