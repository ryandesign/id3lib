// $Id$

//  The authors have released ID3Lib as Public Domain (PD) and claim no
//  copyright, patent or other intellectual property protection in this work.
//  This means that it may be modified, redistributed and used in commercial
//  and non-commercial software and hardware without restrictions.  ID3Lib is
//  distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either
//  express or implied.
//
//  The ID3Lib authors encourage improvements and optimisations to be sent to
//  the ID3Lib coordinator, currently Dirk Mahoney (dirk@id3.org).  Approved
//  submissions may be altered, and will be included and released under these
//  terms.
//
//  Mon Nov 23 18:34:01 1998

#ifndef ID3LIB_MISC_SUPPORT_H
#define ID3LIB_MISC_SUPPORT_H

#include <wchar.h>
#include <string.h>
#include <id3/types.h>
#include <id3/tag.h>

// in 'id3_misc_support.cpp'
char *ID3_GetArtist(ID3_Tag *tag);
bool  ID3_AddArtist(ID3_Tag *tag, char *text);
char *ID3_GetAlbum(ID3_Tag *tag);
bool  ID3_AddAlbum(ID3_Tag *tag, char *text);
char *ID3_GetTitle(ID3_Tag *tag);
bool  ID3_AddTitle(ID3_Tag *tag, char *text);
char *ID3_GetYear(ID3_Tag *tag);
bool  ID3_AddYear(ID3_Tag *tag, char *text);
char *ID3_GetComment(ID3_Tag *tag);
bool  ID3_AddComment(ID3_Tag *tag, char *text);
char *ID3_GetTrack(ID3_Tag *tag);
luint ID3_GetTrackNum(ID3_Tag *tag);
bool  ID3_AddTrack(ID3_Tag *tag, uchar ucTrack, uchar ucTotal = 0);
char *ID3_GetGenre(ID3_Tag *tag);
luint ID3_GetGenreNum(ID3_Tag *tag);
bool  ID3_AddGenre(ID3_Tag *tag, luint ucGenre);
char *ID3_GetLyrics(ID3_Tag *tag);
bool  ID3_AddLyrics(ID3_Tag *tag, char *text);
void  ID3_ASCIItoUnicode(wchar_t *unicode, const char *ascii, const luint len);
void  ID3_UnicodeToASCII(char *ascii, const wchar_t *unicode, const luint len);
char *ID3_GetString(const ID3_Frame *frame, const ID3_FieldID fldName);
// in 'id3_tag_parse_v1.cpp'
void ID3_RemoveTrailingSpaces(char *buffer, luint length);

#endif

// $Log$
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
