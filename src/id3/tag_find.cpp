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

#include <string.h>
#include "tag.h"
#include "misc_support.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

ID3_Elem *ID3_Tag::Find(ID3_Frame *frame) const
{
  ID3_Elem *elem = NULL;
  
  for (ID3_Elem *cur = __pFrameList; NULL != cur; cur = cur->pNext)
    if (cur->pFrame == frame)
    {
      elem = cur;
      break;
    }
  
  return elem;
}

ID3_Frame *ID3_Tag::Find(ID3_FrameID id)
{
  ID3_Frame *frame = NULL;
  
  // reset the cursor if it isn't set
  if (NULL == __pFindCursor)
    __pFindCursor = __pFrameList;

  for (int iCount = 0; iCount < 2 && frame == NULL; iCount++)
  {
    // We want to cycle through the list to find the matching frame.  We
    // should start from the cursor, search each successive frame, wrapping
    // if necessary.  The enclosing loop and the assignment statments below
    // ensure that we first start at the cursor and search to the end of the
    // list and, if unsuccessful, start from the beginning of the list and
    // search to the cursor.
    ID3_Elem
      *pStart  = (0 == iCount ? __pFindCursor : __pFrameList), 
      *pFinish = (0 == iCount ? NULL          : __pFindCursor);
    // search from the cursor to the end
    for (ID3_Elem *cur = pStart; cur != pFinish; cur = cur->pNext)
    {
      if ((cur->pFrame != NULL) && (cur->pFrame->GetID() == id))
      {
        // We've found a valid frame.  Set the cursor to be the next element
        frame = cur->pFrame;
        __pFindCursor = cur->pNext;
        break;
      }
    }
  }
  
  return frame;
}

ID3_Frame *ID3_Tag::Find(ID3_FrameID id, ID3_FieldID fld, char *data)
{
  ID3_Frame *frame = NULL;
  unicode_t *temp;
  
  temp = new unicode_t[strlen(data) + 1];
  if (NULL == temp)
    ID3_THROW(ID3E_NoMemory);

  mbstoucs(temp, data, strlen(data) + 1);
    
  frame = Find(id, fld, temp);
    
  delete[] temp;
  
  return frame;
}

ID3_Frame *ID3_Tag::Find(ID3_FrameID id, ID3_FieldID fld, unicode_t *data)
{
  ID3_Frame *frame = NULL;
  
  // reset the cursor if it isn't set
  if (NULL == __pFindCursor)
    __pFindCursor = __pFrameList;

  for (int iCount = 0; iCount < 2 && frame == NULL; iCount++)
  {
    // We want to cycle through the list to find the matching frame.  We
    // should start from the cursor, search each successive frame, wrapping
    // if necessary.  The enclosing loop and the assignment statments below
    // ensure that we first start at the cursor and search to the end of the
    // list and, if unsuccessful, start from the beginning of the list and
    // search to the cursor.
    ID3_Elem
      *pStart  = (0 == iCount ? __pFindCursor : __pFrameList), 
      *pFinish = (0 == iCount ? NULL          : __pFindCursor);
    // search from the cursor to the end
    for (ID3_Elem *cur = pStart; cur != pFinish; cur = cur->pNext)
    {
      if ((cur->pFrame != NULL) && (cur->pFrame->GetID() == id) &&
          (data != NULL) && ucslen(data) > 0 && 
          BS_ISSET(cur->pFrame->__auiFieldBits, fld))
      {
        luint ulSize = cur->pFrame->Field(fld).BinSize();
        unicode_t *wsBuffer = new unicode_t[ulSize];
          
        if (NULL == wsBuffer)
          ID3_THROW(ID3E_NoMemory);
          
        cur->pFrame->Field(fld).Get(wsBuffer, ulSize);
          
        bool bInFrame = (ucscmp(wsBuffer, data) == 0);
          
        delete [] wsBuffer;

        if (bInFrame)
        {
          // We've found a valid frame.  Set cursor to be the next element
          frame = cur->pFrame;
          __pFindCursor = cur->pNext;
          break;
        }
      }
    }
  }
  
  return frame;
}

ID3_Frame *ID3_Tag::Find(ID3_FrameID id, ID3_FieldID fld, luint data)
{
  ID3_Frame *frame = NULL;
  
  // reset the cursor if it isn't set
  if (NULL == __pFindCursor)
    __pFindCursor = __pFrameList;

  for (int iCount = 0; iCount < 2 && frame == NULL; iCount++)
  {
    // We want to cycle through the list to find the matching frame.  We
    // should start from the cursor, search each successive frame, wrapping
    // if necessary.  The enclosing loop and the assignment statments below
    // ensure that we first start at the cursor and search to the end of the
    // list and, if unsuccessful, start from the beginning of the list and
    // search to the cursor.
    ID3_Elem
      *pStart  = (0 == iCount ? __pFindCursor : __pFrameList), 
      *pFinish = (0 == iCount ? NULL          : __pFindCursor);
    // search from the cursor to the end
    for (ID3_Elem *cur = pStart; cur != pFinish; cur = cur->pNext)
    {
      if ((cur->pFrame != NULL) && (cur->pFrame->GetID() == id) &&
          (cur->pFrame->Field(fld).Get() == data))
      {
        // We've found a valid frame.  Set the cursor to be the next element
        frame = cur->pFrame;
        __pFindCursor = cur->pNext;
        break;
      }
    }
  }
  
  return frame;
}

ID3_Frame *ID3_Tag::GetFrameNum(luint num) const
{
  ID3_Frame *frame = NULL;

  luint curNum = 0;
  for (ID3_Elem *cur = __pFrameList; cur != NULL; cur = cur->pNext)
    // compare and advance counter
    if (num == curNum++)
    {
      frame = cur->pFrame;
      break;
    }
  
  return frame;
}

ID3_Frame *ID3_Tag::operator[](luint num) const
{
  return GetFrameNum(num);
}

// $Log$
// Revision 1.12  2000/04/05 05:21:15  eldamitri
// Updated initial comment information to reflect license, copyright
// change.
//
// Revision 1.11  2000/01/04 15:42:49  eldamitri
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.10  1999/12/17 16:13:04  scott
// Updated opening comment block.
//
// Revision 1.9  1999/12/01 18:00:59  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places (and to make compiling under
// windows easier).
//
// Revision 1.8  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.7  1999/11/29 18:47:37  scott
// (): Updated methods to use unicode_t instead of wchar_t.  unicode_t is
// defined in globals.h.  The type unicode_t is more likely to be two
// bytes on every platform.  Updated calls to unicode methods to more
// general methods defined in misc_support.cpp.
//
// Revision 1.6  1999/11/19 19:10:14  scott
// * tag_find.cpp (Find): Add const qualifier.
//
// Revision 1.5  1999/11/15 20:20:47  scott
// Added include for config.h.  Removed assignments from if checks;
// first makes assignment, then checks for appropriate value.  Made
// private member variable names more descriptive.  Major code
// cleanup: use of for loops instead of while's to ease code
// readability, more compact looping structures to reduce possibility
// in coding or human parsing errors,
//
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
