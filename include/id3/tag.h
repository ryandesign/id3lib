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

#ifndef ID3LIB_TAG_H
#define ID3LIB_TAG_H

#include <list>
#include "tag_header.h"
#include "frame.h"

using std::list;
using std::string;

namespace id3
{
  /** The representative class of an id3 tag.
   **
   ** This is the 'container' class for everything else.  It is through a
   ** tag that most of the productive stuff happens.  Let's look at what's
   ** required to start using ID3v2 tags.
   **
   ** \code 
   **   #include<id3/tag.h>
   ** \endcode
   **
   ** This simple \c #include does it all.  In order to parse an existing tag
   ** from a file...
   **
   ** \code
   **   id3::tag tag;
   **   std::ifstream file("foo.mp3");
   **   file >> tag;
   ** \endcode
   **
   ** That's all there is to it.  Now all that is necessary is to use the
   ** has_frame_id class, along with the \c std::find_if function, to locate
   ** the frames of interest.
   **
   ** \code
   **   id3::has_frame_id search(id3::frm::TITLE);
   **   id3::tag::iterator ti = std::find_if(tag.begin(), tag.end(), search);
   **   if (ti != my_tag.end() && ti->contains(id3::fld::TEXT))
   **   {
   **     id3::string title = ti->get(fld::TEXT).ascii();
   **     std::cout << "Title: " << title << std::endl;
   **   }
   ** \endcode
   **
   ** This code snippet locates the \c TITLE frame, returns the contents of
   ** text field as an ASCII string, and displays the string.  Not difficult, 
   ** eh?
   **
   ** \author Scott Haug
   ** \version $Id$
   ** \sa frame
   ** \sa field
   **/
  class tag
  {
  public:
    typedef list<frame> frames;
    typedef frames::iterator iterator;
    typedef frames::const_iterator const_iterator;

    tag();
    tag(const tag&);
    ~tag();
    
    tag&           operator=(const tag&);   // header & frames
    
    size_t         size() const;            // header & frames
    bool           changed() const;         // header & frames
    size_t         num_frames() const;      // frames
    size_t         padding() const;         // padding
    spec::version  version() const;         // header
    tag_header     hdr() const;             // header
    bool           unsync() const;          // header
    bool           extended_header() const; // header
    
    iterator       begin();                 // frames
    const_iterator begin() const;           // frames
    iterator       end();                   // frames
    const_iterator end() const;             // frames
  
    void           clear();                 // header & frames
    bool           version(spec::version);  // header
    bool           unsync(bool);            // header
    bool           extended_header(bool);   // header
    bool           padding(size_t);         // padding
    iterator       add(const frame&);       // frames
    iterator       erase(iterator&);        // frames
    iterator       erase(const iterator&);  // frames

    template <class Predicate>
    void           remove_if(Predicate);    // frames
  
    size_t         parse(const bstring&);   // header & frames
    bstring        render() const;          // header & frames
  private:
    tag_header     __hdr;           // the header associated with this tag
    frames         __frames;        // list of frames
    bool           __padding;       // how much padding to add to a tag
    mutable bool   __changed;       // whether the tag has changed since parse/render
  };

  istream& operator>>(istream&, tag&);

  template <class Predicate>
  void tag::remove_if(Predicate pred)
  {
    __frames.remove_if(pred);
  }
};

#endif

// $Log$
// Revision 1.6  2000/01/04 15:42:22  eldamitri
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.5  1999/12/17 16:05:02  scott
// Updated opening comment block.
//
// Revision 1.4  1999/12/13 04:23:25  scott
// (): Include sys/param.h (if available) to define MAXPATHLEN.
// (class tag): Made private methods protected.  Changed __sFileName
// from a char * to a char array of size MAXPATHLEN+1.
//
// Revision 1.3  1999/12/09 02:45:59  scott
// (class tag): Added copy constructor and operator= method
// declarations.
//
// Revision 1.2  1999/12/02 22:45:28  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places.
//
// Revision 1.1  1999/12/01 17:16:10  scott
// moved from src/id3 to include/id3
//
// Revision 1.10  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.9  1999/11/29 19:09:31  scott
// Updated documentation to work better with doc++ (probably doesn't work
// well with kdoc anymore).
//
// Revision 1.8  1999/11/25 19:22:18  scott
// (): Added doc++/kdoc/javadoc-like documentation, mostly transcribed
// from Dirk's ID3Lib Documentation MSWord doc.
// (tagTypes): Added.  The different types of tags that can be parsed
// and/or rendered by id3lib.
// (class tag): Changed the interface to AddFrame slightly.  Now there
// is two AddTag-like methods: AddTag and AddNewTag.  The former merely
// attaches the frame to the tag without taking repsonsibility for
// deallocating the frame when the tag goes out of scope or is deleted.
// The latter does take this responsibility.  Granted, there could be a
// better interface for this, but it works for now...  Update and Strip
// now return flags which indicate what tags were actually updated or
// stripped.  Removed OpenLinkedFile method; replaced with
// OpenFileForReading, OpenFileForWriting, and CreateFile.
//
// Revision 1.7  1999/11/19 19:07:13  scott
// * tag.h: Added new constants: STR_V1_COMMENT_DESC (to be added to
// the description of comments converted from id3v1 tags); V1_TAG,
// V2_TAG, and BOTH_TAGS (used for methods Strip and Update to
// determine which tag to act on); and LEN_V1, LEN_V1_ID,
// LEN_V1_TITLE, LEN_V1_ARTIST, LEN_V1_ALBUM, LEN_V1_YEAR,
// LEN_V1_COMMENT, LEN_V1_GENRE (the lengths of the id3v1 tag and its
// fields). Generalized ID3V1_Tag struct using newly defined
// constants.  Added 1 to each char array size for null terminator.
// Added const qualifier to appropriate methods and parameters.  Added
// declaration of RenderV1ToHandle method.  Renamed "RenderToHandle" to
// "RenderV2ToHandle".
//
// Revision 1.6  1999/11/15 20:20:30  scott
// Made variable names more descriptive.  Added const qualifier to
// appropriate methods.
//
// Revision 1.5  1999/11/04 04:32:11  scott
// Initial revision
//
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
