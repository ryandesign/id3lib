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
#include <zlib.h>
#include "tag.h"
#include "uint28.h"
#include "utils.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

size_t ID3_ParseFrames(ID3_Tag& tag, const uchar* const data, size_t size) 
{ 
  const uchar* const data_end = data + size; 
  size_t total_size = 0; 
  size_t frame_size = 0; 
  for (const uchar* p = data; p < data_end && *p != '\0'; p += frame_size) 
  { 
    ID3_Frame* f = new ID3_Frame; 
    f->SetSpec(tag.GetSpec());
    frame_size = f->Parse(p, data_end - p); 
    total_size += frame_size; 
     
    if (f->BadParse()) 
    { 
      // bad parse!  we can't attach this frame.  should probably log this 
      // TODO: log this 
      delete f; 
    } 
    else if (!frame_size) 
    { 
      // There is a problem. 
      // If the frame size is 0, then we can't progress. 
      // TODO: log this 
      delete f; 
      // Break for now. 
      break; 
    } 
    else if (f->GetID() != ID3FID_METACOMPRESSION) 
    { 
      // a good, uncompressed frame.  attach away! 
      tag.AttachFrame(f); 
    } 
    else 
    { 
      // hmm.  an ID3v2.2.1 compressed frame.  It contains 1 or more compressed
      // frames.  Uncompress and call ParseFrames recursively.
      const uchar* const bin = f->Field(ID3FN_DATA).GetBinary(); 
      if (*bin != 'z') 
      { 
        // unknown compression method 
        // TODO: log this 
      } 
      else 
      { 
        uint32 new_size = ParseNumber(&bin[1]); 
         
        uchar* uncompressed = new uchar[new_size]; 
         
        uncompress(uncompressed, (luint *) &new_size, 
                   &bin[1 + sizeof(uint32)], 
                   f->GetDataSize() - sizeof(uint32) - 1); 

        if (ID3_ParseFrames(tag, uncompressed, new_size) != new_size) 
        { 
          // hmm.  it didn't parse the entire uncompressed data.  wonder why. 
          // TODO: log this. 
        } 
         
        delete [] uncompressed; 
      } 
    } 
  } 
   
  return total_size; 
} 

/** Turns a binary tag into a series of ID3_Frame objects attached to the
 ** tag.
 ** 
 ** \code
 **   ID3_Tag myTag;
 **   uchar header[ID3_TAGHEADERSIZE];
 **   uchar *buffer;
 **   luint tagSize;
 ** 
 **   // get ID3_TAGHEADERSIZE from a socket or somewhere
 **   // ...
 ** 
 **   if ((tagSize = ID3_IsTagHeader(ourSourceBuffer)) > -1)
 **   {
 **     // read a further 'tagSize' bytes in
 **     // from our data source
 **     // ...
 **   
 **     if (buffer = new uchar[tagSize])
 **     {
 **       // now we will call ID3_Tag::Parse()
 **       // with these values (explained later)
 **       myTag.Parse(header, buffer);
 **       
 **       // do something with the objects,
 **       // like look for titles, artists, etc.
 **       // ...
 **       
 **       // free the buffer
 **       delete [] buffer;
 **     }
 **   }
 ** \endcode
 ** 
 ** @see ID3_Frame
 ** @param header The byte header read in from the data source.
 ** @param buffer The remainder of the tag (not including the data source) 
 **               read in from the data source.
 **/
size_t ID3_Tag::Parse(const uchar header[ID3_TagHeader::SIZE],
		      const uchar *buffer)
{
  size_t hdr_size = __hdr.Parse(header, ID3_TagHeader::SIZE);
  if (!hdr_size)
  {
    return 0;
  }

  size_t data_size = __hdr.GetDataSize();
  uchar* unsynced_data = NULL;
  if (__hdr.GetUnsync())
  {
    unsynced_data = new uchar[data_size];
    memcpy(unsynced_data, buffer, data_size);
    data_size = ID3_ReSync(unsynced_data, data_size);
    buffer = unsynced_data;
  }

  ID3_ParseFrames(*this, buffer, data_size);

  // set the flag which says that the tag hasn't changed
  __changed = false;

  delete [] unsynced_data;
  
  return hdr_size + __hdr.GetDataSize();
}

size_t ParseID3v2(ID3_Tag& tag, fstream& file)
{
  size_t size = 0;

  if (!file)
  {
    return 0;
  }
  uchar header[ID3_TAGHEADERSIZE];
  file.read(header, ID3_TAGHEADERSIZE);
  if (file.gcount() != ID3_TAGHEADERSIZE)
  {
    return 0;
  }
  
  size_t tagSize = ID3_Tag::IsV2Tag(header);
  if (tagSize > 0)
  {
    uchar* bin = new uchar[tagSize];
    file.read(bin, tagSize - ID3_TagHeader::SIZE);
    if (file.gcount() != tagSize)
    {
      // log this...
      //return 0;
      //ID3_THROW_DESC(ID3E_NoFile, 
      //               "ID3_Tag::ParseFromHandle: Ack! Couldn't read");
    }
    
    size += tag.Parse(header, bin);
    
    delete[] bin;
  }

  return size;
}

size_t ParseMusicMatch(ID3_Tag&, fstream&);
size_t ParseLyrics3(ID3_Tag&, fstream&);
size_t ParseLyrics3v2(ID3_Tag&, fstream&);
size_t ParseID3v1(ID3_Tag&, fstream&);

void ID3_Tag::ParseFile()
{
  fstream file;
  if (ID3E_NoError != ID3_OpenReadableFile(this->GetFileName(), file))
  {
    // log this...
    return;
  }

  size_t bytes = 0;
  __file_tags.clear();
  __prepended_bytes = 0;
  __file_size = ID3_GetFileSize(file);
  do
  {
    file.seekg(__prepended_bytes, ios::beg);
    bytes = 0;

    // Parse tags at the beginning of the file first...
    if (__tags_to_parse.test(ID3TT_ID3V2))
    {
      bytes = ParseID3v2(*this, file);
      if (bytes)
      {
        // say we have v2 tags
        __file_tags.add(ID3TT_ID3V2);
      }
    }
    __prepended_bytes += bytes;
  } while (bytes);
  
  __appended_bytes = 0;
  do
  {
    bytes = 0;
    // ...then the tags at the end
    if (!bytes && __tags_to_parse.test(ID3TT_MUSICMATCH))
    {
      file.seekg(- static_cast<long>(__appended_bytes), ios::end);
      bytes = ParseMusicMatch(*this, file);
      if (bytes)
      {
        __file_tags.add(ID3TT_MUSICMATCH);
      }
    }
    if (!bytes && __tags_to_parse.test(ID3TT_LYRICS3))
    {
      file.seekg(- static_cast<long>(__appended_bytes), ios::end);
      bytes = ParseLyrics3(*this, file);
      if (bytes)
      {
        __file_tags.add(ID3TT_LYRICS3);
      }
    }
    if (!bytes && __tags_to_parse.test(ID3TT_LYRICS3V2))
    {
      file.seekg(- static_cast<long>(__appended_bytes), ios::end);
      bytes = ParseLyrics3v2(*this, file);
      if (bytes)
      {
        __file_tags.add(ID3TT_LYRICS3V2);
      }
    }
    if (!bytes && __tags_to_parse.test(ID3TT_ID3V1))
    {
      file.seekg(- static_cast<long>(__appended_bytes), ios::end);
      bytes = ParseID3v1(*this, file);
      if (bytes)
      {
        __file_tags.add(ID3TT_ID3V1);
      }
    }
    __appended_bytes += bytes;
  } while (bytes);
}
