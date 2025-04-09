#ifndef SIMULATION_2D_HPP
#define SIMULATION_2D_HPP

#include <fstream>
#include <vector>
#include <memory>
#include <utility>
#include <map>
#include <cmath>
#include "seo_class.hpp"
#include "grid_2dim.hpp"
#include "printdata_class.hpp" // ←これも2D版に書き換えるなら適宜変更

template <typename Element>
class Simulation2D
{
private:
    double t;                  // 現在の時間（不定期に増える）
    double dt;                 // 基本刻み（参考値）
    double endtime;            // 終了時刻
    double outputInterval;     // 出力間隔（例: 0.1）
    double nextOutputTime;     // 次に出力すべき時刻（0.1, 0.2, ...）
    std::vector<Grid2D<Element>> grids;                 // Grid2Dのインスタンス配列
    // std::vector<PrintData<Element>> printdatavector;    // 出力データ
    std::map<std::string, std::vector<std::vector<std::vector<std::vector<double>>>>> outputs; // oyl-video用データ形式

public:
    Simulation2D(double dT, double EndTime);

    std::pair<bool, std::shared_ptr<Grid2D<Element>>> comparewt();  // 最小wt比較
    void handleTunnels(Grid2D<Element>& tunnelelement);             // トンネル処理

    void openFiles() const;
    void closeFiles() const;
    void outputToFile(); // 累積時間に応じて出力
    void outputTooyl();

    void runStep();             // 1ステップ実行
    void addGrid(const std::vector<Grid2D<Element>>& Gridinstance); // Grid追加
    void run();                 // 全体実行
    std::vector<Grid2D<Element>>& getGrids(); // グリッド取得
};

// コンストラクタ
template <typename Element>
Simulation2D<Element>::Simulation2D(double dT, double EndTime)
    : t(0.0), dt(dT), endtime(EndTime), outputInterval(0.1), nextOutputTime(0.1) {}

// 最小wtを探索する
template <typename Element>
std::pair<bool, std::shared_ptr<Grid2D<Element>>> Simulation2D<Element>::comparewt()
{
    double minwt = dt;
    std::shared_ptr<Grid2D<Element>> tunnelelement = nullptr;
    for (auto &grid : grids)
    {
        if (grid.gridminwt(dt))
        {
            double candidate = grid.getMinWT();
            if (candidate < minwt)
            {
                minwt = candidate;
                tunnelelement = std::make_shared<Grid2D<Element>>(grid);
            }
        }
    }
    if (minwt < dt) return {true, tunnelelement};
    return {false, nullptr};
}

// トンネル処理を実行
template <typename Element>
void Simulation2D<Element>::handleTunnels(Grid2D<Element>& tunnelelement)
{
    tunnelelement.getTunnelPlace()->setTunnel(tunnelelement.getTunnelDirection());
}

// ファイルを開く
template <typename Element>
void Simulation2D<Element>::openFiles() const
{
    // for (auto &outputdata : printdatavector)
    // {
    //     outputdata.openFile();
    // }
}

// ファイルを閉じる
template <typename Element>
void Simulation2D<Element>::closeFiles() const
{
    // for (auto &outputdata : printdatavector)
    // {
    //     outputdata.closeFile();
    // }
}

// 出力処理（未実装部分を仮追加）
template <typename Element>
void Simulation2D<Element>::outputToFile()
{
    // if (accumulatedTime >= outputInterval)
    // {
    //     accumulatedTime -= outputInterval;
    //     // TODO: 各 printdatavector に対してデータを渡す処理を入れる
    // }
}

template <typename Element>
void Simulation2D<Element>::outputTooyl()
{
    if (t >= nextOutputTime)
    {
        size_t timeIndex = static_cast<size_t>(nextOutputTime / outputInterval);

        for (const auto &grid : grids)
        {
            if (!grid.isOutputEnabled()) continue;

            int rows = grid.numRows();
            int cols = grid.numCols();

            std::vector<std::vector<double>> vnGrid(rows, std::vector<double>(cols));
            for (int i = 0; i < rows; ++i)
            {
                for (int j = 0; j < cols; ++j)
                {
                    vnGrid[i][j] = grid.getElement(i, j)->getVn();
                }
            }

            outputs["Vn"].resize(timeIndex + 1);
            outputs["Vn"][timeIndex].push_back(vnGrid);
        }

        nextOutputTime += outputInterval; // 次の出力時刻を更新
    }
}


// シミュレーションの1ステップを実行
template <typename Element>
void Simulation2D<Element>::runStep()
{
    double steptime = dt;

    outputToFile();

    for (int i = 0; i < 5; i++)
    {
        for (auto &grid : grids)
        {
            grid.updateGridVn();
        }
    }

    for (auto &grid : grids)
    {
        grid.updateGriddE();
    }

    auto compared = this->comparewt();
    if (compared.first)
    {
        handleTunnels(*compared.second);
        steptime = compared.second->getMinWT();
    }

    for (auto &grid : grids)
    {
        grid.updateGridQn(steptime);
    }

    t += steptime;
    accumulatedTime += steptime;
}

// Gridインスタンスの配列を登録
template <typename Element>
void Simulation2D<Element>::addGrid(const std::vector<Grid2D<Element>>& Gridinstance)
{
    grids = Gridinstance;
}

// 全体シミュレーションの実行
template <typename Element>
void Simulation2D<Element>::run()
{
    openFiles();
    while (t < endtime)
    {
        runStep();
    }
    closeFiles();
}

// グリッド取得
template <typename Element>
std::vector<Grid2D<Element>>& Simulation2D<Element>::getGrids()
{
    return grids;
}

#endif // SIMULATION_2D_HPP
