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

#ifndef ID3LIB_DETAILS_H
#define ID3LIB_DETAILS_H

#include "enums.h"
#include "types.h"

namespace id3
{
  spec::version to_version(int, int);

  class details
  {
  public:
    bool    extended_header() const { return __extended_header; }
    bool    experimental() const { return __experimental; }
    size_t  extended_header_size() const { return __extended_header_size; }
    size_t  frame_id_bytes() const { return __frame_id_bytes; }
    size_t  frame_size_bytes() const { return __frame_size_bytes; }
    size_t  frame_flags_bytes() const { return __frame_flags_bytes; }
    uchar   major() const { return __major; }
    uchar   minor() const { return __minor; }
    
    static const details* instance(spec::version v = spec::CURRENT)
    { 
      if (spec::NUMVERSIONS > v && v >= spec::EARLIEST) 
      { 
        return &__versions[v];
      }
      return &__versions[spec::CURRENT];
    }

  private:
    uchar    __major;
    uchar    __minor;
    bool     __extended_header;
    bool     __experimental;
    size_t   __extended_header_size;
    size_t   __frame_id_bytes;
    size_t   __frame_size_bytes;
    size_t   __frame_flags_bytes;

    // Private constructor to prevent instantiation
    details();
    details(uchar M, uchar m, bool eh, bool exp, 
            size_t ehs, size_t fid, size_t fsb, size_t ffb)
      : __major(M),
        __minor(m),
        __extended_header(eh),
        __experimental(exp),
        __extended_header_size(ehs),
        __frame_id_bytes(fid),
        __frame_size_bytes(fsb),
        __frame_flags_bytes(ffb)
    { ; }

    static const details __versions[spec::NUMVERSIONS];
  };

};

#endif

// $Log$
