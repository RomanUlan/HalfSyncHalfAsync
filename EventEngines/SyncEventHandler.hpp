#ifndef SYC_EVENT_HANDLER_HPP
#define SYC_EVENT_HANDLER_HPP

#include <memory>

template <typename EventSource, class SyncResult>
class SyncEventHandler
{
public:
	typedef typename std::shared_ptr<SyncEventHandler<EventSource, SyncResult> > Ptr;

public:
	virtual ~SyncEventHandler();

public:
	virtual SyncResult handle(const typename EventSource::EventTypes&) = 0;
	typename EventSource::Ptr getEventSource() const;
	typename EventSource::EventTypes getEventTypes() const;

protected:
	explicit SyncEventHandler(typename EventSource::Ptr);
	SyncEventHandler(const SyncEventHandler&);
	SyncEventHandler& operator=(const SyncEventHandler&);

protected:
	typename EventSource::Ptr m_eventSource;
}; //class SyncEventHandler

template <typename EventSource, class SyncResult>
SyncEventHandler<EventSource, SyncResult>::SyncEventHandler(typename EventSource::Ptr p_es)
	: m_eventSource(p_es)
{
}

template <typename EventSource, class SyncResult>
SyncEventHandler<EventSource, SyncResult>::~SyncEventHandler()
{
}

template <typename EventSource, class SyncResult>
typename EventSource::Ptr SyncEventHandler<EventSource, SyncResult>::getEventSource() const
{
	return m_eventSource;
}

template <typename EventSource, class SyncResult>
typename EventSource::EventTypes SyncEventHandler<EventSource, SyncResult>::getEventTypes() const
{
	return m_eventSource->getEventTypes();
}

#endif //SYC_EVENT_HANDLER_HPP

