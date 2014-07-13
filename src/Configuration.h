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
#define WORKING_DIRECTORY_NAME "/fullerene"

enum Quality {
  QUALITY_HIGH,
  QUALITY_MIDDLE,
  QUALITY_LOW
};

class Configuration {
  // friend classes & functions

  // members
private:
  char p_configuration_file_name[PATH_LENGTH + 1];
  char p_working_directory_name[PATH_LENGTH + 1];
  Quality p_picture_quality;
  Quality p_motion_quality;

  // private tools

  // constructors & the destructor
public:
  Configuration(const char* home_directory);
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
  const char* get_working_directory_name() const { return p_working_directory_name; }
  void set_working_directory_name(const char* path);

};

extern Configuration *configuration;

#endif /* __CONFIGURATION_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
