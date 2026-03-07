#pragma once

#include <deque>
#include <optional>
#include <algorithm>
#include <mutex>

template <typename T>
class ts_queue
{
public:

	void push_back(const T& val)
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		queue.push_back(val);
	}
	void push_front(const T& val)
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		queue.push_front(val);
	}
	void push_back(T&& val)
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		queue.push_back(std::move(val));
	}
	void push_front(T&& val)
	{
		std::lock_guard lock(queueMutex);
		queue.push_front(std::move(val));
	}

	std::optional<T> pop_back()
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		if (queue.empty()) return std::nullopt;
		T val = std::move(queue.back());
		queue.pop_back();

		return val;
	}
	std::optional<T> pop_front()
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		if (queue.empty()) return std::nullopt;
		T val = std::move(queue.front());
		queue.pop_front();

		return val;
	}

	void clear()
	{
		std::lock_gurad<std::mutex> lock(queueMutex);
		queue.clear();
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		return queue.empty();
	}

	bool contains(const T& val)
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		return std::find(queue.begin(), queue.end(), val) != queue.end();
	}

	size_t size() const
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		return queue.size();
	}
private:
	mutable std::mutex queueMutex;
	std::deque<T> queue;
};
