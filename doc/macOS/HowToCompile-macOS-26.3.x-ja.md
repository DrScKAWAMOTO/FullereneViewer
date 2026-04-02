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

### 7) ビルドの注意点（AGL フレームワーク問題の解消）
　以前のバージョンの Qt 6 (6.9.1 まで) では、最新の macOS SDK (SDK 26.2 / macOS 15.x 以降) で削除された古い `AGL.framework` を誤ってリンクしようとする不具合がありました。
　しかし、**最新の Qt 6.11.0 以降ではこの問題は解消されています**。

　もし古い Qt 6 環境でビルドが通らない場合は、`brew upgrade qt` を実行して最新の Qt 6.11.0 以上にアップデートしてください。
　本プロジェクトの `Makefile` および `FullereneViewer.pro` は、最新の Qt 6 環境に最適化されており、特別な修正なしでビルドが可能です。

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
