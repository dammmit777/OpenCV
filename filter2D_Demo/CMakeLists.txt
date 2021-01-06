project(filter2D_Demo)

add_executable(filter2D_Demo main.cpp)
target_link_libraries(filter2D_Demo ${OpenCV_LIBS})