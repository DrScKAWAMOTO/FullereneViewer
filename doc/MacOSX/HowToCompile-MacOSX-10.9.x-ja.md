# FullereneViewer(フラーレンビューア)
## コンパイル方法 (Mac OS X 10.9.x 編)
### 1) XQuartz 2.7.6 と Xcode 5.1.1 とコマンドライン開発ツール(command_line_tools_for_osx_10.9_september_2014)をインストールする。
### 2) Qt 5.3.2 をダウンロードする。
　<http://qt-users.jp/download.html> の「Qt 5.3.2 - Mac (456 MB)」をクリックして `qt-opensource-mac-x64-clang-5.3.2.dmg` をダウンロードする。

### 3) Qt 5.3.2 をインストールする。
　ダウンロードした `qt-opensource-mac-x64-clang-5.3.2.dmg` をダブルクリックして dmg ファイルを開き、qt-opensource-mac-x64-clang-5.3.2.app をダブルクリックする。

### 4) homebrew をインストールする。
　<http://brew.sh/index_ja.html> の「Install Homebrew」項目を参考に homebrew をインストールする。そしてターミナルで引き続き `brew update` と `brew doctor` を実行する。

### 5) povray をインストールする。
　一般ユーザーで `brew install povray` を実行する。

### 6) github からソースファイル取得
    home> git clone https://github.com/DrScKAWAMOTO/FullereneViewer
    home> cd FullereneViewer
    FullereneViewer> 

### 7) Launch `Qt Creator`
　`FullereneViewer/src/Qt/FullereneViewer.pro` をダブルクリックする。
　「Desktop Qt 5.3 clang 64bit」のデバッグとそのすぐ下のリリースのチェックマークを付け、一番下のリリースのチェックマークは外す。
　「プロジェクトの設定」ボタンを押す。

### 8) Build project "FullereneViewer"
　[ビルド] - [プロジェクト "FullereneViewer" をビルド] を選択する。

### 9) Debug project "FullereneViewer".
　動作します。

### 10) Execute `FullereneViewer`
　`FullereneViewer/src/build-FullereneViewer-Desktop_Qt_5_3_clang_64bit-Debug/FullereneViewer.app` をダブルクリックする。
　動作します。

### 11) フラーレンジェネレータ(ca-*)のメイク
    FullereneViewer> make
    FullereneViewer> 

### 12) お願い
　Mac の他の OS での動作実績があれば連絡ください。
