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

#include "frame_header.h"
#include "conversion_ops.h"
#include "details.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{

  frame_header::def find_frame_def(const frm::id id)
  {
    frame_header::def* fdi = 
      const_cast<frame_header::def *>(frame_header::defs);
    for (; fdi->id != frm::NOFRAME; ++fdi)
    {
      if (fdi->id == id)
      {
        break;
      }
    }
    return *fdi;
  }

  frm::id find_frame_id(const string& id)
  {
    frame_header::def* fdi = 
      const_cast<frame_header::def *>(frame_header::defs);
    
    for (; fdi->id != frm::NOFRAME; ++fdi)
    {
      if (fdi->short_id == id || fdi->long_id == id)
      {
        break;
      }
    }
    return fdi->id;
  }

  frame_header::frame_header(spec::version v)
    : header(v),
      __expanded_size(0),
      __grouping_id(uchar()),
      __encryption_id(uchar())
  {
    // this will initialize the __def member variable
    this->id(frm::NOFRAME);
  }

  // correct assignment operator
  frame_header& frame_header::operator=(const frame_header &rhs)
  {
    return this->assign(rhs);
  }

  frame_header& frame_header::assign(const frame_header &rhs)
  {
    if (this != &rhs)
    {
      this->header::operator=(rhs);
      __def = rhs.__def;
      this->expanded_size(rhs.expanded_size());
      this->grouping_id(rhs.grouping_id());
      this->encryption_id(rhs.encryption_id());
    }
    
    return *this;
  }

  size_t frame_header::parse(const bstring& data)
  {
    const details* vp = details::instance(this->version());
    size_t size = vp->frame_id_bytes();
    string text_id(to_string(data), 0, size);

    this->id(text_id);

    // Parse the size of the data that immediately follows the header
    size_t size_bytes = vp->frame_size_bytes();
    this->data_size(to_integer(&data.data()[size], size_bytes));
    size += size_bytes;

    // Parse the flags
    size_t fb = vp->frame_flags_bytes();
    this->add(static_cast<uint16>(to_integer(&data.data()[size], fb)));
    size += fb;

    // Parse the extras that can be appended to a header before the data
    if (this->test(COMPRESSION))
    {
      this->expanded_size(to_integer(&data.data()[size]));
      size += sizeof(uint32);
    }

    if (this->test(ENCRYPTION))
    {
      __encryption_id = data[size++];
    }

    if (this->test(GROUPING))
    {
      __grouping_id = data[size++];
    }
  
    return size;
  }

  bstring frame_header::render() const
  {
    // render most recent version
    const details* vp = details::instance(this->version());
    size_t bytesUsed = 0;
    bstring data = 
      to_bstring(this->text_id()) +
      to_bstring(this->data_size(), vp->frame_size_bytes()) +
      to_bstring(this->flags(), vp->frame_flags_bytes());

    // Now render the extras
    if (this->test(COMPRESSION))
    {
      data += to_bstring(this->expanded_size());
    }
    
    if (this->test(ENCRYPTION))
    {
      data += this->encryption_id();
    }
    
    if (this->test(GROUPING))
    {
      data += this->grouping_id();
    }
    
    return data;
  }

  size_t frame_header::size() const
  {
    const details* vp = details::instance(this->version());
    return
      vp->frame_id_bytes() + vp->frame_size_bytes() + vp->frame_flags_bytes();
  }

  size_t frame_header::expanded_size() const
  {
    return __expanded_size;
  }

  // the "extras" are rendered by the header, but are considered as part of the
  // data size.  so in order to determine which part of the data_size is the 
  // "extras", frame_header makes this method available
  size_t frame_header::extras_size() const
  {
    return 
      (this->test(COMPRESSION) ? sizeof(uint32) : 0) + 
      (this->test(ENCRYPTION)  ? sizeof(uchar)  : 0) +
      (this->test(GROUPING)    ? sizeof(uchar)  : 0);
  }
  
  frm::id frame_header::id() const
  {
    return __def.id;
  }

  const field::def* frame_header::fields() const
  {
    return __def.fields;
  }

  string frame_header::text_id() const
  {
    const details* vp = details::instance(this->version());
    string id;
    if(vp->frame_id_bytes() < 4)
    {
      id = __def.short_id;
    }
    else
    {
      id = __def.long_id;
    }
    id.resize(vp->frame_id_bytes());
    return id;
  }
  
  uchar frame_header::encryption_id() const
  {
    return __encryption_id;
  }
  
  uchar frame_header::grouping_id() const
  {
    return __grouping_id;
  }

  bool frame_header::expanded_size(size_t size)
  {
    bool changed = size != __expanded_size;
    if (changed)
    {
      __expanded_size = size;
    }
    return changed;
  }
  
  bool frame_header::id(frm::id id)
  {
    bool changed = id != this->id();
    if (changed)
    {
      __def = find_frame_def(id);
      this->set(frame_header::TAGALTER, __def.tag_discard);
      this->set(frame_header::FILEALTER, __def.file_discard);
    }
    return changed;
  }
  
  bool frame_header::id(const string& id)
  {
    bool changed = this->id(find_frame_id(id));

    if (this->id() == frm::NOFRAME)
    {
      __def.short_id = id;
      __def.short_id.resize(3);
      __def.long_id = id;
      __def.long_id.resize(4);
      changed = true;
    }
    return changed;
  }
  
  bool frame_header::encryption_id(uchar id)
  {
    bool changed = __encryption_id != id;
    if (changed)
    {
      __encryption_id = id;
    }
    return changed;
  }

  bool frame_header::grouping_id(uchar id)
  {
    bool changed = __grouping_id != id;
    if (changed)
    {
      __grouping_id = id;
    }
    return changed;
  }
}

// $Log$
