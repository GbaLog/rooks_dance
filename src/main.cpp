#include "BoardMng.h"
#include <iostream>
#include <string_view>
//-----------------------------------------------------------------------------
BoardMngParams parseArgs(int argc, char ** argv)
{
  BoardMngParams res;

  for (int i = 1; i < argc; ++i)
  {
    std::string_view arg{argv[i]};
    if (arg == "--nofield")
      res._noField = true;
  }

  if (argc > 1)
  {
    res._rookCount = std::stoi(argv[1]);
  }

  if (res._rookCount > 6)
    res._rookCount = 6;
  if (res._rookCount < 1)
    res._rookCount = 1;

  return res;
}
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  try
  {
    BoardMngParams params = parseArgs(argc, argv);

    BoardMng mng{params};
    return mng.run();
  }
  catch (const std::exception & ex)
  {
    std::cerr << "Exception occured: " << ex.what() << std::endl;
    return EXIT_FAILURE;
  }
}
