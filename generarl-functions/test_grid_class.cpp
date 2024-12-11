#include <gtest/gtest.h>
#include <stdexcept>
#include "grid_class.hpp"
#include "seo_class.hpp"

// GoogleTestのSetUpをオーバーライドしてパラメータのセットアップ
class GridTest : public ::testing::Test
{
protected:

    void SetUp() override
    {
       
    }
};

// テストケース: コンストラクタの動作確認
TEST_F(GridTest, Constructor)
{
    // 正常な次元指定
    Grid grid({3, 4});
    EXPECT_EQ(grid.getDimensions(), vector<int>({3, 4}));

    // 無効な次元指定（空）
    EXPECT_THROW(Grid({}), invalid_argument);

    // 無効な次元指定（負の値）
    EXPECT_THROW(Grid({-1, 3}), invalid_argument);
}