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

#include "details.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3 
{

  spec::version to_version(int major, int minor)
  {
    if (3 == major && 0 == minor)
    {
      return spec::VER_3_0;
    }
    if (2 == major && 0 == minor)
    {
      return spec::VER_2_1;
    }
    if (2 == major && 1 == minor)
    {
      return spec::VER_2_0;
    }
    return spec::NOVERSION;
  }

  // 1: major version
  // 2: minor version
  // 3: must have extended header
  // 4: experimental version
  // 5: maximum size for extended header
  // 6: # bytes used for frame id
  // 7: # bytes used for frame size
  // 8: # bytes used for frame flags
  const details details::__versions[spec::NUMVERSIONS] = 
  { 
    details(2, 0, false, false,  0, 3, 3, 0), // version 2.2.0
    details(2, 1,  true,  true,  8, 3, 3, 0), // version 2.2.1
    details(3, 0, false, false, 10, 4, 4, 2)  // version 2.3.0
  };
}

// $Log$
