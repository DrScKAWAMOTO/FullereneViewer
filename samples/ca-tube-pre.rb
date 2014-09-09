#!/usr/local/bin/ruby -Ke
# -*- coding: euc-jp -*-
# -*- ruby -*-

## ./ca-tube-pre.rb Tn,m,h
##    n>0,m>=0,h>0 ������

if ARGV.length != 1 then
  print "usage: ./ca-tube-pre.rb Tn,m,h\n"
  print "    n>0 m>=0 h>0 : integers\n"
  exit 1
end
# $key �˥ե����̾���������롣(��: "T5,5,3")
$key = ARGV[0]
# �ե�������ʤ���к������롣
Dir.mkdir($key, 0755) if !File.exists?($key)
# �ե���������롣
Dir.chdir($key)
# work.log �ե��������¦�򥯥������������ܥ�ʥΥ��塼�֤Υꥹ�Ȥ��Ǽ���롣
# ��¦�������Ǥϡ�Ĺ�����������������ܥ�ʥΥ��塼�֤�ꥹ�ȥ��åפ��ʤ����פ����롣
# õ���λ���û�̤ˤ�׸����롣
$command = "ca-loop --close=1 --tube=300 #{$key}- #{$key}-\\* --log=work.log"
print $command,"\n"
system($command)
# close1.log �ե�����ˤ� work.log ��Ʊ����������������ܥ�ʥΥ��塼�֤��Ǽ���롣
$command = "ca-sort < work.log | ca-uniq | grep -v Warning > close1.log"
print $command,"\n"
system($command)
# close1.log �ˤ�����¦�����������ܥ�ʥΥ��塼�֤�ҤȤĤŤļ��Ф���
$fmllogs = []
File.open("close1.log", "r") { |file|
  file.each_line { |line|
    line.chomp!
    if line =~ /^[^ ]+ [^ ]+ ([^ ]+)$/ then
      $fml = $1
      $fmllog = "#{$fml}.log"
# $fmllogs �ˤϡ���¦�����������ܥ�ʥΥ��塼�֤Τ⤦�����򥯥��������ꥹ�ȥե�����̾
      $fmllogs.push($fmllog)
# ��¦�����������ܥ�ʥΥ��塼�֤Τ⤦�����򥯥��������ꥹ�Ȥ��Ǽ���롣
# �⤦�����������Ǥϥȡ������Ĺ�����������������ܥ�ʥΥ��塼�֤�ꥹ�Ȥ���������ꡣ
# ��¦��������0.5 �����ơ��⤦������������ 0.5 �����륱������
      $command = "ca-loop --close=2 --tube=300 #{$fml} #{$fml}\\* --log=#{$fmllog}"
      print $command,"\n"
      system($command)
    end
  }
}
File.unlink("work.log")
# �Ĥ��������ܥ�ʥΥ��塼�֤���Ĥ�ʤ���� close2.log ����ˤ��ƽ�λ��
if $fmllogs.length() == 0 then
  File.open("close2.log", "w") { }
  exit 0
end
# �������Ĥ��������ܥ�ʥΥ��塼�֤Υꥹ�Ȥ� close2.log �˳�Ǽ���롣
$command = "cat #{$fmllogs.join(' ')} | ca-sort | ca-uniq | grep -v Warning > close2.log"
print $command,"\n"
system($command)

