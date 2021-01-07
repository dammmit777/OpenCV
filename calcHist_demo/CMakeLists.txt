project(calcHist_demo)

add_executable(calcHist_demo main.cpp)
target_link_libraries(calcHist_demo ${OpenCV_LIBS})