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

void Guruguru::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    qint64 start_nsecs_etimer;
    qint64 end_nsecs_etimer;
    start_nsecs_etimer = etimer->nsecsElapsed();
    updateGL();
    end_nsecs_etimer = etimer->nsecsElapsed();
    qint64 diff_nsecs_etimer;
    diff_nsecs_etimer = end_nsecs_etimer - start_nsecs_etimer;

    if (OpenGLUtil::picking_done)
      {
        assert(OpenGLUtil::drawing_done);
        elapsed_time_sum_picking_and_drawing += diff_nsecs_etimer;
        picking_and_drawing_count++;
      }
    else if (OpenGLUtil::simulation_done)
      {
        assert(OpenGLUtil::drawing_done);
        elapsed_time_sum_simulation_and_drawing += diff_nsecs_etimer;
        simulation_and_drawing_count++;
      }
    else if (OpenGLUtil::drawing_done)
      {
        elapsed_time_sum_drawing_only += diff_nsecs_etimer;
        drawing_only_count++;
      }
    if (picking_and_drawing_count)
      printf("time(picking drawing) = %f\n", (double)elapsed_time_sum_picking_and_drawing / (double)picking_and_drawing_count);
    if (simulation_and_drawing_count)
      printf("time(simulation drawing) = %f\n", (double)elapsed_time_sum_simulation_and_drawing / (double)simulation_and_drawing_count);
    if (drawing_only_count)
      printf("time(drawing only) = %f\n", (double)elapsed_time_sum_drawing_only / (double)drawing_only_count);
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
  //タイマーの開始、間隔を16ms(≒60fps)に設定
  timer->start(16, this);
  //経過タイマーの開始
  etimer->start();
  elapsed_time_sum_picking_and_drawing = 0.0;
  picking_and_drawing_count = 0;
  elapsed_time_sum_simulation_and_drawing = 0.0;
  simulation_and_drawing_count = 0;
  elapsed_time_sum_drawing_only = 0.0;
  drawing_only_count = 0;
}

void Guruguru::resizeGL(int w, int h)
{
  OpenGLUtil::reshape(w, h);
}

void Guruguru::paintGL()
{
  OpenGLUtil::display();
}
