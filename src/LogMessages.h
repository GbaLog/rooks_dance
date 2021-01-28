#ifndef LOGMESSAGES_H
#define LOGMESSAGES_H
//-----------------------------------------------------------------------------
#include <variant>
#include "CommonTypes.h"
#include "Rook.h"
//-----------------------------------------------------------------------------
namespace Log
{
//-----------------------------------------------------------------------------
struct MoveMade
{
  uint32_t _id;
  RookPosition _old;
  RookPosition _new;
};
//-----------------------------------------------------------------------------
struct MoveExpired
{
  uint32_t _id;
  RookPosition _old;
  RookPosition _new; // re-generated
};
//-----------------------------------------------------------------------------
struct NextPosition
{
  uint32_t _id;
  RookPosition _new;
};
//-----------------------------------------------------------------------------
using MessageBody = std::variant<MoveMade, MoveExpired, NextPosition>;
//-----------------------------------------------------------------------------
enum class Type
{
  MoveMade,
  MoveExpired,
  NextPosition
};
//-----------------------------------------------------------------------------
struct Message
{
  Type _type;
  MessageBody _body;
};
//-----------------------------------------------------------------------------
} // namespace Log
//-----------------------------------------------------------------------------
#endif // LOGMESSAGES_H
//-----------------------------------------------------------------------------
