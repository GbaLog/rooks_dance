#ifndef ROOKHANDLER_H
#define ROOKHANDLER_H

#include <random>
#include "Rook.h"
#include "CommonTypes.h"

struct IRookHandlerOwner
{
  virtual ~IRookHandlerOwner() = default;

  // Если на пути нет препятствий, то переставляет ладью на новую позицию.
  // Возвращает результат вычисления.
  virtual bool tryMoveRook(uint32_t id, const RookPosition & oldPos, const RookPosition & newPos) = 0;

  // Сообщает, что эта ладья сделала 50 ходов.
  virtual void onRookFinished(uint32_t id) = 0;
};

// Обработчик ходов ладьи. Живёт полностью в другом потоке.
class RookHandler
{
public:
  RookHandler(IRookHandlerOwner & owner, uint32_t id, const RookPosition & pos);

  void run();

private:
  IRookHandlerOwner & _owner;
  const uint32_t _id;
  TimePointType _wholeMoveExpire;
  TimePointType _moveExpire;
  RookPosition _currentPos;
  RookPosition _nextPos;
  uint32_t _movesMade;
  std::mt19937 _rand;

  enum
  {
    MaxMovesCount = 50,
    SleepIntervalMSec = 10
  };

  RookPosition genNextPos();
  void genNewPosAndResetTimer(const TimePointType & now);
  int generateInt(int min, int max);
};

#endif // ROOKHANDLER_H
