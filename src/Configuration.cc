/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2014/07/17 08:18:14 JST
 */

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Configuration.h"
#include "OpenGLUtil.h"

#define LINE_LENGTH 1024

Configuration *configuration = 0;

struct Word {
  const char* word_top;
  int length;
  Word(const char* buffer);
  ~Word();
  bool next();
  bool equals_to(const char *ptr);
};

Word::Word(const char* buffer)
{
  word_top = buffer;
  length = 0;
  next();
}

Word::~Word()
{
}

bool Word::next()
{
  word_top += length;
  while (*word_top == ' ')
    ++word_top;
  const char* ptr = word_top;
  while (*ptr && (*ptr != ' ') && (*ptr != '\n'))
    ++ptr;
  length = ptr - word_top;
  if (*ptr == ' ')
    return true;
  else
    return false;
}

bool Word::equals_to(const char* ptr)
{
  if (strncmp(word_top, ptr, length) == 0)
    return true;
  else
    return false;
}

Configuration::Configuration(const char* home_directory, const char* desktop_directory)
{
  int length = strlen(home_directory);
  assert(length > 0);
  assert(length + strlen(CONFIGURATION_FILE_NAME) < PATH_LENGTH);
  strcpy(p_configuration_file_name, home_directory);
  if (p_configuration_file_name[length - 1] == '/')
    p_configuration_file_name[length - 1] = '\0';
  strcat(p_configuration_file_name, CONFIGURATION_FILE_NAME);

  length = strlen(desktop_directory);
  assert(length > 0);
  assert(length + strlen(WORKING_DIRECTORY_NAME) < PATH_LENGTH);
  strcpy(p_working_directory_name, desktop_directory);
  if (p_working_directory_name[length - 1] == '/')
    p_working_directory_name[length - 1] = '\0';
  strcat(p_working_directory_name, WORKING_DIRECTORY_NAME);

  p_picture_quality = QUALITY_HIGH;
  p_motion_quality = QUALITY_HIGH;
}

Configuration::~Configuration()
{
}

static bool read_path(Word& word, char* path)
{
  if (word.next() == false)
    return false;
  if (word.equals_to("=") == false)
    return false;
  if (word.next())
    return false;
  strncpy(path, word.word_top, word.length);
  path[word.length] = '\0';
  return true;
}

static bool name_to_quality(Word& word, Quality& result)
{
  if (word.next() == false)
    return false;
  if (word.equals_to("=") == false)
    return false;
  if (word.next())
    return false;
  if (word.equals_to("high"))
    {
      result = QUALITY_HIGH;
      return true;
    }
  else if (word.equals_to("middle"))
    {
      result = QUALITY_MIDDLE;
      return true;
    }
  else if (word.equals_to("low"))
    {
      result = QUALITY_LOW;
      return true;
    }
  return false;
}

void Configuration::load()
{
  FILE* fptr = fopen(p_configuration_file_name, "r");
  if (fptr == NULL)
    {
      save();
      return;
    }
  char buffer[LINE_LENGTH + 1];
  while (fgets(buffer, LINE_LENGTH, fptr))
    {
      Word word(buffer);
      if (word.equals_to("working_directory_name"))
        {
          read_path(word, p_working_directory_name);
        }
      else if (word.equals_to("picture_quality"))
        {
          Quality quality;
          if (name_to_quality(word, quality) == false)
            continue;
          p_picture_quality = quality;
        }
      else if (word.equals_to("motion_quality"))
        {
          Quality quality;
          if (name_to_quality(word, quality) == false)
            continue;
          p_motion_quality = quality;
        }
    }
  fclose(fptr);
}

static const char* quality_to_name(Quality quality)
{
  switch (quality)
    {
    case QUALITY_HIGH:
    default:
      return "high";
    case QUALITY_MIDDLE:
      return "middle";
    case QUALITY_LOW:
      return "low";
    }
}

void Configuration::save() const
{
  FILE* fptr = fopen(p_configuration_file_name, "w");
  fprintf(fptr, "working_directory_name = %s\n", p_working_directory_name);
  fprintf(fptr, "picture_quality = %s\n", quality_to_name(p_picture_quality));
  fprintf(fptr, "motion_quality = %s\n", quality_to_name(p_motion_quality));
  fclose(fptr);
}

static void make_directory_recursive(const char *path)
{
  struct stat sb;
  if (stat(path, &sb) == 0)
    {
      if (S_ISDIR(sb.st_mode) && access(path, X_OK))
        return;
    }
  int result;
#if defined(__unix)
  result = mkdir(path, 0755);
#else
  result = mkdir(path);
#endif
  if (result == 0)
    return;
  char parent[PATH_LENGTH + 1];
  assert(strlen(path) <= PATH_LENGTH);
  strcpy(parent, path);
  char* delimiter;
  delimiter = strrchr(parent, '/');
  if (delimiter == NULL)
    delimiter = strrchr(parent, '\\');
  if (delimiter == NULL)
    return;
  *delimiter = '\0';
  make_directory_recursive(parent);
#if defined(__unix)
  mkdir(path, 0755);
#else
  mkdir(path);
#endif
}

void Configuration::reflect() const
{
  switch (p_motion_quality)
    {
    case QUALITY_HIGH:
      OpenGLUtil::config_viewer_target_fps = 60;
      break;
    case QUALITY_MIDDLE:
      OpenGLUtil::config_viewer_target_fps = 40;
      break;
    case QUALITY_LOW:
      OpenGLUtil::config_viewer_target_fps = 20;
      break;
    }
  if (OpenGLUtil::interval_timer_setup_callback)
    (*OpenGLUtil::interval_timer_setup_callback)();

  int length = strlen(configuration->p_working_directory_name);
  assert(length <= PATH_LENGTH);
  char path[PATH_LENGTH + 1];
  strcpy(path, configuration->p_working_directory_name);
  if ((length >= 1) && ((path[length - 1] == '/') || (path[length - 1] == '\\')))
    path[length - 1] = '\0';
  make_directory_recursive(path);
  chdir(path);
}

void Configuration::set_working_directory_name(const char* path)
{
  assert(strlen(path) < PATH_LENGTH);
  strcpy(p_working_directory_name, path);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
