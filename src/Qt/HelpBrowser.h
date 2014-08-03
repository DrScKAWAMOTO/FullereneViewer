/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 16:57:28 JST
 */

#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <QtWidgets>

class QWebView;
QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class HelpBrowser : public QMainWindow
{
  Q_OBJECT

public:
  HelpBrowser(const QUrl& url);
  static void showHelp();

protected:
  virtual void closeEvent(QCloseEvent *);

private:
  QWebView *view;
};

#endif // HELPBROWSER_H
