# Microsoft Developer Studio Project File - Name="id3lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=id3lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "id3lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "id3lib.mak" CFG="id3lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "id3lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "id3lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "id3lib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".\\" /I "..\\" /I "..\include" /I "..\include\id3" /I "..\zlib\include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_WINDOWS" /D "HAVE_CONFIG_H" /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\id3lib.lib"

!ELSEIF  "$(CFG)" == "id3lib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".\\" /I "..\\" /I "..\include" /I "..\include\id3" /I "..\zlib\include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_WINDOWS" /D "HAVE_CONFIG_H" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\id3lib.lib"

!ENDIF 

# Begin Target

# Name "id3lib - Win32 Release"
# Name "id3lib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\id3\error.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\field.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\field_binary.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\field_integer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\field_string_ascii.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\field_string_unicode.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\frame.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\frame_parse.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\frame_render.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\header.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\header_frame.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\header_tag.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\int28.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\misc_support.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\tag.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\tag_file.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\tag_find.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\tag_parse.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\tag_parse_lyrics3.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\tag_parse_v1.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\tag_render.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\tag_sync.cpp
# End Source File
# Begin Source File

SOURCE=..\src\id3\version.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\config.h
# End Source File
# Begin Source File

SOURCE=..\include\id3\dll.h
# End Source File
# Begin Source File

SOURCE=..\include\id3\error.h
# End Source File
# Begin Source File

SOURCE=..\include\id3\externals.h
# End Source File
# Begin Source File

SOURCE=..\include\id3\field.h
# End Source File
# Begin Source File

SOURCE=..\include\id3\frame.h
# End Source File
# Begin Source File

SOURCE=..\include\id3\globals.h
# End Source File
# Begin Source File

SOURCE=..\include\id3\header.h
# End Source File
# Begin Source File

SOURCE=..\include\id3\header_frame.h
# End Source File
# Begin Source File

SOURCE=..\include\id3\header_tag.h
# End Source File
# Begin Source File

SOURCE=..\include\id3\int28.h
# End Source File
# Begin Source File

SOURCE=..\include\id3\misc_support.h
# End Source File
# Begin Source File

SOURCE=..\include\id3\sized_types.h
# End Source File
# Begin Source File

SOURCE=..\include\id3\tag.h
# End Source File
# Begin Source File

SOURCE=..\include\id3\types.h
# End Source File
# Begin Source File

SOURCE=..\include\id3\version.h
# End Source File
# End Group
# End Target
# End Project
