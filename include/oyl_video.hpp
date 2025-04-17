// oyl_video.hpp (oyl:oyalab)
#ifndef OYLVIDEO_HPP
#define OYLVIDEO_HPP

#include <opencv2/opencv.hpp>
#include <vector>

namespace oyl {
    // 参照で渡せるように修正
    std::vector<std::vector<std::vector<int>>> normalizeto255(const std::vector<std::vector<std::vector<double>>>& video_data_double);

    void basic_makevideo_int(std::vector<std::vector<std::vector<int>>> basic_video_data_int);
    void basic_makevideo_double(std::vector<std::vector<std::vector<double>>> basic_video_data_double);

    class VideoClass {
    private: 
        std::vector<std::vector<std::vector<int>>> video_data;
        std::string filename;
        int t_size;     //size of frame
        int x_size;     //size of x direction
        int y_size;     //size of y direction
        int cell_size;  //size of celll
        int x_width;    //x_width depends on cell_size.
        int y_height;   //y_height depends on cell_size.
        double fps;
        int codec;

        bool flag_scaleBar;
        int bar_width;
        int gap_width;

        cv::Mat create_frame(const std::vector<std::vector<int>>& frame_data) const;
        void    initialize_scaleBar();
        cv::Mat add_scaleBar(const cv::Mat& frame) const;
        void    update_xwidth_yheight();
    public:
        VideoClass(std::vector<std::vector<std::vector<int>>> video_data);
        
        void makevideo() const;
        void set_filename(std::string filename);
        void set_codec(int codec);
        void set_fps(double fps);
        void set_cellsize(int cell_size);
        VideoClass& set_scaleBar(bool flag_scaleBar);
        VideoClass& set_barwidth(int bar_width);
        VideoClass& set_gapwidth(int gap_width);
        void show_parameters() const;
        void show_size() const;
    };

}// namespace oyl

#if defined(UNIVERSAL_HEADER_ONLY) || defined(OYL_VIDEO_HEADER_ONLY)
  #include "cpp_files/oyl_video.cpp"
#endif

#endif // OYLVIDEO_HPP

//スケールバーのオプション
//->背景の色のオプション しろ 黒

/*coordinate and direction
 o - - - - - -> x
 |(0,0) (1,0) (2,0)...
 |(0,1) (1,1) (2,1)...
 |(0,2) (1,2) (2,2)...
 | :
 | :
 V 
 y
 
*/