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

#if defined HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream.h>
#include <memory.h>
#include "tag.h"
#include "misc_support.h"

#ifdef  MAXPATHLEN
#  define ID3_PATH_LENGTH   (MAXPATHLEN + 1)
#elif   defined (PATH_MAX)
#  define ID3_PATH_LENGTH   (PATH_MAX + 1)
#else   /* !MAXPATHLEN */
#  define ID3_PATH_LENGTH   (2048 + 1)
#endif  /* !MAXPATHLEN && !PATH_MAX */

  /** Renders a binary image of the tag into the supplied buffer.
   ** 
   ** See <a href="#Size">Size</a> for an example.  This method returns the
   ** actual number of the bytes of the buffer used to store the tag.  This
   ** will be no more than the size of the buffer itself, because
   ** <a href="#Size">Size</a> over estimates the required buffer size when
   ** padding is enabled.
   ** 
   ** Before calling this method, it is advisable to call <a
   ** href="#HasChanged">HasChanged</a> first as this will let you know
   ** whether you should bother rendering the tag.
   ** 
   ** @see    ID3_IsTagHeader
   ** @see    ID3_Tag#HasChanged
   ** @return The actual number of the bytes of the buffer used to store the
   **         tag
   ** @param  buffer The buffer that will contain the rendered tag.
   **/
luint ID3_Tag::Render(uchar *buffer)
{
  luint bytesUsed = 0;
  
  if (NULL == buffer)
  {
    ID3_THROW(ID3E_NoBuffer);
  }

  ID3_Elem *cur = __pFrameList;
  ID3_TagHeader header;
    
  this->SetSpec(ID3V2_LATEST);
    
  header.SetSpec(this->GetSpec());
  bytesUsed += header.Size();
    
  // set up the encryption and grouping IDs
    
  // ...
    
  while (cur)
  {
    if (cur->pFrame != NULL)
    {
      cur->pFrame->SetSpec(this->GetSpec());
      bytesUsed += cur->pFrame->Render(&buffer[bytesUsed]);
    }
      
    cur = cur->pNext;
  }
    
  if (__bSyncOn)
  {
    uchar *tempz;
    luint newTagSize;
    
    newTagSize = GetUnSyncSize(&buffer[header.Size()], 
                               bytesUsed - header.Size());
    if (newTagSize > 0 && (newTagSize + header.Size()) > bytesUsed)
    {
      tempz = new uchar[newTagSize];
      if (NULL == tempz)
      {
        ID3_THROW(ID3E_NoMemory);
      }

      UnSync(tempz, newTagSize, &buffer[header.Size()],
             bytesUsed - header.Size());
      header.SetFlags(ID3HF_UNSYNC);

      memcpy(&buffer[header.Size()], tempz, newTagSize);
      bytesUsed = newTagSize + header.Size();
      delete[] tempz;
    }
  }
    
  // zero the remainder of the buffer so that our padding bytes are zero
  luint nPadding = PaddingSize(bytesUsed);

  memset(&buffer[bytesUsed], '\0', nPadding);
  bytesUsed += nPadding;
    
  header.SetDataSize(bytesUsed - header.Size());
  header.Render(buffer);
    
  // set the flag which says that the tag hasn't changed
  __bHasChanged = false;
  
  return bytesUsed;
}

  /** Returns an over estimate of the number of bytes required to store a
   ** binary version of a tag. 
   ** 
   ** When using <a href="#Render">Render</a> to render a binary tag to a
   ** memory buffer, first use the result of this call to allocate a buffer of
   ** unsigned chars.
   ** 
   ** \code
   **   luint tagSize;
   **   uchar *buffer;
   **   if (myTag.HasChanged())
   **   {
   **     if ((tagSize= myTag.Size()) > 0)
   **     {
   **       if (buffer = new uchar[tagSize])
   **       {
   **         luint actualSize = myTag.Render(buffer);
   **         // do something useful with the first
   **         // 'actualSize' bytes of the buffer,
   **         // like push it down a socket
   **         delete [] buffer;
   **       }
   **     }
   **   }
   ** \endcode
   **
   ** @see #Render
   ** @return The (overestimated) number of bytes required to store a binary
   **         version of a tag
   **/
luint ID3_Tag::Size(void) const
{
  luint bytesUsed = 0;
  ID3_Elem *cur = __pFrameList;
  ID3_TagHeader header;
  
  header.SetSpec(this->GetSpec());
  bytesUsed += header.Size();
  
  while (cur)
  {
    if (cur->pFrame)
    {
      cur->pFrame->SetSpec(this->GetSpec());
      bytesUsed += cur->pFrame->Size();
    }
    
    cur = cur->pNext;
  }
  
  // add 30% for sync
  if (__bSyncOn)
  {
    bytesUsed += bytesUsed / 3;
  }
    
  bytesUsed += PaddingSize(bytesUsed);
  
  return bytesUsed;
}


void ID3_Tag::RenderExtHeader(uchar *buffer)
{
  if (this->GetSpec() == ID3V2_3_0)
  {
  }
  
  return ;
}


  /** Renders an id3v1.1 version of the tag into the supplied buffer.
   ** 
   ** @return The actual number of the bytes of the buffer used to store the
   **         tag (should always be 128)
   ** @param  buffer The buffer that will contain the id3v1.1 tag.
   **/
luint ID3_Tag::RenderV1(char *buffer)
{
  // Sanity check our buffer
  if (NULL == buffer)
  {
    ID3_THROW(ID3E_NoBuffer);
  }

  // pCur is used to mark where to next write in the buffer
  // sTemp is used as a temporary string pointer for functions that return
  //  dynamically created strings
  char *pCur = buffer, *sTemp = NULL;

  // The default char for a v1 tag is null
  // Note: Using strncpy should make this unnecessary, as strncpy will pad
  //  the destination string with nulls.  But unless it becomes a performance
  //  issue (I doubt it, for 128 bit strings), its an extra layer of
  //  protection.
  memset(buffer, '\0', ID3_V1_LEN);

  // Write the TAG identifier
  strncpy(pCur, "TAG", ID3_V1_LEN_ID);
  pCur = &pCur[ID3_V1_LEN_ID];

  // Write the TITLE
  sTemp = ID3_GetTitle(this);
  if (sTemp != NULL)
  {
    strncpy(pCur, sTemp, ID3_V1_LEN_TITLE);
    delete [] sTemp;
  }
  pCur = &pCur[ID3_V1_LEN_TITLE];

  // Write the ARTIST
  sTemp = ID3_GetArtist(this);
  if (sTemp != NULL)
  {
    strncpy(pCur, sTemp, ID3_V1_LEN_ARTIST);
    delete [] sTemp;
  }
  pCur = &pCur[ID3_V1_LEN_ARTIST];

  // Write the ALBUM
  sTemp = ID3_GetAlbum(this);
  if (sTemp != NULL)
  {
    strncpy(pCur, sTemp, ID3_V1_LEN_ALBUM);
    delete [] sTemp;
  }
  pCur = &pCur[ID3_V1_LEN_ALBUM];

  // Write the YEAR
  sTemp = ID3_GetYear(this);
  if (sTemp != NULL)
  {
    strncpy(pCur, sTemp, ID3_V1_LEN_YEAR);
    delete [] sTemp;
  }
  pCur = &pCur[ID3_V1_LEN_YEAR];

  // Write the COMMENT
  sTemp = ID3_GetComment(this);
  if (sTemp != NULL)
  {
    strncpy(pCur, sTemp, ID3_V1_LEN_COMMENT);
    delete [] sTemp;
  }
  pCur = &pCur[ID3_V1_LEN_COMMENT];

  // Write the TRACK, if it isn't 0
  luint nTrack = ID3_GetTrackNum(this);
  if (0 != nTrack)
  {
    pCur -= 2;
    pCur[0] = '\0';
    pCur[1] = (uchar) nTrack;
    pCur += 2;
  }

  // Write the GENRE
  pCur[0] = (uchar) ID3_GetGenreNum(this);

  return ID3_V1_LEN;
}

void ID3_Tag::RenderV1ToHandle(void)
{
  char sTag[ID3_V1_LEN];
  char sID[ID3_V1_LEN_ID];

  RenderV1(sTag);

  // We want to check if there is already an id3v1 tag, so we can write over
  // it.  First, seek to the beginning of any possible id3v1 tag
  if (fseek(__fFileHandle, 0-ID3_V1_LEN, SEEK_END) != 0)
  {
    // TODO:  This is a bad error message.  Make it more descriptive
    ID3_THROW(ID3E_NoData);
  }

  // Read in the TAG characters
  if (fread(sID, 1, ID3_V1_LEN_ID, __fFileHandle) != ID3_V1_LEN_ID)
  {
    // TODO:  This is a bad error message.  Make it more descriptive
    ID3_THROW(ID3E_NoData);
  }

  // If those three characters are TAG, then there's a preexisting id3v1 tag,
  // so we should set the file cursor so we can overwrite it with a new tag.
  if (memcmp(sID, "TAG", ID3_V1_LEN_ID) == 0)
  {
    if (fseek(__fFileHandle, 0-ID3_V1_LEN, SEEK_END) != 0)
    {
      // TODO:  This is a bad error message.  Make it more descriptive
      ID3_THROW(ID3E_NoData);
    }
  }
  // Otherwise, set the cursor to the end of the file so we can append on 
  // the new tag.
  else
  {
    if (fseek(__fFileHandle, 0, SEEK_END) != 0)
    {
      // TODO:  This is a bad error message.  Make it more descriptive
      ID3_THROW(ID3E_NoData);
    }
  }

  fwrite(sTag, 1, ID3_V1_LEN, __fFileHandle);
  __bHasV1Tag = true;
}

void ID3_Tag::RenderV2ToHandle(void)
{
  uchar *buffer;
  luint size = Size();
  
  if (NULL == __fFileHandle)
  {
    ID3_THROW(ID3E_NoData);
  }

  if (size == 0)
  {
    return;
  }

  buffer = new uchar[size];
  if (NULL == buffer)
  {
    ID3_THROW(ID3E_NoMemory);
  }

  size = Render(buffer);      
  if (0 == size)
  {
    delete [] buffer;
    return;
  }

  // if the new tag fits perfectly within the old and the old one
  // actually existed (ie this isn't the first tag this file has had)
  if ((0 == __ulOldTagSize && 0 == __ulFileSize) || 
      (size == __ulOldTagSize && size != 0))
  {
    fseek(__fFileHandle, 0, SEEK_SET);
    fwrite(buffer, 1, size, __fFileHandle);
    __ulOldTagSize = size;
  }
  else
  {
#if !defined HAVE_MKSTEMP
    // This section is for Windows folk

    FILE *tempOut = tmpfile();
    if (NULL == tempOut)
    {
      ID3_THROW(ID3E_ReadOnly);
    }
    
    fwrite(buffer, 1, size, tempOut);
    
    fseek(__fFileHandle, __ulOldTagSize, SEEK_SET);
    
    uchar buffer2[BUFSIZ];
    while (! feof(__fFileHandle))
    {
      size_t nBytes = fread(buffer2, 1, BUFSIZ, __fFileHandle);
      fwrite(buffer2, 1, nBytes, tempOut);
    }
    
    rewind(tempOut);
    freopen(__sFileName, "wb+", __fFileHandle);
    
    while (!feof(tempOut))
    {
      size_t nBytes = fread(buffer2, 1, BUFSIZ, tempOut);
      fwrite(buffer2, 1, nBytes, __fFileHandle);
    }
    
    fclose(tempOut);
    
    __ulOldTagSize = size;
#else

    // else we gotta make a temp file, copy the tag into it, copy the
    // rest of the old file after the tag, delete the old file, rename
    // this new file to the old file's name and update the __fFileHandle

    const char sTmpSuffix[] = ".XXXXXX";
    if (strlen(__sFileName) + strlen(sTmpSuffix) > ID3_PATH_LENGTH)
    {
      ID3_THROW_DESC(ID3E_NoFile, "filename too long");
    }
    char sTempFile[ID3_PATH_LENGTH];
    strcpy(sTempFile, __sFileName);
    strcat(sTempFile, sTmpSuffix);
    
    int fd = mkstemp(sTempFile);
    if (fd < 0)
    {
      remove(sTempFile);
      ID3_THROW_DESC(ID3E_NoFile, "couldn't open temp file");
    }

    ofstream tmpOut(sTempFile);
    if (!tmpOut.is_open())
    {
      remove(sTempFile);
      ID3_THROW(ID3E_ReadOnly);
    }
    tmpOut.write(buffer, size);
    fseek(__fFileHandle, __ulOldTagSize, SEEK_SET);
      
    uchar buffer2[BUFSIZ];
    while (! feof(__fFileHandle))
    {
      size_t nBytes = fread(buffer2, 1, BUFSIZ, __fFileHandle);
      tmpOut.write(buffer2, nBytes);
    }
      
    tmpOut.close();
    CloseFile();
    remove(__sFileName);
    rename(sTempFile, __sFileName);
    
    __ulOldTagSize = size;
#endif
  }
        
  delete[] buffer;
    
  return ;
}


#define ID3_PADMULTIPLE (2048)
#define ID3_PADMAX  (4096)


luint ID3_Tag::PaddingSize(luint curSize) const
{
  luint newSize = 0;
  
  // if padding is switched off or there is no attached file
  if (! __bPadding || __ulFileSize == 0)
  {
    return 0;
  }
    
  // if the old tag was large enough to hold the new tag, then we will simply
  // pad out the difference - that way the new tag can be written without
  // shuffling the rest of the song file around
  if (__ulOldTagSize && (__ulOldTagSize >= curSize) && 
      (__ulOldTagSize - curSize) < ID3_PADMAX)
  {
    newSize = __ulOldTagSize;
  }
  else
  {
    luint tempSize = curSize + __ulFileSize;
    
    // this method of automatic padding rounds the COMPLETE FILE up to the
    // nearest 2K.  If the file will already be an even multiple of 2K (with
    // the tag included) then we just add another 2K of padding
    tempSize = ((tempSize / ID3_PADMULTIPLE) + 1) * ID3_PADMULTIPLE;
    
    // the size of the new tag is the new filesize minus the audio data
    newSize = tempSize - __ulFileSize;
  }
  
  return newSize - curSize;
}
