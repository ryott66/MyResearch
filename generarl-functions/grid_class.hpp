#ifndef GRID_CLASS_HPP
#define GRID_CLASS_HPP

#include "seo_class.hpp"

// テンプレートを利用しているためファイル分割は無し
template <typename Element>

class Grid
{
private:
    vector<int> dimensions;           // グリッドの各次元のサイズ
    vector<shared_ptr<Element>> grid; // フラット化されたグリッドデータ
    double minwt;                     // gridごとの最小の待ち時間

    // 多次元インデックスを1次元インデックスに変換
    int toFlatIndex(const vector<int> &indices) const;

public:
    // コンストラクタ（指定する型、サイズで動的配列を確保）
    Grid(const vector<int> &dims);

    // indicesで指定した場所のオブジェクトを取得
    shared_ptr<Element> getElement(const vector<int> &indices) const;

    // indicesで指定した場所のオブジェクトを更新
    void setElement(const vector<int> &indices, const shared_ptr<Element> &element);

    // Gridインスタンス全体のVnを計算して更新
    void updateGridVn();

    // Gridインスタンス全体のdEを計算して更新
    void updateGriddE();

    // Gridインスタンス全体のwtを計算して比較、minwtに代入
    void gridminwt(const double dt);

    // Gridインスタンス全体のノード電荷を計算して更新
    void updateGridQn(const double dt);

    // フラット化されたデータの取得
    vector<shared_ptr<Element>> &getFlatGrid() const;

    // グリッドサイズを取得
    vector<int> getDimensions() const;
};

//-------------private----------------//

// 多次元インデックスを1次元インデックスに変換
template <typename Element>
int Grid<Element>::toFlatIndex(const vector<int> &indices) const
{
    if (indices.size() != dimensions.size())
    {
        throw invalid_argument("Invalid number of indices.");
    }
    int flatIndex = 0;
    int multiplier = 1;
    for (int i = dimensions.size() - 1; i >= 0; --i)
    {
        if (indices[i] < 0 || indices[i] >= dimensions[i])
        {
            throw out_of_range("Index out of bounds.");
        }
        flatIndex += indices[i] * multiplier;
        multiplier *= dimensions[i];
    }
    return flatIndex;
}

//-------------public-----------------//

// コンストラクタ（指定する型、サイズで動的配列を確保）
template <typename Element>
Grid<Element>::Grid(const vector<int> &dims) : dimensions(dims)
{
    if (dims.empty())
    {
        throw invalid_argument("Dimensions cannot be empty.");
    }
    int totalSize = 1;
    for (int dim : dims)
    {
        if (dim <= 0)
        {
            throw invalid_argument("All dimensions must be greater than zero.");
        }
        totalSize *= dim;
    }
    grid.resize(totalSize);
    for (int i = 0; i < totalSize; ++i)
    {
        grid.at(i) = make_shared<Element>();
    }
}

// indicesで指定した場所の要素を取得
template <typename Element>
shared_ptr<Element> Grid<Element>::getElement(const vector<int> &indices) const
{
    return grid[toFlatIndex(indices)];
}

// indicesで指定した場所の要素を更新
template <typename Element>
void Grid<Element>::setElement(const vector<int> &indices, const shared_ptr<Element> &element)
{
    grid[toFlatIndex(indices)] = element;
}

// Gridインスタンス全体のVnを計算して更新
template <typename Element>
void Grid<Element>::updateGridVn()
{
    // 周囲の電圧を自動的に設定
    for (auto &element : grid)
    {
        element->setSurroundingVoltages();
    }
    // ノード電圧の計算
    for (auto &element : grid)
    {
        element->setPcalc();
    }
}

// Gridインスタンス全体のdEを計算して更新
template <typename Element>
void Grid<Element>::updateGriddE()
{
    // エネルギー変化dEを計算
    for (auto &element : grid)
    {
        element->setdEcalc();
    }
}

// Gridインスタンス全体のwtを計算して比較、minwtに代入
template <typename Element>
void Grid<Element>::gridminwt(const double dt)
{
    minwt = dt;
    // トンネル待ち時間wtを計算、比較
    for (auto &element : grid)
    {
        // dEが正の場合はwtが計算されてtrueが返るため、if文が実行される
        if(element->calculateTunnelWt())
        {
            double tmpwt = 0;
            // wtは計算される場合以外は0であるため、大きい方が計算された側(wtは常に正)
            tmpwt = max(element->getWT("up"), element->getWT("down"));
            // 最小を更新
            minwt = min(minwt, tmpwt);
        }
    }
}

// Gridインスタンス全体のノード電荷を計算して更新
template <typename Element>
void Grid<Element>::updateGridQn(const double dt)
{
    // ノード電荷Qを計算
    for (auto &element : grid)
    {
        element->setNodeCharge(dt);
    }
}

// フラット化されたデータの取得
template <typename Element>
vector<shared_ptr<Element>> &Grid<Element>::getFlatGrid() const
{
    return grid;
}

// グリッドサイズを取得
template <typename Element>
vector<int> Grid<Element>::getDimensions() const
{
    return dimensions;
}

#endif // GRID_CLASS_HPP
