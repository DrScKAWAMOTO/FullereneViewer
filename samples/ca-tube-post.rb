#!/usr/local/bin/ruby -Ke
# -*- coding: euc-jp -*-
# -*- ruby -*-

## ./ca-tube-post.rb Tn,m,h
##    n>0,m>=0,h>0 ������

if ARGV.length != 1 then
  print "usage: ./ca-tube-post.rb Tn,m,h\n"
  print "    n>0 m>=0 h>0 : integers\n"
  exit 1
end
# $key1 �˥ե����̾���������롣(��: "T5,5,3")
$key1 = ARGV[0]
# $key2 �˰��Ĺ����Ĺ��̾�����������롣(��: "T5,5,4")
if $key1 =~ /^(T[0-9]+,[0-9]+,)([0-9]+)$/ then
  $base = $1
  $num = $2
  $key2 = $base + ($num.to_i + 1).to_s
else
  print "usage: ./ca-tube-post.rb Tn,m,h\n"
  print "    n>0 m>=0 h>0 : integers\n"
  exit 1
end
# ����Ĺ�����ä���Υ����ܥ�ʥΥ��塼�֤Τߤ�Ĥ���
$command = "cat #{$key1}/close2.log #{$key2}/close2.log | ca-sort | ca-uniq | grep -v Warning | grep #{$key1} > #{$key1}.log"
print $command,"\n"
system($command)

