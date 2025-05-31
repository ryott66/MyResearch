# bash内で.shファイルに書かれたコマンドを実行できる

#!/usr/bin/env bash

# 出力ディレクトリ作成
mkdir -p output
# 出力ファイルのリセット
rm -f output/*

# ビルドフォルダに移動
cd "$(dirname "$0")/build"

# CMakeで構成（初回またはCMakeLists.txt変更時だけ）
if [ ! -f Makefile ]; then
# -DBUILD_TESTING=ONだとテスト用ビルド込み。=OFFだとテスト用のビルドはしない
  cmake -G "MinGW Makefiles"  -DBUILD_TESTING=OFF .. || exit 1   # CMakeをインストールしてればcmakeコマンドでCMakelistを読み込み、Makefileなどを作る
fi

# makeでビルド
mingw32-make || exit 1   # makeコマンドで、Makefile等を読み込み、実際にg++コンパイラ等に適切に渡すことでビルド⇒今回でいうとMainApp.exeもここで作成

# 実行
./MainApp.exe
# ./UnitTests