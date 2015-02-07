/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

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
  void setupDialog();
  void selectFullereneDialog();
  void formulaDialog();
  void setGuruguruMode();
  void setPickingMode();
  void turnInsideOut();
  void deleteTheFarMostRing();
  void switchDisplayingObjects();
  void memoryShape();
  void recallShape();
  void drawSixViews();
  void drawSnapshot();
  void showHelp();
  void showAbout();

private:
  Ui::MainWindow *ui;
};

#endif /* __MAINWINDOW_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
