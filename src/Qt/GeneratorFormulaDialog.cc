/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <QString>
#include <QRegularExpression>
#include "GeneratorFormulaDialog.h"
#include "ui_GeneratorFormulaDialog.h"
#include "OpenGLUtil.h"

GeneratorFormulaDialog::GeneratorFormulaDialog(QWidget *parent)
  : QDialog(parent), ui(new Ui::GeneratorFormulaDialog)
{
  ui->setupUi(this);
  ui->generatorFormulaLineEdit->setText(QString((char*)OpenGLUtil::generator_formula));

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
  formula.remove(QRegularExpression("[\n\r\t]"));
  if (OpenGLUtil::change_fullerene(0, qPrintable(formula)))
    setWindowTitle((char*)OpenGLUtil::window_title);
  accept();
}

void GeneratorFormulaDialog::slot_reject()
{
  reject();
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
