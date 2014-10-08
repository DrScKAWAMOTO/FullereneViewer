/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <QTreeView>
#include <QItemSelectionModel>
#include <QVariant>
#include "SelectFullereneDialog.h"
#include "ui_SelectFullereneDialog.h"
#include "TreeModel.h"
#include "TreeItem.h"
#include "OpenGLUtil.h"

SelectFullereneDialog::SelectFullereneDialog(QWidget *parent)
  : QDialog(parent), ui(new Ui::SelectFullereneDialog)
{
  ui->setupUi(this);
  ui->treeView->setModel(&model);
  connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slot_accept()));
  connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(slot_reject()));
}

SelectFullereneDialog::~SelectFullereneDialog()
{
  delete ui;
}

void SelectFullereneDialog::slot_accept()
{
  QModelIndex index = ui->treeView->selectionModel()->currentIndex();
  if (!index.isValid())
    return;
  const TreeItem *item = static_cast<const TreeItem*>(index.internalPointer());
  QString formula = item->data_as_string();
  formula.remove("(鏡像)");
  formula.remove(QRegExp("\\(NoA=\\d+M?\\)"));
  formula.remove(QRegExp("[ \n\r\t]"));
  QString noa = item->data_as_string();
  if (!noa.contains("(NoA="))
    {
      item = item->parent();
      noa = item->data_as_string();
    }
  noa.remove(QRegExp(" .+$"));
  const TreeItem *cnitem = item->parent();
  QString cn = cnitem->data_as_string();
  cn.remove(QRegExp(" .+$"));
  QString name = cn + " " + noa;
  if (OpenGLUtil::change_fullerene(qPrintable(name), qPrintable(formula)))
    setWindowTitle(OpenGLUtil::window_title);
  accept();
}

void SelectFullereneDialog::slot_reject()
{
  reject();
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
