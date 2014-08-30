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
  if (OpenGLUtil::change_fullerene("", qPrintable(ui->generatorFormulaLineEdit->text())))
    setWindowTitle(OpenGLUtil::window_title);
  accept();
}

void GeneratorFormulaDialog::slot_reject()
{
  reject();
}
