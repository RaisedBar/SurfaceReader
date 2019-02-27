// DispatchQueue.cpp
// Asynchronous despatch queue

#include "DispatchQueue.h"

dispatch_queue::dispatch_queue(size_t thread_cnt) 
	:
		threads_(thread_cnt)
{
	std::vector<std::thread>::iterator it;

	for (it = threads_.begin(); it != threads_.end(); it++)
	{
		* it = std::thread(&dispatch_queue::dispatch_thread_handler, this);
	}
}

dispatch_queue::~dispatch_queue()
{
	// printf("Destructor: Destroying dispatch threads...\n");

	// Signal to dispatch threads that it's time to wrap up 
	std::unique_lock<std::mutex> lock(lock_);
	quit_ = true;
	lock.unlock();
	cv_.notify_all();

	// Wait for threads to finish before we exit 
	std::vector<std::thread>::iterator it;

	for (it = threads_.begin(); it != threads_.end(); it++)
		{
		if (it->joinable())
		{
			it->join();
					}
	}
}

void dispatch_queue::dispatch(const fp_t& op)
{
	std::unique_lock<std::mutex> lock(lock_);
	q_.push(op);

	// Manual unlocking is done before notifying, to avoid waking up 
// the waiting thread only to block again (see notify_one for details) 
	lock.unlock();
	cv_.notify_all();
}

void dispatch_queue::dispatch(fp_t&& op)
{
	std::unique_lock<std::mutex> lock(lock_);
	q_.push(std::move(op));

	// Manual unlocking is done before notifying, to avoid waking up 
// the waiting thread only to block again (see notify_one for details) 
	lock.unlock();
	cv_.notify_all();
}

void dispatch_queue::dispatch_thread_handler(void)
{
	std::unique_lock<std::mutex> lock(lock_);

	do
	{
		//Wait until we have data or a quit signal 
		cv_.wait(lock, [this] {
			return (q_.size() || quit_);
		});

		//after wait, we own the lock 
		if (!quit_ && q_.size())
		{
			auto op = std::move(q_.front());
			q_.pop();

			//unlock now that we're done messing with the queue 
			lock.unlock();
						op();
									lock.lock();
		}
	} while (!quit_);
}
