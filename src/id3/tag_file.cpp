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

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <unistd.h>
#include <id3/tag.h>

bool exists(char *name)
{
  bool doesExist = false;
  FILE *in = NULL;
  
  if (NULL == name)
    ID3_THROW(ID3E_NoData);

  in = fopen(name, "rb");
  doesExist = (NULL != in);
  if (doesExist)
    fclose(in);
    
  return doesExist;
}

void ID3_Tag::CreateFile(void)
{
  CloseFile();

  // Create a new file
  __fFileHandle = fopen(__sFileName, "wb+");

  // Check to see if file could not be created
  if (NULL == __fFileHandle)
    ID3_THROW(ID3E_ReadOnly);

  // Determine the size of the file
  fseek(__fFileHandle, 0, SEEK_END);
  __ulFileSize = ftell(__fFileHandle);
  fseek(__fFileHandle, 0, SEEK_SET);
  
  return ;
}

void ID3_Tag::OpenFileForWriting(void)
{
  CloseFile();
  
  if (exists(__sFileName))
    // Try to open the file for reading and writing.
    __fFileHandle = fopen(__sFileName, "rb+");
  else
    // No such file to open
    ID3_THROW(ID3E_NoFile);

  // Check to see if file could not be opened for writing
  if (NULL == __fFileHandle)
    ID3_THROW(ID3E_ReadOnly);

  // Determine the size of the file
  fseek(__fFileHandle, 0, SEEK_END);
  __ulFileSize = ftell(__fFileHandle);
  fseek(__fFileHandle, 0, SEEK_SET);
  
  return ;
}

void ID3_Tag::OpenFileForReading(void)
{
  CloseFile();

  __fFileHandle = fopen(__sFileName, "rb");
  if (NULL == __fFileHandle)
    ID3_THROW(ID3E_NoFile);

  // Determine the size of the file
  fseek(__fFileHandle, 0, SEEK_END);
  __ulFileSize = ftell(__fFileHandle);
  fseek(__fFileHandle, 0, SEEK_SET);

  return ;
}

bool ID3_Tag::CloseFile(void)
{
  bool bReturn = ((NULL != __fFileHandle) && (0 == fclose(__fFileHandle)));
  if (bReturn) __fFileHandle = NULL;
  return bReturn;
}

luint ID3_Tag::Link(char *fileInfo)
{
  luint posn = 0;
  
  if (NULL == fileInfo)
    ID3_THROW(ID3E_NoData);

  size_t nLen = strlen(fileInfo) + 1;
  if (NULL != __sFileName)
    delete [] __sFileName;
  __sFileName = new char[nLen];
  strncpy(__sFileName, fileInfo, nLen);
    
  // if we were attached to some other file then abort
  if (__fFileHandle != NULL)
    ID3_THROW(ID3E_TagAlreadyAttached);

  OpenFileForReading();

  __ulOldTagSize = ParseFromHandle();

  CloseFile();
    
  if (__ulOldTagSize > 0)
    __ulOldTagSize += ID3_TAGHEADERSIZE;
      
  __ulFileSize -= __ulOldTagSize;
  posn = __ulOldTagSize;
    
  return posn;
}

luint ID3_Tag::Update(luint ulTagFlag)
{
  OpenFileForWriting();
  luint ulTags = NO_TAG;

  if ((ulTagFlag & V1_TAG) && (!__bHasV1Tag || HasChanged()))
  {
    RenderV1ToHandle();
    ulTags |= V1_TAG;
  }
  if ((ulTagFlag & V2_TAG) && HasChanged())
  {
    RenderV2ToHandle();
    ulTags |= V2_TAG;
  }

  CloseFile();
    
  return ulTags;
}

luint ID3_Tag::Strip(const luint ulTagFlag)
{
  luint ulTags = NO_TAG;

  if (!(ulTagFlag & V1_TAG) && !(ulTagFlag & V2_TAG))
  {
    cerr << "--- No tag to strip. exiting." << endl;
    return ulTags;
  }

  // First remove the v2 tag, if requested
  if (ulTagFlag & V2_TAG)
  {
    OpenFileForWriting();

    // We will remove the id3v2 tag in place: since it comes at the beginning
    // of the file, we'll effectively move all the data that comes after the
    // tag back n bytes, where n is the size of the id3v2 tag.  Once we've
    // copied the data, we'll truncate the file.
    //
    // To copy the data, we'll need to keep two "pointers" in the file: one
    // will mark where to read from next, the other will indicate where to 
    // write to. 
    long nNextRead, nNextWrite;
    nNextWrite = ftell(__fFileHandle);
    // Set the read pointer past the tag
    fseek(__fFileHandle, __ulOldTagSize, SEEK_CUR);
    nNextRead = ftell(__fFileHandle);
    
    uchar aucBuffer[BUFF_SIZE];
    
    // The nBytesRemaining variable indicates how many bytes are to be copied
    size_t nBytesToCopy = __ulFileSize;

    // Here we reduce the nBytesToCopy by the size of any tags that appear
    // at the end of the file (e.g the id3v1 and lyrics tag).  This isn't
    // strictly necessary, since the truncation stage will remove these,
    // but this check prevents us from copying them unnecessarily.
    if ((__ulExtraBytes > 0) && (ulTagFlag & V1_TAG))
    {
      nBytesToCopy -= __ulExtraBytes;
    }
    
    // The nBytesRemaining variable indicates how many bytes are left to be 
    // moved in the actual file.
    // The nBytesCopied variable keeps track of how many actual bytes were
    // copied (or moved) so far.
    size_t 
      nBytesRemaining = nBytesToCopy,
      nBytesCopied = 0;
    while (! feof(__fFileHandle))
    {
      // Move to the next read position
      fseek(__fFileHandle, nNextRead, SEEK_SET);
      size_t
        nBytesToRead = MIN(nBytesRemaining - nBytesCopied, BUFF_SIZE),
        nBytesRead   = fread(aucBuffer, 1, nBytesToRead, __fFileHandle);
      // Now that we've read, mark the current spot as the next spot for
      // reading
      nNextRead = ftell(__fFileHandle);

      if (nBytesRead > 0)
      {
        // Move to the next write position
        fseek(__fFileHandle, nNextWrite, SEEK_SET);
        size_t nBytesWritten = fwrite(aucBuffer, 1, nBytesRead, __fFileHandle);
        if (nBytesRead > nBytesWritten)
          cerr << "--- attempted to write " << nBytesRead << " bytes, "
               << "only wrote " << nBytesWritten << endl;
        // Marke the current spot as the next write position
        nNextWrite = ftell(__fFileHandle);
        nBytesCopied += nBytesWritten;
      }


      if (nBytesCopied == nBytesToCopy)
        break;
      if (nBytesToRead < BUFF_SIZE)
        break;
    }

    CloseFile();
  }
  
  size_t nNewFileSize = __ulFileSize;
  if ((__ulExtraBytes > 0) && (ulTagFlag & V1_TAG))
  {
    nNewFileSize -= __ulExtraBytes;
    ulTags |= V1_TAG;
  }
  if (ulTagFlag & V2_TAG && (__ulOldTagSize > 0))
  {
    nNewFileSize -= __ulOldTagSize;
    ulTags |= V2_TAG;
  }
  if (ulTags && (truncate(__sFileName, nNewFileSize) == -1))
    ID3_THROW(ID3E_NoFile);

  __ulOldTagSize = (ulTags & V2_TAG) ? 0 : __ulOldTagSize;
  __ulExtraBytes = (ulTags & V1_TAG) ? 0 : __ulExtraBytes;
      
  __bHasV1Tag    = __bHasV1Tag && !(ulTagFlag & V1_TAG);
        
  __bHasChanged  = ((ulTags & V1_TAG) || (ulTags & V2_TAG));
  
  return ulTags;
}



// $Log$
// Revision 1.5  1999/11/19 19:09:12  scott
// (Update): Changed parameter to be a flag which indicates which type of
// tag to update, either V1_TAG, V2_TAG, or BOTH_TAGS.  Updated method to
// act appropriately based on the parameter passed in.
// (Strip): Changed parameter to be a flag which indicates which type of
// tag to update, either V1_TAG, V2_TAG, or BOTH_TAGS.  Updated method to
// act appropriately based on the parameter passed in.
//
// Revision 1.4  1999/11/15 20:20:37  scott
// Added include for config.h.  Minor code cleanup.  Removed
// assignments from if checks; first makes assignment, then checks
// for appropriate value.  Made private member variable names more
// descriptive.  OpenLinkedFile now checks to see if the file is
// writable or not, and still opens the file, even if it can't be
// written to.  This needs to be more robust (i.e., we shouldn't be
// creating temp files if the file isn't writable).  Made use of
// bFileWrtiable variable, which was introduced.
//
// Revision 1.3  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
