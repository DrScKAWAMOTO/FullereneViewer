# FullereneViewer
## フラーレンビューア
### コンパイル方法 (ubuntu/VirtualBox 編)
　VirtualBox のゲストOSとして ubuntu ja 12.04 desktop i386 をインストールしている想定です。

#### 1) パッケージのインストール
##### 1-1) ubuntu を起動する。
##### 1-2) Dash ホームをクリックする。
##### 1-3) 検索窓で「terminal」を入力する
##### 1-4) 「端末」を起動する。
##### 1-5) 端末で以下の入力を行う。
    $ sudo apt-get update
        ------ データベース更新が終わるのを待つ。
    $ sudo apt-get upgrade
        ------ パッケージ更新が終わるのを待つ。
    $ sudo apt-get install mesa-utils
        ------ mesa-utils のインストールが完了するのを待つ。
    $ glxinfo
        ------ 長いログが表示されることを確認する。
    $ sudo apt-get install qt4-qmake libqt4-dev g++
        ------ qt4-qmake libqt4-dev g++ のインストールが完了するのを待つ。

#### 2) github からソースファイル取得
    home> git clone https://github.com/DrScKAWAMOTO/FullereneViewer
    home> cd FullereneViewer
    FullereneViewer> 

#### 3) コンパイル
##### 3-1) フラーレンビューア(FullereneViewer)
    FullereneViewer> cd src
    src> make Qt
    src> 

##### 3-2) フラーレンジェネレータ(ca-*)
    FullereneViewer> cd src
    src> make depend
    src> make
    src> 

#### 4) 実行例
　ビューアをコンパイルした ubuntu ホストで X11 を起動する。

    FullereneViewer> cd src/Qt
    Qt> ./FullereneViewer

#### 5) お願い
　VMWare などの他の仮想マシン上 Linux での動作報告を [Issue](https://github.com/DrScKAWAMOTO/FullereneViewer/issues) で教えてください。  
　実マシンの Linux での動作報告を [Issue](https://github.com/DrScKAWAMOTO/FullereneViewer/issues) で教えてください。
