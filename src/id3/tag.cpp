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

#include <id3/tag.h>

luint ID3_Tag::s_ulInstances = 0;


ID3_Tag& operator<<(ID3_Tag& tag, ID3_Frame& frame)
{
  tag.AddFrame(&frame);
  
  return tag;
}


ID3_Tag& operator<<(ID3_Tag& tag, ID3_Frame *frame)
{
  tag.AddFrame(frame);
  
  return tag;
}


ID3_Tag::ID3_Tag(char *name)
{
  SetupTag(name);
  
  s_ulInstances++;
}


void ID3_Tag::SetupTag(char *fileInfo)
{
  __ucVersion       = ID3_TAGVERSION;
  __ucRevision      = ID3_TAGREVISION;
  __pFrameList      = NULL;
  __pBinaryList     = NULL;
  __pFindCursor     = NULL;
  __fFileHandle     = NULL;
  __bSyncOn         = false;
  __bCompression    = true;
  __bPadding        = true;
  __bExtendedHeader = true;
  __bFileWritable   = false;
  __bHasV1Tag       = false;
  __ulFileSize      = 0;
  __ulOldTagSize    = 0;
  __ulExtraBytes    = 0;

  strcpy(__sFileName, "");
  strcpy(__sTempName, "");
  
  Clear();
  
  if (fileInfo)
    Link(fileInfo);
    
  return ;
}


ID3_Tag::~ID3_Tag(void)
{
  if (NULL != __fFileHandle)
    fclose(__fFileHandle);
    
  Clear();
  
  s_ulInstances--;
  
  if (s_ulInstances == 0)
  {}
  
}


void ID3_Tag::Clear(void)
{
  if (NULL != __pFrameList)
  {
    ClearList(__pFrameList);
    __pFrameList = NULL;
  }
  
  if (NULL != __pBinaryList)
  {
    ClearList(__pBinaryList);
    __pBinaryList = NULL;
  }
  
  __pFindCursor = NULL;
  __bHasChanged = true;
  
  return ;
}


void ID3_Tag::DeleteElem(ID3_Elem *cur)
{
  if (cur)
  {
    if (cur->bTagOwns)
    {
      if (cur->pFrame)
      {
        delete cur->pFrame;
        cur->pFrame = NULL;
      }
      
      if (cur->acBinary)
      {
        delete[] cur->acBinary;
        cur->acBinary = NULL;
      }
    }
    
    __pFindCursor = NULL;
    __bHasChanged = true;
    
    delete cur;
  }
  
  return ;
}


void ID3_Tag::ClearList(ID3_Elem *list)
{
  ID3_Elem *cur = list;
  
  while (cur)
  {
    ID3_Elem *pNext;
    
    pNext = cur->pNext;
    DeleteElem(cur);
    cur = pNext;
  }
  
  return ;
}


void ID3_Tag::AddFrame(ID3_Frame *newFrame, bool freeWhenDone)
{
  ID3_Elem *elem;
  
  if (NULL == newFrame)
    ID3_THROW(ID3E_NoData);

  elem = new ID3_Elem;
  if (NULL == elem)
    ID3_THROW(ID3E_NoMemory);

  elem->pNext = __pFrameList;
  elem->pFrame = newFrame;
  elem->acBinary = NULL;
  elem->bTagOwns = freeWhenDone;
      
  __pFrameList = elem;
  __pFindCursor = NULL;
      
  __bHasChanged = true;
    
  return ;
}


void ID3_Tag::AddFrames(ID3_Frame *frames, luint numFrames, bool freeWhenDone)
{
  lsint i;
  
  for (i = numFrames - 1; i >= 0; i--)
    AddFrame(&frames[i], freeWhenDone);
    
  return ;
}


void ID3_Tag::RemoveFrame(ID3_Frame *frame)
{
  ID3_Elem *elem = Find(frame);
  if (NULL != elem)
    RemoveFromList(elem, &__pFrameList);
    
  return ;
}


void ID3_Tag::RemoveFromList(ID3_Elem *which, ID3_Elem **list)
{
  ID3_Elem *cur = *list;
  
  if (cur == which)
  {
    *list = which->pNext;
    DeleteElem(which);
  }
  else
  {
    while (cur)
    {
      if (cur->pNext == which)
      {
        cur->pNext = which->pNext;
        DeleteElem(which);
        break;
      }
      else
        cur = cur->pNext;
    }
  }
  
  return ;
}


bool ID3_Tag::HasChanged(void) const
{
  bool changed = __bHasChanged;
  
  if (! changed)
  {
    ID3_Elem *cur = __pFrameList;
    
    while (cur)
    {
      if (cur->pFrame)
        changed = cur->pFrame->HasChanged();
        
      if (changed)
        break;
      else
        cur = cur->pNext;
    }
  }
  
  return changed;
}


void ID3_Tag::SetVersion(uchar ver, uchar rev)
{
  if (__ucVersion != ver || __ucRevision != rev)
    __bHasChanged = true;
    
  __ucVersion = ver;
  __ucRevision = rev;
  
  return ;
}


void ID3_Tag::SetUnsync(bool newSync)
{
  if (__bSyncOn != newSync)
    __bHasChanged = true;
    
  __bSyncOn = newSync;
  
  return ;
}


void ID3_Tag::SetExtendedHeader(bool ext)
{
  if (__bExtendedHeader != ext)
    __bHasChanged = true;
    
  __bExtendedHeader = ext;
  
  return ;
}


void ID3_Tag::SetCompression(bool comp)
{
  if (__bCompression != comp)
    __bHasChanged = true;
    
  __bCompression = comp;
  
  return ;
}


void ID3_Tag::SetPadding(bool pad)
{
  __bHasChanged = __bHasChanged && (__bPadding != pad);
    
  __bPadding = pad;
  
  return ;
}


luint ID3_Tag::NumFrames(void) const
{
  luint numFrames = 0;
  for (ID3_Elem *cur = __pFrameList; NULL != cur; cur = cur->pNext)
  {
    numFrames++;
  }
  
  return numFrames;
}

// $Log$
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
