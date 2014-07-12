# FullereneViewer
## フラーレンビューア
### コンパイル方法 (Mac OS X 10.6.x 編)
#### 1) XQuartz と Xcode 3.2.6 とコマンドライン開発ツールをインストールする。
#### 2) Qtライブラリ4.8.6 をダウンロードする。
　<http://qt-users.jp/download.html> の「Qt ライブラリ 4.8.6 - Mac (185 MB)」をクリックして `qt-opensource-mac-4.8.6.dmg` をダウンロードする。

#### 3) Qtライブラリ4.8.6 をインストールする。
　ダウンロードした `qt-opensource-mac-4.8.6.dmg` をダブルクリックして dmg ファイルを開き、Qt.mpkg をダブルクリックする。

#### 4) github からソースファイル取得
    home> git clone https://github.com/DrScKAWAMOTO/FullereneViewer
    home> cd FullereneViewer
    FullereneViewer> 

#### 5) コンパイル
##### 5-1) フラーレンビューア(FullereneViewer)
    FullereneViewer> cd src
    src> make Qt
    src> 

　ファインダから `src/Qt/FullereneViewer.xcodeproj` をダブルクリック「ビルドと実行」を選択する。

##### 5-2) フラーレンジェネレータ(ca-*)
    FullereneViewer> cd src
    src> make depend
    src> make
    src> 

#### 6) 実行
　POVRay が現在インストール出来ないため、[出力] の [展開図] [スナップショット図] が実行できないが、それらを除いて動作します。  
　POVRay for Mac の Unofficial 版(GUI & CUI) をインストールしてみましたが、使い方が良くわからないです。自動レンダリングが出来ていません。

#### 7) `scripts/unix` に起動スクリプト `FullereneViewer.sh` を用意しました。
　なお、 `FullereneViewer` を起動したフォルダには展開図やスナップショット図が出力されます。

#### 8) お願い
　Mac 版 POVRay がインストールできたら連絡ください。  
　Mac の他の OS での動作実績があれば連絡ください。
