project(matrix)

add_executable(matrix1 drawing1.cpp)
target_link_libraries(matrix1 ${OpenCV_LIBS})

add_executable(matrix2 drawing2.cpp)
target_link_libraries(matrix2 ${OpenCV_LIBS})