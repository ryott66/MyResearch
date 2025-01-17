#ifndef PRINTDATA_CLASS_HPP
#define PRINTDATA_CLASS_HPP

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
class PrintData
{
private:
    string filename;              // 出力ファイル名
    vector<vector<Element>> data; // 二次元vectorに格納されたデータ
    ofstream outputFile;          // ファイルストリーム

    // 現在の日時を文字列として取得する関数
    string getCurrentTimeAsString();

public:
    // コンストラクタ
    PrintData(const string &baseFilename);

    // ファイルを開く
    void openFile();

    // ファイルを閉じる
    void closeFile();

    // データの設定
    void updateData(const vector<vector<Element>> &newData);

    // データをファイルに書き出す
    // ほかの出力をしたい場合は関数を引数として渡すことによって行う
    void writeToFile2dim() const;
};

// -----------private---------------

// 現在の日時を文字列として取得する関数
template <typename Element>
string PrintData<Element>::getCurrentTimeAsString()
{
    auto now = chrono::system_clock::now();
    auto now_t = chrono::system_clock::to_time_t(now);
    tm tm = *localtime(&now_t);
    ostringstream oss;
    oss << put_time(&tm, "%Y%m%d_%H%M%S");
    return oss.str();
}

// -----------public------------------

// コンストラクタ
template <typename Element>
PrintData<Element>::PrintData(const string &baseFilename)
{
    // ファイル名に現在の日時を追加
    filename = baseFilename + "_" + getCurrentTimeAsString() + ".txt";
}

// ファイルを開く
template <typename Element>
void PrintData<Element>::openFile()
{
    outputFile.open(filename, ios::out); // ファイルのモード（出力）
    if (!outputFile.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
    }
}

// ファイルを閉じる
template <typename Element>
void PrintData<Element>::closeFile()
{
    if (outputFile.is_open())
    {
        outputFile.close();
    }
}

// データの設定
template <typename Element>
void PrintData<Element>::updateData(const vector<vector<Element>> &newData)
{
    data = newData; // 新しいデータで上書き
}

// データをファイルに書き出す
// ほかの出力をしたい場合は関数を引数として渡すことによって行う
template <typename Element>
void PrintData<Element>::writeToFile2dim() const
{
    if (!outputFile.is_open())
    {
        cerr << "File is not open" << endl;
        return;
    }

    for (int x = 0; x < data.size() - 1; x++)
    {
        for (int y = 0; y < data.at(x).size(); y++)
        {
            if ((x + y) % 2 == 0)
            {
                if (y == 0)
                {
                    outputFile << x << " " << y << " " << data.at(y + 1).at(x + 1).getVn() << endl;
                }
                else if (y == data.at(x).size() - 1)
                {
                    outputFile << x << " " << y << " " << -data.at(y).at(x + 1).getVn() << endl; // abs
                }
                else
                {
                    outputFile << x << " " << y << " " << -data.at(y).at(x + 1).getVn() << endl;
                    outputFile << x << " " << y << " " << data.at(y + 1).at(x + 1).getVn() << endl;
                }
            }
            else
            {
                if (y == 0)
                {
                    outputFile << x << " " << y << " " << -data.at(y + 1).at(x + 1).getVn() << endl;
                }
                else if (y == data.at(x).size() - 1)
                {
                    outputFile << x << " " << y << " " << data.at(y).at(x + 1).getVn() << endl;
                }
                else
                {
                    outputFile << x << " " << y << " " << data.at(y).at(x + 1).getVn() << endl;
                    outputFile << x << " " << y << " " << -data.at(y + 1).at(x + 1).getVn() << endl;
                }
            }
        }

        outputFile << endl;

        for (int y = 0; y < data.at(x).size(); y++)
        {
            if ((x + y) % 2 == 0)
            {
                if (y == 0)
                {
                    outputFile << x + 1 << " " << y << " " << data.at(y + 1).at(x + 1).getVn() << endl;
                }
                else if (y == data.at(x).size() - 1)
                {
                    outputFile << x + 1 << " " << y << " " << -data.at(y).at(x + 1).getVn() << endl;
                }
                else
                {
                    outputFile << x + 1 << " " << y << " " << -data.at(y).at(x + 1).getVn() << endl;
                    outputFile << x + 1 << " " << y << " " << data.at(y + 1).at(x + 1).getVn() << endl;
                }
            }
            else
            {
                if (y == 0)
                {
                    outputFile << x + 1 << " " << y << " " << -data.at(y + 1).at(x + 1).getVn() << endl;
                }
                else if (y == data.at(x).size() - 1)
                {
                    outputFile << x + 1 << " " << y << " " << data.at(y).at(x + 1).getVn() << endl;
                }
                else
                {
                    outputFile << x + 1 << " " << y << " " << data.at(y).at(x + 1).getVn() << endl;
                    outputFile << x + 1 << " " << y << " " << -data.at(y + 1).at(x + 1).getVn() << endl;
                }
            }
        }
        outputFile << endl;
    }
    outputFile << endl;
}
#endif // PRINTDATA_CLASS_HPP
