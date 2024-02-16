/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include "HelpBrowser.h"
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <Qt/QtWebKit>
#else
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
#include <QtWebKitWidgets>
#else
#include <QtWebEngineWidgets>
#define QWebView QWebEngineView
#endif
#endif

static HelpBrowser *helpBrowser = NULL;

HelpBrowser::HelpBrowser(const QUrl& url)
{
  view = new QWebEngineView(this);
  view->load(url);
  QToolBar *toolBar = addToolBar(tr("Navigation"));
  toolBar->addAction(view->pageAction(QWebPage::Back));
  toolBar->addAction(view->pageAction(QWebPage::Forward));
  setCentralWidget(view);
}

void HelpBrowser::showHelp()
{
  if (helpBrowser)
    helpBrowser->raise();
  else
    {
      helpBrowser = new HelpBrowser(QUrl("qrc:/help/README.html"));
      helpBrowser->resize(480, 640);
      helpBrowser->show();
    }
}

void HelpBrowser::closeEvent(QCloseEvent *event)
{
  helpBrowser = NULL;
  QMainWindow::closeEvent(event);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
