#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <fstream>
#include "seo_class.hpp"
#include "grid_class.hpp"
#include "printdata_class.hpp"

// テンプレートを利用しているためファイル分割は無し
template <typename Element>

class Simulation
{
private:
    double t;                                        // 現在のシミュレーション時間[ns]
    double dt;                                       // 刻み時間[ns]
    double endtime;                                  // シミュレーションの終了時間[ns]
    double outputInterval;                           // ファイルに出力する時間間隔(0.1)
    double accumulatedTime;                          // ファイル出力ごとの累積時間
    vector<Grid<Element>> grids;                     // 複数のGridインスタンスを保持するベクトル
    vector<PrintData<Element>> printdatavector;      // 出力するデータを保持するベクトル

public:
    // コンストラクタ（刻み時間とシミュレーション終了時間を入力）
    Simulation(double dT, double EndTime);

    // wtの比較
    pair<bool, shared_ptr<Grid<Element>>> comparewt();

    // トンネルの処理
    void handleTunnels(Grid<Element> &tunnelelement);

    // ファイルを開く
    void openFiles() const;

    // ファイルを閉じる
    void closeFiles() const;

    // 出力するデータの２次元配列を入力
    void closeFiles() const;

    // ファイル出力の処理
    void outputToFile() const;

    // シミュレーションの１ステップ
    void runStep();

    // Gridインスタンスの管理
    void addGrid(const vector<Grid<Element>> &Gridinstance); // 新しいGridインスタンスを追加

    // シミュレーションの全体の実行
    void run(double dt, double endtime);

    // テスト用
    // gridを取得
    vector<Grid<Element>> &getGrids();
};

//-------------public----------------//

// コンストラクタの定義
template <typename Element>
Simulation<Element>::Simulation(double dT, double EndTime) : t(0.0), dt(dT), 
endtime(EndTime),outputInterval(0.1), accumulatedTime(0.0) {}

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

// ファイルを開く
template <typename Element>
void Simulation<Element>::openFiles() const
{
    for (auto &outputdata : printdatavector)
    {
        outputdata.openFile();
    }
}

// ファイルを閉じる
template <typename Element>
void Simulation<Element>::closeFiles() const
{
    for (auto &outputdata : printdatavector)
    {
        outputdata.closeFile();
    }
}

// ファイル出力の処理
template <typename Element>
void Simulation<Element>::outputToFile() const
{
    if(accumulatedTime >= outputInterval){
        accumulatedTime -= outputInterval
    }
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
    accumulatedTime += steptime; // 累積時間
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
    // ファイルを開く
    openFiles();
    // シミュレーションが終了するまでステップを繰り返し実行
    while (t < endtime)
    {
        runStep(); // 1ステップ実行
    }
    // ファイルを閉じる
    closeFiles();
}

// gridを取得
template <typename Element>
vector<Grid<Element>> &Simulation<Element>::getGrids()
{
    return grids;
}
#endif // SIMULATION_HPP
