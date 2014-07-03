/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2014/06/12 06:58:17 JST
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* ca-modeling の POVRay アニメーションで法線を表示する。 */
// #define CONFIG_DRAW_NORMAL_VECTOR_IN_POVRAY_ANIMATION 1

/* fl-guruguru の POVRay 6面展開図で法線を表示する。 */
// #define CONFIG_DRAW_NORMAL_VECTOR_IN_POVRAY_SIX_VIEWS 1

/* fl-guruguru のぐるぐるモードで法線を表示する。 */
// #define CONFIG_DRAW_NORMAL_VECTOR_IN_GURUGURU_MODE 1

/* fl-guruguru のぐるぐるモードで3方向線(長径・中径・短径)を表示する。 */
#define CONFIG_DRAW_PRINCIPAL_COMPONENT_AXES_IN_GURUGURU_MODE 1

/* fl-guruguru のぐるぐるモードで主要対象軸を表示する。 */
// #define CONFIG_DRAW_MAJOR_AXES_SYMMETRY_IN_GURUGURU_MODE 1

/* fl-guruguru のぐるぐるモードで全対象軸を表示する。 */
// #define CONFIG_DRAW_ALL_AXES_SYMMETRY_IN_GURUGURU_MODE 1

/* fl-guruguru の POVRay スナップショットの背景を透明色にする。 */
// #define CONFIG_PAINT_TRANSPARENT_COLOR_TO_BACKGROUND_IN_POVRAY_SNAPSHOT 1

/* fl-guruguru/ca-modeling で五員環にセロファンを貼る。 */
#define CONFIG_PUT_COLORED_CELLOPHANE_IN_5_MEMBER_RING 1

/* fl-guruguru/ca-modeling での五員環セロファンは淡い色。 */
#define CONFIG_COLORED_CELLOPHANE_IS_TINT 1

#endif /* __CONFIG_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
