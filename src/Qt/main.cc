/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2014/05/28 22:36:40 JST
 */

#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui/QApplication>
#else
#include <QtWidgets/QApplication>
#endif
#include <QTextCodec>
#include <stdlib.h>
#include "Version.h"
#include "mainwindow.h"
#include "OpenGLUtil.h"

int main(int argc, char *argv[])
{
  if ((argc >= 2) &&
      ((strcmp(argv[1], "--version") == 0) || (strcmp(argv[1], "-v") == 0)))
    {
      print_version("FullereneViewer Ver 1.1");
      exit(0);
    }

  QString home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  QString path = home.append("/fullerene");
  QDir dir = QDir(home);
  dir.mkdir(path);
  QDir::setCurrent(path);

  QApplication a(argc, argv);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("SJIS"));
#endif
  OpenGLUtil::initialize_pre(argc, argv);
  MainWindow w;
  w.showMaximized();
  return a.exec();
}
