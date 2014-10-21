#ifndef THREADING_THREAD_POOL_HPP
#define THREADING_THREAD_POOL_HPP

#include "Memory/Ptr.hpp"

#include <list>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace EpollLib { namespace Threading {
	class ThreadPool {
		private:
			class Queue {
				public:
					struct Job {
						public:
							typedef std::function<void (void)> Task;
							enum Type {
								STOP,
								REGULAR
							};
							Job(Type, const Task&);
							Type m_Type;
							Task m_Task;

						private:
							Job();
					};
				public:
					Queue();
					~Queue();

				public:
					void PushFront(const Job&);
					void PushBack(const Job&);
					Job Pop();

				private:
					Queue(const Queue&);
					Queue& operator=(const Queue&);

				private:
					typedef std::list<Job> jobs;
					typedef std::unique_lock<std::mutex> UniqueScopedLock;

				private:
					mutable std::mutex m_mutex;
					mutable std::condition_variable m_condition;
					jobs m_jobs;
			}; //class Queue

		public:
			typedef Queue::Job::Task Task;
			ThreadPool(size_t);
			~ThreadPool();

		public:
			void Start();
			void Stop(bool);
			void Add(const Task&);

		protected:
			static void threadFunc(Queue&);

		private:
			ThreadPool(const ThreadPool&);
			ThreadPool& operator=(const ThreadPool&);

		private:
			typedef Memory::SmartPointers<std::thread>::Ptr threadH;
			typedef std::vector<threadH> threads;

		private:
			Queue m_queue;
			size_t m_size;
			threads m_threads;
	}; //class ThreadPool
}} //namespace EpollLib::Threading

#endif //THREADING_THREAD_POOL_HPP

