/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 16:57:28 JST
 */

#include <QMessageBox>
#include "MainWindow.h"
#include "OpenGLUtil.h"
#include "CarbonAllotrope.h"
#include "Fullerene.h"
#include "Guruguru.h"
#include "QtFullereneMenu.h"
#include "ConfigurationDialog.h"
#include "ui_MainWindow.h"
#include "HelpBrowser.h"

static void alert_dialog(const char* message)
{
  QMessageBox msgBox;
  msgBox.setText(message);
  msgBox.exec();
}

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setCentralWidget(new Guruguru(this));
  setWindowTitle(OpenGLUtil::window_title);
  make_qt_fullerene_menu(this, ui->menu_C);
  connect(ui->action_Z, SIGNAL(triggered()), this, SLOT(setGuruguruMode()));
  connect(ui->action_X, SIGNAL(triggered()), this, SLOT(setPickingMode()));
  connect(ui->action_M, SIGNAL(triggered()), this, SLOT(memoryShape()));
  connect(ui->action_R, SIGNAL(triggered()), this, SLOT(recallShape()));
  connect(ui->action_6, SIGNAL(triggered()), this, SLOT(drawSixViews()));
  connect(ui->action_S, SIGNAL(triggered()), this, SLOT(drawSnapshot()));
  connect(ui->action_C, SIGNAL(triggered()), this, SLOT(setupDialog()));
  connect(ui->action_H, SIGNAL(triggered()), this, SLOT(showHelp()));
  OpenGLUtil::alert_dialog_callback = alert_dialog;
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::setGuruguruMode()
{
  OpenGLUtil::set_guruguru_mode();
}

void MainWindow::setPickingMode()
{
  OpenGLUtil::set_carbon_picking_mode();
}

void MainWindow::memoryShape()
{
  OpenGLUtil::ca->memory_shape(OpenGLUtil::fullerene->get_generator_label());
}

void MainWindow::recallShape()
{
  OpenGLUtil::ca->recall_shape(OpenGLUtil::fullerene->get_generator_label());
}

static void make_file_name(char *file_name)
{
  char* dst = file_name;
  const char* src = OpenGLUtil::fullerene->get_fullerene_name();
  int succeeded = 0;
  while (*src)
    {
      if ((*src == ' ') || (*src == '-') ||
          (*src == '(') || (*src == ')') || (*src == '='))
        {
          if (succeeded == 0)
            *dst++ = '-';
          succeeded = 1;
        }
      else
        {
          *dst++ = *src;
          succeeded = 0;
        }
      ++src;
    }
  if ((dst > file_name) && (dst[-1] != '-'))
    *dst++ = '-';
  strcpy(dst, OpenGLUtil::fullerene->get_generator_label());
}

void MainWindow::drawSixViews()
{
  char work[1000];
  make_file_name(work);
  OpenGLUtil::ca->draw_six_views_by_POVRay(work);
}

void MainWindow::drawSnapshot()
{
  char work[1000];
  make_file_name(work);
  OpenGLUtil::ca->OpenGL_to_POVRay(work, OpenGLUtil::view, OpenGLUtil::rotation);
}

void MainWindow::setupDialog()
{
  ConfigurationDialog *config = new ConfigurationDialog(this);
  int result = config->exec();
  if (result == QDialog::Accepted)
    ;
  else if (result == QDialog::Rejected)
    ;
  delete config;
}

void MainWindow::showHelp()
{
  HelpBrowser::showHelp();
}

void MainWindow::fullereneSelected()
{
  QAction* action = qobject_cast<QAction *>(sender());
  OpenGLUtil::change_fullerene(qPrintable(action->text()),
                               qPrintable(action->statusTip()));
  setWindowTitle(OpenGLUtil::window_title);
}
