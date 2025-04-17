// main.cpp
#include <iostream>
#include "seo_class.hpp"
#include "grid_2dim.hpp"
#include "simulation_2d.hpp"
#include "oyl_video.hpp"

int main()
{
    using Sim = Simulation2D<SEO>;
    using Grid = Grid2D<SEO>;

    // シミュレーションパラメータ
    double dt = 0.1;
    double endtime = 200;

    // Grid作成（30x30、出力あり）
    Grid grid(30, 30, true);
    grid.setOutputLabel("seo");

    // SEOの初期設定（例として全ての素子に同一パラメータを設定）
    for (int i = 0; i < grid.numRows(); ++i)
    {
        for (int j = 0; j < grid.numCols(); ++j)
        {
            auto seo = grid.getElement(i, j);
            seo->setUp(1.0, 0.001, 1.0, 1.0, 2.0, 4); // R, Rj, Cj, C, Vd, legs
        }
    }

    // シミュレーション初期化・Grid登録
    Sim sim(dt, endtime);
    sim.addGrid({grid});

    // 実行
    sim.run();

    // 出力取得・正規化・動画化
    const auto& outputs = sim.getOutputs();
    if (outputs.count("seo"))
    {
        const auto& video_data_double = outputs.at("seo");
        auto normalized = oyl::normalizeto255(video_data_double); // 0〜255のintに変換

        oyl::VideoClass video(normalized);
        video.set_filename("seo_simulation.mp4");
        video.set_codec(cv::VideoWriter::fourcc('a', 'v', 'c', '1')); // mp4用
        video.set_fps(60.0);
        video.set_scaleBar(1);
        video.set_cellsize(20);
        video.makevideo();
    }
    else
    {
        std::cerr << "出力ラベル 'seo' が見つかりませんでした。" << std::endl;
    }

    return 0;
}
