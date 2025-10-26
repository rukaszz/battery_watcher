# CMakeの最低バージョンの指定(最近の環境なら3.16以上がデフォルト)
cmake_minimum_required(VERSION 3.16)
# プロジェクト名と使用言語の宣言
project(battery_watcher LANGUAGES CXX)

# C++の標準をC++17に設定(必須化)
set(CMAKE_CXX_STANDARD 17)
# ここで必須化して17より前の標準ではビルドしないようにする
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6のパッケージと必要なモジュールの要求
# REQUIRED：見つからなかったらエラー
# COMPONENTS：必要なモジュールを列挙する
# Widget：GUIの部品とか
# Core：オブジェクト間のやり取りとかするやつ
# Gui：ウィンドウシステムとかのやつ
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

# Qt の自動処理を有効にする
set(CMAKE_AUTOMOC ON)

# 実行ファイルの定義と列挙
add_executable(battery_watcher
    src/main.cpp
    src/watcher.h src/watcher.cpp
    src/toast.h src/toast.cpp
    src/notifyutil.h src/notifyutil.cpp
    src/config.h src/config.cpp
    src/settingsdialog.h src/settingsdialog.cpp
)

# リンクするライブラリを指定(Widgetsを指定するだけでも良いらしい)
target_link_libraries(battery_watcher PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)

# Install target (optional)
# ビルドしたファイルをインストール先へ配置するための命令
install(TARGETS battery_watcher RUNTIME DESTINATION bin)

