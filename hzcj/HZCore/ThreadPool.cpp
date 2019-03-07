#include "ThreadPool.hpp"
#include"SyncQueue.hpp"
#define MaxTaskCount 10000;
ThreadPool* ThreadPool::GetInstance()
{
	static ThreadPool instance;
	return &instance;
}

void ThreadPool::Init(int numThreads /*= std::thread::hardware_concurrency()*/)
{
	Start(numThreads);
}

void ThreadPool::UnInit()
{
	//如果没有停止时则主动停止线程池
	Stop();
}

void ThreadPool::Stop()
{
	std::call_once(m_flag, [this]{StopThreadGroup(); }); //保证多线程情况下只调用一次StopThreadGroup
}

void ThreadPool::AddTask(Task&&task)
{
	m_pQueue->Put(std::forward<Task>(task));
}

void ThreadPool::AddTask(const Task& task)
{
	m_pQueue->Put(task);
}

ThreadPool::ThreadPool()
{
	int count = MaxTaskCount;
	m_pQueue = new SyncQueue<Task>(count);
}

ThreadPool::~ThreadPool(void)
{
	delete m_pQueue;
}

void ThreadPool::Start(int numThreads)
{
	m_running = true;
	//创建线程组
	for (int i = 0; i < numThreads; ++i)
	{
		m_threadgroup.push_back(std::make_shared<std::thread>(&ThreadPool::RunInThread, this));
	}
}

void ThreadPool::RunInThread()
{
	while (m_running)
	{
		//取任务分别执行
		std::list<Task> list;
		m_pQueue->Take(list);

		for (auto& task : list)
		{
			if (!m_running)
				return;
			task.pfun();
		}
	}
}

void ThreadPool::StopThreadGroup()
{
	m_pQueue->Stop(); //让同步队列中的线程停止
	m_running = false; //置为false，让内部线程跳出循环并退出

	for (auto thread : m_threadgroup) //等待线程结束
	{
		if (thread)
			thread->join();
	}
	m_threadgroup.clear();
}