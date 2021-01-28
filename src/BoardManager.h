#ifndef BOARDMANAGER_H
#define BOARDMANAGER_H
//-----------------------------------------------------------------------------
#include <map>
#include <mutex>
#include "RookHandler.h"
#include "CommonTypes.h"
#include "ConcurrentQueue.h"
#include "LogMessages.h"
//-----------------------------------------------------------------------------
struct BoardMngParams
{
  int _rookCount = 6;
  bool _noField = false;
};
//-----------------------------------------------------------------------------
class BoardManager : public IRookHandlerOwner
{
public:
  BoardManager(const BoardMngParams & params);

  int run();
  void stop();

  // IRookHandlerOwner
  bool tryMoveRook(uint32_t id, const RookPosition & oldPos,
                   const RookPosition & newPos) override;
  void onRookFinished(uint32_t id) override;
  void onMoveExpired(uint32_t id,
                     const RookPosition & oldPos, const RookPosition & newPos) override;
  void onWayChosen(uint32_t id, const RookPosition & newPos, uint32_t movesRemain) override;

private:
  int _rookCount;
  bool _noField;
  bool _run;
  using RookHandlerPtr = std::unique_ptr<RookHandler>;
  using MapIdToRookHandler = std::map<uint32_t, RookHandlerPtr>;
  MapIdToRookHandler _activeHandlers;

  ConcurrentQueue<RookHandlerPtr> _terminatingQueue;
  ConcurrentQueue<Log::Message> _logQueue;

  std::mutex _fieldMutex;
  FieldRows _field;

  void spawnRooks(int count);
  void terminateRook(RookHandlerPtr handler);
  void runSelf();
  void processLogMsg(Log::Message msg);

  bool hasPathCollision(const RookPosition & oldPos, const RookPosition & newPos) const;
};
//-----------------------------------------------------------------------------
#endif // BOARDMANAGER_H
//-----------------------------------------------------------------------------
