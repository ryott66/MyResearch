#!/usr/bin/env bash

# ビルドフォルダに移動
cd "$(dirname "$0")/build"

# CMakeで構成（初回またはCMakeLists.txt変更時だけ）
if [ ! -f Makefile ]; then
  cmake -G "MinGW Makefiles"  -DBUILD_TESTING=ON .. || exit 1
fi

# makeでビルド
mingw32-make || exit 1

# 実行
./MainApp.exe
./UnitTests