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

#include "field.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{
  const field::def field::default_binary =
  {
    fld::DATA,                        // field name
    fld::BINARY,                      // field type
    0,                                // fixed size
    spec::VER_2_0,                    // spec version
    fld::HIGHER,                      // version control scope
    fld::NOFLAGS,                     // flags
    fld::NOFIELD                      // linked field
  };

  const field::def field::default_text =
  {
    fld::TEXT,                        // field name
    fld::TEXTSTRING,                  // field type
    0,                                // fixed size
    spec::VER_2_0,                    // spec version
    fld::HIGHER,                      // version control scope
    fld::NOFLAGS,                     // flags
    fld::NOFIELD                      // linked field
  };

  const field::def field::default_integer =
  {
    fld::DATA,                        // field name
    fld::INTEGER,                     // field type
    0,                                // fixed size 
    spec::VER_2_0,                    // spec version
    fld::HIGHER,                      // version control scope
    fld::NOFLAGS,                     // flags
    fld::NOFIELD                      // linked field
  };

  const field::def field::binary_data[] =
  {
    {
      fld::DATA,                        // field name
      fld::BINARY,                      // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    { fld::NOFIELD }
  };

  const field::def field::url[] =
  { 
    {
      fld::URL,                         // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    { fld::NOFIELD }
  };
  
  const field::def field::user_url[] =
  {
    {
      fld::TEXTENC,                     // field name        
      fld::INTEGER,                     // field type
      1,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::DESCRIPTION,                 // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::CSTR | fld::ENCODABLE,       // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::URL,                         // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    { fld::NOFIELD }
  };
  
  const field::def field::text[] =
  {
    {
      fld::TEXTENC,                     // field name
      fld::INTEGER,                     // field type
      1,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::TEXT,                        // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::ENCODABLE,                   // flags
      fld::NOFIELD                      // linked field
    },
    { fld::NOFIELD }
  };
  
  
  const field::def field::user_text[] =
  {
    {
      fld::TEXTENC,                     // field name
      fld::INTEGER,                     // field type
      1,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::DESCRIPTION,                 // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::CSTR | fld::ENCODABLE,       // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::TEXT,                        // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::ENCODABLE,                   // flags
      fld::NOFIELD                      // linked field
    },
    { fld::NOFIELD }
  };
  
  
  const field::def field::general_text[] =
  {
    {
      fld::TEXTENC,                     // field name
      fld::INTEGER,                     // field type
      1,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::LANGUAGE,                    // field name
      fld::TEXTSTRING,                  // field type
      3,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::DESCRIPTION,                 // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::CSTR | fld::ENCODABLE,       // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::TEXT,                        // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::ENCODABLE,                   // flags
      fld::NOFIELD                      // linked field
    },
    { fld::NOFIELD }
  };

  const field::def field::terms_of_use[] =
  {
    {
      fld::TEXTENC,                     // field name
      fld::INTEGER,                     // field type
      1,                                // fixed size
      spec::VER_3_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::LANGUAGE,                    // field name
      fld::TEXTSTRING,                  // field type
      3,                                // fixed size
      spec::VER_3_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::TEXT,                        // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_3_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::ENCODABLE,                   // flags
      fld::NOFIELD                      // linked field
    },
    { fld::NOFIELD }
  };

  const field::def field::linked_info[] =
  {
    {
      fld::DATA,                        // field name
      fld::INTEGER,                     // field type
      3,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::LOWER,                       // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::TEXT,                        // field name
      fld::TEXTSTRING,                  // field type
      3,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::LOWER,                       // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::TEXT,                        // field name
      fld::TEXTSTRING,                  // field type
      4,                                // fixed size
      spec::VER_3_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::URL,                         // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::CSTR,                        // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::TEXT,                        // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    { fld::NOFIELD }
  };

  const field::def field::picture[] =
  {
    {
      fld::TEXTENC,                     // field name
      fld::INTEGER,                     // field type
      1,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::IMAGEFORMAT,                 // field name
      fld::TEXTSTRING,                  // field type
      3,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::LOWER,                       // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::MIMETYPE,                    // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_3_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::CSTR,                        // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::PICTURETYPE,                 // field name
      fld::INTEGER,                     // field type
      1,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::DESCRIPTION,                 // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::CSTR | fld::ENCODABLE,       // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::DATA,                        // field name
      fld::BINARY,                      // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    { fld::NOFIELD }
  };
  
  const field::def field::geo[] =
  {
    {
      fld::TEXTENC,                     // field name
      fld::INTEGER,                     // field type
      1,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::MIMETYPE,                    // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::CSTR,                        // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::FILENAME,                    // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::CSTR | fld::ENCODABLE,       // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::DESCRIPTION,                 // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::CSTR | fld::ENCODABLE,       // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::DATA,                        // field name
      fld::BINARY,                      // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    { fld::NOFIELD }
  };
  
  const field::def field::ufi[] =
  {
    {
      fld::OWNER,                       // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::CSTR,                        // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::DATA,                        // field name
      fld::BINARY,                      // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    { fld::NOFIELD }
  };
  
  const field::def field::play_counter[] =
  {
    {
      fld::COUNTER,                     // field name
      fld::INTEGER,                     // field type
      4,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    { fld::NOFIELD }
  };
  
  const field::def field::popularimeter[] =
  {
    {
      fld::EMAIL,                       // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::CSTR,                        // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::RATING,                      // field name
      fld::INTEGER,                     // field type
      1,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::COUNTER,                     // field name
      fld::INTEGER,                     // field type
      4,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    { fld::NOFIELD }
  };
  
  const field::def field::registration[] =
  {
    {
      fld::OWNER,                       // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_3_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::CSTR,                        // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::SYMBOL,                      // field name
      fld::INTEGER,                     // field type
      1,                                // fixed size
      spec::VER_3_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::DATA,                        // field name
      fld::BINARY,                      // field type
      0,                                // fixed size
      spec::VER_3_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    { fld::NOFIELD }
  };
  
  const field::def field::text_list[] =
  {
    {
      fld::TEXTENC,                     // field name
      fld::INTEGER,                     // field type
      1,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::TEXT,                        // field name
      fld::TEXTSTRING,                  // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::TEXTLIST,                    // flags
      fld::NOFIELD                      // linked field
    },
    { fld::NOFIELD }
  };

  // Currently unused
#if defined __UNDEFINED__
  const field::def field::volume[] =
  {
    {
      fld::VOLUMEADJ,                   // field name
      fld::INTEGER,                     // field type
      1,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::NUMBITS,                     // field name
      fld::INTEGER,                     // field type
      1,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::NOFLAGS,                     // flags
      fld::NOFIELD                      // linked field
    },
    {
      fld::VOLCHGRIGHT,                 // field name
      fld::BITFIELD,                    // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::ADJUSTEDBY,                  // flags
      fld::NUMBITS                      // linked field
    },
    {
      fld::VOLCHGLEFT,                  // field name
      fld::BITFIELD,                    // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::ADJUSTEDBY,                  // flags
      fld::NUMBITS                      // linked field
    },
    {
      fld::PEAKVOLRIGHT,                // field name
      fld::BITFIELD,                    // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::ADJUSTEDBY,                  // flags
      fld::NUMBITS                      // linked field
    },
    {
      fld::PEAKVOLLEFT,                 // field name
      fld::BITFIELD,                    // field type
      0,                                // fixed size
      spec::VER_2_0,                    // spec version
      fld::HIGHER,                      // version control scope
      fld::ADJUSTEDBY,                  // flags
      fld::NUMBITS                      // linked field
    },
    { fld::NOFIELD }
  };
#endif

}

// $Log$
