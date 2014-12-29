/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __GENERATORFORMULADIALOG_H__
#define __GENERATORFORMULADIALOG_H__

#include <QDialog>

namespace Ui {
  class GeneratorFormulaDialog;
}

class GeneratorFormulaDialog : public QDialog
{
  Q_OBJECT

public:
  explicit GeneratorFormulaDialog(QWidget *parent = 0);
  ~GeneratorFormulaDialog();

private slots:
  void slot_accept();
  void slot_reject();

private:
  Ui::GeneratorFormulaDialog *ui;
};

#endif /* __GENERATORFORMULADIALOG_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
