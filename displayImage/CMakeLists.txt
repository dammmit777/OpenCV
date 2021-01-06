project(displayImage)

add_executable(displayImage displayImage.cpp)
target_link_libraries(displayImage ${OpenCV_LIBS})