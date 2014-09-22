#include "ConfigurationDialog.h"
#include "ui_ConfigurationDialog.h"
#include "Configuration.h"

ConfigurationDialog::ConfigurationDialog(QWidget *parent)
  : QDialog(parent), ui(new Ui::ConfigurationDialog)
{
  ui->setupUi(this);
  switch (configuration->get_picture_quality())
    {
    case QUALITY_HIGH:
      ui->pictureHighRadioButton->setChecked(true);
      break;
    case QUALITY_MIDDLE:
    case QUALITY_LOW:
      ui->pictureLowRadioButton->setChecked(true);
      break;
    }
  switch (configuration->get_motion_quality())
    {
    case QUALITY_HIGH:
      ui->motionHighRadioButton->setChecked(true);
      break;
    case QUALITY_MIDDLE:
      ui->motionMiddleRadioButton->setChecked(true);
      break;
    case QUALITY_LOW:
      ui->motionLowRadioButton->setChecked(true);
      break;
    }
  ui->workingFolderLineEdit->setText(QString(configuration->get_working_folder_name()));
  ui->povrayCommandLineLineEdit->setText(QString(configuration->get_povray_command_line()));

  connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slot_accept()));
  connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(slot_reject()));
}

ConfigurationDialog::~ConfigurationDialog()
{
  delete ui;
}

void ConfigurationDialog::slot_accept()
{
  if (ui->pictureHighRadioButton->isChecked())
    configuration->set_picture_quality(QUALITY_HIGH);
  else
    configuration->set_picture_quality(QUALITY_LOW);
  if (ui->motionHighRadioButton->isChecked())
    configuration->set_motion_quality(QUALITY_HIGH);
  else if (ui->motionMiddleRadioButton->isChecked())
    configuration->set_motion_quality(QUALITY_MIDDLE);
  else
    configuration->set_motion_quality(QUALITY_LOW);
  configuration->set_working_folder_name(qPrintable(ui->workingFolderLineEdit->text()));
  configuration->set_povray_command_line(qPrintable(ui->povrayCommandLineLineEdit->text()));

  configuration->save();
  configuration->reflect();
  accept();
}

void ConfigurationDialog::slot_reject()
{
  reject();
}
