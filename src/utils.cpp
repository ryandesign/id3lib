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

#include "utils.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{

  // Extract a 32-bit number from a 4-byte character array
  uint32 ParseNumber(const uchar *buffer, size_t size)
  {
    size_t nSize = 0;
    for (size_t nIndex = 0; nIndex < size; nIndex++)
    {
      nSize |= buffer[nIndex] << ((size - nIndex - 1) * 8);
    }
    return nSize;
  }

  void RenderNumber(uchar *buffer, size_t val, size_t size)
  {
    for (size_t nIndex = 0; nIndex < size; nIndex++)
    {
      buffer[nIndex] = (uchar)((val >> ((size - nIndex - 1) * 8)) & MASK8);
    }
  }

  // converts an ASCII string into a Unicode one

  void mbstoucs(unicode_t *unicode, const char *ascii, const luint len)
  {
    if (NULL != ascii && NULL != unicode)
    {
      for (luint i = 0; i < len; i++)
      {
        unicode[i] = ascii[i] & 0xFF;
      }
    }
  }

  // converts a Unicode string into ASCII

  void ucstombs(char *ascii, const unicode_t *unicode, const luint len)
  {
    if (NULL != unicode && NULL != ascii)
    {
      for (luint i = 0; i < len; i++)
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
      size_t nIndex;
      for (nIndex = 0; NULL_UNICODE != src[nIndex]; nIndex++)
      {
        dest[nIndex] = src[nIndex];
      }
      dest[nIndex] = NULL_UNICODE;
    }
  }

  void ucsncpy(unicode_t *dest, const unicode_t *src, size_t len)
  {
    if (NULL != dest && NULL != src)
    {
      size_t nIndex;
      for (nIndex = 0; nIndex < len && NULL_UNICODE != src[nIndex]; nIndex++)
      {
        dest[nIndex] = src[nIndex];
      }
      for (; nIndex < len; nIndex++)
      {
        dest[nIndex] = NULL_UNICODE;
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
    for (size_t nIndex = 0; true; nIndex++)
    {
      if ((NULL_UNICODE == s1[nIndex]) ||
          (NULL_UNICODE == s2[nIndex]) ||
          (s1[nIndex]   != s2[nIndex]) ||
          (nIndex + 1   == len))
      {
        return s2[nIndex] - s1[nIndex];
      }
    }
  }

}