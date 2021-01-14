project(imageSegmentation)

add_executable(imageSegmentation imageSegmentation.cpp)
target_link_libraries(imageSegmentation ${OpenCV_LIBS})