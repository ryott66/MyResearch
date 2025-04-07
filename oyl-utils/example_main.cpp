//example_main.cpp
// #define OYL_VIDEO_HEADER_ONLY
#include "oyl_video.hpp"

int main(){
    int t_frames = 400;
    int width = 30;
    int height = 30;
    std::vector<std::vector<std::vector<int>>> video_data_A(t_frames, std::vector<std::vector<int>>(width, std::vector<int>(height, 0)));
    std::vector<std::vector<std::vector<double>>> video_data_B(t_frames, std::vector<std::vector<double>>(width, std::vector<double>(height, 0)));
    std::vector<std::vector<std::vector<int>>> video_data_C(t_frames, std::vector<std::vector<int>>(width, std::vector<int>(height, 0)));
    std::vector<std::vector<std::vector<int>>> video_data_D(t_frames, std::vector<std::vector<int>>(width, std::vector<int>(height, 0)));
    std::vector<std::vector<std::vector<double>>> video_data_E(t_frames, std::vector<std::vector<double>>(width, std::vector<double>(height, 0)));
   
    for (int t = 0; t < t_frames; ++t) {
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                video_data_A[t][x][y] = static_cast<int>((x + y + t) % 256);  // 0～255
                video_data_B[t][x][y] = 12.3*((x + y + t) % 256);
                video_data_C[t][x][y] = static_cast<int>((x + y + t) % 256);
                video_data_D[t][x][y] = static_cast<int>((x + y + t) % 256);
                video_data_E[t][x][y] = 45.6*((x + y + t) % 256);
            }
        }
    }
//*
    std::cout << "A ";
    oyl::basic_makevideo_int(video_data_A);

    std::cout << "B ";
    oyl::basic_makevideo_double(video_data_B);
//*/

    std::cout << "C ";
    oyl::VideoClass VideoClass_C(video_data_C);
    VideoClass_C.set_filename("output_int_fps60.mp4");
    VideoClass_C.set_codec(cv::VideoWriter::fourcc('m', 'p', '4', 'v'));
    VideoClass_C.set_fps(60.0);
    VideoClass_C.set_scaleBar(1)
                .set_gapwidth(4)
                .set_barwidth(8);
    VideoClass_C.set_cellsize(20);
    VideoClass_C.makevideo();
    VideoClass_C.show_size();
    VideoClass_C.show_parameters();

//*
    std::cout << "D ";
    oyl::VideoClass VideoClass_D(video_data_D);
    VideoClass_D.set_filename("output_int_fps75.mp4");
    VideoClass_D.set_codec(cv::VideoWriter::fourcc('m', 'p', '4', 'v'));
    VideoClass_D.set_fps(75.0);
    VideoClass_D.set_scaleBar(1);
    VideoClass_D.makevideo();


    std::cout << "E ";
    oyl::VideoClass VideoClass_E(oyl::normalizeto255(video_data_E));
    VideoClass_E.set_filename("output_double_fps45.mp4");
    VideoClass_E.set_codec(cv::VideoWriter::fourcc('a', 'v', 'c', '1'));
    VideoClass_E.set_fps(45.0);
    VideoClass_E.makevideo();
//*/
    return 0;
}