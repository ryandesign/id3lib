// $Id$

// id3lib: a C++ library for creating and manipulating id3v1/v2 tags
// Copyright 1999, 2000  Scott Thomas Haug

// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 2 of the License, or (at your
// option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
// License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

// The id3lib authors encourage improvements and optimisations to be sent to
// the id3lib coordinator.  Please see the README file for details on where to
// send such submissions.  See the AUTHORS file for a list of people who have
// contributed to id3lib.  See the ChangeLog file for a list of changes to
// id3lib.  These files are distributed with id3lib at
// http://download.sourceforge.net/id3lib/

#include <ctype.h>
#include "utils.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

#if defined ID3_UNDEFINED
namespace id3
{
#endif /* ID3_UNDEFINED */

  size_t ID3_TimeToSeconds(const char* data, size_t size)
  {
    size_t seconds = 0;
    size_t cur = 0;
    for (const char* p = data; p < data + size; ++p)
    {
      if (':' == *p)
      {
        seconds += 60 * cur;
        cur = 0;
      }
      else if (!isdigit(*p))
      {
        return 0;
      }
      else
      {
        cur = cur * 10 + (*p - '0');
      }
    }
    return seconds + cur;
  }

  bool ID3_IsCRLF(const char* begin, const char* end)
  {
    if (begin > end || (end - begin) < 2)
    {
      return false;
    }
    const char* cur = begin;
    return 0x0D == *cur++ && 0x0A == *cur;
  }
  
  size_t ID3_CRLFtoLF(char *buffer, size_t size)
  {
    size_t newSize = 0;
    char *dest = buffer;
    char *source = buffer;
    
    if (NULL == buffer || size == 0)
    {
      // TODO: log this
      return 0;
      // ID3_THROW(ID3E_NoData);
    }
    
    while (source < (buffer + size))
    {
      if (ID3_IsCRLF(source, buffer + size))
      {
        source++;
      }
      else
      {
        *dest++ = *source++;
      }
    }
    
    newSize = dest - buffer;
    
    return newSize;
  }

  void RemoveTrailingSpaces(char *buffer, size_t length)
  {
    for (size_t i = length; i > 0 && 0x20 == buffer[i-1]; --i)
    {
      buffer[i-1] = '\0';
    }
  }

  // Extract a 32-bit number from a 4-byte character array
  uint32 ParseNumber(const uchar *buffer, size_t size)
  {
    size_t num = 0;
    for (size_t nIndex = 0; nIndex < size; nIndex++)
    {
      num <<= 8;
      num |= buffer[nIndex];
    }
    return num;
  }

  size_t RenderNumber(uchar *buffer, uint32 val, size_t size)
  {
    uint32 num = val;
    for (size_t i = 0; i < size; i++)
    {
      buffer[size - i - 1] = (uchar)(num & MASK8);
      num >>= 8;
    }
    return size;
  }

  // converts an ASCII string into a Unicode one

  void mbstoucs(unicode_t *unicode, const char *ascii, size_t len)
  {
    if (NULL != ascii && NULL != unicode)
    {
      for (index_t i = 0; i < len; i++)
      {
        unicode[i] = ascii[i] & 0xFF;
      }
    }
  }

  // converts a Unicode string into ASCII

  void ucstombs(char *ascii, const unicode_t *unicode, size_t len)
  {
    if (NULL != unicode && NULL != ascii)
    {
      for (index_t i = 0; i < len; i++)
      {
        ascii[i] = unicode[i] & 0x00FF;
      }
    }
  }

  size_t ucslen(const unicode_t *unicode)
  {
    if (NULL != unicode)
    {
      for (size_t size = 0; true; size++)
      {
        if (NULL_UNICODE == unicode[size])
        {
          return size;
        }
      }
    }
    return 0;
  }

  void ucscpy(unicode_t *dest, const unicode_t *src)
  {
    if (NULL != dest && NULL != src)
    {
      size_t i;
      for (i = 0; NULL_UNICODE != src[i]; i++)
      {
        dest[i] = src[i];
      }
      dest[i] = NULL_UNICODE;
    }
  }

  void ucsncpy(unicode_t *dest, const unicode_t *src, size_t len)
  {
    if (NULL != dest && NULL != src)
    {
      size_t i;
      for (i = 0; i < len && NULL_UNICODE != src[i]; i++)
      {
        dest[i] = src[i];
      }
      for (; i < len; i++)
      {
        dest[i] = NULL_UNICODE;
      }
    }
  }

  int ucscmp(const unicode_t *s1, const unicode_t *s2)
  {
    return ucsncmp(s1, s2, (size_t) -1);
  }

  int ucsncmp(const unicode_t *s1, const unicode_t *s2, size_t len)
  {
    if (NULL == s1 && NULL == s2)
    {
      return  0;
    }
    if (NULL == s1)
    {
      return  1;
    }
    if (NULL == s2)
    {
      return -1;
    }
    for (size_t i = 0; true; i++)
    {
      if ((NULL_UNICODE == s1[i]) || (NULL_UNICODE == s2[i]) ||
          (s1[i] != s2[i]) || (i + 1 == len))
      {
        return s2[i] - s1[i];
      }
    }
  }

  bool exists(const char *name)
  {
    ifstream file(name, ios::nocreate);
    return file.is_open();
  }
  
  ID3_Err ID3_CreateFile(const char* name, fstream& file)
  {
    size_t size = 0;
    if (file.is_open())
    {
      file.close();
    }
    
    file.open(name, ios::out | ios::binary | ios::trunc);
    if (!file)
    {
      return ID3E_ReadOnly;
    }
    
    return ID3E_NoError;
  }
  
  size_t ID3_GetFileSize(fstream& file)
  {
    size_t size = 0;
    if (file.is_open())
    {
      streamoff curpos = file.tellp();
      file.seekp(0, ios::end);
      size = file.tellp();
      file.seekp(curpos);
    }
    return size;
  }
  
  ID3_Err ID3_OpenWritableFile(const char* name, fstream& file)
  {
    if (!exists(name))
    {
      return ID3E_NoFile;
    }
    
    if (file.is_open())
    {
      file.close();
    }
    file.open(name, ios::in | ios::out | ios::binary | ios::nocreate);
    if (!file)
    {
      return ID3E_ReadOnly;
    }
    
    return ID3E_NoError;
  }
  
  ID3_Err ID3_OpenReadableFile(const char* name, fstream& file)
  {
    if (file.is_open())
    {
      file.close();
    }
    file.open(name, ios::in | ios::binary | ios::nocreate);
    if (!file)
    {
      return ID3E_NoFile;
    }
    
    return ID3E_NoError;
  }
  
#if defined ID3_UNDEFINED
}
#endif /* ID3_UNDEFINED */

