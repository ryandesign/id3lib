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

#if !defined ID3LIB_FRAME_H
#define ID3LIB_FRAME_H

#include "frame_header.h"
#include <bitset>
#include <vector>

namespace id3
{
  // forward decarations
  class  field;
  
  /** The representative class of an id3v2 frame.
   ** 
   ** id3lib defines frames in a funny way.  Using some nice c++ conventions,
   ** frame class objects appear to be quite polymorphic; that is, they can
   ** take on many forms.  The same frame class provides the facilities for
   ** the implementation of a complex APIC frame and for a simple text frame.
   ** 
   ** @author Dirk Mahoney
   ** @version $Id$
   ** @see ID3_Tag
   ** @see ID3_Field
   ** @see ID3_Err
   **/
  class frame
  {
  public:
    typedef vector<field *> fields;
    typedef fields::iterator iterator;
    typedef fields::const_iterator const_iterator;

    // constructors/destructors
    frame(frm::id, spec::version = spec::CURRENT);
    frame(spec::version = spec::CURRENT);
    frame(const frame&);
    ~frame();
  
    // assignment operator
    frame&       operator=(const frame&);
  
    // parsing/redering operations
    bstring      render() const;
    size_t       parse(const bstring&);
  
    // const accessor methods
    frm::id       id() const;
    size_t        size() const;
    bool          contains(fld::id) const;
    field&        get(fld::id) const;
    spec::version version() const;
    frame_header  hdr() const;
    bool          read_only() const;
    bool          compression() const;
    bool          changed() const;
    bool          bad_parse() const;
  
    // modification methods
    void         clear();
    bool         encoding(fld::encoding);
    bool         id(frm::id);
    bool         version(spec::version);
    bool         read_only(bool);
    bool         compression(bool);

    // iterator methods
    fields::iterator       begin();
    fields::const_iterator begin() const;
    fields::iterator       end();
    fields::const_iterator end() const;

  protected:
    void         _make_fields();
    void         _add(field*);
    void         _clear_fields();
    void         _id(frm::id);
  
  private:
    frame_header  __hdr;         // the frame header
    fields        __fields;      // collection of field pointers
    bool          __bad_parse;   // did we suffer a bad parse?
    bool          __changed;     // changed since last parse/render?
    bitset<fld::NUMFIELDS> __bits;
  };
};

#endif

// $Log$
// Revision 1.6  1999/12/27 06:14:22  scott
// (class ID3_Frame): Added declaration for constructor which accepts a
// frame header to copy.  Removed redundant data members that are now
// encapsulated within new frame header member.  Added declaration for
// new methods InitFields and InitFieldBits.
//
// Revision 1.5  1999/12/26 16:40:08  scott
// (class ID3_Frame): Added bReadOnly property.
//
// Revision 1.4  1999/12/17 16:05:02  scott
// Updated opening comment block.
//
// Revision 1.3  1999/12/09 02:45:46  scott
// (class ID3_Frame): Added operator= method declaration.
//
// Revision 1.2  1999/12/02 22:45:28  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places.
//
// Revision 1.1  1999/12/01 17:16:02  scott
// moved from src/id3 to include/id3
//
// Revision 1.9  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.8  1999/11/29 19:06:50  scott
// Updated documentation to work better with doc++ (probably doesn't work
// well with kdoc anymore).
//
// Revision 1.7  1999/11/25 19:15:15  scott
// Added doc++/kdoc/javadoc-like documentation, mostly transcribed
// from Dirk's ID3Lib Documentation MSWord doc.
//
// Revision 1.6  1999/11/15 20:18:11  scott
// Made ID3_Tag a friend of ID3_Frame to allow access to the private
// members in ID3_Frame.  Made variable names more descriptive.
// Added const qualifier to appropriate methods.
//
// Revision 1.5  1999/11/04 04:32:11  scott
// Initial revision
//
// Revision 1.4  1999/11/04 04:15:54  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
