#ifndef ROOKHANDLER_H
#define ROOKHANDLER_H
//-----------------------------------------------------------------------------
#include <random>
#include <thread>
#include <atomic>
#include "Rook.h"
#include "CommonTypes.h"
//-----------------------------------------------------------------------------
struct IRookHandlerOwner
{
  virtual ~IRookHandlerOwner() = default;

  // Если на пути нет препятствий, то переставляет ладью на новую позицию.
  virtual bool tryMoveRook(uint32_t id, const RookPosition & oldPos, const RookPosition & newPos) = 0;

  // Сообщает, что эта ладья сделала 50 ходов.
  virtual void onRookFinished(uint32_t id) = 0;

  virtual void onMoveExpired(uint32_t id, const RookPosition & oldPos, const RookPosition & newPos) = 0;
  virtual void onWayChosen(uint32_t id, const RookPosition & newPos, uint32_t movesRemain) = 0;
};
//-----------------------------------------------------------------------------
class RookHandler
{
public:
  // NOTE: Maybe owner as std::weak/shared_ptr?
  RookHandler(IRookHandlerOwner & owner, uint32_t id, const RookPosition & pos, uint32_t seed);
  ~RookHandler();

  uint32_t id() const { return _id; }

  void run();
  void stop();

private:
  IRookHandlerOwner & _owner;
  const uint32_t _id;
  std::atomic_bool _run;
  std::thread _workThread;
  std::mt19937 _rand;
  TimePointType _wholeMoveExpire;
  TimePointType _moveExpire;
  RookPosition _currentPos;
  RookPosition _nextPos;
  uint32_t _movesMade;

  enum
  {
    MaxMovesCount = 50,
    SleepIntervalMSec = 10
  };

  void runInThread();

  RookPosition genNextPos();
  void onWholeTimerExpired(const TimePointType & now);
  void onMoveMade(const TimePointType & now);
  int generateInt(int min, int max);
};
//-----------------------------------------------------------------------------
#endif // ROOKHANDLER_H
//-----------------------------------------------------------------------------
