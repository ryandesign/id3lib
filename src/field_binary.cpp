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


#include <stdio.h>
#include <memory.h>
#include "field_impl.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

/** Copies the supplied unicode string to the field.
 ** 
 ** Again, like the string types, the binary Set() function copies the data
 ** so you may dispose of the source data after a call to this method.
 **/
size_t 
ID3_FieldImpl::Set(const uchar *newData, //< data to assign to this field.
                   size_t newSize          //< number of bytes to be copied
                   )
{
  if (this->GetType() == ID3FTY_BINARY)
  {
    this->Clear();
    
    if (newSize > 0 && newData != NULL)
    {
      size_t fixed = this->Size();
      if (fixed == 0)
      {
        _bytes = newSize;
        _binary = new uchar[_bytes];
      }
      ::memcpy(_binary, newData, MIN(_bytes, newSize));
    }
    _changed = true;
  }
  return MIN(_bytes, newSize);
}


/** Copies the field's internal string to the buffer.
 ** 
 ** It copies the data in the field into the buffer, for as many bytes as the
 ** field contains, or the size of buffer, whichever is smaller.
 ** 
 ** \code
 **   uchar buffer[1024];
 **   myFrame.Field(ID3FN_DATA).Get(buffer, sizeof(buffer));
 ** \endcode
 **/
size_t ID3_FieldImpl::Get(uchar *buffer,    //< Destination of retrieved string
                          size_t max_bytes //< Max number of bytes to copy
                          ) const
{
  size_t bytes = 0;
  if (this->GetType() == ID3FTY_BINARY)
  {
    bytes = MIN(max_bytes, this->Size());
    if (NULL != buffer && NULL != _binary && bytes > 0)
    {
      ::memcpy(buffer, _binary, bytes);
    }
  }
  return bytes;
}


/** Copies binary data from the file specified to the field.
 ** 
 ** \code
 **   myFrame.Field(ID3FN_DATA).FromFile("mypic.jpg");
 ** \endcode
 **/
void ID3_FieldImpl::FromFile(const char *info //< Source filename
                             )
{
  if (this->GetType() != ID3FTY_BINARY || NULL == info)
  {
    return;
  }
    
  FILE* temp_file = ::fopen(info, "rb");
  if (temp_file != NULL)
  {
    ::fseek(temp_file, 0, SEEK_END);
    size_t fileSize = ::ftell(temp_file);
    ::fseek(temp_file, 0, SEEK_SET);
    
    uchar* buffer = new uchar[fileSize];
    if (buffer != NULL)
    {
      ::fread(buffer, 1, fileSize, temp_file);
      
      this->Set(buffer, fileSize);
      
      delete [] buffer;
    }
    
    ::fclose(temp_file);
  }
}


/** Copies binary data from the field to the specified file.
 ** 
 ** \code
 **   myFrame.Field(ID3FN_DATA).ToFile("output.bin");
 ** \endcode
 **/
void ID3_FieldImpl::ToFile(const char *info //< Destination filename
                           ) const
{
  if (this->GetType() != ID3FTY_BINARY || NULL == info)
  {
    return;
  }
    
  size_t size = this->Size();
  if ((_binary != NULL) && (size > 0))
  {
    FILE* temp_file = ::fopen(info, "wb");
    if (temp_file != NULL)
    {
      ::fwrite(_binary, 1, size, temp_file);
      ::fclose(temp_file);
    }
  }
  
  return ;
}


size_t
ID3_FieldImpl::ParseBinary(const uchar *buffer, size_t size)
{
  // copy the remaining bytes, unless we're fixed length, in which case copy
  // the minimum of the remaining bytes vs. the fixed length
  size_t bytesUsed = this->Set(buffer, size);
  _changed = false;
  
  return bytesUsed;
}


size_t ID3_FieldImpl::RenderBinary(uchar *buffer) const
{
  size_t bytesUsed = this->BinSize();
  ::memcpy(buffer, _binary, bytesUsed);
  _changed = false;
  return bytesUsed;
}
