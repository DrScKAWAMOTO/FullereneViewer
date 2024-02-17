# FullereneViewer(フラーレンビューア)
## コンパイル方法 (macOS 14.3.x 編)
### 1) XQuartz 2.8.5 と Xcode 15.2 のコマンドラインツールをインストールする。

### 2) homebrew をインストールする。
　<http://brew.sh/index_ja.html> の「Install Homebrew」項目を参考に homebrew をインストールする。そしてターミナルで引き続き `brew update` と `brew doctor` を実行する。

### 2) Qt@5 をインストールする。
　一般ユーザーで `brew install qt` を実行する。

### 3) Qt@5 を使えるようにする。
　一般ユーザーで `brew unlink qt@5` と `brew link qt` を実行する。

### 4) povray をインストールする。
　一般ユーザーで `brew install povray` を実行する。

### 5) github からソースファイルを取得する。
    home> git clone https://github.com/DrScKAWAMOTO/FullereneViewer
    home> cd FullereneViewer
    FullereneViewer> 

### 6) qmake で Makefile を作成する。
　一般ユーザーで `cd FullereneViewer/src/Qt; qmake FullereneViewer.pro` とする。

### 7) FullereneViewer.app をビルドする。
　一般ユーザーで `make` とする。

### 8) `FullereneViewer` を実行する。
　`FullereneViewer/src/Qt/FullereneViewer.app` をダブルクリックする。
　動作します。
　必要に応じて FullereneViewer.app を好きなフォルダに移動してください。

### 9) フラーレンジェネレータ(ca-*)のメイク
    FullereneViewer> make
    FullereneViewer> 

### 10) povray の設定
　`FullereneViewer/src/Qt/FullereneViewer.app` をダブルクリックする。
　メニュー [ファイル] - [設定] を開いて povray コマンドのパスを修正する。

### 11) お願い
　Mac の他の OS での動作実績があれば連絡ください。
