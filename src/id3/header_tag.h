// $Id$
// 
// The authors have released ID3Lib as Public Domain (PD) and claim no
// copyright, patent or other intellectual property protection in this work.
// This means that it may be modified, redistributed and used in commercial
// and non-commercial software and hardware without restrictions.  ID3Lib is
// distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either
// express or implied.
// 
// The ID3Lib authors encourage improvements and optimisations to be sent to
// the ID3Lib coordinator, currently Scott Haug (sth2@cs.wustl.edu).  Approved
// submissions may be altered, and will be included and released under these
// terms.

#ifndef ID3LIB_HEADER_TAG_H
#define ID3LIB_HEADER_TAG_H

#include <id3/types.h>
#include <id3/header.h>

#define ID3_TAGID               "ID3"
#define ID3_TAGHEADERSIZE       (10)

#define ID3HF_UNSYNC            (1 << 7)
#define ID3HF_EXTENDEDHEADER    (1 << 6)
#define ID3HF_EXPERIMENTAL      (1 << 5)

class ID3_TagHeader : public ID3_Header
{
public:
  virtual luint Size(void);
  virtual luint Render(uchar *buffer);
};

CDLLEXPORT
lsint ID3_IsTagHeader(uchar header[ID3_TAGHEADERSIZE]);

#endif

// $Log$
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
