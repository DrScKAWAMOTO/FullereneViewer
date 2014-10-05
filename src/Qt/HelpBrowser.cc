/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 16:57:28 JST
 */

#include <QtWidgets>
#include <QtWebKitWidgets>
#include "HelpBrowser.h"

static HelpBrowser *helpBrowser = NULL;

HelpBrowser::HelpBrowser(const QUrl& url)
{
  view = new QWebView(this);
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
