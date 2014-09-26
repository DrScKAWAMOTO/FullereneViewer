# FullereneViewer(フラーレンビューア)
## リリースノート

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
