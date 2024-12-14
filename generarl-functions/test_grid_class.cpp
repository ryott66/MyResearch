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