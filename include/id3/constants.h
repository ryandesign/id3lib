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

#ifndef ID3LIB_CONSTANTS_H
#define ID3LIB_CONSTANTS_H

#include "enums.h"
#include "types.h"

namespace id3
{
  namespace frm
  {
    const string id_s[] =
    {
      /* AENC - frm::AUDIOCRYPTO:       */ "audio encryption",
      /* APIC - frm::PICTURE:           */ "attached picture",
      /* COMM - frm::COMMENT:           */ "comments",
      /* COMR - frm::COMMERCIAL:        */ "commercial",
      /* ENCR - frm::CRYPTOREG:         */ "encryption method registration",
      /* EQUA - frm::EQUALIZATION:      */ "equalization",
      /* ETCO - frm::EVENTTIMING:       */ "event timing codes",
      /* GEOB - frm::GENERALOBJECT:     */ "general encapsulated object",
      /* GRID - frm::GROUPINGREG:       */ "group identification registration",
      /* IPLS - frm::INVOLVEDPEOPLE:    */ "involved people list",
      /* LINK - frm::LINKEDINFO:        */ "linked information",
      /* MCDI - frm::CDID:              */ "music CD identifier",
      /* MLLT - frm::MPEGLOOKUP:        */ "MPEG location lookup table",
      /* OWNE - frm::OWNERSHIP:         */ "ownership frame",
      /* PRIV - frm::PRIVATE:           */ "private frame",
      /* PCNT - frm::PLAYCOUNTER:       */ "play counter",
      /* POPM - frm::POPULARIMETER:     */ "popularimeter",
      /* POSS - frm::POSITIONSYNC:      */ "position synchronisation frame",
      /* RBUF - frm::BUFFERSIZE:        */ "recommended buffer size",
      /* RVAD - frm::VOLUMEADJ:         */ "relative volume adjustment",
      /* RVRB - frm::REVERB:            */ "reverb",
      /* SYLT - frm::SYNCEDLYRICS:      */ "synchronized lyric/text",
      /* SYTC - frm::SYNCEDTEMPO:       */ "synchronized tempo codes",
      /* TALB - frm::ALBUM:             */ "album/movie/show title",
      /* TBPM - frm::BPM:               */ "BPM (beats per minute)",
      /* TCOM - frm::COMPOSER:          */ "composer",
      /* TCON - frm::CONTENTTYPE:       */ "content type",
      /* TCOP - frm::COPYRIGHT:         */ "copyright message",
      /* TDAT - frm::DATE:              */ "date",
      /* TDLY - frm::PLAYLISTDELAY:     */ "playlist delay",
      /* TENC - frm::ENCODEDBY:         */ "encoded by",
      /* TEXT - frm::LYRICIST:          */ "lyricist/text writer",
      /* TFLT - frm::FILETYPE:          */ "file type",
      /* TIME - frm::TIME:              */ "time",
      /* TIT1 - frm::CONTENTGROUP:      */ "content group description",
      /* TIT2 - frm::TITLE:             */ "title/songname/content description",
      /* TIT3 - frm::SUBTITLE:          */ "subtitle/description refinement",
      /* TKEY - frm::INITIALKEY:        */ "initial key",
      /* TLAN - frm::LANGUAGE:          */ "language(s)",
      /* TLEN - frm::SONGLEN:           */ "length",
      /* TMED - frm::MEDIATYPE:         */ "media type",
      /* TOAL - frm::ORIGALBUM:         */ "original album/movie/show title",
      /* TOFN - frm::ORIGFILENAME:      */ "original filename",
      /* TOLY - frm::ORIGLYRICIST:      */ "original lyricist(s)/text writer(s)",
      /* TOPE - frm::ORIGARTIST:        */ "original artist(s)/performer(s)",
      /* TORY - frm::ORIGYEAR:          */ "original release year",
      /* TOWN - frm::FILEOWNER:         */ "file owner/licensee",
      /* TPE1 - frm::LEADARTIST:        */ "lead performer(s)/soloist(s)",
      /* TPE2 - frm::BAND:              */ "band/orchestra/accompaniment",
      /* TPE3 - frm::CONDUCTOR:         */ "conductor/performer refinement",
      /* TPE4 - frm::MIXARTIST:         */ "interpreted, remixed, or otherwise modified by",
      /* TPOS - frm::PARTINSET:         */ "part of a set",
      /* TPUB - frm::PUBLISHER:         */ "publisher",
      /* TRCK - frm::TRACKNUM:          */ "track number/position in set",
      /* TRDA - frm::RECORDINGDATES:    */ "recording dates",
      /* TRSN - frm::NETRADIOSTATION:   */ "internet radio station name",
      /* TRSO - frm::NETRADIOOWNER:     */ "internet radio station owner",
      /* TSIZ - frm::SIZE:              */ "size",
      /* TSRC - frm::ISRC:              */ "ISRC (international standard recording code)",
      /* TSSE - frm::ENCODERSETTINGS:   */ "software/hardware and settings used for encoding",
      /* TXXX - frm::USERTEXT:          */ "user defined text information",
      /* TYER - frm::YEAR:              */ "year",
      /* UFID - frm::UNIQUEFILEID:      */ "unique file identifier",
      /* USER - frm::TERMSOFUSE:        */ "terms of use",
      /* USLT - frm::UNSYNCEDLYRICS:    */ "unsynchronized lyric/text transcription",
      /* WCOM - frm::WWWCOMMERCIALINFO: */ "commercial information",
      /* WCOP - frm::WWWCOPYRIGHT:      */ "copyright/legal infromation",
      /* WOAF - frm::WWWAUDIOFILE:      */ "official audio file webpage",
      /* WOAR - frm::WWWARTIST:         */ "official artist/performer webpage",
      /* WOAS - frm::WWWAUDIOSOURCE:    */ "official audio source webpage",
      /* WORS - frm::WWWRADIOPAGE:      */ "official internet radio station homepage",
      /* WPAY - frm::WWWPAYMENT:        */ "payment",
      /* WPUB - frm::WWWPUBLISHER:      */ "official publisher webpage",
      /* WXXX - frm::WWWUSER:           */ "user defined URL link",
      /*      - frm::METACRYPTO:        */ "encrypted meta frame"
    };
  };

  namespace spec
  {
    const string version_s[] =
    {
      /* spec::VER_2_0: */ "2.2.0",
      /* spec::VER_2_1: */ "2.2.1",
      /* spec::VER_3_0: */ "2.3.0"
    };
  };

  namespace fld
  {
    const string id_s[] =
    {
      /* fld::TEXTENC:      */ "text encoding",
      /* fld::TEXT:         */ "text",
      /* fld::URL:          */ "url",
      /* fld::DATA:         */ "data",
      /* fld::DESCRIPTION:  */ "description",
      /* fld::OWNER:        */ "owner",
      /* fld::EMAIL:        */ "email",
      /* fld::RATING:       */ "rating",
      /* fld::FILENAME:     */ "filename",
      /* fld::LANGUAGE:     */ "language",
      /* fld::PICTURETYPE:  */ "picture type",
      /* fld::IMAGEFORMAT:  */ "image format",
      /* fld::MIMETYPE:     */ "mimetype",
      /* fld::COUNTER:      */ "counter",
      /* fld::SYMBOL:       */ "symbol",
      /* fld::VOLUMEADJ:    */ "volume adjustment",
      /* fld::NUMBITS:      */ "number of bits",
      /* fld::VOLCHGRIGHT:  */ "volume chage on the right channel",
      /* fld::VOLCHGLEFT:   */ "volume chage on the left channel", 
      /* fld::PEAKVOLRIGHT: */ "peak volume on the right channel",
      /* fld::PEAKVOLLEFT:  */ "peak volume on the left channel"
    }; 
    
    const string encoding_s[] =
    {
      /* fld::ASCII:   */ "ascii",
      /* fld::UNICODE: */ "unicode"
    };

    const string type_s[] =
    {
      /* fld::INTEGER:       */ "integer",
      /* fld::BINARY:        */ "binary",
      /* fld::ASCIISTRING:   */ "ascii-text",
      /* fld::UNICODESTRING: */ "unicode-text"
    };
  };

  const string genre_s[] =
  {
    /* BLUES */ "Blues",
    /* CLASSIC_ROCK */ "Classic Rock",
    "Country",
    "Dance",
    "Disco",
    "Funk",
    "Grunge",
    "Hip-Hop",
    "Jazz",
    "Metal",
    "New Age",
    "Oldies",
    "Other",
    "Pop",
    "R&B",
    "Rap",
    "Reggae",
    "Rock",
    "Techno",
    "Industrial",
    "Alternative ",
    "Ska",
    "Death Metal",
    "Pranks",
    "Soundtrack",
    "Euro-Techno",
    "Ambient",
    "Trip-Hop",
    "Vocal",
    "Jazz+Funk",
    "Fusion",
    "Trance",
    "Classical",
    "Instrumental",
    "Acid",
    "House",
    "Game",
    "Sound Clip",
    "Gospel",
    "Noise",
    "AlternRock",
    "Bass",
    "Soul",
    "Punk",
    /* SPACE             */ "Space",
    /* MEDITATIVE        */ "Meditative",
    /* INSTRUMENTAL_POP  */ "Instrumental Pop",
    /* INSTRUMENTAL_ROCK */ "Instrumental Rock",
    "Ethnic",
    "Gothic",
    "Darkwave",
    "Techno-Industrial",
    "Electronic",
    "Pop-Folk",
    "Eurodance",
    "Dream",
    "Southern Rock",
    "Comedy",
    "Cult",
    "Gangsta",
    "Top 40",
    "Christian Rap",
    "Pop/Funk",
    "Jungle",
    "Native American",
    "Cabaret",
    "New Wave",
    "Psychadelic",
    "Rave",
    "Showtunes",
    "Trailer",
    "Lo-Fi",
    "Tribal",
    "Acid Punk",
    "Acid Jazz",
    "Polka",
    "Retro",
    "Musical",
    "Rock & Roll",
    "Hard Rock",
    "Folk",
    "Folk-Rock",
    "National Folk",
    "Swing",
    "Fast Fusion",
    "Bebob",
    "Latin",
    "Revival",
    "Celtic",
    "Bluegrass",
    "Avantgarde",
    "Gothic Rock",
    "Progressive Rock",
    "Psychedelic Rock",
    "Symphonic Rock",
    "Slow Rock",
    "Big Band",
    "Chorus",
    "Easy Listening",
    "Acoustic",
    "Humour",
    "Speech",
    "Chanson",
    "Opera",
    "Chamber Music",
    "Sonata",
    "Symphony",
    "Booty Bass",
    "Primus",
    "Porn Groove",
    "Satire",
    "Slow Jam",
    "Club",
    "Tango",
    "Samba",
    "Folklore",
    "Ballad",
    "Power Ballad",
    "Rhythmic Soul",
    "Freestyle",
    "Duet",
    "Punk Rock",
    "Drum Solo",
    "A capella",
    "Euro-House",
    "Dance Hall"
  };
};

#endif /* ID3LIB_CONSTANTS_H */

// $Log$
