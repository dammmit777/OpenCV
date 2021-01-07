project(equalizehist_demo)

add_executable(equalizehist_demo main.cpp)
target_link_libraries(equalizehist_demo ${OpenCV_LIBS})