/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include "ConfigurationDialog.h"
#include "ui_ConfigurationDialog.h"
#include "Configuration.h"
#include "OpenGLUtil.h"

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
  switch (configuration->get_display_symmetry_axes())
    {
    case DISPLAY_ALL_SYMMETRY_AXES:
      ui->displayAllSymmetryAxesButton->setChecked(true);
      break;
    case DISPLAY_MAJOR_SYMMETRY_AXES:
      ui->displayMajorSymmetryAxesButton->setChecked(true);
      break;
    case DISPLAY_NO_SYMMETRY_AXES:
      ui->displayNoSymmetryAxesButton->setChecked(true);
      break;
    }
  switch (configuration->get_display_principal_component_axes())
    {
    case DISPLAY_ALL_PRINCIPAL_COMPONENT_AXES:
      ui->displayAllPrincipalComponentAxesButton->setChecked(true);
      break;
    case DISPLAY_NO_PRINCIPAL_COMPONENT_AXES:
      ui->displayNoPrincipalComponentAxesButton->setChecked(true);
      break;
    }
  switch (configuration->get_draw_pentagon_cellophanes())
    {
    case DRAW_PENTAGON_STRONG_CELLOPHANES:
      ui->drawPentagonStrongCellophanesButton->setChecked(true);
      break;
    case DRAW_PENTAGON_TINT_CELLOPHANES:
      ui->drawPentagonTintCellophanesButton->setChecked(true);
      break;
    case DRAW_PENTAGON_MONO_CHROME_CELLOPHANES:
      ui->drawPentagonMonoChromeCellophanesButton->setChecked(true);
      break;
    case DRAW_PENTAGON_TRANSPARENT:
      ui->drawPentagonTransparentButton->setChecked(true);
      break;
    }
  switch (configuration->get_arrange_open_fullerene())
    {
    case ARRANGE_OPEN_FULLERENE_AS_TSUZUMI:
      ui->arrangeOpenFullereneAsTsuzumiButton->setChecked(true);
      break;
    case ARRANGE_OPEN_FULLERENE_AS_SPHERE:
      ui->arrangeOpenFullereneAsSphereButton->setChecked(true);
      break;
    }
  switch (configuration->get_display_clustering())
    {
    case DISPLAY_CLUSTERING_BY_CELLOPHANES:
      ui->displayClustersByCellophanesButton->setChecked(true);
      break;
    case DISPLAY_NO_CLUSTERING:
      ui->displayNoClustersButton->setChecked(true);
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
  if (ui->displayAllSymmetryAxesButton->isChecked())
    configuration->set_display_symmetry_axes(DISPLAY_ALL_SYMMETRY_AXES);
  else if (ui->displayMajorSymmetryAxesButton->isChecked())
    configuration->set_display_symmetry_axes(DISPLAY_MAJOR_SYMMETRY_AXES);
  else
    configuration->set_display_symmetry_axes(DISPLAY_NO_SYMMETRY_AXES);
  if (ui->displayAllPrincipalComponentAxesButton->isChecked())
    configuration->set_display_principal_component_axes(DISPLAY_ALL_PRINCIPAL_COMPONENT_AXES);
  else
    configuration->set_display_principal_component_axes(DISPLAY_NO_PRINCIPAL_COMPONENT_AXES);
  if (ui->drawPentagonStrongCellophanesButton->isChecked())
    configuration->set_draw_pentagon_cellophanes(DRAW_PENTAGON_STRONG_CELLOPHANES);
  else if (ui->drawPentagonTintCellophanesButton->isChecked())
    configuration->set_draw_pentagon_cellophanes(DRAW_PENTAGON_TINT_CELLOPHANES);
  else if (ui->drawPentagonMonoChromeCellophanesButton->isChecked())
    configuration->set_draw_pentagon_cellophanes(DRAW_PENTAGON_MONO_CHROME_CELLOPHANES);
  else
    configuration->set_draw_pentagon_cellophanes(DRAW_PENTAGON_TRANSPARENT);
  if (ui->arrangeOpenFullereneAsTsuzumiButton->isChecked())
    configuration->set_arrange_open_fullerene(ARRANGE_OPEN_FULLERENE_AS_TSUZUMI);
  else
    configuration->set_arrange_open_fullerene(ARRANGE_OPEN_FULLERENE_AS_SPHERE);
  if (ui->displayClustersByCellophanesButton->isChecked())
    configuration->set_display_clustering(DISPLAY_CLUSTERING_BY_CELLOPHANES);
  else
    configuration->set_display_clustering(DISPLAY_NO_CLUSTERING);
  configuration->set_working_folder_name(qPrintable(ui->workingFolderLineEdit->text()));
  configuration->set_povray_command_line(qPrintable(ui->povrayCommandLineLineEdit->text()));

  configuration->save();
  configuration->reflect();
  OpenGLUtil::rebuild_fullerene();
  accept();
}

void ConfigurationDialog::slot_reject()
{
  reject();
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
