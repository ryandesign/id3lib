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

#ifndef ID3LIB_TAG_FIND_H
#define ID3LIB_TAG_FIND_H

#include <functional>
#include "frame.h"

namespace id3
{
  /** Class used for finding a particular frame in a tag.
   **
   ** \code
   **   id3::has_frame_id pred(id3::frm::TITLE);
   **   id3::tag::iterator ti = std::find_if(tag.begin(), tag.end(), pred);
   **   if (ti != tag.end())
   **   {
   **     // do something with the frame, like copy
   **     // the contents into a buffer, display the
   **     // contents in a window, etc.
   **   }
   ** \endcode
   ** 
   ** If there happens to more than one such frame, use the next iterator as
   ** the starting point of the call to \c std::find_if.  The following code
   ** will find all frames with the above property.
   **
   ** \code
   **   id3::has_frame_id pred(id3::frm::TITLE);
   **   for (id3::tag::iterator ti = tag.begin();
   **        (ti = std::find_if(ti, tag.end(), pred)) != tag.end(); ++ti)
   **   {
   **     // print out the content of the frame
   **     std::cout << "Title: " << ti->get(id3::fld::TEXT).ascii()
   **               << std::endl;
   **   }
   ** \endcode
   **/
  class has_frame_id : public std::unary_function<frame, bool>
  {
    frm::id __id;
  public:
    has_frame_id(frm::id id) : __id(id) { ; }
    int operator() (const frame& f) { return f.id() == __id; }
  };
    
  /** Class used for finding a tag for a frame with a specific integer field.
   **
   ** The following code returns the first COMMENT frame that uses ascii as
   ** its text encoding.
   **
   ** \code
   **   id3::has_field_integer hfi(id3::frm::COMMENT, id3::fld::TEXTENC, id3::fld::ASCII);
   **   id3::tag::iterator ti = std::find(tag.begin(), tag.end(), hfi);
   **   if (ti != tag.end())
   **   {
   **     // found it, do something with it
   **   }
   ** \endcode
   **
   **/
  class has_field_integer : public std::unary_function<frame, bool>
  {
    frm::id __frm_id;
    fld::id __fld_id;
    uint32  __num;
  public:
    has_field_integer(frm::id frm, fld::id fld, uint32 num)
      : __frm_id(frm), __fld_id(fld), __num(num) { ; }
    int operator() (const frame& f) const
    {
      return 
        f.id() == __frm_id           && 
        f.contains(__fld_id)         && 
        f.get(__fld_id).is_integer() && 
        f.get(__fld_id).integer() == __num;
    }
  };

  /** Class used for finding a a tag for a frame with a specific ascii field.
   **
   ** The following example will return the first TITLE frame whose TEXT
   ** field is 'Nirvana'.
   **
   ** \code
   **   id3::has_field_ascii pred(id3::frm::TITLE, id3::fld::TEXT, "Nirvana");
   **   id3::tag::iterator ti  = std::find(tag.begin(), tag.end(), pred);
   **   if (ti != tag.end())
   **   {
   **     // found it, do something with it
   **   }
   ** \endcode
   **/
  class has_field_ascii : public std::unary_function<frame, bool>
  {
    frm::id __frm_id;
    fld::id __fld_id;
    string  __str;
  public:
    has_field_ascii(frm::id frm, fld::id fld, string s)
      : __frm_id(frm), __fld_id(fld), __str(s) { ; }
    int operator() (const frame& f) const
    {
      return 
        f.id() == __frm_id        && 
        f.contains(__fld_id)      &&
        f.get(__fld_id).is_text() && 
        f.get(__fld_id).ascii() == __str;
    }
  };
  
  /** Class used for finding a a tag for a frame with a specific unicode
   ** field.
   **/
  class has_field_unicode : public std::unary_function<frame, bool>
  {
    frm::id __frm_id;
    fld::id __fld_id;
    wstring __str;
  public:
    has_field_unicode(frm::id frm, fld::id fld, wstring ws)
      : __frm_id(frm), __fld_id(fld), __str(ws) { ; }
    int operator() (const frame& f) const
    {
      return 
        f.id() == __frm_id        && 
        f.contains(__fld_id)      &&
        f.get(__fld_id).is_text() && 
        f.get(__fld_id).unicode() == __str;
    }
  };
}

#endif /* !defined ID3LIB_TAG_FIND_H */
