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

#ifndef ID3LIB_CONVERSION_OPS_H
#define ID3LIB_CONVERSION_OPS_H

#include "types.h"

namespace id3
{
  uint32  to_integer(const uchar*, size_t = sizeof(uint32));
  uint32  to_integer(const bstring& buffer, size_t size = sizeof(uint32));

  bstring to_bstring(uint32, size_t = sizeof(uint32));
  bstring to_bstring(const string&);
  bstring to_bstring(const wstring&);

  string  to_string(const wstring&);
  string  to_string(const bstring&);

  wchar_t to_wchar(const uchar*);

  const wchar_t BOM = static_cast<wchar_t>(0xFFFE);
  const wchar_t MOB = static_cast<wchar_t>(0xFEFF);

  wstring to_wstring(const string&);
  wstring to_wstring(const bstring&);
}

#endif

// $Log$
