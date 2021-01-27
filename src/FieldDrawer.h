#ifndef FIELDDRAWER_H
#define FIELDDRAWER_H

#include "CommonTypes.h"
#include <iostream>

inline
void drawField(const FieldRows & field)
{
  std::cout << "    a   b   c   d   e   f   g   h  \n"
               "  +---+---+---+---+---+---+---+---+\n";
  for (int i = 0; i < 8; ++i)
  {
    std::cout << i + 1 << " ";
    for (int j = 0; j < 8; ++j)
    {
      if (field[i][j] == 0)
        std::cout << "+   ";
      else
        std::cout << "+ " << field[i][j] << " ";
    }
    std::cout << "+ " << 8 - i << "\n";
    std::cout << "  +---+---+---+---+---+---+---+---+\n";
  }
  std::cout << "    a   b   c   d   e   f   g   h  " << std::endl;
}

#endif // FIELDDRAWER_H
