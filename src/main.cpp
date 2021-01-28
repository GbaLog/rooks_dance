#include "BoardManager.h"
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

  if (res._rookCount > 6 || res._rookCount < 4)
  {
    throw std::invalid_argument{"Range of rooks must be from 4 to 6"};
  }

  return res;
}
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  try
  {
    BoardMngParams params = parseArgs(argc, argv);

    BoardManager mng{params};
    return mng.run();
  }
  catch (const std::invalid_argument & ex)
  {
    std::cout << "Error occured, you possibly enter wrong arguments: " << ex.what() << std::endl;
    std::cerr << "USAGE: " << argv[0] << " <number-of-rooks> [--nofield]" << std::endl;
    return EXIT_FAILURE;
  }
  catch (const std::exception & ex)
  {
    std::cerr << "Exception occured: " << ex.what() << std::endl;
    return EXIT_FAILURE;
  }
}
