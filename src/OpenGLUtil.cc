/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/24 15:35:29 JST
 */

#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#if defined(__unix) || defined(__APPLE__)
#include <dirent.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif
#include "Vector3.h"
#include "Matrix3.h"
#include "Quaternion.h"
#include "CarbonAllotrope.h"
#include "Fullerene.h"
#include "Ring.h"
#include "Utils.h"
#include "MenuEntry.h"
#include "OpenGLUtil.h"
#include "DebugMemory.h"

int OpenGLUtil::click_x = 0;
int OpenGLUtil::click_y = 0;
int OpenGLUtil::drag_x = 0;
int OpenGLUtil::drag_y = 0;
int OpenGLUtil::release_x = 0;
int OpenGLUtil::release_y = 0;
bool OpenGLUtil::dragging = false;
bool OpenGLUtil::clicked = false;
bool OpenGLUtil::released = false;
Quaternion OpenGLUtil::rotation = Quaternion(1.0, 0.0, 0.0, 0.0);
Quaternion OpenGLUtil::rotation_sub = Quaternion(1.0, 0.0, 0.0, 0.0);
CarbonAllotrope* OpenGLUtil::ca = 0;
Fullerene* OpenGLUtil::fullerene = 0;
bool OpenGLUtil::p_guruguru_mode = true;
bool OpenGLUtil::p_carbon_picking_mode = false;
int OpenGLUtil::p_carbon_picking_sequence_no = 0;
Vector3 OpenGLUtil::p_carbon_picking_point = Vector3();
Vector3 OpenGLUtil::p_last_real_motion = Vector3();
char OpenGLUtil::fullerene_name[1024];
char OpenGLUtil::generator_label[1024];
char OpenGLUtil::window_title[3072];

int OpenGLUtil::view = 40;
static GLfloat lightpos[] = { 0.0, 40.0, 100.0, 1.0 };
#define BUFMAX 100

static void select_hits(int*& selectBuf, int& min, int& sequence_no)
{
  assert(selectBuf[0] <= 1);
  if (selectBuf[0] == 1)
    {
      if (selectBuf[1] < min)
        {
          min = selectBuf[1];
          sequence_no = selectBuf[3];
        }
      selectBuf += 4;
    }
  else
    selectBuf += 3;
}

static void select_hits(int hits, int* selectBuf)
{
  int min = INT_MAX;
  int sequence_no = -1;
  while (hits-- > 0)
    select_hits(selectBuf, min, sequence_no);
  if (sequence_no > 0)
    OpenGLUtil::set_picking_carbon(sequence_no);
}

static void usage(char* argv0)
{
  fprintf(stderr, "usage: %s [[C[int]=]generator_label]\n", argv0);
  exit(1);
}

void OpenGLUtil::initialize_pre(int argc, char *argv[], bool call_glutInit)
{
  if (call_glutInit)
    glutInit(&argc, argv);
  if (argc == 1)
    {
      strcpy(fullerene_name, "C60 (NoA=120)");
      strcpy(generator_label, "S1-5b6c5b6b5b");
    }
  else if (argc == 2)
    {
      switch (argv[1][0])
        {
        case 'C':
          {
            char* ptr = argv[1] + 1;
            int num = atoi(ptr);
            while (*ptr && (*ptr != '=') && (*ptr != ' '))
              ++ptr;
            if (!*ptr)
              usage(argv[0]);
            if (*ptr == '=')
              {
                ++ptr;
                sprintf(fullerene_name, "C%d (NoA=XXX)", num);
                strcpy(generator_label, ptr);
              }
            else
              {
                ++ptr;
                while (*ptr && (*ptr != ' '))
                  ++ptr;
                if (!*ptr)
                  usage(argv[0]);
                strncpy(fullerene_name, argv[1], ptr - argv[1]);
                fullerene_name[ptr - argv[1]] = '\0';
                strcpy(generator_label, ptr + 1);
              }
          }
          break;
        case 'A':
        case 'S':
          strcpy(fullerene_name, "CXX (NoA=XXX)");
          strcpy(generator_label, argv[1]);
          break;
        case 'T':
          strcpy(fullerene_name, "CXX (NoA=XXX)");
          strcpy(generator_label, argv[1]);
          break;
        default:
          usage(argv[0]);
          break;
        }
    }
  else
    usage(argv[0]);
  sprintf(window_title, "%s %s %s", WINDOW_TITLE, fullerene_name, generator_label);
}

void OpenGLUtil::initialize_post()
{
  glClearColor((GLclampf)0.2, (GLclampf)0.2, (GLclampf)0.4, (GLclampf)1.0);
  glEnable(GL_DEPTH_TEST);
#if !defined(AVOID_GLLIGHTMODEL_BUG_IN_WGL)
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
#endif
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL); //マテリアルの設定
  glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE); //マテリアルの設定
  fullerene = new Fullerene(generator_label);
  ca = fullerene->get_carbon_allotrope();
  fullerene->set_fullerene_name(fullerene_name);
  ca->register_interactions();
}

void OpenGLUtil::reshape(int w, int h)
{
  glViewport(0, 0, w, h);  //ビューポートの設定
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, (double)w / (double)h, 1.0, 200.0); //視野の設定
  glMatrixMode(GL_MODELVIEW);
}

void OpenGLUtil::display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, (double)view, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //視点の設定
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos); //ライトの設定
  if (OpenGLUtil::rotate())
    {
      float current_aspect = 0.0;
      int selectBuf[BUFMAX];
      int hits;
      int viewport[4];
      glGetIntegerv(GL_VIEWPORT, viewport);
      glSelectBuffer(BUFMAX, (GLuint*)selectBuf);
      glRenderMode(GL_SELECT);
      glInitNames();
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluPickMatrix(OpenGLUtil::click_x, viewport[3] - OpenGLUtil::click_y - 1,
                    5.0, 5.0, viewport);
      current_aspect = (float)viewport[2] / (float)viewport[3];
      gluPerspective(30.0, current_aspect, 1.0, 100.0);
      glMatrixMode(GL_MODELVIEW);
      OpenGLUtil::ca->draw_by_OpenGL(true);
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      hits = glRenderMode(GL_RENDER);
      select_hits(hits, selectBuf);
      glMatrixMode(GL_MODELVIEW);
    }
  else
    {
      OpenGLUtil::ca->operate_interactions(0.1);
    }
  OpenGLUtil::ca->draw_by_OpenGL(false);
}

void OpenGLUtil::set_color(int color)
{
  glColor3d(((color>>16)&255)/255.0,((color>>8)&255)/255.0,((color>>0)&255)/255.0);
}

void OpenGLUtil::set_color(int color, double alpha)
{
  glColor4d(((color>>16)&255)/255.0,((color>>8)&255)/255.0,((color>>0)&255)/255.0,
            alpha);
}

void OpenGLUtil::draw_sphere(double radius, const Vector3& center)
{
  int slices = 30;
  int stacks = 30;
  glPushMatrix();
  glTranslated(center.x(), center.y(), center.z());
  glFrontFace(GL_CCW);
  glutSolidSphere(radius, slices, stacks);
  glPopMatrix();
}

void OpenGLUtil::draw_cylinder(double radius, const Vector3& from, const Vector3& to)
{
  int slices = 30;
#ifdef FREEGLUT
  int stacks = 30;
#endif
  Vector3 direction = from - to;
  double height = direction.abs();
  double inner = inner_product(Vector3(0.0, 0.0, 1.0), direction) / height;
  double degree = acos(inner) / PAI * 180.0;
  Vector3 axis = exterior_product(Vector3(0.0, 0.0, 1.0), direction);

  glPushMatrix();
  glTranslated(to.x(), to.y(), to.z());
  glRotated(degree, axis.x(), axis.y(), axis.z());
#ifdef FREEGLUT
  glutSolidCylinder(radius, height, slices, stacks);
#else
  glBegin(GL_QUAD_STRIP);
  for (int i = 0; i <= slices; ++i)
    {
      double t = (double)i * 2.0 * PAI / (double)slices;
      glNormal3d(cos(t), sin(t), 0.0);
      glVertex3d(radius * cos(t), radius * sin(t), height);
      glVertex3d(radius * cos(t), radius * sin(t), 0.0);
    }
  glEnd();
#endif
  glPopMatrix();
}

void OpenGLUtil::draw_ring(bool frontface, const Ring* ring)
{
  int num = ring->number_of_carbons();
  Vector3 center = Vector3();
  for (int i = 0; i < num; ++i)
    center += ring->get_carbon(i)->carbon_location();
  center /= (double)num;
  Vector3 loc1 = ring->get_carbon(0)->carbon_location();
  Vector3 loc2 = ring->get_carbon(1)->carbon_location();
  Vector3 loc3 = ring->get_carbon(2)->carbon_location();
  Vector3 normal = ring->get_normal();
  double side = inner_product(exterior_product(loc3 - loc2, loc2 - loc1), normal);
  glFrontFace((side > 0.0) ? GL_CW : GL_CCW);
  glBegin(GL_TRIANGLE_FAN);
#if defined(AVOID_GLLIGHTMODEL_BUG_IN_WGL)
  if (frontface)
#endif
    glNormal3d(normal.x(), normal.y(), normal.z());
#if defined(AVOID_GLLIGHTMODEL_BUG_IN_WGL)
  else
    glNormal3d(-normal.x(), -normal.y(), -normal.z());
#endif
  glVertex3d(center.x(), center.y(), center.z());
  for (int i = 0; i < num; ++i)
    {
      Vector3 loc = ring->get_carbon(i)->carbon_location();
      glVertex3d(loc.x(), loc.y(), loc.z());
    }
  glVertex3d(loc1.x(), loc1.y(), loc1.z());
  glEnd();
}

void OpenGLUtil::draw_XYZ(int light)
{
  set_color(0xffffff & light);
  draw_cylinder(0.1, Vector3(), Vector3(10.0, 0.0, 0.0));
  draw_cylinder(0.1, Vector3(), Vector3(0.0, 10.0, 0.0));
  draw_cylinder(0.1, Vector3(), Vector3(0.0, 0.0, 10.0));
  draw_sphere(0.2, Vector3());
  for (int i = 1; i < 10; ++i)
    {
      set_color(0xff0000 & light);
      draw_sphere(0.2, Vector3(1.0, 0.0, 0.0) * (double)i);
      set_color(0x00ff00 & light);
      draw_sphere(0.2, Vector3(0.0, 1.0, 0.0) * (double)i);
      set_color(0x0000ff & light);
      draw_sphere(0.2, Vector3(0.0, 0.0, 1.0) * (double)i);
    }
}

void OpenGLUtil::semitransparent_mode()
{
  glEnable(GL_BLEND);
  glDepthMask(GL_FALSE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLUtil::backface_mode()
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
}

void OpenGLUtil::frontface_mode()
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

void OpenGLUtil::opaque_mode()
{
  glDisable(GL_CULL_FACE);
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
}

void OpenGLUtil::naming_start(unsigned int label)
{
  glPushName(label);
}

void OpenGLUtil::naming_end()
{
  glPopName();
}

void OpenGLUtil::left_click(int x, int y)
{
  click_x = x;
  click_y = y;
  clicked = true;
  dragging = false;
  released = false;
}

void OpenGLUtil::drag(int x, int y)
{
  if (clicked || dragging)
    {
      drag_x = x;
      drag_y = y;
      clicked = false;
      dragging = true;
    }
}

void OpenGLUtil::left_release(int x, int y)
{
  release_x = x;
  release_y = y;
  if (dragging)
    {
      drag_x = x;
      drag_y = y;
    }
  released = true;
}

void OpenGLUtil::wheel(int direction)
{
  view += (direction ? 2 : -2);
  if (view < 0)
    view = 0;
}

bool OpenGLUtil::rotate()
{
  if (p_carbon_picking_mode)
    {
      Matrix3 mat = Matrix3(rotation);
      glMultMatrixd(mat.to_array44());
      if (!p_carbon_picking_sequence_no)
        return clicked;
      if (dragging)
        {
          Vector3 drag_point = un_project(drag_x, drag_y);
          Vector3 real_motion = drag_point - p_carbon_picking_point;
          Carbon* carbon = ca->get_carbon_by_sequence_no(p_carbon_picking_sequence_no);
          carbon->move_by(real_motion - p_last_real_motion);
          p_last_real_motion = real_motion;
          if (released)
            {
              p_last_real_motion = Vector3();
              p_carbon_picking_sequence_no = 0;
              dragging = false;
              released = false;
            }
        }
      else
        {
          if (released)
            {
              if (clicked)
                clicked = false;
              released = false;
            }
        }
      return false;
    }
  else if (p_guruguru_mode)
    {
      if (dragging)
        {
          Vector3 direction = Vector3(drag_y - click_y, drag_x - click_x, 0.0);
          rotation_sub = Quaternion(direction, direction.abs() * 1.0) * rotation;
          glMultMatrixd(Matrix3(rotation_sub).to_array44());
          if (released)
            {
              rotation = rotation_sub;
              dragging = false;
              released = false;
            }
        }
      else
        {
          glMultMatrixd(Matrix3(rotation).to_array44());
          if (released)
            {
              if (clicked)
                clicked = false;
              released = false;
            }
        }
      return false;
    }
  else
    {
      glMultMatrixd(Matrix3(rotation).to_array44());
      if (dragging)
        {
          if (released)
            {
              dragging = false;
              released = false;
            }
        }
      else
        {
          if (released)
            {
              if (clicked)
                clicked = false;
              released = false;
            }
        }
      return false;
    }
}

void OpenGLUtil::set_guruguru_mode()
{
  p_guruguru_mode = true;
  p_carbon_picking_mode = false;
}

void OpenGLUtil::set_carbon_picking_mode()
{
  if (p_guruguru_mode)
    rotation = rotation_sub;
  p_guruguru_mode = false;
  p_carbon_picking_mode = true;
}

void OpenGLUtil::reset_mode()
{
  if (p_guruguru_mode)
    rotation = rotation_sub;
  p_guruguru_mode = false;
  p_carbon_picking_mode = false;
}

void OpenGLUtil::set_picking_carbon(int sequence_no)
{
  p_carbon_picking_sequence_no = sequence_no;
  p_carbon_picking_point = un_project(click_x, click_y);
}

Vector3 OpenGLUtil::un_project(int win_x, int win_y)
{
  GLdouble model_view[16], projection[16];
  GLint view_port[4];
  double x, y, z;
  glGetIntegerv(GL_VIEWPORT, view_port);
  glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  gluUnProject((double)win_x, (double)(view_port[3] - win_y - 1), 1.0,
               model_view, projection, view_port, &x, &y, &z);
  return Vector3(x, y, z);
}

void OpenGLUtil::change_fullerene(const char* fullerene_name, const char* generator_label)
{
  if (fullerene)
    delete fullerene;
  fullerene = new Fullerene(generator_label);
  fullerene->set_fullerene_name(fullerene_name);
  sprintf(window_title, "%s %s %s", WINDOW_TITLE,
          fullerene->get_fullerene_name(), fullerene->get_generator_label());
  ca = fullerene->get_carbon_allotrope();
  ca->register_interactions();
  rotation = Quaternion(1.0, 0.0, 0.0, 0.0);
  rotation_sub = Quaternion(1.0, 0.0, 0.0, 0.0);
  p_guruguru_mode = false;
  p_carbon_picking_mode = false;
  p_carbon_picking_sequence_no = 0;
  p_last_real_motion = Vector3();
}

int OpenGLUtil::find_unused_file_number(const char* file_name_base)
{
  assert(file_name_base[0] != '/');
  int length = strlen(file_name_base);
  int maxnum = 0;
#if defined(__unix) || defined(__APPLE__)
  DIR* dirp = opendir(".");
  assert(dirp);
  while (1)
    {
      struct dirent entry;
      struct dirent* result;
      assert(readdir_r(dirp, &entry, &result) == 0);
      if (!result)
        {
          closedir(dirp);
          return maxnum;
        }
#define FILE_NAME result->d_name
#endif
#ifdef _WIN32
  WIN32_FIND_DATA fd;
  HANDLE h;

  h = FindFirstFileEx(TEXT("*"), FindExInfoStandard, &fd, FindExSearchNameMatch, NULL, 0);
  assert(h != INVALID_HANDLE_VALUE);
  do
    {
      if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        continue;
      char work[1024];
      ::WideCharToMultiByte(CP_THREAD_ACP,0,fd.cFileName,(int)::wcslen(fd.cFileName)+1,work,1024,NULL,NULL);
#define FILE_NAME work
#endif
      if (strncmp(FILE_NAME, file_name_base, length) != 0)
        continue;
      if (FILE_NAME[length] != '-')
        continue;
      bool found = false;
      for (int numoff = 1; numoff > 0; ++numoff)
        {
          if (FILE_NAME[length + numoff] == '.')
            {
              found = true;
              break;
            }
          if (FILE_NAME[length + numoff] < '0')
            break;
          if (FILE_NAME[length + numoff] > '9')
            break;
        }
      if (!found)
        continue;
      int foundnum = strtol(FILE_NAME + length + 1, NULL, 10);
      if (foundnum >= maxnum)
        maxnum = foundnum + 1;
#if defined(__unix) || defined(__APPLE__)
    }
#endif
#ifdef _WIN32
    }
  while (FindNextFile(h, &fd));
  FindClose(h);
#endif
  return maxnum;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
