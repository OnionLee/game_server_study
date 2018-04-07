#include "stdafx.h"
#include "cThread.h"
#include <Windows.h>

cThread::cThread()
{
	m_hThread = NULL;
	m_bIsRun = false;
	m_dwWaitTick = 0;
	m_dwTickCount = 0;
	m_hQuitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}


cThread::~cThread()
{
	CloseHandle(m_hQuitEvent);
	if (m_hThread)
		CloseHandle(m_hThread);
}

unsigned int WINAPI CallTickThread(LPVOID p)
{
	cThread* pTickThread = (cThread*)p;
	pTickThread->TickThread();

	return 1;
}

bool cThread::CreateThread(DWORD dwWaitTick)
{
	unsigned int uiThreadId = 0;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, &CallTickThread, this, CREATE_SUSPENDED, &uiThreadId);

	if (m_hThread == NULL)
	{
		return false;
	}

	m_dwWaitTick = dwWaitTick;
	
	return true;
}

void cThread::DestroyThread()
{
	Run();
	SetEvent(m_hQuitEvent);
	WaitForSingleObject(m_hThread, INFINITE);
}

void cThread::Run()
{
	if (m_bIsRun == false)
	{
		m_bIsRun = true;
		ResumeThread(m_hThread);
	}
}

void cThread::Stop()
{
	if (m_bIsRun == true)
	{
		m_bIsRun = false;
		SuspendThread(m_hThread);
	}
}

void cThread::TickThread()
{
	DWORD dwRet = WaitForSingleObject(m_hQuitEvent, m_dwWaitTick);
	if (WAIT_OBJECT_0 == dwRet)
	{
		m_dwTickCount++;
		OnProcess();
	}
}
