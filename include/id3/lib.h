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

#ifndef ID3LIB_LIB_H
#define ID3LIB_LIB_H

#include "types.h"

namespace id3
{
/** id3lib defines a lib class within the id3 namespace with static data
 ** members for making known the library information.
 **
 ** The members are used to identify the version and date of the id3lib that is
 ** currently being used.
 **/
  class lib
  {
  public:
    static const string NAME;      ///< i.e., "id3lib"
    static const string RELEASE;   ///< i.e., "3.0.6"
    static const string FULLNAME;  ///< i.e., "id3lib-3.0.6"
    static const string DATE;      ///< i.e., "2000/01/04 16:38:15"
    static const size_t MAJOR;     ///< i.e., 3
    static const size_t MINOR;     ///< i.e., 0
    static const size_t PATCH;     ///< i.e., 6
  private:
    lib();
    lib(const lib&);
  };
}

#endif

// $Log$
