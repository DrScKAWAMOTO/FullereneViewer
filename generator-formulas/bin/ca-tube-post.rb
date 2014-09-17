#!/usr/local/bin/ruby -Ke
# -*- coding: euc-jp -*-
# -*- ruby -*-

## ../bin/ca-tube-post.rb Tn,m,h
##    n>0,m>=0,h>0 は整数

if ARGV.length != 2 then
  print "usage: ./ca-tube-post.rb Tn,m,h extension\n"
  print "    n>0 m>=0 h>0 : integers\n"
  print "    extension : \"mirror.gf\" or \"gf\"\n"
  exit 1
end
# $ext に拡張子を代入する。(例: "mirror.gf")
$ext = ARGV[1]
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
$command = "cat #{$key1}/close2.#{$ext} #{$key2}/close2.#{$ext} | ca-sort | ca-uniq | grep #{$key1} > #{$key1}.#{$ext}"
print $command,"\n"
system($command)

