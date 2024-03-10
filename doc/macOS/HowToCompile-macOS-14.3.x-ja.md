# FullereneViewer(フラーレンビューア)
## コンパイル方法 (macOS 14.3.x 編)
### 1) XQuartz 2.8.5 と Xcode 15.2 のコマンドラインツールをインストールする。

### 2) homebrew をインストールする。
　<http://brew.sh/index_ja.html> の「Install Homebrew」項目を参考に homebrew をインストールする。そしてターミナルで引き続き `brew update` と `brew doctor` を実行する。

### 2) Qt をインストールする。
　一般ユーザーで `brew install qt` を実行する。

### 3) Qt を使えるようにする。
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

### 12) Qt6 におけるバグ

フラーレンの形状シミュレーションが止まる(形が変わらなくなる)と、描画をやめてし
まうバグあり。
タイトルバーの D が消えると同時に描画も止まる。
Qt5 では問題は発生しない。（タイトルバーの D が消えても描画されたまま）

Qt6.6 のドキュメントには QOpenGLWidget::paintGL() では

```c
Before invoking this function, the context and the framebuffer are bound,
and the viewport is set up by a call to glViewport().
No other state is set and no clearing or drawing is performed by the framework.
```

と書いてあったが、paintGL() 呼び出し直前では glClear() が呼び出され済みのような
状態になっている。描画実施直前の自前の glClear() を呼ぶのを止めても、フレームの
描画が重なることはなかった！！

対策として、Qt5 およびそれ以前で実施していた以下の実装を Qt6 では廃止することに
した。タイトルバーの D は永久的に表示されたままとなり、その結果、上記問題は起こ
らなくなった。

Qt5 およびそれ以前で実施していた実装とは、以下の条件を全て満たした時は描画しない
というものである。

1. シミュレーションが止まった。
2. ぐるぐる回していない。
3. ピッキングを実施していない。
