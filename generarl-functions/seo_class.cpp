#include "seo_class.hpp"
//------ コンストラクタ（パラメータの初期設定）---------//
// 初期値無し
SEO::SEO() : R(0), Rj(0), Cj(0), C(0), Vd(0), Q(0), Vn(0), legs(0), V_sum(0)
{
    dE["up"] = 0.0;
    dE["down"] = 0.0;
    wt["up"] = 0.0;
    wt["down"] = 0.0;
}

// 初期値あり
SEO::SEO(double r, double rj, double cj, double c, double vd, int legscounts)
    : R(r), Rj(rj), Cj(cj), C(c), Vd(vd), Q(0.0), Vn(0.0), legs(legscounts),
      V_sum(0.0), connection(0)
{
    dE["up"] = 0.0;
    dE["down"] = 0.0;
    wt["up"] = 0.0;
    wt["down"] = 0.0;
}

//-----------セッター------------//
// パラメータセットアップ
void SEO::setUp(double r, double rj, double cj, double c, double vd, int legscounts)
{
    R = r;
    Rj = rj;
    Cj = cj;
    C = c;
    Vd = vd;
    legs = legscounts;
}

// バイアス電圧を設定
void SEO::setVias(const double vd)
{
    Vd = vd;
}

// 接続情報を設定
void SEO::setConnections(const vector<shared_ptr<SEO>> &connectedSEOs)
{
    connection.clear();
    if (connectedSEOs.size() > legs)
    {
        throw invalid_argument("The size of connections must match the number of legs.");
    }
    for (const auto &seo : connectedSEOs)
    {
        if (this == seo.get())
        {
            throw invalid_argument("Cannot connect to itself.");
        }
        connection.push_back(seo);
    }
}

// 周囲の電圧を設定
void SEO::setSurroundingVoltages()
{
    V_sum = 0;
    for (auto seo : connection)
    {
        V_sum += seo->Vn;
    }
}

// 振動子のパラメータ計算
void SEO::setPcalc()
{
    Vn = Q / Cj + (C / (Cj * (legs * C + Cj))) * (Cj * V_sum - legs * Q);
}

// 振動子のエネルギー計算
void SEO::setdEcalc()
{
    dE["up"] = -e * (e - 2 * (Q + C * V_sum)) / (2 * (legs * C + Cj));
    dE["down"] = -e * (e + 2 * (Q + C * V_sum)) / (2 * (legs * C + Cj));
}

// 電荷の更新
void SEO::setNodeCharge(const double dt)
{
    Q += (Vd - Vn) * dt / R;
}

// トンネル待ち時間計算(upまたはdownが正の時にwtを計算してtrueを返す)
bool SEO::calculateTunnelWt()
{
    // 初期化
    wt["up"] = 0;
    wt["down"] = 0;
    if (dE["up"] > 0)
    {
        wt["up"] = (e * e * Rj / dE["up"]) * log(1 / Random());
        return true;
    }
    if (dE["down"] > 0)
    {
        wt["down"] = (e * e * Rj / dE["down"]) * log(1 / Random());
        return true;
    }
    return false;
}

// 振動子のトンネル
void SEO::setTunnel(const string direction)
{
    if (direction == "up")
    {
        Q += -e;
    }
    else if (direction == "down")
    {
        Q += e;
    }
    else
    {
        throw invalid_argument("Invalid tunnel direction");
    }
}
//-----------ゲッター------------//

// ノード電圧を取得
double SEO::getVn() const
{
    return Vn;
}

// 接続されてる振動子を取得
vector<shared_ptr<SEO>> SEO::getConnection() const
{
    return connection;
}

// 接続されてる振動子の電圧の総和を取得
double SEO::getSurroundingVsum() const
{
    return V_sum;
}

// dEの取得
map<string, double> SEO::getdE() const
{
    return dE;
}

// Qの取得
double SEO::getQ() const
{
    return Q;
}

// wtの取得
map<string, double> SEO::getWT() const
{
    return wt;
}

//-------- 汎用処理 -------------//
// 0から1の間の乱数を生成
double SEO::Random()
{
    static random_device rd;
    static mt19937 mt(rd());
    static uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(mt);
}

//-------- テスト用 -----------//
// テスト用Rゲッター
double SEO::getR() const
{
    return R;
}

// テスト用Rjゲッター
double SEO::getRj() const
{
    return Rj;
}

// テスト用Cjゲッター
double SEO::getCj() const
{
    return Cj;
}

// テスト用Cゲッター
double SEO::getC() const
{
    return C;
}

// テスト用Vdゲッター
double SEO::getVd() const
{
    return Vd;
}

// テスト用legsゲッター
int SEO::getlegs() const
{
    return legs;
}

// テスト用dEセッター
void SEO::setdE(const string &direction, double value)
{
    dE[direction] = value;
}

// テスト用Vnセッター
void SEO::setVn(double vn)
{
    Vn = vn;
}

// テスト用Qnセッター
void SEO::setQ(double qn)
{
    Q = qn;
}