#ifndef HALF_SYNC_HALF_ASYNC_HPP
#define HALF_SYNC_HALF_ASYNC_HPP

#include "SyncEventHandler.hpp"
#include "AsyncEventHandler.hpp"
#include "EventDemultiplexer.hpp"
#include "../Threading/ThreadPool.hpp"

#include <map>

template <class SyncResult>
class HalfSyncHalfAsync
{
public:
  typedef typename std::shared_ptr<HalfSyncHalfAsync<SyncResult> > Ptr;
  typedef typename std::pair<typename SyncEventHandler<SyncResult>::Ptr, typename AsyncEventHandler<SyncResult>::Ptr> Handlers;

public:
  HalfSyncHalfAsync(EventDemultiplexer::Ptr, size_t);
  virtual ~HalfSyncHalfAsync();

  void add(const Handlers&);
  void modify(EventSource::Ptr);
  void remove(EventSource::Descriptor);
  void eventLoop();
  void interrupt(bool);

private:
  HalfSyncHalfAsync(const HalfSyncHalfAsync&);
  HalfSyncHalfAsync& operator=(const HalfSyncHalfAsync&);

private:
  typedef std::map<EventSource::Descriptor, Handlers> t_handlers;
  typedef std::pair<Handlers, EventSource::EventTypes> t_toHandle;
  typedef std::vector<t_toHandle> t_toHandles;

  std::mutex m_mutex;
  bool m_run;
  t_handlers m_handlers;
  EventDemultiplexer::Ptr m_eventDemultiplexer;
  Threading::ThreadPool m_threadPool;
}; //class HalfSyncHalfAsync

template <class SyncResult>
HalfSyncHalfAsync<SyncResult>::HalfSyncHalfAsync(EventDemultiplexer::Ptr p_ed, size_t p_tpSize)
  : m_run(false), m_eventDemultiplexer(p_ed), m_threadPool(p_tpSize)
{
  m_threadPool.start();
}

template <class SyncResult>
HalfSyncHalfAsync<SyncResult>::~HalfSyncHalfAsync()
{
  m_threadPool.stop(true);
}

template <class SyncResult>
void HalfSyncHalfAsync<SyncResult>::add(const Handlers& p_handlers)
{
  EventSource::Ptr es = p_handlers.first->getEventSource();
  EventSource::Descriptor syncD = es->getDescriptor();
  if (p_handlers.second && (syncD != p_handlers.second->getEventSource()->getDescriptor()))
  {
    throw std::runtime_error("Event handlers descriptors don't match");
  }

  { //m_mutex lock scope begin
    std::unique_lock<std::mutex> lock(m_mutex);
    m_eventDemultiplexer->add(es);
    m_handlers.insert(std::make_pair(syncD, p_handlers));
  } //m_mutex lock scope end
}

template <class SyncResult>
void HalfSyncHalfAsync<SyncResult>::modify(EventSource::Ptr p_eventSource)
{
  std::unique_lock<std::mutex> lock(m_mutex);
  m_eventDemultiplexer->modify(p_eventSource);
}

template <class SyncResult>
void HalfSyncHalfAsync<SyncResult>::remove(EventSource::Descriptor p_descriptor)
{
  { //m_mutex lock scope begin
    std::unique_lock<std::mutex> lock(m_mutex);
    auto i = m_handlers.find(p_descriptor);
    if (i != m_handlers.end())
    {
      m_eventDemultiplexer->remove(p_descriptor);
      m_handlers.erase(i);
    }
  } //m_mutex lock scope end
}

template <class SyncResult>
void HalfSyncHalfAsync<SyncResult>::eventLoop()
{
  m_run = true;
  while (m_run)
  {
    EventDemultiplexer::Events events;
    m_eventDemultiplexer->wait(events);

    t_toHandles toHandles;
    { //m_mutex lock scope begin
      std::unique_lock<std::mutex> lock(m_mutex);
      for (auto i = events.begin(); i != events.end(); ++i)
      {
        auto ih = m_handlers.find(i->descriptor);
        if (ih != m_handlers.end())
        {
          toHandles.push_back(t_toHandle(ih->second, i->eventTypes));
        }
        else
        {
          throw std::runtime_error("EventDemultiplexer returned event for \
												unfounded handler");
        }
      }
    } //m_mutex lock scope end

    for (auto i = toHandles.begin(); i != toHandles.end(); ++i)
    {
      auto aeh = i->first.second;
      SyncResult res = i->first.first->handle(i->second);
      if (aeh)
      {
        Threading::ThreadPool::Task t = [aeh, res](void)->void { aeh->handle(res); };
        m_threadPool.add(t);
      }
    }
  } //while (1)
}

template <class SyncResult>
void HalfSyncHalfAsync<SyncResult>::interrupt(bool p_immediately)
{
  m_run = false;
  m_eventDemultiplexer->interrupt();
}

#endif //HALF_SYNC_HALF_ASYNC_HPP

