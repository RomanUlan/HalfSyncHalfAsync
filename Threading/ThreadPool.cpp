#include "ThreadPool.hpp"
#include "Exceptions/Exception.hpp"

#include <sstream>

using namespace EpollLib::Threading;

ThreadPool::Queue::Job::Job(Type p_Type, const Task& p_Task) : m_Type(p_Type), m_Task(p_Task)
{
}

ThreadPool::Queue::Queue() : m_mutex(), m_condition(), m_jobs()
{
}

ThreadPool::Queue::~Queue()
{
}

void ThreadPool::Queue::PushFront(const Job& p_Job)
{
	{ //Mutex scope lock begin
		UniqueScopedLock guard(m_mutex);
		m_jobs.push_front(p_Job);
	} //Mutex scope lock end

	m_condition.notify_all();
}

void ThreadPool::Queue::PushBack(const Job& p_Job)
{
	{ //Mutex scope lock begin
		UniqueScopedLock guard(m_mutex);
		m_jobs.push_back(p_Job);
	} //Mutex scope lock end

	m_condition.notify_all();
}

ThreadPool::Queue::Job ThreadPool::Queue::Pop()
{
	std::unique_ptr<Job> result;
	{ //Mutex scope lock begin
		UniqueScopedLock guard(m_mutex);
		while (m_jobs.empty()) {
			m_condition.wait(guard);
		}

		result.reset(new Job(m_jobs.front()));
		m_jobs.pop_front();
	} //Mutex scope lock end

	return *result;
}

ThreadPool::ThreadPool(size_t p_Size) : m_queue(), m_size(p_Size), m_threads()
{
}

ThreadPool::~ThreadPool()
{
	//TODO check if kill all threads before
}

void ThreadPool::Start()
{
	if (!m_threads.empty()) {
		throw Exception("ThreadPool started already");
	}

	std::string threadCreationError;
	for (size_t i = 0; (i < m_size) && threadCreationError.empty(); ++i) {
		try {
			m_threads.push_back(threadH(new std::thread(std::bind(ThreadPool::threadFunc, std::ref(m_queue)))));
		} catch (const std::exception e) {
			threadCreationError = e.what();
			break;
		}
	}

	if (!threadCreationError.empty()) {
		if (m_threads.size()) {
			Stop(true);
		}

		std::stringstream ss;
		ss << "ThreadPool thread creation error: " << threadCreationError << ", ThreadPool not started at all";
		throw Exception(ss.str());
	}
}

void ThreadPool::Stop(bool p_Immediately)
{
	if (p_Immediately) {
		for (size_t i = 0; i < m_size; ++i) {
			m_queue.PushFront(Queue::Job(Queue::Job::STOP, Task()));
		}
	} else {
		for (size_t i = 0; i < m_size; ++i) {
			m_queue.PushBack(Queue::Job(Queue::Job::STOP, Task()));
		}
	}

	for (auto i = m_threads.begin(); i != m_threads.end(); ++i) {
		(*i)->join();
	}

	m_threads.clear();
}

void ThreadPool::Add(const Task& p_Task)
{
	m_queue.PushBack(Queue::Job(Queue::Job::REGULAR, p_Task));
}

void ThreadPool::threadFunc(Queue& p_Queue)
{
	while (1) {
		Queue::Job job = p_Queue.Pop();
		if (job.m_Type == Queue::Job::REGULAR) {
			job.m_Task();
		} else {
			break;
		}
	}
}

