#if defined HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <id3/uint28.h>

int main()
{
  const size_t power = 28;
  for (size_t i = 0; i < ((1 << power) - 1); ++i)
  {
    id3::uint28 i2, i1 = i;
    i2.parse(i1.render());
    //cout << endl;
    //cout << "i1.to_uint32() = " << i1.to_uint32() << endl;
    //cout << "i2.to_uint32() = " << i2.to_uint32() << endl;
    if (i1.to_uint32() != i2.to_uint32())
    {
      cout << "*** doesn't work for " << i << endl;
    }
    else if (i % 10000000 == 0)
    {
      cout << "*** works for " << i << endl;
    }
    
  }
  
}

