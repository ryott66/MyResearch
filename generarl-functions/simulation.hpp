#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "seo_class.hpp"

class Simulation
{
private:
    int sx;                                      // x軸方向のサイズ
    int sy;                                      // y軸方向のサイズ
    int sz;                                      // z軸方向のサイズ
    double t;                                    // 現在のシミュレーション時間[ns]
    double dt;                                   // 刻み時間[ns]
    double endtime;                              // シミュレーションの終了時間[ns]
    map<vector<shared_ptr<SEO>>, string> tunnel; // トンネルを発生させる素子とトンネル方向
    vector<FILE *> fp;                           // 出力ファイルポインタのベクトル

public:
//     Simulation(double vds, double rj, double r, double cj, double c)
//         : t(0.0), st(T_TRG - 1)
//     {
//         initialize(vds, rj, r, cj, c);
//     }

//     void initialize(double vds, double rj, double r, double cj, double c)
//     {
//         grid.resize(tate, std::vector<SEO>(yoko, SEO(r, rj, cj, c, vds)));
//         char filename[] = "outside.dat";
//         if (fopen_s(&fp, filename, "w") != 0)
//         {
//             throw std::runtime_error("Failed to open output file.");
//         }
//     }

//     double makerand()
//     {
//         return static_cast<double>(rand()) / RAND_MAX;
//     }

    // シミュレーションの全体の実行
    void run(double dt, double endtime);
//     {
//         srand(static_cast<unsigned>(time(nullptr)));
//         while (t <= T_END)
//         {
//             step();
//         }
//         fclose(fp);
//     }

//     void step()
//     {
//         double Tmin = dt;
//         int tempx = -1, tempy = -1, sflag = 0;

//         if (t >= T_TRG && st == T_TRG - 1)
//         {
//             grid[0][1].updateCharge(0.02);
//         }

//         for (int y = 1; y < tate - 1; ++y)
//         {
//             for (int x = 1; x < yoko - 1; ++x)
//             {
//                 auto &node = grid[y][x];

//                 node.setSurroundingVoltages({
//                     grid[y][x - 1].getVoltage(),
//                     grid[y - 1][x].getVoltage(),
//                     grid[y][x + 1].getVoltage(),
//                     grid[y + 1][x].getVoltage(),
//                 });

//                 node.calculateNodeVoltage();

//                 double dEup = node.calculateEnergyChange(true);
//                 double dEdown = node.calculateEnergyChange(false);

//                 double r = makerand();
//                 if (dEup > 0 || dEdown > 0)
//                 {
//                     double Ganma = std::min(dEup, dEdown) / (e * e * 0.002);
//                     double T = (1 / Ganma) * std::log(1 / r);
//                     if (T < Tmin)
//                     {
//                         Tmin = T;
//                         tempx = x;
//                         tempy = y;
//                         sflag = (dEup < dEdown) ? 1 : 2;
//                     }
//                 }
//             }
//         }

//         if (Tmin != dt && tempx != -1 && tempy != -1)
//         {
//             grid[tempy][tempx].tunnel(
//                 grid[tempy][tempx].calculateEnergyChange(true),
//                 grid[tempy][tempx].calculateEnergyChange(false),
//                 sflag);
//         }

//         t += Tmin;
//     }
};

#endif // SIMULATION_HPP
