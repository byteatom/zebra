#ifndef BLOCKING_QUEUE
#define BLOCKING_QUEUE

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class BlockingQueue
{
public:

	BlockingQueue()=default;
	BlockingQueue(const BlockingQueue&) = delete;            // disable copying
	BlockingQueue& operator=(const BlockingQueue&) = delete; // disable assignment

	void push(const T& item)
	{
		std::unique_lock<std::mutex> lock(mutex);
		queue.push(item);
		lock.unlock();
		cond.notify_one();
	}

	T pop()
	{
		std::unique_lock<std::mutex> lock(mutex);
		if (queue.empty())
			cond.wait(lock);
		auto item = queue.front();
		queue.pop();
		return item;
	}

	void pop(T& item)
	{
		std::unique_lock<std::mutex> lock(mutex);
		if (queue.empty())
			cond.wait(lock);
		item = queue.front();
		queue.pop();
	}

private:
	std::queue<T> queue;
	std::mutex mutex;
	std::condition_variable cond;
};

#endif
