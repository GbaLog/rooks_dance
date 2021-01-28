#include "RookHandler.h"
#include <thread>
#include <iostream>
//-----------------------------------------------------------------------------
RookHandler::RookHandler(IRookHandlerOwner & owner, uint32_t id, const RookPosition & pos, uint32_t seed) :
  _owner{owner},
  _id{id},
  _run{false},
  _rand{seed},
  _currentPos{pos},
  _movesMade{0}
{}
//-----------------------------------------------------------------------------
RookHandler::~RookHandler()
{
  if (_run)
    stop();
}
//-----------------------------------------------------------------------------
void RookHandler::run()
{
  _run = true;
  _nextPos = genNextPos();
  _workThread = std::thread(&RookHandler::runInThread, this);
}
//-----------------------------------------------------------------------------
void RookHandler::stop()
{
  _run = false;
  _workThread.join();
}
//-----------------------------------------------------------------------------
void RookHandler::runInThread()
{
  _wholeMoveExpire = std::chrono::steady_clock::now() + std::chrono::seconds(5);
  _moveExpire = std::chrono::steady_clock::now();

  auto now = std::chrono::steady_clock::now();
  while (_run)
  {
    if (_wholeMoveExpire <= now)
    {
      onWholeTimerExpired(now);
    }

    if (_moveExpire <= now)
    {
      if (_owner.tryMoveRook(_id, _currentPos, _nextPos))
      {
        onMoveMade(now);
      }
      _moveExpire = now + std::chrono::milliseconds(generateInt(200, 300));
    }

    if (_movesMade >= MaxMovesCount)
    {
      _owner.onRookFinished(_id);
      break;
    }

    now += std::chrono::milliseconds(SleepIntervalMSec);
    std::this_thread::sleep_until(now);
  }
}
//-----------------------------------------------------------------------------
RookPosition RookHandler::genNextPos()
{
  RookPosition newPos;
  do
  {
    newPos = _currentPos;
    // Просто генерирует новую x или y координату случайным образом.
    if (generateInt(0, 1) == 0)
    {
      newPos._x = generateInt(0, 7);
    }
    else
    {
      newPos._y = generateInt(0, 7);
    }
  } while (newPos == _currentPos);

  return newPos;
}
//-----------------------------------------------------------------------------
void RookHandler::onWholeTimerExpired(const TimePointType & now)
{
  RookPosition newPos = genNextPos();
  while (newPos == _nextPos)
    newPos = genNextPos();

  _owner.onMoveExpired(_id, _nextPos, newPos);
  _nextPos = newPos;
  _wholeMoveExpire = now + std::chrono::seconds(5);
}
//-----------------------------------------------------------------------------
void RookHandler::onMoveMade(const TimePointType & now)
{
  ++_movesMade;
  _currentPos = _nextPos;
  _nextPos = genNextPos();
  _wholeMoveExpire = now + std::chrono::seconds(5);
  _owner.onWayChosen(_id, _nextPos, MaxMovesCount - _movesMade);
}
//-----------------------------------------------------------------------------
int RookHandler::generateInt(int min, int max)
{
  std::uniform_int_distribution<> dist(min, max);
  return dist(_rand);
}
//-----------------------------------------------------------------------------
