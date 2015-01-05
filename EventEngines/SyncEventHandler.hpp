#ifndef SYC_EVENT_HANDLER_HPP
#define SYC_EVENT_HANDLER_HPP

#include "EventSource.hpp"

template <class SyncResult>
class SyncEventHandler
{
public:
  typedef typename std::shared_ptr<SyncEventHandler<SyncResult> > Ptr;

public:
  virtual ~SyncEventHandler();

public:
  virtual SyncResult handle(const EventSource::EventTypes&) = 0;
  EventSource::Ptr getEventSource() const;
  EventSource::EventTypes getEventTypes() const;

protected:
  explicit SyncEventHandler(EventSource::Ptr);
  SyncEventHandler(const SyncEventHandler&);
  SyncEventHandler& operator=(const SyncEventHandler&);

protected:
  EventSource::Ptr m_eventSource;
}; //class SyncEventHandler

template <class SyncResult>
SyncEventHandler<SyncResult>::SyncEventHandler(EventSource::Ptr p_es)
  : m_eventSource(p_es)
{
}

template <class SyncResult>
SyncEventHandler<SyncResult>::~SyncEventHandler()
{
}

template <class SyncResult>
EventSource::Ptr SyncEventHandler<SyncResult>::getEventSource() const
{
  return m_eventSource;
}

template <class SyncResult>
EventSource::EventTypes SyncEventHandler<SyncResult>::getEventTypes() const
{
  return m_eventSource->getEventTypes();
}

#endif //SYC_EVENT_HANDLER_HPP
