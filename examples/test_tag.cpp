#if defined HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <id3/debug.h>
#include <assert.h>

using namespace id3;

void display_tag(const tag& t)
{
  cout << to_string(t) << endl;
  for (tag::const_iterator ti = t.begin(); ti != t.end(); ++ti)
  {
    const frame& f = *ti;
    cout << to_string(f) << endl;
    for (frame::const_iterator fi = f.begin(); fi != f.end(); ++fi)
    {
      cout << to_string(**fi) << endl;
    }
    cout << endl;
  }
  cout << endl;
}


int main()
{
  tag tag1, tag2;
  for (size_t id = 0; id < frm::NUMFRAMES; ++id)
  {
    frame f(static_cast<frm::id>(id));
    populate(f);
    tag::iterator ti = tag1.add(f);
  }

  tag2 = tag1;
  tag1.clear();
  
  display_tag(tag1);
  display_tag(tag2);
}
