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
#include <exception>

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{

  text_field::text_field(const field::def& def, spec::version v)
    : field(def, v),
      __encoding(fld::ASCII)
  {
  }

  bool text_field::_assign(const wstring& str)
  {
    if (this->unicode() == str)
    {
      return false;
    }
    
    this->clear();
    this->_add(str);
    return true;
  }

  void text_field::_add(const wstring& str)
  {
    __data.push_back(str);
  }

  wstring text_field::unicode(int item) const
  {
    // if the item is within the range of the collection, return the 
    // appropriate item
    if (0 <= item && item < __data.size())
    {
      return __data[item];
    }
    
    // if item isn't nonnegative, then it was outside the collection's range;
    // or, if there aren't any items in the collection, then there's nothing
    // to retrieve
    if (0 <= item || __data.empty())
    {
      return wstring();
    }

    // otherwise, build up a null-divided string
    vector<wstring>::const_iterator vi = __data.begin();
    wstring ws = *vi;
    for (++vi; vi != __data.end(); ++vi)
    {
      ws += L'\0' + *vi;
    }
    return ws;
  }

  size_t text_field::num_items(void) const
  {
    return __data.size();
  }

  text_field& text_field::operator=(const text_field& f)
  {
    if (this != &f)
    {
      this->field::operator=(f);
      this->__data = f.__data;
    }
    return *this;
  }

  field* text_field::clone() const
  {
    return new text_field(*this);
  }

  void text_field::clear()
  {
    __data.clear();
  }

  void text_field::_encoding(fld::encoding e)
  {
    __encoding = e;
  }
  
  size_t text_field::_size() const
  {
    size_t size = 0;
    if (!__data.empty())
    {
      
      // initialize size to the number of dividers in the text list
      size = __data.size() - 1;
      
      // add to size the size of each string in the text list
      for (vector<wstring>::const_iterator vi = __data.begin(); 
           vi != __data.end(); ++vi)
      {
        size += vi->size();
      }
      
    }
    // add one for a null terminator, if any
    if (this->is_cstr())
    {
      size++;
    }
    
    // adjust for unicode
    if (fld::UNICODE == this->encoding())
    {
      size++;    // add one for the BOM
      size *= 2; // double for 2 bytes per unicode character
    }
    
    return size;
  }

  // this returns the number of characters parsed
  size_t text_field::_parse(const wstring& data)
  {
    // if this field is a text list, parse out all the null-separated strings 
    // and add them to the field
    if (this->is_list())
    {
      for (size_t index = 0; index < data.size(); ++index)
      {
        wstring ws(&data.data()[index]);
        this->_add(ws);
        index += ws.size();
      }
      return data.size();
    }
    
    // If we're not null terminated, then assign the whole string
    if (!this->is_cstr())
    {
      this->_assign(data);
      return data.size();
    }
    
    // we're null terminated.  thus finding the string is as simple as...
    wstring ws((data + L'\0').data());
    // ...since the constructor that accepts a wchar_t array will only copy
    // the array up to the first null.  and even if the data doesn't have a
    // null in it (if it doesn't, it's an error in the data, which we'll handle
    // next), the c_str method makes sure to append one.  clever, huh?
    
    // if the new string size is the same as (or, heaven forbid, greater than)
    // the data size, then there was no null terminator in the data.  throw!
    if (ws.size() >= data.size())
    {
      cerr << "*** ack! error in data.  no null terminator" << endl;
      throw std::exception(); // ID3_THROW_DESC(ID3E_BadData, "field information invalid");
    }

    this->_assign(ws);            // assign the new string
    return ws.size() + 1;         // add one for the null terminator
  }

  size_t text_field::_parse(const bstring& data)
  {
    this->clear();
    size_t size = 0;

    // if we're an ascii string, convert the data to ascii, then convert the
    // ascii to unicode
    switch (this->encoding())
    {
      case fld::ASCII:
      {
        size += this->_parse(to_wstring(to_string(data)));
        break;
      }
      case fld::UNICODE:
      {
        // if the data begins with a BOM, take out the BOM and swap every pair
        // of bytes, if necessary
        wstring ws = to_wstring(data);
        if (data.size() > 1)
        {
          wchar_t ch = to_wchar(data.data());
          if (BOM == ch || MOB == ch)
          {
            size++;
          }
          size += this->_parse(to_wstring(data));
        }
        // double the size since there's two bytes per unicode character
        size *= 2;
        break;
      }
      default:
      {
        cerr << "*** error! unknown text encoding" << endl;
      }
    }
    return std::min(size, data.size());
  }

  bstring text_field::_render() const
  {
    if (fld::ASCII == this->encoding())
    {
      string data = this->ascii() + '\0';
      if (!this->is_cstr())
      {
        data.resize(data.size() - 1);
      }
      return to_bstring(data);
    }

    wstring data = this->unicode() + L'\0';
    if (!this->is_cstr())
    {
      data.resize(data.size() - 1);
    }
    return to_bstring(data);
  }
}

// $Log$
