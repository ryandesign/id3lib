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

#include "header.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{
  header::header(spec::version v)
    : __data_size(0),
      __flags(0),
      __changed(false),
      __version(v)
  {
  }

  header &header::operator=( const header &hdr )
  {
    if (this != &hdr)
    {
      this->version(hdr.version());
      this->data_size(hdr.data_size());
      this->flags(hdr.flags());
    }
    return *this;
  }

  // Candidate to be made into a helper function (unless SetFlags isn't public)
  void header::clear()
  {
    this->version(spec::CURRENT);
    this->data_size(0);
    this->flags(0);
  }

  bool header::data_size(size_t size)
  {
    bool changed = __data_size != size;
    if (changed)
    {
      __data_size = size;
      __changed = true;
    }
    return changed;
  }

  bool header::version(spec::version version)
  {
    bool changed = version != __version;
    if (changed)
    {
      __version = version;
      __changed = true;
    }
    return changed;
  }

  bool header::flags(uint16 flags)
  {
    bool changed = __flags != flags;
    if (changed)
    {
      __flags = flags;
      __changed = true;
    }
    return changed;
  }
  
}

// $Log$
// Revision 1.11  2000/01/04 15:42:49  eldamitri
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.10  1999/12/27 05:43:28  scott
// (header): Updated for new version constants.
// (AddFlags): Added implementation.  Adds flag(s) to current flags.
// (RemoveFlags): Added implementation.  Removes flag(s) from current
// flags.
// (GetVersion): Added implementation.  Returns version number of header.
// (GetRevision): Added implementation.  Returns revision of header.
// (Clear): Added implementation.  Clears contents of header.
// (operator=): Added implementation. Assignment operator.
// (Copy): Added implementation. Copies content of another header.
//
// Revision 1.9  1999/12/26 15:11:13  scott
// (GetDataSize): Added implementation.
// (GetFlags): Added implementation.
//
// Revision 1.8  1999/12/17 16:13:04  scott
// Updated opening comment block.
//
// Revision 1.7  1999/12/01 18:00:59  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places (and to make compiling under
// windows easier).
//
// Revision 1.6  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.5  1999/11/15 20:18:46  scott
// Added include for config.h.  Minor code cleanup.  Made private
// member variable names more descriptive.  Fixed bug I introduced in
// logic of checking version/revision numbers in
// ID3_LookupHeaderInfo, and cleaned up the code in the process.
//
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
