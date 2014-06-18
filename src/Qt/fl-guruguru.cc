/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 16:57:28 JST
 */

#include <QtOpenGL>
#include <QBasicTimer>
#include "OpenGLUtil.h"
#include "fl-guruguru.h"

Guruguru::Guruguru(QWidget *parent)
    : QGLWidget(parent), timer(new QBasicTimer)
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
    updateGL();
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
}

void Guruguru::resizeGL(int w, int h)
{
  OpenGLUtil::reshape(w, h);
}

void Guruguru::paintGL()
{
  OpenGLUtil::display();
}
