// $Id$

// id3lib: a C++ library for creating and manipulating id3v1/v2 tags
// Copyright 1999, 2000  Scott Thomas Haug

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

#include "version.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

const char *ID3_Name()
{
  return PACKAGE;
}

const char *ID3_Release()
{
  return VERSION;
}

const char *ID3_FullName()
{
  return ID3LIB_FULLNAME;
}

unsigned short ID3_Version()
{
  return ID3LIB_VERSION;
}

unsigned short ID3_Revision()
{
  return ID3LIB_REVISION;
}

unsigned short ID3_Patch()
{
  return ID3LIB_PATCH;
}

// $Log$
// Revision 1.3  2000/01/04 15:42:49  eldamitri
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.2  1999/12/17 16:13:04  scott
// Updated opening comment block.
//
// Revision 1.1  1999/11/30 20:19:26  scott
// Added.  Implements version functions declared in version.h.
//
