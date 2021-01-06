project(change_image)

add_executable(change_image change_image.cpp)
target_link_libraries(change_image ${OpenCV_LIBS})