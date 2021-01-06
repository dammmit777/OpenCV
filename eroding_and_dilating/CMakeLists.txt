project(eroding_and_dilating)

add_executable(eroding_and_dilating main.cpp)
target_link_libraries(eroding_and_dilating ${OpenCV_LIBS})