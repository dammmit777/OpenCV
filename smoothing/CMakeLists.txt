project(smoothing)

add_executable(smoothing smoothing.cpp)
target_link_libraries(smoothing ${OpenCV_LIBS})