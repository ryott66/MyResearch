#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <fstream>
#include "seo_class.hpp"
#include "grid_class.hpp"

// テンプレートを利用しているためファイル分割は無し
template <typename Element>

class Simulation
{
private:
    double t;                                        // 現在のシミュレーション時間[ns]
    double dt;                                       // 刻み時間[ns]
    double endtime;                                  // シミュレーションの終了時間[ns]
    vector<Grid<Element>> grids;                     // 複数のGridインスタンスを保持するベクトル
    vector<string> filenames;                        // 出力するファイルのベクトル

public:
    // コンストラクタ（刻み時間とシミュレーション終了時間を入力）
    Simulation(double dT, double EndTime);

    // wtの比較
    pair<bool, shared_ptr<Grid<Element>>> comparewt();

    // トンネルの処理
    void handleTunnels(Grid<Element> &tunnelelement);

    // ファイル出力の処理
    void outputToFile() const;

    // シミュレーションの１ステップ
    void runStep();

    // Gridインスタンスの管理
    void addGrid(const vector<Grid<Element>> &Gridinstance); // 新しいGridインスタンスを追加
    // void removeGrid(int index);               // Gridインスタンスを削除

    // シミュレーションの全体の実行
    void run(double dt, double endtime);

    // テスト用
    // girdを取得
    vector<Grid<Element>> &getGrids();
};

//-------------public----------------//

// コンストラクタの定義
template <typename Element>
Simulation<Element>::Simulation(double dT, double EndTime) : t(0.0), dt(dT), endtime(EndTime) {}

// wtの比較
template <typename Element>
pair<bool, shared_ptr<Grid<Element>>> Simulation<Element>::comparewt()
{
    double minwt = dt;
    shared_ptr<Grid<Element>> tunnelelement = nullptr;
    for (auto &grid : grids)
    {
        if (grid.gridminwt(dt)) // gridの中でwtを計算したもののみifを実行
        {
            minwt = min(minwt, grid.minwt); // 最小を更新
            tunnelelement = make_shared<Grid<Element>>(grid); // gridをshared_ptrに変換
        }
    }
    if (minwt < dt) return {true, tunnelelement};
    return {false, nullptr};
}

// トンネル処理
template <typename Element>
void Simulation<Element>::handleTunnels(Grid<Element> &tunnelelement)
{
    tunnelelement->tunnelplace->setTunnel(tunnelelement->tunneldirection);
}

// ファイル出力の処理
template <typename Element>
void Simulation<Element>::outputToFile() const
{
    // ファイル出力処理をここに追加
    // fpを使って、必要なデータを出力ファイルに保存する
}

// シミュレーションの１ステップを実行
template <typename Element>
void Simulation<Element>::runStep()
{
    double steptime = dt;
    // ファイル出力
    outputToFile();
    // Vnの更新(5回実行)
    for (int i = 0; i < 5; i++)
    {
        for (auto &grid : grids)
        {
            grid.updateGridVn();
        }
    }
    // dEの計算
    for (auto &grid : grids)
    {
        grid.updateGriddE();
    }
    // wtの計算とトンネル処理
    auto compared = this->comparewt();
    if(compared.first)
    {
        handleTunnels(*compared.second); // 参照渡しでトンネル処理
        steptime = compared.second->minwt;
    }
    
    // 電荷のチャージ
    for (auto &grid : grids)
    {
        grid.updateGridQn(steptime);
    }
    t += steptime; // 時間を進める
}

// Gridインスタンスの配列を引数として受け取り、gridsに代入するメソッド
template <typename Element>
void Simulation<Element>::addGrid(const vector<Grid<Element>> &Gridinstance)
{
    grids = Gridinstance; // Gridインスタンスの配列を代入
}

// シミュレーションの全体の実行
template <typename Element>
void Simulation<Element>::run(double dt, double endtime)
{
    // シミュレーションが終了するまでステップを繰り返し実行
    while (t < endtime)
    {
        runStep(); // 1ステップ実行
    }
}

// gridを取得
template <typename Element>
vector<Grid<Element>> &Simulation<Element>::getGrids()
{
    return grids;
}
#endif // SIMULATION_HPP
