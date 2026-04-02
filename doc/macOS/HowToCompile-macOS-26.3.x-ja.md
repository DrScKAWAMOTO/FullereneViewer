# FullereneViewer(フラーレンビューア)
## コンパイル方法 (macOS 26.3.x 編)
### 1) XQuartz 2.8.5 と Xcode 26.x (SDK 26.2) のコマンドラインツールをインストールする。

### 2) homebrew をインストールする。
　<http://brew.sh/index_ja.html> の「Install Homebrew」項目を参考に homebrew をインストールする。そしてターミナルで引き続き `brew update` と `brew doctor` を実行する。

### 3) Qt をインストールする。
　一般ユーザーで `brew install qt` を実行する。

### 4) Qt を使えるようにする。
　一般ユーザーで `brew unlink qt@5` と `brew link qt` を実行する。
　これにより、最新の Qt 6 系が使用されるようになります。

### 5) povray をインストールする。
　一般ユーザーで `brew install povray` を実行する。

### 6) github からソースファイルを取得する。
    home> git clone https://github.com/DrScKAWAMOTO/FullereneViewer
    home> cd FullereneViewer
    FullereneViewer> 

### 7) ビルドの注意点（AGL フレームワーク問題への対応）
　最新の macOS SDK (SDK 26.2 / macOS 15.x 以降) では、古い `AGL.framework` が完全に削除されています。
　しかし、Qt 6 (特に qmake 3.1) が生成する `Makefile` は、依然として `-framework AGL` をリンクしようとする不具合があります。
　本プロジェクトでは、この問題を回避するために以下の対応を行っています。

*   `src/Makefile` 内で、`qmake` 実行直後に `sed` コマンドを使用して `Makefile` から `-framework AGL` を自動的に除去するようにしました。
*   `src/Qt/FullereneViewer.pro` 内で、`QMAKE_LIBS_OPENGL` を直接指定し、可能な限りリンクエラーを防ぐようにしました。

　したがって、以下の手順通り `make` を実行するだけで、この問題を意識することなくビルドが完了します。

### 8) FullereneViewer をビルドする。
　トップディレクトリ、または `src/` ディレクトリに移動し、`make` を実行してください。
    FullereneViewer> make
    
　これにより、`ca-*` ツール群と、`src/Qt/FullereneViewer.app` が一括でビルドされます。

### 9) `FullereneViewer` を実行する。
　`FullereneViewer/src/Qt/FullereneViewer.app` をダブルクリックする。
　動作します。
　必要に応じて FullereneViewer.app を好きなフォルダに移動してください。

### 10) povray の設定
　`FullereneViewer/src/Qt/FullereneViewer.app` をダブルクリックする。
　メニュー [ファイル] - [設定] を開いて povray コマンドのパスを修正する。

### 11) お願い
　Mac の他の OS での動作実績があれば連絡ください。
