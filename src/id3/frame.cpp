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
#include <id3/tag.h>

ID3_Frame::ID3_Frame(ID3_FrameID id)
{
  luint lwordsForFields = 0;
  
  __ucVersion  = ID3_TAGVERSION;
  __ucRevision = ID3_TAGREVISION;
  __ulNumFields = 0;
  __apFields = NULL;
  __sGroupingID[0] = 0;
  __sEncryptionID[0] = 0;
  __bCompression = true;
  
  lwordsForFields =(((luint) ID3FN_LASTFIELDID) - 1) / (sizeof(luint) * 8);
  
  if ((((luint) ID3FN_LASTFIELDID) - 1) %(sizeof(luint) * 8) != 0)
    lwordsForFields++;
    
  __auiFieldBits = new luint[lwordsForFields];
  if (NULL == __auiFieldBits)
    ID3_THROW(ID3E_NoMemory);

  for (luint i = 0; i < lwordsForFields; i++)
    __auiFieldBits[i] = 0;
    
  SetID(id);
}


ID3_Frame::~ID3_Frame(void)
{
  Clear();
  
  if (__auiFieldBits != NULL)
    delete [] __auiFieldBits;
}


void ID3_Frame::Clear(void)
{
  if (__ulNumFields && __apFields)
  {
    for (luint i = 0; i < __ulNumFields; i++)
      delete __apFields[i];
      
    delete [] __apFields;
    
    __apFields = NULL;
    __ulNumFields = 0;
    __bHasChanged = true;
  }
  
  return ;
}


void ID3_Frame::SetID(ID3_FrameID id)
{
  ID3_FrameDef *info;
  
  Clear();
  
  if (id != ID3FID_NOFRAME)
  {
    info = ID3_FindFrameDef(id);
    if (NULL == info)
      ID3_THROW(ID3E_InvalidFrameID);

    __FrameID = id;
      
    __ulNumFields = 0;
      
    while(info->aeFieldDefs[__ulNumFields].eID != ID3FN_NOFIELD)
      __ulNumFields++;
      
    __apFields = new ID3_Field * [__ulNumFields];
    if (NULL == __apFields)
      ID3_THROW(ID3E_NoMemory);

    for (luint i = 0; i < __ulNumFields; i++)
    {
      __apFields[i] = new ID3_Field;
      if (NULL == __apFields[i])
        ID3_THROW(ID3E_NoMemory);

      __apFields[i]->__eName        = info->aeFieldDefs[i].eID;
      __apFields[i]->__eType        = info->aeFieldDefs[i].eType;
      __apFields[i]->__lFixedLength = info->aeFieldDefs[i].lFixedLength;
      __apFields[i]->__ucIOVersion  = info->aeFieldDefs[i].ucVersion;
      __apFields[i]->__ucIORevision = info->aeFieldDefs[i].ucRevision;
      __apFields[i]->__eControl     = info->aeFieldDefs[i].eControl;
      __apFields[i]->__ulFlags      = info->aeFieldDefs[i].ulFlags;
            
      // tell the frame that this field is present
      BS_SET(__auiFieldBits, __apFields[i]->__eName);
    }
        
    __bHasChanged = true;
  }
  
  return ;
}


ID3_FrameID ID3_Frame::GetID(void) const
{
  return __FrameID;
}


void ID3_Frame::SetVersion(uchar ver, uchar rev)
{
  if (__ucVersion != ver || __ucRevision != rev)
    __bHasChanged = true;
    
  __ucVersion  = ver;
  __ucRevision = rev;
  
  return ;
}

lsint ID3_Frame::FindField(ID3_FieldID fieldName) const
{
  
  if (BS_ISSET(__auiFieldBits, fieldName))
  {
    for (lsint num = 0; num < (lsint) __ulNumFields; num++)
      if (__apFields[num]->__eName == fieldName)
        return num;
  }

  return -1;
}

ID3_Field& ID3_Frame::Field(ID3_FieldID fieldName) const
{
  lsint fieldNum = FindField(fieldName);
  
  if (fieldNum < 0)
    ID3_THROW(ID3E_FieldNotFound);
    
  return *__apFields[fieldNum];
}

void ID3_Frame::UpdateFieldDeps(void)
{
  for (luint i = 0; i < __ulNumFields; i++)
  {
    if (__apFields[i]->__ulFlags & ID3FF_ADJUSTEDBY)
    {
      switch(__apFields[i]->__eType)
      {
        case ID3FTY_BITFIELD:
        {
          //luint value = 0;
          
          // now find the field on which this field is dependent and get a
          // copy of the value of that field.  then adjust the fixedLength of
          // this field to that value / 8.
          break;
        }
        
        default:
          break;
      }
    }
  }
  
  return ;
}


void ID3_Frame::UpdateStringTypes(void)
{
  for (luint i = 0; i < __ulNumFields; i++)
  {
    if (__apFields[i]->__ulFlags & ID3FF_ADJUSTENC)
    {
      ID3_TextEnc enc;
      ID3_FieldType newType;
      
      enc = (ID3_TextEnc) Field(ID3FN_TEXTENC).Get();
      
      switch(enc)
      {
        case ID3TE_ASCII:
          newType = ID3FTY_ASCIISTRING;
          break;
          
        case ID3TE_UNICODE:
          newType = ID3FTY_UNICODESTRING;
          break;
          
        default:
          newType = ID3FTY_ASCIISTRING;
          break;
      }
      
      __apFields[i]->__eType = newType;
    }
  }
  
  return ;
}


luint ID3_Frame::Size(void)
{
  luint bytesUsed = 0;
  //float factor = 1.0;
  ID3_FrameHeader header;
  
  header.SetVersion(__ucVersion, __ucRevision);
  bytesUsed += header.Size();
  
  if (strlen(__sEncryptionID))
    bytesUsed++;
    
  if (strlen(__sGroupingID))
    bytesUsed++;
    
  // this call is to tell the string fields what they should be rendered/parsed
  // as (ASCII or Unicode)
  UpdateStringTypes();
  
  for (luint i = 0; i < __ulNumFields; i++)
  {
    __apFields[i]->SetVersion(__ucVersion, __ucRevision);
    bytesUsed += __apFields[i]->BinSize();
  }
  
  return bytesUsed;
}


bool ID3_Frame::HasChanged(void) const
{
  bool changed = __bHasChanged;
  
  if (! changed)
  {
    for (luint i = 0; i < __ulNumFields && !changed; i++)
    {
      changed = __apFields[i]->HasChanged();
    }
  }
  
  return changed;
}

// $Log$
// Revision 1.5  1999/11/15 20:17:04  scott
// Added include for config.h.  Minor code cleanup.  Removed
// assignments from if checks; first makes assignment, then checks
// for appropriate value.  Made private member variable names more
// descriptive.
//
// Revision 1.4  1999/11/04 04:15:54  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
