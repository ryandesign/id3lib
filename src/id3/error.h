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


#ifndef ID3LIB_ERROR_H
#define ID3LIB_ERROR_H

#include <id3/types.h>

enum ID3_Err
{
  ID3E_NoMemory = 0,
  ID3E_NoData,
  ID3E_NoBuffer,
  ID3E_InvalidFrameID,
  ID3E_FieldNotFound,
  ID3E_UnknownFieldType,
  ID3E_TagAlreadyAttached,
  ID3E_InvalidTagVersion,
  ID3E_NoFile,
  ID3E_zlibError
};

class ID3_Error
{
public:
  ID3_Err GetErrorID(void);
  char   *GetErrorDesc(void);
  char   *GetErrorFile(void);
  luint   GetErrorLine(void);
  
  // *** PRIVATE INTERNAL DATA - DO NOT USE ***
private:
  ID3_Error(ID3_Err id, char *file, luint lineNum);
protected:
  ID3_Err error;
  luint errLine;
  char  errFile[ 256 ];
};

#define ID3_THROW(x) throw ID3_Error(x, __FILE__, __LINE__)

#endif

// $Log$
// Revision 1.3  1999/11/04 04:15:54  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
