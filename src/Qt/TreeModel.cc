/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <QStringList>

#include "TreeItem.h"
#include "TreeModel.h"
#include "ShutUp.h"

TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent)
{
  rootItem = TreeItem::top_item();
}

TreeModel::~TreeModel()
{
}

int TreeModel::columnCount(const QModelIndex &UNUSED(parent)) const
{
  return 1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
  if ((!index.isValid()) || (role != Qt::DisplayRole))
    return QVariant();
  const TreeItem *item = static_cast<const TreeItem*>(index.internalPointer());
  return item->data();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;
  return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 1)
    return QVariant("フラーレン");
  return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();
  const TreeItem *parentItem;
  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<const TreeItem*>(parent.internalPointer());
  const TreeItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, (void*)childItem);
  else
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();
  const TreeItem *childItem = static_cast<const TreeItem*>(index.internalPointer());
  const TreeItem *parentItem = childItem->parent();
  if (parentItem == rootItem)
    return QModelIndex();
  return createIndex(parentItem->row(), 0, (void*)parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
  const TreeItem *parentItem;
  if (parent.column() > 0)
    return 0;
  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<const TreeItem*>(parent.internalPointer());
  return parentItem->number_of_children();
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
