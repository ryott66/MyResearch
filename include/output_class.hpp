#ifndef OUTPUT_CLASS_HPP
#define OUTPUT_CLASS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <sstream>

#include<grid_class.hpp>

using namespace std;

template <typename Element>
class Outputdata
{
private:
    string filename;              // 出力ファイル名
    vector<vector<Element>> data; // 二次元vectorに格納されたデータ
    ofstream outputFile;          // ファイルストリーム
    Grid<Element> outputdata;     // 出力するGridインスタンス（２次元上で定義されたgrid）

public:
    // コンストラクタ
};

// -----------private---------------

// -----------public------------------
#endif // OUTPUT_CLASS_HPP