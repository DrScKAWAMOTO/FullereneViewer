/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/24 15:35:29 JST
 */

#ifndef __OPENGLUTIL_H__
#define __OPENGLUTIL_H__

#include <GL/glut.h>
#ifdef FREEGLUT
#include <GL/freeglut_ext.h>
#endif
#include "Quaternion.h"

#define WINDOW_TITLE "Fullerene Viewer -"
#define DRAWING_THRESHOLD 2

class Vector3;
class CarbonAllotrope;
class Fullerene;
struct MenuEntry;
class Ring;

class OpenGLUtil {

public:
  static int click_x, click_y;
  static int drag_x, drag_y;
  static int release_x, release_y;
  static bool dragging;
  static bool clicked;
  static bool released;
  static Quaternion rotation;
  static Quaternion rotation_sub;
  static CarbonAllotrope* ca;
  static Fullerene* fullerene;
  static char fullerene_name[1024];
  static char generator_label[1024];
  static char window_title[3072];
  static int view;
private:
  static int p_need_drawing;
  static bool p_guruguru_mode;
  static bool p_carbon_picking_mode;
  static int p_carbon_picking_sequence_no;
  static Vector3 p_carbon_picking_point;
  static Vector3 p_last_real_motion;

  // OpenGL initialize
public:
  static void initialize_pre(int argc, char *argv[], bool call_glutInit);
  static void initialize_post();

  // OpenGL reshape
public:
  static void reshape(int w, int h);

  // OpenGL display
public:
  static void display();

  // OpenGL drawing objects
public:
  static void set_color(int color);
  static void set_color(int color, double alpha);
  static void draw_sphere(double radius, const Vector3& center);
  static void draw_cylinder(double radius, const Vector3& from, const Vector3& to);
  static void draw_ring(bool frontface, const Ring* ring);
  static void draw_XYZ(int light);
  static void semitransparent_mode();
  static void backface_mode();
  static void frontface_mode();
  static void opaque_mode();

  // OpenGL selection
public:
  static void naming_start(unsigned int label);
  static void naming_end();

  // display control
public:
  static void stop_drawing();
  static void resume_drawing();

  // OpenGL mouse and keyboard handling
public:
  static void left_click(int x, int y);
  static void drag(int x, int y);
  static void left_release(int x, int y);
  static void wheel(int direction);
  static bool rotate();
  static void set_guruguru_mode();
  static void set_carbon_picking_mode();
  static void reset_mode();
  static void set_picking_carbon(int sequence_no);

  // OpenGL UnProjection
public:
  static Vector3 un_project(int win_x, int win_y);

  // menu common
public:
  static void change_fullerene(const char* fullerene_name, const char* generator_label);

  // file name handling
public:
  static int find_unused_file_number(const char* file_name_base);

  // alert dialog
public:
  static void (*alert_dialog_callback)(const char* message);

};

#endif /* __OPENGLUTIL_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
