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
#include <memory.h>
#include "header_frame.h"
#include "error.h"
#include "utils.h"
#include "frame_def.h"
#include "field_def.h"
#include "field_impl.h"
#include "reader_decorators.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

void ID3_FrameHeader::SetUnknownFrame(const char* id)
{
  Clear();
  _frame_def = new ID3_FrameDef;
  if (NULL == _frame_def)
  {
    // log this;
    return;
  }
  _frame_def->eID = ID3FID_NOFRAME;
  _frame_def->bTagDiscard = false;
  _frame_def->bFileDiscard = false;
  _frame_def->aeFieldDefs = ID3_FieldDef::DEFAULT;
  _frame_def->sDescription = NULL;
  if (strlen(id) <= 3)
  {
    strcpy(_frame_def->sShortTextID, id);
    strcpy(_frame_def->sLongTextID, "");
  }
  else
  {
    strcpy(_frame_def->sLongTextID, id);
    strcpy(_frame_def->sShortTextID, "");
  }
  _dyn_frame_def = true;
}

bool ID3_FrameHeader::SetFrameID(ID3_FrameID id)
{
  if (id == ID3FID_NOFRAME || id == this->GetFrameID())
  {
    return false;
  }
  _frame_def = ID3_FindFrameDef(id);
  _flags.set(TAGALTER, _frame_def->bTagDiscard);
  _flags.set(FILEALTER, _frame_def->bFileDiscard);

  _changed = true;
  return true;
}

size_t ID3_FrameHeader::Size() const
{
  if (!_info)
  {
    return 0;
  }
  return 
    _info->frame_bytes_id   + 
    _info->frame_bytes_size + 
    _info->frame_bytes_flags;
}

size_t ID3_FrameHeader::Parse(const uchar *buffer, size_t size)
{
  size_t index = 0;
  char text_id[5];

  if (!_info)
  {
    return 0;
  }

  strncpy(text_id, (char *) buffer, _info->frame_bytes_id);
  text_id[_info->frame_bytes_id] = '\0';
  index += _info->frame_bytes_id;

  ID3_FrameID fid = ID3_FindFrameID(text_id);
  if (ID3FID_NOFRAME == fid)
  {
    this->SetUnknownFrame(text_id);
  }
  else
  {
    this->SetFrameID(fid);
  }

  this->SetDataSize(id3::parseNumber(&buffer[index], _info->frame_bytes_size));
  index += _info->frame_bytes_size;

  _flags.add(id3::parseNumber(&buffer[index], _info->frame_bytes_flags));
  index += _info->frame_bytes_flags;
  
  return index;
}

void ID3_FrameHeader::Parse(ID3_Reader& reader)
{
  char text_id[5];

  if (!_info)
  {
    return;
  }

  reader.readChars(reinterpret_cast<uchar *>(text_id), _info->frame_bytes_id);
  text_id[_info->frame_bytes_id] = '\0';

  ID3_FrameID fid = ID3_FindFrameID(text_id);
  if (ID3FID_NOFRAME == fid)
  {
    this->SetUnknownFrame(text_id);
  }
  else
  {
    this->SetFrameID(fid);
  }

  id3::NumberReader nr(reader);
  this->SetDataSize(nr.readNumber(_info->frame_bytes_size));

  _flags.add(nr.readNumber(_info->frame_bytes_flags));
}

size_t ID3_FrameHeader::Render(uchar *buffer) const
{
  size_t size = 0;

  if (NULL == _frame_def)
  {
    // TODO: log this
    return 0;
    //ID3_THROW(ID3E_InvalidFrameID);
  }
  char *text_id;
  if (_info->frame_bytes_id == strlen(_frame_def->sShortTextID))
  {
    text_id = _frame_def->sShortTextID;
  }
  else
  {
    text_id = _frame_def->sLongTextID;
  }

  memcpy(&buffer[size], (uchar *) text_id, _info->frame_bytes_id);
  size += _info->frame_bytes_id;
  
  size += id3::renderNumber(&buffer[size], _data_size, _info->frame_bytes_size);
  size += id3::renderNumber(&buffer[size], _flags.get(), _info->frame_bytes_flags);
  
  return size;
}

const char* ID3_FrameHeader::GetTextID() const
{
  char *text_id = "";
  if (_info && _frame_def)
  {
    if (_info->frame_bytes_id == strlen(_frame_def->sShortTextID))
    {
      text_id = _frame_def->sShortTextID;
    }
    else
    {
      text_id = _frame_def->sLongTextID;
    }
  }
  return text_id;
}

ID3_FrameHeader& ID3_FrameHeader::operator=(const ID3_FrameHeader& hdr)
{
  if (this != &hdr)
  {
    this->Clear();
    this->ID3_Header::operator=(hdr);
    if (!hdr._dyn_frame_def)
    {
      _frame_def = hdr._frame_def;
    }
    else
    {
      _frame_def = new ID3_FrameDef;
      if (NULL == _frame_def)
      {
        // TODO: throw something here...
      }
      _frame_def->eID = hdr._frame_def->eID;
      _frame_def->bTagDiscard = hdr._frame_def->bTagDiscard;
      _frame_def->bFileDiscard = hdr._frame_def->bFileDiscard;
      _frame_def->aeFieldDefs = hdr._frame_def->aeFieldDefs;
      strcpy(_frame_def->sShortTextID, hdr._frame_def->sShortTextID);
      strcpy(_frame_def->sLongTextID, hdr._frame_def->sLongTextID);
      _dyn_frame_def = true;
    }
  }
  return *this;
}

ID3_FrameID ID3_FrameHeader::GetFrameID() const
{
  ID3_FrameID eID = ID3FID_NOFRAME;
  if (NULL != _frame_def)
  {
    eID = _frame_def->eID;
  }

  return eID;
}

const ID3_FrameDef *ID3_FrameHeader::GetFrameDef() const
{
  return _frame_def;
}

bool ID3_FrameHeader::Clear()
{
  bool changed = this->ID3_Header::Clear();
  if (_dyn_frame_def)
  {
    delete _frame_def;
    _dyn_frame_def = false;
    changed = true;
  }
  if (_frame_def)
  {
    _frame_def = NULL;
    changed = true;
  }
  return changed;
}
