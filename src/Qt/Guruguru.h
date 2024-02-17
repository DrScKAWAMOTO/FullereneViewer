/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __GURUGURU_H__
#define __GURUGURU_H__

#include <QtCore/QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#include <QOpenGLWidget>
#else
#include <QGLWidget>
#endif


class QBasicTimer;
class QElapsedTimer;

class Guruguru :
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
 public QOpenGLWidget
#else
 public QGLWidget
#endif
{
  Q_OBJECT

public:
  Guruguru(QWidget *parent = 0);
  ~Guruguru();
  void interval_timer_setup();

protected:
  void update_window_status();
  void timerEvent(QTimerEvent *e);
  void mousePressEvent(QMouseEvent* e);
  void mouseReleaseEvent(QMouseEvent* e);
  void mouseMoveEvent(QMouseEvent* e);
  void wheelEvent(QWheelEvent* e);
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

private:
  QBasicTimer* timer;
  // calculate flame-rate
  QElapsedTimer* etimer;
};

#endif /* __GURUGURU_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
