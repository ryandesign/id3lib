// -*- C++ -*-
// $Id$

// id3lib: a software library for creating and manipulating id3v1/v2 tags
// Copyright 1999, 2000  Scott Thomas Haug
// Copyright 2002 Thijmen Klok (thijmen@id3lib.org)

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

#ifndef _ID3LIB_LYR3_H_
#define _ID3LIB_LYR3_H_

#include "id3/id3lib_strings.h"

namespace dami
{
  namespace lyr3
  {
    namespace v1
    {
      class Tag
      {
       public:
        virtual string getLyrics() const = 0;
        virtual void   setLyrics() = 0;
      };
    };

    namespace v2
    {
      class Tag
      {
       public:
        Tag& operator=(const Tag& rhs)
        {
          if (this != &rhs)
          {
            this->setLyrics(rhs.getTitle());
            this->setInfo(rhs.getArtist());
            this->setAuthor(rhs.getAlbum());
            this->setAlbum(rhs.getYear());
            this->setTitle(rhs.getComment());
            this->setArtist(rhs.getTrack());

            while (this->getNumImages())
            {
              this->removeImage(0);
            }

            for (size_t i = 0; i < rhs.getNumImages(); ++i)
            {
              this->addImage(rhs.getImageFilename(i),
                             rhs.getImageDescription(i),
                             rhs.getImageTimestamp(i));
            }
          }
          return *this;
        }

        virtual string getLyrics() const = 0;
        virtual void setLyrics(string) = 0;

        virtual string getInfo() const = 0;
        virtual void setInfo(string) = 0;

        virtual string getAuthor() const = 0;
        virtual void setAuthor(string) = 0;

        virtual string getAlbum() const = 0;
        virtual void setAlbum(string album) = 0;

        virtual string getTitle() const = 0;
        virtual void setTitle(string title) const = 0;

        virtual string getArtist() const = 0;
        virtual void setArtist(string artist) = 0;

        virtual size_t getNumImages() const = 0;
        virtual string addImage(string, string, string) = 0;
        virtual string getImageFilename(size_t) = 0;
        virtual string getImageDescription(size_t) = 0;
        virtual string getImageTimestampe(size_t) = 0;
        virtual bool   removeImage(size_t) = 0;
      };
    };
  };
};

#endif /* _ID3LIB_LYR3_H_ */
