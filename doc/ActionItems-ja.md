# FullereneViewer
## フラーレンビューア
### やることリスト
　完了したら、 [FullereneViewer/doc/DONE-ja.md](https://github.com/DrScKAWAMOTO/FullereneViewer/blob/master/doc/DONE-ja.md) の、一番下の行へ追記します。


#### COMMUNICATIONS
　Qiita の活用。

#### GAME
　フラーレン当てっこゲーム。

#### ART
　見栄のいい角度でスナップショットを撮る。  
　CarbonAllotrope::calculate_three_axes() で分散共分散行列の固有値を計算して長径、中径、短径を求めている。  
　直方体は見栄のいい角度が常識であるから、長径、中径、短径をこれと同じ向きに回転したものをスナップショット撮ればよい。

#### ART
　スクリーンセーバにする。  
　うようよ状態から丸くなるまでを表示。  
　終わったら次のフラーレンに移る。

#### ART
　ぐるぐるモードで慣性感をだす。

#### ART
　五員環を区別する目的は度外視して、芸術的にセンスの良いセロファン配色 `例：落ち着いたモノトーン` を提案できる方、情報提供お願いします。  

#### ART
　フラーレンの表示を、球棒モデルばかりでなく、原子核の確率分布を雲状にレンダリングしてみてはどうか？

#### ART
　フラーレンビューアで綺麗に丸くしたフラーレンのスナップショット集を作る。

#### ART
　3D プリンタへの出力。

#### CHEMISTRY
　数学的に構成された全フラーレンのうち、化学的に発見されているものリストを作成する。

#### CHEMISTRY
　化学的な構成可能性を調べるツール開発。  
　本ソフトは、綺麗に見せることや、数学的に構成することに注力しており、独自力学シミュレーションを採用している。  
　クーロン力などの正確な自然力学シミュレーションを使って化学的な構成可能性を調べる。

#### MATHEMATICS
　五員環のセロファン配色で、12枚を区別しやすい配色を提案できる方、情報提供お願いします。

#### MATHEMATICS
　[FullereneViewer/doc/Mathematics-ja.md](https://github.com/DrScKAWAMOTO/FullereneViewer/blob/master/doc/Mathematics-ja.md) の「生成式」「特性式」の意味・定義を思い出してドキュメント化する。  
　「生成式」は、フラーレン生成アルゴリズムを解読すれば思い出せる。  
　「特性式」は、ca-char --pentagon-distances [generator-name] の出力結果。

#### MATHEMATICS
　全対称軸の表示。

#### MATHEMATICS
　穴ありのサポート。Y字分岐の２端をくっつけると、穴ありフラーレンができる。

#### MATHEMATICS
　カーボン・ナノ・チューブの全パターンを生成する。格子点で分類できる。

#### ENGINEERING
　extra メニューを修正してドキュメントと同じにする。povray 出力ファイル名生成ルーチンも修正する。

#### ENGINEERING
　修正モードは、炭素をつまむのではなく、五員環をつまむように変更したほうが良いか？

#### ENGINEERING
　アイコンの半透明セロファン部分の色落ちを直す。

#### ENGINEERING
　利用ソフトの著作権関係をクリアして、Windows 版のバイナリインストーラ作成作成。  
　できたらバイナリインストーラの公開サイト募集。

#### ENGINEERING
　利用ソフトの著作権関係をクリアして、Mac 版のバイナリインストーラ作成。  
　作成できたらバイナリインストーラの公開サイト募集。

#### ENGINEERING
　利用ソフトの著作権関係をクリアして、Linux 版のバイナリインストーラ作成。  
　作成できたらバイナリインストーラの公開サイト募集。

#### ENGINEERING
　OpenGL プログラミングガイド P238「点や線のアンチエイリアシング処理」対応。

#### ENGINEERING
　ca-char ca-cmp ca-generator ca-modeling ca-sort ca-test ca-uniq のドキュメント化。

#### ENGINEERING
　Mac の povray 動作させる。

#### ENGINEERING
　セキュリティー・ホールを閉じる。  
例：コマンドライン引数から長さをチェックせずに strcpy()

#### ENGINEERING
　Mac 版で実行したら、 `2014-06-04 22:23:05.006 fl-guruguru[1101:903] GLUT Warning: glutInit being called a second time.` というワーニングが表示された。

#### ENGINEERING
　SSE を使ってマトリックス計算やベクター計算を高速化する。

#### ENGINEERING
　スナップショット描画にて、モデル回転行列は x 軸中心回転が逆回転？  
　スナップショット描画にて、モデル回転行列は y 軸中心回転も逆回転？  
　conjugation をかけたらうまくいった。

　もしかしたら、quaternion を OpenGL の4x4マトリックスに変換するところ、転置してるかもしれない。  
　そこを直して、conjugation もやめて、画面からのピッキングも上下左右逆転などの補正をしたら、トータルうまくいくかも？

#### ENGINEERING
　フラーレンビューアの iPhone や Android への移植。

#### ENGINEERING
　WebGL 移植。

#### ENGINEERING
　フレームレートが落ちるPCでは、シミュレーションはするが表示を間引く？

#### ENGINEERING
　Config.h の `// #define CONFIG_DRAW_NORMAL_VECTOR_IN_POVRAY_ANIMATION 1` を
`#define CONFIG_DRAW_NORMAL_VECTOR_IN_POVRAY_ANIMATION 1` に変更すると、ca-modeling の POVRay アニメーションで法線を表示する。

#### ENGINEERING
　Config.h の `// #define CONFIG_DRAW_NORMAL_VECTOR_IN_POVRAY_SIX_VIEWS 1` を
`#define CONFIG_DRAW_NORMAL_VECTOR_IN_POVRAY_SIX_VIEWS 1` に変更すると、fl-guruguru の POVRay 6面展開図で法線を表示する。

#### ENGINEERING
　Config.h の `// #define CONFIG_DRAW_NORMAL_VECTOR_IN_GURUGURU_MODE 1` を
`#define CONFIG_DRAW_NORMAL_VECTOR_IN_GURUGURU_MODE 1` に変更すると、fl-guruguru のぐるぐるモードで法線を表示する。

#### ENGINEERING
　Config.h の `// #define CONFIG_DRAW_PRINCIPAL_COMPONENT_AXIS_IN_GURUGURU_MODE 1` を
`#define CONFIG_DRAW_PRINCIPAL_COMPONENT_AXIS_IN_GURUGURU_MODE 1` に変更すると、fl-guruguru のぐるぐるモードで3方向線(長径・中径・短径)を表示する。

#### ENGINEERING
　Config.h の `// #define CONFIG_DRAW_MAJOR_AXES_SYMMETRY_IN_GURUGURU_MODE 1` を
`#define CONFIG_DRAW_MAJOR_AXES_SYMMETRY_IN_GURUGURU_MODE 1` に変更すると、fl-guruguru のぐるぐるモードで主要対称軸を表示する。  
　しかし現状アサートする。

#### ENGINEERING
　Config.h の `// #define CONFIG_PAINT_TRANSPARENT_COLOR_TO_BACKGROUND_IN_POVRAY_SNAPSHOT 1` を
`#define CONFIG_PAINT_TRANSPARENT_COLOR_TO_BACKGROUND_IN_POVRAY_SNAPSHOT 1` に変更すると、fl-guruguru の POVRay スナップショットの背景を透明色にする。

#### ENGINEERING
　Config.h の `#define CONFIG_PUT_COLORED_CELLOPHANE_IN_5_MEMBER_RING 1` を
`// #define CONFIG_PUT_COLORED_CELLOPHANE_IN_5_MEMBER_RING 1` に変更すると、五員環・七員環の半透明セロファンを表示しなくなる。

#### ENGINEERING
　CarbonAllotrope.cc の `#if 1 // TODO 005 どっちが正しい？` を
`#if 0 // TODO 005 どっちが正しい？` にすると ca-generator がセグメンテーションフォールトする。  
　とりあえず、1 のまま。

#### ENGINEERING
　WindowsXP の OpenGL では、両面ライティングを設定していても、ピッキングを開始すると片面ライティング状態になってしまう。  
　FreeBSD の OpenGL では再現しない。

　AvoidBugs.h の `#define AVOID_GLLIGHTMODEL_BUG_IN_WGL 1 // TODO 008` を
`// #define AVOID_GLLIGHTMODEL_BUG_IN_WGL 1 // TODO 008` に変更すると回避策を講じなくなる。

#### ENGINEERING
　ring の追加方法は二つある。

A) concave_boundary_segment() を使って端 bond を求め、その端 bond 上に new ring して ring を生成する方法。

B) 任意の2面carbon を指定して、その carbon 上に new ring して ring を生成する方法。

　最初のうちは、A) で生成することを想定し、端でなければ new ring 出来ないようになっていたが、B) が簡単に出来るように、new ring は、concave の端かどうか確認し、端でなければ端まで移動して、そこから ring を作成し始めるように修正を行った。

　二通りのアプローチがあり、どちらかに統一したい。

　二通りのアプローチは、要件が微妙に違う。

　A) は、作りたい ring の画数が決まっており、その n-polygon を作成して欲しい場合に使用し、B) は、自分で concave を走査して最小の画数の ring を作成したい場合に使用する。

　A) のメソッドを拡張することで、最小の画数の ring が作成できるようにしたい。
