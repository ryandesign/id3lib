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

#ifndef ID3LIB_COMMON_OPS_H
#define ID3LIB_COMMON_OPS_H

#include "types.h"

namespace id3
{
  class tag;

  // these are 'convenience functions,' to make using the library easier for
  // the most common of operations
  string get_artist(const tag&);
  bool   add_artist(tag&, const string& text, bool = false);
  size_t remove_artists(tag&);
  
  string get_album(const tag&);
  bool   add_album(tag&, const string& text, bool = false);
  size_t remove_albums(tag&);
  
  string get_title(const tag&);
  bool   add_title(tag&, const string&, bool = false);
  size_t remove_titles(tag&);

  string get_year(const tag&);
  bool   add_year(tag&, const string&, bool = false);
  size_t remove_years(tag&);

  string get_comment(const tag&);
  bool   add_comment(tag&, const string&, const string& = "", bool = false);
  size_t remove_comments(tag&, const string& desc);

  string get_track(const tag&);
  size_t get_track_num(tag&);
  bool   add_track(tag&, char, char = 0, bool = false);
  bool   add_track(tag&, char, bool = false);
  size_t remove_tracks(tag&);

  string get_genre(const tag&);
  size_t get_genre_num(tag&);
  bool   add_genre(tag&, size_t, bool = false);
  size_t remove_genres(tag&);

  string get_lyrics(const tag&);
  bool   add_lyrics(tag&, const string&, bool = false);
  size_t remove_lyrics(tag&);
}

#endif /* ID3LIB_COMMON_OPS_H */

// $Log$
