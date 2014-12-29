/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __TREEITEM_H__
#define __TREEITEM_H__

#include <QVariant>

class TreeItem {
  // friend classes & functions

  // members
private:
  int p_row;
  const TreeItem* p_parent;
  int p_number_of_children;
  const TreeItem* p_children;
  const char* p_data;

  // private tools

  // constructors & the destructor
public:
  TreeItem(int row, const TreeItem* parent,
           int number_of_children, const TreeItem* children, const char* data)
    : p_row(row), p_parent(parent),
      p_number_of_children(number_of_children), p_children(children), p_data(data) { }
  ~TreeItem() { }

  // type converters

  // comparators

  // math operators

  // I/O

  // class decision
  static TreeItem* top_item();

  // member accessing methods
  const TreeItem* child(int row) const { return p_children + row; }
  int number_of_children() const { return p_number_of_children; }
  QVariant data() const { return p_data; }
  const char* data_as_string() const { return p_data; }
  int row() const { return p_row; }
  const TreeItem* parent() const { return p_parent; }

};

extern TreeItem root_mtis[];
extern const TreeItem ring_mtis[];
extern const int number_of_ring_mtis_top;
extern const TreeItem carbon_mtis[];
extern const int number_of_carbon_mtis_top;
extern const TreeItem bond_mtis[];
extern const int number_of_bond_mtis_top;
extern const TreeItem tube_mtis[];
extern const int number_of_tube_mtis_top;
extern const TreeItem merged_mtis[];
extern const int number_of_merged_mtis_top;

#endif /* __TREEITEM_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
