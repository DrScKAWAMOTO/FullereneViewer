/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
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
      print_version("FullereneViewer");
      exit(0);
    }

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#if defined(Q_OS_DARWIN) || defined(Q_OS_UNIX)
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

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
