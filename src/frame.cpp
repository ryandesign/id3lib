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
#include "tag.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

ID3_Frame::ID3_Frame(ID3_FrameID id)
  : __changed(false),
    __field_bitset(NULL),
    __num_fields(0),
    __fields(NULL)
{
  __encryption_id[0] = '\0';
  __grouping_id[0]   = '\0';
  
  InitFieldBits();
  SetID(id);
}

ID3_Frame::ID3_Frame(const ID3_FrameHeader &hdr)
  : __changed(false),
    __field_bitset(NULL),
    __num_fields(0),
    __fields(NULL),
    __hdr(hdr)
{
  __encryption_id[0] = '\0';
  __grouping_id[0]   = '\0';
  
  this->InitFieldBits();
  this->InitFields();
}

ID3_Frame::ID3_Frame(const ID3_Frame& frame)
  : __changed(false),
    __field_bitset(NULL),
    __num_fields(0),
    __fields(NULL)
{
  __encryption_id[0] = '\0';
  __grouping_id[0]   = '\0';
  
  InitFieldBits();
  *this = frame;
}

void ID3_Frame::InitFieldBits()
{
  luint lWordsForFields =
    (((luint) ID3FN_LASTFIELDID) - 1) / (sizeof(luint) * 8);
  
  if ((((luint) ID3FN_LASTFIELDID) - 1) % (sizeof(luint) * 8) != 0)
  {
    lWordsForFields++;
  }
   
  if (__field_bitset != NULL)
  {
    delete [] __field_bitset;
  }
  __field_bitset = new luint[lWordsForFields];
  if (NULL == __field_bitset)
  {
    ID3_THROW(ID3E_NoMemory);
  }

  for (luint i = 0; i < lWordsForFields; i++)
  {
    __field_bitset[i] = 0;
  }
}

ID3_Frame::~ID3_Frame(void)
{
  Clear();
  
  if (__field_bitset != NULL)
  {
    delete [] __field_bitset;
  }
}


void ID3_Frame::Clear(void)
{
  if (__num_fields > 0 && NULL != __fields)
  {
    for (luint i = 0; i < __num_fields; i++)
    {
      delete __fields[i];
    }
      
    delete [] __fields;
    
    __changed = true;
  }

  __hdr.Clear();
  __encryption_id[0] = '\0';
  __grouping_id[0]   = '\0';
  __num_fields      = 0;
  __fields         = NULL;
}

void ID3_Frame::InitFields()
{
  const ID3_FrameDef* info = __hdr.GetFrameDef();
  if (NULL == info)
  {
    ID3_THROW(ID3E_InvalidFrameID);
  }
      
  __num_fields = 0;
      
  while (info->aeFieldDefs[__num_fields].eID != ID3FN_NOFIELD)
  {
    __num_fields++;
  }
      
  __fields = new ID3_Field * [__num_fields];
  if (NULL == __fields)
  {
    ID3_THROW(ID3E_NoMemory);
  }

  for (luint i = 0; i < __num_fields; i++)
  {
    __fields[i] = new ID3_Field;
    if (NULL == __fields[i])
    {
      ID3_THROW(ID3E_NoMemory);
    }

    __fields[i]->__id        = info->aeFieldDefs[i].eID;
    __fields[i]->__type        = info->aeFieldDefs[i].eType;
    __fields[i]->__length = info->aeFieldDefs[i].ulFixedLength;
    __fields[i]->__spec_begin   = info->aeFieldDefs[i].eSpecBegin;
    __fields[i]->__spec_end     = info->aeFieldDefs[i].eSpecEnd;
    __fields[i]->__flags      = info->aeFieldDefs[i].ulFlags;
            
    // tell the frame that this field is present
    BS_SET(__field_bitset, __fields[i]->__id);
  }
        
  __changed = true;
}

void ID3_Frame::SetID(ID3_FrameID id)
{
  Clear();
  
  if (id != ID3FID_NOFRAME)
  {
    __hdr.SetFrameID(id);
    this->InitFields();
  }
}


ID3_FrameID ID3_Frame::GetID(void) const
{
  return __hdr.GetFrameID();
}


bool ID3_Frame::SetSpec(const ID3_V2Spec spec)
{
  return __hdr.SetSpec(spec);
}

ID3_V2Spec ID3_Frame::GetSpec() const
{
  return __hdr.GetSpec();
}

lsint ID3_Frame::FindField(ID3_FieldID fieldName) const
{
  
  if (BS_ISSET(__field_bitset, fieldName))
  {
    for (lsint num = 0; num < (lsint) __num_fields; num++)
    {
      if (__fields[num]->__id == fieldName)
      {
        return num;
      }
    }
  }

  return -1;
}

ID3_Field& ID3_Frame::Field(ID3_FieldID fieldName) const
{
  lsint fieldNum = FindField(fieldName);
  
  if (fieldNum < 0)
  {
    ID3_THROW(ID3E_FieldNotFound);
  }
    
  return *__fields[fieldNum];
}

void ID3_Frame::UpdateFieldDeps(void)
{
  for (luint i = 0; i < __num_fields; i++)
  {
    if (__fields[i]->__flags & ID3FF_ADJUSTEDBY)
    {
      switch(__fields[i]->__type)
      {
        case ID3FTY_BITFIELD:
        {
          //luint value = 0;
          
          // now find the field on which this field is dependent and get a
          // copy of the value of that field.  then adjust the fixedLength of
          // this field to that value / 8.
          break;
        }
        
        default:
          break;
      }
    }
  }
  
  return ;
}


void ID3_Frame::UpdateStringTypes(void)
{
  for (luint i = 0; i < __num_fields; i++)
  {
    if (__fields[i]->__flags & ID3FF_ADJUSTENC)
    {
      ID3_TextEnc enc;
      ID3_FieldType newType;
      
      enc = (ID3_TextEnc) Field(ID3FN_TEXTENC).Get();
      
      switch(enc)
      {
        case ID3TE_ASCII:
          newType = ID3FTY_ASCIISTRING;
          break;
          
        case ID3TE_UNICODE:
          newType = ID3FTY_UNICODESTRING;
          break;
          
        default:
          newType = ID3FTY_ASCIISTRING;
          break;
      }
      
      __fields[i]->__type = newType;
    }
  }
  
  return ;
}


luint ID3_Frame::Size(void)
{
  luint bytesUsed = 0;
  bytesUsed += __hdr.Size();
  
  if (strlen(__encryption_id))
  {
    bytesUsed++;
  }
    
  if (strlen(__grouping_id))
  {
    bytesUsed++;
  }
    
  // this call is to tell the string fields what they should be rendered/parsed
  // as (ASCII or Unicode)
  UpdateStringTypes();
  
  for (luint i = 0; i < __num_fields; i++)
  {
    __fields[i]->SetSpec(__hdr.GetSpec());
    bytesUsed += __fields[i]->BinSize();
  }
  
  return bytesUsed;
}


bool ID3_Frame::HasChanged(void) const
{
  bool changed = __changed;
  
  if (! changed)
  {
    for (luint i = 0; i < __num_fields && !changed; i++)
    {
      changed = __fields[i]->HasChanged();
    }
  }
  
  return changed;
}

ID3_Frame &
ID3_Frame::operator=( const ID3_Frame &rFrame )
{
  if (this != &rFrame)
  {
    ID3_FrameID eID = rFrame.GetID();
    SetID(eID);
    for (size_t nIndex = 0; nIndex < __num_fields; nIndex++)
    {
      if (rFrame.__fields[nIndex] != NULL)
      {
        *(__fields[nIndex]) = *(rFrame.__fields[nIndex]);
      }
    }
  }
  return *this;
}
