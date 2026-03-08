#pragma once

#include <unordered_map>
#include <shared_mutex>
#include <optional>
#include <functional>
#include "../profiling/profiler.h"

template <typename K, typename V>
class ts_unordered_map
{
public:

	void insert(const K& key, const V& value)
	{
		std::unique_lock lock(mapMutex);
		map[key] = value;
	}

	std::optional<V> get(const K& key) const
	{
		std::shared_lock lock(mapMutex);
		auto it = map.find(key);
		if (it == map.end())
			return std::nullopt;

		return it->second;
	}

	bool contains(const K& key) const
	{
		std::shared_lock lock(mapMutex);
		return map.find(key) != map.end();
	}

	bool erase(const K& key)
	{
		PROFILER_ZONE;
		std::unique_lock lock(mapMutex);
		return map.erase(key) > 0;
	}

	size_t size() const
	{
		std::shared_lock lock(mapMutex);
		return map.size();
	}

	void clear()
	{
		std::unique_lock lock(mapMutex);
		map.clear();
	}

	void for_each(const std::function<void(const K& key, const V& value)>& func) const
	{
		PROFILER_ZONE_N("Const for_each");
		//std::shared_lock lock(mapMutex);
		for (const auto& [key, value] : map)
		{
			func(key, value);
		}
	}

	void for_each(const std::function<void(const K& key, V& value)>& func)
	{
		PROFILER_ZONE_N("Mutable for_each");
		//std::unique_lock lock(mapMutex);
		for (auto& [key, value] : map)
		{
			func(key, value);
		}
	}
private:
	mutable std::shared_mutex mapMutex;
	std::unordered_map<K, V> map;
};
