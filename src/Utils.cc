/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/17 12:55:55 JST
 */

#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "Utils.h"

char No_to_character(int No)
{
  if (No < 0)
    return 'Y';
  if (No < 10)
    return '0' + No;
  if (No < 10 + 26)
    return 'a' - 10 + No;
  return 'X';
}

int character_to_No(char character)
{
  if ((character >= '0') && (character <= '9'))
    return character - '0';
  if ((character >= 'a') && (character <= 'z'))
    return character - 'a' + 10;
  return -1;
}

void No_to_digits26x7(int No, char*& buffer)
{
  char work[8];
  int index;
  if (No < 0)
    {
      strcpy(buffer, "X");
      return;
    }
  for (index = 0; index < 7; ++index)
    {
      if (No == 0)
        break;
      work[index] = (No % 26) + 'a';
      No /= 26;
    }
  while (--index >= 0)
    *buffer++ = work[index];
  *buffer = '\0';
}

void No_to_digits10x10(int No, char*& buffer)
{
  char work[11];
  int index;
  if (No < 0)
    {
      strcpy(buffer, "X");
      return;
    }
  for (index = 0; index < 10; ++index)
    {
      if (No == 0)
        break;
      work[index] = (No % 10) + '0';
      No /= 10;
    }
  while (--index >= 0)
    *buffer++ = work[index];
  *buffer = '\0';
}

int digits26x7_to_No(const char*& buffer)
{
  if (*buffer == '*')
    {
      ++buffer;
      return -1;
    }
  int result = 0;
  while ((*buffer >= 'a') && (*buffer <= 'z'))
    {
      result *= 26;
      result += *buffer++ - 'a';
    }
  return result;
}

int digits10x10_to_No(const char*& buffer)
{
  if (*buffer == '*')
    {
      ++buffer;
      return -1;
    }
  int result = 0;
  while ((*buffer >= '0') && (*buffer <= '9'))
    {
      result *= 10;
      result += *buffer++ - '0';
    }
  return result;
}

void No_No_to_digits10x10_digits26x7(int No1, int No2, char*& buffer)
{
  No_to_digits10x10(No1, buffer);
  No_to_digits26x7(No2, buffer);
}

int compare_generator_formula(const char* base_formula, const char* target_formula)
{
  while (*base_formula && *target_formula)
    {
      if (*base_formula != *target_formula)
        return *base_formula - *target_formula;
      if (*base_formula == '-')
        break;
      ++base_formula;
      ++target_formula;
    }
  if (*base_formula == '\0')
    return 0;
  if (*target_formula == '\0')
    return 1;
  return compare_sub_generator_formula(base_formula + 1, target_formula + 1);
}

int compare_sub_generator_formula(const char* base_formula, const char* target_formula)
{
  while (1)
    {
      int base_No1;
      int base_No2;
      int target_No1;
      int target_No2;
      base_No1 = digits10x10_to_No(base_formula);
      base_No2 = digits26x7_to_No(base_formula);
      target_No1 = digits10x10_to_No(target_formula);
      target_No2 = digits26x7_to_No(target_formula);
      if ((base_No1 == -1) || (target_No1 == -1))
        return 0;
      if ((base_No1 == 0) && (target_No1 == 0))
        return 0;
      if (base_No1 != target_No1)
        return base_No1 - target_No1;
      if ((base_No2 == -1) || (target_No2 == -1))
        return 0;
      if (base_No2 == 0)
        return 0;
      if (target_No2 == 0)
        return -1;
      if (base_No2 < target_No2)
        {
          base_No1 = digits10x10_to_No(base_formula);
          if (base_No1 <= 0)
            return 0;
          return base_No1 - target_No1;
        }
      else if (base_No2 > target_No2)
        {
          target_No1 = digits10x10_to_No(target_formula);
          if (target_No1 == -1)
            return 0;
          if (target_No1 == 0)
            return 1;
          return base_No1 - target_No1;
        }
    }
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
