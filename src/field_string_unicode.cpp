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

#include <string.h>
#include <stdlib.h>
#include "field.h"
#include "utils.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

/** \fn ID3_Field& ID3_Field::operator=(const unicode_t*)
 ** \brief Shortcut for the Set operator.
 ** Performs similarly as operator=(const char*), taking a unicode_t
 ** string as a parameter rather than an ascii string.
 ** \sa Set(const unicode_t*)
 ** \param string The string to assign to the field
 **/

/** \brief Copies the supplied unicode string to the field.
 ** 
 ** Performs similarly as the ASCII Set() method, taking a unicode_t string
 ** as a parameter rather than an ascii string.
 ** 
 ** \param string The unicode string to set this field to.
 ** \sa Add(const unicode_t*)
 **/
size_t ID3_Field::Set_i(const unicode_t *string, size_t size)
{
  this->Clear();
  this->SetEncoding(ID3TE_UNICODE);
  if (_chars == 0 && size > 0)
  {
    _chars = size;
    _unicode = new unicode_t[_chars];
  }
  if (_chars > 0)
  {
    size_t loc = 0;
    // if there is a BOM, skip past it and check to see if we need to swap
    // the byte order around
    if (string[0] == 0xFEFF || string[0] == 0xFFFE)
    {
      loc++;
      _chars--;
        
      // The following is taken from the following URL:
      // http://community.roxen.com/developers/idocs/rfc/rfc2781.html
      /* The Unicode Standard and ISO 10646 define the character "ZERO WIDTH
         NON-BREAKING SPACE" (0xFEFF), which is also known informally as "BYTE
         ORDER MARK" (abbreviated "BOM"). The latter name hints at a second
         possible usage of the character, in addition to its normal use as a
         genuine "ZERO WIDTH NON-BREAKING SPACE" within text. This usage,
         suggested by Unicode section 2.4 and ISO 10646 Annex F (informative),
         is to prepend a 0xFEFF character to a stream of Unicode characters as
         a "signature"; a receiver of such a serialized stream may then use the
         initial character both as a hint that the stream consists of Unicode
         characters and as a way to recognize the serialization order. In
         serialized UTF-16 prepended with such a signature, the order is
         big-endian if the first two octets are 0xFE followed by 0xFF; if they
         are 0xFF followed by 0xFE, the order is little-endian. Note that
         0xFFFE is not a Unicode character, precisely to preserve the
         usefulness of 0xFEFF as a byte-order mark. */
    }
      
    _unicode = new unicode_t[_chars];
    memcpy((void *)_unicode, (void *)&string[loc], _chars * 2);
    if (0xFFFE == string[0])
    {
      // we need to swap the byte order...
      for (index_t i = loc; i < _chars; i++)
      {
        uchar
          u1 = ((uchar *)(&_unicode[i]))[0],
          u2 = ((uchar *)(&_unicode[i]))[1];
        _unicode[i] = (u1 << 8) | u2;
      }
    }
  }
  _changed = true;
  if (string == NULL || size == 0)
  {
    _num_items = 0;
  }
  else
  {
    _num_items = 1;
  }

  return MIN(_chars, size);
}

size_t ID3_Field::Set(const unicode_t *string)
{
  size_t len = 0;
  if (this->GetType() == ID3FTY_TEXTSTRING)
  {
    if (string == NULL)
    {
      len = this->Set_i(string, 0);
    }
    else
    {
      len = this->Set_i(string, ucslen(string));
    }
  }
  return len;
}
 
 
/** For fields which support this feature, adds a string to the list of
 ** strings currently in the field.
 ** 
 ** Performs similarly as the ASCII Add(const char*) method, taking a unicode_t
 ** string as a parameter rather than an ascii string.
 **/
size_t ID3_Field::Add_i(const unicode_t* str, size_t strLen)
{
  size_t len = 0;
  if (this->GetNumTextItems() == 0)
  {
    // there aren't any text items in the field so just assign the string to
    // the field
    len = this->Set_i(str, strLen);
  }
  else
  {
    this->SetEncoding(ID3TE_UNICODE);
    
    unicode_t *oldStr = _unicode;
    size_t oldLen = this->Size();
    //ASSERT(oldLen > 0);
    if (_fixed_length > 0)
    {
      _chars = _fixed_length;
    }
    else
    {
      _chars = oldLen + 1 + strLen;
    }
    
    if (oldLen + 1 >= _chars)
    {
      // our new length isn't any bigger than our old length, so there's 
      // nothing to copy.  set the null pointer, if possible, and set the
      // length of what we copied to 0
      _chars = oldLen;
      len = 0;
    }
    else
    {
      // our new length is bigger than our old, so we can copy some (possibly
      // all) of the bytes from str into this field
      len = _chars - oldLen - 1;
      // ASSERT(len > 0);
      _unicode = new unicode_t[_chars];
      ::memcpy((void *)_unicode, (void *)oldStr, oldLen * 2);
      delete [] oldStr;
      _unicode[oldLen] = NULL_UNICODE;
      ::memcpy((void *) &_unicode[oldLen + 1], (void *) str, len);
      _num_items++;
    }
  }
  return len;
}

size_t ID3_Field::Add(const unicode_t *str)
{
  size_t len = 0;
  if (this->GetType() == ID3FTY_TEXTSTRING)
  {
    if (NULL == str)
    {
      len = this->Set_i(str, 0);
    }
    else
    {
      len = this->Add_i(str, ucslen(str));
    }
  }
  return len;
}


/** Copies the contents of the field into the supplied buffer, up to the
 ** number of characters specified; for fields with multiple entries, the
 ** optional third parameter indicates which of the fields to retrieve.
 ** 
 ** Performs similarly as the ASCII Get(char *, size_t, index_t) method, taking
 ** a unicode_t string as a parameter rather than an ascii string.  The
 ** maxChars parameter still represents the maximum number of characters, not
 ** bytes.
 **   
 ** \code
 **   unicode_t myBuffer[1024];
 **   size_t charsUsed = myFrame.Field(ID3FN_UNICODE).Get(buffer, 1024);
 ** \endcode 
 **   
 ** \param buffer   Where the field's data is copied to
 ** \param maxChars The maximum number of characters to copy to the buffer.
 ** \param itemNum  For fields with multiple items (such as the involved
 **                 people frame, the item number to retrieve.
 ** \sa Get(char *, size_t, index_t)
 **/
size_t ID3_Field::Get(unicode_t *buffer, size_t maxLength) const
{
  size_t length = 0;
  if (this->GetType() == ID3FTY_TEXTSTRING && 
      this->GetEncoding() == ID3TE_UNICODE &&
      buffer != NULL && maxLength > 0)
  {
    size_t size = this->Size();
    length = MIN(maxLength, size);
    memcpy((void *)buffer, (void *)_unicode, length * 2);
    if (length < maxLength)
    {
      buffer[length] = NULL_UNICODE;
    }
  }
  return length;
}

size_t ID3_Field::Get(unicode_t *buffer, size_t maxLength, index_t itemNum) const
{
  size_t length = 0;
  size_t total_items = this->GetNumTextItems();
  if (this->GetType() == ID3FTY_TEXTSTRING && 
      this->GetEncoding() == ID3TE_UNICODE &&
      buffer != NULL && maxLength > 0 && itemNum < total_items)
  {
    unicode_t* cur = _unicode;
    unicode_t* end = _unicode + _chars;
    size_t num = 0;
    while (cur < end && num < itemNum)
    {
      // the last item in the list probably doesn't have a null terminator, so
      // we must anticipate so that we don't determine its length with ucslen
      if (num == total_items - 1)
      {
        break;
      }
      cur += ucslen(cur) + 1;
      num++;
    }
    if (cur < end)
    {
      // the last item in the list probably doesn't have a null terminator, so
      // we must anticipate so that we don't determine its length with ucslen
      if (itemNum == total_items - 1)
      {
        length = end - cur;
      }
      else
      {
        length = ucslen(cur);
      }
      ::memcpy((void *)buffer, (void *)cur, length * 2);
      if (length < maxLength)
      {
        buffer[length] = NULL_UNICODE;
      }
    }
  }

  return length;
}


/** Returns the number of items in a text list.
 ** 
 ** \code
 **   size_t numItems = myFrame.Field(ID3FN_UNICODE).GetNumItems();
 ** \endcode
 ** 
 ** \return The number of items in a text list.
 **/
size_t ID3_Field::GetNumTextItems() const
{
  return _num_items;
}


size_t 
ID3_Field::ParseUnicodeString(const uchar *buffer, size_t nSize)
{
  size_t nChars = 0;
  const unicode_t* unicode = (const unicode_t*) buffer;

  if (_fixed_length > 0)
  {
    nChars = _fixed_length;
  }
  else if (!(_flags & ID3FF_CSTR) || (_flags & ID3FF_LIST))
  {
    // If the string isn't null-terminated or if it is null divided, we're
    // assured this is the last field of of the frame, and we can claim the
    // remaining bytes for ourselves
    nChars = nSize / 2;
  }
  else
  {
    // it would be nice to use ucslen to find nBytes, but the buffer might be
    // invalid, so there might not be a null character to mark the end of the
    // string
    while (nChars < (nSize / 2) && unicode[nChars] != NULL_UNICODE)
    {
      nChars++;
    }
  }

  if (nChars == 0)
  {
    this->Set_i(static_cast<const char *>(NULL), 0);
  }
  // This check needs to come before the check for ID3FF_CSTR
  else if (_flags & ID3FF_LIST)
  {
    const unicode_t* cur = unicode;
    const unicode_t* end = unicode + nChars;
    while (cur < end)
    {
      size_t length = ucslen(cur);
      // sanity check!
      if (length + cur > end)
      {
        length = end - cur;
      }
      this->Add_i(cur, length);
      cur += length + 1;
    }
  }
  // This check needs to come after the check for ID3FF_LIST
  else if (_flags & ID3FF_CSTR)
  {
    this->Set_i(unicode, nChars);
  }
  else
  {
    // Sanity check our indices and sizes before we start copying memory
    if (nChars * 2 > nSize)
    {
      nChars = nSize / 2;
    }

    this->Set_i(unicode, nChars);
  }
  
  if (_flags & ID3FF_CSTR && !(_flags & ID3FF_LIST))
  {
    nChars++;
  }
    
  _changed = false;
  
  return nChars * 2;
}
