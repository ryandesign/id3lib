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

#ifndef ID3LIB_DEBUG_H
#define ID3LIB_DEBUG_H

#include "tag.h"

namespace id3
{
  string to_string(fld::id);
  string to_string(fld::type type);
  string to_string(fld::encoding e);
  string to_string(frm::id id);
  string to_string(spec::version v);
  string to_string(const field&);
  string to_string(const frame&);
  string to_string(const tag&);

  void populate(frame&);
};

#endif /* ID3LIB_DEBUG_H */

// $Log$
