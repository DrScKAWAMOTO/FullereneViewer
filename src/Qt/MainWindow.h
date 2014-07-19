/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 16:57:28 JST
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
    
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
    
private slots:
  void setGuruguruMode();
  void centering();
  void setPickingMode();
  void memoryShape();
  void recallShape();
  void drawSixViews();
  void drawSnapshot();
  void setupDialog();
  void fullereneSelected();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
