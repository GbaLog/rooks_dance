#ifndef BOARDMNG_H
#define BOARDMNG_H
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
class BoardMng : public IRookHandlerOwner
{
public:
  BoardMng(const BoardMngParams & params);

  int run();
  void stop();

  // IRookHandlerOwner
  virtual bool tryMoveRook(uint32_t id, const RookPosition & oldPos,
                           const RookPosition & newPos) override;
  virtual void onRookFinished(uint32_t id) override;
  virtual void onMoveExpired(uint32_t id, const RookPosition & oldPos, const RookPosition & newPos) override;
  virtual void onWayChosen(uint32_t id, const RookPosition & newPos) override;

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

  void spawnRooks(int n);
  void terminateRook(RookHandlerPtr handler);
  void runSelf();
  void processLogMsg(Log::Message msg);

  bool hasPathCollision(const RookPosition & oldPos, const RookPosition & newPos) const;
};
//-----------------------------------------------------------------------------
#endif // BOARDMNG_H
//-----------------------------------------------------------------------------
