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

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

#include "debug.h"

#if defined HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#include <string.h>

#include "tag_impl.h"
#include "uint28.h"
#include "utils.h"
#include "io_helpers.h"
#include "io_strings.h"

using namespace dami;

size_t ID3_TagImpl::IsV2Tag(ID3_Reader& reader)
{
  io::ExitTrigger et(reader);
  size_t tagSize = 0;
  String id = io::readText(reader, ID3_TagHeader::ID_SIZE);
  String ver = io::readText(reader, 2);
  char flags = reader.readChar();
  String size = io::readText(reader, 4);
  
  if (id == ID3_TagHeader::ID &&
      (uchar) ver [0] < 0xFF   &&      (uchar) ver [1] < 0xFF   &&
      (uchar) size[0] < 0x80   &&      (uchar) size[1] < 0x80   &&
      (uchar) size[2] < 0x80   &&      (uchar) size[3] < 0x80)
  {
    io::StringReader sr(size);
    uint28 data_size;
    data_size.Parse(sr);
    tagSize = data_size.to_uint32() + ID3_TagHeader::SIZE;
  }
  else if (id != ID3_TagHeader::ID)
  {
    // clog << "*** IsV2Tag: Not an id3v2 tag header" << endl;
  }
  else if ((uchar)ver[0] >= 0xFF)
  {
    // clog << "*** IsV2Tag: Major offset" << endl;
  }
  else if ((uchar)ver[1] >= 0xFF)
  {
    // clog << "*** ISV2Tag: Minor offset" << endl;
  }
  else if ((uchar)size[0] >= 0x80)
  {
    // clog << "*** ISV2Tag: 1st size offset" << endl;
  }
  else if ((uchar)size[1] >= 0x80)
  {
    // clog << "*** ISV2Tag: 2nd size offset" << endl;
  }
  else if ((uchar)size[2] >= 0x80)
  {
    // clog << "*** ISV2Tag: 3rd size offset" << endl;
  }
  else if ((uchar)size[3] >= 0x80)
  {
    // clog << "*** ISV2Tag: 4th size offset" << endl;
  }
  else
  {
    // clog << "*** shouldn't get here!" << endl;
  }
  
  return tagSize;
}

void ID3_RemoveFromList(ID3_Elem *which, ID3_Elem **list)
{
  ID3_Elem *cur = *list;

  if (cur == which)
  {
    *list = which->pNext;
    delete which;
    which = NULL;
  }
  else
  {
    while (cur)
    {
      if (cur->pNext == which)
      {
        cur->pNext = which->pNext;
        delete which;
        which = NULL;
        break;
      }
      else
      {
        cur = cur->pNext;
      }
    }
  }
}


void ID3_ClearList(ID3_Elem *list)
{
  ID3_Elem *next = NULL;
  for (ID3_Elem *cur = list; cur; cur = next)
  {
    next = cur->pNext;
    delete cur;
  }
}

ID3_TagImpl::ID3_TagImpl(const char *name)
  : _frames(NULL),
    _file_name(new char[ID3_PATH_LENGTH]),
    _file_size(0),
    _prepended_bytes(0),
    _appended_bytes(0),
    _is_file_writable(false)
{
  this->Clear();
  if (name)
  {
    this->Link(name);
  }
}

ID3_TagImpl::ID3_TagImpl(const ID3_Tag &tag)
  : _frames(NULL),
    _file_name(new char[ID3_PATH_LENGTH]),
    _file_size(0),
    _prepended_bytes(0),
    _appended_bytes(0),
    _is_file_writable(false)
{
  *this = tag;
}

ID3_TagImpl::~ID3_TagImpl()
{
  this->Clear();
  
  delete [] _file_name;
}

void ID3_TagImpl::Clear()
{
  if (_frames)
  {
    ID3_ClearList(_frames);
    _frames = NULL;
  }
  _num_frames = 0;
  _cursor = NULL;
  _is_padded = true;
  
  _hdr.Clear();
  _hdr.SetSpec(ID3V2_LATEST);
  
  _tags_to_parse.clear();

  _changed = true;
}


void ID3_TagImpl::AddFrame(const ID3_Frame& frame)
{
  this->AddFrame(&frame);
}

void ID3_TagImpl::AddFrame(const ID3_Frame* frame)
{
  if (frame)
  {
    ID3_Frame* new_frame = new ID3_Frame(*frame);
    this->AttachFrame(new_frame);
  }
}

void ID3_TagImpl::AttachFrame(ID3_Frame *frame)
{
  
  if (NULL == frame)
  {
    // log this
    return;
    //ID3_THROW(ID3E_NoData);
  }

  ID3_Elem *elem = new ID3_Elem;
  elem->pNext = _frames;
  elem->pFrame = frame;
  
  _frames = elem;
  _num_frames++;
  _cursor = NULL;
  
  _changed = true;
}


ID3_Frame* ID3_TagImpl::RemoveFrame(const ID3_Frame *frame)
{
  ID3_Frame *the_frame = NULL;
  ID3_Elem *elem = Find(frame);
  if (NULL != elem)
  {
    the_frame = elem->pFrame;
    //assert(the_frame == frame);
    elem->pFrame = NULL;
    ID3_RemoveFromList(elem, &_frames);
    --_num_frames;
    _changed = true;
  }
    
  return the_frame;
}


bool ID3_TagImpl::HasChanged() const
{
  bool changed = _changed;
  
  if (! changed)
  {
    ID3_Elem *cur = _frames;
    
    while (cur)
    {
      if (cur->pFrame)
      {
        changed = cur->pFrame->HasChanged();
      }
        
      if (changed)
      {
        break;
      }
      else
      {
        cur = cur->pNext;
      }
    }
  }
  
  return changed;
}

bool ID3_TagImpl::SetSpec(ID3_V2Spec spec)
{
  bool changed = _hdr.SetSpec(spec);
  _changed = _changed || changed;
  return changed;
}

ID3_V2Spec ID3_TagImpl::GetSpec() const
{
  return _hdr.GetSpec();
}

bool ID3_TagImpl::SetUnsync(bool b)
{
  bool changed = _hdr.SetUnsync(b);
  _changed = changed || _changed;
  return changed;
}

bool ID3_TagImpl::SetExtended(bool ext)
{
  bool changed = _hdr.SetExtended(ext);
  _changed = changed || _changed;
  return changed;
}

bool ID3_TagImpl::SetExperimental(bool exp)
{
  bool changed = _hdr.SetExperimental(exp);
  _changed = changed || _changed;
  return changed;
}

bool ID3_TagImpl::GetUnsync() const
{
  return _hdr.GetUnsync();
}

bool ID3_TagImpl::GetExtended() const
{
  return _hdr.GetExtended();
}

bool ID3_TagImpl::GetExperimental() const
{
  return _hdr.GetExperimental();
}

bool ID3_TagImpl::SetPadding(bool pad)
{
  bool changed = (_is_padded != pad);
  _changed = changed || _changed;
  if (changed)
  {
    _is_padded = pad;
  }
  
  return changed;
}


ID3_TagImpl &
ID3_TagImpl::operator=( const ID3_Tag &rTag )
{
  Clear();
  size_t nFrames = rTag.NumFrames();
  for (size_t nIndex = 0; nIndex < nFrames; nIndex++)
  {
    ID3_Frame *frame = new ID3_Frame;
    // Copy the frames in reverse order so that they appear in the same order
    // as the original tag when rendered.
    *frame = *(rTag[nFrames - nIndex - 1]);
    AttachFrame(frame);
  }
  return *this;
}

size_t ID3_GetDataSize(const ID3_TagImpl& tag)
{
  return tag.GetFileSize() - tag.GetPrependedBytes() - tag.GetAppendedBytes();
}

