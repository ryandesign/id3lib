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

#ifndef ID3LIB_ENUMS_H
#define ID3LIB_ENUMS_H

namespace id3
{
  /// This namespace declares constants used in defining frames.
  namespace frm
  {
    /// The frame identifier
    enum id
    { /*      */ NOFRAME = -1,      ///< No known frame
      /* AENC */ AUDIOCRYPTO,       ///< Audio encryption
      /* APIC */ PICTURE,           ///< Attached picture
      /* COMM */ COMMENT,           ///< Comments
      /* COMR */ COMMERCIAL,        ///< Commercial frame
      /* ENCR */ CRYPTOREG,         ///< Encryption method registration
      /* EQUA */ EQUALIZATION,      ///< Equalization
      /* ETCO */ EVENTTIMING,       ///< Event timing codes
      /* GEOB */ GENERALOBJECT,     ///< General encapsulated object
      /* GRID */ GROUPINGREG,       ///< Group identification registration
      /* IPLS */ INVOLVEDPEOPLE,    ///< Involved people list
      /* LINK */ LINKEDINFO,        ///< Linked information
      /* MCDI */ CDID,              ///< Music CD identifier
      /* MLLT */ MPEGLOOKUP,        ///< MPEG location lookup table
      /* OWNE */ OWNERSHIP,         ///< Ownership frame
      /* PRIV */ PRIVATE,           ///< Private frame
      /* PCNT */ PLAYCOUNTER,       ///< Play counter
      /* POPM */ POPULARIMETER,     ///< Popularimeter
      /* POSS */ POSITIONSYNC,      ///< Position synchronisation frame
      /* RBUF */ BUFFERSIZE,        ///< Recommended buffer size
      /* RVAD */ VOLUMEADJ,         ///< Relative volume adjustment
      /* RVRB */ REVERB,            ///< Reverb
      /* SYLT */ SYNCEDLYRICS,      ///< Synchronized lyric/text
      /* SYTC */ SYNCEDTEMPO,       ///< Synchronized tempo codes
      /* TALB */ ALBUM,             ///< Album/Movie/Show title
      /* TBPM */ BPM,               ///< BPM (beats per minute)
      /* TCOM */ COMPOSER,          ///< Composer
      /* TCON */ CONTENTTYPE,       ///< Content type
      /* TCOP */ COPYRIGHT,         ///< Copyright message
      /* TDAT */ DATE,              ///< Date
      /* TDLY */ PLAYLISTDELAY,     ///< Playlist delay
      /* TENC */ ENCODEDBY,         ///< Encoded by
      /* TEXT */ LYRICIST,          ///< Lyricist/Text writer
      /* TFLT */ FILETYPE,          ///< File type
      /* TIME */ TIME,              ///< Time
      /* TIT1 */ CONTENTGROUP,      ///< Content group description
      /* TIT2 */ TITLE,             ///< Title/songname/content description
      /* TIT3 */ SUBTITLE,          ///< Subtitle/Description refinement
      /* TKEY */ INITIALKEY,        ///< Initial key
      /* TLAN */ LANGUAGE,          ///< Language(s)
      /* TLEN */ SONGLEN,           ///< Length
      /* TMED */ MEDIATYPE,         ///< Media type
      /* TOAL */ ORIGALBUM,         ///< Original album/movie/show title
      /* TOFN */ ORIGFILENAME,      ///< Original filename
      /* TOLY */ ORIGLYRICIST,      ///< Original lyricist(s)/text writer(s)
      /* TOPE */ ORIGARTIST,        ///< Original artist(s)/performer(s)
      /* TORY */ ORIGYEAR,          ///< Original release year
      /* TOWN */ FILEOWNER,         ///< File owner/licensee
      /* TPE1 */ LEADARTIST,        ///< Lead performer(s)/Soloist(s)
      /* TPE2 */ BAND,              ///< Band/orchestra/accompaniment
      /* TPE3 */ CONDUCTOR,         ///< Conductor/performer refinement
      /* TPE4 */ MIXARTIST,         ///< Interpreted, remixed, or modified by
      /* TPOS */ PARTINSET,         ///< Part of a set
      /* TPUB */ PUBLISHER,         ///< Publisher
      /* TRCK */ TRACKNUM,          ///< Track number/Position in set
      /* TRDA */ RECORDINGDATES,    ///< Recording dates
      /* TRSN */ NETRADIOSTATION,   ///< Internet radio station name
      /* TRSO */ NETRADIOOWNER,     ///< Internet radio station owner
      /* TSIZ */ SIZE,              ///< Size
      /* TSRC */ ISRC,              ///< International Standard Recording Code
      /* TSSE */ ENCODERSETTINGS,   ///< Software/Hardware, encoder settings
      /* TXXX */ USERTEXT,          ///< User defined text information
      /* TYER */ YEAR,              ///< Year
      /* UFID */ UNIQUEFILEID,      ///< Unique file identifier
      /* USER */ TERMSOFUSE,        ///< Terms of use
      /* USLT */ UNSYNCEDLYRICS,    ///< Unsynchronized lyric/text transcription
      /* WCOM */ WWWCOMMERCIALINFO, ///< Commercial information
      /* WCOP */ WWWCOPYRIGHT,      ///< Copyright/Legal infromation
      /* WOAF */ WWWAUDIOFILE,      ///< Official audio file webpage
      /* WOAR */ WWWARTIST,         ///< Official artist/performer webpage
      /* WOAS */ WWWAUDIOSOURCE,    ///< Official audio source webpage
      /* WORS */ WWWRADIOPAGE,      ///< Official internet radio station webpage
      /* WPAY */ WWWPAYMENT,        ///< Payment
      /* WPUB */ WWWPUBLISHER,      ///< Official publisher webpage
      /* WXXX */ WWWUSER,           ///< User defined URL link
      /*      */ METACRYPTO,        ///< Encrypted meta frame (id3v2.2.x),
      /*      */ NUMFRAMES          ///< Number of types of frames
    };
    /*
    enum type
    {
      NOTYPE = -1,
      BINARYDATA,
      TEXT,
      USERTEXT,      /// already defined
      GENERALTEXT,
      PICTURE,       /// already defined
      GEO,
      TEXTLIST,
      LINKEDINFO,    /// already defined
      URL,
      USERURL,
      TERMSOFUSE,    /// already defined
      UFI,
      PLAYCOUNTER,   /// already defined
      POPULARIMETER, /// already defined
      REGISTRATION,
      VOLUME,
    }
    */
  };

  /// This namespace declares constants used in defining fields.
  namespace fld
  {
    /// The field identifiers
    enum id
    {
      NOFIELD = -1,                 ///< No known field
      TEXTENC,                      ///< Text encoding (unicode or ASCII)
      TEXT,                         ///< Text field
      URL,                          ///< A URL
      DATA,                         ///< Data field
      DESCRIPTION,                  ///< Description field
      OWNER,                        ///< Owner field
      EMAIL,                        ///< Email field
      RATING,                       ///< Rating field
      FILENAME,                     ///< Filename field
      LANGUAGE,                     ///< Language field
      PICTURETYPE,                  ///< Picture type field
      IMAGEFORMAT,                  ///< Image format field
      MIMETYPE,                     ///< Mimetype field
      COUNTER,                      ///< Counter field
      SYMBOL,                       ///< Symbol field
      VOLUMEADJ,                    ///< Volume adjustment field
      NUMBITS,                      ///< Number of bits field
      VOLCHGRIGHT,                  ///< Volume chage on the right channel
      VOLCHGLEFT,                   ///< Volume chage on the left channel
      PEAKVOLRIGHT,                 ///< Peak volume on the right channel
      PEAKVOLLEFT,                  ///< Peak volume on the left channel
      NUMFIELDS,                    ///< The number of fields
    };

    /// Types of encodings for text fields (default is ASCII)
    enum encoding
    {
      NOENCODING = -1,              ///< No field encoding
      ASCII = 0,                    ///< ascii text
      UNICODE,                      ///< unicode text
      NUMENCODINGS                  ///< The number of encodings
    };

    /// Flags used to define text field attributes
    enum data_flags
    {
      NOFLAGS        =      0,      ///< No flag information
      CSTR           = 1 << 0,      ///< Data is a null-terminated text string
      LIST           = 1 << 1,      ///< Data is a null-divided list of strings
      ENCODABLE      = 1 << 2,      ///< Data has an encoding (ascii/unicode)
      TEXTLIST       = CSTR | LIST | ENCODABLE
    };

    /// Types of fields
    enum type
    {
      NOTYPE = -1,                  ///< Field has no type
      INTEGER = 0,                  ///< Field represents a positive integer
      BINARY,                       ///< Field represents binary data
      TEXTSTRING,                   ///< Field represents a text string
      NUMTYPES                      ///< Number of field types
    };

    /// The versioning scope in which this frame is defined
    enum scope                // Used for version control
    {
      NOSCOPE = -1,                ///< Field has no scope
      HIGHER  = 0,                 ///< Field defined in all later versions
      LOWER,                       ///< Field defined in all earlier versions
      NUMSCOPES                    ///< Number of scopes
    };
  };

  /// This namespace is for constants that define the current id3v2 spec
  namespace spec
  {
    /// Constants representing id3v2 versioning information
    enum version
    {
      NOVERSION = -1,              ///< No version
      VER_2_0,                     ///< id3v2.2.0
      VER_2_1,                     ///< id3v2.2.1 (experimental)
      VER_3_0,                     ///< id3v2.3.0
      NUMVERSIONS,                 ///< Number of versions defined
      CURRENT = VER_3_0,           ///< Current version recognized by id3lib
      EARLIEST = VER_2_0           ///< Earliest version recognized by id3lib
    };
  };

  /// ID3v1 and Winamp genre's, ordered according to their numerical value 
  enum genre
  {
    NOGENRE = -1,
    BLUES = 0,                     ///< 000 = Blues
    CLASSIC_ROCK,                  ///< 001 = Classic Rock
    COUNTRY,                       ///< 002 = Country
    DANCE,                         ///< 003 = Dance
    DISCO,                         ///< 004 = Disco
    FUNK,                          ///< 005 = Funk
    GRUNGE,                        ///< 006 = Grunge
    HIP_HOP,                       ///< 007 = Hip-Hop
    JAZZ,                          ///< 008 = Jazz
    METAL,                         ///< 009 = Metal
    NEW_AGE,                       ///< 010 = New Age
    OLDIES,                        ///< 011 = Oldies
    OTHER,                         ///< 012 = Other
    POP,                           ///< 013 = Pop
    R_B,                           ///< 014 = R&B
    RAP,                           ///< 015 = Rap
    REGGAE,                        ///< 016 = Reggae
    ROCK,                          ///< 017 = Rock
    TECHNO,                        ///< 018 = Techno
    INDUSTRIAL,                    ///< 019 = Industrial
    ALTERNATIVE,                   ///< 020 = Alternative
    SKA,                           ///< 021 = Ska
    DEATH_METAL,                   ///< 022 = Death Metal
    PRANKS,                        ///< 023 = Pranks
    SOUNDTRACK,                    ///< 024 = Soundtrack
    EURO_TECHNO,                   ///< 025 = Euro-Techno
    AMBIENT,                       ///< 026 = Ambient
    TRIP_HOP,                      ///< 027 = Trip-Hop
    VOCAL,                         ///< 028 = Vocal
    JAZZ_FUNK,                     ///< 029 = Jazz+Funk
    FUSION,                        ///< 030 = Fusion
    TRANCE,                        ///< 031 = Trance
    CLASSICAL,                     ///< 032 = Classical
    INSTRUMENTAL,                  ///< 033 = Instrumental
    ACID,                          ///< 034 = Acid
    HOUSE,                         ///< 035 = House
    GAME,                          ///< 036 = Game
    SOUND_CLIP,                    ///< 037 = Sound Clip
    GOSPEL,                        ///< 038 = Gospel
    NOISE,                         ///< 039 = Noise
    ALTERNROCK,                    ///< 040 = AlternRock
    BASS,                          ///< 041 = Bass
    SOUL,                          ///< 042 = Soul
    PUNK,                          ///< 043 = Punk
    SPACE,                         ///< 044 = Space
    MEDITATIVE,                    ///< 045 = Meditative
    INSTRUMENTAL_POP,              ///< 046 = Instrumental Pop
    INSTRUMENTAL_ROCK,             ///< 047 = Instrumental Rock
    ETHNIC,                        ///< 048 = Ethnic
    GOTHIC,                        ///< 049 = Gothic
    DARKWAVE,                      ///< 050 = Darkwave
    TECHNO_INDUSTRIAL,             ///< 051 = Techno-Industrial
    ELECTRONIC,                    ///< 052 = Electronic
    POP_FOLK,                      ///< 053 = Pop-Folk
    EURODANCE,                     ///< 054 = Eurodance
    DREAM,                         ///< 055 = Dream
    SOUTHERN_ROCK,                 ///< 056 = Southern Rock
    COMEDY,                        ///< 057 = Comedy
    CULT,                          ///< 058 = Cult
    GANGSTA,                       ///< 059 = Gangsta
    TOP_40,                        ///< 060 = Top 40
    CHRISTIAN_RAP,                 ///< 061 = Christian Rap
    POP_FUNK,                      ///< 062 = Pop/Funk
    JUNGLE,                        ///< 063 = Jungle
    NATIVE_AMERICAN,               ///< 064 = Native American
    CABARET,                       ///< 065 = Cabaret
    NEW_WAVE,                      ///< 066 = New Wave
    PSYCHADELIC,                   ///< 067 = Psychadelic
    RAVE,                          ///< 068 = Rave
    SHOWTUNES,                     ///< 069 = Showtunes
    TRAILER,                       ///< 070 = Trailer
    LO_FI,                         ///< 071 = Lo-Fi
    TRIBAL,                        ///< 072 = Tribal
    ACID_PUNK,                     ///< 073 = Acid Punk
    ACID_JAZZ,                     ///< 074 = Acid Jazz
    POLKA,                         ///< 075 = Polka
    RETRO,                         ///< 076 = Retro
    MUSICAL,                       ///< 077 = Musical
    ROCK_ROLL,                     ///< 078 = Rock & Roll
    HARD_ROCK,                     ///< 079 = Hard Rock
    FOLK,                          ///< 080 = Folk
    FOLK_ROCK,                     ///< 081 = Folk-Rock
    NATIONAL_FOLK,                 ///< 082 = National Folk
    SWING,                         ///< 083 = Swing
    FAST_FUSION,                   ///< 084 = Fast Fusion
    BEBOB,                         ///< 085 = Bebob
    LATIN,                         ///< 086 = Latin
    REVIVAL,                       ///< 087 = Revival
    CELTIC,                        ///< 088 = Celtic
    BLUEGRASS,                     ///< 089 = Bluegrass
    AVANTGARDE,                    ///< 090 = Avantgarde
    GOTHIC_ROCK,                   ///< 091 = Gothic Rock
    PROGRESSIVE_ROCK,              ///< 092 = Progressive Rock
    PSYCHEDELIC_ROCK,              ///< 093 = Psychedelic Rock
    SYMPHONIC_ROCK,                ///< 094 = Symphonic Rock
    SLOW_ROCK,                     ///< 095 = Slow Rock
    BIG_BAND,                      ///< 096 = Big Band
    CHORUS,                        ///< 097 = Chorus
    EASY_LISTENING,                ///< 098 = Easy Listening
    ACOUSTIC,                      ///< 099 = Acoustic
    HUMOUR,                        ///< 100 = Humour
    SPEECH,                        ///< 101 = Speech
    CHANSON,                       ///< 102 = Chanson
    OPERA,                         ///< 103 = Opera
    CHAMBER_MUSIC,                 ///< 104 = Chamber Music
    SONATA,                        ///< 105 = Sonata
    SYMPHONY,                      ///< 106 = Symphony
    BOOTY_BASS,                    ///< 107 = Booty Bass
    PRIMUS,                        ///< 108 = Primus
    PORN_GROOVE,                   ///< 109 = Porn Groove
    SATIRE,                        ///< 110 = Satire
    SLOW_JAM,                      ///< 111 = Slow Jam
    CLUB,                          ///< 112 = Club
    TANGO,                         ///< 113 = Tango
    SAMBA,                         ///< 114 = Samba
    FOLKLORE,                      ///< 115 = Folklore
    BALLAD,                        ///< 116 = Ballad
    POWER_BALLAD,                  ///< 117 = Power Ballad
    RHYTHMIC_SOUL,                 ///< 118 = Rhythmic Soul
    FREESTYLE,                     ///< 119 = Freestyle
    DUET,                          ///< 120 = Duet
    PUNK_ROCK,                     ///< 121 = Punk Rock
    DRUM_SOLO,                     ///< 122 = Drum Solo
    A_CAPELLA,                     ///< 123 = A capella
    EURO_HOUSE,                    ///< 124 = Euro-House
    DANCE_HALL                     ///< 125 = Dance Hall
  };
};

#endif /* !defined ID3LIB_ENUMS_H */

// $Log$
