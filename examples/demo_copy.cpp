// $Id$
//
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

#include <iostream.h>
#include <id3/tag.h>
#include <popt.h>

static const char* VERSION_NUMBER = "$Revision$";

void PrintUsage(const char *sName)
{
  cout << "Usage: " << sName << " [OPTION]... SOURCE DEST" << endl;
  cout << "Copy the tag(s) from SOURCE to DEST." << endl;
  cout << endl;
  cout << "  -1, --v1tag     Render only the id3v1 tag" << endl;
  cout << "  -2, --v2tag     Render only the id3v2 tag" << endl;
  cout << "  -h, --help      Display this help and exit" << endl;
  cout << "  -v, --version   Display version information and exit" << endl;
  cout << endl;
}

void PrintVersion(const char *sName)
{
  size_t nIndex;
  cout << sName << " ";
  for (nIndex = 0; nIndex < strlen(VERSION_NUMBER); nIndex++)
  {
    if (VERSION_NUMBER[nIndex] == ' ') 
    {
      break;
    }
  }
  nIndex++;
  for (; nIndex < strlen (VERSION_NUMBER); nIndex++)
  {
    if (VERSION_NUMBER[nIndex] == ' ') 
    {
      break;
    }
    cout << VERSION_NUMBER[nIndex];
  }
  cout << endl;
  cout << "Uses " << ID3LIB_FULL_NAME << endl << endl;

  cout << "This program copies tags from one file to another" << endl;
}

void DisplayTags(ostream &os, luint nTags)
{
  if (!((nTags & ID3TT_ID3V1) || (nTags & ID3TT_ID3V2)))
  {
    os << "no tag";
  }
  if (nTags & ID3TT_ID3V1)
  {
    os << "v1";
  }
  if ((nTags & ID3TT_ID3V1) && (nTags & ID3TT_ID3V2))
  {
    os << " and ";
  }
  if (nTags & ID3TT_ID3V2)
  {
    os << "v2";
  }
}

int main( unsigned int argc, const char *argv[])
{
  const char* appname = argv[0];
  int ret = 0;
  int ulFlag = ID3TT_ID3;
  bool bVersion = false;
  bool bHelp = false;
  static struct poptOption options[] =
  {
    { "v1tag",   '1', POPT_ARG_NONE, NULL,      ID3TT_ID3V1 },
    { "v2tag",   '2', POPT_ARG_NONE, NULL,      ID3TT_ID3V2 },
    { "version", 'v', POPT_ARG_NONE, &bVersion, 0 },
    { "help",    'h', POPT_ARG_NONE, &bHelp,    0 },
    { NULL,      0,   0,             NULL,      0 }
  };
  poptContext con = poptGetContext(NULL, argc, argv, options, 0);
  int rc = 0;
  while ((rc = poptGetNextOpt(con)) > 0)
  {
    ulFlag = rc;
  }
  if (rc < -1)
  {
    cerr << poptBadOption(con, POPT_BADOPTION_NOALIAS) << ": "
         << poptStrerror(rc) << endl;
    
    cout << "Try `" << appname << " --help' for more information." << endl;
    ret = -rc;
  }
  else if (bHelp)
  {
    PrintVersion(appname);
    PrintUsage(appname);
  }
  else if (bVersion)
  {
    PrintVersion(appname);
  }
  else
  {
    const char* filename = NULL;
    while((filename = poptGetArg(con)) != NULL)
    {
      try
      {
        ID3_Tag myTag;

        cout << "Parsing " << filename << ": ";

        myTag.Clear();
        myTag.Link(filename, ID3TT_ALL);

        cout << "done.  Copying to " << filename << ": ";
        
        myTag.Link(filename, ID3TT_NONE);
        myTag.Update(ulFlag);
        
        cout << "done" << endl;
        cerr << "numframes = " << myTag.NumFrames() << endl;
      }

      catch(ID3_Error& err)
      {
        cout << endl;
        cout << err.GetErrorFile() << " (" << err.GetErrorLine() << "): "
             << err.GetErrorType() << ": " << err.GetErrorDesc() << endl;
        ret = 1;
        break;
      }
    }
  }

  poptFreeContext(con);
  return ret;
}


