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

#include "field.h"
#include "conversion_ops.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{
  field* field::make(const field::def& def)
  {
    switch (def.type)
    {
      case fld::BINARY:
        return new binary_field(def);
      case fld::INTEGER:
        return new integer_field(def);
      case fld::TEXTSTRING:
        return new text_field(def);
    }
    
    return NULL;
  }

  /** Renders the contents of the field to a binary string
   **
   ** @return A binary string representing the field
   **/
  bstring field::render() const
  {
    __changed = false;
    // make sure this field in the current spec
    if (!this->in_scope(spec::CURRENT))
    {
      return bstring();
    }
    
    // call the protected render method defined in subclasses
    bstring data = this->_render();
    // if the field is of fixed length, resize the rendered data appropriately.
    // this way, subclasses need not concern themselves with making sure 
    // they're within the confines of a fixed length field
    if (__def.fixed_size)
    {
      data.resize(__def.fixed_size);
    }
    return data;
  }
  
  /** Initializes the field based on the contents of \c data
   **
   ** @param data The binary string to parse
   ** @return The number of characters parsed from data.
   **/
  size_t field::parse(const bstring& data)
  {
    __changed = false;
    // make sure this field is in scope
    if (!this->in_scope(this->version()))
    {
      return 0;
    }
    // make sure we're not parsing more characters than this field can handle.
    // this way, subclasses need not concern themselves with making sure 
    // they're within the confines of a fixed length field
    bstring new_data = data;
    if (__def.fixed_size)
    {
      new_data.resize(__def.fixed_size);
    }
    return this->_parse(new_data);
  }
  
  bool field::version(spec::version v)
  {
    bool changed = (v != __version);
    if (changed)
    {
      __version = v;
      __changed = true;
    }
    return changed;
  }
  
  bool field::encoding(fld::encoding e)
  {
    bool changed = this->is_encodable() && (e != this->encoding()) &&
      (e > fld::NOENCODING && e < fld::NUMENCODINGS);
    if (changed)
    {
      this->_encoding(e);
      __changed = true;
    }
    return changed;
  }
      
  fld::type field::type() const
  {
    return __def.type;
  }
  
  fld::id field::id() const
  {
    return __def.id;
  }
  
  /** Returns the size (in bytes) of the rendered field.
   **
   ** \code
   **   size_t size = my_frame.get(id3::fld::DATA).size();
   ** \endcode
   ** 
   ** This does not return the number of characters in a represented string.
   ** To determine the number of characters of the represented string, call
   ** the size method on the result of calling ascii(int) or unicode(int)
   **
   ** @return The size of the field, in bytes
   **/
  size_t field::size() const
  {
    // make sure we're in scope
    if (!this->in_scope())
    {
      return 0;
    }
    // make sure we don't have a fixed length
    if (__def.fixed_size)
    {
      return __def.fixed_size;
    }
    // call the protected size method implemented in subclasses
    return this->_size();
  }
  
  spec::version field::version() const
  {
    return __version;
  }

  fld::encoding field::encoding() const
  {
    return fld::NOENCODING;
  }

  bool field::changed() const
  {
    return __changed;
  }
  
  /** Whether or not this field will be rendered or parsed, given the current
   ** version of the field.
   ** @return true if the field is in scope, false otherwise.
   **/
  bool field::in_scope(spec::version v) const
  {
    return 
      (fld::HIGHER == __def.scope && v >= __def.version) ||
      (fld::LOWER  == __def.scope && v <= __def.version);
  }
  
  bool field::is_integer() const
  {
    return false;
  }
  
  bool field::is_binary() const
  {
    return false;
  }
  
  bool field::is_text() const
  {
    return false;
  }
  
  bool field::is_list() const
  {
    return __def.flags & fld::LIST;
  }
  
  bool field::is_cstr() const
  {
    return __def.flags & fld::CSTR;
  }
  
  bool field::is_encodable() const
  {
    return __def.flags & fld::ENCODABLE;
  }

  size_t field::fixed_size() const
  {
    return __def.fixed_size;
  }

  /** A shortcut for the assign method.
   ** 
   ** \code
   **   my_frame.get(id3::fld::PICTURETYPE) = 0x0B;
   ** \endcode
   ** 
   ** @param data The data to assign this field to.
   ** @see assign
   **/
  field& field::operator=(const uint32 data)
  {
    this->assign(data);
    return *this;
  }
  
  /** Assigns the value of the field to the specified integer.
   ** 
   ** @param data The data to assign this field to.
   **/
  void field::assign(const uint32 data) 
  {
    __changed = this->_assign(data) || __changed;
  }
  
  bool field::_assign(const uint32)
  {
    return false;
  }
  
  /** Returns the value of the integer field.
   ** 
   ** \code
   **   uint32 pic = my_frame.get(id3::fld::PICTURETYPE).integer();
   ** \endcode
   ** 
   ** @return The value of the integer field
   **/
  uint32 field::integer() const 
  { 
    return uint32(); 
  }
  
  /** Returns the number of items in a text list.
   **      
   ** \code
   **   size_t num_items = my_frame.get(id3::fld::TEXT).num_items();
   ** \endcode
   ** 
   ** @return The number of items in a text list.
   **/
  size_t field::num_items() const
  {
    return 1;
  }
  
  /** Shortcut for the assign operator.
   ** 
   ** @param data The string to assign this field to
   ** @see field::assign(const string&)
   **/
  field& field::operator=(const string& data)
  {
    this->assign(data);
    return *this;
  }
  
  /** Copies the supplied string to the field.
   **   
   ** \code
   **   my_frame.get(id3::fld::TEXT).assign("ID3Lib is very cool!");
   ** \endcode
   **/
  void field::assign(const string& data)
  {
    if (this->is_text())
    {
      //__frame.encoding(field::ASCII);
      __changed = this->_assign(to_wstring(data)) || __changed;
    }
  }
  
  /** Returns the contents of the field as an ascii string.  If the field isn't
   ** a string field, the empty string is returned.
   **
   ** \code
   **   id3::string data = my_frame.get(id3::fld::TEXT).ascii();
   ** \endcode
   ** 
   ** The optional parameter is used for accessing the elements of a text list.
   ** If no parameter is given, the entire text is returned.  If a nonnegative
   ** integer is given as a parameter, the specific string at the given
   ** (0-based) index is returned.  See field::add for more details.  If the
   ** index given doesn't pertain to any element in the list, the empty string
   ** is returned.
   **
   ** \code
   **   id3::string data = my_frame.get(id3::fld::TEXT).ascii(2);
   ** \endcode
   **
   ** The above example returns the third string element of the text list.
   **
   ** @param item For fields with multiple entries, the item number to retrieve
   ** @see field::add(int)
   **/
  string field::ascii(int item) const
  {
    return to_string(this->unicode(item));
  }
  
  /** For fields which support this feature, adds a string to the list of
   ** strings currently in the field.
   ** 
   ** This is useful with frames such as the involved people list, composer,
   ** and part-of-set.  You can use the num_items() method to find out how many
   ** such items are in a list.
   **
   ** \code
   **   my_frame.get(id3::fld::TEXT).add("this is a test");
   ** \endcode
   **
   ** @param data The string to add to this field 
   **/
  void field::add(const string& data)
  {
    if (this->is_text())
    {
      __changed = true;
      //__frame.encoding(field::ASCII);
      this->_add(to_wstring(data));
    }
  }
  
  /** Shortcut for the assign operator.
   **  
   ** Peforms similarly as the ASCII assignment operator, taking a unicode
   ** string as a parameter rather than an ascii string.
   **
   ** @see add(const string&)
   **/
  field& field::operator= (const wstring& data)
  { 
    this->assign(data);
    return *this;
  }
  
  /** Copies the supplied unicode string to the field.
   **
   ** Peforms similarly as the ASCII assign(const string&) method,
   ** taking a unicode string as a parameter rather than an ascii string.
   ** 
   ** @param data The unicode string to assign this field to.
   ** @see add(const wstring&)
   **/
  void field::assign(const wstring& data)
  {
    if (this->is_text())
    {
      //__frame.encoding(field::UNICODE);
      __changed = this->_assign(data) || __changed;
    }
  }
  
  bool field::_assign(const wstring&)
  {
    return false; // do nothing by default
  }

  /** Returns the field's contents as a wstring. For fields with multiple text 
   ** entries, the optional third parameter indicates which of the entries to
   ** return.
   ** 
   ** Peforms similarly as the ascii method, returning a unicode string rather
   ** than an ascii string.
   ** 
   ** \code
   **   id3::wstring data = my_frame.get(id3::fld::TEXT).unicode();
   ** \endcode
   **
   ** @param item For fields with multiple entries, the item number to retrieve
   ** @see ascii(int)
   **/
  wstring field::unicode(int item) const
  {
    return wstring();
  }

  /** For fields which support this feature, adds a string to the list of
   ** strings currently in the field.
   ** 
   ** Peforms similarly as the ASCII add(int) method, taking a unicode string
   ** as a parameter rather than an ascii string.
   **/
  void field::add(const wstring& data)
  {
    if (this->is_text())
    {
      __changed = true;
      //__frame.encoding(field::UNICODE);
      this->_add(data);
    }
  }
  
  void field::_add(const wstring&)
  {
    ; // do nothing by default
  }
  
  field& field::operator=(const bstring& data)
  {
    this->assign(data);
    return *this;
  }
  
  /** Copies the supplied binary string to the field.
   ** 
   ** @param data The data to assign to this field.
   **/
  void field::assign(const bstring& data)
  {
    __changed = this->_assign(data) || __changed;
  }
  
  bool field::_assign(const bstring&)
  {
    return false;
  }

  /** Copies the field's internal string to the buffer.
   **
   ** It copies the data in the field into the buffer, for as many bytes as the
   ** field contains, or the size of buffer, whichever is smaller.
   **
   ** \code
   **   id3::bstring data = my_frame.get(id3::fld::DATA).binary();
   ** \endcode
   **
   ** @return A binary string
   **/
  bstring field::binary() const
  {
    return bstring();
  }
  
  ostream& operator<<(ostream& os, const field& fld)
  {
    os << fld.render();
    return os;
  }
  
}

// $Log$
// Revision 1.15  2000/01/04 15:42:49  eldamitri
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.14  1999/12/26 00:33:15  scott
// (): Updated list of implemented and unimplemented frames to include
// their id3lib frame id, id3v2.2 id, and www.id3.org description.
// (ID3FD_Unimplemented): Added for unimplemented frames so that their
// data is preseved when parsing and rendering.
// (ID3_FrameDefs): Added entries for new frames, using
// ID3FD_Unimplemented as frame definition.  Updated renamed entries.
//
// Revision 1.13  1999/12/17 16:13:03  scott
// Updated opening comment block.
//
// Revision 1.12  1999/12/13 05:24:00  scott
// (ID3FD_TermsOfUse): Added for defining USER frame.
// (ID3FD_LinkedInfo): Added for defining LINK frame.
// (): Added comments that detail which frames are defined and which
// aren't.
// (ID3_FrameDefs): Added definitions for the following new frames:
// ID3FID_LINKEDINFO, ID3FID_BEATSPERMINUTE, ID3FID_PLAYLISTDELAY,
// ID3FID_KEY, ID3FID_SONGLEN, ID3FID_ORIGYEAR, ID3FID_OWNER, ID3FID_SIZE,
// ID3FID_ISRC, and ID3FID_TERMSOFUSE.  Also sorted the elements of the
// FrameDefs array based on the long id field.
//
// Revision 1.11  1999/12/09 03:31:58  scott
// (): Better comments of the ID3VC_HIGHER/ID3VC_LOWER section in the
// field defs.
// (operator=): Added implementation.
//
// Revision 1.10  1999/12/01 18:00:59  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places (and to make compiling under
// windows easier).
//
// Revision 1.9  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.8  1999/11/29 17:21:06  scott
// Updated methods to use unicode_t instead of wchar_t.  unicode_t is
// defined in globals.h.  The type unicode_t is more likely to be two
// bytes on every platform.
//
// Revision 1.7  1999/11/19 17:18:49  scott
// * field.cpp
// (BinSize): Added check for an ascii string when called by Size, since
// we store every string internally as unicode.
// (Parse): Updated interface to make parameters const.
// (ID3_FindFrameDef): Updated interface to make parameter const.
// (ID3_FindFrameID): Updated interface to make parameter const.
//
// Revision 1.6  1999/11/16 22:50:19  scott
// * field.cpp (Render): Minor reformatting.
//
// Revision 1.5  1999/11/15 20:15:25  scott
// Added include for config.h.  Replaced LU_NULL with field::NONE for
// more consistency in flag naming.  Blocked out the ID3FD_Volume
// array definition since it is currently unused.  Reformatted
// ID3_FrameDefs for easier reading (although most lines are more
// than 80 chars long now).  Made private member variable names more
// descriptive.  Fixed bug in SetVersion (was checking "rev != rev").
// Adjusted logic somewhat in BinSize to ease code reading.  Fixed
// bug in BinSize that was causing incorrect parsing of correct
// frames, such as COMM (previously assumed a wchar_t is 2 bytes,
// which might not be true on some platforms).
//
// Revision 1.4  1999/11/04 04:15:54  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
