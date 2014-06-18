# FullereneViewer
## フラーレンビューア
### 完了リスト
　やることリストから終わった順番に、一番下の行へ追記。

#### ENGINEERING
　Interactive クラスへの移行。

#### ENGINEERING
　Ring::draw_semitransparent_by_OpenGL(), OpenGLUtil::draw_pentagon() を修正。6角形以外の任意の多角形に対応する。draw_pentagon を draw_ring に変更して、Ring クラスを引数にわたす。

#### MATHEMATICS
　五員環の間で距離を計り、分布を調べる。サマリの出力。距離マトリックスを算出。

#### ENGINEERING
　カーボン軌道を計算してフラーレンを描画する。→失敗

0) SymmetryAxis クラスを実装する。
1) automorphisms によるカーボン軌道を計算し、その群構造を決定する。
2) 裏返しは省く。
3) 群構造から、カーボン軌道の単位球面上の位置を計算する。
4) カーボン軌道を一つ選択し、そのカーボン達はその球面上の位置に固定する。ただし、半径方向には自由度を与える。
5) 力学シミュレーションを実施する。
6) レイトレーシング出力する。

#### ENGINEERING
　マウスぐりぐりソフトでフラーレンを表示する。

#### ENGINNERING
　マウスぐりぐりソフトのメニューを増やす。

#### ENGINEERING
　OpenGL のマウスピッキング技術を使って、一つのカーボンだけ揺らして形を整える。一つのカーボンだけつまんで移動。

#### ENGINEERING
　現在のイメージを POVRay へ出力(s)して高画質描画を実施。

#### ENGINEERING
　現在のイメージを形状記憶(m)。次回再起動時に形状思いだし(r)。

#### ENGINEERING
　メモリリーク報告時に、確保された時のファイル名行番号も報告するように改造。

#### ENGINEERING
　OpenGL で赤い半透明セロファンを貼りたい。セロファンは、半透明を使って描画順番を考えないといけないため断念。星印描画で代用

#### DEBUG
    C144 (NoA=1) A1-5b6b5b6b5b6b5b6bd5b6f5b6i5b6e5b6c5b6b5b6f5b
    C142 (NoA=1) A1-5b6b5b6b5b6b5b6bd5b6f5b6i5b6f5c6d5b6b5b6c
    C142 (NoA=1) A1-5b6b5b6b5b6b5b6bd5b6f5b6i5b6f5c6e5b6c5b
    [1]    19120 killed     ./ca-generator --ordinary=150

　再現しにくくなったためとりあえず完了

#### DEBUG
　OpenGL で起動時にウィンドウがちらつく。メニュー作成時にちらつくので、Qt に移行してメニューも移植すればたぶん大丈夫。

#### DEBUG
　`ca-generator --symmetric=60` が C60 を報告しなくなった。いつのまにか、直っている。

#### MATHEMATICS
　統計処理、主成分分析。6面図描画のために３軸の登録、３軸の計算、３軸の表示を実施。独自アルゴリズムを interaction に登録する方法の開発。POVRay で6面図描画する。

#### ENGINEERING
　半透明オブジェクトは奥にあるオブジェクトから描画しなければならない。
    OpenGLUtil::opaque_mode();
      不透明オブジェクトの描画
    OpenGLUtil::semitransparent_mode();
    OpenGLUtil::backface_mode();
      半透明オブジェクトの描画
    OpenGLUtil::frontface_mode();
      半透明オブジェクトの描画

#### ENGINEERING
　Qt で実現。

#### ENGINEERING
　半透明オブジェクトは奥にあるオブジェクトから描画しなければならない。
    OpenGLUtil::opaque_mode();
      不透明オブジェクトの描画
    OpenGLUtil::semitransparent_mode();
    OpenGLUtil::backface_mode();
      半透明オブジェクトの描画
    OpenGLUtil::frontface_mode();
      半透明オブジェクトの描画

#### ENGINEERING
　半透明オブジェクトの裏側が見えている場合は、黒くなるが、これを明るくするためには法線を逆向きにする必要が有る。

#### ENGINEERING
　半透明オブジェクトの裏側が見えている場合は、黒くなるが、これを明るくするためには法線を逆向きにする必要が有る。

#### DEBUG
　UNIX 環境でタイトルが表示されない。

#### ENGINEERING
　タイトルに、Fullerene の名前と生成ラベルを表示する。

#### MATHEMATICS
　七員環の実現。

#### ENGINEERING
　povray 出力ファイルの拡張子を直す。

#### DEBUG
　Windows 版 Mac 版がタイマー周期でちらつく

    void Guruguru::initializeGL()
    {
      OpenGLUtil::initialize_post();
      //タイマーの開始、間隔を16ms(≒60fps)に設定
      timer->start(16, this);
    }

でタイマー間隔を 1s くらいに変更して動作を観察してみる。swap_buffers() を呼び出してはいけなくなったらしい。
