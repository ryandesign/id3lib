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

#include <algorithm>
#include "tag.h"
#include "debug.h"

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{

  /** Default constructor
   **/
  tag::tag()
  {
    this->clear();
  }

  /** Standard copy constructor.
   **
   ** \param tag The tag to copy.
   **/
  tag::tag(const tag& rhs)
  {
    this->clear();
    *this = rhs;
  }
  
  /** Destructor
   **/
  tag::~tag()
  {
    this->clear();
  }

  tag& tag::operator=(const tag& rhs)
  {
    if (this != &rhs)
    {
      this->clear();
      __hdr = rhs.hdr();
      __frames = rhs.__frames;
      this->padding(rhs.padding());
      __changed = true;
    }
    return *this;
  }

  /** Returns the size of the rendered tag, not counting the effects of 
   ** unsynchronization or padding.
   **
   ** \code
   **   if (my_tag.changed())
   **   {
   **     size_t size = my_tag.size();
   **     if (size)
   **     {
   **       bstring data = my_tag.render();
   **       // do something useful with the data
   **       // like push it down a socket
   **     }
   **   }
   ** \endcode
   ** 
   ** The effects of unsynchronization of the binary tag data aren't known 
   ** until the entire tag is rendered, therefore the tag's size is calculated
   ** without unsynchronization considered.
   **
   ** Likewise, padding is often dependant on factors outside the actual
   ** content of the tag, so the amount of padding must be explicitely set and
   ** is not included in the size calculation for the tag.
   **
   ** Therefore, the most accurate way to determine the size of a rendered
   ** tag is as follows.
   **
   ** \code
   **   size_t tag_size = my_tag.render().size();
   ** \endcode
   **
   ** \sa render()
   ** \return The size of the rendered tag in bytes, not counting 
   **         unsynchronization or padding.
   **/
  size_t tag::size() const
  {
    size_t size = __hdr.size();
  
    for (const_iterator p = this->begin(); p != this->end(); ++p)
    {
      size += p->size();
    }
    
    return size;
  }

  /** Indicates whether the tag has been altered since the last parse or render
   **
   ** This method is primarily intended as a status indicator for applications
   ** that use the parse(const bstring&) and render() methods.
   **
   ** Setting a field, changing the id of an attached frame, setting grouping
   ** or encryption id's, and clearing a frame or field all constitute a change
   ** to the tag, as do calls to the unsync(bool), extended_header(bool), 
   ** compression(bool0), and padding(size_t) methods.
   **
   ** \code
   **   if (my_tag.changed())
   **   {
   **     bstring data = my_tag.render();
   **   }
   ** \endcode
   **
   ** @return Whether or not the tag has been altered.
   **/
  bool tag::changed() const
  {
    bool changed = __changed || __hdr.changed();
    
    for (const_iterator p = this->begin(); !changed && p != this->end(); ++p)
    {
      changed = p->changed();
    }
    
    return changed;
  }

  /** Returns the number of frames present in the tag object.
   **
   ** @return The number of frames present in the tag object.
   **/
  size_t tag::num_frames() const
  {
    return __frames.size();
  }

  /**
   **/
  size_t tag::padding() const
  {
    return __padding;
  }

  /**
   **/
  spec::version tag::version() const
  {
    return __hdr.version();
  }

  /**
   **/
  tag_header tag::hdr() const
  {
    return __hdr;
  }

  /**
   **/
  bool tag::unsync() const
  {
    return __hdr.test(tag_header::UNSYNC);
  }

  /**
   **/
  bool tag::extended_header() const
  {
    return __hdr.test(tag_header::EXTENDEDHEADER);
  }

  /**
   **/
  tag::iterator tag::begin()
  {
    return __frames.begin();
  }

  /**
   **/
  tag::const_iterator tag::begin() const
  {
    return __frames.begin();
  }

  /**
   **/
  tag::iterator tag::end()
  {
    return __frames.end();
  }

  /**
   **/
  tag::const_iterator tag::end() const
  {
    return __frames.end();
  }

  /** Clears the object.
   **
   ** It frees any resources for which the object is responsible, and the
   ** object is now free to be used again for any new or existing tag.
   **/
  void tag::clear()
  {
    __frames.clear();
    __hdr.clear();
    __changed = true;
  }

  /** Change the version of the tag.
   **
   ** If the given version is unrecognized by the tag, the most current 
   ** recognized version will be used instead.
   **/
  bool tag::version(spec::version v)
  {
    bool changed = __hdr.version(v);
    spec::version ver = __hdr.version();
    for (iterator p = this->begin(); p != this->end(); ++p)
    {
      changed = p->version(ver) || changed;
    }
    return changed;
  }

  /** Turns unsynchronization on or off
   **
   ** Passing 'false' as the parameter will turn off unsynchronization,
   ** regardless of the content of the tag.  On the other hand, setting unsync
   ** to 'true' for a tag that doesn't need unsynchronization will affect the
   ** rendered tag.  This option is useful when the file is only going to be
   ** used by ID3v2-compliant software.  See the id3v2 standard document for
   ** futher details on unsynchronization.
   **
   ** Be default, unsynchronization is turned off
   **
   ** \code
   **   my_tag.unsync(false);
   ** \endcode
   **
   ** @param b Whether the tag should be unsynchronized
   **/
  bool tag::unsync(bool b)
  {
    return __hdr.set(tag_header::UNSYNC, b);
  }

  /** Turns extended header rendering on or off.
   **
   ** This option is currently ignored as id3lib doesn't yet create extended
   ** headers.  This option only applies when rendering tags for id3v2 versions
   ** that support extended headers.  
   **
   ** By default, id3lib will generate extended headers for all tags in which
   ** extended headers are supported.
   **
   ** \code
   **   my_tag.extended_header(true);
   ** \endcode
   **
   ** @param b Whether to render an extended header
   **/
  bool tag::extended_header(bool b)
  {
    return __hdr.set(tag_header::EXTENDEDHEADER, b);
  }

  /** Set the amount of padding to be used when the tag is rendered.
   **
   ** When using id3v2 tags in association with files, id3lib can optionally
   ** add padding to the tags to ensure minmal file write times when updating
   ** the tag in the future.
   **
   ** The 'ID3v2 Programming Guidelines' specify that enough padding should
   ** be added to tags rendered at the beginning of the file to round out the
   ** entire size of the file (song plus tag) to an even multiple of 2K bytes.
   **
   ** This calculation, however, must be done outside the tag proper, since
   ** the tag class (alone) is not file-aware.  Therefore, any padding amount
   ** can be specified using this method.
   **
   ** One caveat is that if unsynchronization is turned on for a tag, any 
   ** additional data added to the tag due to unsynchronization will result
   ** in an equal amount of padding removed.  In this manner, the overall tag
   ** size should remain unchanged, regardless of unsynchronization.  However,
   ** if more data added due to unsynchronization than is allowed for in 
   ** padding, the size of the rendered tag will grow accordingly.
   **
   **    
   ** id3lib's addition to the guidelines for padding, is that if frames are
   ** removed from a pre-existing tag (or the tag simply shrinks because of
   ** other reasons), the new tag will continue to stay the same size as the
   ** old tag (with padding making the difference of course) until such time as
   ** the padding is greater than 4K.  When this happens, the padding will be
   ** reduced and the new tag will be smaller than the old.
   **
   ** By default, no padding is specified.
   **
   ** \code
   **   my_tag.padding(my_tag.size() % 2048);
   ** \endcode
   **
   ** @param b The amount of padding to add to a rendered tag.
   **/
  bool tag::padding(size_t padding)
  {
    bool changed = __padding != padding;
    if (changed)
    {
      __padding = padding;
      __changed = true;
    }
    return changed;
  }
  
  /** Adds a copy of the frame to the tag
   **
   ** \code
   **   frame my_frame;
   **   my_tag.add(my_frame);
   ** \endcode
   **
   ** @param frame The frame that is being copied to the tag.
   ** @return The iterator that points to this frame.
   ** @see frame
   **/
  tag::iterator tag::add(const frame& frame)
  {
    __frames.push_back(frame);
    __changed = true;
    iterator fi = this->end();

    return --fi;
  }
  
  /** Erases a frame (represented by an iterator) from the tag.
   **
   ** Use \c std::find_if with one of the find classes to obtain a frame
   ** iterator to pass to this method.
   ** 
   ** \code
   **   tag::iterator ti = std::find_if(tag.begin(), tag.end(), has_frame_id(frame::TITLE));
   **   if (ti != my_tag.end())
   **   {
   **     my_tag.erase(ti);
   **   }
   ** \endcode
   **   
   ** @param fi A frames iterator that points to the frame to erase
   ** @return An iterator
   **/
  tag::iterator tag::erase(iterator& fi)
  {
    return __frames.erase(fi);
  }

  tag::iterator tag::erase(const iterator& fi)
  {
    return __frames.erase(fi);
  }

  // To be used when reading an ID3v2-tag
  // Transforms all FF 00 sequences into FF

  void resync(bstring& data)
  {
    bstring::const_iterator src = data.begin();
    bstring::iterator dest = data.begin();
    for (; (src + 1) != data.end() && src != data.end(); ++src, ++dest)
    {
      if (src > dest)
      {
        *dest = *src;
      }

      if (0xFF == *src && '\0' == *(src + 1))
      {
        ++src;
      }
    }
    data.resize(data.size() - (src - dest));
  }

  // Determine if cur is at a point in the pStart buffer where unsyncing is 
  // necessary
  bool should_unsync(const bstring::const_iterator cur, 
                     const bstring::const_iterator begin, 
                     const bstring::const_iterator end)
  {
    const bstring::const_iterator next = cur + 1;
    // The current byte is a sync if it's equal to 0xFF and 
    // 1) It's the last byte in the file, or
    // 2) It comes before 111xxxxx (second part of an mp3 sync), or
    // 3) It comes before 00000000 (consequence of unsyncing)
    return
      (cur           >= begin ) &&
      (cur           <  end)    &&
      (*cur          == 0xFF) && // first sync
      ((next         == end)    || // last byte?
       (*next & 0xE0 == 0xE0)   || // second sync
       (*next        == 0x00));    // second null
  }

  // To be used when writing an ID3v2-tag
  // Transforms:
  // 11111111 111xxxxx -> 11111111 00000000 111xxxxx
  // 11111111 00000000 -> 11111111 00000000 00000000
  // 11111111 <EOF> -> 11111111 00000000 <EOF>

  bstring unsync(const bstring& data)
  {
    bstring new_data;
    // Now do the real transformation
    for (bstring::const_iterator bi = data.begin(); bi != data.end(); ++bi)
    {
      // Copy the current character from source to destination
      new_data += *bi;

      // If we're at a sync point in the source, insert an extra null character
      // in the destination buffer
      if (should_unsync(bi, data.begin(), data.end()))
      {
        new_data += static_cast<uchar>('\0');
      }
    }
  }

  /** Renders a binary image of the tag into a binary string.
   **
   ** When rendered, however, any additional data added due to 
   ** unsynchronization will eat into space reserved for padding.  If more data
   ** is added than is allowed for in padding, the rendered tag will be forced
   ** to exceed the size calculated by this method.
   **
   ** Before calling this method, it is advisable to call changed() first as
   ** this will let you know whether you should bother rendering the tag.
   **
   ** @see    is_tag
   ** @see    changed
   ** @return The rendered tag as a binary string
   **/
  bstring tag::render() const
  {
    bstring data;

    tag tag = *this;
    tag.version(spec::CURRENT);
    
    tag_header hdr = tag.hdr();
    for (iterator p = tag.begin(); p != tag.end(); ++p)
    {
      data += p->render();
    }
    
    size_t before_sync = data.size();
    if (hdr.test(tag_header::UNSYNC))
    {
      data = id3::unsync(data);
    }
    
    size_t padding_adjust = 
      std::max(size_t(), tag.padding() - data.size() + before_sync);
    // zero the remainder of the buffer so that our padding bytes are zero
    data.resize(data.size() + padding_adjust);
    
    hdr.data_size(data.size());
    data = hdr.render() + data;
    
    // set the flag which says that the tag hasn't changed
    __changed = false;
  
    return data;
  }

  /** Parse given binary data to populate the tag, returning the number of
   ** bytes parsed.
   **
   ** \code
   **   id3::tag tag;
   **   
   **   // this will parse the source buffer and 
   **   // extract all tag information.  it will
   **   // return the number of bytes parsed total.
   **   // if 0 is the return value, the source 
   **   // buffer did not begin with a tag.
   **   size_t size = tag.parse(buffer);
   **  
   **   if (size)
   **   {
   **     // do something with the objects,
   **     // like look for titles, artists, etc.
   **   }
   ** \endcode
   **
   ** \param data The binary data source.
   **/
  size_t tag::parse(const bstring& buffer)
  {
    this->clear();
    size_t hdr_size = __hdr.parse(buffer);
    if (!hdr_size)
    {
      return 0;
    }
    size_t data_size = __hdr.data_size();
    bstring data = buffer;
    data.erase(0, hdr_size);
    if (this->unsync())
    {
      resync(data);
    }

    //
    //  Here would be a good spot to check for 2.1 compression.
    //

    // Now parse all of the frames.  Attach all of them for now.
    while (data.size() > 0 && data[0] != '\0')
    {
      frame f;
      size_t frame_size = f.parse(data);
      if (frame_size > 0)
      {
        if (!f.bad_parse())
        {
          this->add(f);
        }
        data.erase(0, frame_size);
      }
      else
      {
        // This is a problem.  If the frame size is 0, then we can't progress.
        // Break for now.
        break;
      }
    }

    // set the flag which says that the tag hasn't changed
    __changed = false;

    return hdr_size + data_size - data.size();
  }

  istream& operator>>(istream& is, tag& tag)
  {
    streampos pos = is.tellg();
    uchar header[tag_header::SIZE + 1];
    is.read(header, tag_header::SIZE);
    is.seekg(pos);

    bstring hdr(header, tag_header::SIZE);
    size_t size = is_tag(bstring(header, tag_header::SIZE));

    if (!size)
    {
      return is;
    }

    uchar* data = new uchar[size + 1];
    is.read(data, size);
    tag.parse(bstring(data, size));
    delete [] data;

    return is;
  }
}

// $Log$
// Revision 1.13  2000/01/04 15:42:49  eldamitri
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.12  1999/12/27 05:30:03  scott
// (SetupTag): Updated for new version constants.
//
// Revision 1.11  1999/12/17 16:13:04  scott
// Updated opening comment block.
//
// Revision 1.10  1999/12/13 04:44:17  scott
// (SetupTag, ~tag): Slight changes to reflect new __sFileName type.
//
// Revision 1.9  1999/12/09 03:32:28  scott
// (tag): Added copy constructor implementation.
// (operator=): Added implementation.
//
// Revision 1.8  1999/12/01 18:00:59  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places (and to make compiling under
// windows easier).
//
// Revision 1.7  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.6  1999/11/25 18:24:27  scott
// (SetupTag): Initialized sFileName to NULL rather than the empty string
// to indicate no filename is present.
// (tag): Deleted sFileName if allocated.  Added call to CloseFile.
// (AddFrame): Modified to reflect change in interface.  AddFrame now adds
// a frame without taking responsibility for deallocating the memory when
// the tag goes out of scope.
// (AddNewFrame): New method.  AddNewFrame adds a frame and takes
// responsibility for deallocating the frame when the tag goes out of
// scope.
//
// Revision 1.5  1999/11/15 20:20:17  scott
// Added include for config.h.  Minor code cleanup.  Removed
// assignments from if checks; first makes assignment, then checks
// for appropriate value.  Made private member variable names more
// descriptive.
//
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
