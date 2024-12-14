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

    // // 指定位置のSEOオブジェクトを取得
    // shared_ptr<SEO> getSEO(const vector<int>& indices) const;

    // // 指定位置のSEOオブジェクトを設定
    // void setSEO(const vector<int>& indices, const shared_ptr<SEO>& seo);

    // // 周囲の接続を設定
    // void configureConnections();

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




// shared_ptr<SEO> Grid::getSEO(const vector<int>& indices) const {
//     return grid[toFlatIndex(indices)];
// }

// void Grid::setSEO(const vector<int>& indices, const shared_ptr<SEO>& seo) {
//     grid[toFlatIndex(indices)] = seo;
// }

// void Grid::configureConnections() {
//     int totalSize = grid.size();
//     for (int i = 0; i < totalSize; ++i) {
//         vector<int> indices(dimensions.size());
//         int temp = i;
//         for (int j = dimensions.size() - 1; j >= 0; --j) {
//             indices[j] = temp % dimensions[j];
//             temp /= dimensions[j];
//         }

//         vector<shared_ptr<SEO>> connections;
//         for (int d = 0; d < dimensions.size(); ++d) {
//             for (int offset : {-1, 1}) {
//                 vector<int> neighbor = indices;
//                 neighbor[d] += offset;
//                 if (neighbor[d] >= 0 && neighbor[d] < dimensions[d]) {
//                     connections.push_back(getSEO(neighbor));
//                 }
//             }
//         }
//         getSEO(indices)->setConnections(connections);
//     }
// }

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
