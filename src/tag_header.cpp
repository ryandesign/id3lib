// id3lib: a C++ library for creating and manipulating id3v1/v2 tags
// $Id$

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

#include "uint28.h"
#include "tag_header.h"
#include "details.h"
#include "conversion_ops.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{
  
  const bstring tag_header::ID           = to_bstring("ID3");
  const size_t  tag_header::ID_SIZE      = tag_header::ID.size();
  const size_t  tag_header::MAJOR_OFFSET = tag_header::ID_SIZE;
  const size_t  tag_header::MINOR_OFFSET = tag_header::MAJOR_OFFSET + sizeof(uchar);
  const size_t  tag_header::FLAGS_OFFSET = tag_header::MINOR_OFFSET + sizeof(uchar);
  const size_t  tag_header::SIZE_OFFSET  = tag_header::FLAGS_OFFSET + sizeof(uint8);
  const size_t  tag_header::SIZE         = tag_header::SIZE_OFFSET  + sizeof(uint32);
  
  // Analyses a buffer to determine if we have a valid ID3v2 tag header.
  // If so, return the number of bytes (including the header) to
  // read so we get all of the tag
  // 
  size_t is_tag(const bstring& data)
  {
    if (data.size() < tag_header::SIZE)
    {
      return 0;
    }
    
    if (tag_header::ID == data.substr(0, tag_header::ID_SIZE) &&
        data[tag_header::MAJOR_OFFSET]    <  0xFF &&
        data[tag_header::MINOR_OFFSET]    <  0xFF &&
        data[tag_header::SIZE_OFFSET + 0] <  0x80 &&
        data[tag_header::SIZE_OFFSET + 1] <  0x80 &&
        data[tag_header::SIZE_OFFSET + 2] <  0x80 &&
        data[tag_header::SIZE_OFFSET + 3] <  0x80)
    {
      uint28 data_size(data.substr(tag_header::SIZE_OFFSET, sizeof(uint32)));
      return data_size.to_uint32() + tag_header::SIZE;
    }
    
    return 0;
  }
  
  bool tag_header::version(spec::version v)
  {
    bool changed = this->header::version(v);
    
    if (changed)
    {
      const details* pv = details::instance(v);
      this->set(EXPERIMENTAL, pv->experimental());
      this->set(EXTENDEDHEADER, pv->extended_header());
    }
  }
  
  spec::version tag_header::version() const
  {
    return header::version();
  }
  
  size_t tag_header::size() const
  {
    size_t size = tag_header::SIZE;
    
    if (this->test(EXTENDEDHEADER))
    {
      size += details::instance()->extended_header_size() + sizeof(uint32);
    }
    
    return size;
  }
  
  bstring tag_header::extended_header() const
  {
    return bstring();
  }
  
  bstring tag_header::render() const
  {
    size_t size = 0;
    bstring data = tag_header::ID;
    
    data += details::instance()->major();
    data += details::instance()->minor();
    
    // render the flags
    data += static_cast<uchar>(this->flags() & MASK8);
    
    // render the size in uint28 format
    data += uint28(this->data_size()).render();
    
    // now we render the extended header
    if (this->test(EXTENDEDHEADER))
    {
      data += this->extended_header();
    }
    
    return data;
  }
  
  size_t tag_header::parse(const bstring& data)
  {
    
    if (!is_tag(data))
    {
      return 0;
    }
    
    size_t size = tag_header::SIZE;

    // major/minor versions are bits 4 & 5
    uchar major = data[MAJOR_OFFSET];
    uchar minor = data[MINOR_OFFSET];
    this->version(to_version(major, minor));
    
    // flags are bit 6
    this->flags(data[FLAGS_OFFSET]);
    
    // data size are bits 7, 8, 9, & 10
    this->data_size(size - SIZE);
    
    if (this->test(EXTENDEDHEADER))
    {
      cerr << "*** has an extended header" << endl;
      // the extended header size doesn't include the actual bytes
      // representing the size
      bstring extended = data.substr(tag_header::SIZE);
      size += to_integer(extended.data()) + sizeof(uint32);
      
      // okay, if we are 2.1, then let's skip over the extended header for now
      // because I am lazy
      if (this->version() == spec::VER_2_1)
      {
      }
      
      // okay, if we are 3.0, then let's actually parse the extended header
      // (for now, we skip it because we are lazy)
      if (this->version() >= spec::VER_3_0)
      {
      }
    }
    
    return size;
  }
}

  
// $Log$
