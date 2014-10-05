/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2014/07/17 08:08:13 JST
 */

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#define PATH_LENGTH 1024
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
  DISPLAY_ALL_AXES,
  DISPLAY_MAJOR_AXES,
  DISPLAY_NO_AXES
};

class Configuration {
  // friend classes & functions

  // members
private:
  char p_configuration_file_name[PATH_LENGTH + 1];
  char p_working_folder_name[PATH_LENGTH + 1];
  char p_povray_command_line[PATH_LENGTH + 1];
  Quality p_picture_quality;
  Quality p_motion_quality;
  DisplaySymmetryAxes p_display_symmetry_axes;

  // private tools

  // constructors & the destructor
public:
  Configuration(const char* home, const char* desktop);
  ~Configuration();

  // type converters

  // comparators

  // math operators

  // I/O
  void load();
  void save() const;
  void reflect() const;

  // class decision

  // member accessing methods
  Quality get_picture_quality() const { return p_picture_quality; }
  void set_picture_quality(Quality quality) { p_picture_quality = quality; }
  Quality get_motion_quality() const { return p_motion_quality; }
  void set_motion_quality(Quality quality) { p_motion_quality = quality; }
  DisplaySymmetryAxes get_display_symmetry_axes() const
  { return p_display_symmetry_axes; }
  void set_display_symmetry_axes(DisplaySymmetryAxes display_symmetry_axes)
  { p_display_symmetry_axes = display_symmetry_axes; }
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
