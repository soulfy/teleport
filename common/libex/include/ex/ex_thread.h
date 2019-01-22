#ifndef __EX_THREAD_H__
#define __EX_THREAD_H__

#include "ex_str.h"

#include <list>

#ifdef EX_OS_WIN32
#	include <process.h>
typedef HANDLE EX_THREAD_HANDLE;
#else
#	include <pthread.h>
typedef pthread_t EX_THREAD_HANDLE;
#endif

class ExThreadManager;

class ExThreadBase
{
public:
	ExThreadBase(const char* thread_name);
	virtual ~ExThreadBase();

	bool is_running(void) { return m_is_running; }

	// �����������̣߳�ִ�б������˵�run()������
	bool start(void);
	// �����̣߳��ȴ�wait_timeout_ms���룬���wait_timeout_msΪ0�������޵ȴ���
	bool stop(void);
	// ֱ�ӽ����̣߳�ǿɱ��������ʹ�ã�
	bool terminate(void);

protected:
	// main loop of this thread.
	virtual void _thread_loop(void) = 0;
	// called by another thread when thread ready to stop.
    virtual void _on_stop(void) {};
    // called inside thread when thread fully stopped.
    virtual void _on_stopped(void) {};

#ifdef EX_OS_WIN32
	static unsigned int WINAPI _thread_func(LPVOID lpParam);
#else
	static void* _thread_func(void * pParam);
#endif

protected:
	ex_astr m_thread_name;
	EX_THREAD_HANDLE m_handle;
	bool m_is_running;
	bool m_need_stop;
};


// �߳�����������ʹ�ã�
class ExThreadLock
{
public:
	ExThreadLock();
	virtual ~ExThreadLock();

	void lock(void);
	void unlock(void);

private:
#ifdef EX_OS_WIN32
	CRITICAL_SECTION m_locker;
#else
	pthread_mutex_t m_locker;
#endif
};

// �߳���������
class ExThreadSmartLock
{
public:
	ExThreadSmartLock(ExThreadLock& lock) : m_lock(lock)
	{
		m_lock.lock();
	}
	~ExThreadSmartLock()
	{
		m_lock.unlock();
	}

private:
	ExThreadLock& m_lock;
};

typedef std::list<ExThreadBase*> ex_threads;

class ExThreadManager
{
	friend class ExThreadBase;

public:
	ExThreadManager();
	virtual ~ExThreadManager();

	void stop_all(void);

//private:
	void add(ExThreadBase* tb);
	void remove(ExThreadBase* tb);

private:
	ExThreadLock m_lock;
	ex_threads m_threads;
};


// ԭ�Ӳ���
int ex_atomic_add(volatile int* pt, int t);
int ex_atomic_inc(volatile int* pt);
int ex_atomic_dec(volatile int* pt);

// �߳���ز���
ex_u64 ex_get_thread_id(void);

#endif // __EX_THREAD_H__
