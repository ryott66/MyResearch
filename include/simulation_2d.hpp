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
// #include "output_class.hpp"
#include "tsp_methods.hpp"

#include <iostream>
#include <string>
#include <algorithm>

template <typename Element>
class Simulation2D
{
private:
    double t;                           // 現在の時間（不定期に増える）
    double dt;                          // 基本刻み（参考値）
    double xt;                          // 計算calcLを行う時間の管理
    double endtime;                     // 終了時刻
    double outputInterval;              // 出力間隔（例: 0.1）
    double nextOutputTime;              // 次に出力すべき時刻（0.1, 0.2, ...）
    std::vector<Grid2D<Element>> grids; // Grid2Dのインスタンス配列
    // oyl-video形式のデータ
    std::map<
        std::string,                                  // ラベル名
        std::vector<std::vector<std::vector<double>>> // [timeframe][y][x]
        >
        outputs;
    // トリガを表すベクトル（どのgridか、時刻、位置、値)
    std::vector<std::tuple<Grid2D<Element>*,double, int, int, double>> voltageTriggers; // (grid, time, x, y, V)
    // ファイル出力する素子をファイル名とともに格納するベクトル
    // std::vector<std::pair<std::ofstream*, std::shared_ptr<Element>>> selectedElements;
    // 複数素子を1つのファイルに出力するための構造
    std::vector<std::pair<std::shared_ptr<std::ofstream>, std::vector<std::shared_ptr<Element>>>> selectedElements;

    Calculate_NN CalcNN; //ニューラルネットワークの計算をシミュレーション内で行うためのクラス

public:
    // コンストラクタ(刻み時間,シミュレーションの終了タイミング)
    Simulation2D(double dT, double EndTime);

    // wtの比較（gridごと）
    std::pair<bool, std::shared_ptr<Grid2D<Element>>> comparewt();

    // トンネルの処理
    void handleTunnels(Grid2D<Element> &tunnelgrid);

    // ファイル作成
    void openFiles() const;

    // ファイル閉じる
    void closeFiles() const;

    // ファイル出力
    void outputToFile();

    // oyl-video形式に合わせた出力を生成
    void outputTooyl();

    // シミュレーションの1ステップ
    void runStep();

    // シミュレーションにgridを追加
    void addGrid(const std::vector<Grid2D<Element>> &Gridinstance);

    // シミュレーションの実行
    void run();

    //到達回数の表示
    void printCt();

    // グリッド取得
    std::vector<Grid2D<Element>> &getGrids();

    // outputsを取得
    const std::map<std::string, std::vector<std::vector<std::vector<double>>>> &getOutputs() const;

    // トリガーを追加する
    void addVoltageTrigger(double triggerTime, Grid2D<Element>* grid, int y, int x, double voltage);

    // トリガを適用させる
    void applyVoltageTriggers();

    //Vdの変更
    void applychangeVd();

    // ファイル出力する素子とファイルを格納するベクトルに追加する
    void addSelectedElements(std::shared_ptr<std::ofstream> ofs, const std::vector<std::shared_ptr<Element>>& elems);

    // selectedElementsから該当する素子のVnを記録するファイル出力
    void outputSelectedElements();

    // gnuplot用データ出力
    void generateGnuplotScript(const std::string& dataFilename, const std::vector<std::string>& labels);

    // 実行中の進捗状況を表示する
    void printProgressBar();

    //結果表示ファイル
    void writeresFile();

};


// コンストラクタ
template <typename Element>
Simulation2D<Element>::Simulation2D(double dT, double EndTime)
    : t(0.0), dt(dT), xt(0.0), endtime(EndTime), outputInterval(dT), nextOutputTime(0.0), CalcNN(Cost) {}

// 最小wtをもつgridを探索する（最小wtがdtより小さいかどうかのbool, 最小のwtを持つgrid）
template <typename Element>
std::pair<bool, std::shared_ptr<Grid2D<Element>>> Simulation2D<Element>::comparewt()
{
    double minwt = dt;
    std::shared_ptr<Grid2D<Element>> tunnelgrid = nullptr;
    for (auto &grid : grids)
    {
        if (grid.gridminwt(dt))
        {
            double candidate = grid.getMinWT();
            if (candidate < minwt)
            {
                minwt = candidate;
                tunnelgrid = std::make_shared<Grid2D<Element>>(grid);
            }
        }
    }
    if (minwt < dt) return {true, tunnelgrid};
    return {false, nullptr};
}


// トンネル処理を実行
template <typename Element>
void Simulation2D<Element>::handleTunnels(Grid2D<Element> &tunnelgrid)
{
    auto ptr = tunnelgrid.getTunnelPlace();
    //----------------トンネル場所の記録--------------------------------
    auto [y, x] = tunnelgrid.getPositionOf(ptr);
    if (((x + 1) % tenthlane == 0 ) && (y % (WideLane + 1) == CenterLane)){  //レーン長XSIZEを10分割して各場所に到達したらカウント+1
        CalcNN.Ctvk[y / (WideLane + 1)] += 1;
        if (CalcNN.Nvk[y / (WideLane + 1)] < 30){
            CalcNN.Nvk[y / (WideLane + 1)] += 1;
        }else{
            //NR
        }
    }
    std::ofstream log("../output/tunnel_log.txt", std::ios::app);
    if (log.is_open()) {
        log << "t=" << t
            << ", x=" << x
            << ", y=" << y
            << ", dir=" << tunnelgrid.getTunnelDirection()
            << ", grid=" << tunnelgrid.getOutputLabel()
            << std::endl;
    }
    //-----------------------------------------------------------------
    // 実際のトンネル処理
    ptr->setTunnel(tunnelgrid.getTunnelDirection());
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

// template <typename Element>
// void Simulation2D<Element>::addSelectedElement(std::ofstream& ofs, std::shared_ptr<Element> element)
// {
//     selectedElements.emplace_back(&ofs, element);
// }
template <typename Element>
void Simulation2D<Element>::addSelectedElements(std::shared_ptr<std::ofstream> ofs, const std::vector<std::shared_ptr<Element>>& elems)
{
    selectedElements.emplace_back(ofs, elems);
}



// template <typename Element>
// void Simulation2D<Element>::outputSelectedElements()
// {
//     for (auto& [ofsPtr, elemPtr] : selectedElements)
//     {
//         // pair型で格納したファイルと素子の情報からファイルに出力する
//         if (!ofsPtr || !(*ofsPtr) || !elemPtr)
//             // 情報がない場合はcontinue
//             continue;
//         (*ofsPtr) << t << " " << elemPtr->getVn() << endl;
//     }
// }
template <typename Element>
void Simulation2D<Element>::outputSelectedElements()
{
    for (auto& [ofsPtr, elemPtrs] : selectedElements)
    {
        if (!ofsPtr || !(*ofsPtr)) continue;  // ファイルが開けていない場合はスキップ

        (*ofsPtr) << t;
        for (const auto& elemPtr : elemPtrs)
        {
            if (elemPtr)
                (*ofsPtr) << " " << elemPtr->getVn();
            else
                (*ofsPtr) << " nan";  // nullポインタの場合の保険
        }
        (*ofsPtr) << std::endl;
    }
}

// oyl-video形式に合わせた出力を生成
// 1 <= x <= max-1, 1 <= y <= max-1の範囲で出力される(sizex=32,sizey=32の場合は1から31までの範囲で30×30になる)
template <typename Element>
void Simulation2D<Element>::outputTooyl()
{
    if (t >= nextOutputTime)
    {
        // 出力形式に合わせて整数値にならす
        int timeframe = static_cast<int>(std::round(nextOutputTime / outputInterval));
        int outputIndex = 0; // 出力順にindex付けするカウンタ

        for (const auto &grid : grids)
        {
            if (!grid.isOutputEnabled())
                continue;

            std::string label;
            if (grid.hasOutputLabel())
            {
                label = grid.getOutputLabel();
            }
            else
            {
                label = "../output" + std::to_string(outputIndex);
                ++outputIndex;
            }

            int rows = grid.numRows();
            int cols = grid.numCols();

            std::vector<std::vector<double>> vnGrid(rows - 2, std::vector<double>(cols - 2));
            for (int i = 1; i < rows - 1; ++i)
            {
                for (int j = 1; j < cols - 1; ++j)
                {
                    auto elem = grid.getElement(i, j);
                    double vn = elem->getVn();
                    double vd = elem->getVd();
            
                    // Vdが負のとき、Vnを反転して記録
                    if (vd < 0) {
                        vn *= -1.0;
                    }
            
                    vnGrid[i-1][j-1] = vn;
                }
            }            

            outputs[label].resize(timeframe + 1);
            outputs[label][timeframe] = vnGrid;
        }
        nextOutputTime += outputInterval;
    }
}

// シミュレーションの1ステップを実行
template <typename Element>
void Simulation2D<Element>::runStep()
{
    double steptime = dt;

    // oyl-video形式に出力
    outputTooyl();
    // ファイル出力
    outputSelectedElements();


    // grid全体のVn計算(5回計算してならす)
    for (int i = 0; i < 5; i++)
    {
        for (auto &grid : grids)
        {
            // 接続されている電圧を更新
            grid.updateGridSurVn();
            // トリガの適用
            applyVoltageTriggers();
            // 電圧を更新
            grid.updateGridVn();
        }
    }

    // grid全体のdE計算
    for (auto &grid : grids)
    {    
        // if(t > 150 && t < 151) cout << "t = " << t << " dE = " << grid.getElement(15,15)->getdE()["up"] << endl;
        grid.updateGriddE();
    }



    // wtの計算と比較
    auto compared = this->comparewt();
    if (compared.first)
    {
        handleTunnels(*compared.second);
        steptime = compared.second->getMinWT();
    }

    // チャージの計算
    for (auto &grid : grids)
    {
        grid.updateGridQn(steptime);
    }

    // tの増加
    t += steptime;
    xt += steptime;
}

// Gridインスタンスの配列を登録
template <typename Element>
void Simulation2D<Element>::addGrid(const std::vector<Grid2D<Element>> &Gridinstance)
{
    grids = Gridinstance;
}

// 全体シミュレーションの実行
template <typename Element>
void Simulation2D<Element>::run()
{

    while (t < endtime)
    {
        if (calcL_time < xt){  //calcL_time毎にcalcLを行う
            CalcNN.calcL();
            applychangeVd();

            xt = 0;
        }

        runStep();
        printProgressBar();
    }
}


// 到達回数表示
template <typename Element>
void Simulation2D<Element>::printCt() {
    for (int ct : CalcNN.Ctvk) {
        std::cout << ct << "ct " << std::endl;
    }
}

// グリッド取得
template <typename Element>
std::vector<Grid2D<Element>> &Simulation2D<Element>::getGrids()
{
    return grids;
}

template <typename Element>
const std::map<std::string, std::vector<std::vector<std::vector<double>>>> &Simulation2D<Element>::getOutputs() const
{
    return outputs;
}

template <typename Element>
void Simulation2D<Element>::addVoltageTrigger(double triggerTime, Grid2D<Element>* grid, int y, int x, double voltage) {
    voltageTriggers.emplace_back(grid, triggerTime, y, x, voltage);
}

template <typename Element>
void Simulation2D<Element>::applyVoltageTriggers()
{
    for (const auto& [gridPtr, triggerTime, y, x, voltage] : voltageTriggers) {
        if (t >= triggerTime && t < triggerTime + dt * 10) {
            if (!gridPtr) {
                throw std::invalid_argument("Trigger references a null grid pointer.");
            }

            if (x < 0 || x >= gridPtr->numCols() || y < 0 || y >= gridPtr->numRows()) {
                throw std::out_of_range(
                    "Trigger coordinates (x=" + std::to_string(x) +
                    ", y=" + std::to_string(y) + ") are out of grid bounds (" +
                    std::to_string(gridPtr->numCols()) + "x" + std::to_string(gridPtr->numRows()) + ")."
                );
            }

            auto elem = gridPtr->getElement(y, x);
            elem->setVsum(elem->getSurroundingVsum() + voltage);
        }
    }
}

template <typename Element>
void Simulation2D<Element>::applychangeVd(){
    double biasVd = 0;
    std::shared_ptr<Element> chseo;
    //経路幅調整
    for (auto &grid : grids)
    {    
        for(int vk = 0; vk < N2 ; vk++){  //Lane x    
            if(CalcNN.Lvk[vk] > 0.5){//光照射アリ⇒経路狭く
                for(int x = 2; x < size_x; x++){
                    for(int y = 1; y <= ((CenterLane) - ((NarrowLane + 1) / 2)); y++){//ややこしいけど多分あってる
                        // Center下側
                        biasVd = (((vk * (WideLane + 1) + y) + x) % 2 == 0) ? 0.003 : -0.003;
                        //applyVd
                        chseo = grid.getElement((vk * (WideLane + 1) + y), x);
                        chseo->setVias(biasVd);

                        // Center上側
                        biasVd = ((((vk + 1) * (WideLane + 1) - y) + x) % 2 == 0) ? 0.003 : -0.003;
                        chseo = grid.getElement(((vk + 1) * (WideLane + 1) - y), x);
                        chseo->setVias(biasVd);
                    }
                }
            }else{
                for(int x = 2; x < size_x; x++){
                    for(int y = 1; y <= ((CenterLane) - ((NarrowLane + 1) / 2)); y++){//ややこしいけど多分あってる
                        // Center下側
                        biasVd = (((vk * (WideLane + 1) + y) + x) % 2 == 0) ? Vd : -Vd;
                        //applyVd
                        chseo = grid.getElement((vk * (WideLane + 1) + y), x);
                        chseo->setVias(biasVd);

                        // Center上側
                        biasVd = ((((vk + 1) * (WideLane + 1) - y) + x) % 2 == 0) ? Vd : -Vd;
                        chseo = grid.getElement(((vk + 1) * (WideLane + 1) - y), x);
                        chseo->setVias(biasVd);
                    }
                }
            }  
        }
    }
}

//一般関数（下のファイル出力名に使う）
std::string sanitizeTimeString(const std::string& raw) {
    // ファイル名で使えない文字（スペースやコロン）をアンダースコアに置換
    std::string s = raw;
    std::replace(s.begin(), s.end(), ' ', '_');
    std::replace(s.begin(), s.end(), ':', '_');
    return s;
}

/*
//出力ファイル
template <typename Element>
void writeresFile() {
    constexpr const char* buildDate = __DATE__;  // 例: "May 31 2025"
    constexpr const char* buildTime = __TIME__;  // 例: "11:45:23"

    std::string dateStr = sanitizeTimeString(buildDate);
    std::string timeStr = sanitizeTimeString(buildTime);

    std::string filename = "output/res_" + dateStr + "_" + timeStr + ".txt";

    std::ofstream result(filename);
    if (!result) {
        std::cerr << "ファイル作成に失敗しました: " << filename << std::endl;
        return;
    }


    //結果出力
    for(int i = 0; i < N2; i++){
        if(i % N == 0){
            result << "\n";
        }
        result << NAME[i/N] << (i % N) + 1 << ct[i];
    }
    result << "\n\n";

    int max[N][2]={0};
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(max[i][0] < ct[(i*N) + j]){
                max[i][0] = ct[(i*N) + j];
                max[i][1] = j;
            }else{
                //NR
            }
        }
    }

    int sort[N] = {0};
    for(int x = 0; x < N; x++){  //iとかjとかややこしいのでx
        sort[max[x][1]] = x;   //0はsort[max[0]], 1はsort[max[1]],...となる（ややこしかった）
    }

    for(int i=0;i<N;i++){
        if(i!=N-1){
            result << NAME[sort[i]] << "⇒ ";
        }else{
            result << NAME[sort[i]] << "\n\n";
        }
    }

    int sumcost=0;
    int small,large=0;
    for(int i=0;i<N;i++){ //コストの合計を求める。もし4都市でABCDだったら、AB,BC,CDの3通りのコストを足す。

        if(sort[i%N]<sort[(i+1)%N]){ //costarrayの仕様上小さいほうを決めなきゃいけない。（ＢＡではなくＡＢのように）
            small=sort[i%N];  //%Nにしているのは、i=N-1のとき、(i+1)%Nが0になって、巡回できるようになってる。
            large=sort[(i+1)%N];
        }else{
            small=sort[(i+1)%N];
            large=sort[i%N];
        }
        sumcost+=costarray[small][large];
    }

    result << "合計コスト : " << sumcost << "\n\n\n";

    for(int i=0;i<N2;i++){
        if(Lvk[i]<0.5){
            result << "Lvk[" << i << "]" << NAME[i/N] << (i%N)+1 << Lvk[i] << "\n";
        }
    }

    result.close();

    std::cout << "ファイル作成: " << filename << std::endl;
}
*/


template <typename Element>
void Simulation2D<Element>::generateGnuplotScript(const std::string& dataFilename, const std::vector<std::string>& labels)
{
    std::string scriptFilename = dataFilename.substr(0, dataFilename.find_last_of('.')) + "_gnu.txt";
    std::ofstream gnuFile(scriptFilename);
    if (!gnuFile.is_open()) {
        std::cerr << "[ERROR] Failed to create gnuplot script: " << scriptFilename << std::endl;
        return;
    }
    // 空のラベルがあるかチェック
    for (size_t i = 0; i < labels.size(); ++i) {
        if (labels[i].empty()) {
            std::cerr << "[ERROR] Label for index " << i << " is empty. Cannot generate gnuplot script.\n";
            return;
        }
    }

    gnuFile << "#unset key\n";
    gnuFile << "#set title 'no'\n";
    gnuFile << "set terminal qt font \"Arial,10\"\n";
    gnuFile << "set xl 't[ns]'\n";
    gnuFile << "set yl 'V[V]'\n";
    gnuFile << "\n";

    gnuFile << "p ";
    for (size_t i = 0; i < labels.size(); ++i) {
        gnuFile << "'" << dataFilename << "' u 1:" << (i + 2) << " title '" << labels[i] << "' w steps lw 3";
        if (i != labels.size() - 1)
            gnuFile << ",\\\n  ";
    }
    gnuFile << "\n";

    std::cout << "[INFO] Gnuplot script generated: " << scriptFilename << std::endl;
}

// 実行中の進捗状況を表示する
template <typename Element>
void Simulation2D<Element>::printProgressBar(){
    static int last_stage = -1;
    int stage = static_cast<int>((t / endtime) * 10);  // 0〜10（10%刻み）

    if (stage != last_stage) {
        last_stage = stage;
        int percent = stage * 10;

        std::string bar = "[";
        for (int i = 0; i < 10; ++i)
            bar += (i < stage) ? "=" : " ";
        bar += "]";

        std::cout << "[INFO] Simulation progress: " << bar
                  << " " << percent << "%" << std::endl;
    }
}

#endif // SIMULATION_2D_HPP