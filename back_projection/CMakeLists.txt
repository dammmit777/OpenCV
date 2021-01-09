project(back_projection)

add_executable(calcBackProject_Demo1 calcBackProject_Demo1.cpp)
target_link_libraries(calcBackProject_Demo1 ${OpenCV_LIBS})

add_executable(calcBackProject_Demo2 calcBackProject_Demo2.cpp)
target_link_libraries(calcBackProject_Demo2 ${OpenCV_LIBS})