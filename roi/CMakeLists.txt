project(roi)

add_executable(roi roi.cpp)
target_link_libraries(roi ${OpenCV_LIBS})