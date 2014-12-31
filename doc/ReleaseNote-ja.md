# FullereneViewer(フラーレンビューア)
## リリースノート

### 2015/01/01 Version 1.4.2
　FreeBSD 9.3 で pkg で qt5 をインストールした場合のコンパイルエラー・ドキュメントを修正。(Yuji Nomura)  
　FreeBSD 9.2 で pkg で qt4 をインストールした場合のコンパイルエラーを修正。(Yuji Nomura)  
　copyright 修正。

### 2014/11/03 Version 1.4.1
　ca-server ca-parallel 実装。  
　ca-generator をパラレル実行させることができる。  
　assert() に副作用のあるコードを書かないようにした。  
　ObjectString と MyString クラス実装。  
　SortedList.h 廃止。  
　セグメントを複数指定できるように修正 プロセス割り当ては、「未割り当て」は即座に割り当てるが、「割り当て済みを分割」は、30秒タイマで遅延させる。  
　コールバック用ソケットはあらかじめ作っておく。  
　ca-parallel wait 実装。  
　ca-parallel: 分割基準を「残りが多いもの」に変更。  
　ca-parallel: FreeBSD 9.x でコールバックが待てないのを修正。  
　ca-parallel: search を collector に変更。  
　ca-collector のプログレス表示バグ修正。  
　ca-uniq ca-mirror ca-char を一つのプログラム(ca-char)にまとめた。  
　ca-char: ヘッダ出力無し対応。  
　ca-sort: 出力分割対応。  
　ca-mirror 削除。  
　主成分軸の表示・非表示を設定画面で指定できるようにした。  
　主成分軸の長さは sqrt にした。  
　五員環セロファンの表示・非表示を設定画面で指定できるようにした。

### 2014/10/13 Version 1.4.0
　FreeBSD 10.0 用の howToCompile ファイルの修正漏れ対応。  
　半開カーボンナノチューブで ca-char した時落ちるのを修正。  
　ca-char では引数に生成式を指定する方法をやめて、*.gf ファイル指定方法のみにした。  
　デバッグ表示修正。  
　境界探索で、探索方向を固定化した。  
　デバッグ時のログを増やした。  
　ca-generator のデフォルトを --symmetric=100 --step-backward にした。  
　ca-generator --symmetric 時のスクラップを「S1(五員環中心)」「S2/S3/S4/S5(六員環中心)」「S6/S7(カーボン中心)」「S8/S9(ボンド中心)」に変更。  
　scrap_order 導入。  
　p_list_most_inside_carbons_on_boundary() などのリストアップ順番を一定方向に統一。  
　生成式で S1-566566 のような非圧縮形式も許すように拡張。  
　povray 起動コマンドライン間違い修正。  
　refleiso 版は削除。  
　ca-generator の対称フラーレン生成は、五員環への距離を元に次の生成箇所を決定する版と、scrap_order を元に次の生成箇所を決定する版とに分離。コンフィギュレーションで指定。  
　set_clockwise(-1) は必要とする箇所の直前で実施。  
　メモリリークテスト実施。  
　フラーレンメニューをツリービューベースに変更。  
　ツリービュー実体は自動生成。  
　unused parameter のウォーニングを黙らせるマクロ導入。  
　Tube は径の太いものもピックアップ。  
　リリース手順書作成。  
　対称軸表示の設定を、静的設定(Config.h)から動的設定(Configuration.h)へ移動した。  
　時計回り管理をリング生成時に行うように修正し、ca-generator の高速化。  
　設定ファイルの読み込みが出来なくなっていたのを修正。  
　メモリリーク退治。  
　clang 対応。  
　フラーレン選択ツリーで名前を付けるようにした。  
　OpenGLUtil::change_fullerene() で fullerene_name がなくてもいいように自分で生成する。  
　鏡映対称フラーレンは (NoA=60M) などと表示するように変更。  
　Yフラーレンが C474 (NoA=6M) を表示するようにした。  
　copy_string() へ移行

### 2014/09/26 Version 1.3.3
　doc/FreeBSD/HowToCompile-FreeBSD-10.0-ja.md 修正。  
　POVRay起動コマンドラインを設定画面からひっぱってくる際のパラメータ設定間違い修正。

### 2014/09/22 Version 1.3.2
　鏡像もリストアップする版(新)と鏡像を略する版(従来)を Config.h で指定できるようにした。  
　鏡像もリストアップする版の場合は、鏡像関係を出力するコマンド ca-mirror 追加。  
　バイナリサーチを使って ca-generator / ca-uniq 高速化。  
　距離計算のアルゴリズム改良で ca-generator / ca-uniq 高速化。  
　パターン生成のステップアルゴリズム改良で ca-generator 高速化。  
　ステップアルゴリズム --step-backward をデフォルトに。  
　ca-loop / ca-generator は環境変数 CA_STEP_ALGORITHM 採用。  
　ca-loop 引数省略可能にした。  
　ca-char は Pdd= 属性、MirrorSymmetric 属性、Mirror= 属性を出力する機能追加。  
　ca- シリーズで --help も使えるようにした。  
　Qt pro ファイル整理。  
　カーボンナノチューブは両方閉じるとパターンが多くなるので、半分だけ閉じたものをリストアップした。n+m<=12。  
　clang++ でもコンパイルできるようにした。  
　generator-formulas フォルダは別リポジトリに分離した。  
　povray コマンドラインを設定画面で指定できるようにした。

### 2014/09/10 Version 1.3.1
　FullereneViewer について画面追加。  
　閉・開カーボンナノチューブの生成関連追加(Un,m,h-....)。  
　「ジェネレータラベル」を「生成式」に改名。  
　五員環同士の距離分布の出力フォーマット変更。  
　生成式指定画面追加。  
　各種リファクタリング  
　半開状態の Fullerene でも generate/view できるようにした。  
　ヘルプドキュメント更新。  
　カーボンナノチューブの生成は、チューブ方向の長さが長くならない制限を付与。  
　境界の不動性もチェックするようにし、筒の回転軸も表示できるようにした。  
　カーボンナノチューブの生成スクリプト開発。  
　n+m=10 高さ 3 のカーボンナノチューブのリストアップ。  
　フラーレンの表現を改良("b"追加+圧縮)し、ca-generator / ca-uniq の Warning を潰した。  
　ca-uniq の効率化。  
　src/Depend.mk を管理対象から外した。  
　ca-char でファイル名を指定した場合に落ちるのを修正。  
　samples 内の各種ログを更新。  
　C90〜C100までのフラーレン指定メニューの不足分追加。

### 2014/08/14 Version 1.3.0
　Mac 版、mkdir 引数間違いと、povray 起動できない問題を修正。  
　Webkit を使った HelpBrowser を実装し、markdown ファイルを html に変換したものを表示。

### 2014/07/21 Version 1.2.0
　issue30「quaternion → OpenGL の 4x4マトリックス変換が転置？」を修正。  
　issue53「主要対称軸表示でアサート」を修正。  
　issue16「全対称軸の表示」を対応。  
　NoA と対称軸の本数の違いについて解説 [doc/Mathematics-ja.md](Mathematics-ja.md)。  
　3方向線(長径・中径・短径)もスマートな軸に変更。  
　ピッキング範囲を拡大。  
　五員環の色として淡い色を追加。  
　issue56「src/Config.h で３方向線を表示して C200-1 をぐるぐるしたら、突然停止」を修正。  
　issue48 povray 起動に失敗したらダイアログを出力するように修正。  
　issue33 目標FPS/目標CPU使用率対応。ウィンドウタイトルバーにステータス表示。  
　五員環の色はデフォルトで淡くない色。  
　issue58 「freeglut 依存性解消」完了。  
　issue22 Windows 版インストーラに向けて「設定画面を追加」完了。  
　issue65 「中心に移動は指定しなくても良いようにする」完了。

### 2014/06/29 Version 1.1
　フラーレンが収束したら、シミュレーションを停止する。  
　マウス操作がなく、フラーレンが収束したら、描画も停止する。  
　浮動小数点計算は double で実施し、収束判定は float にした。  
　HowToCompile (Mac OS X 10.7.5 with MacPorts 2.3.0編) 追加。  
　Mac 版で glutInit being called a second time のウォーニング修正。

### 2014/06/21
　YouTube に [フラーレンビューア紹介ビデオ](https://www.youtube.com/watch?v=J_qkGaMuqyE) 公開。

### 2014/06/19 Version 1.0
　github にソースコード公開。
