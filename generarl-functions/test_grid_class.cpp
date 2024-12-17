#include <gtest/gtest.h>
#include <stdexcept>
#include "grid_class.hpp"
#include "seo_class.hpp"

// // GoogleTestのSetUpをオーバーライドしてパラメータのセットアップ
// class GridTest : public ::testing::Test
// {
// protected:
//     void SetUp() override
//     {
//     }
// };

// Mock用のElementクラス
class MockElement {
private:
    int value;

public:
    MockElement() : value(0) {}
    MockElement(int v) : value(v) {}

    int getValue() const { return value; }
    void setValue(int v) { value = v; }
};

// テストフィクスチャ
class GridTest : public ::testing::Test {
protected:
    vector<int> dimensions = {2, 3}; // 2x3のグリッドを作成
    Grid<MockElement> *grid;

    void SetUp() override {
        grid = new Grid<MockElement>(dimensions);
    }

    void TearDown() override {
        delete grid;
    }
};

// テストケース: 振動子でのコンストラクタの動作確認
TEST_F(GridTest, Constructor)
{
    // 正常な次元指定
    Grid<SEO> grid1({3, 4});
    EXPECT_EQ(grid1.getDimensions(), vector<int>({3, 4}));

    // 正常な次元指定
    Grid<SEO> grid2({1, 2, 3, 4, 5});
    EXPECT_EQ(grid2.getDimensions(), vector<int>({1, 2, 3, 4, 5}));

    // 無効な次元指定（空）
    EXPECT_THROW(Grid<SEO>({}), invalid_argument);

    // 無効な次元指定（負の値）
    EXPECT_THROW(Grid<SEO>({-1, 3}), invalid_argument);
}

// getElementのテスト
TEST_F(GridTest, GetElementTest) {
    vector<int> indices = {1, 2};

    // 初期化時にはデフォルトコンストラクタの値を返す
    auto element = grid->getElement(indices);
    ASSERT_EQ(element->getValue(), 0) << "Element should be initialized to 0.";
}

// setElementとgetElementの連携テスト
TEST_F(GridTest, SetAndGetElementTest) {
    vector<int> indices = {0, 1};

    auto newElement = make_shared<MockElement>(42); // 新しい要素をセット
    grid->setElement(indices, newElement);

    auto retrievedElement = grid->getElement(indices);
    ASSERT_EQ(retrievedElement->getValue(), 42) << "Element should return the set value.";
}

// インデックス範囲外のテスト
TEST_F(GridTest, OutOfRangeIndexTest) {
    vector<int> invalidIndices = {2, 3}; // 範囲外のインデックス

    EXPECT_THROW(grid->getElement(invalidIndices), out_of_range);
    EXPECT_THROW(grid->setElement(invalidIndices, make_shared<MockElement>(10)), out_of_range);
}