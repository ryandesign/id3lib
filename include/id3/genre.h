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

#ifndef ID3LIB_INT28_H
#define ID3LIB_INT28_H

#include "types.h"

namespace id3
{
  class genre
  {
    uchar  __val;
    string __desc;
  public:
    string to_string() const { return __desc; }
    uchar  to_uchar() const { return __val; }

    static const genre BLUES;                  ///< 000 = Blues
    static const genre CLASSIC_ROCK;           ///< 001 = Classic Rock
    static const genre COUNTRY;                ///< 002 = Country
    static const genre DANCE;                  ///< 003 = Dance
    static const genre DISCO;                  ///< 004 = Disco
    static const genre FUNK;                   ///< 005 = Funk
    static const genre GRUNGE;                 ///< 006 = Grunge
    static const genre HIP_HOP;                ///< 007 = Hip-Hop
    static const genre JAZZ;                   ///< 008 = Jazz
    static const genre METAL;                  ///< 009 = Metal
    static const genre NEW_AGE;                ///< 010 = New Age
    static const genre OLDIES;                 ///< 011 = Oldies
    static const genre OTHER;                  ///< 012 = Other
    static const genre POP;                    ///< 013 = Pop
    static const genre R_B;                    ///< 014 = R&B
    static const genre RAP;                    ///< 015 = Rap
    static const genre REGGAE;                 ///< 016 = Reggae
    static const genre ROCK;                   ///< 017 = Rock
    static const genre TECHNO;                 ///< 018 = Techno
    static const genre INDUSTRIAL;             ///< 019 = Industrial
    static const genre ALTERNATIVE;            ///< 020 = Alternative
    static const genre SKA;                    ///< 021 = Ska
    static const genre DEATH_METAL;            ///< 022 = Death Metal
    static const genre PRANKS;                 ///< 023 = Pranks
    static const genre SOUNDTRACK;             ///< 024 = Soundtrack
    static const genre EURO_TECHNO;            ///< 025 = Euro-Techno
    static const genre AMBIENT;                ///< 026 = Ambient
    static const genre TRIP_HOP;               ///< 027 = Trip-Hop
    static const genre VOCAL;                  ///< 028 = Vocal
    static const genre JAZZ_FUNK;              ///< 029 = Jazz+Funk
    static const genre FUSION;                 ///< 030 = Fusion
    static const genre TRANCE;                 ///< 031 = Trance
    static const genre CLASSICAL;              ///< 032 = Classical
    static const genre INSTRUMENTAL;           ///< 033 = Instrumental
    static const genre ACID;                   ///< 034 = Acid
    static const genre HOUSE;                  ///< 035 = House
    static const genre GAME;                   ///< 036 = Game
    static const genre SOUND_CLIP;             ///< 037 = Sound Clip
    static const genre GOSPEL;                 ///< 038 = Gospel
    static const genre NOISE;                  ///< 039 = Noise
    static const genre ALTERNROCK;             ///< 040 = AlternRock
    static const genre BASS;                   ///< 041 = Bass
    static const genre SOUL;                   ///< 042 = Soul
    static const genre PUNK;                   ///< 043 = Punk
    static const genre SPACE;                  ///< 044 = Space
    static const genre MEDITATIVE;             ///< 045 = Meditative
    static const genre INSTRUMENTAL_POP;       ///< 046 = Instrumental Pop
    static const genre INSTRUMENTAL_ROCK;      ///< 047 = Instrumental Rock
    static const genre ETHNIC;                 ///< 048 = Ethnic
    static const genre GOTHIC;                 ///< 049 = Gothic
    static const genre DARKWAVE;               ///< 050 = Darkwave
    static const genre TECHNO_INDUSTRIAL;      ///< 051 = Techno-Industrial
    static const genre ELECTRONIC;             ///< 052 = Electronic
    static const genre POP_FOLK;               ///< 053 = Pop-Folk
    static const genre EURODANCE;              ///< 054 = Eurodance
    static const genre DREAM;                  ///< 055 = Dream
    static const genre SOUTHERN_ROCK;          ///< 056 = Southern Rock
    static const genre COMEDY;                 ///< 057 = Comedy
    static const genre CULT;                   ///< 058 = Cult
    static const genre GANGSTA;                ///< 059 = Gangsta
    static const genre TOP_40;                 ///< 060 = Top 40
    static const genre CHRISTIAN_RAP;          ///< 061 = Christian Rap
    static const genre POP_FUNK;               ///< 062 = Pop/Funk
    static const genre JUNGLE;                 ///< 063 = Jungle
    static const genre NATIVE_AMERICAN;        ///< 064 = Native American
    static const genre CABARET;                ///< 065 = Cabaret
    static const genre NEW_WAVE;               ///< 066 = New Wave
    static const genre PSYCHADELIC;            ///< 067 = Psychadelic
    static const genre RAVE;                   ///< 068 = Rave
    static const genre SHOWTUNES;              ///< 069 = Showtunes
    static const genre TRAILER;                ///< 070 = Trailer
    static const genre LO_FI;                  ///< 071 = Lo-Fi
    static const genre TRIBAL;                 ///< 072 = Tribal
    static const genre ACID_PUNK;              ///< 073 = Acid Punk
    static const genre ACID_JAZZ;              ///< 074 = Acid Jazz
    static const genre POLKA;                  ///< 075 = Polka
    static const genre RETRO;                  ///< 076 = Retro
    static const genre MUSICAL;                ///< 077 = Musical
    static const genre ROCK_ROLL;              ///< 078 = Rock & Roll
    static const genre HARD_ROCK;              ///< 079 = Hard Rock
    static const genre FOLK;                   ///< 080 = Folk
    static const genre FOLK_ROCK;              ///< 081 = Folk-Rock
    static const genre NATIONAL_FOLK;          ///< 082 = National Folk
    static const genre SWING;                  ///< 083 = Swing
    static const genre FAST_FUSION;            ///< 084 = Fast Fusion
    static const genre BEBOB;                  ///< 085 = Bebob
    static const genre LATIN;                  ///< 086 = Latin
    static const genre REVIVAL;                ///< 087 = Revival
    static const genre CELTIC;                 ///< 088 = Celtic
    static const genre BLUEGRASS;              ///< 089 = Bluegrass
    static const genre AVANTGARDE;             ///< 090 = Avantgarde
    static const genre GOTHIC_ROCK;            ///< 091 = Gothic Rock
    static const genre PROGRESSIVE_ROCK;       ///< 092 = Progressive Rock
    static const genre PSYCHEDELIC_ROCK;       ///< 093 = Psychedelic Rock
    static const genre SYMPHONIC_ROCK;         ///< 094 = Symphonic Rock
    static const genre SLOW_ROCK;              ///< 095 = Slow Rock
    static const genre BIG_BAND;               ///< 096 = Big Band
    static const genre CHORUS;                 ///< 097 = Chorus
    static const genre EASY_LISTENING;         ///< 098 = Easy Listening
    static const genre ACOUSTIC;               ///< 099 = Acoustic
    static const genre HUMOUR;                 ///< 100 = Humour
    static const genre SPEECH;                 ///< 101 = Speech
    static const genre CHANSON;                ///< 102 = Chanson
    static const genre OPERA;                  ///< 103 = Opera
    static const genre CHAMBER_MUSIC;          ///< 104 = Chamber Music
    static const genre SONATA;                 ///< 105 = Sonata
    static const genre SYMPHONY;               ///< 106 = Symphony
    static const genre BOOTY_BASS;             ///< 107 = Booty Bass
    static const genre PRIMUS;                 ///< 108 = Primus
    static const genre PORN_GROOVE;            ///< 109 = Porn Groove
    static const genre SATIRE;                 ///< 110 = Satire
    static const genre SLOW_JAM;               ///< 111 = Slow Jam
    static const genre CLUB;                   ///< 112 = Club
    static const genre TANGO;                  ///< 113 = Tango
    static const genre SAMBA;                  ///< 114 = Samba
    static const genre FOLKLORE;               ///< 115 = Folklore
    static const genre BALLAD;                 ///< 116 = Ballad
    static const genre POWER_BALLAD;           ///< 117 = Power Ballad
    static const genre RHYTHMIC_SOUL;          ///< 118 = Rhythmic Soul
    static const genre FREESTYLE;              ///< 119 = Freestyle
    static const genre DUET;                   ///< 120 = Duet
    static const genre PUNK_ROCK;              ///< 121 = Punk Rock
    static const genre DRUM_SOLO;              ///< 122 = Drum Solo
    static const genre A_CAPELLA;              ///< 123 = A capella
    static const genre EURO_HOUSE;             ///< 124 = Euro-House
    static const genre DANCE_HALL;             ///< 125 = Dance Hall

  private:
    genre();
    genre(const genre&);
    genre(uchar val, const string& desc) : __val(val), __desc(desc) { ; }
  };
};

#endif

// $Log$
