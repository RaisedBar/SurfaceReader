// queue.h
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
	dispatch_queue(size_t thread_cnt = 1);
	~dispatch_queue();

	// dispatch and copy 
	void dispatch(const fp_t& op);
	// dispatch and move 
	void dispatch(fp_t&& op);
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
	void dispatch_thread_handler(void);
};
