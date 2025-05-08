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
#include "base_element.hpp"
#include "constants.hpp"

class SEO : public BaseElement {
private:
    double Q;               // ノード電荷
    double Vn;              // ノード電圧
    double Vd;              // バイアス電圧
    double R;               // 抵抗
    double Rj;              // トンネル抵抗
    double Cj;              // 接合容量
    double C;               // 接続容量
    int legs;               // 足の数
    double V_sum;           // 周囲のノード電圧の総和
    std::map<std::string, double> dE; // エネルギー変化量(up, down)
    std::map<std::string, double> wt; // トンネル待時間(up, down)
    // std::vector<std::shared_ptr<SEO>> connection; // 接続されている素子のポインタ
    std::vector<std::shared_ptr<BaseElement>> connections;  // 接続されている素子のポインタ

public:
    //-----------コンストラクタ---------// 
    // vectorの初期化用
    SEO();
    // 引数あり初期設定用
    SEO(double r, double rj, double cj, double c, double vd, int legscounts);

    //-----------セッター------------//
    // パラメータセットアップ
    void setUp(double r, double rj, double cj, double c, double vd, int legscounts);

    // バイアス電圧を設定
    void setVias(const double vd);

    // V_sumを設定
    void setVsum(double v) override;

    // 接続情報を設定
    // void setConnections(const std::vector<std::shared_ptr<SEO>> &connectedSEOs);

    void setConnections(const std::vector<std::shared_ptr<BaseElement>>& conns) override;

    // 周囲の電圧を設定
    void setSurroundingVoltages() override;

    // 振動子のパラメータ計算
    void setPcalc() override;

    // 振動子のエネルギー計算
    void setdEcalc() override;

    // 電荷の更新
    void setNodeCharge(const double dt) override;

    // トンネル待ち時間計算(upまたはdownが正の時にwtを計算してtrueを返す)
    bool calculateTunnelWt() override;

    // 振動子のトンネル
    void setTunnel(const std::string& direction) override;

    //-----------ゲッター------------//    
    // ノード電圧を取得
    double getVn() const override;

    // // 接続されてる振動子を取得
    // std::vector<std::shared_ptr<SEO>> getConnection() const;

    // 接続されてる振動子の電圧の総和を取得
    double getSurroundingVsum() const override;

    // dEの取得
    std::map<std::string, double> getdE() const;

    // Qの取得
    double getQ() const;

    // wtの取得
    std::map<std::string, double> getWT() const override;

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

    // Vdゲッター
    double getVd() const override;
    
    // テスト用legsゲッター
    int getlegs() const;

    // テスト用dEセッター
    void setdE(const std::string& direction, double value);

    // テスト用Vnセッター
    void setVn(double vn);

    // テスト用Qnセッター
    void setQ(double qn);
};

#endif // SEO_HPP
