project(mat_mask_operations)

add_executable(mat_mask_operations main.cpp)
target_link_libraries(mat_mask_operations ${OpenCV_LIBS})