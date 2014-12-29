/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <assert.h>
#include <QtOpenGL>
#include <QBasicTimer>
#include <QElapsedTimer>
#include <QMainWindow>
#include "Config.h"
#include "OpenGLUtil.h"
#include "Guruguru.h"

static Guruguru *my_guruguru = NULL;

static void static_interval_timer_setup()
{
  if (my_guruguru)
    my_guruguru->interval_timer_setup();
}

Guruguru::Guruguru(QWidget *parent)
  : QGLWidget(parent), timer(new QBasicTimer), etimer(new QElapsedTimer)
{
  my_guruguru = this;
  OpenGLUtil::interval_timer_setup_callback = static_interval_timer_setup;
}

Guruguru::~Guruguru()
{   
  delete timer;
  timer = 0;
}

void Guruguru::update_window_status()
{
  char title_status[WINDOW_TITLE_STATUS_SIZE + 1];
  int flame_rate = 0;
  int cpu_usage_rate = 0;
  if (OpenGLUtil::flame_rate_updateGL <= 0)
    {
      sprintf(title_status, "--f--%%---");
    }
  else if (OpenGLUtil::flame_rate_updateGL <= OpenGLUtil::config_viewer_target_fps)
    {
      flame_rate = OpenGLUtil::flame_rate_updateGL;
      cpu_usage_rate = 99;
      sprintf(title_status, "%2df%2d%%---", flame_rate, cpu_usage_rate);
    }
  else
    {
      flame_rate = OpenGLUtil::config_viewer_target_fps;
      cpu_usage_rate = (100.0 * OpenGLUtil::config_viewer_target_fps /
                        OpenGLUtil::flame_rate_updateGL);
      if (cpu_usage_rate < 0)
        cpu_usage_rate = 0;
      else if (cpu_usage_rate >= 100)
        cpu_usage_rate = 99;
      sprintf(title_status, "%2df%2d%%---", flame_rate, cpu_usage_rate);
    }
  if (OpenGLUtil::picking_done)
    title_status[WINDOW_TITLE_STATUS_SIMULATION] = 'P';
  else if (OpenGLUtil::simulation_done)
    title_status[WINDOW_TITLE_STATUS_SIMULATION] = 'S';
  else
    title_status[WINDOW_TITLE_STATUS_SIMULATION] = '-';
  if (OpenGLUtil::drawing_done)
    title_status[WINDOW_TITLE_STATUS_DRAWING] = 'D';
  else
    title_status[WINDOW_TITLE_STATUS_DRAWING] = '-';
  if (OpenGLUtil::slice == OpenGLUtil::slice_table[0])
    title_status[WINDOW_TITLE_STATUS_RESOLUTION] = 'H';
  else if (OpenGLUtil::slice == OpenGLUtil::slice_table[1])
    title_status[WINDOW_TITLE_STATUS_RESOLUTION] = 'M';
  else
    title_status[WINDOW_TITLE_STATUS_RESOLUTION] = 'L';
  if (strncmp(title_status, OpenGLUtil::get_window_title_status(),
              WINDOW_TITLE_STATUS_SIZE) != 0)
    {
      strncpy(OpenGLUtil::get_window_title_status(), title_status,
              WINDOW_TITLE_STATUS_SIZE);
      parentWidget()->setWindowTitle((char*)OpenGLUtil::window_title);
    }
}

void Guruguru::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    qint64 start_nsecs_etimer;
    qint64 end_nsecs_etimer;
    start_nsecs_etimer = etimer->nsecsElapsed();
    updateGL();
    update_window_status();
    end_nsecs_etimer = etimer->nsecsElapsed();
    qint64 diff_nsecs_etimer;
    diff_nsecs_etimer = end_nsecs_etimer - start_nsecs_etimer;

    OpenGLUtil::elapsed_time_updateGL += diff_nsecs_etimer;
    OpenGLUtil::count_updateGL++;
}

void Guruguru::mousePressEvent(QMouseEvent* e)
{
  if (e->button() == Qt::LeftButton)
    OpenGLUtil::left_click(e->x(), e->y());
}

void Guruguru::mouseReleaseEvent(QMouseEvent* e)
{
  if (e->button() == Qt::LeftButton)
    OpenGLUtil::left_release(e->x(), e->y());
}

void Guruguru::mouseMoveEvent(QMouseEvent* e)
{
  OpenGLUtil::drag(e->x(), e->y());
}

void Guruguru::wheelEvent(QWheelEvent* e)
{
  OpenGLUtil::wheel(e->delta() > 0);
}

void Guruguru::initializeGL()
{
  OpenGLUtil::initialize_post();
  //タイマーの開始
  interval_timer_setup();
  //経過タイマーの開始
  etimer->start();
}

void Guruguru::interval_timer_setup()
{
  //間隔を 1000 / OpenGLUtil::config_viewer_target_fps (切り捨て) に設定 issue#33
  //60fps の場合、16msに設定
  timer->start(1000 / OpenGLUtil::config_viewer_target_fps, this);
}

void Guruguru::resizeGL(int w, int h)
{
  OpenGLUtil::reshape(w, h);
}

void Guruguru::paintGL()
{
  OpenGLUtil::display();
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
