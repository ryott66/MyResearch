#ifndef GRID_CLASS_HPP
#define GRID_CLASS_HPP

#include "seo_class.hpp"

class Grid {
private:
    vector<int> dimensions; // グリッドの各次元のサイズ
    vector<shared_ptr<SEO>> grid; // フラット化されたグリッドデータ

    // 多次元インデックスを1次元インデックスに変換
    int toFlatIndex(const vector<int>& indices) const;

public:
    // コンストラクタ
    Grid(const vector<int>& dims);

    // 指定位置のSEOオブジェクトを取得
    shared_ptr<SEO> getSEO(const vector<int>& indices) const;

    // 指定位置のSEOオブジェクトを設定
    void setSEO(const vector<int>& indices, const shared_ptr<SEO>& seo);

    // 周囲の接続を設定
    void configureConnections();

    // グリッド全体のSEOオブジェクトを更新
    void updateGrid(double dt);

    // グリッドサイズを取得
    vector<int> getDimensions() const;
};

#endif // GRID_CLASS_HPP
