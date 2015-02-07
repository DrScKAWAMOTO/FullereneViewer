/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "MyString.h"

#define CONFIGURATION_FILE_NAME "/.FullereneViewer.conf"
#define WORKING_FOLDER_NAME "/fullerene"
#ifdef _WIN32
# define POVRAY_COMMAND_LINE "\"C:\\Program Files\\POV-Ray\\v3.7\\bin\\pvengine32-sse2.exe\" /RENDER +FN +UA %s /EXIT"
#elif defined(__APPLE__)
# define POVRAY_COMMAND_LINE "/usr/local/bin/povray -W1600 -H1200 +FN +UA %s"
#elif defined(__unix)
# define POVRAY_COMMAND_LINE "povray37 -W1600 -H1200 +FN +UA %s"
#else
# define POVRAY_COMMAND_LINE "bad command line"
#endif

enum Quality {
  QUALITY_HIGH,
  QUALITY_MIDDLE,
  QUALITY_LOW
};

enum DisplaySymmetryAxes {
  DISPLAY_ALL_SYMMETRY_AXES,
  DISPLAY_MAJOR_SYMMETRY_AXES,
  DISPLAY_NO_SYMMETRY_AXES
};

enum DisplayPrincipalComponentAxes {
  DISPLAY_ALL_PRINCIPAL_COMPONENT_AXES,
  DISPLAY_NO_PRINCIPAL_COMPONENT_AXES
};

enum DrawPentagonCellophanes {
  DRAW_PENTAGON_STRONG_CELLOPHANES,
  DRAW_PENTAGON_TINT_CELLOPHANES,
  DRAW_PENTAGON_MONO_CHROME_CELLOPHANES,
  DRAW_PENTAGON_TRANSPARENT
};

enum ArrangeOpenFullerene {
  ARRANGE_OPEN_FULLERENE_AS_TSUZUMI,
  ARRANGE_OPEN_FULLERENE_AS_SPHERE
};

enum DisplayClustering {
  DISPLAY_CLUSTERING_BY_CELLOPHANES,
  DISPLAY_NO_CLUSTERING
};

class Configuration {
  // friend classes & functions

  // members
private:
  MyString p_configuration_file_name;
  MyString p_working_folder_name;
  MyString p_povray_command_line;
  Quality p_picture_quality;
  Quality p_motion_quality;
  DisplaySymmetryAxes p_display_symmetry_axes;
  DisplayPrincipalComponentAxes p_display_principal_component_axes;
  DrawPentagonCellophanes p_draw_pentagon_cellophanes;
  ArrangeOpenFullerene p_arrange_open_fullerene;
  DisplayClustering p_display_clustering;

  // private tools

  // constructors & the destructor
public:
  Configuration(const char* home, const char* desktop);
  ~Configuration();

  // comparators

  // I/O
  void load();
  void save() const;
  void reflect() const;

  // member accessing methods
  Quality get_picture_quality() const { return p_picture_quality; }
  void set_picture_quality(Quality quality) { p_picture_quality = quality; }
  Quality get_motion_quality() const { return p_motion_quality; }
  void set_motion_quality(Quality quality) { p_motion_quality = quality; }
  DisplaySymmetryAxes get_display_symmetry_axes() const
  { return p_display_symmetry_axes; }
  void set_display_symmetry_axes(DisplaySymmetryAxes display_symmetry_axes)
  { p_display_symmetry_axes = display_symmetry_axes; }
  DisplayPrincipalComponentAxes get_display_principal_component_axes() const
  { return p_display_principal_component_axes; }
  void set_display_principal_component_axes(DisplayPrincipalComponentAxes
                                            display_principal_component_axes)
  { p_display_principal_component_axes = display_principal_component_axes; }
  DrawPentagonCellophanes get_draw_pentagon_cellophanes() const
  { return p_draw_pentagon_cellophanes; }
  void set_draw_pentagon_cellophanes(DrawPentagonCellophanes draw_pentagon_cellophanes)
  { p_draw_pentagon_cellophanes = draw_pentagon_cellophanes; }
  ArrangeOpenFullerene get_arrange_open_fullerene() const
  { return p_arrange_open_fullerene; }
  void set_arrange_open_fullerene(ArrangeOpenFullerene arrange_open_fullerene)
  { p_arrange_open_fullerene = arrange_open_fullerene; }
  DisplayClustering get_display_clustering() const
  { return p_display_clustering; }
  void set_display_clustering(DisplayClustering display_clustering)
  { p_display_clustering = display_clustering; }
  const char* get_working_folder_name() const { return p_working_folder_name; }
  void set_working_folder_name(const char* path);
  const char* get_povray_command_line() const { return p_povray_command_line; }
  void set_povray_command_line(const char* command_line);

};

extern Configuration *configuration;

#endif /* __CONFIGURATION_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
