project(study_mat)

add_executable(study_mat study_mat.cpp)
target_link_libraries(study_mat ${OpenCV_LIBS})