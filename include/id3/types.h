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

#ifndef ID3LIB_TYPES_H
#define ID3LIB_TYPES_H

#include "sized_types.h"
#include <string>

#if defined major
#undef major
#endif

#if defined minor
#undef minor
#endif

namespace id3
{
  /// Shorthand type definition for unsigned char
  typedef       unsigned char   uchar;

  /// typedefs for different types of strings
  typedef std::basic_string<char>     string;
  typedef std::basic_string<wchar_t> wstring;
  typedef std::basic_string<uchar>   bstring;

  /// Used for masking the n least significant bits
#define MASK(bits) ((1 << (bits)) - 1)
#define MASK1 MASK(1)
#define MASK2 MASK(2)
#define MASK3 MASK(3)
#define MASK4 MASK(4)
#define MASK5 MASK(5)
#define MASK6 MASK(6)
#define MASK7 MASK(7)
#define MASK8 MASK(8)
};
  
#endif

// $Log$
// Revision 1.4  2000/01/04 15:42:22  eldamitri
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.3  1999/12/17 16:05:02  scott
// Updated opening comment block.
//
// Revision 1.2  1999/12/02 22:45:28  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places.
//
// Revision 1.1  1999/12/01 17:16:11  scott
// moved from src/id3 to include/id3
//
// Revision 1.7  1999/11/30 20:14:22  scott
// Removed include for wchar.h
//
// Revision 1.6  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.5  1999/11/25 19:24:24  scott
// Removed typedefs, which are now included in the globas.h file.
//
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
