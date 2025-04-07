#include <gtest/gtest.h>
#include "simulation.hpp"
#include "grid_class.hpp"
#include "seo_class.hpp"

// ここでは仮のElementクラスを定義
class Element {
public:
    int value;
    // 引数ありコンストラクタ
    Element(int val) : value(val) {}
    
    // デフォルトコンストラクタを追加
    Element() : value(0) {}  // 初期値を設定するか、必要に応じて適切に変更
};

// SimulationクラスとGridクラスのテスト
class SimulationTest : public ::testing::Test {
protected:
    // シミュレーションのセットアップ
    Simulation<Element> sim;

    SimulationTest() : sim(1.0, 10.0) {} // dT=1.0, EndTime=10.0で初期化
};

// addGridメソッドのテスト
TEST_F(SimulationTest, AddGridTest) {
    // 追加するGridインスタンスを作成
    vector<int> dims = {5, 5};  // 5x5の次元のGrid
    Grid<Element> grid(dims);

    // Gridインスタンスを格納したベクトルを作成
    vector<Grid<Element>> Grids;
    Grids.push_back(grid);  // Gridインスタンスをベクトルに追加

    // addGridメソッドを実行
    sim.addGrid(Grids);

    // gridsに新しく追加されたGridが格納されているか確認
    ASSERT_EQ(sim.getGrids().size(), 1);  // 新しいGridが1つ追加されていることを確認
    ASSERT_EQ(sim.getGrids()[0].getDimensions()[0], 5);  // 追加したGridの次元を確認
    ASSERT_EQ(sim.getGrids()[0].getDimensions()[1], 5);  // 追加したGridの次元を確認
}
