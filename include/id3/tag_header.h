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

#ifndef ID3LIB_TAG_HEADER_H
#define ID3LIB_TAG_HEADER_H

#include "header.h"

namespace id3
{
  class tag_header : public header
  {
  public:
    static const uint8 UNSYNC         = 1 << 7;
    static const uint8 EXTENDEDHEADER = 1 << 6;
    static const uint8 EXPERIMENTAL   = 1 << 5;
    
    // id3v2 tag header signature:  $49 44 33 MM mm GG ss ss ss ss
    // MM = major version (will never be 0xFF)
    // mm = minor version (will never be 0xFF)
    // ff = flags byte 
    // ss = size bytes (less than $80)
    static const bstring ID;
    static const size_t ID_SIZE;
    static const size_t MAJOR_OFFSET;
    static const size_t MINOR_OFFSET;
    static const size_t FLAGS_OFFSET;
    static const size_t SIZE_OFFSET;
    static const size_t SIZE;
  
  public:
    size_t  size() const;
    bstring render() const;
    size_t  parse(const bstring&);
    bool    version(spec::version);
    bstring extended_header() const;
    spec::version version() const;
  };
  
  size_t is_tag(const bstring&);
}

#endif

// $Log$
