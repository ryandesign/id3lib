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

#ifndef ID3LIB_FIELD_H
#define ID3LIB_FIELD_H

#include <vector>
#include "types.h"
#include "enums.h"

namespace id3
{
  /** The representative class of an id3v2 field.
   **
   ** As a general rule, you need never create an object of this type.  id3lib
   ** uses them internally as part of the frame class.  You must know how
   ** to interact with these objects, though, and that's what this section is
   ** about.
   ** 
   ** The field contains many overloaded methods to provide these
   ** facilities for four different data types: integers, ASCII strings,
   ** unicode strings, and binary data.
   ** 
   ** An integer field supports the integer(), assign(uint32),  and 
   ** operator=(uint32) methods.
   **
   ** Both types of strings support the num_items() method.
   **
   ** An ASCII string field supports the ascii(int), assign(const string&),
   ** add(const string&), and operator=(const string&) methods.
   **
   ** A unicode string field supports the unicode(int), assign(const wstring&),
   ** add(const wstring&), and operator=(const wstring&) methods.  Without
   ** elaborating, the Unicode methods behave exactly the same as their ASCII
   ** counterparts, using wstring's in place of string's.
   **
   ** All strings in id3lib are handled internally as unicode.  This means that
   ** when you assign a field with an ASCII source type, it will be converted
   ** and stored internally as a unicode string.  id3lib will handle all
   ** necessary conversions when parsing, rendering, and accessing via the
   ** ascii() and unicode() methods.  If you assign a field as an ASCII string,
   ** then call the unicode() method, id3lib will automatically convert the
   ** string into unicode.  The same holds true true in reverse.  Of course,
   ** when converting from unicode to ASCII, you will experience problems when
   ** the unicode string contains characters that don't map to ISO-8859-1.
   **
   ** A binary field supports the binary(), assign(const bstring&), and 
   ** operator=(const bstring&) methods.  The binary field holds miscellaneous
   ** data that can't easily be described any other way, such as a JPEG image.
   **
   ** As a general implementation note, you should be prepared to support all
   ** fields in an id3lib frame, even if all fields in the id3lib version of
   ** the frame aren't present in the id3v2 version.  This is because of frames
   ** like the picture frame, which changed slightly from one version of the
   ** id3v2 standard to the next (the IMAGEFORMAT format in 2.0 changed to a
   ** MIMETYPE in 3.0).  If you support all id3lib fields in a given frame,
   ** id3lib can generate the correct id3v2 frame for the id3v2 version you
   ** wish to support.  Alternatively, just support the fields you know will be
   ** used in, say, 3.0 if you only plan to generate 3.0 tags.
   **
   ** @author Dirk Mahoney
   ** @version $Id$
   ** @see tag
   ** @see frame
   ** @see err 
   ***/
  class field
  {
  public:
    struct def
    {
      fld::id              id;           // the ID of this field
      fld::type            type;         // the type of this field
      size_t               fixed_size;   // length of field (fixed if positive)
      spec::version        version;      // version
      fld::scope           scope;        // versioning scope
      uint8                flags;        // special field flags
      fld::id              linked_field; // *unused*
    };
    
  public:
    field(const def& def, spec::version v) 
      : __def(def), __version(v), __changed(false) { ; }
    field(const field& f) { *this = f; __changed = false; }
    virtual ~field() { ; }
    
    // field factory
    static  field*      make(const def&);
    
    // copying/creation methods
    /*   */ field&      operator=(const field& rhs)
    { __def = rhs.__def; __version = rhs.__version; __changed = true; }
    
    virtual field*      clone() const = 0;
    
    // parsing/rendering methods
    /*   */ bstring     render() const;
    /*   */ size_t      parse(const bstring&);
    
    // other modification methods
    virtual void        clear() = 0;
    /*   */ bool        version(spec::version);
    virtual bool        encoding(fld::encoding);
    
    // common const status methods
    /*   */ fld::type   type() const;
    /*   */ fld::id     id() const;
    /*   */ size_t      size() const;
    /*   */ spec::version version() const;
    virtual fld::encoding encoding() const;
    /*   */ bool        changed() const;
    /*   */ bool        in_scope(spec::version = spec::CURRENT) const;
    virtual bool        is_integer() const;
    virtual bool        is_binary() const;
    virtual bool        is_text() const;
    /*   */ bool        is_list() const;
    /*   */ bool        is_cstr() const;
    /*   */ bool        is_encodable() const;
    /*   */ size_t      fixed_size() const;
    
    // integer field methods
    virtual uint32      integer() const;
    /*   */ field&      operator= (const uint32 data);
    /*   */ void        assign(const uint32);
    
    // text field methods
    virtual size_t      num_items() const;
    
    // ASCII text field methods
    /*   */ string      ascii(int = -1) const;
    /*   */ field&      operator= (const string&);
    /*   */ void        assign(const string&);
    /*   */ void        add(const string&);
    
    // Unicode text field methods
    virtual wstring     unicode(int = -1) const;
    /*   */ field&      operator= (const wstring&);
    /*   */ void        assign(const wstring&);
    /*   */ void        add(const wstring&);
    
    // binary field functions
    virtual bstring     binary() const;
    /*   */ field&      operator= (const bstring&);
    /*   */ void        assign(const bstring&);
    
  protected:
    virtual size_t      _size() const = 0;
    virtual bstring     _render() const = 0;
    virtual size_t      _parse(const bstring&) = 0;
    virtual bool        _assign(const wstring&);
    virtual bool        _assign(const bstring&);
    virtual bool        _assign(const uint32);
    virtual void        _encoding(fld::encoding) { ; }
    virtual void        _add(const wstring&);
    
  private:
    // data members
    def                 __def;        // field information
    spec::version       __version;
    mutable bool        __changed;    // changed since last parse/render?
    
  public:
    static const def default_binary;
    static const def default_text;
    static const def default_integer;
    static const def binary_data[];
    static const def url[];
    static const def user_url[];
    static const def text[];
    static const def user_text[];
    static const def general_text[];
    static const def terms_of_use[];
    static const def linked_info[];
    static const def picture[];
    static const def geo[];
    static const def ufi[];
    static const def play_counter[];
    static const def popularimeter[];
    static const def registration[];
    static const def text_list[];
  };

  ostream& operator<<(ostream& os, const field& fld);

  class binary_field : public field
  {
  public:
    binary_field(const field::def& = default_binary, spec::version = spec::CURRENT);
    bstring       binary() const;
    binary_field& operator=(const binary_field&);
    field*        clone() const;
    void          clear();
    bool          is_binary() const { return true; }
  protected:
    bool          _assign(const bstring&);
    size_t        _size() const;
    size_t        _parse(const bstring&);
    bstring       _render() const;
  private:
    bstring       __data;
  };

  class integer_field : public field
  {
  public:
    integer_field(const field::def& = default_integer, spec::version = spec::CURRENT);
    uint32         integer() const;
    integer_field& operator=(const integer_field&);
    field*         clone() const;
    void           clear();
    bool           is_integer() const { return true; }
  protected:
    bool           _assign(const uint32);
    size_t         _size() const;
    size_t         _parse(const bstring&);
    bstring        _render() const;
  private:
    uint32         __data;
  };

  class text_field : public field
  {
  public:
    text_field(const field::def& = default_text, spec::version = spec::CURRENT);
    void           add(const wstring&);
    wstring        unicode(int = -1) const;
    size_t         num_items() const;
    text_field&    operator=(const text_field&);
    field*         clone() const;
    void           clear();
    fld::encoding  encoding() const { return __encoding; }
    bool           is_text() const { return true; }
    
  protected:
    bool           _assign(const wstring&);
    void           _add(const wstring&);
    size_t         _size() const;
    size_t         _parse(const bstring&);
    size_t         _parse(const wstring&);
    bstring        _render() const;
    void           _encoding(fld::encoding);
  private:
    vector<wstring>  __data;
    fld::encoding  __encoding;
  };
};

#endif

// $Log$
// Revision 1.7  2000/01/04 15:42:22  eldamitri
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.6  1999/12/26 16:40:04  scott
// (class ID3_Field): Cleaned up interface to Parse* methods.
//
// Revision 1.5  1999/12/17 16:05:02  scott
// Updated opening comment block.
//
// Revision 1.4  1999/12/15 06:37:39  scott
// Reformatted some comments.
//
// Revision 1.3  1999/12/09 02:45:41  scott
// (class ID3_Field): Added const to Get method.  Added operator= method
// declaration.
//
// Revision 1.2  1999/12/02 22:45:28  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places.
//
// Revision 1.1  1999/12/01 17:16:01  scott
// moved from src/id3 to include/id3
//
// Revision 1.10  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.9  1999/11/29 19:05:41  scott
// Updated documentation to work better with doc++ (probably doesn't work
// well with kdoc anymore).
//
// Revision 1.8  1999/11/25 19:11:40  scott
// Added doc++/kdoc/javadoc-like documentation, mostly transcribed
// from Dirk's ID3Lib Documentation MSWord doc.
// (ID3_FieldFlags ): Created enumeration to replace generic constants.
// (ID3_FieldID): Removed.  Replaced in globals.h.
// (ID3_FrameID): Removed.  Replaced in globals.h.
// (class ID3_Field): Made the constructor private.
//
// Revision 1.7  1999/11/19 17:19:58  scott
// * field.h (class ID3_Field): Updated method interfaces to make
// appropriate parameters const.
//
// Revision 1.6  1999/11/15 20:15:36  scott
// Made ID3_Frame and ID3_Tag friends of the ID3_Field class to allow
// access to the private members in ID3_Field.  Made ID3FF field
// flags const luints, rather than #define's.  Made member variable
// names of classes and structs more descriptive.
//
// Revision 1.5  1999/11/04 04:32:11  scott
// Initial revision
//
// Revision 1.4  1999/11/04 04:15:54  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
