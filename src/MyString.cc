/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "MyString.h"
#include "Debug.h"
#include "DebugMemory.h"

void MyString::p_init_to(int length)
{
  if (length + 1 > p_buffer_size)
    {
      p_buffer_size = length + 1;
      p_round_up_buffer_size();
      if (p_buffer)
        delete[] p_buffer;
      p_buffer = new char[p_buffer_size];
    }
  p_length = length;
  p_buffer[length] = '\0';
}

void MyString::p_glow_to(int length)
{
  if (length + 1 > p_buffer_size)
    {
      p_buffer_size = length + 1;
      p_round_up_buffer_size();
      char* new_buffer = new char[p_buffer_size];
      if (p_buffer)
        {
          memmove(new_buffer, p_buffer, p_length + 1);
          delete[] p_buffer;
        }
      p_buffer = new_buffer;
    }
  p_length = length;
  p_buffer[length] = '\0';
}

void MyString::p_round_up_buffer_size()
{
  const int mask = 32 - 1;
  p_buffer_size += mask;
  p_buffer_size &= ~mask;
}

MyString::MyString() : p_buffer_size(0), p_length(0), p_buffer(0)
{
  p_init_to(0);
  strcpy(p_buffer, "");
}

MyString::MyString(const char* value) : p_buffer_size(0), p_length(0), p_buffer(0)
{
  p_init_to(strlen(value));
  strcpy(p_buffer, value);
}

MyString::MyString(const MyString& you) : p_buffer_size(0), p_length(0), p_buffer(0)
{
  int len = you.length();
  p_init_to(len);
  memmove(p_buffer, (char*)you, len + 1);
}

MyString& MyString::operator = (const char* value)
{
  if (value != p_buffer)
    {
      p_init_to(strlen(value));
      strcpy(p_buffer, value);
    }
  return *this;
}

MyString& MyString::operator = (const MyString& you)
{
  if (&you != this)
    {
      int len = you.length();
      p_init_to(len);
      memmove(p_buffer, (char*)you, len + 1);
    }
  return *this;
}

MyString::~MyString()
{
  delete[] p_buffer;
}

int MyString::compare(const MyString* you) const
{
  return strcmp(p_buffer, you->p_buffer);
}

int MyString::compare(const char* you) const
{
  return strcmp(p_buffer, you);
}

void MyString::set_value(const char* value)
{
  p_init_to(strlen(value));
  strcpy(p_buffer, value);
}

void MyString::set_value(const char* value, int length)
{
  p_init_to(length);
  memmove(p_buffer, value, length);
}

char& MyString::operator [](int index)
{
  assert(index >= 0);
  assert(index < p_length);
  return p_buffer[index];
}

void MyString::resize(int new_length)
{
  assert(new_length >= 0);
  p_glow_to(new_length);
}

void MyString::set_char_at(int index, char value)
{
  assert(index >= 0);
  if (index >= p_length)
    p_glow_to(index + 1);
  p_buffer[index] = value;
}

void MyString::append_char(char appendant)
{
  p_glow_to(p_length + 1);
  p_buffer[p_length - 1] = appendant;
}

void MyString::append_string(const char* appendant)
{
  int orig_len = p_length;
  int len = orig_len + strlen(appendant);
  p_glow_to(len);
  strcpy(p_buffer + orig_len, appendant);
}

void MyString::append_int(int appendant)
{
  char work[20];
  snprintf(work, 20, "%d", appendant);
  append_string(work);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
