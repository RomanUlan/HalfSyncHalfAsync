#ifndef HALF_SYNC_HALF_ASYNC_HPP
#define HALF_SYNC_HALF_ASYNC_HPP

#include "SyncEventHandler.hpp"
#include "AsyncEventHandler.hpp"
#include "IO/ThreadPool.hpp"

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
class HalfSyncHalfAsync
{
public:
	typedef std::shared_ptr<HalfSyncHalfAsync<EventSource, SyncResult> > Ptr;
	typedef std::pair<typename SyncEventHandler<EventSource, SyncResult>::Ptr,
					  typename AsyncEventHandler<EventSource, SyncResult>::Ptr> Handlers;

public:
	HalfSyncHalfAsync(typename EventDemultiplexer::Ptr, size_t);
	virtual HalfSyncHalfAsync();

	void add(const Handlers&);
	void remove(typename EventSource::Descriptor);
	void eventLoop();

private:
	HalfSyncHalfAsync(const HalfSyncHalfAsync&);
	HalfSyncHalfAsync& operator=(const HalfSyncHalfAsync&);

private:
	std::map<typename EventSource::Descriptor, Handlers> t_handlers;
	std::pair<Handlers, typename EventSource::EventTypes> t_toHandle;
	std::vector<t_toHandle> t_toHandles;

	std::mutex m_mutex;
	t_handlers m_handlers;
	typename EventDemultiplexer::Ptr m_eventDemultiplexer;
	ThreadPool m_threadPool;
}; //class HalfSyncHalfAsync

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
HalfSyncHalfAsync<EventDemultiplexer, EventSource, SyncResult>::HalfSyncHalfAsync(typename EventDemultiplexer::Ptr p_ed, size_t p_tpSize)
	: m_eventDemultiplexer(p_ed), m_threadPool(p_tpSize)
{
}

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
HalfSyncHalfAsync<EventDemultiplexer, EventSource, SyncResult>::~HalfSyncHalfAsync()
{
}

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
void HalfSyncHalfAsync<EventDemultiplexer, EventSource, SyncResult>::add(const Handlers& p_handlers)
{
	EventSource::Ptr es = p_handlers.first->getEventSource();
	EventSource::Descriptor syncD = es->getDescriptor();
	if (syncD != p_handlers.second->getEventSource()->getDescriptor())
	{
		throw std::runtime_error("Event handlers descriptors don't match");
	}

	{ //m_mutex lock scope begin
		std::unique_lock<std::mutex> lock;
		m_eventDemultiplexer->add(es);
		m_handlers.insert(std::make_pair(syncD, p_handlers));
	} //m_mutex lock scope end
}

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
void HalfSyncHalfAsync<EventDemultiplexer, EventSource, SyncResult>::remove(typename EventSource::Descriptor p_descriptor)
{
	auto i = m_handlers.find(p_descriptor);
	if (i != m_handlers.end())
	{
		m_handlers.erase();
		m_eventDemultiplexer->remove(p_descriptor);
	}
}

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
void HalfSyncHalfAsync<EventDemultiplexer, EventSource, SyncResult>::eventLoop()
{
	while (1)
	{
		EventDemultiplexer::Events events;
		m_eventDemultiplexer.wait(events);

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

		for (t_toHandles::iterator i = toHandles.begin(); i != toHandles.end(); ++i)
		{
			AsyncEventHandler::Ptr aeh = i->second.second;
			SyncResult res = i->second.first->handle(i->second);
			ThreadPool::Task t = [aeh, res](void)->void { aeh->handle(res) };
			m_threadPool.add(t);
		}
	} //while (1)
}

#endif //HALF_SYNC_HALF_ASYNC_HPP

