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
#include <memory.h>
#include "tag.h"
#include "misc_support.h"
#include "utils.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

size_t ParseID3v1(ID3_Tag& tag, fstream& file)
{
  size_t num_bytes = 0;
  if (!file)
  {
    return num_bytes;
  }

  ID3V1_Tag tagID3v1;
    
  // posn ourselves at 128 bytes from the current position
  file.seekg(-128, ios::cur);
  //file.seekg(-static_cast<long>(ID3_V1_LEN), ios::cur);
  if (!file)
  {
    return num_bytes;
    // TODO:  This is a bad error message.  Make it more descriptive
  }
    
  // read the next 128 bytes in;
  file.read(tagID3v1.sID, ID3_V1_LEN_ID);
  if (file.gcount() != ID3_V1_LEN_ID)
  {
    // TODO:  This is a bad error message.  Make it more descriptive
    return num_bytes;
    //ID3_THROW(ID3E_NoData);
  }
    
  // check to see if it was a tag
  if (memcmp(tagID3v1.sID, "TAG", ID3_V1_LEN_ID) == 0)
  {
    const size_t data_size = ID3_V1_LEN - ID3_V1_LEN_ID;
    uchar tag_bytes[data_size];
    file.read(tag_bytes, data_size);
    if (file.gcount() != data_size)
    {
      return num_bytes;
    }
    // guess so, let's start checking the v2 tag for frames which are the
    // equivalent of the v1 fields.  When we come across a v1 field that has
    // no current equivalent v2 frame, we create the frame, copy the data
    // from the v1 frame and attach it to the tag

    size_t offset = 0;

    memcpy(tagID3v1.sTitle, &tag_bytes[offset], ID3_V1_LEN_TITLE);
    tagID3v1.sTitle[ID3_V1_LEN_TITLE] = '\0';
    RemoveTrailingSpaces(tagID3v1.sTitle,  ID3_V1_LEN_TITLE);
    ID3_AddTitle(&tag, tagID3v1.sTitle);
    offset += ID3_V1_LEN_TITLE;

    memcpy(tagID3v1.sArtist, &tag_bytes[offset], ID3_V1_LEN_ARTIST);
    tagID3v1.sTitle[ID3_V1_LEN_ARTIST] = '\0';
    RemoveTrailingSpaces(tagID3v1.sArtist,  ID3_V1_LEN_ARTIST);
    ID3_AddArtist(&tag, tagID3v1.sArtist);
    offset += ID3_V1_LEN_ARTIST;

    memcpy(tagID3v1.sAlbum, &tag_bytes[offset], ID3_V1_LEN_ALBUM);
    tagID3v1.sTitle[ID3_V1_LEN_ALBUM] = '\0';
    RemoveTrailingSpaces(tagID3v1.sAlbum,  ID3_V1_LEN_ALBUM);
    ID3_AddAlbum(&tag, tagID3v1.sAlbum);
    offset += ID3_V1_LEN_ALBUM;

    memcpy(tagID3v1.sYear, &tag_bytes[offset], ID3_V1_LEN_YEAR);
    tagID3v1.sTitle[ID3_V1_LEN_YEAR] = '\0';
    RemoveTrailingSpaces(tagID3v1.sYear,  ID3_V1_LEN_YEAR);
    ID3_AddYear(&tag, tagID3v1.sYear);
    offset += ID3_V1_LEN_YEAR;
    
    memcpy(tagID3v1.sComment, &tag_bytes[offset], ID3_V1_LEN_COMMENT);
    tagID3v1.sTitle[ID3_V1_LEN_COMMENT] = '\0';
    RemoveTrailingSpaces(tagID3v1.sComment,  ID3_V1_LEN_COMMENT);
    if ('\0' != tagID3v1.sComment[ID3_V1_LEN_COMMENT - 2] ||
        '\0' == tagID3v1.sComment[ID3_V1_LEN_COMMENT - 1])
    {
      RemoveTrailingSpaces(tagID3v1.sComment, ID3_V1_LEN_COMMENT);
    }
    else
    {
      // This is an id3v1.1 tag.  The last byte of the comment is the track
      // number.  
      RemoveTrailingSpaces(tagID3v1.sComment, ID3_V1_LEN_COMMENT - 1);
      ID3_AddTrack(&tag, tagID3v1.sComment[ID3_V1_LEN_COMMENT - 1]);
    }
    ID3_AddComment(&tag, tagID3v1.sComment, STR_V1_COMMENT_DESC);
    offset += ID3_V1_LEN_COMMENT;
      
    // the GENRE field/frame
    tagID3v1.ucGenre = tag_bytes[offset];
    ID3_AddGenre(&tag, tagID3v1.ucGenre);

    num_bytes += ID3_V1_LEN;
  }
    
  return num_bytes;
}
