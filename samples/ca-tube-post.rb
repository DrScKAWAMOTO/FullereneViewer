#!/usr/local/bin/ruby -Ke
# -*- coding: euc-jp -*-
# -*- ruby -*-

## ./ca-tube-post.rb Tn,m,h
##    n>0,m>=0,h>0 は整数

if ARGV.length != 1 then
  print "usage: ./ca-tube-post.rb Tn,m,h\n"
  print "    n>0 m>=0 h>0 : integers\n"
  exit 1
end
# $key1 にフォルダ名を代入する。(例: "T5,5,3")
$key1 = ARGV[0]
# $key2 に一つ長さの長い名前を代入する。(例: "T5,5,4")
if $key1 =~ /^(T[0-9]+,[0-9]+,)([0-9]+)$/ then
  $base = $1
  $num = $2
  $key2 = $base + ($num.to_i + 1).to_s
else
  print "usage: ./ca-tube-post.rb Tn,m,h\n"
  print "    n>0 m>=0 h>0 : integers\n"
  exit 1
end
# 該当長さきっちりのカーボンナノチューブのみを残す。
$command = "cat #{$key1}/close2.log #{$key2}/close2.log | ca-sort | ca-uniq | grep -v Warning | grep #{$key1} > #{$key1}.log"
print $command,"\n"
system($command)

