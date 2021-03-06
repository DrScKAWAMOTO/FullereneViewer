# FullereneViewer(フラーレンビューア)
## コンパイル方法 (Mac OS X 10.7.5 with MacPorts 2.3.0編)
### 1) Xcode 4.6.2 とコマンドライン開発ツールをインストールする。
### 2) MacPorts 2.3.0 をダウンロードする。
　<http://www.macports.org/install.php> の Quickstartから「OS X 10.7 Lion」をクリックして `MacPorts-2.3.0-10.7-Lion.pkg` をダウンロードする。

### 3) MacPorts 2.3.0 をインストールする。
　`MacPorts-2.3.0-10.7-Lion.pkg` をダブルクリックしてインストーラを実行する。

### 4) MacPorts で Qt 4.8.6 をインストールする。
    home> sudo port install qt4-mac

### 5) MacPorts で POV-Ray 3.7.0.0_1 をインストールする。
    home> sudo port install povray

### 6) github からソースファイル取得
    home> git clone https://github.com/DrScKAWAMOTO/FullereneViewer
    home> cd FullereneViewer
    FullereneViewer> 

### 7) コンパイル
#### 7-1) フラーレンビューア(FullereneViewer)
    FullereneViewer> make Qt
    FullereneViewer> 

　`Click folder FullereneViewer.xcodeproj by finder.`と表示されるが、生成された src/Qt/Makefile を使ってコマンドラインでビルドする。
    FullereneViewer> cd src/Qt
    Qt> make

#### 7-2) フラーレンジェネレータ(ca-*)
    FullereneViewer> make depend
    FullereneViewer> make
    FullereneViewer> 

### 8) 実行
　以下のいずれかの方法で実行できる。
* Finder から src/Qt/FullereneViewer を起動する
* コマンドラインから FullereneViewer.app を起動する
    FullereneViewer> open src/Qt/FullereneViewer.app
* コマンドラインから FullereneViewer を起動する
    FullereneViewer> ./src/Qt/FullereneViewer.app/Contents/MacOS/FullereneViewer

### 9) その他
　Xcode を使う方法として、7-1 の手順を以下に置き換えることが可能。

    FullereneViewer> cd src/Qt
    Qt> qmake -spec macx-xcode

　これにより、Xcode 用プロジェクトファイル FullereneViewer/src/Qt/FullereneViewer.xcodeproj が作成されるので、Xcode から開いてビルドすることが可能となる。  
　ただし、初めてプロジェクトファイルの Xcode から開いたとき、プロジェクトファイルのフォーマットを新しいフォーマットに変換するかの問い合わせがある。許可しない場合、ビルドは成功する。許可した場合、ヘッダファイル検索パスの設定が消失するためか、そのままではビルドできない。  
　また、Xcode から実行させると argv[1] と argv[2] に FullereneViewer が許容しない引数が指定されるため、FullereneViewer は usage を表示して終了していまう。
