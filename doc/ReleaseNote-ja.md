# FullereneViewer(フラーレンビューア)
## リリースノート

### 2014/08/26 Version 1.3.1
　draw_opaque_by_OpenGL() 引数間違い修正。__
　ca シリーズのデバッグ。__
　FullereneViewr について画面追加。

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
