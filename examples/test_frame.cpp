#if defined HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <id3/debug.h>

using namespace id3;

void display_frame(const frame& f)
{
  cout << to_string(f) << endl;
  for (frame::const_iterator fi = f.begin(); fi != f.end(); ++fi)
  {
    cout << to_string(**fi) << endl;
  }
  cout << endl;
}


int main()
{
  //tag tag1, tag2;
  frame f1, f2;

  cout << "*** ***" << endl << endl;
  display_frame(f1);
  display_frame(f2);

  f1.id(frm::COMMENT);
  f1.get(fld::TEXT).assign("comment text");
  f1.get(fld::DESCRIPTION).assign("comment description");
  f1.get(fld::LANGUAGE).assign("english");

  cout << "*** ***" << endl << endl;
  display_frame(f1);
  display_frame(f2);

  f2 = f1;

  cout << "*** ***" << endl << endl;
  display_frame(f1);
  display_frame(f2);

  f1.clear();

  cout << "*** ***" << endl << endl;
  display_frame(f1);
  display_frame(f2);

  f1.id(frm::TITLE);
  f1.get(fld::TEXT).assign("a title");

  cout << "*** ***" << endl << endl;
  display_frame(f1);
  display_frame(f2);

  f2.clear();

  cout << "*** ***" << endl << endl;
  display_frame(f1);
  display_frame(f2);


  /*
  for (size_t id = 0; id < frm::NUMFRAMES; ++id)
  {
    frame f (static_cast<frm::id>(id));
    frame* pf = &f;
    pop_frame(f);
    tag::iterator ti = tag1.add(f);
    cout << "added a new frame:" << endl;
    display_frame(*ti);
    frame 
      *pti = &*ti, 
      *begin = &*(tag1.begin()), 
      *end = &*(tag1.end());
    cout << "*** (pf, pti, begin, end) = " << hex 
         << pf << " " << pti << " " << begin << " " << end << dec << endl;
    display_frame(*(tag1.begin()));
    cout << endl;
  }

  cout << "frm::NUMFRAMES = " << static_cast<int>(frm::NUMFRAMES) << endl;
  cout << "tag1.num_frames() = " << tag1.num_frames() << endl;

  cout << "&*tag1.begin() = " << &*(tag1.begin()) << endl;
  display_frame(*(tag1.begin()));

  //tag2 = tag1;
  //tag1.clear();
  
  for (tag::iterator ti = tag1.begin(); ti != tag1.end(); ++ti)
  {
    cout << "&*ti = " << &*ti << endl;
    display_frame(*ti);
  }
  */
}
