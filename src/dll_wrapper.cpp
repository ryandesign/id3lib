// $Id$
// 
// This program is free software; you can distribute it and/or modify it under
// the terms discussed in the COPYING file, which should have been included
// with this distribution.
//  
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the COPYING file for more details.
//  
// The id3lib authors encourage improvements and optimisations to be sent to
// the id3lib coordinator.  Please see the README file for details on where
// to send such submissions.

#include <cstring>
#include "tag.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef __DLL

struct ID3_VerInfo
{
  char name [ 30 ];
  luint version,
  revision;
};


// misc wrappers

CDLLEXPORT
void ID3_GetVersion(ID3_VerInfo *info)
{
  info->version = ID3LIB_VERSION;
  info->revision = ID3LIB_REVISION;
  strcpy(info->name, PACKAGE);
  
  return ;
}


// tag wrappers

CDLLEXPORT
ID3_Tag *ID3Tag_New(void)
{
  return new ID3_Tag;
}


CDLLEXPORT
void ID3Tag_Delete(ID3_Tag *tag)
{
  if (tag)
    delete tag;
    
  return ;
}


CDLLEXPORT
void ID3Tag_Clear(ID3_Tag *tag)
{
  if (tag)
    tag->Clear();
    
  return ;
}


CDLLEXPORT
bool ID3Tag_HasChanged(ID3_Tag *tag)
{
  bool changed = false;
  
  if (tag)
    changed = tag->HasChanged();
    
  return changed;
}


CDLLEXPORT
void ID3Tag_SetUnsync(ID3_Tag *tag, bool unsync)
{
  if (tag)
    tag->SetUnsync(unsync);
    
  return ;
}


CDLLEXPORT
void ID3Tag_SetExtendedHeader(ID3_Tag *tag, bool ext)
{
  if (tag)
    tag->SetExtendedHeader(ext);
    
  return ;
}


CDLLEXPORT
void ID3Tag_SetCompression(ID3_Tag *tag, bool comp)
{
  if (tag)
    tag->SetCompression(comp);
    
  return ;
}


CDLLEXPORT
void ID3Tag_SetPadding(ID3_Tag *tag, bool pad)
{
  if (tag)
    tag->SetPadding(pad);
    
  return ;
}


CDLLEXPORT
void ID3Tag_AddFrame(ID3_Tag *tag, ID3_Frame *frame)
{
  if (tag)
    tag->AddFrame(frame);
    
  return ;
}


CDLLEXPORT
void ID3Tag_AddFrames(ID3_Tag *tag, ID3_Frame *frames, luint num)
{
  if (tag)
    tag->AddFrames(frames, num);
    
  return ;
}


CDLLEXPORT
void ID3Tag_RemoveFrame(ID3_Tag *tag, ID3_Frame *frame)
{
  if (tag)
    tag->RemoveFrame(frame);
    
  return ;
}


CDLLEXPORT
void ID3Tag_Parse(ID3_Tag *tag, uchar header[ ID3_TAGHEADERSIZE ], uchar *buffer)
{
  if (tag)
    tag->Parse(header, buffer);
    
  return ;
}


CDLLEXPORT
luint ID3Tag_Link(ID3_Tag *tag, char *fileName)
{
  luint offset = 0;
  
  if (tag)
    offset = tag->Link(fileName);
    
  return offset;
}


CDLLEXPORT
void ID3Tag_Update(ID3_Tag *tag)
{
  if (tag)
    tag->Update();
    
  return ;
}


CDLLEXPORT
void ID3Tag_Strip(ID3_Tag *tag, luint ulTagFlags)
{
  if (tag)
    tag->Strip(ulTagFlags);
    
  return ;
}


CDLLEXPORT
ID3_Frame *ID3Tag_FindFrameWithID(ID3_Tag *tag, ID3_FrameID id)
{
  ID3_Frame *frame = NULL;
  
  if (tag)
    frame = tag->Find(id);
    
  return frame;
}


CDLLEXPORT
ID3_Frame *ID3Tag_FindFrameWithINT(ID3_Tag *tag, ID3_FrameID id, 
                                   ID3_FieldID fld, luint data)
{
  ID3_Frame *frame = NULL;
  
  if (tag)
    frame = tag->Find(id, fld, data);
    
  return frame;
}


CDLLEXPORT
ID3_Frame *ID3Tag_FindFrameWithASCII(ID3_Tag *tag, ID3_FrameID id, 
                                     ID3_FieldID fld, char *data)
{
  ID3_Frame *frame = NULL;
  
  if (tag)
    frame = tag->Find(id, fld, data);
    
  return frame;
}


CDLLEXPORT
ID3_Frame *ID3Tag_FindFrameWithUNICODE(ID3_Tag *tag, ID3_FrameID id, 
                                       ID3_FieldID fld, unicode_t *data)
{
  ID3_Frame *frame = NULL;
  
  if (tag)
    frame = tag->Find(id, fld, data);
    
  return frame;
}


CDLLEXPORT
luint ID3Tag_NumFrames(ID3_Tag *tag)
{
  luint num = 0;
  
  if (tag)
    num = tag->NumFrames();
    
  return num;
}


CDLLEXPORT
ID3_Frame *ID3Tag_GetFrameNum(ID3_Tag *tag, luint num)
{
  ID3_Frame *frame = NULL;
  
  if (tag)
    frame = tag->GetFrameNum(num);
    
  return frame;
}


// frame wrappers

CDLLEXPORT
void ID3Frame_Clear(ID3_Frame *frame)
{
  if (frame)
    frame->Clear();
    
  return ;
}


CDLLEXPORT
void ID3Frame_SetID(ID3_Frame *frame, ID3_FrameID id)
{
  if (frame)
    frame->SetID(id);
    
  return ;
}


CDLLEXPORT
ID3_FrameID ID3Frame_GetID(ID3_Frame *frame)
{
  ID3_FrameID id = ID3FID_NOFRAME;
  
  if (frame)
    id = frame->GetID();
    
  return id;
}


CDLLEXPORT
ID3_Field *ID3Frame_GetField(ID3_Frame *frame, ID3_FieldID name)
{
  ID3_Field *field = NULL;
  
  if (frame)
    field = &( frame->Field(name));
    
  return field;
}


// field wrappers


CDLLEXPORT
void ID3Field_Clear(ID3_Field *field)
{
  if (field)
    field->Clear();
    
  return ;
}


CDLLEXPORT
luint ID3Field_Size(ID3_Field *field)
{
  luint size = 0;
  
  if (field)
    size = field->Size();
    
  return size;
}


CDLLEXPORT
luint ID3Field_GetNumTextItems(ID3_Field *field)
{
  luint items = 0;
  
  if (field)
    items = field->GetNumTextItems();
    
  return items;
}


CDLLEXPORT
void ID3Field_SetINT(ID3_Field *field, luint data)
{
  if (field)
    field->Set(data);
    
  return ;
}


CDLLEXPORT
luint ID3Field_GetINT(ID3_Field *field)
{
  luint value = 0;
  
  if (field)
    value = field->Get();
    
  return value;
}


CDLLEXPORT
void ID3Field_SetUNICODE(ID3_Field *field, unicode_t *string)
{
  if (field)
    field->Set(string);
    
  return ;
}


CDLLEXPORT
luint ID3Field_GetUNICODE(ID3_Field *field, unicode_t *buffer, 
                          luint maxChars, luint itemNum)
{
  luint numChars = 0;
  
  if (field)
    numChars = field->Get(buffer, maxChars, itemNum);
    
  return numChars;
}


CDLLEXPORT
void ID3Field_AddUNICODE(ID3_Field *field, unicode_t *string)
{
  if (field)
    field->Add(string);
    
  return ;
}


CDLLEXPORT
void ID3Field_SetASCII(ID3_Field *field, char *string)
{
  if (field)
    field->Set(string);
    
  return ;
}


CDLLEXPORT
luint ID3Field_GetASCII(ID3_Field *field, char *buffer, 
                        luint maxChars, luint itemNum)
{
  luint numChars = 0;
  
  if (field)
    numChars = field->Get(buffer, maxChars, itemNum);
    
  return numChars;
}


CDLLEXPORT
void ID3Field_AddASCII(ID3_Field *field, char *string)
{
  if (field)
    field->Add(string);
    
  return ;
}


CDLLEXPORT
void ID3Field_SetBINARY(ID3_Field *field, uchar *data, luint size)
{
  if (field)
    field->Set(data, size);
    
  return ;
}


CDLLEXPORT
void ID3Field_GetBINARY(ID3_Field *field, uchar *buffer, luint buffLength)
{
  if (field)
    field->Get(buffer, buffLength);
    
  return ;
}


CDLLEXPORT
void ID3Field_FromFile(ID3_Field *field, char *fileName)
{
  if (field)
    field->FromFile(fileName);
    
  return ;
}


CDLLEXPORT
void ID3Field_ToFile(ID3_Field *field, char *fileName)
{
  if (field)
    field->ToFile(fileName);
    
  return ;
}


#endif

// $Log$
// Revision 1.12  2000/01/04 15:42:49  eldamitri
// * include/id3/field.h:
// * include/id3/int28.h:
// * include/id3/misc_support.h:
// * include/id3/tag.h:
// * include/id3/types.h:
// * src/id3/dll_wrapper.cpp
// * src/id3/error.cpp
// * src/id3/field.cpp
// * src/id3/field_binary.cpp
// * src/id3/field_integer.cpp
// * src/id3/field_string_ascii.cpp
// * src/id3/field_string_unicode.cpp
// * src/id3/frame.cpp
// * src/id3/frame_parse.cpp
// * src/id3/frame_render.cpp
// * src/id3/header.cpp
// * src/id3/header_frame.cpp
// * src/id3/header_tag.cpp
// * src/id3/int28.cpp
// * src/id3/misc_support.cpp
// * src/id3/tag.cpp
// * src/id3/tag_file.cpp:
// * src/id3/tag_find.cpp:
// * src/id3/tag_parse.cpp:
// * src/id3/tag_parse_lyrics3.cpp:
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.11  1999/12/17 16:13:03  scott
// Updated opening comment block.
//
// Revision 1.10  1999/12/02 22:51:37  scott
// Fixed config.h macros.
//
// Revision 1.9  1999/12/01 18:00:59  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places (and to make compiling under
// windows easier).
//
// Revision 1.8  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.7  1999/11/29 15:13:00  scott
// Updated unicode function declarations to now pass unicode_t instead of
// wchar_t.  unicode_t now defined in globals.h.
//
// Revision 1.6  1999/11/19 17:13:19  scott
// * dll_wrapper.cpp (ID3Tag_Strip): Updated for interface change to
// Tag::Strip.  Now accepts tag flags rather than a v1 boolean.
//
// Revision 1.5  1999/11/15 20:23:48  scott
// Added include for config.h.
//
// Revision 1.4  1999/11/04 04:15:54  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
