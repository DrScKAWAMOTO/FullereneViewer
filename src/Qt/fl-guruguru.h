/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 16:57:28 JST
 */

#ifndef FL_GURUGURU_H
#define FL_GURUGURU_H

#include <QGLWidget>
class QBasicTimer;

class Guruguru : public QGLWidget
{
  Q_OBJECT
    
public:
  Guruguru(QWidget *parent = 0);
  ~Guruguru();

protected:
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
};

#endif // FL_GURUGURU_H
