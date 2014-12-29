/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __ABOUTWINDOW_H__
#define __ABOUTWINDOW_H__

#include <QMainWindow>

namespace Ui {
class AboutWindow;
}

class AboutWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = 0);
    ~AboutWindow();
    static void showAbout();

protected:
  virtual void closeEvent(QCloseEvent *);

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::AboutWindow *ui;
};

#endif /* __ABOUTWINDOW_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
