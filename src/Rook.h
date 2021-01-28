#ifndef ROOK_H
#define ROOK_H
//-----------------------------------------------------------------------------
#include <tuple>
#include <sstream>
//-----------------------------------------------------------------------------
struct RookPosition
{
  int _x = 0;
  int _y = 0;
};
//-----------------------------------------------------------------------------
constexpr bool operator ==(const RookPosition & lhs, const RookPosition & rhs)
{ return std::tie(lhs._x, lhs._y) == std::tie(rhs._x, rhs._y); }
//-----------------------------------------------------------------------------
constexpr bool operator <(const RookPosition & lhs, const RookPosition & rhs)
{ return std::tie(lhs._x, lhs._y) < std::tie(rhs._x, rhs._y); }
//-----------------------------------------------------------------------------
inline
std::ostream & operator <<(std::ostream & strm, const RookPosition & pos)
{
  return strm << "(" << pos._x + 1 << ", " << char('a' + pos._y) << ")";
}
//-----------------------------------------------------------------------------
#endif // ROOK_H
//-----------------------------------------------------------------------------
