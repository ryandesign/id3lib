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
#include "field.h"

static ID3_FieldDef ID3FD_URL[] =
{ 
  {
    ID3FN_URL,                          // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  { ID3FN_NOFIELD }
};
  
static ID3_FieldDef ID3FD_UserURL[] =
{
  {
    ID3FN_TEXTENC,                      // FIELD NAME        
    ID3FTY_INTEGER,                     // FIELD TYPE
    1, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_DESCRIPTION,                  // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NULL | ID3FF_ADJUSTENC,       // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_URL,                          // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  { ID3FN_NOFIELD }
};
  
static ID3_FieldDef ID3FD_Text[] =
{
  {
    ID3FN_TEXTENC,                      // FIELD NAME
    ID3FTY_INTEGER,                     // FIELD TYPE
    1, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_TEXT,                         // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_ADJUSTENC,                    // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  { ID3FN_NOFIELD }
};
  
  
static ID3_FieldDef ID3FD_UserText[] =
{
  {
    ID3FN_TEXTENC,                      // FIELD NAME
    ID3FTY_INTEGER,                     // FIELD TYPE
    1, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_DESCRIPTION,                  // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NULL | ID3FF_ADJUSTENC,       // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_TEXT,                         // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_ADJUSTENC,                    // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  { ID3FN_NOFIELD }
};
  
  
static ID3_FieldDef ID3FD_GeneralText[] =
{
  {
    ID3FN_TEXTENC,                      // FIELD NAME
    ID3FTY_INTEGER,                     // FIELD TYPE
    1, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_LANGUAGE,                     // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    3, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_DESCRIPTION,                  // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NULL | ID3FF_ADJUSTENC,       // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_TEXT,                         // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_ADJUSTENC,                    // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  { ID3FN_NOFIELD }
};

static ID3_FieldDef ID3FD_TermsOfUse[] =
{
  {
    ID3FN_TEXTENC,                      // FIELD NAME
    ID3FTY_INTEGER,                     // FIELD TYPE
    1, 3, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_LANGUAGE,                     // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    3, 3, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_TEXT,                         // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 3, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_ADJUSTENC,                    // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  { ID3FN_NOFIELD }
};

static ID3_FieldDef ID3FD_LinkedInfo[] =
{
  {
    ID3FN_DATA,                         // FIELD NAME
    ID3FTY_INTEGER,                     // FIELD TYPE
    3, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_LOWER,                        // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_TEXT,                         // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    3, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_LOWER,                        // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_TEXT,                         // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    4, 3, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_URL,                          // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NULL,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_TEXT,                         // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  { ID3FN_NOFIELD }
};

static ID3_FieldDef ID3FD_Picture[] =
{
  {
    ID3FN_TEXTENC,                      // FIELD NAME
    ID3FTY_INTEGER,                     // FIELD TYPE
    1, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_IMAGEFORMAT,                  // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    3, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_LOWER,                        // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_MIMETYPE,                     // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 3, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NULL,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_PICTURETYPE,                  // FIELD NAME
    ID3FTY_INTEGER,                     // FIELD TYPE
    1, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_DESCRIPTION,                  // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NULL | ID3FF_ADJUSTENC,       // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_DATA,                         // FIELD NAME
    ID3FTY_BINARY,                      // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  { ID3FN_NOFIELD }
};
  
static ID3_FieldDef ID3FD_GEO[] =
{
  {
    ID3FN_TEXTENC,                      // FIELD NAME
    ID3FTY_INTEGER,                     // FIELD TYPE
    1, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_MIMETYPE,                     // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NULL,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_FILENAME,                     // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NULL | ID3FF_ADJUSTENC,       // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_DESCRIPTION,                  // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NULL | ID3FF_ADJUSTENC,       // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_DATA,                         // FIELD NAME
    ID3FTY_BINARY,                      // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  { ID3FN_NOFIELD }
};
  
static ID3_FieldDef ID3FD_UFI[] =
{
  {
    ID3FN_OWNER,                        // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NULL,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_DATA,                         // FIELD NAME
    ID3FTY_BINARY,                      // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  { ID3FN_NOFIELD }
};
  
static ID3_FieldDef ID3FD_PlayCounter[] =
{
  {
    ID3FN_COUNTER,                      // FIELD NAME
    ID3FTY_INTEGER,                     // FIELD TYPE
    4, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  { ID3FN_NOFIELD }
};
  
static ID3_FieldDef ID3FD_Popularimeter[] =
{
  {
    ID3FN_EMAIL,                        // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NULL,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_RATING,                       // FIELD NAME
    ID3FTY_INTEGER,                     // FIELD TYPE
    1, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_COUNTER,                      // FIELD NAME
    ID3FTY_INTEGER,                     // FIELD TYPE
    4, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  { ID3FN_NOFIELD }
};
  
static ID3_FieldDef ID3FD_Registration[] =
{
  {
    ID3FN_OWNER,                        // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 3, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NULL,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_SYMBOL,                       // FIELD NAME
    ID3FTY_INTEGER,                     // FIELD TYPE
    1, 3, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_DATA,                         // FIELD NAME
    ID3FTY_BINARY,                      // FIELD TYPE
    -1, 3, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  { ID3FN_NOFIELD }
};
  
static ID3_FieldDef ID3FD_InvolvedPeople[] =
{
  {
    ID3FN_TEXTENC,                      // FIELD NAME
    ID3FTY_INTEGER,                     // FIELD TYPE
    1, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_TEXT,                         // FIELD NAME
    ID3FTY_ASCIISTRING,                 // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NULL | ID3FF_NULLDIVIDE | ID3FF_ADJUSTENC, // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  { ID3FN_NOFIELD }
};
/*
 * Currently unused
 */
#if defined __UNDEFINED__
static ID3_FieldDef ID3FD_Volume[] =
{
  {
    ID3FN_VOLUMEADJ,                    // FIELD NAME
    ID3FTY_INTEGER,                     // FIELD TYPE
    1, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_NUMBITS,                      // FIELD NAME
    ID3FTY_INTEGER,                     // FIELD TYPE
    1, 2, 0,                            // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_NONE,                         // FLAGS
    ID3FN_NOFIELD                       // LINKED FIELD
  },
  {
    ID3FN_VOLCHGRIGHT,                  // FIELD NAME
    ID3FTY_BITFIELD,                    // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_ADJUSTEDBY,                   // FLAGS
    ID3FN_NUMBITS                       // LINKED FIELD
  },
  {
    ID3FN_VOLCHGLEFT,                   // FIELD NAME
    ID3FTY_BITFIELD,                    // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_ADJUSTEDBY,                   // FLAGS
    ID3FN_NUMBITS                       // LINKED FIELD
  },
  {
    ID3FN_PEAKVOLRIGHT,                 // FIELD NAME
    ID3FTY_BITFIELD,                    // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_ADJUSTEDBY,                   // FLAGS
    ID3FN_NUMBITS                       // LINKED FIELD
  },
  {
    ID3FN_PEAKVOLLEFT,                  // FIELD NAME
    ID3FTY_BITFIELD,                    // FIELD TYPE
    -1, 2, 0,                           // FIXED LEN, VERSION, REVISION
    ID3VC_HIGHER,                       // VERSION CONTROL SCOPE
    ID3FF_ADJUSTEDBY,                   // FLAGS
    ID3FN_NUMBITS                       // LINKED FIELD
  },
  { ID3FN_NOFIELD }
};
#endif /* __UNEFINED__ */
  

// *** Currently Defined Frames
// APIC  Attached picture
// COMM  Comments
// ENCR  Encryption method registration
// GEOB  General encapsulated object
// GRID  Group identification registration
// IPLS  Involved people list
// LINK  Linked information
// PCNT  Play counter
// POPM  Popularimeter
// TALB  Album/Movie/Show title
// TBPM  BPM (beats per minute)
// TCOM  Composer
// TCON  Content type
// TCOP  Copyright message
// TDAT  Date
// TDLY  Playlist delay
// TENC  Encoded by
// TEXT  Lyricist/Text writer
// TFLT  File type
// TKEY  Initial key
// TIME  Time
// TIT1  Content group description
// TIT2  Title/songname/content description
// TIT3  Subtitle/Description refinement
// TLAN  Language(s)
// TLEN  Length
// TMED  Media type
// TOAL  Original album/movie/show title
// TOFN  Original filename
// TOLY  Original lyricist(s)/text writer(s)
// TOPE  Original artist(s)/performer(s)
// TORY  Original release year
// TOWN  File owner/licensee
// TPE1  Lead performer(s)/Soloist(s)
// TPE2  Band/orchestra/accompaniment
// TPE3  Conductor/performer refinement
// TPE4  Interpreted, remixed, or otherwise modified by
// TPOS  Part of a set
// TPUB  Publisher
// TRCK  Track number/Position in set
// TRDA  Recording dates
// TRSN  Internet radio station name
// TRSO  Internet radio station owner
// TSIZ  Size
// TSRC  ISRC (international standard recording code)
// TSSE  Software/Hardware and settings used for encoding
// TYER  Year
// TXXX  User defined text information frame
// UFID  Unique file identifier
// USER  Terms of use
// USLT  Unsychronized lyric/text transcription
// WCOM  Commercial information
// WCOP  Copyright/Legal information
// WOAF  Official audio file webpage
// WOAR  Official artist/performer webpage
// WOAS  Official audio source webpage
// WORS  Official internet radio station homepage
// WPAY  Payment
// WPUB  Publishers official webpage
// WXXX  User defined URL link frame

// *** Currently Undefined Frames
// AENC  Audio encryption
// COMR  Commercial frame
// EQUA  Equalization
// ETCO  Event timing codes
// MCDI  Music CD identifier
// MLLT  MPEG location lookup table
// OWNE  Ownership frame
// PRIV  Private frame
// POSS  Position synchronisation frame
// RBUF  Recommended buffer size
// RVAD  Relative volume adjustment
// RVRB  Reverb
// SYLT  Synchronized lyric/text
// SYTC  Synchronized tempo codes


static  ID3_FrameDef ID3_FrameDefs[] =
{
  //                          short  long      tag    file
  // frame id                 id     id    pri discrd discrd handler field defs
  {ID3FID_PICTURE,           "PIC", "APIC", 0, false, false, NULL, ID3FD_Picture},
  {ID3FID_COMMENT,           "COM", "COMM", 0, false, false, NULL, ID3FD_GeneralText},
  {ID3FID_CRYPTOREG,         "   ", "ENCR", 0, false, false, NULL, ID3FD_Registration},
  {ID3FID_GENERALOBJECT,     "GEO", "GEOB", 0, false, false, NULL, ID3FD_GEO},
  {ID3FID_GROUPINGREG,       "   ", "GRID", 0, false, false, NULL, ID3FD_Registration},
  {ID3FID_INVOLVEDPEOPLE,    "IPL", "IPLS", 0, false, false, NULL, ID3FD_InvolvedPeople},
  {ID3FID_LINKEDINFO,        "LNK", "LINK", 0, false, false, NULL, ID3FD_LinkedInfo},
  {ID3FID_PLAYCOUNTER,       "CNT", "PCNT", 0, false, false, NULL, ID3FD_PlayCounter},
  {ID3FID_POPULARIMETER,     "POP", "POPM", 0, false, false, NULL, ID3FD_Popularimeter},
  {ID3FID_ALBUM,             "TAL", "TALB", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_BEATSPERMINUTE,    "TBP", "TBPM", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_COMPOSER,          "TCM", "TCOM", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_CONTENTTYPE,       "TCO", "TCON", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_COPYRIGHT,         "TCR", "TCOP", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_DATE,              "TDA", "TDAT", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_PLAYLISTDELAY,     "TDY", "TDLY", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_ENCODEDBY,         "TEN", "TENC", 0, false, true,  NULL, ID3FD_Text},
  {ID3FID_LYRICIST,          "TXT", "TEXT", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_FILETYPE,          "TFT", "TFLT", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_KEY,               "TKE", "TKEY", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_TIME,              "TIM", "TIME", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_CONTENTGROUP,      "TT1", "TIT1", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_TITLE,             "TT2", "TIT2", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_SUBTITLE,          "TT3", "TIT3", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_LANGUAGE,          "TLA", "TLAN", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_SONGLEN,           "TLE", "TLEN", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_MEDIATYPE,         "TMT", "TMED", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_ORIGALBUM,         "TOT", "TOAL", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_ORIGFILENAME,      "TOF", "TOFN", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_ORIGLYRICIST,      "TOL", "TOLY", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_ORIGARTIST,        "TOA", "TOPE", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_ORIGYEAR,          "TOR", "TORY", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_OWNER,             "   ", "TOWN", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_LEADARTIST,        "TP1", "TPE1", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_BAND,              "TP2", "TPE2", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_CONDUCTOR,         "TP3", "TPE3", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_MIXARTIST,         "TP4", "TPE4", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_PARTINSET,         "TPA", "TPOS", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_PUBLISHER,         "TPB", "TPUB", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_TRACKNUM,          "TRK", "TRCK", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_RECORDINGDATES,    "TRD", "TRDA", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_NETRADIOSTATION,   "TRN", "TRSN", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_NETRADIOOWNER,     "TRO", "TRSO", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_SIZE,              "TSI", "TSIZ", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_ISRC,              "TRC", "TSRC", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_ENCODERSETTINGS,   "TSS", "TSSE", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_USERTEXT,          "TXX", "TXXX", 0, false, false, NULL, ID3FD_UserText},
  {ID3FID_YEAR,              "TYE", "TYER", 0, false, false, NULL, ID3FD_Text},
  {ID3FID_UNIQUEFILEID,      "UFI", "UFID", 0, false, false, NULL, ID3FD_UFI},
  {ID3FID_TERMSOFUSE,        "   ", "USER", 0, false, false, NULL, ID3FD_TermsOfUse},
  {ID3FID_UNSYNCEDLYRICS,    "ULT", "USLT", 0, false, false, NULL, ID3FD_GeneralText},
  {ID3FID_WWWCOMMERCIALINFO, "WCM", "WCOM", 0, false, false, NULL, ID3FD_URL},
  {ID3FID_WWWCOPYRIGHT,      "WCP", "WCOP", 0, false, false, NULL, ID3FD_URL},
  {ID3FID_WWWAUDIOFILE,      "WAF", "WOAF", 0, false, false, NULL, ID3FD_URL},
  {ID3FID_WWWARTIST,         "WAR", "WOAR", 0, false, false, NULL, ID3FD_URL},
  {ID3FID_WWWAUDIOSOURCE,    "WAS", "WOAS", 0, false, false, NULL, ID3FD_URL},
  {ID3FID_WWWRADIOPAGE,      "WRA", "WORS", 0, false, false, NULL, ID3FD_URL},
  {ID3FID_WWWPAYMENT,        "WPY", "WPAY", 0, false, false, NULL, ID3FD_URL},
  {ID3FID_WWWPUBLISHER,      "WPB", "WPUB", 0, false, false, NULL, ID3FD_URL},
  {ID3FID_WWWUSER,           "WXX", "WXXX", 0, false, false, NULL, ID3FD_UserURL},
  {ID3FID_NOFRAME}
};
  
ID3_Field::ID3_Field(void)
{
  __eName   = ID3FN_NOFIELD;
  __eType   = ID3FTY_INTEGER;
  __sData   = NULL;
  __ulSize  = 0;
  __ulFlags = 0;
  
  Clear();
}

ID3_Field::~ID3_Field(void)
{
  Clear();
}

void
ID3_Field::Clear(void)
{
  if (__sData != NULL && __ulSize > 0 && __eType != ID3FTY_INTEGER)
  {
    delete[] __sData;
  }
    
  __eType       = ID3FTY_INTEGER;
  __sData       = NULL;
  __ulSize      = sizeof (luint);
  __bHasChanged = true;
  
  return ;
}

void
ID3_Field::SetVersion(uchar ver, uchar rev)
{
  // If the version or revision has changed, set the hasChanged flag
  __bHasChanged = (__bHasChanged || __ucVersion != ver || __ucRevision != rev);

  __ucVersion = ver;
  __ucRevision = rev;
  
  return ;
}

bool
ID3_Field::HasChanged(void)
{
  return __bHasChanged;
}

luint
ID3_Field::Size(void)
{
  return BinSize(false);
}

luint
ID3_Field::BinSize(bool withExtras)
{
  luint bytes   = 0;

  if ((__eControl == ID3VC_HIGHER &&
       __ucVersion >= __ucIOVersion && __ucRevision >= __ucIORevision) ||
      (__ucVersion <= __ucIOVersion && __ucRevision <= __ucIORevision))
  {
    bytes = __ulSize;
    
    // check to see if we are within the legal limit for this field -1 means
    // arbitrary length field
    if (__lFixedLength != -1)
    {
      bytes = __lFixedLength;
    }
    else if (withExtras)
    {
      if (NULL == __sData && __ulSize > 0)
      {
        bytes = (__ulFlags & ID3FF_NULL) ? sizeof(unicode_t) : 0;
      }
      
      // if we are a Unicode string, add 2 bytes for the BOM (but only if there
      // is a string to render - regardless of NULL)
      if (__eType == ID3FTY_UNICODESTRING && __sData != NULL && __ulSize > 0)
      {
        bytes += sizeof(unicode_t);
      }
        
      // if we are an ASCII string, divide by sizeof(unicode_t) because
      // internally we store the string as Unicode, so the ASCII version will
      // only be half as long
      if (__eType == ID3FTY_ASCIISTRING)
      {
        bytes /= sizeof(unicode_t);
      }
    }
    else if (__eType == ID3FTY_UNICODESTRING || __eType == ID3FTY_ASCIISTRING)
    {
      // because it seems that the application called us via ID3_Field::Size()
      // we are going to return the number of characters, not bytes.  since we
      // store every string internally as unicode, we will divide the 'bytes'
      // variable by the size of a unicode character (should be two bytes)
      // because Unicode strings have twice as many bytes as they do characters
      bytes /= sizeof(unicode_t);
    }
  }
  
  return bytes;
}

luint
ID3_Field::Parse(const uchar *buffer, const luint posn, const luint buffSize)
{
  luint bytesUsed       = 0;

  if ((__eControl == ID3VC_HIGHER &&
       __ucVersion >= __ucIOVersion && __ucRevision >= __ucIORevision) ||
      (__ucVersion <= __ucIOVersion && __ucRevision <= __ucIORevision))
  {
    switch (__eType)
    {
      case ID3FTY_INTEGER:
        bytesUsed = ParseInteger(buffer, posn, buffSize);
        break;
        
      case ID3FTY_BINARY:
        bytesUsed = ParseBinary( buffer, posn, buffSize);
        break;
        
      case ID3FTY_ASCIISTRING:
        bytesUsed = ParseASCIIString(buffer, posn, buffSize);
        break;
        
      case ID3FTY_UNICODESTRING:
        bytesUsed = ParseUnicodeString(buffer, posn, buffSize);
        break;
        
      default:
        ID3_THROW(ID3E_UnknownFieldType);
        break;
    }
  }
  
  return bytesUsed;
}

ID3_FrameDef *
ID3_FindFrameDef(const ID3_FrameID id)
{
  ID3_FrameDef  *info   = NULL;

  for (luint cur = 0; ID3_FrameDefs[cur].eID != ID3FID_NOFRAME; cur++)
  {
    if (ID3_FrameDefs[cur].eID == id)
    {
      info = &ID3_FrameDefs[cur];
      break;
    }
  }
    
  return info;
}

ID3_FrameID
ID3_FindFrameID(const char *id)
{
  ID3_FrameID fid = ID3FID_NOFRAME;
  
  for (luint cur = 0; ID3_FrameDefs[cur].eID != ID3FID_NOFRAME; cur++)
  {
    if (((strcmp(ID3_FrameDefs[cur].sShortTextID, id) == 0) &&
         strlen(id) == 3) ||
        ((strcmp(ID3_FrameDefs[cur].sLongTextID,  id) == 0) &&
         strlen(id) == 4))
    {
      fid = ID3_FrameDefs[cur].eID;
      break;
    }
  }
  
  return fid;
}

luint
ID3_Field::Render(uchar *buffer)
{
  luint bytesUsed = 0;
  
  if ((__eControl == ID3VC_HIGHER &&
       __ucVersion >= __ucIOVersion && __ucRevision >= __ucIORevision) ||
      (__ucVersion <= __ucIOVersion && __ucRevision <= __ucIORevision))
  {
    switch (__eType) 
    {
      case ID3FTY_INTEGER:
        bytesUsed = RenderInteger(buffer);
        break;
        
      case ID3FTY_BINARY:
        bytesUsed = RenderBinary(buffer);
        break;
        
      case ID3FTY_ASCIISTRING:
        bytesUsed = RenderASCIIString(buffer);
        break;
        
      case ID3FTY_UNICODESTRING:
        bytesUsed = RenderUnicodeString(buffer);
        break;
        
      default:
        ID3_THROW (ID3E_UnknownFieldType);
        break;
    }
  }
    
  return bytesUsed;
}

ID3_Field &
ID3_Field::operator=( const ID3_Field &rField )
{
  if (this != &rField)
  {
    if ((__eControl == ID3VC_HIGHER &&
         __ucVersion >= __ucIOVersion && __ucRevision >= __ucIORevision) ||
        (__ucVersion <= __ucIOVersion && __ucRevision <= __ucIORevision))
    {
      switch (__eType)
      {
        case ID3FTY_INTEGER:
          *this = rField.Get();
          break;
        case ID3FTY_BITFIELD:
        case ID3FTY_ASCIISTRING:
        case ID3FTY_UNICODESTRING:
        case ID3FTY_BINARY:
          Set(rField.__sData, rField.__ulSize);
          break;
      }
    }
  }
}

// $Log$
// Revision 1.11  1999/12/09 03:31:58  scott
// (): Better comments of the ID3VC_HIGHER/ID3VC_LOWER section in the
// field defs.
// (operator=): Added implementation.
//
// Revision 1.10  1999/12/01 18:00:59  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places (and to make compiling under
// windows easier).
//
// Revision 1.9  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.8  1999/11/29 17:21:06  scott
// Updated methods to use unicode_t instead of wchar_t.  unicode_t is
// defined in globals.h.  The type unicode_t is more likely to be two
// bytes on every platform.
//
// Revision 1.7  1999/11/19 17:18:49  scott
// * field.cpp
// (BinSize): Added check for an ascii string when called by Size, since
// we store every string internally as unicode.
// (Parse): Updated interface to make parameters const.
// (ID3_FindFrameDef): Updated interface to make parameter const.
// (ID3_FindFrameID): Updated interface to make parameter const.
//
// Revision 1.6  1999/11/16 22:50:19  scott
// * field.cpp (Render): Minor reformatting.
//
// Revision 1.5  1999/11/15 20:15:25  scott
// Added include for config.h.  Replaced LU_NULL with ID3FF_NONE for
// more consistency in flag naming.  Blocked out the ID3FD_Volume
// array definition since it is currently unused.  Reformatted
// ID3_FrameDefs for easier reading (although most lines are more
// than 80 chars long now).  Made private member variable names more
// descriptive.  Fixed bug in SetVersion (was checking "rev != rev").
// Adjusted logic somewhat in BinSize to ease code reading.  Fixed
// bug in BinSize that was causing incorrect parsing of correct
// frames, such as COMM (previously assumed a wchar_t is 2 bytes,
// which might not be true on some platforms).
//
// Revision 1.4  1999/11/04 04:15:54  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
