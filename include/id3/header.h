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

#ifndef ID3LIB_HEADER_H
#define ID3LIB_HEADER_H

#include "enums.h"
#include "types.h"

namespace id3
{
  class header
  {
  public:
    header(spec::version = spec::CURRENT);
    
    header &operator=(const header&);
    
    virtual bstring render() const = 0;
    virtual size_t  parse(const bstring&) = 0;
    
    virtual size_t  size() const = 0;
    /*   */ size_t  data_size() const
    { return __data_size; }
    /*   */ bool    changed() const
    { return __changed; }
    virtual spec::version version() const
    { return __version; }
    /*   */ uint16  flags() const
    { return __flags; }
    /*   */ bool    test(uint16 flag) const
    { return (this->flags() & flag) == flag; }
    
    virtual void    clear();
    /*   */ bool    data_size(size_t);
    virtual bool    version(spec::version);
    /*   */ bool    flags(uint16);
    /*   */ bool    add(uint16 f)
    { return this->flags(this->flags() | f); }
    /*   */ bool    remove(uint16 f)
    { return this->flags(this->flags() & ~f); }
    /*   */ bool    set(uint16 f, bool b)
    { if (b) { return this->add(f); } return this->remove(f); }

  protected:
    spec::version   __version;     // version information
    size_t          __data_size;    // the size of the data in bytes
    uint16          __flags;        // header flags
    bool            __changed;      // if the header has changed since it
                                    // was constructed or rendered
  };
}
#endif

// $Log$
// Revision 1.5  1999/12/27 06:11:33  scott
// (ID3_VERSION, ID3_REVISION): Renamed to ID3v2_*
// (class ID3_Header): Added declarations for GetVersion, GetRevision,
// AddFlags, SetFlags, Clear, Copy, and operator= methods.  Changed
// return type for Size and Render from luint to size_t.
//
// Revision 1.4  1999/12/26 16:40:13  scott
// (class ID3_Header): Minor cleanup to interface.
//
// Revision 1.3  1999/12/17 16:05:02  scott
// Updated opening comment block.
//
// Revision 1.2  1999/12/02 22:45:28  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places.
//
// Revision 1.1  1999/12/01 17:16:04  scott
// moved from src/id3 to include/id3
//
// Revision 1.6  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.5  1999/11/15 20:18:59  scott
// Made variable names more descriptive.
//
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
