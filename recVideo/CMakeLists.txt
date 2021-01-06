project(recVideo)

add_executable(recVideo recVideo.cpp)
target_link_libraries(recVideo ${OpenCV_LIBS})