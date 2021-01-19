project(video_write)

add_executable(video_write video_write.cpp)
target_link_libraries(video_write ${OpenCV_LIBS})