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

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <memory.h>
#include "tag.h"
#include "misc_support.h"
#include <zlib.h>

luint ID3_Frame::Render(uchar *buffer)
{
  luint bytesUsed = 0;
  
  if (NULL == buffer)
    ID3_THROW(ID3E_NoBuffer);

  ID3_FrameHeader header;
  ID3_FrameDef *info;
  luint flags;
  luint extras = 0;
  bool didCompress = false;
    
  header.SetVersion(__ucVersion, __ucRevision);
  bytesUsed += header.Size();
    
  // here is where we include things like grouping IDs and crypto IDs
  if (strlen(__sEncryptionID) > 0)
  {
    buffer[bytesUsed] = __sEncryptionID[0];
    bytesUsed++;
    extras++;
  }
    
  if (strlen(__sGroupingID) > 0)
  {
    buffer[bytesUsed] = __sGroupingID[0];
    bytesUsed++;
    extras++;
  }
    
  // this call is to tell the string fields what they should be rendered/parsed
  // as (ASCII or Unicode)
  UpdateStringTypes();
    
  for (luint i = 0; i < __ulNumFields; i++)
  {
    __apFields[i]->SetVersion(__ucVersion, __ucRevision);
    bytesUsed += __apFields[i]->Render(&buffer[bytesUsed]);
  }
    
  // if we can compress frames individually and we have been asked to compress
  // the frames
  if (__bCompression && __ucVersion >= 3)
  {
    luint newFrameSize;
    uchar *newTemp;
      
    bytesUsed -= header.Size();
      
    newFrameSize = bytesUsed +(bytesUsed / 10) + 12;
      
    newTemp = new uchar[newFrameSize];
    if (NULL == newTemp)
      ID3_THROW(ID3E_NoMemory);

    if (compress(newTemp, &newFrameSize, &buffer[header.Size() + extras],
                 bytesUsed - extras) != Z_OK)
      ID3_THROW(ID3E_zlibError);

    // if the compression actually saves space
    if ((newFrameSize + sizeof(luint)) < bytesUsed)
    {
      luint posn, i;
            
      posn = header.Size();
      extras += sizeof(luint);
            
      memcpy(&buffer[posn + sizeof(luint)], newTemp, newFrameSize);
            
      for (i = 0; i < sizeof(luint); i++)
        buffer[posn + i] =
          (uchar)((bytesUsed >> ((sizeof(luint) - i - 1) * 8)) & 0xFF);
              
      bytesUsed = newFrameSize + sizeof(luint);
      didCompress = true;
    }
          
    bytesUsed += header.Size();
        
    delete[] newTemp;
  }
    
  // perform any encryption here
  if (strlen(__sEncryptionID))
  {}
    
  // determine which flags need to be set
  info = ID3_FindFrameDef(__FrameID);
  if (NULL == info)
    ID3_THROW(ID3E_InvalidFrameID);

  flags = 0;
      
  if (info->bTagDiscard)
    flags |= ID3FL_TAGALTER;
        
  if (info->bFileDiscard)
    flags |= ID3FL_FILEALTER;
        
  if (didCompress)
    flags |= ID3FL_COMPRESSION;
        
  if (strlen(__sEncryptionID))
    flags |= ID3FL_ENCRYPTION;
        
  if (strlen(__sGroupingID))
    flags |= ID3FL_GROUPING;
      
  header.SetFrameID(__FrameID);
  header.SetFlags(flags);
  header.SetDataSize(bytesUsed - header.Size());
  header.Render(buffer);
  __bHasChanged = false;
    
  return bytesUsed;
}

// $Log$
// Revision 1.6  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.5  1999/11/15 20:18:27  scott
// Added include for config.h.  Minor code cleanup.  Removed
// assignments from if checks; first makes assignment, then checks
// for appropriate value.  Made private member variable names more
// descriptive.
//
// Revision 1.4  1999/11/04 04:15:54  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
