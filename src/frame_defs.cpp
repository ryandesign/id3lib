// id3lib: a C++ library for creating and manipulating id3v1/v2 tags
// $Id$

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

#include "enums.h"
#include "frame_header.h"
#include "field.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{
  // **** Currently Implemented Frames
  // APIC  PIC  frame::PICTURE           Attached picture
  // COMM  COM  frame::COMMENT           Comments
  // ENCR       frame::CRYPTOREG         Encryption method registration
  // GEOB  GEO  frame::GENERALOBJECT     General encapsulated object
  // GRID       frame::GROUPINGREG       Group identification registration
  // IPLS  IPL  frame::INVOLVEDPEOPLE    Involved people list
  // LINK  LNK  frame::LINKEDINFO        Linked information
  // PCNT  CNT  frame::PLAYCOUNTER       Play counter
  // POPM  POP  frame::POPULARIMETER     Popularimeter
  // TALB  TAL  frame::ALBUM             Album/Movie/Show title
  // TBPM  TBP  frame::BPM               BPM (beats per minute)
  // TCOM  TCM  frame::COMPOSER          Composer
  // TCON  TCO  frame::CONTENTTYPE       Content type
  // TCOP  TCR  frame::COPYRIGHT         Copyright message
  // TDAT  TDA  frame::DATE              Date
  // TDLY  TDY  frame::PLAYLISTDELAY     Playlist delay
  // TENC  TEN  frame::ENCODEDBY         Encoded by
  // TEXT  TXT  frame::LYRICIST          Lyricist/Text writer
  // TFLT  TFT  frame::FILETYPE          File type
  // TIME  TKE  frame::TIME              Time
  // TIT1  TIM  frame::CONTENTGROUP      Content group description
  // TIT2  TT1  frame::TITLE             Title/songname/content description
  // TIT3  TT2  frame::SUBTITLE          Subtitle/Description refinement
  // TKEY  TT3  frame::INITIALKEY        Initial key
  // TLAN  TLA  frame::LANGUAGE          Language(s)
  // TLEN  TLE  frame::SONGLEN           Length
  // TMED  TMT  frame::MEDIATYPE         Media type
  // TOAL  TOT  frame::ORIGALBUM         Original album/movie/show title
  // TOFN  TOF  frame::ORIGFILENAME      Original filename
  // TOLY  TOL  frame::ORIGLYRICIST      Original lyricist(s)/text writer(s)
  // TOPE  TOA  frame::ORIGARTIST        Original artist(s)/performer(s)
  // TORY  TOR  frame::ORIGYEAR          Original release year
  // TOWN       frame::FILEOWNER         File owner/licensee
  // TPE1  TP1  frame::LEADARTIST        Lead performer(s)/Soloist(s)
  // TPE2  TP2  frame::BAND              Band/orchestra/accompaniment
  // TPE3  TP3  frame::CONDUCTOR         Conductor/performer refinement
  // TPE4  TP4  frame::MIXARTIST         Interpreted, remixed, or otherwise modified
  // TPOS  TPA  frame::PARTINSET         Part of a set
  // TPUB  TPB  frame::PUBLISHER         Publisher
  // TRCK  TRK  frame::TRACKNUM          Track number/Position in set
  // TRDA  TRD  frame::RECORDINGDATES    Recording dates
  // TRSN  TRN  frame::NETRADIOSTATION   Internet radio station name
  // TRSO  TRO  frame::NETRADIOOWNER     Internet radio station owner
  // TSIZ  TSI  frame::SIZE              Size
  // TSRC  TRC  frame::ISRC              ISRC(international standard recording code)
  // TSSE  TSS  frame::ENCODERSETTINGS   Software/Hardware and encoding settings
  // TXXX  TXX  frame::USERTEXT          User defined text information
  // TYER  TYE  frame::YEAR              Year
  // UFID  UFI  frame::UNIQUEFILEID      Unique file identifier
  // USER       frame::TERMSOFUSE        Terms of use
  // USLT  ULT  frame::UNSYNCEDLYRICS    Unsynchronized lyric/text transcription
  // WCOM  WCM  frame::WWWCOMMERCIALINFO Commercial information
  // WCOP  WCM  frame::WWWCOPYRIGHT      Copyright/Legal infromation
  // WOAF  WCP  frame::WWWAUDIOFILE      Official audio file webpage
  // WOAR  WAF  frame::WWWARTIST         Official artist/performer webpage
  // WOAS  WAR  frame::WWWAUDIOSOURCE    Official audio source webpage
  // WORS  WAS  frame::WWWRADIOPAGE      Official internet radio station homepage
  // WPAY  WRA  frame::WWWPAYMENT        Payment
  // WPUB  WPY  frame::WWWPUBLISHER      Official publisher webpage
  // WXXX  WXX  frame::WWWUSER           User defined URL link

  // **** Currently unimplemented frames
  // AENC  CRA  frame::AUDIOCRYPTO       Audio encryption
  // COMR       frame::COMMERCIAL        Commercial frame
  // EQUA  EQU  frame::EQUALIZATION      Equalization
  // ETCO  ETC  frame::EVENTTIMING       Event timing codes
  // MCDI  MCI  frame::CDID              Music CD identifier
  // MLLT  MLL  frame::MPEGLOOKUP        MPEG location lookup table
  // OWNE       frame::OWNERSHIP         Ownership frame
  // POSS       frame::POSITIONSYNC      Position synchronisation frame
  // PRIV       frame::PRIVATE           Private frame
  // RBUF  BUF  frame::BUFFERSIZE        Recommended buffer size
  // RVAD  RVA  frame::VOLUMEADJ         Relative volume adjustment
  // RVRB  REV  frame::REVERB            Reverb
  // SYLT  SLT  frame::SYNCEDLYRICS      Synchronized lyric/text
  // SYTC  STC  frame::SYNCEDTEMPO       Synchronized tempo codes
  //       CRM  frame::METACRYPTO        Encrypted meta frame

  const frame_header::def frame_header::defs[] =
  {
    //                       short  long    tag    file
    // frame id              id     id      discrd discrd field defs
    {frm::AUDIOCRYPTO,       "CRA", "AENC", false, false, field::binary_data},
    {frm::PICTURE,           "PIC", "APIC", false, false, field::picture},
    {frm::COMMENT,           "COM", "COMM", false, false, field::general_text},
    {frm::COMMERCIAL,        "",    "COMR", false, false, field::binary_data},
    {frm::CRYPTOREG,         "",    "ENCR", false, false, field::registration},
    {frm::EQUALIZATION,      "EQU", "EQUA", false, true,  field::binary_data},
    {frm::EVENTTIMING,       "ETC", "ETCO", false, true,  field::binary_data},
    {frm::GENERALOBJECT,     "GEO", "GEOB", false, false, field::geo},
    {frm::GROUPINGREG,       "",    "GRID", false, false, field::registration},
    {frm::INVOLVEDPEOPLE,    "IPL", "IPLS", false, false, field::text_list},
    {frm::LINKEDINFO,        "LNK", "LINK", false, false, field::linked_info},
    {frm::CDID,              "MCI", "MCDI", false, false, field::binary_data},
    {frm::MPEGLOOKUP,        "MLL", "MLLT", false, true,  field::binary_data},
    {frm::OWNERSHIP,         "",    "OWNE", false, false, field::binary_data},
    {frm::PLAYCOUNTER,       "CNT", "PCNT", false, false, field::play_counter},
    {frm::POPULARIMETER,     "POP", "POPM", false, false, field::popularimeter},
    {frm::POSITIONSYNC,      "",    "POSS", false, true,  field::binary_data},
    {frm::PRIVATE,           "",    "PRIV", false, false, field::binary_data},
    {frm::BUFFERSIZE,        "BUF", "RBUF", false, false, field::binary_data},
    {frm::VOLUMEADJ,         "RVA", "RVAD", false, true,  field::binary_data},
    {frm::REVERB,            "REV", "RVRB", false, false, field::binary_data},
    {frm::SYNCEDLYRICS,      "SLT", "SYLT", false, true,  field::binary_data},
    {frm::SYNCEDTEMPO,       "STC", "SYTC", false, true,  field::binary_data},
    {frm::ALBUM,             "TAL", "TALB", false, false, field::text},
    {frm::BPM,               "TBP", "TBPM", false, false, field::text},
    {frm::COMPOSER,          "TCM", "TCOM", false, false, field::text},
    {frm::CONTENTTYPE,       "TCO", "TCON", false, false, field::text},
    {frm::COPYRIGHT,         "TCR", "TCOP", false, false, field::text},
    {frm::DATE,              "TDA", "TDAT", false, false, field::text},
    {frm::PLAYLISTDELAY,     "TDY", "TDLY", false, false, field::text},
    {frm::ENCODEDBY,         "TEN", "TENC", false, true,  field::text},
    {frm::LYRICIST,          "TXT", "TEXT", false, false, field::text},
    {frm::FILETYPE,          "TFT", "TFLT", false, false, field::text},
    {frm::INITIALKEY,        "TKE", "TKEY", false, false, field::text},
    {frm::TIME,              "TIM", "TIME", false, false, field::text},
    {frm::CONTENTGROUP,      "TT1", "TIT1", false, false, field::text},
    {frm::TITLE,             "TT2", "TIT2", false, false, field::text},
    {frm::SUBTITLE,          "TT3", "TIT3", false, false, field::text},
    {frm::LANGUAGE,          "TLA", "TLAN", false, false, field::text},
    {frm::SONGLEN,           "TLE", "TLEN", false, true,  field::text},
    {frm::MEDIATYPE,         "TMT", "TMED", false, false, field::text},
    {frm::ORIGALBUM,         "TOT", "TOAL", false, false, field::text},
    {frm::ORIGFILENAME,      "TOF", "TOFN", false, false, field::text},
    {frm::ORIGLYRICIST,      "TOL", "TOLY", false, false, field::text},
    {frm::ORIGARTIST,        "TOA", "TOPE", false, false, field::text},
    {frm::ORIGYEAR,          "TOR", "TORY", false, false, field::text},
    {frm::FILEOWNER,         "",    "TOWN", false, false, field::text},
    {frm::LEADARTIST,        "TP1", "TPE1", false, false, field::text},
    {frm::BAND,              "TP2", "TPE2", false, false, field::text},
    {frm::CONDUCTOR,         "TP3", "TPE3", false, false, field::text},
    {frm::MIXARTIST,         "TP4", "TPE4", false, false, field::text},
    {frm::PARTINSET,         "TPA", "TPOS", false, false, field::text},
    {frm::PUBLISHER,         "TPB", "TPUB", false, false, field::text},
    {frm::TRACKNUM,          "TRK", "TRCK", false, false, field::text},
    {frm::RECORDINGDATES,    "TRD", "TRDA", false, false, field::text},
    {frm::NETRADIOSTATION,   "TRN", "TRSN", false, false, field::text},
    {frm::NETRADIOOWNER,     "TRO", "TRSO", false, false, field::text},
    {frm::SIZE,              "TSI", "TSIZ", false, true,  field::text},
    {frm::ISRC,              "TRC", "TSRC", false, false, field::text},
    {frm::ENCODERSETTINGS,   "TSS", "TSSE", false, false, field::text},
    {frm::USERTEXT,          "TXX", "TXXX", false, false, field::user_text},
    {frm::YEAR,              "TYE", "TYER", false, false, field::text},
    {frm::UNIQUEFILEID,      "UFI", "UFID", false, false, field::ufi},
    {frm::TERMSOFUSE,        "",    "USER", false, false, field::terms_of_use},
    {frm::UNSYNCEDLYRICS,    "ULT", "USLT", false, false, field::general_text},
    {frm::WWWCOMMERCIALINFO, "WCM", "WCOM", false, false, field::url},
    {frm::WWWCOPYRIGHT,      "WCP", "WCOP", false, false, field::url},
    {frm::WWWAUDIOFILE,      "WAF", "WOAF", false, false, field::url},
    {frm::WWWARTIST,         "WAR", "WOAR", false, false, field::url},
    {frm::WWWAUDIOSOURCE,    "WAS", "WOAS", false, false, field::url},
    {frm::WWWRADIOPAGE,      "WRA", "WORS", false, false, field::url},
    {frm::WWWPAYMENT,        "WPY", "WPAY", false, false, field::url},
    {frm::WWWPUBLISHER,      "WPB", "WPUB", false, false, field::url},
    {frm::WWWUSER,           "WXX", "WXXX", false, false, field::user_url},
    {frm::METACRYPTO,        "CRM", "",     false, false, field::binary_data},
    {frm::NOFRAME,           "",    "",     false, false, field::binary_data}
  };
}

// $Log$
