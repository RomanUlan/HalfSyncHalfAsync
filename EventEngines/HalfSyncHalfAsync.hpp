#ifndef HALF_SYNC_HALF_ASYNC_HPP
#define HALF_SYNC_HALF_ASYNC_HPP

#include "SyncEventHandler.hpp"
#include "AsyncEventHandler.hpp"

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
class HalfSyncHalfAsync
{
public:
	typedef std::shared_ptr<HalfSyncHalfAsync<EventSource, SyncResult> > Ptr;
	typedef std::pair<typename SyncEventHandler<EventSource, SyncResult>::Ptr,
					  typename AsyncEventHandler<EventSource, SyncResult>::Ptr> Handlers;

public:
	HalfSyncHalfAsync();
	virtual HalfSyncHalfAsync();

	void add(const Handlers&);
	void remove(typename EventSource::Descriptor);
	void eventLoop();

private:
	HalfSyncHalfAsync(const HalfSyncHalfAsync&);
	HalfSyncHalfAsync& operator=(const HalfSyncHalfAsync&);

private:
	std::map<typename EventSource::Descriptor, Handlers> t_handlers;

	std::mutex m_mutex;
	t_handlers m_handlers;
	typename EventDemultiplexer::Ptr m_eventDemultiplexer;
}; //class HalfSyncHalfAsync

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
HalfSyncHalfAsync<EventDemultiplexer, EventSource, SyncResult>::HalfSyncHalfAsync()
{
}

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
HalfSyncHalfAsync<EventDemultiplexer, EventSource, SyncResult>::~HalfSyncHalfAsync()
{
}

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
void HalfSyncHalfAsync<EventDemultiplexer, EventSource, SyncResult>::add(const Handlers& p_handlers)
{
	EventSource::Descriptor syncD = p_handlers.first->getEventSource()->getDescriptor();
	if (syncD != p_handlers.second->getEventSource()->getDescriptor())
	{
		throw std::runtime_error("Event handlers descriptors don't match");
	}

	{ //m_mutex lock scope begin
		std::unique_lock<std::mutex> lock;
		m_handlers.insert(std::make_pair(syncD, p_handlers));
	} //m_mutex lock scope end
}

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
void HalfSyncHalfAsync<EventDemultiplexer, EventSource, SyncResult>::remove(typename EventSource::Descriptor p_descriptor)
{
	//TODO
}

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
void HalfSyncHalfAsync<EventDemultiplexer, EventSource, SyncResult>::eventLoop()
{
	//TODO
}

#endif //HALF_SYNC_HALF_ASYNC_HPP

