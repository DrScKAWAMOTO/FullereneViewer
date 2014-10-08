/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include "TreeItem.h"

TreeItem root_mtis[] = {
  TreeItem( 0, root_mtis, 6, root_mtis + 1, "" ),
  TreeItem( 0, root_mtis, 1, ring_mtis, "リング中心対称フラーレン(C200まで)" ),
  TreeItem( 1, root_mtis, 1, carbon_mtis, "カーボン中心対称フラーレン(C200まで)" ),
  TreeItem( 2, root_mtis, 1, bond_mtis, "ボンド中心対称フラーレン(C200まで)" ),
  TreeItem( 3, root_mtis, 1, tube_mtis, "カーボン・ナノ・チューブ(筒長4)" ),
  TreeItem( 4, root_mtis, 1, merged_mtis, "全フラーレン(C100まで)" ),
  TreeItem( 5, root_mtis, 1, root_mtis + 7, "特殊フラーレン" ),
  TreeItem( 0, root_mtis + 6, 1, root_mtis + 8, "C474 (Y分岐フラーレン)" ),
  TreeItem( 0, root_mtis + 7, 0, root_mtis, "(NoA=6M) Y" )
};

TreeItem* TreeItem::top_item()
{
  root_mtis[1].p_number_of_children = number_of_ring_mtis_top;
  root_mtis[2].p_number_of_children = number_of_carbon_mtis_top;
  root_mtis[3].p_number_of_children = number_of_bond_mtis_top;
  root_mtis[4].p_number_of_children = number_of_tube_mtis_top;
  root_mtis[5].p_number_of_children = number_of_merged_mtis_top;
  return root_mtis;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
