// $Id$

#include <iostream.h>
#include <getopt.h>
#include <stdlib.h>

#include <id3/tag.h>
#include <id3/misc_support.h>

int main( int argc, char *argv[])
{
  try
  {
    ID3_Tag tag;
    ID3_Frame frame;

    tag.Link("test-230-unicode.tag");
    tag.Strip(ID3TT_ALL);
    tag.Update();

    tag.Clear();
    frame.SetID(ID3FID_USERTEXT);
    frame.Field(ID3FN_DESCRIPTION).Set("example text frame");
    frame.Field(ID3FN_TEXT).Set("This text and the description should be in Unicode.");
    frame.Field(ID3FN_TEXTENC).Set(ID3TE_UNICODE);
    tag.AddFrame(frame);

    tag.SetPadding(false);
    tag.SetUnsync(false);
    tag.Update(ID3TT_ID3V2);
  }   
  catch(const ID3_Error& err)
  {
    cout << endl;
    cout << err.GetErrorFile() << " (" << err.GetErrorLine() << "): "
         << err.GetErrorType() << ": " << err.GetErrorDesc() << endl;
  }

  return 0;
}
