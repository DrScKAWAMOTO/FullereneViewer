/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2014/05/28 22:36:40 JST
 */

#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui/QApplication>
#else
#include <QtCore/QStandardPaths>
#include <QtWidgets/QApplication>
#endif
#include <QTextCodec>
#include <stdlib.h>
#include "Version.h"
#include "MainWindow.h"
#include "OpenGLUtil.h"
#include "Configuration.h"

int main(int argc, char *argv[])
{
  if ((argc >= 2) &&
      ((strcmp(argv[1], "--version") == 0) || (strcmp(argv[1], "-v") == 0)))
    {
      print_version("FullereneViewer Ver 1.2.0");
      exit(0);
    }

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#if defined(__APPLE__) || defined(__unix)
  QString home = getenv("HOME");
  QString desktop = home + "/Desktop";
#else
# error "Broken under Windows + Qt4."
#endif
#else
  QString home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
#endif
  configuration = new Configuration(home.toLocal8Bit().data(),
                                    desktop.toLocal8Bit().data());
  configuration->load();
  QApplication a(argc, argv);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("SJIS"));
#endif
  OpenGLUtil::initialize_pre(argc, argv);
  MainWindow w;
  configuration->reflect();
  w.showMaximized();
  return a.exec();
}
