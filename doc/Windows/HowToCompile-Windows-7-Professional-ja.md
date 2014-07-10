# FullereneViewer
## フラーレンビューア
### コンパイル方法 (Windows 7 Professional 編)
#### 1) Download and install `Qt5`.
　<http://qt-users.jp/download.html> の「Qt 5.2.1 - Windows 32-bit (MinGW 4.8, OpenGL, 634 MB)」を選択してダウンロードする。  
　ダウンロードした `qt-opensource-windows-x86-mingw48_opengl-5.2.1.exe` をダブルクリックする。  
　これは mingw4.8 を同梱している。

#### 2) Install `POVRay 3.7.0`.
　<http://www.povray.org/download/> から POVRay の binary インストーラである `povwin-3.7-agpl3-setup.exe` をダウンロードして実行し、インストール先を確認する。

#### 3) Adjust `POVRay`.
　デスクトップ上の「POV-Ray v3.7」をクリックして POVRay を起動し、New ボタンの下のリストから [1600x1200, No AA] などの4対3の画角を選択し終了する。  
　ご自分のディスプレイの解像度や、出力したいデータフォーマットなどにあわせてください。

##### 3-1) 参考(2014/06/02 時点)
　デスクトップに作成されたアイコンは、 `C:\Program Files\POV-Ray\v3.7\bin\pvengine-sse2.exe` を指していて実行できない。`C:\Program Files\POV-Ray\v3.7\bin\pvengine32-sse2.exe` に変更する。

#### 4) Get source codes from `github`.
    home> git clone https://github.com/DrScKAWAMOTO/FullereneViewer
    home> cd FullereneViewer
    FullereneViewer> 

#### 5) Modify `FullereneViewer\src\CarbonAllotrope.cc` as need.
　1776行目(付近)に `pvengine32-sse2.exe` を起動するコードが書かれているので、インストールパスが正しいか確認し、異なっておればソースコードを修正する。

#### 6) Launch `Qt Creator`.
　`FullereneViewer\src\Qt\fl-guruguru.pro` をダブルクリックする。  
　「Configure Project」ボタンを押す。

#### 7) Build project "fl-guruguru".
　[ビルド] - [プロジェクト "fl-guruguru" をビルド] を選択する。

#### 8) Debug project "fl-guruguru".
　動作します。

#### 9) Make folder and copy batch file into it.
　`scripts/windows/fl-guruguru.bat` の記述が正しいか確認し、これをお好きなフォルダにコピーして下さい。  
　なお、 `fl-guruguru.bat` をコピーしたフォルダには展開図やスナップショット図が出力されます。

#### 10) Execute batch file.
　動作します。

#### 11) お願い
　Windows の他の OS での動作実績があれば連絡ください。  
　cygwin や Qt5 + Visual Studio Express 2012 では qmake を使って make で出来ると思われます。これをトライされた方は [Issue](https://github.com/DrScKAWAMOTO/FullereneViewer/issues) をオープンして情報提供お願いします。
