// Copyright 1999 Scott Thomas Haug <sth2@cs.wustl.edu>
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//  
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//  
// $Id$

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

#include <fstream>
#include <iostream>
#include <id3/debug.h>
#include <id3/lib.h>
#include <getopt.h>

using id3::tag;
using id3::frame;
using id3::field;
using id3::frm;
using id3::fld;

void usage(const string& name)
{
  cout << "Usage: " << name << " [OPTION]... [FILE]..." << endl;
  cout << "Display the id3v2 tag information for a file." << endl;
  cout << endl;
  cout << "  -h, --help      Display this help and exit" << endl;
  cout << "  -v, --version   Display version information and exit" << endl;
  cout << endl;
}

void version(const string& name)
{
  cout << name << " 1.0" << endl;
  cout << "Displays id3v2 tag information - Written by Scott Thomas Haug" << endl;
  cout << "Uses " << id3::lib::FULLNAME << endl << endl;
}

void display(const tag& t)
{
  for (tag::const_iterator ti = t.begin(); ti != t.end(); ++ti)
  {
    frm::id id = ti->id();
    string divider;
    cout << "=== " << id3::to_string(id) << ": ";
    for (frame::const_iterator fi = ti->begin(); fi != ti->end(); ++fi)
    {
      const field& f = **fi;
      if (f.in_scope())
      {
        cout << divider;
        divider = "; ";
        switch (f.id())
        {
          case fld::TEXT:
          {
            size_t items = f.num_items();
            for (size_t i = 0; i < items; ++i)
            {
              cout << f.ascii(i);
              if ((i + 1) < items)
              {
                cout << ", ";
              }
            }
            break;
          }
          case fld::EMAIL:
          case fld::URL:
          {
            cout << f.ascii();
            break;
          }
          case fld::DESCRIPTION:
          {
            cout << "(" << f.ascii() << ")";
            break;
          }
          case fld::LANGUAGE:
          {
            cout << "[" << f.ascii() << "]";
            break;
          }
          case fld::DATA:
          {
            cout << f.size() << " bytes";
            break;
          }
          case fld::FILENAME:
          {
            cout << "[" << f.ascii() << "]";
            break;
          }
          case fld::PICTURETYPE:
          {
            cout << "[" << f.integer() << "]";
            break;
          }
          case fld::MIMETYPE:
          case fld::OWNER:
          {
            cout << f.ascii();
            break;
          }
          case fld::COUNTER:
          {
            cout << f.integer();
            break;
          }
          case fld::SYMBOL:
          {
            cout << "(" << f.integer() << ")";
            break;
          }
          case fld::TEXTENC:
          {
            fld::encoding enc = static_cast<fld::encoding>(f.integer());
            if (fld::UNICODE == enc)
            {
              cout << id3::to_string(enc);
            }
            else
            {
              divider = "";
            }
            break;
          }
          case fld::RATING:
          {
            cout << "rating=" << f.integer();
            break;
          }
          case fld::IMAGEFORMAT:
          case fld::VOLUMEADJ:
          case fld::NUMBITS:
          case fld::VOLCHGRIGHT:
          case fld::VOLCHGLEFT:
          case fld::PEAKVOLRIGHT:
          case fld::PEAKVOLLEFT:
          {
            cout << "(" << f.size() << " bytes)";
            break;
          }
        }
      }
    }
    cout << endl;
  }
}

int main( int argc, char *argv[])
{

  int opt;
  bool error = false;
  while (true)
  {
    int option_index = 0;
    int long_opt = 0;
    static struct option long_options[] = 
    { 
      { "version", no_argument, &long_opt, 'v' },
        { "help",    no_argument, &long_opt, 'h' },
          { 0, 0, 0, 0 }
    };
    opt = getopt_long (argc, argv, "vh", long_options, &option_index);

    if (opt == -1)
    {
      break;
    }

    if (opt == 0)
    {
      opt = long_opt;
    }

    switch (opt)
    {
      case 'v':
      {
        version(argv[0]);
        exit (0);
        break;
      }
      case 'h':
      {
        usage(argv[0]);
        exit (0);
        break;
      }
      case '?':
      {
        error = true;
        break;
      }
      default:
      {
        cout << "?? getopt returned character code " << opt << " ??" << endl;
      }
    }
  }
  if (error)
  {
    cout << "Try `" << argv[0] << " --help' for more information." << endl;
  }
  else
  {
    for (size_t index = optind; index < argc; index++)
    {
      try
      {
        tag t;
        ifstream file(argv[index]);

        file >> t;
        cout << endl << "*** Tag information for " << argv[index] << endl;

        display(t);
      }
      catch(...)
      {
        cout << "exception caught" << endl;
      }
    }
  }

  return 0;
}

// $Log$
