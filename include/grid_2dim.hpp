#ifndef GRID_2DIM_HPP
#define GRID_2DIM_HPP

#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <string>

// 2次元グリッドで任意の素子（Element）を管理するテンプレートクラス
template <typename Element>
class Grid2D
{
public:
    // コンストラクタ：指定した行数・列数でグリッドを初期化
    Grid2D(int rows, int cols);

    // 指定位置の要素を取得
    std::shared_ptr<Element> getElement(int row, int col) const;

    // 指定位置の要素を設定
    void setElement(int row, int col, const std::shared_ptr<Element> &element);

    // グリッド全体のノード電圧Vnを更新
    void updateGridVn();

    // グリッド全体のエネルギー変化dEを計算・更新
    void updateGriddE();

    // グリッド全体のトンネル待ち時間wtを計算し、最小wtとトンネル素子を更新
    bool gridminwt(const double dt);

    // グリッド全体のノード電荷Qnを更新
    void updateGridQn(const double dt);

    // グリッド（全体の2次元vector）を取得
    std::vector<std::vector<std::shared_ptr<Element>>> &getGrid();

    // 行数を取得
    int numRows() const;

    // 列数を取得
    int numCols() const;

    // トンネルが発生する素子を取得
    std::shared_ptr<Element> getTunnelPlace() const;

    // トンネルの方向（"up" or "down"）を取得
    std::string getTunnelDirection() const;

    // 最小トンネル待ち時間wtを取得
    double getMinWT() const;

private:
    std::vector<std::vector<std::shared_ptr<Element>>> grid;
    int rows_, cols_;
    std::shared_ptr<Element> tunnelplace;
    std::string tunneldirection;
    double minwt;
};

// コンストラクタ：全要素をmake_sharedで初期化
template <typename Element>
Grid2D<Element>::Grid2D(int rows, int cols)
    : rows_(rows), cols_(cols), grid(rows, std::vector<std::shared_ptr<Element>>(cols))
{
    if (rows <= 0 || cols <= 0)
    {
        throw std::invalid_argument("Grid size must be positive");
    }
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            grid[i][j] = std::make_shared<Element>();
        }
    }
}

// 指定位置の要素を取得
template <typename Element>
std::shared_ptr<Element> Grid2D<Element>::getElement(int row, int col) const
{
    return grid.at(row).at(col);
}

// 指定位置の要素を設定
template <typename Element>
void Grid2D<Element>::setElement(int row, int col, const std::shared_ptr<Element> &element)
{
    grid.at(row).at(col) = element;
}

// グリッド全体のノード電圧Vnを計算・更新
template <typename Element>
void Grid2D<Element>::updateGridVn()
{
    for (auto &row : grid)
    {
        for (auto &elem : row)
        {
            elem->setSurroundingVoltages();
        }
    }

    for (auto &row : grid)
    {
        for (auto &elem : row)
        {
            elem->setPcalc();
        }
    }
}

// グリッド全体のエネルギー変化dEを計算・更新
template <typename Element>
void Grid2D<Element>::updateGriddE()
{
    for (auto &row : grid)
    {
        for (auto &elem : row)
        {
            elem->setdEcalc();
        }
    }
}

// グリッド全体のトンネル待ち時間wtを計算し、最小wtとトンネル素子・方向を記録
template <typename Element>
bool Grid2D<Element>::gridminwt(const double dt)
{
    minwt = dt;
    for (auto &row : grid)
    {
        for (auto &elem : row)
        {
            if (elem->calculateTunnelWt())
            {
                double tmpwt = std::max(elem->getWT()["up"], elem->getWT()["down"]);
                tunneldirection = (tmpwt == elem->getWT()["up"]) ? "up" : "down";
                tunnelplace = elem;
                minwt = std::min(minwt, tmpwt);
            }
        }
    }
    return minwt < dt;
}

// グリッド全体のノード電荷Qnを計算・更新
template <typename Element>
void Grid2D<Element>::updateGridQn(const double dt)
{
    for (auto &row : grid)
    {
        for (auto &elem : row)
        {
            elem->setNodeCharge(dt);
        }
    }
}

// グリッド全体のデータを取得
template <typename Element>
std::vector<std::vector<std::shared_ptr<Element>>> &Grid2D<Element>::getGrid()
{
    return grid;
}

// グリッドの行数を取得
template <typename Element>
int Grid2D<Element>::numRows() const
{
    return rows_;
}

// グリッドの列数を取得
template <typename Element>
int Grid2D<Element>::numCols() const
{
    return cols_;
}

// 最小wtでトンネルが発生する素子を取得
template <typename Element>
std::shared_ptr<Element> Grid2D<Element>::getTunnelPlace() const
{
    return tunnelplace;
}

// トンネルの方向を取得（"up" または "down"）
template <typename Element>
std::string Grid2D<Element>::getTunnelDirection() const
{
    return tunneldirection;
}

// 最小トンネル待ち時間wtを取得
template <typename Element>
double Grid2D<Element>::getMinWT() const
{
    return minwt;
}

#endif // GRID_2DIM_HPP
