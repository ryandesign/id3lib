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

#include "lib.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{
  const string lib::NAME     = ID3LIB_NAME;
  const size_t lib::MAJOR    = ID3LIB_MAJOR;
  const size_t lib::MINOR    = ID3LIB_MINOR;
  const size_t lib::PATCH    = ID3LIB_PATCH;
  const string lib::RELEASE  = ID3LIB_RELEASE;
  const string lib::FULLNAME = ID3LIB_FULLNAME;
  const string lib::DATE     = string(ID3LIB_DATE).substr(2);
}

// $Log$
