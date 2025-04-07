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
class MockElement
{
private:
    int value;

public:
    MockElement() : value(0) {}
    MockElement(int v) : value(v) {}

    int getValue() const { return value; }
    void setValue(int v) { value = v; }
};

// テストフィクスチャ
class GridTest : public ::testing::Test
{
protected:
    vector<int> dimensions = {2, 3}; // 2x3のグリッドを作成
    Grid<MockElement> *grid;

    void SetUp() override
    {
        grid = new Grid<MockElement>(dimensions);
    }

    void TearDown() override
    {
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
TEST_F(GridTest, GetElementTest)
{
    vector<int> indices = {1, 2};

    // 初期化時にはデフォルトコンストラクタの値を返す
    auto element = grid->getElement(indices);
    ASSERT_EQ(element->getValue(), 0) << "Element should be initialized to 0.";
}

// setElementとgetElementの連携テスト
TEST_F(GridTest, SetAndGetElementTest)
{
    vector<int> indices = {0, 1};

    auto newElement = make_shared<MockElement>(42); // 新しい要素をセット
    grid->setElement(indices, newElement);

    auto retrievedElement = grid->getElement(indices);
    ASSERT_EQ(retrievedElement->getValue(), 42) << "Element should return the set value.";
}

// インデックス範囲外のテスト
TEST_F(GridTest, OutOfRangeIndexTest)
{
    vector<int> invalidIndices = {2, 3}; // 範囲外のインデックス

    EXPECT_THROW(grid->getElement(invalidIndices), out_of_range);
    EXPECT_THROW(grid->setElement(invalidIndices, make_shared<MockElement>(10)), out_of_range);
}

// Vn
TEST_F(GridTest, VnCalcTest)
{
    double dt = 0.1;
    int sx = 31, sy = 31;
    vector<int> dimensions = {sx, sy}; // 31x31のグリッドを作成
    Grid<SEO> seogrid(dimensions);     // 指定したサイズのseo型の配列を用意
    for (int y = 0; y < sy; y++)
    {
        for (int x = 0; x < sx; x++)
        {
            // 指定した位置のパラメータを設定
            vector<int> indices = {x, y};
            // 上下左右の振動子を接続
            vector<int> indices_u;
            vector<int> indices_r;
            vector<int> indices_d;
            vector<int> indices_l;
            // 偶数
            if ((x + y) % 2 == 0)
            {
                seogrid.getElement(indices)->setUp(1.0, 0.001, 12.0, 2.0, 0.004, 4);
                seogrid.getElement(indices)->setVn(0.003);
                // cout << seogrid.getElement(indices)->getR() << endl;
                indices_u = {1, 2};
                indices_r = {2, 1};
                indices_d = {1, 0};
                indices_l = {0, 1};
            }
            // 奇数
            else
            {
                seogrid.getElement(indices)->setUp(1.0, 0.001, 12.0, 2.0, -0.004, 4);
                seogrid.getElement(indices)->setVn(-0.003);
                indices_u = {1, 3};
                indices_r = {2, 2};
                indices_d = {1, 1};
                indices_l = {0, 2};
            }
            // 上下左右の振動子を接続
            // vector<int> indices_u = {x, y + 1};
            // vector<int> indices_r = {x + 1, y};
            // vector<int> indices_d = {x, y - 1};
            // vector<int> indices_l = {x - 1, y};
            vector<shared_ptr<SEO>> connections = {
                seogrid.getElement(indices_u),
                seogrid.getElement(indices_r),
                seogrid.getElement(indices_d),
                seogrid.getElement(indices_l)};
            seogrid.getElement(indices)->setConnections(connections);
        }
    }
    vector<int> indices = {0, 1};
    double beforeVn = seogrid.getElement(indices)->getVn();
    seogrid.updateGridVn();
    double afterVn = seogrid.getElement(indices)->getVn();
    ASSERT_NE(beforeVn, afterVn);
    // 指定した位置のパラメータを設定
    cout << "Vn: " << beforeVn << endl;
    cout << "Vd: " << seogrid.getElement(indices)->getVd() << endl;
    cout << "Vn: " << seogrid.getElement(indices)->getVn() << endl;
    seogrid.updateGriddE();
    cout << "dE[up]: " << seogrid.getElement(indices)->getdE()["up"] << endl;
    cout << "Q: " << seogrid.getElement(indices)->getQ() << endl;
    seogrid.updateGridQn(0.1);
    cout << "Qn: " << seogrid.getElement(indices)->getQ() << endl;
    seogrid.gridminwt(dt);
    cout << "wt[up]: " << seogrid.getElement(indices)->getWT()["up"] << endl;

    indices = {1, 1};
    cout << "Vd: " << seogrid.getElement(indices)->getVd() << endl;
    cout << "Vn: " << seogrid.getElement(indices)->getVn() << endl;
    seogrid.updateGriddE();
    cout << "dE[down]: " << seogrid.getElement(indices)->getdE()["down"] << endl;
    cout << "Q: " << seogrid.getElement(indices)->getQ() << endl;
    seogrid.updateGridQn(0.1);
    cout << "Qn: " << seogrid.getElement(indices)->getQ() << endl;
    seogrid.gridminwt(dt);
    cout << "wt[down]: " << seogrid.getElement(indices)->getWT()["down"] << endl;
    seogrid.getElement(indices)->setdE("down", 1);
    seogrid.gridminwt(dt);
    cout << "wt[down]: " << seogrid.getElement(indices)->getWT()["down"] << endl;
    ASSERT_GT(seogrid.getElement(indices)->getWT()["down"], 0);
}

// dE

// charge