#if defined HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <id3/lib.h>

int main()
{
  cout << "NAME:     " << id3::lib::NAME << endl;
  cout << "RELEASE:  " << id3::lib::RELEASE << endl;
  cout << "FULLNAME: " << id3::lib::FULLNAME << endl;
  cout << "DATE:     " << id3::lib::DATE << endl;
  cout << "MAJOR:    " << id3::lib::MAJOR << endl;
  cout << "MINOR:    " << id3::lib::MINOR << endl;
  cout << "PATCH:    " << id3::lib::PATCH << endl;
}

