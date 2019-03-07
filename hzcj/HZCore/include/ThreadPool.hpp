#include<list>
#include<thread>
#include<functional>
#include<memory>
#include <atomic>
#include "SyncQueue.hpp"
#include "hzcore_global.h"

struct Task
{
	std::function<void()> pfun;
	bool operator ==(const Task& task)
	{
		return false;
	}
};

class HZCORESHARED_EXPORT ThreadPool
{
public:
	static ThreadPool* GetInstance();

	void Init(int numThreads = std::thread::hardware_concurrency());

	void UnInit();

	void Stop();

	void AddTask(Task&&task);

	void AddTask(const Task& task);

private:
	ThreadPool();

	~ThreadPool(void);

	void Start(int numThreads);

	void RunInThread();

	void StopThreadGroup();

	std::list<std::shared_ptr<std::thread>> m_threadgroup; //处理任务的线程组
	SyncQueue<Task>* m_pQueue; //同步队列     
	atomic_bool m_running; //是否停止的标志
	std::once_flag m_flag;
};
