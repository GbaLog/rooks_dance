#include "BoardMng.h"
#include <iostream>
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  try
  {
    BoardMng mng{argc, argv};
    return mng.run();
  }
  catch (const std::exception & ex)
  {
    std::cerr << "Exception occured: " << ex.what() << std::endl;
    return EXIT_FAILURE;
  }
}
