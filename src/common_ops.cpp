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

#include "common_ops.h"
#include "tag.h"
#include "enums.h"
#include "tag_find.h"
#include <cctype>
#include <algorithm>

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{
  template <class Predicate>
  string get_frame_text(const tag& tag, Predicate op)
  {
    tag::const_iterator fi;
    if (tag.end() != (fi = std::find_if(tag.begin(), tag.end(), op)))
    {
      return fi->get(fld::TEXT).ascii();
    }
    return string();
  }
  
  frame text_frame(frm::id id, const string& text)
  {
    frame f(id);
    f.get(fld::TEXT).assign(text);
    return f;
  }

  /** Adds a frame to a tag **/
  template <class Predicate>
  bool add_frame(tag& tag, const frame& f, Predicate op, bool replace)
  {
    bool success = false;
    if (replace)
    {
      tag.remove_if(op);
    }
    
    if (replace || std::find_if(tag.begin(), tag.end(), op) == tag.end())
    {
      tag.add(f);
      success = true;
    }
    
    return success;
  }

  bool add_frame_id(tag& tag, frm::id id, const string& s, bool replace)
  {
    return add_frame(tag, text_frame(id, s), has_frame_id(id), replace);
  }

  template <class Predicate>
  size_t remove_frames(tag& tag, Predicate op)
  {
    size_t count = count_if(tag.begin(), tag.end(), op);
    tag.remove_if(op);
    return count;
  }

  bool is_artist(const frame& f)
  {
    return 
      f.id() == frm::LEADARTIST || f.id() == frm::BAND ||
      f.id() == frm::CONDUCTOR  || f.id() == frm::COMPOSER;
  }

  string get_artist(const tag& tag)
  {
    return get_frame_text(tag, is_artist);
  }

  bool add_artist(tag& tag, const string& s, bool replace)
  {
    return add_frame(tag, text_frame(frm::LEADARTIST, s), 
                     is_artist, replace);
  }

  size_t remove_artists(tag& tag)
  {
    return remove_frames(tag, is_artist);
  }

  string get_album(tag& tag)
  {
    return get_frame_text(tag, has_frame_id(frm::ALBUM));
  }

  string get_title(tag& tag)
  {
    return get_frame_text(tag, has_frame_id(frm::TITLE));
  }

  bool add_album(tag& tag, const string& s, bool replace)
  {
    return add_frame_id(tag, frm::ALBUM, s, replace);
  }

  bool add_title(tag& tag, const string& s, bool replace)
  {
    return add_frame_id(tag, frm::TITLE, s, replace);
  }

  size_t remove_albums(tag& tag)
  {
    return remove_frames(tag, has_frame_id(frm::ALBUM));
  }

  size_t remove_titles(tag& tag)
  {
    return remove_frames(tag, has_frame_id(frm::TITLE));
  }

  string get_year(tag& tag)
  {
    return get_frame_text(tag, has_frame_id(frm::YEAR));
  }

  bool add_year(tag& tag, const string& s, bool replace)
  {
    return add_frame_id(tag, frm::YEAR, s, replace);
  }

  size_t remove_years(tag& tag)
  {
    return remove_frames(tag, has_frame_id(frm::YEAR));
  }

  string get_comment(const tag& tag)
  {
    return get_frame_text(tag, has_frame_id(frm::COMMENT));
  }

  bool add_comment(tag& tag, const string& comment, const string& desc, 
                   bool replace)
  {
    frame f = text_frame(frm::COMMENT, comment);
    f.get(fld::LANGUAGE).assign("eng");
    f.get(fld::DESCRIPTION).assign(desc);
    has_field_ascii op(frm::COMMENT, fld::DESCRIPTION, desc);
    return add_frame(tag, f, op, replace);
  }

  // Remove all comments with the given description (remove all comments if
  // sDescription is NULL)
  size_t remove_comments(tag& tag, const string& desc)
  {
    has_field_ascii op(frm::COMMENT, fld::DESCRIPTION, desc);
    return remove_frames(tag, op);
  }

  string get_track(const tag& tag)
  {
    return get_frame_text(tag, has_frame_id(frm::TRACKNUM));
  }

  size_t get_track_num(const tag& tag)
  {
    return atoi(get_track(tag).c_str());
  }

  bool add_track(tag& tag, char track, char total, bool replace)
  {
    string s;
    s += track;
    if (0 != total)
    {
      s += '/';
      s += total;
    }
    return add_frame_id(tag, frm::TRACKNUM, s, replace);
  }

  bool add_track(tag& tag, char track, bool replace)
  {
    return add_track(tag, track, 0, replace);
  }

  size_t remove_tracks(tag& tag)
  {
    return remove_frames(tag, has_frame_id(frm::TRACKNUM));
  }

  string get_genre(const tag& tag)
  {
    return get_frame_text(tag, has_frame_id(frm::CONTENTTYPE));
  }

  uchar get_genre_num(const tag& tag)
  {
    string s = get_genre(tag);
    uchar genre = 0xFF;
    // If the genre string begins with "(ddd)", where "ddd" is a number, then 
    // "ddd" is the genre number---get it
    if (s[0] == '(')
    {
      size_t index = 1;
      while (isdigit(index))
      {
        index++;
      }
      if (s[index] == ')')
      {
        // if the genre number is greater than 255, its invalid.
        genre = std::min(0xFF, ::atoi(&s.c_str()[1]));
      }
    }
    return genre;
  }

  bool add_genre(tag& tag, size_t genre, bool replace)
  {
    string s = "(";
    s += genre;
    s += ")";
    return add_frame_id(tag, frm::CONTENTTYPE, s, replace);
  }

  size_t remove_genres(tag& tag)
  {
    return remove_frames(tag, has_frame_id(frm::CONTENTTYPE));
  }

  string get_lyrics(const tag& tag)
  {
    return get_frame_text(tag, has_frame_id(frm::UNSYNCEDLYRICS));
  }

  bool add_lyrics(tag& tag, const string& s, bool replace)
  {
    frame f = text_frame(frm::UNSYNCEDLYRICS, s);
    f.get(fld::LANGUAGE).assign("eng");
    return add_frame(tag, f, has_frame_id(frm::UNSYNCEDLYRICS), replace);
  }

  size_t remove_lyrics(tag& tag)
  {
    return remove_frames(tag, has_frame_id(frm::UNSYNCEDLYRICS));
  }
}

// $Log$
