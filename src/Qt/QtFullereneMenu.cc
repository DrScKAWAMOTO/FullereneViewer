/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/24 15:35:29 JST
 */

#include "QtFullereneMenu.h"
#include "FullereneMenu.h"
#include "DebugMemory.h"

#define MAKE_SUBMENU(mw,mn,No) for (int i = 0; i < (int)(sizeof(entry##No) / sizeof(MenuEntry)); ++i) { \
  QAction* action = new QAction(entry##No[i].fullerene_name, mw); \
  action->setStatusTip(entry##No[i].generator_label); \
  QObject::connect(action, SIGNAL(triggered()), mw, SLOT(fullereneSelected())); \
  mn->addAction(action); \
  }

void make_qt_fullerene_menu(QMainWindow* main_window, QMenu* fullerene_menu)
{
  QMenu* sub_menu;

  sub_menu = fullerene_menu->addMenu("C60-C82");
  MAKE_SUBMENU(main_window, sub_menu, 0);

  sub_menu = fullerene_menu->addMenu("C84");
  MAKE_SUBMENU(main_window, sub_menu, 1);

  sub_menu = fullerene_menu->addMenu("C86");
  MAKE_SUBMENU(main_window, sub_menu, 2);

  sub_menu = fullerene_menu->addMenu("C88");
  MAKE_SUBMENU(main_window, sub_menu, 3);

  sub_menu = fullerene_menu->addMenu("C90");
  MAKE_SUBMENU(main_window, sub_menu, 4);

  sub_menu = fullerene_menu->addMenu("C92");
  MAKE_SUBMENU(main_window, sub_menu, 5);

  sub_menu = fullerene_menu->addMenu("C94");
  MAKE_SUBMENU(main_window, sub_menu, 6);

  sub_menu = fullerene_menu->addMenu("C96");
  MAKE_SUBMENU(main_window, sub_menu, 7);

  sub_menu = fullerene_menu->addMenu("C98");
  MAKE_SUBMENU(main_window, sub_menu, 8);

  sub_menu = fullerene_menu->addMenu("C100");
  MAKE_SUBMENU(main_window, sub_menu, 9);

  sub_menu = fullerene_menu->addMenu("C104-C120");
  MAKE_SUBMENU(main_window, sub_menu, 10);

  sub_menu = fullerene_menu->addMenu("C122-C140");
  MAKE_SUBMENU(main_window, sub_menu, 11);

  sub_menu = fullerene_menu->addMenu("C142-C160");
  MAKE_SUBMENU(main_window, sub_menu, 12);

  sub_menu = fullerene_menu->addMenu("C164-C180");
  MAKE_SUBMENU(main_window, sub_menu, 13);

  sub_menu = fullerene_menu->addMenu("C182-C200");
  MAKE_SUBMENU(main_window, sub_menu, 14);

  sub_menu = fullerene_menu->addMenu("extras");
  MAKE_SUBMENU(main_window, sub_menu, 15);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
