// Copyright 1999 Scott Thomas Haug <sth2@cs.wustl.edu>
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//  
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//  
// $Id$


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <id3/debug.h>

#include <iostream.h>
#include <id3/tag.h>
#include <id3/utils.h>
#include <id3/misc_support.h>
#include <id3/readers.h>
#include <id3/reader_decorators.h>
#include <id3/error.h>

#include "demo_info_options.h"

using namespace dami;

static String VERSION_NUMBER = "$Revision$";

void PrintUsage(const char *sName)
{
  cout << "Usage: " << sName << " [OPTION]... [FILE]..." << endl;
  cout << "Display the id3 (both v1 and v2) tag information for a file." << endl;
  cout << endl;
  cout << "  -h, --help      Display this help and exit" << endl;
  cout << "  -v, --version   Display version information and exit" << endl;
  cout << endl;
  cout << "Will not differentiate between the two types of tags" << endl;
}

void PrintVersion(const char *sName)
{
  cout << sName << " " << VERSION_NUMBER << endl;
  cout << "Displays ID3 Tag Information - Written by Scott Thomas Haug" << endl;
  cout << "Uses " << ID3LIB_FULL_NAME << endl << endl;
}

void PrintInformation(const ID3_Tag &myTag)
{
  for (size_t nFrames = 0; nFrames < myTag.NumFrames(); nFrames++)
  {
    ID3_Frame *myFrame = myTag[nFrames];
    if (NULL != myFrame)
    { 
      const char* desc = myFrame->GetDescription();
      if (!desc) desc = "";
      cout << "=== " << myFrame->GetTextID() << " (" << desc << "): ";
      ID3_FrameID eFrameID = myFrame->GetID();
      switch (eFrameID)
      {
        case ID3FID_ALBUM:
        case ID3FID_BPM:
        case ID3FID_COMPOSER:
        case ID3FID_CONTENTTYPE:
        case ID3FID_COPYRIGHT:
        case ID3FID_DATE:
        case ID3FID_PLAYLISTDELAY:
        case ID3FID_ENCODEDBY:
        case ID3FID_LYRICIST:
        case ID3FID_FILETYPE:
        case ID3FID_TIME:
        case ID3FID_CONTENTGROUP:
        case ID3FID_TITLE:
        case ID3FID_SUBTITLE:
        case ID3FID_INITIALKEY:
        case ID3FID_LANGUAGE:
        case ID3FID_SONGLEN:
        case ID3FID_MEDIATYPE:
        case ID3FID_ORIGALBUM:
        case ID3FID_ORIGFILENAME:
        case ID3FID_ORIGLYRICIST:
        case ID3FID_ORIGARTIST:
        case ID3FID_ORIGYEAR:
        case ID3FID_FILEOWNER:
        case ID3FID_LEADARTIST:
        case ID3FID_BAND:
        case ID3FID_CONDUCTOR:
        case ID3FID_MIXARTIST:
        case ID3FID_PARTINSET:
        case ID3FID_PUBLISHER:
        case ID3FID_TRACKNUM:
        case ID3FID_RECORDINGDATES:
        case ID3FID_NETRADIOSTATION:
        case ID3FID_NETRADIOOWNER:
        case ID3FID_SIZE:
        case ID3FID_ISRC:
        case ID3FID_ENCODERSETTINGS:
        case ID3FID_YEAR:
        {
          char *sText = ID3_GetString(myFrame, ID3FN_TEXT);
          cout << sText << endl;
          delete [] sText;
          break;
        }
        case ID3FID_USERTEXT:
        {
          char 
            *sText = ID3_GetString(myFrame, ID3FN_TEXT), 
            *sDesc = ID3_GetString(myFrame, ID3FN_DESCRIPTION);
          cout << "(" << sDesc << "): " << sText << endl;
          delete [] sText;
          delete [] sDesc;
          break;
        }
        case ID3FID_COMMENT:
        case ID3FID_UNSYNCEDLYRICS:
        {
          char 
            *sText = ID3_GetString(myFrame, ID3FN_TEXT), 
            *sDesc = ID3_GetString(myFrame, ID3FN_DESCRIPTION), 
            *sLang = ID3_GetString(myFrame, ID3FN_LANGUAGE);
          cout << "(" << sDesc << ")[" << sLang << "]: "
               << sText << endl;
          delete [] sText;
          delete [] sDesc;
          delete [] sLang;
          break;
        }
        case ID3FID_WWWAUDIOFILE:
        case ID3FID_WWWARTIST:
        case ID3FID_WWWAUDIOSOURCE:
        case ID3FID_WWWCOMMERCIALINFO:
        case ID3FID_WWWCOPYRIGHT:
        case ID3FID_WWWPUBLISHER:
        case ID3FID_WWWPAYMENT:
        case ID3FID_WWWRADIOPAGE:
        {
          char *sURL = ID3_GetString(myFrame, ID3FN_URL);
          cout << sURL << endl;
          delete [] sURL;
          break;
        }
        case ID3FID_WWWUSER:
        {
          char 
            *sURL = ID3_GetString(myFrame, ID3FN_URL),
            *sDesc = ID3_GetString(myFrame, ID3FN_DESCRIPTION);
          cout << "(" << sDesc << "): " << sURL << endl;
          delete [] sURL;
          delete [] sDesc;
          break;
        }
        case ID3FID_INVOLVEDPEOPLE:
        {
          size_t nItems = myFrame->GetField(ID3FN_TEXT)->GetNumTextItems();
          for (size_t nIndex = 0; nIndex < nItems; nIndex++)
          {
            char *sPeople = ID3_GetString(myFrame, ID3FN_TEXT, nIndex);
            cout << sPeople;
            delete [] sPeople;
            if (nIndex + 1 < nItems)
            {
              cout << ", ";
            }
          }
          cout << endl;
          break;
        }
        case ID3FID_PICTURE:
        {
          char
            *sMimeType = ID3_GetString(myFrame, ID3FN_MIMETYPE),
            *sDesc     = ID3_GetString(myFrame, ID3FN_DESCRIPTION),
            *sFormat   = ID3_GetString(myFrame, ID3FN_IMAGEFORMAT);
          size_t
            nPicType   = myFrame->GetField(ID3FN_PICTURETYPE)->Get(),
            nDataSize  = myFrame->GetField(ID3FN_DATA)->Size();
          cout << "(" << sDesc << ")[" << sFormat << ", "
               << nPicType << "]: " << sMimeType << ", " << nDataSize
               << " bytes" << endl;
          delete [] sMimeType;
          delete [] sDesc;
          delete [] sFormat;
          break;
        }
        case ID3FID_GENERALOBJECT:
        {
          char 
            *sMimeType = ID3_GetString(myFrame, ID3FN_TEXT), 
            *sDesc = ID3_GetString(myFrame, ID3FN_DESCRIPTION), 
            *sFileName = ID3_GetString(myFrame, ID3FN_FILENAME);
          size_t 
            nDataSize = myFrame->GetField(ID3FN_DATA)->Size();
          cout << "(" << sDesc << ")[" 
               << sFileName << "]: " << sMimeType << ", " << nDataSize
               << " bytes" << endl;
          delete [] sMimeType;
          delete [] sDesc;
          delete [] sFileName;
          break;
        }
        case ID3FID_UNIQUEFILEID:
        {
          char *sOwner = ID3_GetString(myFrame, ID3FN_TEXT);
          size_t nDataSize = myFrame->GetField(ID3FN_DATA)->Size();
          cout << sOwner << ", " << nDataSize
               << " bytes" << endl;
          delete [] sOwner;
          break;
        }
        case ID3FID_PLAYCOUNTER:
        {
          size_t nCounter = myFrame->GetField(ID3FN_COUNTER)->Get();
          cout << nCounter << endl;
          break;
        }
        case ID3FID_POPULARIMETER:
        {
          char *sEmail = ID3_GetString(myFrame, ID3FN_EMAIL);
          size_t
            nCounter = myFrame->GetField(ID3FN_COUNTER)->Get(),
            nRating = myFrame->GetField(ID3FN_RATING)->Get();
          cout << sEmail << ", counter=" 
               << nCounter << " rating=" << nRating << endl;
          delete [] sEmail;
          break;
        }
        case ID3FID_CRYPTOREG:
        case ID3FID_GROUPINGREG:
        {
          char *sOwner = ID3_GetString(myFrame, ID3FN_OWNER);
          size_t 
            nSymbol = myFrame->GetField(ID3FN_ID)->Get(),
            nDataSize = myFrame->GetField(ID3FN_DATA)->Size();
          cout << "(" << nSymbol << "): " << sOwner
               << ", " << nDataSize << " bytes" << endl;
          break;
        }
        case ID3FID_SYNCEDLYRICS:
        {
          char 
            *sDesc = ID3_GetString(myFrame, ID3FN_DESCRIPTION), 
            *sLang = ID3_GetString(myFrame, ID3FN_LANGUAGE);
          size_t
            nTimestamp = myFrame->GetField(ID3FN_TIMESTAMPFORMAT)->Get(),
            nRating = myFrame->GetField(ID3FN_CONTENTTYPE)->Get();
          const char* format = (2 == nTimestamp) ? "ms" : "frames";
          cout << "(" << sDesc << ")[" << sLang << "]: ";
          switch (nRating)
          {
            case ID3CT_OTHER:    cout << "Other"; break;
            case ID3CT_LYRICS:   cout << "Lyrics"; break;
            case ID3CT_TEXTTRANSCRIPTION:     cout << "Text transcription"; break;
            case ID3CT_MOVEMENT: cout << "Movement/part name"; break;
            case ID3CT_EVENTS:   cout << "Events"; break;
            case ID3CT_CHORD:    cout << "Chord"; break;
            case ID3CT_TRIVIA:   cout << "Trivia/'pop up' information"; break;
          }
          cout << endl;
          ID3_Field* fld = myFrame->GetField(ID3FN_DATA);
          if (fld)
          {
            ID3_MemoryReader mr(fld->GetBinary(), fld->BinSize());
            io::BinaryNumberReader bnr(mr);
            io::TextReader tr(mr);
            while (!mr.atEnd())
            {
              cout << tr.readText();
              cout << " [" << bnr.readNumber(sizeof(uint32)) << " " << format << "] ";
            }
          }
          cout << endl;
          delete [] sDesc;
          delete [] sLang;
          break;
        }
        case ID3FID_AUDIOCRYPTO:
        case ID3FID_EQUALIZATION:
        case ID3FID_EVENTTIMING:
        case ID3FID_CDID:
        case ID3FID_MPEGLOOKUP:
        case ID3FID_OWNERSHIP:
        case ID3FID_PRIVATE:
        case ID3FID_POSITIONSYNC:
        case ID3FID_BUFFERSIZE:
        case ID3FID_VOLUMEADJ:
        case ID3FID_REVERB:
        case ID3FID_SYNCEDTEMPO:
        case ID3FID_METACRYPTO:
        {
          cout << " (unimplemented)" << endl;
          break;
        }
        default:
        {
          cout << " frame" << endl;
          break;
        }
      }
    }
  }
}

#define DEBUG

int main( unsigned int argc, char * const argv[])
{
  ID3D_INIT_DOUT();

  gengetopt_args_info args;

  if (cmdline_parser(argc, argv, &args) != 0)
  {
    exit(1);
  }

#if defined ID3_ENABLE_DEBUG
  if (args.warning_given)
  {
    cout << "warnings turned on" << endl;
    ID3D_INIT_WARNING();
    ID3D_WARNING ( "warnings turned on" );
  }
  if (args.notice_given)
  {
    cout << "notices turned on" << endl;
    ID3D_INIT_NOTICE();
    ID3D_NOTICE ( "notices turned on" );
  }
#endif

  const char* filename = NULL;
  for (size_t i = 0; i < args.inputs_num; ++i)
  {
    filename = args.inputs[i];
    try
    {
      ID3_Tag myTag;
      
      myTag.Link(filename, ID3TT_ALL);
      cout << endl << "*** Tag information for " << filename << endl;
      if (!args.assign_given)
      {
        PrintInformation(myTag);
      }
      else
      {
        cout << "*** Testing assignment operator" << endl;
        ID3_Tag tmpTag(myTag);
        PrintInformation(tmpTag);
      }
    }
    catch(ID3_Error &err)
    {
      ID3D_WARNING( err.GetErrorFile() << " ("  << 
                    err.GetErrorLine() << "): " << 
                    err.GetErrorType() << ": "  << 
                    err.GetErrorDesc() );
    }
  }

  return 0;
}
