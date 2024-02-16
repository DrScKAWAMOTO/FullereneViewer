/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __HELPBROWSER_H__
#define __HELPBROWSER_H__

#include <QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
#define QWebView QWebEngineView
#define QWebPage QWebEnginePage
#endif
#endif

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

#endif /* __HELPBROWSER_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
