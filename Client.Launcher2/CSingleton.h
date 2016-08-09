#pragma once

template <class T>
class CSingleton
{
protected:
	CSingleton() = default;
	~CSingleton() = default;

public:
	CSingleton(const CSingleton&) = delete;
	CSingleton &operator=(const CSingleton&) = delete;

	static T &getInstance()
	{
		static T instance;
		return instance;
	}
};