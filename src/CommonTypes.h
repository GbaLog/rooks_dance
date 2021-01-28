#ifndef COMMONTYPES_H
#define COMMONTYPES_H
//-----------------------------------------------------------------------------
#include <array>
#include <chrono>
//-----------------------------------------------------------------------------
using FieldColumn = std::array<int, 8>;
using FieldRows = std::array<FieldColumn, 8>;
//-----------------------------------------------------------------------------
using ClockType = std::chrono::steady_clock;
using TimePointType = ClockType::time_point;
//-----------------------------------------------------------------------------
#endif // COMMONTYPES_H
//-----------------------------------------------------------------------------
