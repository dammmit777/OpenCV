project(templateMatching)

add_executable(templateMatching main.cpp)
target_link_libraries(templateMatching ${OpenCV_LIBS})