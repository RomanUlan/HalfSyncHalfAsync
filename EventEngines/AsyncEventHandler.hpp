#ifndef ASYC_EVENT_HANDLER_HPP
#define ASYC_EVENT_HANDLER_HPP

#include "EventSource.hpp"

#include <memory>

template <class SyncResult>
class AsyncEventHandler
{
public:
  typedef typename std::shared_ptr<AsyncEventHandler<SyncResult> > Ptr;

public:
  virtual ~AsyncEventHandler();

public:
  virtual void handle(const SyncResult&) = 0;
  EventSource::Ptr getEventSource() const;
  EventSource::EventTypes getEventTypes() const;

protected:
  explicit AsyncEventHandler(EventSource::Ptr);
  AsyncEventHandler(const AsyncEventHandler&);
  AsyncEventHandler& operator=(const AsyncEventHandler&);

protected:
  EventSource::Ptr m_eventSource;
}; //class AsyncEventHandler

template <class SyncResult>
AsyncEventHandler<SyncResult>::AsyncEventHandler(EventSource::Ptr p_es)
  : m_eventSource(p_es)
{
}

template <class SyncResult>
AsyncEventHandler<SyncResult>::~AsyncEventHandler()
{
}

template <class SyncResult>
EventSource::Ptr AsyncEventHandler<SyncResult>::getEventSource() const
{
  return m_eventSource;
}

template <class SyncResult>
EventSource::EventTypes AsyncEventHandler<SyncResult>::getEventTypes() const
{
  return m_eventSource->getEventTypes();
}

#endif //ASYC_EVENT_HANDLER_HPP
