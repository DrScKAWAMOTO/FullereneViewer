/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/17 12:55:55 JST
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#define PAI 3.14159265358979323846

char No_to_character(int No);
int character_to_No(char character);
void No_to_digits26x7(int No, char*& buffer);
void No_to_digits10x10(int No, char*& buffer);
int digits26x7_to_No(const char*& buffer);
int digits10x10_to_No(const char*& buffer);
void No_No_to_digits10x10_digits26x7(int No1, int No2, char*& buffer);

#endif /* __UTILS_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
