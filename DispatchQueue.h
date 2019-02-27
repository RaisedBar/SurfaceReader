// DispatchQueue.h
// Asynchronous despatch queue
// https://embeddedartistry.com/blog/2017/2/1/c11-implementing-a-dispatch-queue-using-stdfunction

#pragma once

#include <thread> 
#include <functional> 
#include <queue> 
#include <mutex> 
#include <condition_variable> 

class dispatch_queue 
{
	typedef std::function<void(void)> fp_t;

public:
	dispatch_queue(size_t thread_cnt = 1)
	{
		std::vector<std::thread>::iterator it;

		for (it = threads_.begin(); it != threads_.end(); it++)
		{
			*it = std::thread(&dispatch_queue::dispatch_thread_handler, this);
		}
	};
	
	~dispatch_queue()
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
	};

	// dispatch and copy 
	void dispatch(const fp_t& op)
	{
		std::unique_lock<std::mutex> lock(lock_);
		q_.push(op);

		// Manual unlocking is done before notifying, to avoid waking up 
	// the waiting thread only to block again (see notify_one for details) 
		lock.unlock();
		cv_.notify_all();
	};
	
	// dispatch and move 
	void dispatch(fp_t&& op)
	{
		std::unique_lock<std::mutex> lock(lock_);
		q_.push(std::move(op));

		// Manual unlocking is done before notifying, to avoid waking up 
	// the waiting thread only to block again (see notify_one for details) 
		lock.unlock();
		cv_.notify_all();
	};
		
	// Deleted operations 
	dispatch_queue(const dispatch_queue& rhs) = delete;
	dispatch_queue& operator=(const dispatch_queue& rhs) = delete;
	dispatch_queue(dispatch_queue&& rhs) = delete;
	dispatch_queue& operator=(dispatch_queue&& rhs) = delete;

private:
	std::mutex lock_;
	std::vector<std::thread> threads_;
	std::queue<fp_t> q_;
	std::condition_variable cv_;
	bool quit_ = false;
	void dispatch_thread_handler(void)
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
	};
};
