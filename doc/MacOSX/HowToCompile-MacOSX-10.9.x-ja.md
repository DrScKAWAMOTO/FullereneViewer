# FullereneViewer(フラーレンビューア)
## コンパイル方法 (Mac OS X 10.9.x 編)
### 1) XQuartz 2.7.6 と Xcode 5.1.1 (とコマンドライン開発ツール?)をインストールする。
### 2) Qt 5.3.1 をダウンロードする。
　<http://qt-users.jp/download.html> の「Qt 5.3.1 - Mac (453 MB)」をクリックして `qt-opensource-mac-x64-clang-5.3.1.dmg` をダウンロードする。

### 3) Qt 5.3.1 をインストールする。
　ダウンロードした `qt-opensource-mac-x64-clang-5.3.1.dmg` をダブルクリックして dmg ファイルを開き、qt-opensource-mac-x64-clang-5.3.1.app をダブルクリックする。

### 4) homebrew をインストールする。
　<http://brew.sh/index_ja.html> の「Install Homebrew」項目を参考に homebrew をインストールしてください。
　そしてターミナルで引き続き `brew update` と `brew doctor` を実行して下さい。

### 5) povray をインストールする。
　一般ユーザーで `brew install povray` を実行する。

### 6) github からソースファイル取得
    home> git clone https://github.com/DrScKAWAMOTO/FullereneViewer
    home> cd FullereneViewer
    FullereneViewer> 

### 7) Launch `Qt Creator`
　`FullereneViewer/src/Qt/FullereneViewer.pro` をダブルクリックする。
　「プロジェクトの設定」ボタンを押す。

### 8) Build project "FullereneViewer"
　[ビルド] - [プロジェクト "FullereneViewer" をビルド] を選択する。

### 9) Debug project "FullereneViewer".
　動作します。

### 10) Execute `FullereneViewer`
　`FullereneViewer/src/build-FullereneViewer-Desktop_Qt_5_3_clang_64bit-Debug/FullereneViewer.ap` をダブルクリックする。
　動作します。

### 11) フラーレンジェネレータ(ca-*)のメイク
    FullereneViewer> cd src
    src> make depend
    src> make
    src> 

### 12) お願い
　Mac の他の OS での動作実績があれば連絡ください。
