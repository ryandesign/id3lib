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

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "tag.h"
#include "misc_support.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

void ID3_RemoveTrailingSpaces(char *buffer, luint length)
{
  for (lsint i = length - 1; i > -1 && 0x20 == buffer[i]; i--)
  {
    buffer[i] = '\0';
  }
      
  return ;
}


void ID3_Tag::ParseID3v1(void)
{
  if (NULL == __fFileHandle)
  {
    ID3_THROW(ID3E_NoData);
  }

  ID3V1_Tag tagID3v1;
    
  // posn ourselves at 128 bytes from the end of the file
  if (fseek(__fFileHandle, 0-ID3_V1_LEN, SEEK_END) != 0)
  {
    // TODO:  This is a bad error message.  Make it more descriptive
    ID3_THROW(ID3E_NoData);
  }
    
    
  // read the next 128 bytes in;
  if (fread(tagID3v1.sID, 1, ID3_V1_LEN_ID, __fFileHandle) != ID3_V1_LEN_ID)
  {
    // TODO:  This is a bad error message.  Make it more descriptive
    ID3_THROW(ID3E_NoData);
  }
    
  // check to see if it was a tag
  if (memcmp(tagID3v1.sID, "TAG", ID3_V1_LEN_ID) == 0)
  {
    // guess so, let's start checking the v2 tag for frames which are the
    // equivalent of the v1 fields.  When we come across a v1 field that has
    // no current equivalent v2 frame, we create the frame, copy the data
    // from the v1 frame and attach it to the tag
      
    __bHasV1Tag = true;
    __ulExtraBytes += ID3_V1_LEN;

    // the TITLE field/frame
    if (fread(tagID3v1.sTitle, 1, ID3_V1_LEN_TITLE, __fFileHandle) != ID3_V1_LEN_TITLE)
    {
      // TODO:  This is a bad error message.  Make it more descriptive
      ID3_THROW(ID3E_NoData);
    }
    tagID3v1.sTitle[ID3_V1_LEN_TITLE] = '\0';
    ID3_RemoveTrailingSpaces(tagID3v1.sTitle,  ID3_V1_LEN_TITLE);
    ID3_AddTitle(this, tagID3v1.sTitle);
    
    // the ARTIST field/frame
    if (fread(tagID3v1.sArtist, 1, ID3_V1_LEN_ARTIST, __fFileHandle) != 
        ID3_V1_LEN_ARTIST)
    {
      // TODO:  This is a bad error message.  Make it more descriptive
      ID3_THROW(ID3E_NoData);
    }
    tagID3v1.sArtist[ID3_V1_LEN_ARTIST] = '\0';
    ID3_RemoveTrailingSpaces(tagID3v1.sArtist, ID3_V1_LEN_ARTIST);
    ID3_AddArtist(this, tagID3v1.sArtist);
  
    // the ALBUM field/frame
    if (fread(tagID3v1.sAlbum, 1, ID3_V1_LEN_ALBUM, __fFileHandle) != ID3_V1_LEN_ALBUM)
    {
      // TODO:  This is a bad error message.  Make it more descriptive
      ID3_THROW(ID3E_NoData);
    }
    tagID3v1.sAlbum[ID3_V1_LEN_ALBUM] = '\0';
    ID3_RemoveTrailingSpaces(tagID3v1.sAlbum,  ID3_V1_LEN_ALBUM);
    ID3_AddAlbum(this, tagID3v1.sAlbum);
  
    // the YEAR field/frame
    if (fread(tagID3v1.sYear, 1, ID3_V1_LEN_YEAR, __fFileHandle) != ID3_V1_LEN_YEAR)
    {
      // TODO:  This is a bad error message.  Make it more descriptive
      ID3_THROW(ID3E_NoData);
    }
    tagID3v1.sYear[ID3_V1_LEN_YEAR] = '\0';
    ID3_RemoveTrailingSpaces(tagID3v1.sYear,   ID3_V1_LEN_YEAR);
    ID3_AddYear(this, tagID3v1.sYear);
  
    // the COMMENT field/frame
    if (fread(tagID3v1.sComment, 1, ID3_V1_LEN_COMMENT, __fFileHandle) !=
        ID3_V1_LEN_COMMENT)
    {
      // TODO:  This is a bad error message.  Make it more descriptive
      ID3_THROW(ID3E_NoData);
    }
    tagID3v1.sComment[ID3_V1_LEN_COMMENT] = '\0';
    if ('\0' != tagID3v1.sComment[ID3_V1_LEN_COMMENT - 2] ||
        '\0' == tagID3v1.sComment[ID3_V1_LEN_COMMENT - 1])
    {
      ID3_RemoveTrailingSpaces(tagID3v1.sComment, ID3_V1_LEN_COMMENT);
    }
    else
    {
      // This is an id3v1.1 tag.  The last byte of the comment is the track
      // number.  
      ID3_RemoveTrailingSpaces(tagID3v1.sComment, ID3_V1_LEN_COMMENT - 1);
      ID3_AddTrack(this, tagID3v1.sComment[ID3_V1_LEN_COMMENT - 1]);
    }
    ID3_AddComment(this, tagID3v1.sComment, STR_V1_COMMENT_DESC);
      
    // the GENRE field/frame
    fread(&tagID3v1.ucGenre, 1, ID3_V1_LEN_GENRE, __fFileHandle);
    ID3_AddGenre(this, tagID3v1.ucGenre);
  }
    
  return ;
}

// $Log$
// Revision 1.2  2000/04/18 22:13:49  eldamitri
// Moved tag_parse_v1.cpp from src/id3/ to src/
//
// Revision 1.15  2000/04/08 04:42:59  eldamitri
// Changed new ANSI-standard C++ include headers to old-style headers.
//
// Revision 1.14  2000/04/05 05:21:15  eldamitri
// Updated initial comment information to reflect license, copyright
// change.
//
// Revision 1.13  2000/01/04 15:42:49  eldamitri
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.12  1999/12/17 16:13:04  scott
// Updated opening comment block.
//
// Revision 1.11  1999/12/16 14:40:31  scott
// Fixed inclusion config.h (thanks Alexander Voronin).
//
// Revision 1.10  1999/12/05 05:34:27  scott
// (ParseID3v1): Added STR_V1_COMMENT_DESC as description parameter to
// call to ID3_AddComment
//
// Revision 1.9  1999/12/01 22:19:51  scott
// (ParseID3v1): Minor fix for windows compatibility (thanks elrod).
//
// Revision 1.8  1999/12/01 18:00:59  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places (and to make compiling under
// windows easier).
//
// Revision 1.7  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.6  1999/11/19 19:12:46  scott
// (ID3_RemoveTrailingSpaces): Minor code cleanup.
// (ParseID3v1): Massive recoding of method to make use of new functions
// defined in misc_support.  Simplifies flow of method.
//
// Revision 1.5  1999/11/15 20:21:15  scott
// Added include for config.h.  Minor code cleanup.  Removed
// assignments from if checks; first makes assignment, then checks
// for appropriate value.  Made private member variable names more
// descriptive.
//
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
