/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __CONFIGURATIONDIALOG_H__
#define __CONFIGURATIONDIALOG_H__

#include <QDialog>

namespace Ui {
  class ConfigurationDialog;
}

class ConfigurationDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ConfigurationDialog(QWidget *parent = 0);
  ~ConfigurationDialog();

private slots:
  void slot_accept();
  void slot_reject();

private:
  Ui::ConfigurationDialog *ui;
};

#endif /* __CONFIGURATIONDIALOG_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
