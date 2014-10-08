/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* ca-modeling の POVRay アニメーションで法線を表示する。 */
// #define CONFIG_DRAW_NORMAL_VECTOR_IN_POVRAY_ANIMATION 1

/* FullereneViewer の POVRay 6面展開図で法線を表示する。 */
// #define CONFIG_DRAW_NORMAL_VECTOR_IN_POVRAY_SIX_VIEWS 1

/* FullereneViewer のぐるぐるモードで法線を表示する。 */
// #define CONFIG_DRAW_NORMAL_VECTOR_IN_GURUGURU_MODE 1

/* FullereneViewer のぐるぐるモードで3方向線(長径・中径・短径)を表示する。 */
// #define CONFIG_DRAW_PRINCIPAL_COMPONENT_AXES_IN_GURUGURU_MODE 1

/* FullereneViewer の POVRay スナップショットの背景を透明色にする。 */
// #define CONFIG_PAINT_TRANSPARENT_COLOR_TO_BACKGROUND_IN_POVRAY_SNAPSHOT 1

/* FullereneViewer/ca-modeling で五員環にセロファンを貼る。 */
#define CONFIG_PUT_COLORED_CELLOPHANE_IN_5_MEMBER_RING 1

/* FullereneViewer/ca-modeling での五員環セロファンは淡い色。 */
// #define CONFIG_COLORED_CELLOPHANE_IS_TINT 1

/* FullereneViewer のぐるぐるモードでの目標CPU使用率(単位は%)を設定する。 */
/* 値は 1 から 99 までの整数値 */
#define CONFIG_VIEWER_CPU_USAGE_TARGET_RATE 80

/* 以下のマクロ定義を有効化すると、 */
/* ca-generator の対称フラーレン生成は、五員環への距離を元に次の生成箇所を決定する。 */
// #define CONFIG_SYMMETRIC_GENERATOR_DECIDES_NEXT_FILL_POINT_BY_DISTANCES_TO_PENTAGON 1
/* scrap_order を元に次の生成箇所を決定する場合は上記マクロをコメントアウトする。 */

#endif /* __CONFIG_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
