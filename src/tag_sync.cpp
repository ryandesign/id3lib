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

// To be used when reading an ID3v2-tag
// Transforms all FF 00 sequences into FF

luint ID3_ReSync(uchar *data, luint size)
{
  uchar *src, *dest;
  for (src = dest = data; src < data + size - 1; src++, dest++)
  {
    *dest = *src;
    if (0xFF == src[0] && '\0' == src[1])
    {
      src++;
    }
  }
  
  return size - (src - dest);
}

// Determine if pCur is at a point in the pStart buffer where unsyncing is 
// necessary
bool ID3_ShouldUnsync(const uchar *cur, const uchar *start, const size_t size)
{
  // The current byte is a sync if it's equal to 0xFF and 
  // 1) It's the last byte in the file, or
  // 2) It comes before 111xxxxx (second part of an mp3 sync), or
  // 3) It comes before 00000000 (consequence of unsyncing)
  return
    ( cur           >= start )          &&
    ( cur            < start + size)    &&
    ( cur[0]        == 0xFF)            && // first sync
    ((cur    + 1    == (start + size))  || // last byte?
     (cur[1] & 0xE0 == 0xE0)            || // second sync
     (cur[1]        == 0x00));             // second null
}

// How big will the tag be after we unsync?
luint ID3_GetUnSyncSize(uchar *pBuffer, luint size)
{
  size_t new_size = size;
  
  // Determine the size needed for the destination data
  for (uchar *cur = pBuffer; cur < pBuffer + size; cur++)
  {
    if (ID3_ShouldUnsync(cur, pBuffer, size))
    {
      new_size++;
    }
  }
  
  return new_size;
}


// To be used when writing an ID3v2-tag
// Transforms:
// 11111111 111xxxxx -> 11111111 00000000 111xxxxx
// 11111111 00000000 -> 11111111 00000000 00000000
// 11111111 <EOF> -> 11111111 00000000 <EOF>

void ID3_UnSync(uchar *dest_data, luint dest_size, 
                const uchar *src_data, luint src_size)
{
  // Now do the real transformation
  for (uchar *src = src_data, *dest = dest_data; 
       (src < src_data + src_size) && (dest < dest_data + dest_size);
       src++, dest++)
  {
    // Copy the current character from source to destination
    *dest = *src;

    // If we're at a sync point in the source, insert an extra null character
    // in the destination buffer
    if (ID3_ShouldUnsync(src, src_data, src_size))
    {
      dest++;
      *dest = '\0';
    }
  }
}
