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

#ifndef ID3LIB_FRAME_HEADER_H
#define ID3LIB_FRAME_HEADER_H

#include "header.h"
#include "field.h"

namespace id3
{
  class frame_header : public header
  {
  public:
    // Structure used for defining how frames are defined internally.
    struct def
    {
      frm::id      id;
      string       short_id;
      string       long_id;
      bool         tag_discard;
      bool         file_discard;
      const field::def*  fields;
    };

    static const def defs[];

  public:
    frame_header(spec::version = spec::CURRENT);
  
    frame_header&     operator=(const frame_header&);
    frame_header&     assign(const frame_header&);

    size_t            parse(const bstring&);
    bstring           render() const;

    size_t            size() const;
    size_t            expanded_size() const;
    size_t            extras_size() const;
    frm::id           id() const;
    const field::def* fields() const;
    string            text_id() const;
    uchar             encryption_id() const;
    uchar             grouping_id() const;

    bool              expanded_size(size_t size);
    bool              id(frm::id id);
    bool              id(const string&);
    bool              encryption_id(uchar id);
    bool              grouping_id(uchar id);
 
  protected:
    size_t            __expanded_size;
    uchar             __grouping_id;
    uchar             __encryption_id;
    def               __def;

  public:
    static const uint16 TAGALTER    = 1 << 15;
    static const uint16 FILEALTER   = 1 << 14;
    static const uint16 READONLY    = 1 << 13;
    static const uint16 COMPRESSION = 1 <<  7;
    static const uint16 ENCRYPTION  = 1 <<  6;
    static const uint16 GROUPING    = 1 <<  5;
  };

  //frame_header::def   find_frame_def(const frm::id id);
  //frm::id             find_frame_id(const string& id);
};

#endif

// $Log$
