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
#include "utils.h"

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
size_t ID3_Tag::Render(uchar *buffer) const
{
  luint bytesUsed = 0;
  
  if (NULL == buffer)
  {
    ID3_THROW(ID3E_NoBuffer);
  }

  ID3_Elem *cur = __frames;
  ID3_TagHeader header;
    
  header.SetSpec(ID3V2_LATEST);
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
    
  if (__hdr.GetUnsync())
  {
    uchar *tempz;
    luint newTagSize;
    
    newTagSize = ID3_GetUnSyncSize(&buffer[header.Size()], 
                                   bytesUsed - header.Size());
    if (newTagSize > 0 && (newTagSize + header.Size()) > bytesUsed)
    {
      tempz = new uchar[newTagSize];
      if (NULL == tempz)
      {
        ID3_THROW(ID3E_NoMemory);
      }

      ID3_UnSync(tempz, newTagSize, &buffer[header.Size()],
                 bytesUsed - header.Size());
      header.SetUnsync(true);

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
  __changed = false;
  
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
size_t ID3_Tag::Size() const
{
  luint bytesUsed = 0;
  ID3_Elem *cur = __frames;
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
  if (__hdr.GetUnsync())
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
size_t ID3_Tag::RenderV1(uchar *buffer) const
{
  // Sanity check our buffer
  if (NULL == buffer)
  {
    ID3_THROW(ID3E_NoBuffer);
  }

  // pCur is used to mark where to next write in the buffer
  // sTemp is used as a temporary string pointer for functions that return
  //  dynamically created strings
  char* pCur = (char *) buffer;
  char* sTemp = NULL;

  // The default char for a v1 tag is null
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

void ID3_Tag::RenderV1ToHandle()
{
  uchar sTag[ID3_V1_LEN];
  char sID[ID3_V1_LEN_ID];

  RenderV1(sTag);

  if (__file_handle == NULL)
  {
    // log this
    ID3_THROW(ID3E_NoData);
    // cerr << "*** Ack! __file_handle is null!" << endl;
  }

  if (ID3_V1_LEN > __file_size)
  {
    if (fseek(__file_handle, 0, SEEK_END) != 0)
    {
      // TODO:  This is a bad error message.  Make it more descriptive
      ID3_THROW(ID3E_NoData);
    }
  }
  else
  {
    // We want to check if there is already an id3v1 tag, so we can write over
    // it.  First, seek to the beginning of any possible id3v1 tag
    if (fseek(__file_handle, 0-ID3_V1_LEN, SEEK_END) != 0)
    {
      // TODO:  This is a bad error message.  Make it more descriptive
      ID3_THROW(ID3E_NoData);
    }

    // Read in the TAG characters
    if (fread(sID, 1, ID3_V1_LEN_ID, __file_handle) != ID3_V1_LEN_ID)
    {
      // TODO:  This is a bad error message.  Make it more descriptive
      ID3_THROW(ID3E_NoData);
    }

    // If those three characters are TAG, then there's a preexisting id3v1 tag,
    // so we should set the file cursor so we can overwrite it with a new tag.
    if (memcmp(sID, "TAG", ID3_V1_LEN_ID) == 0)
    {
      if (fseek(__file_handle, 0-ID3_V1_LEN, SEEK_END) != 0)
      {
        // TODO:  This is a bad error message.  Make it more descriptive
        ID3_THROW(ID3E_NoData);
      }
    }
    // Otherwise, set the cursor to the end of the file so we can append on 
    // the new tag.
    else
    {
      if (fseek(__file_handle, 0, SEEK_END) != 0)
      {
        // TODO:  This is a bad error message.  Make it more descriptive
        ID3_THROW(ID3E_NoData);
      }
    }
  }

  fwrite(sTag, sizeof(uchar), ID3_V1_LEN, __file_handle);
  __file_tags.add(ID3TT_ID3V1);
}

void ID3_Tag::RenderV2ToHandle()
{
  uchar *buffer;
  luint size = Size();
  
  if (NULL == __file_handle)
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
  if ((0 == __starting_bytes && 0 == __file_size) || 
      (size == __starting_bytes && size != 0))
  {
    fseek(__file_handle, 0, SEEK_SET);
    fwrite(buffer, 1, size, __file_handle);
    __starting_bytes = size;
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
    
    fseek(__file_handle, __starting_bytes, SEEK_SET);
    
    uchar buffer2[BUFSIZ];
    while (! feof(__file_handle))
    {
      size_t nBytes = fread(buffer2, 1, BUFSIZ, __file_handle);
      fwrite(buffer2, 1, nBytes, tempOut);
    }
    
    rewind(tempOut);
    freopen(__file_name, "wb+", __file_handle);
    
    while (!feof(tempOut))
    {
      size_t nBytes = fread(buffer2, 1, BUFSIZ, tempOut);
      fwrite(buffer2, 1, nBytes, __file_handle);
    }
    
    fclose(tempOut);
    
    __starting_bytes = size;
#else

    // else we gotta make a temp file, copy the tag into it, copy the
    // rest of the old file after the tag, delete the old file, rename
    // this new file to the old file's name and update the __file_handle

    const char sTmpSuffix[] = ".XXXXXX";
    if (strlen(__file_name) + strlen(sTmpSuffix) > ID3_PATH_LENGTH)
    {
      ID3_THROW_DESC(ID3E_NoFile, "filename too long");
    }
    char sTempFile[ID3_PATH_LENGTH];
    strcpy(sTempFile, __file_name);
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
    fseek(__file_handle, __starting_bytes, SEEK_SET);
      
    uchar buffer2[BUFSIZ];
    while (! feof(__file_handle))
    {
      size_t nBytes = fread(buffer2, 1, BUFSIZ, __file_handle);
      tmpOut.write(buffer2, nBytes);
    }
      
    tmpOut.close();

    CloseFile();

    remove(__file_name);
    rename(sTempFile, __file_name);

    OpenFileForWriting();
    
    __starting_bytes = size;
#endif
  }
        
  delete[] buffer;
    
  return ;
}


#define ID3_PADMULTIPLE (2048)
#define ID3_PADMAX  (4096)


size_t ID3_Tag::PaddingSize(size_t curSize) const
{
  luint newSize = 0;
  
  // if padding is switched off or there is no attached file
  if (! __is_padded || __file_size == 0)
  {
    return 0;
  }
    
  // if the old tag was large enough to hold the new tag, then we will simply
  // pad out the difference - that way the new tag can be written without
  // shuffling the rest of the song file around
  if (__starting_bytes && (__starting_bytes >= curSize) && 
      (__starting_bytes - curSize) < ID3_PADMAX)
  {
    newSize = __starting_bytes;
  }
  else
  {
    luint tempSize = curSize + __file_size;
    
    // this method of automatic padding rounds the COMPLETE FILE up to the
    // nearest 2K.  If the file will already be an even multiple of 2K (with
    // the tag included) then we just add another 2K of padding
    tempSize = ((tempSize / ID3_PADMULTIPLE) + 1) * ID3_PADMULTIPLE;
    
    // the size of the new tag is the new filesize minus the audio data
    newSize = tempSize - __file_size;
  }
  
  return newSize - curSize;
}
