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

#include <strstream>
#include "constants.h"
#include "conversion_ops.h"
#include "tag.h"

namespace id3
{
  string to_string(frm::id id)
  {
    if (frm::NOFRAME < id && id < frm::NUMFRAMES)
    {
      return frm::id_s[id];
    }
    return "unknown frame id";
  };

  string to_string(spec::version v)
  {
    if (spec::NOVERSION < v && v < spec::NUMVERSIONS)
    {
      return spec::version_s[v];
    }
    return "unknown version";
  };

  string to_string(fld::id id)
  {
    if (fld::NOFIELD < id && id < fld::NUMFIELDS)
    {
      return fld::id_s[id];
    }
    return "unknown field id";
  }

  string to_string(fld::type type)
  {
    if (fld::NOTYPE < type && type < fld::NUMTYPES)
    {
      return fld::type_s[type];
    }
    return "unknown field type";
  }

  string to_string(fld::encoding e)
  {
    if (fld::NOENCODING < e && e < fld::NUMENCODINGS)
    {
      return fld::encoding_s[e];
    }
    return "unknown field encoding";
  }

  string to_string(const field& f)
  {
    char s[1024];
    ostrstream os(s, sizeof(s));
    os << "Field: "
       << to_string(f.id()) << ", "
       << to_string(f.type()) << ", "
       << f.size() << " bytes: ";
    if (f.in_scope()) 
    { 
      os << "scope "; 
    }  
    if (f.fixed_size()) 
    { 
      os << "fixed "; 
    } 
    if (f.is_text()) 
    { 
      os << "text "; 
    } 
    if (f.is_binary()) 
    { 
      os << "binary "; 
    } 
    if (f.is_integer()) 
    { 
      os << "integer "; 
    } 
    if (f.is_list()) 
    { 
      os << "list "; 
    } 
    if (f.is_cstr()) 
    { 
      os << "cstr "; 
    } 
    if (f.is_encodable()) 
    { 
      os << "mutable ";
    }
    return string(os.str(), 0, os.pcount());
  }

  string to_string(const frame& f)
  {
    char s[1024];
    ostrstream os(s, sizeof(s));
    os << "Frame: "
       << to_string(f.id()) << ", "
       << to_string(f.version()) << ", "
       << f.size() << " bytes: ";
    if (f.read_only()) 
    { 
      os << "readonly "; 
    } 
    if (f.compression()) 
    { 
      os << "compression "; 
    }
    return string(os.str(), 0, os.pcount());
  }

  string to_string(const tag& t)
  {
    char s[1024];
    ostrstream os(s, sizeof(s));
    os << "Tag: " << t.size() << " bytes: ";
    return string(os.str(), 0, os.pcount());
  }

  void populate(frame& f)
  {
    if (f.contains(fld::TEXTENC)) 
    { 
      f.get(fld::TEXTENC).assign(fld::ASCII); 
    } 
    if (f.contains(fld::TEXT)) 
    { 
      f.get(fld::TEXT).assign("text field"); 
    } 
    if (f.contains(fld::URL)) 
    { 
      f.get(fld::URL).assign("http://random.url"); 
    } 
    if (f.contains(fld::DATA)) 
    { 
      f.get(fld::DATA).assign(to_bstring("random data")); 
    } 
    if (f.contains(fld::DESCRIPTION)) 
    { 
      f.get(fld::DESCRIPTION).assign("this is a description"); 
    } 
    if (f.contains(fld::OWNER)) 
    { 
      f.get(fld::OWNER).assign("Scott Thomas Haug"); 
    } 
    if (f.contains(fld::EMAIL)) 
    { 
      f.get(fld::EMAIL).assign("sth2@cs.wustl.edu"); 
    } 
    if (f.contains(fld::RATING)) 
    { 
      f.get(fld::RATING).assign(256); 
    } 
    if (f.contains(fld::FILENAME)) 
    { 
      f.get(fld::FILENAME).assign("nofile.txt"); 
    } 
    if (f.contains(fld::LANGUAGE)) 
    { 
      f.get(fld::LANGUAGE).assign("english"); // should only be 3 chars 
    } 
    if (f.contains(fld::PICTURETYPE)) 
    { 
      f.get(fld::PICTURETYPE).assign(5); 
    } 
    if (f.contains(fld::IMAGEFORMAT)) 
    { 
      f.get(fld::IMAGEFORMAT).assign("image format"); // 3 chars 
    } 
    if (f.contains(fld::MIMETYPE)) 
    { 
      f.get(fld::MIMETYPE).assign("image/jpg"); 
    } 
    if (f.contains(fld::COUNTER)) 
    { 
      f.get(fld::COUNTER).assign(109); 
    } 
  }

};

// $Log$
