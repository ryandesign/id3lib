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
#include "conversion_ops.h"

using std::string;

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

namespace id3
{
  // Extract a 32-bit integer from a 4-byte character array
  uint32 to_integer(const uchar* buffer, size_t size)
  {
    uint32 num = 0;
    for (size_t index = 0; index < std::min(size, sizeof(uint32)); index++)
    {
      num |= buffer[index] << ((size - index - 1) * 8);
    }
    return num;
  }

  uint32 to_integer(const bstring& data, size_t bytes)
  {
    uint32 num = 0;
    const size_t size = std::min(data.size(), bytes);
    for (size_t i = 0; i < size; i++)
    {
      num |= data[i] << ((size - i - 1) * 8);
    }
    return num;
  }

  bstring to_bstring(uint32 val, size_t s)
  {
    bstring data;
    const size_t size = std::min(s, sizeof(uint32));
    for (size_t index = 0; index < size; index++)
    {
      data += static_cast<uchar>((val >> ((size - index - 1) * 8)) & MASK8);
    }
    return data;
  }
  
  void add_wchar(bstring& bs, wchar_t ch)
  {
    bs += static_cast<unsigned char>(ch >> 8);
    bs += static_cast<unsigned char>(ch & MASK8);
  }
  
  bstring to_bstring(const wstring& ws)
  {
    bstring bs;
    bs.reserve(ws.size() * 2 + 2);  // reserve two bytes for ever char plus BOM
    add_wchar(bs, BOM);             // add the BOM char
    for (wstring::const_iterator wsi = ws.begin(); wsi != ws.end(); ++wsi)
    {
      add_wchar(bs, *wsi);          // add each char
    }
    return bs;
  }
  
  bstring to_bstring(const string& s)
  {
    return bstring(s.begin(), s.end());
  }
  
  string to_string(const wstring& ws)
  {
    return string(ws.begin(), ws.end());
  }
  
  string to_string(const bstring& bs)
  {
    return string(bs.begin(), bs.end());
  }
  
  wchar_t to_wchar(const uchar ch1, const uchar ch2)
  {
    return static_cast<wchar_t>(ch1 << 8 | ch2);
  }
  
  wchar_t to_wchar(const uchar* uch)
  {
    if (NULL == uch)
    {
      return 0;
    }
    return to_wchar(uch[0], uch[1]);
  }
  
  wstring to_wstring(const bstring& bs)
  {
    // the BOM
    if (bs.empty())
    {
      return wstring();
    }
    
    // prepare the binary string to be converted
    bstring data = bs;
    data.resize(data.size() + data.size() % 2);
    wchar_t bom = to_wchar(data.data());
    if (BOM == bom || MOB == bom)
    {
      data = data.substr(2);
      if (bom == MOB)
      {
        for (bstring::iterator bsi = data.begin(); bsi != data.end(); bsi += 2)
        {
          std::swap(*bsi, *(bsi+1));
        }
      }
    }
    
    // convert the data to a wstring
    wstring ws;
    ws.reserve(data.size() / 2);
    for (bstring::iterator bsi = data.begin(); bsi != data.end(); bsi += 2)
    {
      ws += to_wchar(&*bsi);
    }
    return ws;
  }

  wstring to_wstring(const string& s)
  {
    return wstring(s.begin(), s.end());
  }
}

// $Log$
