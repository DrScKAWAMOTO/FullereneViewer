#!/usr/local/bin/ruby -Ke
# -*- coding: euc-jp -*-
# -*- ruby -*-

## ../bin/ca-tube-pre.rb Tn,m,h
##    n>0,m>=0,h>0 は整数

if ARGV.length != 2 then
  print "usage: ./ca-tube-pre.rb Tn,m,h extension\n"
  print "    n>0 m>=0 h>0 : integers\n"
  print "    extension : \"mirror.gf\" or \"gf\"\n"
  exit 1
end
# $ext に拡張子を代入する。(例: "mirror.gf")
$ext = ARGV[1]
# $key にフォルダ名を代入する。(例: "T5,5,3")
$key = ARGV[0]
# フォルダがなければ作成する。
Dir.mkdir($key, 0755) if !File.exists?($key)
# フォルダに入る。
Dir.chdir($key)
# work.gf ファイルに片側をクローズしたカーボンナノチューブのリストを格納する。
# 片側クローズでは、長さが１増えたカーボンナノチューブをリストアップしない工夫がある。
# 探索の時間短縮にも貢献する。
$command = "ca-loop --close=1 --tube=300 #{$key}- #{$key}-\\* --out=work.#{$ext}"
print $command,"\n"
system($command)
# close1.gf ファイルには work.gf の同型を取り除いたカーボンナノチューブを格納する。
$command = "ca-sort < work.#{$ext} | ca-uniq > close1.#{$ext}"
print $command,"\n"
system($command)
# close1.gf にある片側クローズカーボンナノチューブをひとつづつ取り出す。
$gffiles = []
File.open("close1.#{$ext}", "r") { |file|
  file.each_line { |line|
    line.chomp!
    if line =~ /^[^ ]+ [^ ]+ ([^ ]+)$/ then
      $fml = $1
      $gffile = "#{$fml}.#{$ext}"
# $gffiles には、片側クローズカーボンナノチューブのもう片方をクローズしたリストファイル名
      $gffiles.push($gffile)
# 片側クローズカーボンナノチューブのもう片方をクローズしたリストを格納する。
# もう一方クローズではトータルで長さが１増えたカーボンナノチューブをリストする危惧あり。
# 片側クローズで0.5 増えて、もう一方クローズで 0.5 増えるケース。
      $command = "ca-loop --close=2 --tube=300 #{$fml} #{$fml}\\* --out=#{$gffile}"
      print $command,"\n"
      system($command)
    end
  }
}
File.unlink("work.#{$ext}")
# 閉じたカーボンナノチューブが一つもなければ close2.gf を空にして終了。
if $gffiles.length() == 0 then
  File.open("close2.#{$ext}", "w") { }
  exit 0
end
# 全部の閉じたカーボンナノチューブのリストを close2.gf に格納する。
$command = "cat #{$gffiles.join(' ')} | ca-sort | ca-uniq > close2.#{$ext}"
print $command,"\n"
system($command)

