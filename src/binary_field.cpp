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

#include <fstream>
#include "field.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{
  binary_field::binary_field(const field::def& def, spec::version v)
    : field(def, v)
  {
  }
  
  bool binary_field::_assign(const bstring& data)
  {
    if (this->binary() == data)
    {
      return false;
    }
    
    __data = data;
    return true;
  }
  
  bstring binary_field::binary() const
  {
    return __data;
  }
  
  binary_field& binary_field::operator=(const binary_field& f)
  {
    if (this != &f)
    {
      this->field::operator=(f);
      this->_assign(f.binary());
    }
    return *this;
  }
  
  field* binary_field::clone() const
  {
    return new binary_field(*this);
  }
  
  void binary_field::clear()
  {
    __data = bstring();
  }

  size_t binary_field::_size() const
  {
    return __data.size();
  }

  size_t binary_field::_parse(const bstring& data)
  {
    this->_assign(data);
    return data.size();
  }

  bstring binary_field::_render() const
  {
    return this->binary();
  }

  void from_file(binary_field& fld, const string& info)
  {
    ifstream file(info.c_str(), std::ios::binary);
    if (file)
    {
      bstring data;
      file >> data;
      /*
      uchar buffer[BUFSIZ];
      while (file.read(buffer, BUFSIZ))
      {
        data.append(buffer, file.gcount());
      }
      */
      fld.assign(data);
    }
  }
  
  void to_file(binary_field& fld, const string& info)
  {
    ofstream file(info.c_str(), std::ios::binary | std::ios::out);
    file << fld;
  }
}

// $Log$
