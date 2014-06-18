# FullereneViewer
## フラーレンビューア
### コントリビュータ向けの参考情報

#### ソースファイルを追加する場合
##### 1) 追加作業
　`FullereneViewer/src` か `FullereneViewer/src/Qt` に *.cc か *.h を追加する。  
　何らかまとまりのあるソースファイル群の場合は、 `FullereneViewer/src` 内にディレクトリを作って、その中に追加してください。

##### 2) メイク作業
　`fl-guruguru` に対して追加したのなら、`Fullerene/src/Qt/fl-guruguru.pro` に追加したファイル名を書き足して下さい。  
　`ca-*.cc` シリーズに対して追加し、かつ、ディレクトリを作成した場合は、`Fullerene/src/Makefile` を書き換えて下さい。
