project(remap_demo)

add_executable(remap_demo main.cpp)
target_link_libraries(remap_demo ${OpenCV_LIBS})