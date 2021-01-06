project(addingImages)

add_executable(addingImages addingImages.cpp)
target_link_libraries(addingImages ${OpenCV_LIBS})