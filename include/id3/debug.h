// $Id$

// id3lib: a software library for creating and manipulating id3v1/v2 tags
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

#ifndef _ID3LIB_DEBUG_H_
#define _ID3LIB_DEBUG_H_

#if defined (ID3_ENABLE_DEBUG) && defined (HAVE_LIBCW_SYS_H)

#define DEBUG

#include <libcw/sys.h>
#include <libcw/debug.h>

#define ID3D_INIT_DOUT()    Debug( libcw_do.on() )
#define ID3D_INIT_WARNING() Debug( dc::warning.on() )
#define ID3D_INIT_NOTICE()  Debug( dc::notice.on() )
#define ID3D_NOTICE(x)      Dout( dc::notice, x )
#define ID3D_WARNING(x)     Dout( dc::warning, x )

#else

#error "shouldn't be here"

#  define ID3D_INIT_DOUT()
#  define ID3D_INIT_WARNING()
#  define ID3D_INIT_NOTICE()
#  define ID3D_NOTICE(x)
#  define ID3D_WARNING(x)

#endif /* defined (ID3_ENABLE_DEBUG) && defined (HAVE_LIBCW_SYS_H) */

#endif /* _ID3LIB_DEBUG_H_ */
