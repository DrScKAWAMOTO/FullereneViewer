# FullereneViewer(フラーレンビューア)
## 失敗手法

### 非対称生成アプローチにて、五員環を追加したら五員環のまわりに六員環を埋める手法
　`CarbonAllotrope::fill_n_polygon_around_oldest_carbon(number_of_carbons)` を実施した後、number_of_carbons が 5 に等しければ、以下の孤立五員環ルール対応を実施する。

* 孤立五員環ルール対応

    追加した ring を検出し、`CarbonAllotrope::p_fill_hexagons_around(ring)` 呼び出し

　`A0-...` として実装する。

#### 問題
　`ca-generator --ordinary=60 A0- >! foo.lst` として、出力されるログの以下に注目。

    * detail ***************************************
    C38(1) C45(1) 
    number of bonds = 87
    number of carbons = 60
    number of carbons with one ring = 6
    number of carbons with two rings = 9
    number of carbons with three rings = 45
    number of 5-membered rings = 9
    number of 6-membered rings = 19
    Euler characteristic = 1
    boundary =
      dist 1  1  2  2  1  1  1  2  2  1  1  1  2  2  1  
      carb C--C--C--C--C--C--C--C--C--C--C--C--C--C--C--
       NO. |38|41 60 59|55|52|53 57 58|47|44|45 49 50|39
      ring |6R|6R      |6R|6R|6R      |6R|6R|6R      |6R
       NO. |16|28      |24|23|26      |19|18|21      |15
      carb C  C        C  C  C        C  C  C        C  
       NO.  37 42       56 51 54       48 43 46       40
    distances to pentagons = 000000000000000000000000000000000000011010011010220110102222
    clockwise = ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ************************************************
    Carbon(38) ... 5-polygon
    Open concave boundary segment (length=4) C60-...-C50
    Open concave boundary segment (length=3) C49-...-C59
    * detail ***************************************
    C44(1) 
    number of bonds = 91
    number of carbons = 62
    number of carbons with one ring = 4
    number of carbons with two rings = 8
    number of carbons with three rings = 50
    number of 5-membered rings = 10
    number of 6-membered rings = 20
    Euler characteristic = 1
    boundary =
      dist 1  1  2  2  1  1  1  1  2  2  1  1  
      carb C--C--C--C--C--C--C--C--C--C--C--C--
       NO. |44|47 58 57|53|52|55|59 61 62|49|45
      ring |6R|6R      |6R|6R|6R|6R      |6R|6R
       NO. |19|26      |23|24|28|30      |21|18
      carb C  C        C  C  C  C        C  C  
       NO.  43 48       54 51 56 60       50 46
    distances to pentagons = 00000000000000000000000000000000000000000001101010011010221022
    clockwise = ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ************************************************
    ************************************************
    * ERROR A0-5j Too many carbons 62
    ************************************************

　上の detail は、六員環のまわりに３つの五員環がくっついている複体を、C60 から取り除いた複体である。これに五員環と六員環をひとつづつくっつけたのが下の detail であり、二つの五角形からなる境界になるはずが、そうなっていない！！

#### 原因
　五員環のまわりに六員環を並べる作業が間違い。この段階では、六員環は一番凹んでいる部分ではないのに、並べてしまっている。

#### 結論
　この方法は却下。




