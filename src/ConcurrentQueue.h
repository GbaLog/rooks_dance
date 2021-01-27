#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H
//-----------------------------------------------------------------------------
#include <deque>
#include <mutex>
#include <condition_variable>
//-----------------------------------------------------------------------------
template <typename T>
class ConcurrentQueue
{
public:
  template <typename V>
  void push(V && value)
  {
    std::unique_lock<std::mutex> lock(_mutex);
    _queue.push_back(std::forward<V>(value));
    lock.unlock();
    _cond.notify_one();
  }

  template <typename V>
  void pushFront(V && value)
  {
    std::unique_lock<std::mutex> lock(_mutex);
    _queue.push_front(std::forward<V>(value));
    lock.unlock();
    _cond.notify_one();
  }

  bool tryPop(T & value)
  {
    std::unique_lock<std::mutex> lock(_mutex);
    if(_queue.empty())
      return false;
    value = std::move(_queue.front());
    _queue.pop_front();
    return true;
  }

  void pop(T & value)
  {
    std::unique_lock<std::mutex> lock(_mutex);
    while(_queue.empty())
    {
      _cond.wait(lock);
    }
    value = std::move(_queue.front());
    _queue.pop_front();
  }

  void clear()
  {
    std::unique_lock<std::mutex> lock(_mutex);
    _queue.clear();
  }

private:
  std::deque<T> _queue;
  mutable std::mutex _mutex;
  std::condition_variable _cond;
};
//-----------------------------------------------------------------------------
#endif // CONCURRENTQUEUE_H
