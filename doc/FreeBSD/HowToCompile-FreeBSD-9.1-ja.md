# FullereneViewer
## フラーレンビューア
### コンパイル方法 (FreeBSD 9.1)
#### 1) ポートのインストール
    home> su
    # cd /usr/ports
    # portmaster -d devel/qt5 graphics/freeglut graphics/libGLU graphics/libGL graphics/povray37
        graphics/povray37 は X11 を ON
        devel/sdl12 は OPENGL X11 を ON
        graphics/aalib は X11 を ON

##### 1-2) 参考
　devel/qt5 のかわりに devel/qt4 でもできます。下記の 4-3) 参考を読んで下さい。  
　graphics/povray37 のかわりに graphics/povray36 でもできます。下記の 4-4) 参考を読んで下さい。

##### 1-3) 参考
　X11 サーバーに OpenGL が要求するフレームバッファのサポートがあるかを調べるには、glxinfo を起動します。
　glxinfo で大量にログが出ればフレームバッファのサポートが有るということです。

###### glxinfo のインストール方法
    # cd /usr/ports/graphics/glew
    # make
    # cd work/glew*/config
    # ln -s Makefile.freebsd Makefile.FreeBSD
    # cd ../../..
    # make install
    # make clean
    # cd /usr/ports/graphics/mesa-demos
    # make install
        NVIDIA を ON
    # make clean

#### 2) パスを通す
    export PATH=/usr/local/lib/qt5:$PATH

#### 3) github からソースファイル取得
    home> git clone https://github.com/DrScKAWAMOTO/FullereneViewer
    home> cd FullereneViewer
    FullereneViewer> 

#### 4) コンパイル
##### 4-1) フラーレンビューア(fl-guruguru)
    FullereneViewer> cd src
    src> gmake Qt
    src> 

##### 4-2) フラーレンジェネレータ(ca-*)
    FullereneViewer> cd src
    src> gmake depend
    src> gmake
    src> 

##### 4-3) 参考
　devel/qt5 のかわりに devel/qt4 をインストールした方は、4-1) において

    FullereneViewer> cd src
    src> gmake QMAKE=qmake-qt4 Qt
    src> 

とします。

##### 4-4) 参考
　graphics/povray37 のかわりに graphics/povray36 をインストールした方は、4-1) の直前に `src/CarbonAllotrope.cc` を編集して、以下のように変更してください。

    #if defined(__unix) || defined(__APPLE__)
      sprintf(command_line, "povray36 -W1600 -H1200 %s", pov_name);
    #endif

##### 4-5) 参考
　６面展開図、スナップショット図のサイズの変更は、4-1) の直前に `src/CarbonAllotrope.cc` の以下の `-W1600 -H1200` の部分を編集してください。

    #if defined(__unix) || defined(__APPLE__)
      sprintf(command_line, "povray37 -W1600 -H1200 %s", pov_name);
    #endif

#### 5) 実行例
##### 5-1) その１
　ビューアをコンパイルした FreeBSD ホストで X11 を起動する。

    FullereneViewer> cd src/Qt
    Qt> ./fl-guruguru

　X11 を起動してからはじめての実行では

    QXcbConnection: XCB error: 163 (Unknown), sequence: 158, resource id: 0, major code: 148 (Unknown), minor code: 20

のエラーが出ますが、いったん C-c で終了して再実行するとうまく動作します。  
　このエラーに関する対策など、情報をお持ちの方は、 [Issue](https://github.com/DrScKAWAMOTO/FullereneViewer/issues) をオープンして情報提供お願いします。

##### 5-2) その２
　FreeBSD で X11 を起動する。  
　kterm を立ち上げ、ssh -Y でビューアをコンパイルした FreeBSD ホストへログインする。

    home> cd FullereneViewer/src/Qt
    Qt> export LIBGL_ALWAYS_INDIRECT=1
    Qt> ./fl-guruguru

　遅いが、動作します。

##### 5-3) その３
　Mac で Xquartz を起動する。  
　Mac でターミナルを立ち上げる。  
　ターミナルから ssh -Y でビューアをコンパイルした FreeBSD ホストへログインする。

    home> cd FullereneViewer/src/Qt
    Qt> export LIBGL_ALWAYS_INDIRECT=1
    Qt> ./fl-guruguru

　遅いが、動作します。

#### 6) `scripts/unix` に起動スクリプト `fl-guruguru.sh` を用意しました。
　なお、 `fl-guruguru` を起動したフォルダには展開図やスナップショット図が出力されます。

#### 7) お願い
　仮想マシンでの実行例があれば [Issue](https://github.com/DrScKAWAMOTO/FullereneViewer/issues) で情報提供お願いします。
