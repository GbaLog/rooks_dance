#include "RookHandler.h"
#include <thread>

RookHandler::RookHandler(IRookHandlerOwner & owner, uint32_t id, const RookPosition & pos) :
  _owner(owner),
  _id(id),
  _currentPos(pos),
  _nextPos(genNextPos()),
  _movesMade(0),
  _rand(id)
{}

void RookHandler::run()
{
  _wholeMoveExpire = std::chrono::steady_clock::now() + std::chrono::seconds(5);
  _moveExpire = std::chrono::steady_clock::now();

  auto now = std::chrono::steady_clock::now();
  while (true)
  {
    if (_wholeMoveExpire <= now)
    {
      genNewPosAndResetTimer(now);
    }

    if (_moveExpire <= now)
    {
      // BUG: тут такой момент, в конце концов все ладьи могут окружить одну,
      // в результате чего она не сможет никогда закончить ходить.
      if (_owner.tryMoveRook(_id, _currentPos, _nextPos))
      {
        _currentPos = _nextPos;
        ++_movesMade;
        genNewPosAndResetTimer(now);
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

void RookHandler::genNewPosAndResetTimer(const TimePointType & now)
{
  _nextPos = genNextPos();
  _wholeMoveExpire = now + std::chrono::seconds(5);
}

int RookHandler::generateInt(int min, int max)
{
  std::uniform_int_distribution<> dist(min, max);
  return dist(_rand);
}
