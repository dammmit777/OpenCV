project(affine_transform_demo)

add_executable(affine_transform_demo main.cpp)
target_link_libraries(affine_transform_demo ${OpenCV_LIBS})