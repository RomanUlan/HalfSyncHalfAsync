#ifndef ASYC_EVENT_HANDLER_HPP
#define ASYC_EVENT_HANDLER_HPP

#include <memory>

template <typename EventSource, class SyncResult>
class AsyncEventHandler
{
public:
	typedef typename std::shared_ptr<AsyncEventHandler<EventSource, SyncResult> > Ptr;

public:
	virtual ~AsyncEventHandler();

public:
	virtual void handle(const SyncResult&) = 0;
	typename EventSource::Ptr getEventSource() const;
	typename EventSource::EventTypes getEventTypes() const;

protected:
	explicit AsyncEventHandler(typename EventSource::Ptr);

private:
	AsyncEventHandler(const AsyncEventHandler&);
	AsyncEventHandler& operator=(const AsyncEventHandler&);

private:
	typename EventSource::Ptr m_eventSource;
}; //class AsyncEventHandler

template <typename EventSource, class SyncResult>
AsyncEventHandler<EventSource, SyncResult>::AsyncEventHandler(typename EventSource::Ptr p_es)
	: m_eventSource(p_es)
{
}

template <typename EventSource, class SyncResult>
AsyncEventHandler<EventSource, SyncResult>::~AsyncEventHandler()
{
}

template <typename EventSource, class SyncResult>
typename EventSource::Ptr AsyncEventHandler<EventSource, SyncResult>::getEventSource() const
{
	return m_eventSource;
}

template <typename EventSource, class SyncResult>
typename EventSource::EventTypes AsyncEventHandler<EventSource, SyncResult>::getEventTypes() const
{
	return m_eventSource->getEventTypes();
}

#endif //ASYC_EVENT_HANDLER_HPP

