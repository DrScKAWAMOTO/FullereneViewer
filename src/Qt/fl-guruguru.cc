/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 16:57:28 JST
 */

#include <assert.h>
#include <QtOpenGL>
#include <QBasicTimer>
#include <QElapsedTimer>
#include <QMainWindow>
#include "Config.h"
#include "OpenGLUtil.h"
#include "fl-guruguru.h"

Guruguru::Guruguru(QWidget *parent)
  : QGLWidget(parent), timer(new QBasicTimer), etimer(new QElapsedTimer)
{
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
  else if (OpenGLUtil::flame_rate_updateGL <= CONFIG_GURUGURU_TARGET_FPS)
    {
      flame_rate = OpenGLUtil::flame_rate_updateGL;
      cpu_usage_rate = 99;
      sprintf(title_status, "%2df%2d%%---", flame_rate, cpu_usage_rate);
    }
  else
    {
      flame_rate = CONFIG_GURUGURU_TARGET_FPS;
      cpu_usage_rate = (100.0 * CONFIG_GURUGURU_TARGET_FPS /
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
  switch (OpenGLUtil::slices_and_stacks)
    {
    case 30:
    default:
      title_status[WINDOW_TITLE_STATUS_RESOLUTION] = 'H';
      break;
    case 20:
      title_status[WINDOW_TITLE_STATUS_RESOLUTION] = 'M';
      break;
    case 10:
      title_status[WINDOW_TITLE_STATUS_RESOLUTION] = 'L';
      break;
    }
  if (strncmp(title_status, OpenGLUtil::window_title_status,
              WINDOW_TITLE_STATUS_SIZE) != 0)
    {
      strncpy(OpenGLUtil::window_title_status, title_status, WINDOW_TITLE_STATUS_SIZE);
      parentWidget()->setWindowTitle(OpenGLUtil::window_title);
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
  //間隔を 1000 / CONFIG_GURUGURU_TARGET_FPS (切り捨て) に設定 issue#33
  //60fps の場合、16msに設定
  timer->start(1000 / CONFIG_GURUGURU_TARGET_FPS, this);
  //経過タイマーの開始
  etimer->start();
}

void Guruguru::resizeGL(int w, int h)
{
  OpenGLUtil::reshape(w, h);
}

void Guruguru::paintGL()
{
  OpenGLUtil::display();
}
