# FullereneViewer
## フラーレンビューア
### コンパイル方法 (Mac OS X 10.7.5 with MacPorts 2.3.0編)
#### 1) Xcode 4.6.2 とコマンドライン開発ツールをインストールする。
#### 2) MacPorts 2.3.0 をダウンロードする。
　<http://www.macports.org/install.php> の Quickstartから「OS X 10.7 Lion」をクリックして `MacPorts-2.3.0-10.7-Lion.pkg` をダウンロードする。

#### 3) MacPorts 2.3.0 をインストールする。
　`MacPorts-2.3.0-10.7-Lion.pkg` をダブルクリックしてインストーラを実行する。

#### 4) MacPorts で Qt 4.8.6 をインストールする。
    home> sudo port install qt4-mac

#### 5) MacPorts で POV-Ray 3.7.0.0_1 をインストールする。
    home> sudo port install povray

#### 6) github からソースファイル取得
    home> git clone https://github.com/DrScKAWAMOTO/FullereneViewer
    home> cd FullereneViewer
    FullereneViewer> 

#### 7) コンパイル
##### 7-1) フラーレンビューア(fl-guruguru)
    FullereneViewer> cd src
    src> make Qt

　`Click folder fl-guruguru.xcodeproj by finder.`と表示されるが、生成された Qt/Makefile を使ってコマンドラインでビルドする。
    src> cd Qt
    Qt> make

##### 7-2) フラーレンジェネレータ(ca-*)
    FullereneViewer> cd src
    src> make depend
    src> make
    src> 

#### 8) 実行
　以下のいずれかの方法で実行できる。
* Finder から src/Qt/fl-guruguru を起動する
* コマンドラインから fl-guruguru.app を起動する
    FullereneViewer> open src/Qt/fl-guruguru.app
* コマンドラインから fl-guruguru を起動する
    FullereneViewer> ./src/Qt/fl-guruguru.app/Contents/MacOS/fl-guruguru

#### 9) その他
　Xcode を使う方法として、7-1 の手順を以下に置き換えることが可能。

    FullereneViewer> cd src
    src> cd Qt
    Qt> qmake -spec macx-xcode

　これにより、Xcode 用プロジェクトファイル FullereneViewer/src/Qt/fl-guruguru.xcodeproj が作成されるので、Xcode から開いてビルドすることが可能となる。  
　ただし、初めてプロジェクトファイルの Xcode から開いたとき、プロジェクトファイルのフォーマットを新しいフォーマットに変換するかの問い合わせがある。許可しない場合、ビルドは成功する。許可した場合、ヘッダファイル検索パスの設定が消失するためか、そのままではビルドできない。  
　また、Xcode から実行させると argv[1] と argv[2] に fl-guruguru が許容しない引数が指定されるため、fl-guruguru は usage を表示して終了していまう。
