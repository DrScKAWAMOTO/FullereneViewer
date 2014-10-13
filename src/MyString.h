/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __MYSTRING_H__
#define __MYSTRING_H__

class MyString {
  /* 実装は必ず領域を作る。*/
  /* 途中にヌル文字があってもよい。*/
  /* 常にヌルターミネートしているので、途中にヌル文字がなければ文字列として扱える。*/
  /* p_glow_to() と p_glow_and_copy() がヌルターミネートしていることを保証する。*/

  // friend classes & functions

  // members
private:
  int p_buffer_size;
  int p_length;
  char* p_buffer;

  // private tools
private:
  void p_init_to(int length);
  void p_glow_to(int length);
  void p_round_up_buffer_size();

  // constructors & the destructor
public:
  MyString();
  MyString(const char* value);
  MyString(const MyString& you);
  MyString& operator = (const char* value);
  MyString& operator = (const MyString& you);
  ~MyString();

  // comparators
public:
  int compare(const char* you) const;
  int compare(const MyString* you) const;

  // I/O

  // member accessing methods
public:
  void set_value(const char* value);
  void set_value(const char* value, int length);
  int length() const { return p_length; }
  operator char*() const { return p_buffer; }
  char& operator [](int index);
  void set_char_at(int index, char value);
  void resize(int new_length);
  void append_char(char appendant);
  void append_string(const char* appendant);
  void append_int(int appendant);

};

#endif /* __MYSTRING_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
