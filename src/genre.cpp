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

#include "genre.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{
  const genre genre::BLUES                  (  0, "Blues");
  const genre genre::CLASSIC_ROCK           (  1, "Classic Rock");
  const genre genre::COUNTRY                (  2, "Country");
  const genre genre::DANCE                  (  3, "Dance");
  const genre genre::DISCO                  (  4, "Disco");
  const genre genre::FUNK                   (  5, "Funk");
  const genre genre::GRUNGE                 (  6, "Grunge");
  const genre genre::HIP_HOP                (  7, "Hip-Hop");
  const genre genre::JAZZ                   (  8, "Jazz");
  const genre genre::METAL                  (  9, "Metal");
  const genre genre::NEW_AGE                ( 10, "New Age");
  const genre genre::OLDIES                 ( 11, "Oldies");
  const genre genre::OTHER                  ( 12, "Other");
  const genre genre::POP                    ( 13, "Pop");
  const genre genre::R_B                    ( 14, "R&B");
  const genre genre::RAP                    ( 15, "Rap");
  const genre genre::REGGAE                 ( 16, "Reggae");
  const genre genre::ROCK                   ( 17, "Rock");
  const genre genre::TECHNO                 ( 18, "Techno");
  const genre genre::INDUSTRIAL             ( 19, "Industrial");
  const genre genre::ALTERNATIVE            ( 20, "Alternative");
  const genre genre::SKA                    ( 21, "Ska");
  const genre genre::DEATH_METAL            ( 22, "Death Metal");
  const genre genre::PRANKS                 ( 23, "Pranks");
  const genre genre::SOUNDTRACK             ( 24, "Soundtrack");
  const genre genre::EURO_TECHNO            ( 25, "Euro-Techno");
  const genre genre::AMBIENT                ( 26, "Ambient");
  const genre genre::TRIP_HOP               ( 27, "Trip-Hop");
  const genre genre::VOCAL                  ( 28, "Vocal");
  const genre genre::JAZZ_FUNK              ( 29, "Jazz+Funk");
  const genre genre::FUSION                 ( 30, "Fusion");
  const genre genre::TRANCE                 ( 31, "Trance");
  const genre genre::CLASSICAL              ( 32, "Classical");
  const genre genre::INSTRUMENTAL           ( 33, "Instrumental");
  const genre genre::ACID                   ( 34, "Acid");
  const genre genre::HOUSE                  ( 35, "House");
  const genre genre::GAME                   ( 36, "Game");
  const genre genre::SOUND_CLIP             ( 37, "Sound Clip");
  const genre genre::GOSPEL                 ( 38, "Gospel");
  const genre genre::NOISE                  ( 39, "Noise");
  const genre genre::ALTERNROCK             ( 40, "AlternRock");
  const genre genre::BASS                   ( 41, "Bass");
  const genre genre::SOUL                   ( 42, "Soul");
  const genre genre::PUNK                   ( 43, "Punk");
  const genre genre::SPACE                  ( 44, "Space");
  const genre genre::MEDITATIVE             ( 45, "Meditative");
  const genre genre::INSTRUMENTAL_POP       ( 46, "Instrumental Pop");
  const genre genre::INSTRUMENTAL_ROCK      ( 47, "Instrumental Rock");
  const genre genre::ETHNIC                 ( 48, "Ethnic");
  const genre genre::GOTHIC                 ( 49, "Gothic");
  const genre genre::DARKWAVE               ( 50, "Darkwave");
  const genre genre::TECHNO_INDUSTRIAL      ( 51, "Techno-Industrial");
  const genre genre::ELECTRONIC             ( 52, "Electronic");
  const genre genre::POP_FOLK               ( 53, "Pop-Folk");
  const genre genre::EURODANCE              ( 54, "Eurodance");
  const genre genre::DREAM                  ( 55, "Dream");
  const genre genre::SOUTHERN_ROCK          ( 56, "Southern Rock");
  const genre genre::COMEDY                 ( 57, "Comedy");
  const genre genre::CULT                   ( 58, "Cult");
  const genre genre::GANGSTA                ( 59, "Gangsta");
  const genre genre::TOP_40                 ( 60, "Top 40");
  const genre genre::CHRISTIAN_RAP          ( 61, "Christian Rap");
  const genre genre::POP_FUNK               ( 62, "Pop/Funk");
  const genre genre::JUNGLE                 ( 63, "Jungle");
  const genre genre::NATIVE_AMERICAN        ( 64, "Native American");
  const genre genre::CABARET                ( 65, "Cabaret");
  const genre genre::NEW_WAVE               ( 66, "New Wave");
  const genre genre::PSYCHADELIC            ( 67, "Psychadelic");
  const genre genre::RAVE                   ( 68, "Rave");
  const genre genre::SHOWTUNES              ( 69, "Showtunes");
  const genre genre::TRAILER                ( 70, "Trailer");
  const genre genre::LO_FI                  ( 71, "Lo-Fi");
  const genre genre::TRIBAL                 ( 72, "Tribal");
  const genre genre::ACID_PUNK              ( 73, "Acid Punk");
  const genre genre::ACID_JAZZ              ( 74, "Acid Jazz");
  const genre genre::POLKA                  ( 75, "Polka");
  const genre genre::RETRO                  ( 76, "Retro");
  const genre genre::MUSICAL                ( 77, "Musical");
  const genre genre::ROCK_ROLL              ( 78, "Rock & Roll");
  const genre genre::HARD_ROCK              ( 79, "Hard Rock");
  const genre genre::FOLK                   ( 80, "Folk");
  const genre genre::FOLK_ROCK              ( 81, "Folk-Rock");
  const genre genre::NATIONAL_FOLK          ( 82, "National Folk");
  const genre genre::SWING                  ( 83, "Swing");
  const genre genre::FAST_FUSION            ( 84, "Fast Fusion");
  const genre genre::BEBOB                  ( 85, "Bebob");
  const genre genre::LATIN                  ( 86, "Latin");
  const genre genre::REVIVAL                ( 87, "Revival");
  const genre genre::CELTIC                 ( 88, "Celtic");
  const genre genre::BLUEGRASS              ( 89, "Bluegrass");
  const genre genre::AVANTGARDE             ( 90, "Avantgarde");
  const genre genre::GOTHIC_ROCK            ( 91, "Gothic Rock");
  const genre genre::PROGRESSIVE_ROCK       ( 92, "Progressive Rock");
  const genre genre::PSYCHEDELIC_ROCK       ( 93, "Psychedelic Rock");
  const genre genre::SYMPHONIC_ROCK         ( 94, "Symphonic Rock");
  const genre genre::SLOW_ROCK              ( 95, "Slow Rock");
  const genre genre::BIG_BAND               ( 96, "Big Band");
  const genre genre::CHORUS                 ( 97, "Chorus");
  const genre genre::EASY_LISTENING         ( 98, "Easy Listening");
  const genre genre::ACOUSTIC               ( 99, "Acoustic");
  const genre genre::HUMOUR                 (100, "Humour");
  const genre genre::SPEECH                 (101, "Speech");
  const genre genre::CHANSON                (102, "Chanson");
  const genre genre::OPERA                  (103, "Opera");
  const genre genre::CHAMBER_MUSIC          (104, "Chamber Music");
  const genre genre::SONATA                 (105, "Sonata");
  const genre genre::SYMPHONY               (106, "Symphony");
  const genre genre::BOOTY_BASS             (107, "Booty Bass");
  const genre genre::PRIMUS                 (108, "Primus");
  const genre genre::PORN_GROOVE            (109, "Porn Groove");
  const genre genre::SATIRE                 (110, "Satire");
  const genre genre::SLOW_JAM               (111, "Slow Jam");
  const genre genre::CLUB                   (112, "Club");
  const genre genre::TANGO                  (113, "Tango");
  const genre genre::SAMBA                  (114, "Samba");
  const genre genre::FOLKLORE               (115, "Folklore");
  const genre genre::BALLAD                 (116, "Ballad");
  const genre genre::POWER_BALLAD           (117, "Power Ballad");
  const genre genre::RHYTHMIC_SOUL          (118, "Rhythmic Soul");
  const genre genre::FREESTYLE              (119, "Freestyle");
  const genre genre::DUET                   (120, "Duet");
  const genre genre::PUNK_ROCK              (121, "Punk Rock");
  const genre genre::DRUM_SOLO              (122, "Drum Solo");
  const genre genre::A_CAPELLA              (123, "A capella");
  const genre genre::EURO_HOUSE             (124, "Euro-House");
  const genre genre::DANCE_HALL             (125, "Dance Hall");
}

// $Log$
