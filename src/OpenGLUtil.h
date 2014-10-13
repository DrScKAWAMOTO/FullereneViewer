/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __OPENGLUTIL_H__
#define __OPENGLUTIL_H__

#if __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif
#include "Quaternion.h"
#include "MyString.h"

#define WINDOW_TITLE "Fullerene Viewer [--f--%---]"
#define WINDOW_TITLE_STATUS_FPS 0
#define WINDOW_TITLE_STATUS_CPU_USAGE_RATE 3
#define WINDOW_TITLE_STATUS_SIMULATION 6
#define WINDOW_TITLE_STATUS_DRAWING 7
#define WINDOW_TITLE_STATUS_RESOLUTION 8
#define WINDOW_TITLE_STATUS_SIZE 9
#define DRAWING_THRESHOLD 2

class Vector3;
class CarbonAllotrope;
class Fullerene;
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
  static MyString fullerene_name;
  static MyString generator_formula;
  static MyString window_title;
  static char* get_window_title_status();
  static int view;

  // picking simulation drawing
  static bool picking_done;
  static bool simulation_done;
  static bool drawing_done;

  // control slice by flame-rate, farness, configuration
  static int config_viewer_target_fps;
  static double elapsed_time_updateGL;
  static int count_updateGL;
  static int flame_rate_updateGL;
  static int adjustment_from_flame_rate;
  static int slice;
  static double adjust_forwarding_threshold;
  static double adjust_backwarding_threshold;
  static const int size_of_slice_table;
  static const int slice_table[];
  static const double sin_table[];
  static const double * const cos_table;

private:
  static int p_need_drawing;
  static bool p_guruguru_mode;
  static bool p_carbon_picking_mode;
  static int p_carbon_picking_sequence_no;
  static Vector3 p_carbon_picking_point;
  static Vector3 p_last_real_motion;

  // OpenGL initialize
public:
  static void initialize_pre(int argc, char *argv[]);
  static void initialize_post();

  // OpenGL reshape
public:
  static void reshape(int w, int h);

  // OpenGL display
public:
  static void display();

  // control slices and stacks
public:
  static bool control_slice();

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
  static bool change_fullerene(const char* fullerene_name, const char* generator_formula);
  static bool rebuild_fullerene();

  // file name handling
public:
  static int find_unused_file_number(const char* file_name_base);

  // alert dialog
public:
  static void (*alert_dialog_callback)(const char* message);

  // interval timer setup
public:
  static void (*interval_timer_setup_callback)();

};

#endif /* __OPENGLUTIL_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
