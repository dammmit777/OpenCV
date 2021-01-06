project(IO_image)

add_executable(IO_image IO_image.cpp)
target_link_libraries(IO_image ${OpenCV_LIBS})