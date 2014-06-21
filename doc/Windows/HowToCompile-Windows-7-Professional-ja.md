# FullereneViewer
## フラーレンビューア
### コンパイル方法 (Windows 7 編)
　Windows 7 Professional で確認しました。

#### 1) Download `Visual Studio Express 2012 for Windows Desktop`.
　<http://www.microsoft.com/ja-jp/download/details.aspx?id=34673> のダウンロードボタンを押し、 `wdexpress_full.exe` をダウンロードする。

#### 2) Install `Visual Studio Express 2012 for Windows Desktop`.
　ダウンロードした `wdexpress_full.exe` をダブルクリックする。  
　「ライセンス条項に同意する。」にチェックを入れ、インストールボタンをクリックする。  
　必要に応じてアップデートを適用する。
　Visual Studio Express 2012 for Windows Desktop はフリーだが、30日以上の使用にはライセンス登録が必要。  
　ライセンスを登録するには、マイクロソフトのアカウントも必要。

#### 3) Download `freeglut`.
　<http://sourceforge.net/projects/freeglut/files/freeglut/> から `freeglut-2.8.1.tar.gz` をダウンロードし、解凍する。

#### 4) Build `freeglut`
　解凍してできたフォルダの「freeglut-2.8.1\VisualStudio\2012」の中に、Visual Studio Express 2012用のソリューションファイル(sln)があるのでダブルクリックする。  
　Visual Studio が起動したら、[ビルド] - [ソリューションのビルド] を選択する。

#### 5) Download and install `Qt5`.
　<http://qt-users.jp/download.html> の「Qt 5.2.1 - Windows 32-bit (MinGW 4.8, OpenGL, 634 MB)」を選択してダウンロードする。  
　ダウンロードした `qt-opensource-windows-x86-mingw48_opengl-5.2.1.exe` をダブルクリックする。  
　これは mingw4.8 を同梱している。

#### 6) Install `POVRay 3.7.0`.
　<http://www.povray.org/download/> から POVRay の binary インストーラである `povwin-3.7-agpl3-setup.exe` をダウンロードして実行し、インストール先を確認する。

#### 7) Adjust `POVRay`.
　デスクトップ上の「POV-Ray v3.7」をクリックして POVRay を起動し、New ボタンの下のリストから [1600x1200, No AA] などの4対3の画角を選択し終了する。  
　ご自分のディスプレイの解像度や、出力したいデータフォーマットなどにあわせてください。

##### 7-1) 参考(2014/06/02 時点)
　デスクトップに作成されたアイコンは、 `C:\Program Files\POV-Ray\v3.7\bin\pvengine-sse2.exe` を指していて実行できない。`C:\Program Files\POV-Ray\v3.7\bin\pvengine32-sse2.exe` に変更する。

#### 8) Get source codes from `github`.
    home> git clone https://github.com/DrScKAWAMOTO/FullereneViewer
    home> cd FullereneViewer
    FullereneViewer> 

#### 9) Modify `FullereneViewer\src\Qt\fl-guruguru.pro` as need.
　win32 { } で囲まれた部分の DEPENDPATH と INCLUDEPATH と LIBS を、freeglut-2.8.1 を展開したパスに置き換える。

#### 10) Modify `FullereneViewer\src\CarbonAllotrope.cc` as need.
　1776行目(付近)に `pvengine32-sse2.exe` を起動するコードが書かれているので、インストールパスが正しいか確認し、異なっておればソースコードを修正する。

#### 11) Launch `Qt Creator`.
　`FullereneViewer\src\Qt\fl-guruguru.pro` をダブルクリックする。  
　「Configure Project」ボタンを押す。

#### 12) Build project "fl-guruguru".
　[ビルド] - [プロジェクト "fl-guruguru" をビルド] を選択する。

#### 13) Debug project "fl-guruguru".
　動作します。

#### 14) Make folder and copy batch file into it.
　`scripts/windows/fl-guruguru.bat` の記述が正しいか確認し、これをお好きなフォルダにコピーして下さい。  
　なお、 `fl-guruguru.bat` をコピーしたフォルダには展開図やスナップショット図が出力されます。

#### 15) Execute batch file.
　動作します。

#### 16) お願い
　Windows の他の OS での動作実績があれば連絡ください。  
　cygwin や Qt5 + Visual Studio Express 2012 では qmake を使って make で出来ると思われます。これをトライされた方は [Issue](https://github.com/DrScKAWAMOTO/FullereneViewer/issues) をオープンして情報提供お願いします。
