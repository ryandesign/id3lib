/*
** This file has been automatically generated by 'acconfig' from aclocal.m4
** Copyright (C) 1988 Eleftherios Gkioulekas <lf@amath.washington.edu>
**  
** This file is free software; as a special exception the author gives
** unlimited permission to copy and/or distribute it, with or without 
** modifications, as long as this notice is preserved.
** 
** This program is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
** implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* This is the top section */

@TOP@
/* This is the template section */

/* These are standard for all packages using Automake */
#undef PACKAGE
#undef VERSION

/* And now the rest of the boys */
#undef CXX_HAS_BUGGY_FOR_LOOPS
#undef CXX_HAS_NO_BOOL

/* config.h defines these preprocesser symbols to be used by id3lib for
 * determining internal versioning information.  The intent is that these
 * macros will be made available in the library via constants, functions,
 * or static methods.
 */
#undef ID3LIB_DEBUG
#undef _ID3LIB_NAME
#undef _ID3LIB_DATE
#undef _ID3LIB_VERSION
#undef _ID3LIB_FULLNAME
#undef _ID3LIB_MAJOR_VERSION
#undef _ID3LIB_MINOR_VERSION
#undef _ID3LIB_PATCH_VERSION
#undef _ID3LIB_INTERFACE_AGE
#undef _ID3LIB_BINARY_AGE
#undef _ID3_COMPILED_WITH_DEBUGGING
/* */

@BOTTOM@
/* This is the bottom section */

// This file defines portability work-arounds for various proprietory
// C++ compilers

// Workaround for compilers with buggy for-loop scoping
// That's quite a few compilers actually including recent versions of
// Dec Alpha cxx, HP-UX CC and SGI CC.
// The trivial "if" statement provides the correct scoping to the 
// for loop

#ifdef CXX_HAS_BUGGY_FOR_LOOPS
#undef for
#define for if(1) for
#endif

//
// If the C++ compiler we use doesn't have bool, then
// the following is a near-perfect work-around. 
// You must make sure your code does not depend on "int" and "bool"
// being two different types, in overloading for instance.
//

#ifdef CXX_HAS_NO_BOOL
#define bool int
#define true 1
#define false 0
#endif
   

