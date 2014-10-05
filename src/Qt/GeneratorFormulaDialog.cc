/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 16:57:28 JST
 */

#include <QString>
#include <QRegExp>
#include "GeneratorFormulaDialog.h"
#include "ui_GeneratorFormulaDialog.h"
#include "OpenGLUtil.h"

GeneratorFormulaDialog::GeneratorFormulaDialog(QWidget *parent)
  : QDialog(parent), ui(new Ui::GeneratorFormulaDialog)
{
  ui->setupUi(this);
  ui->generatorFormulaLineEdit->setText(QString(""));

  connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slot_accept()));
  connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(slot_reject()));
}

GeneratorFormulaDialog::~GeneratorFormulaDialog()
{
  delete ui;
}
void GeneratorFormulaDialog::slot_accept()
{
  QString formula = ui->generatorFormulaLineEdit->text();
  formula.remove(QRegExp("[\n\r\t]"));
  if (OpenGLUtil::change_fullerene("", qPrintable(formula)))
    setWindowTitle(OpenGLUtil::window_title);
  accept();
}

void GeneratorFormulaDialog::slot_reject()
{
  reject();
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
