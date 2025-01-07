#ifndef SIMULATION_HPP
#define SIMULATION_HPP

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
    map<vector<shared_ptr<Element>>, string> tunnel; // トンネルを発生させる素子とトンネル方向
    vector<FILE *> fp;                               // 出力ファイルポインタのベクトル

public:
    // コンストラクタ（刻み時間とシミュレーション終了時間を入力）
    Simulation(double dT, double EndTime);

    // トンネルの処理
    void handleTunnels();

    // ファイル出力の処理
    void outputToFile() const;

    // シミュレーションの１ステップ
    void runStep();

    // Gridインスタンスの管理
    void addGrid(const vector<int> &dims);    // 新しいGridインスタンスを追加
    void removeGrid(int index);               // Gridインスタンスを削除

    // シミュレーションの全体の実行
    void run(double dt, double endtime);
};

//-------------public----------------//

// コンストラクタの定義
template <typename Element>
Simulation<Element>::Simulation(double dT, double EndTime)
    : t(0.0), dt(dT), endtime(EndTime)
{
    // 初期状態で1つのGridインスタンスを作成
    vector<int> defaultDims = {10, 10}; // デフォルトのサイズを設定
    grids.push_back(Grid<Element>(defaultDims)); // 最初のGridを追加
}

// トンネル処理
template <typename Element>
void Simulation<Element>::handleTunnels()
{
    // トンネル発生のロジックをここに追加
    // tunnelマップを使って、トンネルを発生させる素子を処理する
}

// ファイル出力の処理
template <typename Element>
void Simulation<Element>::outputToFile() const
{
    // ファイル出力処理をここに追加
    // fpを使って、必要なデータを出力ファイルに保存する
}

// シミュレーションの１ステップ
template <typename Element>
void Simulation<Element>::runStep()
{
    // シミュレーションの1ステップを実行
    // グリッドの更新、トンネル処理、出力などの処理を行う
    for (auto &grid : grids) {
        grid.updateGridVn();
        grid.updateGriddE();
        grid.updateGridQn(dt);
    }
    handleTunnels();
    outputToFile();
    t += dt; // 時間を進める
}

// Gridインスタンスの管理
template <typename Element>
void Simulation<Element>::addGrid(const vector<int> &dims)
{
    grids.push_back(Grid<Element>(dims)); // 新しいGridインスタンスを追加
}

template <typename Element>
void Simulation<Element>::removeGrid(int index)
{
    if (index >= 0 && index < grids.size()) {
        grids.erase(grids.begin() + index); // 指定したインデックスのGridを削除
    }
}

// シミュレーションの全体の実行
template <typename Element>
void Simulation<Element>::run(double dt, double endtime)
{
    // シミュレーションが終了するまでステップを繰り返し実行
    while (t < endtime) {
        runStep(); // 1ステップ実行
    }
}

#endif // SIMULATION_HPP
