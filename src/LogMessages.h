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
enum class Type
{
  MoveMade,
  MoveExpired,
  NextPosition
};
//-----------------------------------------------------------------------------
struct MoveMade
{
  constexpr static Type type() noexcept { return Type::MoveMade; }

  uint32_t _id;
  RookPosition _old;
  RookPosition _new;
};
//-----------------------------------------------------------------------------
struct MoveExpired
{
  constexpr static Type type() noexcept { return Type::MoveExpired; }

  uint32_t _id;
  RookPosition _old;
  RookPosition _new; // re-generated
};
//-----------------------------------------------------------------------------
struct NextPosition
{
  constexpr static Type type() noexcept { return Type::NextPosition; }

  uint32_t _id;
  RookPosition _new;
  uint32_t _movesRemain;
};
//-----------------------------------------------------------------------------
using MessageBody = std::variant<MoveMade, MoveExpired, NextPosition>;
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
