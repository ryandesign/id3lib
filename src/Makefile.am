# $Id$
# 
# Copyright (C) 1999 Scott Thomas Haug <sth2@cs.wustl.edu>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

LTVERSION = @lt_ver@:@lt_rev@:@lt_age@

INCLUDES = -I$(top_srcdir)/include/id3

the_sources =                   \
  binary_field.cpp              \
  common_ops.cpp                \
  conversion_ops.cpp            \
  debug.cpp                     \
  details.cpp                   \
  field.cpp                     \
  field_defs.cpp                \
  frame.cpp                     \
  frame_defs.cpp                \
  frame_header.cpp              \
  header.cpp                    \
  integer_field.cpp             \
  lib.cpp                       \
  tag.cpp                       \
  tag_header.cpp                \
  text_field.cpp                \
  uint28.cpp                    

#EXTRA_DIST =                    \
#  dll_wrapper.cpp               


lib_LTLIBRARIES   = libid3.la 

libid3_la_SOURCES = $(the_sources)
libid3_la_LIBADD  = -lz
libid3_la_LDFLAGS = -version-info $(LTVERSION)

# $Log$
# Revision 1.9  2000/01/04 16:33:26  eldamitri
# (the_sources): Moved dll_wrapper.cpp to EXTRA_DIST.
#
# Revision 1.8  1999/12/02 01:49:12  scott
# Removed EXTRA_DIST (no more ChangeLog)
#
# Revision 1.7  1999/12/01 18:03:28  scott
# (INCLUDES): Now points to new, toplevel include directory.
# (the_headers, id3includedir, id3include_HEADERS): Removed.  Headers are
# no longer in this directory.
#
# Revision 1.6  1999/11/30 20:08:16  scott
# (the_headers): Added sized_headers.h
# (the_sources): Added version.cpp
# (LTVERSION): Updated to make use of configure.in variables.
#
