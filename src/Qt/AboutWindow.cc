/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <QPainter>
#include "Version.h"
#include "AboutWindow.h"
#include "ui_AboutWindow.h"

static AboutWindow *aboutWindow = NULL;

AboutWindow::AboutWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::paintEvent(QPaintEvent *)
{
    ui->label->setText("FullereneViewer Ver " FULLERENE_VIEWER_VERSION);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawImage(0, 50, QImage(":/help/media/FullereneViewer.png"));
}
void AboutWindow::showAbout()
{
  if (aboutWindow)
    aboutWindow->raise();
  else
    {
      aboutWindow = new AboutWindow();
      aboutWindow->show();
    }
}

void AboutWindow::closeEvent(QCloseEvent *event)
{
  aboutWindow = NULL;
  QMainWindow::closeEvent(event);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
