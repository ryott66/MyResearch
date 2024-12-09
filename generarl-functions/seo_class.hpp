#ifndef SEO_CLASS_HPP
#define SEO_CLASS_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <stdexcept>
#include <random>
#include <memory>

constexpr double e = 0.1602; // 電子の電荷量


using namespace std;

class SEO {
private:
    double Q;               // ノード電荷
    double Vn;              // ノード電圧
    double Vd;              // バイアス電圧
    double R;               // 抵抗
    double Rj;              // トンネル抵抗
    double Cj;              // 接合容量
    double C;               // 接続容量
    int legs;               // 足の数
    vector<double> V;       // 周囲のノード電圧
    map<string, double> dE; // エネルギー変化量(up, down)
    map<string, double> wt; // トンネル待時間(up, down)
    vector<shared_ptr<SEO>> connection; // 接続されている素子のポインタ

public:
    //-----------コンストラクタ---------// 
    // vectorの初期化用
    SEO();
    // 引数あり初期設定用
    SEO(double r, double rj, double cj, double c, double vd, int legscounts);

    //-----------セッター------------//
    // バイアス電圧を設定
    void setVias(const double vd);

    // 接続情報を設定
    void setConnections(const vector<shared_ptr<SEO>> &connectedSEOs);

    // 周囲の電圧を設定
    void setSurroundingVoltages();

    // 振動子のパラメータ計算
    void setPcalc();

    // 振動子のエネルギー計算
    void setdEcalc();

    // 電荷の更新
    void setNodeCharge(const double dt);

    // トンネル待ち時間計算
    void calculateTunnelWt();

    // 振動子のトンネル
    void setTunnel(const string direction);

    //-----------ゲッター------------//    
    // ノード電圧を取得
    double getVn() const;

    // 接続されてる振動子を取得
    vector<shared_ptr<SEO>> getConnection() const;

    // 接続されてる振動子の電圧を取得
    vector<double> getSurroundingVoltages() const;

    // dEの取得
    map<string, double> getdE() const;

    // Qの取得
    double getQ() const;

    //-------- 汎用処理 -------------//
    // 0から1の間の乱数を生成
    double Random();

    //-------- テスト用 -------------//
    // テスト用idCounterゲッター
    int getidCounter() const;

    // テスト用Rゲッター
    double getR() const;
    
    // テスト用Rjゲッター
    double getRj() const;
    
    // テスト用Cjゲッター
    double getCj() const;
    
    // テスト用Cゲッター
    double getC() const;

    // テスト用Vdゲッター
    double getVd() const;
    
    // テスト用legsゲッター
    int getlegs() const;

    // テスト用dEセッター
    void setdE(const string& direction, double value);

    // テスト用Vnセッター
    void setVn(double vn);

    // テスト用Qnセッター
    void setQ(double qn);

    // テスト用wtゲッター
    double getWT(const string& direction) const;
    
};

#endif // SEO_HPP
