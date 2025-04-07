#include <simulation.hpp>
#include <grid_class.hpp>
#include <seo_class.hpp>
#include <printdata_class.hpp>

constexpr int size_x = 31;      // x方向の大きさ
constexpr int size_y = 31;      // y方向の大きさ
constexpr double Vd = 0.0044;   // バイアス電圧[V]
constexpr double R = 0.5;       // 抵抗[GΩ]
constexpr double Rj = 0.002;    // トンネル抵抗[GΩ]
constexpr double Cj = 10.0;     // 接合容量[aF]
constexpr double C = 2.0;       // 接続容量[aF]
constexpr double dt = 0.1;      // 刻み時間[ns]
constexpr double endtime = 200; // シミュレーション終了時間[ns]

using namespace std;

int main()
{
    // 31×31の振動子の配列を用意
    vector<int> dimension = {size_x, size_y}; // ex.4次元なら{a,b,c,d}で定義
    Grid<SEO> seogrid(dimension);
    for (int y = 0; y < size_y; y++)
    {
        for (int x = 0; x < size_x; x++)
        {
            // 指定した位置のパラメータを設定
            vector<int> indices = {x, y};
            if ((x + y) % 2 == 0) seogrid.getElement(indices)->setUp(R, Rj, Cj, C, Vd, 4);
            else seogrid.getElement(indices)->setUp(R, Rj, Cj, C, -Vd, 4);
            vector<shared_ptr<SEO>> connections = {
                seogrid.getElement({y + 1, x}),  // 上
                seogrid.getElement({y, x + 1}),  // 右
                seogrid.getElement({y - 1, x}),  // 下
                seogrid.getElement({y, x - 1})}; // 左
            seogrid.getElement(indices)->setConnections(connections);
        }
    }
    Simulation<SEO> sim(dt, endtime);
    sim.addGrid({seogrid});
    sim.run();
}