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

#include "field.h"
#include "conversion_ops.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{

  integer_field::integer_field(const field::def& def, spec::version v)
    : field(def, v)
  {
  }
  
  bool integer_field::_assign(const uint32 data)
  {
    if (data == this->integer())
    {
      return false;
    }
    
    __data = data;
    return true;
  }
  
  uint32 integer_field::integer(void) const
  {
    return __data;
  }
  
  integer_field& integer_field::operator=(const integer_field& f)
  {
    if (this != &f)
    {
      this->field::operator=(f);
      this->_assign(f.integer());
    }
    return *this;
  }

  field* integer_field::clone() const
  {
    return new integer_field(*this);
  }

  void integer_field::clear()
  {
    __data = uint32();
  }

  size_t integer_field::_size() const
  {
    return sizeof(__data);
  }

  size_t integer_field::_parse(const bstring& data)
  {
    this->assign(to_integer(data));
    return std::min(sizeof(__data), data.size());
  }

  bstring integer_field::_render() const
  {
    return to_bstring(this->integer());
  }

}

// $Log$
