#include "BoardMng.h"
#include <thread>
#include <cassert>
#include <iostream>
#include "RookHandler.h"

BoardMng::BoardMng(int argc, char ** argv) :
  _rookCount(0),
  _run(false),
  _field{}
{
  parseArgs(argc, argv);
}

int BoardMng::run()
{
  _run = true;
  spawnRooks(_rookCount);
  while (_run)
  {
    MapIdToRookThread::node_type node;
    if (_terminatingQueue.tryPop(node))
    {
      terminateRook(node.key(), std::move(node.mapped()));
    }

    std::shared_lock lock(_fieldMutex);

    std::cout << "    a   b   c   d   e   f   g   h  \n"
                 "  +---+---+---+---+---+---+---+---+\n";
    for (int i = 0; i < 8; ++i)
    {
      std::cout << i + 1 << " ";
      for (int j = 0; j < 8; ++j)
      {
        if (_field[i][j] == 0)
          std::cout << "+   ";
        else
          std::cout << "+ " << _field[i][j] << " ";
      }
      std::cout << "+ " << 8 - i << "\n";
      std::cout << "  +---+---+---+---+---+---+---+---+\n";
    }

    std::cout << "    a   b   c   d   e   f   g   h  " << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  return 0;
}

void BoardMng::stop()
{
  std::cout << "Stop the application" << std::endl;
  _run = false;
}

bool BoardMng::tryMoveRook(uint32_t id, const RookPosition & oldPos, const RookPosition & newPos)
{
  std::unique_lock lock(_fieldMutex);

  if (hasPathCollision(oldPos, newPos))
    return false;
  std::cout << "Move rook(" << id << ") from " << oldPos << " to " << newPos << std::endl;
  _field[oldPos._x][oldPos._y] = 0;
  _field[newPos._x][newPos._y] = id;
  return true;
}

// Нельзя ждать поток в этой функции, потому что она, очевидно, будет вечно ждать...
void BoardMng::onRookFinished(uint32_t id)
{
  std::unique_lock lock(_fieldMutex);
  auto node = _activeThreads.extract(id);
  if (node.empty())
    return;
  _terminatingQueue.push(std::move(node));
}

void BoardMng::parseArgs(int argc, char ** argv)
{
  uint32_t rooksCount = 6;

  if (argc == 2)
  {
    rooksCount = std::stoi(argv[1]);
    if (rooksCount > 6)
      rooksCount = 6;
  }

  _rookCount = rooksCount;
}

void BoardMng::spawnRooks(uint32_t n)
{
  RookPosition basePos;
  uint32_t id = 1;
  for (uint32_t i = 0; i < n; ++i)
  {
    _field[basePos._x][basePos._y] = id;
    std::cout << "Spawn rook with id: " << id << " at " << basePos << std::endl;
    _activeThreads.emplace(id, std::thread([this, basePos, id] ()
    {
      RookHandler handler{*this, id, basePos};
      handler.run();
    }));
    // По главной диагонали
    ++basePos._x;
    ++basePos._y;
    ++id;
  }
}

void BoardMng::terminateRook(uint32_t id, std::thread thread)
{
  std::cout << "Rook(" << id << ") is finished, terminate the thread" << std::endl;
  thread.join();

  --_rookCount;
  std::cout << "Rooks remaining: " << _rookCount << std::endl;
  if (_rookCount == 0)
  {
    std::cout << "All rooks has turned for 50 times" << std::endl;
    stop();
  }
}

bool BoardMng::hasPathCollision(const RookPosition & oldPos, const RookPosition & newPos) const
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

