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

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <memory.h>
#include "header_frame.h"
#include "error.h"

void ID3_FrameHeader::SetFrameID(ID3_FrameID id)
{
  __eFrameID = id;
  
  return ;
}


luint ID3_FrameHeader::Size(void)
{
  return __pInfo->ucFrameIDBytes + __pInfo->ucFrameSizeBytes + __pInfo->ucFrameFlagsBytes;
}


luint ID3_FrameHeader::GetFrameInfo(ID3_FrameAttr &attr, uchar *buffer)
{
  luint posn = 0;
  luint i = 0;
  
  strncpy(attr.sTextID, (char *) buffer, __pInfo->ucFrameIDBytes);

  attr.sTextID[__pInfo->ucFrameIDBytes] = 0;
  
  posn += __pInfo->ucFrameIDBytes;
  
  attr.ulSize = 0;
  
  for(i = 0; i < __pInfo->ucFrameSizeBytes; i++)
    attr.ulSize |= buffer[posn + i] << ((__pInfo->ucFrameSizeBytes - 1 - i) * 8);
    
  posn += __pInfo->ucFrameSizeBytes;
  
  attr.ulFlags = 0;
  
  for(i = 0; i < __pInfo->ucFrameFlagsBytes; i++)
    attr.ulFlags |= buffer[posn + i] << ((__pInfo->ucFrameFlagsBytes - 1 - i) * 8);
    
  posn += __pInfo->ucFrameFlagsBytes;
  
  return posn;
}


luint ID3_FrameHeader::Render(uchar *buffer)
{
  luint bytesUsed = 0;
  ID3_FrameDef *frameDef = NULL;
  char *textID = NULL;
  luint i;

  frameDef = ID3_FindFrameDef(__eFrameID);
  if (NULL == frameDef)
    ID3_THROW(ID3E_InvalidFrameID);

  if(__pInfo->ucFrameIDBytes < strlen(frameDef->sLongTextID))
    textID = frameDef->sShortTextID;
  else
    textID = frameDef->sLongTextID;
    
  memcpy(&buffer[bytesUsed], (uchar *) textID, __pInfo->ucFrameIDBytes);
  bytesUsed += __pInfo->ucFrameIDBytes;
  
  for(i = 0; i < __pInfo->ucFrameSizeBytes; i++)
    buffer[bytesUsed + i] = 
      (uchar)((__ulDataSize >> ((__pInfo->ucFrameSizeBytes - i - 1) * 8)) & 0xFF);
    
  bytesUsed += __pInfo->ucFrameSizeBytes;
  
  for(i = 0; i < __pInfo->ucFrameFlagsBytes; i++)
    buffer[bytesUsed + i] = 
      (uchar)((__ulFlags >> ((__pInfo->ucFrameFlagsBytes - i - 1) * 8)) & 0xFF);
    
  bytesUsed += __pInfo->ucFrameFlagsBytes;
  
  return bytesUsed;
}



// $Log$
// Revision 1.7  1999/12/01 18:00:59  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places (and to make compiling under
// windows easier).
//
// Revision 1.6  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.5  1999/11/15 20:19:08  scott
// Added include for config.h.  Minor code cleanup.  Removed
// assignments from if checks; first makes assignment, then checks
// for appropriate value.  Made private member variable names more
// descriptive.
//
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
