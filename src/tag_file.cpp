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

#include <string.h>
#include <iostream.h>
#include <stdio.h>
#include "tag.h"

#if defined WIN32
#  include <windows.h>
static int truncate(const char *path, size_t length)
{
  int result = -1;
  HANDLE fh;
  
  fh = ::CreateFile(path,
                    GENERIC_WRITE | GENERIC_READ,
                    0,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);
  
  if(INVALID_HANDLE_VALUE != fh)
  {
    SetFilePointer(fh, length, NULL, FILE_BEGIN);
    SetEndOfFile(fh);
    CloseHandle(fh);
    result = 0;
  }
  
  return result;
}

// prevents a weird error I was getting compiling this under windows
#  if defined CreateFile
#    undef CreateFile
#  endif

#else
#  include <unistd.h>
#endif

#if defined HAVE_CONFIG_H
#  include <config.h>
#endif

bool exists(const char *name)
{
  bool doesExist = false;
  FILE *in = NULL;
  
  if (NULL == name)
  {
    ID3_THROW(ID3E_NoData);
  }

  in = fopen(name, "rb");
  doesExist = (NULL != in);
  if (doesExist)
  {
    fclose(in);
  }
    
  return doesExist;
}



void ID3_Tag::CreateFile(void)
{
  CloseFile();

  // Create a new file
  __fFileHandle = fopen(__sFileName, "wb+");

  // Check to see if file could not be created
  if (NULL == __fFileHandle)
  {
    ID3_THROW(ID3E_ReadOnly);
  }

  // Determine the size of the file
  fseek(__fFileHandle, 0, SEEK_END);
  __ulFileSize = ftell(__fFileHandle);
  fseek(__fFileHandle, 0, SEEK_SET);
  
  return ;
}

ID3_Err ID3_Tag::OpenFileForWriting(void)
{
  CloseFile();
  
  if (exists(__sFileName))
  {
    // Try to open the file for reading and writing.
    __fFileHandle = fopen(__sFileName, "rb+");
  }
  else
  {
    ID3_THROW(ID3E_NoFile);
    return ID3E_NoFile;
  }

  // Check to see if file could not be opened for writing
  if (NULL == __fFileHandle)
  {
    ID3_THROW(ID3E_ReadOnly);
    return ID3E_ReadOnly;
  }

  // Determine the size of the file
  fseek(__fFileHandle, 0, SEEK_END);
  __ulFileSize = ftell(__fFileHandle);
  fseek(__fFileHandle, 0, SEEK_SET);
  
  return ID3E_NoError;
}

ID3_Err ID3_Tag::OpenFileForReading(void)
{
  CloseFile();
  __ulFileSize = 0;

  __fFileHandle = fopen(__sFileName, "rb");
  
  if (NULL == __fFileHandle)
  {
    ID3_THROW(ID3E_NoFile);
    return ID3E_NoFile;
  }

  // Determine the size of the file
  fseek(__fFileHandle, 0, SEEK_END);
  __ulFileSize = ftell(__fFileHandle);
  fseek(__fFileHandle, 0, SEEK_SET);

  return ID3E_NoError;
}

bool ID3_Tag::CloseFile(void)
{
  bool bReturn = ((NULL != __fFileHandle) && (0 == fclose(__fFileHandle)));
  if (bReturn)
  {
    __fFileHandle = NULL;
  }
  return bReturn;
}

luint ID3_Tag::Link(char *fileInfo, bool parseID3v1, bool parseLyrics3)
{
  luint tt = ID3TT_NONE;
  if (parseID3v1)
  {
    tt |= ID3TT_ID3V1;
  }
  if (parseLyrics3)
  {
    tt |= ID3TT_LYRICS;
  }
  return this->Link(fileInfo, tt);
}

  /** Attaches a file to the tag, parses the file, and adds any tag information
   ** found in the file to the tag.
   ** 
   ** Use this method if you created your ID3_Tag object without supplying a
   ** parameter to the constructor (maybe you created an array of ID3_Tag
   ** pointers).  This is the preferred method of interacting with files, since
   ** id3lib can automatically do things like parse foreign tag formats and
   ** handle padding when linked to a file.  When a tag is linked to a file,
   ** you do not need to use the <a href="#Size">Size</a>, <a
   ** href="#Render">Render</a>, or <a href="#Parse">Parse</a> methods or the
   ** <code>ID3_IsTagHeader</code> function---id3lib will take care of those
   ** details for you.  The single parameter is a pointer to a file name.
   ** 
   ** Link returns a 'luint' which is the byte position within the file that
   ** the audio starts (i.e., where the id3v2 tag ends).
   ** 
   ** \code
   **   ID3_Tag *myTag;
   **   if (myTag = new ID3_Tag)
   **   {
   **     myTag->Link("mysong.mp3");
   **     
   **     // do whatever we want with the tag
   **     // ...
   **   
   **     // setup all our rendering parameters
   **     myTag->SetUnsync(false);
   **     myTag->SetExtendedHeader(true);
   **     myTag->SetCompression(true);
   **     myTag->SetPadding(true);
   **     
   **     // write any changes to the file
   **     myTag->Update()
   **     
   **     // free the tag
   **     delete myTag;
   **   }
   ** \endcode
   ** 
   ** @see ID3_IsTagHeader
   ** @param fileInfo The filename of the file to link to.
   **/
luint ID3_Tag::Link(char *fileInfo, const luint tag_types)
{
  luint posn = 0;
  
  __ulTagsToParse = tag_types;
  
  if (NULL == fileInfo)
  {
    ID3_THROW(ID3E_NoData);
  }

  strcpy(__sFileName, fileInfo);
    
  // if we were attached to some other file then abort
  if (__fFileHandle != NULL)
  {
    ID3_THROW(ID3E_TagAlreadyAttached);
  }
  
  if (ID3E_NoError != OpenFileForReading())
  {
    __ulOldTagSize = 0;
  }
  else
  {
    __ulOldTagSize = ParseFromHandle();
    
    CloseFile();
  }
  
  if (__ulOldTagSize > 0)
  {
    __ulOldTagSize += ID3_TAGHEADERSIZE;
  }

  __ulFileSize -= __ulOldTagSize;
  posn = __ulOldTagSize;
  
  return posn;
}

  /** Renders the tag and writes it to the attached file; the type of tag
   ** rendered can be specified as a parameter.  The default is to update only
   ** the ID3v2 tag.  See the ID3_TagType enumeration for the constants that
   ** can be used.
   ** 
   ** Make sure the rendering parameters are set before calling the method.
   ** See the Link dcoumentation for an example of this method in use.
   ** 
   ** \sa ID3_TagType
   ** \sa Link
   ** \param tt The type of tag to update.
   **/
luint ID3_Tag::Update(const luint ulTagFlag)
{
  OpenFileForWriting();
  luint ulTags = ID3TT_NONE;

  if ((ulTagFlag & ID3TT_ID3V1) && (!__bHasV1Tag || HasChanged()))
  {
    RenderV1ToHandle();
    ulTags |= ID3TT_ID3V1;
  }
  if ((ulTagFlag & ID3TT_ID3V2) && HasChanged())
  {
    RenderV2ToHandle();
    ulTags |= ID3TT_ID3V2;
  }

  //CloseFile();
    
  return ulTags;
}

  /** Strips the tag(s) from the attached file. The type of tag stripped
   ** can be specified as a parameter.  The default is to strip all tag types.
   ** 
   ** \param tt The type of tag to strip
   ** \sa ID3_TagType@see
   **/
luint ID3_Tag::Strip(const luint ulTagFlag)
{
  luint ulTags = ID3TT_NONE;

  if (!(ulTagFlag & ID3TT_ID3V1) && !(ulTagFlag & ID3TT_ID3V2))
  {
    return ulTags;
  }

  // First remove the v2 tag, if requested
  if (ulTagFlag & ID3TT_ID3V2)
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
    
    uchar aucBuffer[BUFSIZ];
    
    // The nBytesRemaining variable indicates how many bytes are to be copied
    size_t nBytesToCopy = __ulFileSize;

    // Here we reduce the nBytesToCopy by the size of any tags that appear
    // at the end of the file (e.g the id3v1 and lyrics tag).  This isn't
    // strictly necessary, since the truncation stage will remove these,
    // but this check prevents us from copying them unnecessarily.
    if ((__ulExtraBytes > 0) && (ulTagFlag & ID3TT_ID3V1))
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
        nBytesToRead = MIN(nBytesRemaining - nBytesCopied, BUFSIZ),
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
        {
          cerr << "--- attempted to write " << nBytesRead << " bytes, "
               << "only wrote " << nBytesWritten << endl;
        }
        // Marke the current spot as the next write position
        nNextWrite = ftell(__fFileHandle);
        nBytesCopied += nBytesWritten;
      }


      if (nBytesCopied == nBytesToCopy)
      {
        break;
      }
      if (nBytesToRead < BUFSIZ)
      {
        break;
      }
    }

    CloseFile();
  }
  
  size_t nNewFileSize = __ulFileSize;
  if ((__ulExtraBytes > 0) && (ulTagFlag & ID3TT_ID3V1))
  {
    nNewFileSize -= __ulExtraBytes;
    ulTags |= ID3TT_ID3V1;
  }
  if (ulTagFlag & ID3TT_ID3V2 && (__ulOldTagSize > 0))
  {
    nNewFileSize -= __ulOldTagSize;
    ulTags |= ID3TT_ID3V2;
  }
  if (ulTags && (truncate(__sFileName, nNewFileSize) == -1))
  {
    ID3_THROW(ID3E_NoFile);
  }

  __ulOldTagSize = (ulTags & ID3TT_ID3V2) ? 0 : __ulOldTagSize;
  __ulExtraBytes = (ulTags & ID3TT_ID3V1) ? 0 : __ulExtraBytes;
      
  __bHasV1Tag    = __bHasV1Tag && !(ulTagFlag & ID3TT_ID3V1);
        
  __bHasChanged  = ((ulTags & ID3TT_ID3V1) || (ulTags & ID3TT_ID3V2));
  
  return ulTags;
}



// $Log$
// Revision 1.3  2000/04/23 17:38:00  eldamitri
// - Updated ID3_TagType constants to new names
//
// Revision 1.2  2000/04/18 22:13:03  eldamitri
// Moved tag_file.cpp from src/id3/ to src/
//
// Revision 1.17  2000/04/17 02:31:35  eldamitri
// Updated parameters of certain methods with const modifier to match
// declaration.
//
// Revision 1.16  2000/04/08 04:41:38  eldamitri
// Changed new ANSI-standard C++ include headers to old-style headers.
// Fixed minor problems for windows.
//
// Revision 1.15  2000/04/07 04:34:28  eldamitri
// Added optional parameters to Link to make parsing of id3v1/lyrics3
// tags optional.
//
// Revision 1.14  2000/04/05 05:21:15  eldamitri
// Updated initial comment information to reflect license, copyright
// change.
//
// Revision 1.13  2000/01/04 15:42:49  eldamitri
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.12  1999/12/17 16:13:04  scott
// Updated opening comment block.
//
// Revision 1.11  1999/12/13 04:44:23  scott
// (exists): Add const qualifier to parameter.
// (Link): Change to reflect new __sFileName type.
//
// Revision 1.10  1999/12/09 03:32:33  scott
// Minor code cleanup.
//
// Revision 1.9  1999/12/01 22:16:36  scott
// (truncate): Added.  Defined only for windows, which doesn't have
// unistd.h available (thanks elrod).
//
// Revision 1.8  1999/12/01 18:00:59  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places (and to make compiling under
// windows easier).
//
// Revision 1.7  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.6  1999/11/25 18:39:29  scott
// (OpenLinkedFile): Removed method.  Replaced with CreateFile,
// OpenFileForReading, and OpenFileForWriting.
// (CreateFile): New method.  Creates a new file with file name sFileName.
// Throws if unable to.
// (OpenFileForWriting): New method.  Opens a file with write permissions.
// Throws if unable to.
// (OpenFileForReading): New method.  Opens a file with read permissions.
// Throws if unable to.
// (CloseFile): New method.  Closes a file.  Returns true if successful,
// false otherwise.
// (Link): Now dynamically allocate sFileName.  Make use of new method
// OpenFileForReading rather than OpenLinkedFile.  Closes the file
// when done.
// (Update): Now returns a flag indicating which tags were actually
// written.  Calls OpenFileForWriting to open the file appropriately.
// Closes the file when done.
// (Strip): Now returns a flag indicating which tags were actually
// written.  Now strips a file "in place," by copying the non-stripped
// information to the beginning of the file and using ftruncate to chop
// off the rest.  Prevents having to create a temporary file.  Calls
// OpenFileForWriting to open the file appropriately.  Closes the file
// when done.
//
// Revision 1.5  1999/11/19 19:09:12  scott
// (Update): Changed parameter to be a flag which indicates which type of
// tag to update, either ID3TT_ID3V1, ID3TT_ID3V2, or ID3TT_ID3.  Updated method to
// act appropriately based on the parameter passed in.
// (Strip): Changed parameter to be a flag which indicates which type of
// tag to update, either ID3TT_ID3V1, ID3TT_ID3V2, or ID3TT_ID3.  Updated method to
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
