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

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

#include "debug.h"

#include <string.h>
#include <memory.h>
#include <zlib.h>

#include "tag.h"
#include "utils.h"
#include "frame_impl.h"

// Ideally, Render should render the frame in the order it is parsed, starting
// with the header, then any extra info (resulting from header flags), and then
// the frame data, one field at a time.  This, unfortunately, isn't possible.
// In order to render the frame header, the data size must be known.  One could
// query the fields' sizes before rendering them, but it would be faster to
// render the fields and add their sizes (which is calculated and returned in
// the fields' Render function) at the same time.
//
// Even if we determined this extra processing to be acceptable, it isn't
// possible to precompute the frame data size without rendering them all when
// compression is enabled.  When compression is enabled, then the header
// contains the size of the compressed data.  So all the fields need to
// rendered to a buffer and compressed before the data size can even be
// calculated in this scenario.  And the compressed data might even be
// discarded if it is no smaller than the uncompressed data.
//
// So, Render should progress more-or-less as follows.  The fields should be
// rendered to the target buffer at its desired position (after the projected
// header and extra bytes (at most 2)).  If compression has been specified,
// then compress it into a temporary buffer.  If the compressed size is smaller
// than the uncompressed data, copy the temp buffer to the correct place in the
// target buffer.  Finally, render the header and the extra info.  The
// encryption and grouping id's shouldn't be rendered until after the
// compression takes place, since they will be rendered to a different spots in
// the buffer depending on whether or not the data is compressed.  This is
// because the uncompressed data size must come after before the id's if the
// the data is compressed, but won't be rendered if the data isn't compressed.
//
// One could probably implement Render more "naturally" by using more
// dynamically allocated buffers, but that would degrade performance. Whether
// or not this performance degradation would cause a significant impact on the
// overall performane of typeical apps should be looked in to.
//
// So, in short, Render is a more complicated function than it should be, but
// it doesn't look like there's a much better way that's any faster.
size_t ID3_FrameImpl::Render(uchar *buffer) const
{
  if (NULL == buffer)
  {
    // log this
    return 0;
    //ID3_THROW(ID3E_NoBuffer);
  }

  // Return immediately if we have no fields, which (usually) means we're
  // trying to render a frame which has been Cleared or hasn't been initialized
  if (!_num_fields)
  {
    return 0;
  }

  uchar e_id = this->GetEncryptionID(), g_id = this->GetGroupingID();
  size_t decompressed_size = 0;
  size_t extras = ( e_id > 0 ? 1 : 0 ) + ( g_id > 0 ? 1 : 0 );
  ID3_FrameHeader hdr = _hdr;
  
  const size_t hdr_size = hdr.Size();

  // 1.  Write out the field data to the buffer, with the assumption that
  //     we won't be decompressing, since this is the usual behavior
  ID3_TextEnc enc = ID3TE_ASCII;
  size_t data_size = 0;
    
  for (ID3_Field** fi = _fields; fi != _fields + _num_fields; fi++)
  {
    if ((*fi)->GetID() == ID3FN_TEXTENC)  
    {
      enc = static_cast<ID3_TextEnc>((*fi)->Get());  
    }
    
    if (*fi && (*fi)->InScope(ID3V2_LATEST))
    {
      (*fi)->SetEncoding(enc);
      data_size += (*fi)->Render(&buffer[data_size + hdr_size + extras]);
    }
  }
  
  // Return if there's no data
  if (!data_size)
  {
    return 0;
  }
  
  // 2.  Attempt to compress if specified
  if (this->GetCompression())
  {
    // The zlib documentation specifies that the destination size needs to be
    // an unsigned long at least 0.1% larger than the source buffer, plus 12
    // bytes
    unsigned long new_data_size = data_size + (data_size / 10) + 12;
      
    uchar* compressed_data = new uchar[new_data_size];
    if (NULL == compressed_data)
    {
      ID3_THROW(ID3E_NoMemory);
    }

    if (compress(compressed_data, &new_data_size, &buffer[hdr_size + extras],
                 data_size) != Z_OK)
    {
      ID3_THROW(ID3E_zlibError);
    }

    // if the compression actually saves space...
    if ((new_data_size + sizeof(uint32)) < data_size)
    {
      // add 4 bytes to 'extras' for the decompressed size
      extras += sizeof(uint32);

      memcpy(&buffer[hdr_size + extras], compressed_data, new_data_size);

      decompressed_size = data_size;
      data_size = new_data_size;
    }
    
    delete [] compressed_data;
  }
  
  // determine which flags need to be set
  hdr.SetDataSize(data_size + extras);
  hdr.SetEncryption(e_id > 0);
  hdr.SetGrouping(g_id > 0);
  hdr.SetCompression(decompressed_size > 0);

  hdr.Render(buffer);
  uchar* data = buffer + hdr_size;
  if (decompressed_size)
  {
    data += id3::renderNumber(data, decompressed_size);
  }
  if (e_id)
  {
    *data++ = e_id;
  }
  if (g_id)
  {
    *data++ = g_id;
  }
  _changed = false;
    
  return hdr_size + extras + data_size;
}
