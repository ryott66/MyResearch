#ifndef GRID_CLASS_HPP
#define GRID_CLASS_HPP

#include "seo_class.hpp"

// テンプレートを利用しているためファイル分割は無し
template <typename Element>

class Grid {
private:
    vector<int> dimensions; // グリッドの各次元のサイズ
    vector<shared_ptr<Element>> grid; // フラット化されたグリッドデータ

    // 多次元インデックスを1次元インデックスに変換
    int toFlatIndex(const vector<int>& indices) const;

public:
    // コンストラクタ（指定する型、サイズで動的配列を確保）
    Grid(const vector<int>& dims);

    // 指定位置のSEOオブジェクトを取得
    shared_ptr<Element> getElement(const vector<int>& indices) const;

    // 指定位置のSEOオブジェクトを設定
    void setElement(const vector<int>& indices, const shared_ptr<Element>& element);

    // // グリッド全体のSEOオブジェクトを更新
    // void updateGrid(double dt);

    // フラット化されたデータの取得
    vector<shared_ptr<Element>>& getFlatGrid() const;

    // グリッドサイズを取得
    vector<int> getDimensions() const;
};
//-------------private----------------//
// 多次元インデックスを1次元インデックスに変換
template <typename Element>
int Grid<Element>::toFlatIndex(const vector<int>& indices) const {
    if (indices.size() != dimensions.size()) {
        throw invalid_argument("Invalid number of indices.");
    }
    int flatIndex = 0;
    int multiplier = 1;
    for (int i = dimensions.size() - 1; i >= 0; --i) {
        if (indices[i] < 0 || indices[i] >= dimensions[i]) {
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
Grid<Element>::Grid(const vector<int>& dims) : dimensions(dims) {
    if (dims.empty()) {
        throw invalid_argument("Dimensions cannot be empty.");
    }
    int totalSize = 1;
    for (int dim : dims) {
        if (dim <= 0) {
            throw invalid_argument("All dimensions must be greater than zero.");
        }
        totalSize *= dim;
    }
    grid.resize(totalSize);
    for (int i = 0; i < totalSize; ++i) {
        grid.at(i) = make_shared<Element>();
    }
}

template <typename Element>
shared_ptr<Element> Grid<Element>::getElement(const vector<int>& indices) const {
    return grid[toFlatIndex(indices)];
}

template <typename Element>
void Grid<Element>::setElement(const vector<int>& indices, const shared_ptr<Element>& element) {
    grid[toFlatIndex(indices)] = element;
}

// void Grid::updateGrid(double dt) {
//     for (auto& seo : grid) {
//         seo->setNodeCharge(dt);
//     }
// }

// フラット化されたデータの取得
template <typename Element>
vector<shared_ptr<Element>>& Grid<Element>::getFlatGrid() const { 
    return grid;
}

// グリッドサイズを取得
template <typename Element>
vector<int> Grid<Element>::getDimensions() const {
    return dimensions;
}

#endif // GRID_CLASS_HPP
