/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 16:57:28 JST
 */

#ifndef __SELECTFULLERENEDIALOG_H__
#define __SELECTFULLERENEDIALOG_H__

#include <QDialog>
#include "TreeModel.h"

namespace Ui {
class SelectFullereneDialog;
}

class SelectFullereneDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SelectFullereneDialog(QWidget *parent = 0);
  ~SelectFullereneDialog();

private slots:
  void slot_accept();
  void slot_reject();

private:
  Ui::SelectFullereneDialog *ui;
  TreeModel model;

};

#endif /* __SELECTFULLERENEDIALOG_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
