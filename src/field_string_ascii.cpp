// $Id$
// 
// This program is free software; you can distribute it and/or modify it under
// the terms discussed in the COPYING file, which should have been included
// with this distribution.
//  
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the COPYING file for more details.
//  
// The id3lib authors encourage improvements and optimisations to be sent to
// the id3lib coordinator.  Please see the README file for details on where
// to send such submissions.

#include <cstring>
#include <cstdlib>
#include "field.h"
#include "misc_support.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

ID3_Field& ID3_Field::operator= (const char *string)
{
  Set(string);
  
  return *this;
}


// the ::Set() function for ASCII

void ID3_Field::Set(const char *sString)
{
  if (sString != NULL)
  {
    Clear();
    size_t nStrLen = (-1 == __lFixedLength) ? strlen(sString) : __lFixedLength;
    unicode_t *temp = new unicode_t[nStrLen + 1];
    if (NULL == temp)
    {
      ID3_THROW(ID3E_NoMemory);
    }

    mbstoucs(temp, sString, nStrLen + 1);

    Set(temp);
    delete [] temp;
      
    __eType = ID3FTY_ASCIISTRING;
  }
  
  return ;
}


// the ::Get() function for ASCII

luint ID3_Field::Get(char *buffer, luint maxLength, luint itemNum)
{
  luint bytesUsed = 0;
  unicode_t *temp;
  char *ascii;
  
  temp = new unicode_t[maxLength];
  if (NULL == temp)
  {
    delete [] temp;
    ID3_THROW(ID3E_NoMemory);
  }

  luint len = Get(temp, maxLength, itemNum);

  ascii = new char[len + 1];
  if (NULL == ascii)
  {
    delete [] ascii;
    ID3_THROW(ID3E_NoMemory);
  }

  luint length;
        
  ucstombs(ascii, temp, len + 1);
        
  length = MIN(strlen(ascii), maxLength);
        
  strncpy(buffer, ascii, length);
  buffer[length] = '\0';
  bytesUsed = length;
        
  delete [] ascii;
    
  delete [] temp;
    
  return bytesUsed;
}


void ID3_Field::Add(const char *sString)
{
  if (sString)
  {
    unicode_t *temp;
    
    temp = new unicode_t[strlen(sString) + 1];
    if (NULL == temp)
    {
      ID3_THROW(ID3E_NoMemory);
    }

    mbstoucs(temp, sString, strlen(sString) + 1);
    Add(temp);
    delete [] temp;
    
    __eType = ID3FTY_ASCIISTRING;
  }
  
  return ;
}


size_t 
ID3_Field::ParseASCIIString(const uchar *buffer, luint posn, size_t nSize)
{
  size_t bytesUsed = 0;
  char *temp = NULL;
  
  if (__lFixedLength != -1)
  {
    // The string is of fixed length
    bytesUsed = __lFixedLength;
  }
  else if (!(__ulFlags & ID3FF_NULL) || (__ulFlags & ID3FF_NULLDIVIDE))
  {
    // If the string isn't null-terminated or if it is null divided, we're
    // assured this is the last field of of the frame, and we can claim the
    // remaining bytes for ourselves
    bytesUsed = nSize - posn;
  }
  else
  {
    while ((posn + bytesUsed) < nSize && buffer[posn + bytesUsed] != '\0')
    {
      bytesUsed++;
    }
  }

  if (0 == bytesUsed)
  {
    Set("");
  }
  // This check needs to come before the check for ID3FF_NULL
  else if (__ulFlags & ID3FF_NULLDIVIDE)
  {
    char *sBuffer = (char *) &buffer[posn];
    for (size_t nIndex = 0; nIndex < bytesUsed; 
         nIndex += strlen(&sBuffer[nIndex]) + 1)
    {
      Add(&sBuffer[nIndex]);
    }
  }
  // This check needs to come after the check for ID3FF_NULLDIVIDE
  else if (__ulFlags & ID3FF_NULL)
  {
    char *sBuffer = (char *) &buffer[posn];
    Set(sBuffer);
  }
  else
  {
    // Sanity check our indices and sizes before we start copying memory
    if ((bytesUsed > nSize) || (posn + bytesUsed > nSize))
    {
      ID3_THROW_DESC(ID3E_BadData, "field information invalid");
    }

    temp = new char[bytesUsed + 1];
    if (NULL == temp)
    {
      ID3_THROW(ID3E_NoMemory);
    }
    
    memcpy(temp, &buffer[posn], bytesUsed);
    temp[bytesUsed] = '\0';
    Set(temp);
      
    delete [] temp;
  }
  
  if (__ulFlags & ID3FF_NULL && !(__ulFlags & ID3FF_NULLDIVIDE))
  {
    bytesUsed++;
  }
    
  __bHasChanged = false;
  
  return bytesUsed;
}


luint ID3_Field::RenderASCIIString(uchar *buffer)
{
  luint nChars = BinSize();

  if ((NULL != __sData) && (nChars > 0))
  {
    ucstombs((char *) buffer, (unicode_t *) __sData, nChars);
      
    // now we convert the internal dividers to what they are supposed to be
    for (luint nIndex = 0; nIndex < nChars; nIndex++)
    {
      if ('\1' == buffer[nIndex])
      {
        char sub = '/';
          
        if (__ulFlags & ID3FF_NULLDIVIDE)
        {
          sub = '\0';
        }
        buffer[nIndex] = sub;
      }
    }
  }
  
  if ((1 == nChars) && (__ulFlags & ID3FF_NULL))
  {
    buffer[0] = '\0';
  }
    
  __bHasChanged = false;
  
  return nChars;
}

// $Log$
// Revision 1.17  2000/01/04 15:42:49  eldamitri
// * include/id3/field.h:
// * include/id3/int28.h:
// * include/id3/misc_support.h:
// * include/id3/tag.h:
// * include/id3/types.h:
// * src/id3/dll_wrapper.cpp
// * src/id3/error.cpp
// * src/id3/field.cpp
// * src/id3/field_binary.cpp
// * src/id3/field_integer.cpp
// * src/id3/field_string_ascii.cpp
// * src/id3/field_string_unicode.cpp
// * src/id3/frame.cpp
// * src/id3/frame_parse.cpp
// * src/id3/frame_render.cpp
// * src/id3/header.cpp
// * src/id3/header_frame.cpp
// * src/id3/header_tag.cpp
// * src/id3/int28.cpp
// * src/id3/misc_support.cpp
// * src/id3/tag.cpp
// * src/id3/tag_file.cpp:
// * src/id3/tag_find.cpp:
// * src/id3/tag_parse.cpp:
// * src/id3/tag_parse_lyrics3.cpp:
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.16  1999/12/26 15:10:48  scott
// Minor reformatting.
//
// Revision 1.15  1999/12/23 15:56:50  scott
// (RenderASCIIString): Rewrote the function so that it doesn't create
// temporary dynamic storage, and so that it copies the number of
// characters, not the number of bytes, in the string.
//
// Revision 1.14  1999/12/17 16:13:04  scott
// Updated opening comment block.
//
// Revision 1.13  1999/12/15 06:36:45  scott
// (ParseASCIIString): Better comment for assigning bytesUsed.
//
// Revision 1.12  1999/12/13 04:44:07  scott
// (Get): Potential memory leaks plugged (thanks MusicMatch).
// (RenderASCIIString): Bugfix for coversion from unicode to ascii string
// (thanks MusicMatch).  Some code cleanup.
//
// Revision 1.11  1999/12/09 03:32:06  scott
// (ParseASCIIString): Fixed bug which prevented correct parsing of fields
// separated with NULL dividers (such as the involved people frame).
// Slightly restructured the code for better performance.
//
// Revision 1.10  1999/12/01 18:00:59  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places (and to make compiling under
// windows easier).
//
// Revision 1.9  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.8  1999/11/29 17:23:16  scott
// Updated methods to use unicode_t instead of wchar_t.  unicode_t is
// defined in globals.h.  The type unicode_t is more likely to be two
// bytes on every platform.  Updated calls to unicode methods to more
// general methods defined in misc_support.cpp.
//
// Revision 1.7  1999/11/19 17:34:44  scott
// (operator=): Updated interface to make parameters const.
// (Set): Updated interface to make parameters const.  Bug fix for fixed
// length strings.
// (Get): Removed check for nonempty strings so empty strings would be
// set correctly.  Minor code cleanup.
// (Add): Updated interface to make parameters const.  Made variable name
// more descriptive.
//
// Revision 1.6  1999/11/16 22:50:24  scott
// * field_string_ascii.cpp (ParseASCIIString): Added sanity check
// for indices so we don't call memcpy with out-of-bounds indices.
//
// Revision 1.5  1999/11/15 20:16:15  scott
// Added include for config.h.  Minor code cleanup.  Removed
// assignments from if checks; first makes assignment, then checks
// for appropriate value.  Made private member variable names more
// descriptive.
//
// Revision 1.4  1999/11/04 04:15:54  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
