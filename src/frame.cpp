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

#include <zlib.h>
#include "frame.h"
#include "debug.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{
  /** Default constructor; accepts as a default parameter the type of frame
   ** to create.
   **
   ** The parameter which will internally set the frame's structure.
   ** 
   ** @param id The type of frame to create
   ** @see frame::id
   ** @see id(frame::id)
   **/
  frame::frame(frm::id id, spec::version v)
    : __hdr(frame_header(v)),
      __fields(fields()),
      __bad_parse(false)
  {
    this->_id(id); // this will end up calling _make_fields
    __changed = false;
  }

  frame::frame(spec::version v)
    : __hdr(frame_header(v)),
      __fields(fields()),
      __bad_parse(false)
  {
    this->_id(frm::NOFRAME); // this will end up calling _make_fields
    __changed = false;
  }

  frame::frame(const frame& f)
  {
    *this = f;
  }

  frame::~frame(void)
  {
    this->_clear_fields();
  }


  frame&
  frame::operator=(const frame& rhs)
  {
    if (this != &rhs)
    {
      this->_clear_fields();
      __hdr.assign(rhs.__hdr);
      __bad_parse   = false;
      __changed     = true;
      for (const_iterator fi = rhs.begin(); fi != rhs.end(); ++fi)
      {
        this->_add((*fi)->clone());
      }
    }
    return *this;
  }
  
  bstring frame::render() const
  {
    bstring data;
    bool compressed = false;
    
    frame f = *this;
    frame_header hdr = this->hdr();
    
    // Currently, if a frame is read only but one of its fields has changed,
    // the read only status of the frame is removed.
    if (hdr.test(frame_header::READONLY) && this->changed())
    {
      hdr.add(frame_header::READONLY);
    }
    
    for (iterator p = f.begin(); p != f.end(); ++p)
    {
      data += (*p)->render();
    }
    
    // if we can compress frames individually and we have been asked to
    // compress the frames
    if (hdr.test(frame_header::COMPRESSION) && hdr.version() >= spec::VER_3_0)
    {
      unsigned long new_size = data.size() + (data.size() / 10);
      uchar* raw_data = new uchar[new_size];
      
      if (compress(raw_data, &new_size, data.data(), data.size()) != Z_OK)
      {
        //ID3_THROW(ID3E_zlibError);
        throw;
      }
      
      // if the compression actually saves space
      if ((new_size + sizeof(uint32)) < data.size())
      {
        data.assign(raw_data, new_size);
      }
      
      delete [] raw_data;
    }
    else
    {
      // Currently, if a frame is read-only and it was previously compressed 
      // but now isn't, the read-only status of the frame is removed.
      if (hdr.test(frame_header::READONLY) && 
          hdr.test(frame_header::COMPRESSION))
      {
        hdr.remove(frame_header::READONLY);
      }
      hdr.remove(frame_header::COMPRESSION);
    }
    
    hdr.data_size(data.size());
    
    return hdr.render() + data;
  }

  size_t frame::parse(const bstring& buffer)
  {
    __bad_parse = false;
    size_t hdr_size = __hdr.parse(buffer);
    size_t data_size = __hdr.data_size() - __hdr.extras_size();

    if (!hdr_size)
    {
      return 0;
    }

    // data is the part of the buffer that appears after the header
    bstring data = buffer.substr(hdr_size, data_size);
    
    // expand out the data if it's compressed
    if (__hdr.test(frame_header::COMPRESSION))
    {
      unsigned long expanded_size = __hdr.expanded_size();
      uchar* raw_data = new uchar[expanded_size];
      
      uncompress(raw_data, &expanded_size, data.data(), data.size());
      data.assign(raw_data, expanded_size);
      delete [] raw_data;
    }
    
    // set the type of frame based on the parsed header
    this->_id(__hdr.id());
    try
    {
      fld::encoding enc = fld::ASCII;
      // parse the frame's fields
      for (iterator p = this->begin(); p != this->end(); ++p)
      {
        field& f = **p;
        f.encoding(enc);
        data.erase(0, f.parse(data));
        if (f.id() == fld::TEXTENC)
        {
          enc = static_cast<fld::encoding>(f.integer());
        }
      }
    }
    catch (...)
    {
      cerr << "*** parsing error!" << endl;
      // There's been an error in the parsing of the frame.
      __bad_parse = true;
    }
    
    __changed = false;
    
    return std::min(hdr_size + data_size, buffer.size());
  }

  /** Returns the type of frame that the object represents.
   **    
   ** Useful in conjunction with tag's find() method
   **   
   ** @returns The type, or id, of the frame
   ** @see tag::find()
   **/
  frm::id frame::id() const
  {
    return __hdr.id();
  }


  size_t frame::size() const
  {
    size_t size = __hdr.size();
    
    for (const_iterator p = this->begin(); p != this->end(); ++p)
    {
      size += (*p)->size();
    }
    
    return size;
  }
  
  /**
   **/
  bool frame::contains(fld::id id) const
  {
    return __bits.test(id);
  }

  /** Returns a reference to the frame's internal field indicated by the
   ** parameter.
   ** 
   ** A list of fields that are in given frames appears in id3/field.cpp.  This
   ** method returns a reference to the field in question so that the result
   ** can be used as though it were a field itself.
   **
   ** \code
   **   const id3::field&  f   = my_frame.get(id3::fld::TEXTENC);
   **   id3::fld::encoding enc = static_cast<id3::fld::encoding>(f.integer());
   ** \endcode
   **
   ** @param name The name of the field to be retrieved
   ** @returns A reference to the desired field
   **/
  field& frame::get(fld::id id) const
  {
    if (this->contains(id))
    {
      for (vector<field *>::const_iterator p = __fields.begin(); 
           p != __fields.end(); ++p)
      {
        if ((*p)->id() == id)
        {
          return **p;
        }
      }
    }
    //ID3_THROW(ID3E_FieldNotFound);
    throw;
  }

  spec::version frame::version() const
  {
    return __hdr.version();
  }

  frame_header frame::hdr() const
  {
    return __hdr;
  }

  /** Returns the read-only status of the current frame.  Currently, a 
   ** read-only frame can be altered.  However, 
   ** 
   ** @returns true if the frame is read-only; false otherwise
   **/
  bool frame::read_only() const
  {
    return __hdr.test(frame_header::READONLY);
  }
  
  bool frame::compression() const
  {
    return __hdr.test(frame_header::COMPRESSION);
  }
  
  bool frame::changed() const
  {
    if (__changed)
    {
      return true;
    }

    for (const_iterator p = this->begin(); p != this->end(); ++p)
    {
      if ((*p)->changed())
      {
        return true;
      }
    }
  
    return false;
  }

  bool frame::bad_parse() const
  {
    return __bad_parse;
  }

  /** Clears the frame of all data and resets the frame such that it can take
   ** on the form of any id3v2 frame that id3lib supports.
   ** 
   ** @see tag::clear
   */
  void frame::clear()
  {
    this->_clear_fields();
    __hdr.id(frm::NOFRAME);
    __changed = true;
    __bad_parse = false;
  }

  void frame::_clear_fields()
  {
    for (iterator vi = this->begin(); vi != this->end(); ++vi)
    {
      delete *vi;
    }
    __fields.clear();
    __bits.reset();
  }

  bool frame::encoding(fld::encoding enc)
  {
    bool changed = false;
    for (iterator p = this->begin(); p != this->end(); ++p)
    {
      changed = (*p)->encoding(enc) || changed;
    }
    __changed = changed || __changed;
  }
  
  /** Establishes the internal structure of an frame object so that it
   ** represents the id3v2 frame indicated by the parameter
   **      
   ** Given an frame::id (a list of which is found in id3/frame.h),
   ** id(frame::id) will structure the object according to the frame you wish
   ** to implement.
   **   
   ** Either using this call or via the constructor, this must be the first
   ** command performed on an frame object.
   **   
   ** \code
   **   my_frame.id(id3::frm::TITLE);
   ** \endcode
   **   
   ** @param id The type of frame this frame should be set to
   ** @see frame::id()
   **/
  bool frame::id(frm::id id)
  {
    bool changed = (this->id() != id);
    if (changed)
    {
      this->_id(id);
      __changed = true;
    }
    return changed;
  }

  void frame::_id(frm::id id)
  {
    this->_clear_fields();
    __hdr.id(id);
    this->_make_fields();
  }
  
  bool frame::version(spec::version v)
  {
    bool changed = v != __hdr.version();
    if (changed)
    {
      __hdr.version(v);
      __changed = true;
    }
    return changed;
  }

  bool frame::compression(bool b)
  {
    return __hdr.set(frame_header::COMPRESSION, b);
  }

  bool frame::read_only(bool b)
  {
    return __hdr.set(frame_header::READONLY, b);
  }

  frame::iterator frame::begin()
  {
    return __fields.begin();
  }

  frame::const_iterator frame::begin() const
  {
    return __fields.begin();
  }

  frame::iterator frame::end()
  {
    return __fields.end();
  }

  frame::const_iterator frame::end() const
  {
    return __fields.end();
  }

  void frame::_make_fields()
  {
    this->_clear_fields();
    for (const field::def* f = __hdr.fields(); f && fld::NOFIELD != f->id; ++f)
    {
      this->_add(field::make(*f));
    }
  }

  void frame::_add(field* pf)
  {
    if (!pf)
    {
      throw;
    }
    __bits.set(pf->id());
    __fields.push_back(pf);
    __changed = true;
  }
}

// $Log$
// Revision 1.12  2000/01/04 15:42:49  eldamitri
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.11  1999/12/27 06:05:25  scott
// (ID3_Frame): Added data member initialization list.  Updated to reflect
// abstraction of field bits initialization.  Added new constructor which
// accepts a frame header to copy into the frame.
// (InitFieldBits): Added implementation.  Abstracted from the constuctor.
// (Clear): Updated to reflect addition of new header data member.
// Restructured method so it can be used in more situations.
// (InitFields): Added implementation. Abstracted from the SetID method.
// (SetID): Updated to reflect new method abstractions.
// (GetID, SetVersion, Size): Updated to reflect new frame header data
// member.
//
// Revision 1.10  1999/12/26 15:10:48  scott
// Minor reformatting.
//
// Revision 1.9  1999/12/17 16:13:04  scott
// Updated opening comment block.
//
// Revision 1.8  1999/12/09 03:32:17  scott
// (operator=): Added implementation.
//
// Revision 1.7  1999/12/01 18:00:59  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places (and to make compiling under
// windows easier).
//
// Revision 1.6  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.5  1999/11/15 20:17:04  scott
// Added include for config.h.  Minor code cleanup.  Removed
// assignments from if checks; first makes assignment, then checks
// for appropriate value.  Made private member variable names more
// descriptive.
//
// Revision 1.4  1999/11/04 04:15:54  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
