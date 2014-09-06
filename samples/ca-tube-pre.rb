#!/usr/local/bin/ruby -Ke
# -*- coding: euc-jp -*-
# -*- ruby -*-

## ./ca-tube-pre.rb Tn,m,h
##    n>0,m>=0,h>0 は整数

if ARGV.length != 1 then
  print "usage: ./ca-tube-pre.rb Tn,m,h\n"
  print "    n>0 m>=0 h>0 : integers\n"
  exit 1
end
# $key にフォルダ名を代入する。(例: "T5,5,3")
$key = ARGV[0]
# フォルダがなければ作成する。
Dir.mkdir($key, 0755) if !File.exists?($key)
# フォルダに入る。
Dir.chdir($key)
# work.log ファイルに片側をクローズしたカーボンナノチューブのリストを格納する。
# 片側クローズでは、長さが１増えたカーボンナノチューブをリストアップしない工夫がある。
# 探索の時間短縮にも貢献する。
$command = "ca-loop --close=1 --tube=300 #{$key}- #{$key}-\\* --log=work.log"
print $command,"\n"
system($command)
# close1.log ファイルには work.log の同型を取り除いたカーボンナノチューブを格納する。
$command = "ca-sort < work.log | ca-uniq | grep -v Warning > close1.log"
print $command,"\n"
system($command)
# close1.log にある片側クローズカーボンナノチューブをひとつづつ取り出す。
$fmllogs = []
File.open("close1.log", "r") { |file|
  file.each_line { |line|
    line.chomp!
    if line =~ /^[^ ]+ [^ ]+ ([^ ]+)$/ then
      $fml = $1
      $fmllog = "#{$fml}.log"
# $fmllogs には、片側クローズカーボンナノチューブのもう片方をクローズしたリストファイル名
      $fmllogs.push($fmllog)
# 片側クローズカーボンナノチューブのもう片方をクローズしたリストを格納する。
# もう一方クローズではトータルで長さが１増えたカーボンナノチューブをリストする危惧あり。
# 片側クローズで0.5 増えて、もう一方クローズで 0.5 増えるケース。
      $command = "ca-loop --close=2 --tube=300 #{$fml} #{$fml}\\* --log=#{$fmllog}"
      print $command,"\n"
      system($command)
    end
  }
}
File.unlink("work.log")
# 閉じたカーボンナノチューブが一つもなければ close2.log を空にして終了。
if $fmllogs.length() == 0 then
  File.open("close2.log", "w") { }
  exit 0
end
# 全部の閉じたカーボンナノチューブのリストを close2.log に格納する。
$command = "cat #{$fmllogs.join(' ')} | ca-sort | ca-uniq | grep -v Warning > close2.log"
print $command,"\n"
system($command)

