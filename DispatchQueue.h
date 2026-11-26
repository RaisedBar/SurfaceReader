// DispatchQueue.h
// A single background thread that runs queued tasks in order.
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef DISPATCH_QUEUE_H
#define DISPATCH_QUEUE_H

#pragma once

#include <chrono>
#include <condition_variable>
#include <deque>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <thread>

/* Each MIDI surface owns one of these. The MIDI input callbacks only forward the message and Post a copy
of it here, so analysis, logging and speech never hold up MIDI traffic.

- Tasks run one at a time, in the order they were posted, so a surface's messages are analysed in order.
- PostAfter runs a task once a delay has passed, without blocking the thread in the meantime: tasks posted
  during the delay (for example the display updates a "read display" action is waiting for) run first.
- Stop finishes the task that is running, discards the rest and joins the thread. The owner must call it
  before destroying anything the tasks use; the destructor calls it too. */

class WorkerQueue
{
public:
	typedef std::function <void()> Task;
	typedef std::chrono::steady_clock Clock;

	WorkerQueue()
		: blnStopping( false),
		nSequence( 0),
		myThread( &WorkerQueue::Run, this)
	{}

	~WorkerQueue()
	{
		Stop();
	}

	WorkerQueue( const WorkerQueue &) = delete;
	WorkerQueue & operator=( const WorkerQueue &) = delete;

	// Run the task as soon as the tasks before it have run
	void Post( Task myTask)
	{
		{
			std::lock_guard <std::mutex> myLock( myMutex);
			if (blnStopping)
			{
				return;
			}
			myTasks.push_back( std::move( myTask));
		}
		myCondition.notify_one();
	}

	// Run the task once nDelayMilliseconds have passed
	void PostAfter( int nDelayMilliseconds, Task myTask)
	{
		if (nDelayMilliseconds <= 0)
		{
			Post( std::move( myTask));
			return;
		}

		{
			std::lock_guard <std::mutex> myLock( myMutex);
			if (blnStopping)
			{
				return;
			}
			// The sequence number keeps tasks that fall due at the same moment in the order they were posted
			myDelayedTasks.emplace( std::make_pair( Clock::now() + std::chrono::milliseconds( nDelayMilliseconds), nSequence++), std::move( myTask));
		}
		myCondition.notify_one();
	}

	// Discard queued tasks and wait for the thread to finish. Safe to call more than once.
	void Stop()
	{
		{
			std::lock_guard <std::mutex> myLock( myMutex);
			blnStopping = true;
			myTasks.clear();
			myDelayedTasks.clear();
		}
		myCondition.notify_one();

		if (myThread.joinable()
			&& (myThread.get_id() != std::this_thread::get_id()))
		{
			myThread.join();
		}
	}

private:
	void Run()
	{
		std::unique_lock <std::mutex> myLock( myMutex);

		while (blnStopping == false)
		{
			// Move delayed tasks that have fallen due onto the main queue
			Clock::time_point myNow = Clock::now();
			while ((myDelayedTasks.empty() == false)
				&& (myDelayedTasks.begin()->first.first <= myNow))
			{
				myTasks.push_back( std::move( myDelayedTasks.begin()->second));
				myDelayedTasks.erase( myDelayedTasks.begin());
			}

			if (myTasks.empty())
			{
				if (myDelayedTasks.empty())
				{
					myCondition.wait( myLock);
				}
				else
				{
					myCondition.wait_until( myLock, myDelayedTasks.begin()->first.first);
				}
				continue;
			}

			Task myTask = std::move( myTasks.front());
			myTasks.pop_front();

			// Run the task without holding the lock, so the MIDI callbacks can keep posting
			myLock.unlock();
			try
			{
				myTask();
			}
			catch( ...)
			{
				// A failing task must not take the thread down with it
			}
			myLock.lock();
		}
	}

	std::mutex myMutex;
	std::condition_variable myCondition;
	std::deque <Task> myTasks;
	std::multimap <std::pair <Clock::time_point, unsigned long long>, Task> myDelayedTasks;
	bool blnStopping;
	unsigned long long nSequence;
	std::thread myThread;  // last, so everything it uses exists before it starts
};


/* Debouncing for controls that send a stream of messages, such as faders. Each message calls Moved and schedules a
check (with WorkerQueue::PostAfter) for the same key and the number Moved returned. When a check runs, Settled is
true only if no message has arrived for that key since, so only the check for the last message acts, once the
control has been still for the whole delay. Not thread-safe: use it from the worker thread only. */

class SettleGate
{
public:
	// A message has arrived for this key: returns its number
	unsigned long long Moved( const std::string & strKey)
	{
		return ++myLatest[ strKey];
	}

	// True if nNumber is still the latest message for the key; the key is then forgotten
	bool Settled( const std::string & strKey, unsigned long long nNumber)
	{
		std::map <std::string, unsigned long long>::iterator it = myLatest.find( strKey);

		if ((it == myLatest.end()) || (it->second != nNumber))
		{
			return false;
		}

		myLatest.erase( it);
		return true;
	}

private:
	std::map <std::string, unsigned long long> myLatest;
};

#endif  // DISPATCH_QUEUE_H
