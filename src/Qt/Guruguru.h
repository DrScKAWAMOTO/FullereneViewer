/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 16:57:28 JST
 */

#ifndef __GURUGURU_H__
#define __GURUGURU_H__

#include <QGLWidget>

class QBasicTimer;
class QElapsedTimer;

class Guruguru : public QGLWidget
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
