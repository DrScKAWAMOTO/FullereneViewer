#!/usr/local/bin/ruby -Ke
# -*- coding: euc-jp -*-
# -*- ruby -*-

## ../bin/ca-tube-pre.rb Tn,m,h
##    n>0,m>=0,h>0 ������

if ARGV.length != 2 then
  print "usage: ./ca-tube-pre.rb Tn,m,h extension\n"
  print "    n>0 m>=0 h>0 : integers\n"
  print "    extension : \"mirror.gf\" or \"gf\"\n"
  exit 1
end
# $ext �˳�ĥ�Ҥ��������롣(��: "mirror.gf")
$ext = ARGV[1]
# $key �˥ե����̾���������롣(��: "T5,5,3")
$key = ARGV[0]
# �ե�������ʤ���к������롣
Dir.mkdir($key, 0755) if !File.exists?($key)
# �ե���������롣
Dir.chdir($key)
# work.gf �ե��������¦�򥯥������������ܥ�ʥΥ��塼�֤Υꥹ�Ȥ��Ǽ���롣
# ��¦�������Ǥϡ�Ĺ�����������������ܥ�ʥΥ��塼�֤�ꥹ�ȥ��åפ��ʤ����פ����롣
# õ���λ���û�̤ˤ�׸����롣
$command = "ca-loop --close=1 --tube=300 #{$key}- #{$key}-\\* --out=work.#{$ext}"
print $command,"\n"
system($command)
# close1.gf �ե�����ˤ� work.gf ��Ʊ����������������ܥ�ʥΥ��塼�֤��Ǽ���롣
$command = "ca-sort < work.#{$ext} | ca-uniq > close1.#{$ext}"
print $command,"\n"
system($command)
# close1.gf �ˤ�����¦�����������ܥ�ʥΥ��塼�֤�ҤȤĤŤļ��Ф���
$gffiles = []
File.open("close1.#{$ext}", "r") { |file|
  file.each_line { |line|
    line.chomp!
    if line =~ /^[^ ]+ [^ ]+ ([^ ]+)$/ then
      $fml = $1
      $gffile = "#{$fml}.#{$ext}"
# $gffiles �ˤϡ���¦�����������ܥ�ʥΥ��塼�֤Τ⤦�����򥯥��������ꥹ�ȥե�����̾
      $gffiles.push($gffile)
# ��¦�����������ܥ�ʥΥ��塼�֤Τ⤦�����򥯥��������ꥹ�Ȥ��Ǽ���롣
# �⤦�����������Ǥϥȡ������Ĺ�����������������ܥ�ʥΥ��塼�֤�ꥹ�Ȥ���������ꡣ
# ��¦��������0.5 �����ơ��⤦������������ 0.5 �����륱������
      $command = "ca-loop --close=2 --tube=300 #{$fml} #{$fml}\\* --out=#{$gffile}"
      print $command,"\n"
      system($command)
    end
  }
}
File.unlink("work.#{$ext}")
# �Ĥ��������ܥ�ʥΥ��塼�֤���Ĥ�ʤ���� close2.gf ����ˤ��ƽ�λ��
if $gffiles.length() == 0 then
  File.open("close2.#{$ext}", "w") { }
  exit 0
end
# �������Ĥ��������ܥ�ʥΥ��塼�֤Υꥹ�Ȥ� close2.gf �˳�Ǽ���롣
$command = "cat #{$gffiles.join(' ')} | ca-sort | ca-uniq > close2.#{$ext}"
print $command,"\n"
system($command)

