#ifndef BOARDMNG_H
#define BOARDMNG_H
//-----------------------------------------------------------------------------
#include <map>
#include <mutex>
#include <thread>
#include "RookHandler.h"
#include "CommonTypes.h"
#include "ConcurrentQueue.h"
//-----------------------------------------------------------------------------
class BoardMng : public IRookHandlerOwner
{
public:
  BoardMng(int argc, char ** argv);

  int run();
  void stop();

  // IRookHandlerOwner
  virtual bool tryMoveRook(uint32_t id, const RookPosition & oldPos,
                           const RookPosition & newPos) override;
  virtual void onRookFinished(uint32_t id) override;

private:
  uint32_t _rookCount;
  bool _run;
  using MapIdToRookThread = std::map<uint32_t, std::thread>;
  using MapIdToRookThreadNode = MapIdToRookThread::node_type;
  MapIdToRookThread _activeThreads;

  ConcurrentQueue<MapIdToRookThreadNode> _terminatingQueue;

  std::mutex _fieldMutex;
  FieldRows _field;

  void parseArgs(int argc, char ** argv);
  void spawnRooks(uint32_t n);
  void terminateRook(uint32_t id, std::thread thread);

  bool hasPathCollision(const RookPosition & oldPos, const RookPosition & newPos) const;
};
//-----------------------------------------------------------------------------
#endif // BOARDMNG_H
//-----------------------------------------------------------------------------
