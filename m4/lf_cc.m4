dnl Autoconf support for C++
dnl Copyright (C) 1988 Eleftherios Gkioulekas <lf@amath.washington.edu>
dnl  
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl 
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl 
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software 
dnl Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
dnl 
dnl As a special exception to the GNU General Public License, if you 
dnl distribute this file as part of a program that contains a configuration 
dnl script generated by Autoconf, you may include it under the same 
dnl distribution terms that you use for the rest of that program.

# -------------------------------------------------------------------------
# Use this macro to configure your C compiler
# When called the macro does the following things:
# 1. It finds an appropriate C compiler.
#    If you passed the flag --with-cc=foo then it uses that
#    particular compiler
# 2. Check whether the compiler works.
# 3. Checks whether the compiler accepts the -g 
# -------------------------------------------------------------------------

AC_DEFUN(LF_CONFIGURE_CC,[
  dnl Sing the song
  AC_PROG_CC
  AC_PROG_CPP
  AC_AIX
  AC_ISC_POSIX
  AC_MINIX 
  AC_HEADER_STDC
])

