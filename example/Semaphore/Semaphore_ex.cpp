#include <iostream>
#include <Windows.h>
#include <process.h>

using namespace std;

int g_nInteger = 0;
HANDLE g_hSemaphore;

unsigned int __stdcall ThreadFunc(void* lpVoid)
{
	long lPre = 0;

	while(true)
	{
		WaitForSingleObject(g_hSemaphore, INFINITE);

		g_nInteger++;
		cout << "ThreadID : " << GetCurrentThreadId() << " : " << g_nInteger << endl;
		if( g_nInteger >= 8 )
		{
			break;
		}

		ReleaseSemaphore( g_hSemaphore, 1, &lPre );
	}

	cout << "Thread end. ThreadID : " << GetCurrentThreadId() << endl;
	ReleaseSemaphore( g_hSemaphore, 1, &lPre );
	return 0;
}

int main()
{
	unsigned int uiThreadID = 0;
	g_hSemaphore = CreateSemaphore( NULL, 0, 1, NULL );
	if( NULL == g_hSemaphore )
	{
		cout<<"CreateSemaphore FAILED : "<<GetLastError()<<endl;
	}

	HANDLE hThread_1 = (HANDLE)_beginthreadex( NULL, 0, ThreadFunc, NULL, CREATE_SUSPENDED, &uiThreadID );
	cout << "Create Thread ID : " << uiThreadID << endl;

	HANDLE hThread_2 = (HANDLE)_beginthreadex( NULL, 0, ThreadFunc, NULL, CREATE_SUSPENDED, &uiThreadID );
	cout << "Create Thread ID : " << uiThreadID << endl;

	HANDLE hThread_3 = (HANDLE)_beginthreadex( NULL, 0, ThreadFunc, NULL, CREATE_SUSPENDED, &uiThreadID );
	cout << "Create Thread ID : " << uiThreadID << endl;

	if( hThread_1 == NULL || hThread_2 == NULL || hThread_3 == NULL )
	{
		cout << "ERROR Thread NULL" << endl;
		return 0;
	}

	ResumeThread( hThread_1 );
	ResumeThread( hThread_2 );
	ResumeThread( hThread_3 );

	long lPre;
	ReleaseSemaphore( g_hSemaphore, 1, &lPre );

	WaitForSingleObject( hThread_1, INFINITE );
	WaitForSingleObject( hThread_2, INFINITE );
	WaitForSingleObject( hThread_3, INFINITE );

	CloseHandle( g_hSemaphore );

	CloseHandle( hThread_1 );
	CloseHandle( hThread_2 );
	CloseHandle( hThread_3 );

	getchar();

	return 0;
}

