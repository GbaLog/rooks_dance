#include "BoardManager.h"
#include <iostream>
#include <random>
#include "RookHandler.h"
#include "FieldDrawer.h"
//-----------------------------------------------------------------------------
namespace
{
//-----------------------------------------------------------------------------
template<class T, class ... Args>
Log::Message makeLogMessage(Args &&... args)
{
  Log::Message msg;
  msg._type = T::type();
  msg._body = T{std::forward<Args>(args)...};
  return msg;
}
//-----------------------------------------------------------------------------
} // namespace
//-----------------------------------------------------------------------------
BoardManager::BoardManager(const BoardMngParams & params) :
  _rookCount{params._rookCount},
  _noField{params._noField},
  _run{false},
  _field{}
{}
//-----------------------------------------------------------------------------
int BoardManager::run()
{
  _run = true;
  spawnRooks(_rookCount);
  runSelf();
  return 0;
}
//-----------------------------------------------------------------------------
void BoardManager::stop()
{
  std::cout << "Stop the application" << std::endl;
  _run = false;
}
//-----------------------------------------------------------------------------
bool BoardManager::tryMoveRook(uint32_t id, const RookPosition & oldPos, const RookPosition & newPos)
{
  std::lock_guard lock(_fieldMutex);

  // BUG: тут такой момент, в конце концов все ладьи могут окружить одну,
  // в результате чего она не сможет никогда закончить ходить.

  if (hasPathCollision(oldPos, newPos))
    return false;

  _logQueue.push(makeLogMessage<Log::MoveMade>(id, oldPos, newPos));
  _field[oldPos._x][oldPos._y] = 0;
  _field[newPos._x][newPos._y] = id;
  return true;
}
//-----------------------------------------------------------------------------
// Нельзя ждать поток в этой функции, потому что она, очевидно, будет вечно ждать...
void BoardManager::onRookFinished(uint32_t id)
{
  std::lock_guard lock(_fieldMutex);
  auto node = _activeHandlers.extract(id);
  if (node.empty())
    return;
  _terminatingQueue.push(std::move(node.mapped()));
}
//-----------------------------------------------------------------------------
void BoardManager::onMoveExpired(uint32_t id, const RookPosition & oldPos, const RookPosition & newPos)
{
  _logQueue.push(makeLogMessage<Log::MoveExpired>(id, oldPos, newPos));
}
//-----------------------------------------------------------------------------
void BoardManager::onWayChosen(uint32_t id, const RookPosition & newPos, uint32_t movesRemain)
{
  _logQueue.push(makeLogMessage<Log::NextPosition>(id, newPos, movesRemain));
}
//-----------------------------------------------------------------------------
void BoardManager::spawnRooks(int count)
{
  RookPosition basePos;
  uint32_t id = 1;
  std::random_device rd;

  for (uint32_t i = 0; i < count; ++i)
  {
    _field[basePos._x][basePos._y] = id;
    std::cout << "Spawn rook with id: " << id << " at " << basePos << std::endl;

    auto handler = std::make_unique<RookHandler>(*this, id, basePos, rd());
    _activeHandlers.emplace(id, std::move(handler));

    // По главной диагонали
    ++basePos._x;
    ++basePos._y;
    ++id;
  }

  std::lock_guard lock(_fieldMutex);

  // Запускаем потоки
  for (auto & [_, handler] : _activeHandlers)
  {
    handler->run();
  }
}
//-----------------------------------------------------------------------------
void BoardManager::terminateRook(RookHandlerPtr handler)
{
  std::cout << "Rook(" << handler->id() << ") is finished, terminate the thread" << std::endl;
  handler->stop();

  --_rookCount;
  std::cout << "Rooks remaining: " << _rookCount << std::endl;
  if (_rookCount == 0)
  {
    std::cout << "All rooks has turned for 50 times" << std::endl;
    stop();
  }
}
//-----------------------------------------------------------------------------
void BoardManager::runSelf()
{
  while (_run)
  {
    bool changed = false;
    Log::Message msg;
    while (_logQueue.tryPop(msg))
    {
      changed = changed || msg._type == Log::Type::MoveMade;
      processLogMsg(std::move(msg));
    }

    RookHandlerPtr handler;
    if (_terminatingQueue.tryPop(handler))
    {
      terminateRook(std::move(handler));
    }

    if (changed && _noField == false)
    {
      FieldRows copy;
      {
        std::lock_guard lock(_fieldMutex);
        copy = _field;
      }
      drawField(copy);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}
//-----------------------------------------------------------------------------
void BoardManager::processLogMsg(Log::Message msg)
{
  switch (msg._type)
  {
  case Log::Type::MoveMade:
    {
      auto & mv = std::get<Log::MoveMade>(msg._body);
      std::cout << "Move Rook(" << mv._id  << ") from " << mv._old << " to " << mv._new << std::endl;
      break;
    }
  case Log::Type::MoveExpired:
    {
      auto & mv = std::get<Log::MoveExpired>(msg._body);
      std::cout << "Rook(" << mv._id  << ")'s move to " << mv._old
                << " expired, now go to " << mv._new << std::endl;
      break;
    }
  case Log::Type::NextPosition:
    {
      auto & mv = std::get<Log::NextPosition>(msg._body);
      std::cout << "Rook(" << mv._id  << ") will go to " << mv._new
                << ", moves remain: " << mv._movesRemain << std::endl;
      break;
    }
  default:
    break;
  }
}
//-----------------------------------------------------------------------------
bool BoardManager::hasPathCollision(const RookPosition & oldPos, const RookPosition & newPos) const
{
  if (oldPos._x != newPos._x)
  {
    // left -> right
    for (int i = oldPos._x + 1; i <= newPos._x; ++i)
    {
      if (_field[i][oldPos._y] != 0) return true;
    }
    // right -> left
    for (int i = newPos._x; i <= oldPos._x - 1; ++i)
    {
      if (_field[i][oldPos._y] != 0) return true;
    }
  }
  if (oldPos._y != newPos._y)
  {
    // top -> bottom
    for (int i = oldPos._y + 1; i <= newPos._y; ++i)
    {
      if (_field[oldPos._x][i] != 0) return true;
    }
    // bottom -> top
    for (int i = newPos._y; i <= oldPos._y - 1; ++i)
    {
      if (_field[oldPos._x][i] != 0) return true;
    }
  }

  return false;
}
//-----------------------------------------------------------------------------
