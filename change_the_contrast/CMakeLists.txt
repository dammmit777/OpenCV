project(change_the_contrast)

add_executable(change_the_contrast main.cpp)
target_link_libraries(change_the_contrast ${OpenCV_LIBS})