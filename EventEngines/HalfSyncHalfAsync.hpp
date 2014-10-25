#ifndef HALF_SYNC_HALF_ASYNC_HPP
#define HALF_SYNC_HALF_ASYNC_HPP
#include "SyncEventHandler.hpp"
#include "AsyncEventHandler.hpp"
#include "../Threading/ThreadPool.hpp"

#include <map>

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
class HalfSyncHalfAsync
{
public:
	typedef std::shared_ptr<HalfSyncHalfAsync<EventDemultiplexer, EventSource, SyncResult> > Ptr;
	typedef std::pair<typename SyncEventHandler<EventSource, SyncResult>::Ptr,
					  typename AsyncEventHandler<EventSource, SyncResult>::Ptr> Handlers;

public:
	HalfSyncHalfAsync(typename EventDemultiplexer::Ptr, size_t);
	virtual ~HalfSyncHalfAsync();

	void add(const Handlers&);
	void remove(typename EventSource::Descriptor);
	void eventLoop();

private:
	HalfSyncHalfAsync(const HalfSyncHalfAsync&);
	HalfSyncHalfAsync& operator=(const HalfSyncHalfAsync&);

private:
	typedef std::map<typename EventSource::Descriptor, Handlers> t_handlers;
	typedef std::pair<Handlers, typename EventSource::EventTypes> t_toHandle;
	typedef std::vector<t_toHandle> t_toHandles;

	std::mutex m_mutex;
	t_handlers m_handlers;
	typename EventDemultiplexer::Ptr m_eventDemultiplexer;
	ThreadPool m_threadPool;
}; //class HalfSyncHalfAsync

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
HalfSyncHalfAsync<EventDemultiplexer, EventSource, SyncResult>::HalfSyncHalfAsync(typename EventDemultiplexer::Ptr p_ed, size_t p_tpSize)
	: m_eventDemultiplexer(p_ed), m_threadPool(p_tpSize)
{
	m_threadPool.start();
}

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
HalfSyncHalfAsync<EventDemultiplexer, EventSource, SyncResult>::~HalfSyncHalfAsync()
{
	m_threadPool.stop(true);
}

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
void HalfSyncHalfAsync<EventDemultiplexer, EventSource, SyncResult>::add(const Handlers& p_handlers)
{
	typename EventSource::Ptr es = p_handlers.first->getEventSource();
	typename EventSource::Descriptor syncD = es->getDescriptor();
	if (p_handlers.second && (syncD != p_handlers.second->getEventSource()->getDescriptor()))
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
		m_eventDemultiplexer->remove(p_descriptor);
		m_handlers.erase(i);
	}
}

template <typename EventDemultiplexer, typename EventSource, class SyncResult>
void HalfSyncHalfAsync<EventDemultiplexer, EventSource, SyncResult>::eventLoop()
{
	while (1)
	{
		typename EventDemultiplexer::Events events;
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

		for (typename t_toHandles::iterator i = toHandles.begin(); i != toHandles.end(); ++i)
		{
			typename AsyncEventHandler<EventSource, SyncResult>::Ptr aeh = i->first.second;
			SyncResult res = i->first.first->handle(i->second);
			if (aeh)
			{
				ThreadPool::Task t = [aeh, res](void)->void { aeh->handle(res); };
				m_threadPool.add(t);
			}
		}
	} //while (1)
}

#endif //HALF_SYNC_HALF_ASYNC_HPP

