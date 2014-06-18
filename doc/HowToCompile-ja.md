# FullereneViewer
## フラーレンビューア
### コンパイル方法

#### FreeBSD 9.1
##### 1) ポートのインストール
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

##### 2) パスを通す
    export PATH=/usr/local/lib/qt5:$PATH

##### 3) github からソースファイル取得
    home> git clone https://github.com/DrScKAWAMOTO/FullereneViewer
    home> cd FullereneViewer
    FullereneViewer> 

##### 4) コンパイル
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
　graphics/povray37 のかわりに graphics/povray36 をインストールした方は、4-1) の直前に `FullereneViewer/src/CarbonAllotrope.cc` を編集して、以下のように変更してください。

    #if defined(__unix) || defined(__APPLE__)
      sprintf(command_line, "povray36 -W1600 -H1200 %s", pov_name);
    #endif

##### 4-5) 参考
　６面展開図、スナップショット図のサイズの変更は、4-1) の直前に `FullereneViewer/src/CarbonAllotrope.cc` の以下の `-W1600 -H1200` の部分を編集してください。

    #if defined(__unix) || defined(__APPLE__)
      sprintf(command_line, "povray37 -W1600 -H1200 %s", pov_name);
    #endif

##### 5-1) 実行例その１
　ビューアをコンパイルした FreeBSD ホストで X11 を起動する。

    FullereneViewer> cd src/Qt
    Qt> ./fl-guruguru

　X11 を起動してからはじめての実行では

    QXcbConnection: XCB error: 163 (Unknown), sequence: 158, resource id: 0, major code: 148 (Unknown), minor code: 20

のエラーが出ますが、いったん C-c で終了して再実行するとうまく動作します。  
　このエラーに関する対策など、情報をお持ちの方は、 [Issue](https://github.com/DrScKAWAMOTO/FullereneViewer/issues) をオープンして情報提供お願いします。

##### 5-2) 実行例その２
　FreeBSD で X11 を起動する。  
　kterm を立ち上げ、ssh -Y でビューアをコンパイルした FreeBSD ホストへログインする。

    home> cd FullereneViewer/src/Qt
    Qt> export LIBGL_ALWAYS_INDIRECT=1
    Qt> ./fl-guruguru

　遅いが、動作します。

##### 5-3) 実行例その３
　Mac で Xquartz を起動する。  
　Mac でターミナルを立ち上げる。  
　ターミナルから ssh -Y でビューアをコンパイルした FreeBSD ホストへログインする。

    home> cd FullereneViewer/src/Qt
    Qt> export LIBGL_ALWAYS_INDIRECT=1
    Qt> ./fl-guruguru

　遅いが、動作します。

##### 6) `FullereneViewer/scripts/unix` に起動スクリプト `fl-guruguru.sh` を用意しました。
　なお、 `fl-guruguru` を起動したフォルダには展開図やスナップショット図が出力されます。

##### 7) お願い
　仮想マシンでの実行例があれば [Issue](https://github.com/DrScKAWAMOTO/FullereneViewer/issues) で情報提供お願いします。

#### ubuntu/VirtualBox
　VirtualBox のゲストOSとして ubuntu ja 12.04 desktop i386 をインストールしている想定です。

##### 1) パッケージのインストール
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
    $ sudo apt-get install qt4-qmake libqt4-dev g++ freeglut3 freeglut3-dev freeglut3-dbg
        ------ qt4-qmake libqt4-dev g++ freeglut3 freeglut3-dev freeglut3-dbg のインストールが完了するのを待つ。

##### 2) github からソースファイル取得
    home> git clone https://github.com/DrScKAWAMOTO/FullereneViewer
    home> cd FullereneViewer
    FullereneViewer> 

##### 3) コンパイル
##### 3-1) フラーレンビューア(fl-guruguru)
    FullereneViewer> cd src
    src> make Qt
    src> 

##### 3-2) フラーレンジェネレータ(ca-*)
    FullereneViewer> cd src
    src> make depend
    src> make
    src> 

##### 4) 実行例
　ビューアをコンパイルした ubuntu ホストで X11 を起動する。

    FullereneViewer> cd src/Qt
    Qt> ./fl-guruguru

##### 5) `FullereneViewer/scripts/unix` に起動スクリプト `fl-guruguru.sh` を用意しました。
　なお、 `fl-guruguru` を起動したフォルダには展開図やスナップショット図が出力されます。  
　動作します。

##### 6) お願い
　VMWare などの他の仮想マシン上 Linux での動作報告を [Issue](https://github.com/DrScKAWAMOTO/FullereneViewer/issues) で教えてください。  
　実マシンの Linux での動作報告を [Issue](https://github.com/DrScKAWAMOTO/FullereneViewer/issues) で教えてください。

#### OS-X 10.6.x
##### 1) XQuartz と xcode をインストールする。
##### 2) Qtライブラリ4.8.6 をダウンロードする。
　<http://qt-users.jp/download.html> の「Qt ライブラリ 4.8.6 - Mac (185 MB)」をクリックして `qt-opensource-mac-4.8.6.dmg` をダウンロードする。

##### 3) Qtライブラリ4.8.6 をインストールする。
　ダウンロードした `qt-opensource-mac-4.8.6.dmg` をダブルクリックして dmg ファイルを開き、Qt.mpkg をダブルクリックする。

##### 4) github からソースファイル取得
    home> git clone https://github.com/DrScKAWAMOTO/FullereneViewer
    home> cd FullereneViewer
    FullereneViewer> 

##### 5) コンパイル
##### 5-1) フラーレンビューア(fl-guruguru)
    FullereneViewer> cd src
    src> make Qt
    src> 

　ファインダから `FullereneViewer/src/Qt/fl-guruguru.xcodeproj` をダブルクリック「ビルドと実行」を選択する。

##### 5-2) フラーレンジェネレータ(ca-*)
    FullereneViewer> cd src
    src> make depend
    src> make
    src> 

##### 6) 実行
　POVRay が現在インストール出来ないため、[出力] の [展開図] [スナップショット図] が実行できないが、それらを除いて動作します。  
　POVRay for Mac の Unofficial 版(GUI & CUI) をインストールしてみましたが、使い方が良くわからないです。自動レンダリングが出来ていません。

##### 7) `FullereneViewer/scripts/unix` に起動スクリプト `fl-guruguru.sh` を用意しました。
　なお、 `fl-guruguru` を起動したフォルダには展開図やスナップショット図が出力されます。

##### 8) お願い
　Mac 版 POVRay がインストールできたら連絡ください。  
　Mac の他の OS での動作実績があれば連絡ください。

#### Windows
　Windows 7 Professional で確認しました。

##### 1) Visual Studio Express 2012 for Windows Desktop をダウンロードする。
　<http://www.microsoft.com/ja-jp/download/details.aspx?id=34673> のダウンロードボタンを押し、 `wdexpress_full.exe` をダウンロードする。

##### 2) Visual Studio Express 2012 for Windows Desktop をインストールする。
　ダウンロードした `wdexpress_full.exe` をダブルクリックする。  
　「ライセンス条項に同意する。」にチェックを入れ、インストールボタンをクリックする。  
　必要に応じてアップデートを適用する。
　Visual Studio Express 2012 for Windows Desktop はフリーだが、30日以上の使用にはライセンス登録が必要。  
　ライセンスを登録するには、マイクロソフトのアカウントも必要。

##### 3) freeglut をダウンロードする。
　<http://sourceforge.net/projects/freeglut/files/freeglut/> から `freeglut-2.8.1.tar.gz` をダウンロードし、解凍する。

##### 4) freeglut をメイクする。
　解凍してできたフォルダの「freeglut-2.8.1\VisualStudio\2012」の中に、Visual Studio Express 2012用のソリューションファイル(sln)があるのでダブルクリックする。  
　Visual Studio が起動したら、[ビルド] - [ソリューションのビルド] を選択する。

##### 5) Qt5 のダウンロード＆インストール
　<http://qt-users.jp/download.html> の「Qt 5.2.1 - Windows 32-bit (MinGW 4.8, OpenGL, 634 MB)」を選択してダウンロードする。  
　ダウンロードした `qt-opensource-windows-x86-mingw48_opengl-5.2.1.exe` をダブルクリックする。  
　これは mingw4.8 を同梱している。

##### 6) POVRay 3.7.0 のインストール
　<http://www.povray.org/download/> から POVRay の binary インストーラである `povwin-3.7-agpl3-setup.exe` をダウンロードして実行し、インストール先を確認する。

##### 7) POVRay の調整
　デスクトップ上の「POV-Ray v3.7」をクリックして POVRay を起動し、New ボタンの下のリストから [1600x1200, No AA] などの4対3の画角を選択し終了する。  
　ご自分のディスプレイの解像度や、出力したいデータフォーマットなどにあわせてください。

##### 7-1) 参考(2014/06/02 時点)
　デスクトップに作成されたアイコンは、 `C:\Program Files\POV-Ray\v3.7\bin\pvengine-sse2.exe` を指していて実行できない。`C:\Program Files\POV-Ray\v3.7\bin\pvengine32-sse2.exe` に変更する。

##### 8) github からソースファイル取得
    home> git clone https://github.com/DrScKAWAMOTO/FullereneViewer
    home> cd FullereneViewer
    FullereneViewer> 

##### 9) `FullereneViewer\src\Qt\fl-guruguru.pro` の修正
　win32 { } で囲まれた部分の DEPENDPATH と INCLUDEPATH と LIBS を、freeglut-2.8.1 を展開したパスに置き換える。

##### 10) `FullereneViewer\src\CarbonAllotrope.cc` の修正
　1776行目(付近)に pvengine32-sse2.exe を起動するコードが書かれているので、インストールパスが正しいか確認し、異なっておればソースコードを修正する。

##### 11) Qt Creator 起動
　`FullereneViewer\src\Qt\fl-guruguru.pro` をダブルクリックする。  
　「Configure Project」ボタンを押す。

##### 12) プロジェクト "fl-guruguru" をビルドする。
　[ビルド] - [プロジェクト "fl-guruguru" をビルド] を選択する。

##### 13) プロジェクト "fl-guruguru" をデバッグする。
　動作します。

##### 14) フォルダを作成し、バッチファイルをコピーする。
　`FullereneViewer/scripts/windows/fl-guruguru.bat` の記述が正しいか確認し、これをお好きなフォルダにコピーして下さい。  
　なお、 `fl-guruguru.bat` をコピーしたフォルダには展開図やスナップショット図が出力されます。

##### 15) バッチファイルをダブルクリックする。
　動作します。

##### 16) お願い
　Windows の他の OS での動作実績があれば連絡ください。  
　cygwin や Qt5 + Visual Studio Express 2012 では qmake を使って make で出来ると思われます。これをトライされた方は [Issue](https://github.com/DrScKAWAMOTO/FullereneViewer/issues) をオープンして情報提供お願いします。
