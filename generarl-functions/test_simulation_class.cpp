#include <gtest/gtest.h>
#include <stdexcept>
#include "seo_class.hpp"

// GoogleTestのSetUpをオーバーライドしてパラメータのセットアップ
class SEOTest : public ::testing::Test
{
protected:
    unique_ptr<vector<vector<vector<shared_ptr<SEO>>>>> seoGrid;

    void SetUp() override
    {
        int x = 2, y = 2, z = 2;

        // 3次元ベクトル (サイズ: 2x2x2) を動的に生成
        seoGrid = make_unique<vector<vector<vector<shared_ptr<SEO>>>>>(
            z, vector<vector<shared_ptr<SEO>>>(y, vector<shared_ptr<SEO>>(x)));

        // 各要素を生成して格納
        for (int i = 0; i < z; ++i)
        {
            for (int j = 0; j < y; ++j)
            {
                for (int k = 0; k < x; ++k)
                {
                    (*seoGrid).at(i).at(j).at(k) = make_shared<SEO>(1.0, 0.001, 18.0, 2.0, 0.007, 3);
                }
            }
        }

        // [0][0][0]の振動子に[1][0][0],[0][1][0],[0][0][1]を接続
        vector<shared_ptr<SEO>> connections = {
            (*seoGrid).at(1).at(0).at(0),
            (*seoGrid).at(0).at(1).at(0),
            (*seoGrid).at(0).at(0).at(1)};
        (*seoGrid).at(0).at(0).at(0)->setConnections(connections);
    }
};