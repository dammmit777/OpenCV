project(video_input_psnr_ssim)

add_executable(video_input_psnr_ssim main.cpp)
target_link_libraries(video_input_psnr_ssim ${OpenCV_LIBS})