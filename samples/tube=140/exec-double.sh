#! /bin/sh
ca-loop --close=2 --tube=140 T10,0,3- T10,0,3-8a --log=T10,0,3.log
ca-loop --close=2 --tube=140 T9,1,3- T9,1,3-8a --log=T9,1,3.log
ca-loop --close=2 --tube=140 T8,2,3- T8,2,3-8a --log=T8,2,3.log
ca-loop --close=2 --tube=140 T7,3,3- T7,3,3-8a --log=T7,3,3.log
ca-loop --close=2 --tube=140 T6,4,3- T6,4,3-8a --log=T6,4,3.log
ca-loop --close=2 --tube=140 T5,5,3- T5,5,3-8a --log=T5,5,3.log

