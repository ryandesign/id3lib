# Copyright (C) 1999 Scott Thomas Haug <sth2@cs.wustl.edu>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

INCLUDES = -I$(top_srcdir)/include
LDADD = $(top_builddir)/src/libid3.la

bin_PROGRAMS       = uint28 lib frame tag file
uint28_SOURCES     = test_uint28.cpp
lib_SOURCES        = test_lib.cpp
frame_SOURCES      = test_frame.cpp
tag_SOURCES        = test_tag.cpp
file_SOURCES       = test_file.cpp

EXTRA_DIST =            \
  jules.mp3             \
  jules-badtag.mp3      \
  jules-goodtag.mp3     \
  221-compressed.tag    \
  230-compressed.tag    \
  230-picture.tag       \
  230-unicode.tag
