project(DFT)

add_executable(DFT dft.cpp)
target_link_libraries(DFT ${OpenCV_LIBS})