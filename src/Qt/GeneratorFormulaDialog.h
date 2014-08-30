#ifndef GENERATORFORMULADIALOG_H
#define GENERATORFORMULADIALOG_H

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

#endif // GENERATORFORMULADIALOG_H
