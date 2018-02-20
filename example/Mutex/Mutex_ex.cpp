#include <iostream>
#include <Windows.h>
#include <process.h>

using namespace std;

int g_nInteger = 0;
HANDLE g_hMutex;

unsigned int __stdcall ThreadFunc(void* lpVoid)
{
	while(true)
	{
		WaitForSingleObject(g_hMutex, INFINITE);

		g_nInteger++;
		cout << "ThreadID : " << GetCurrentThreadId() << " : " << g_nInteger << endl;
		if(g_nInteger == 9 || g_nInteger == 10)
		{
			break;
		}

		ReleaseMutex(g_hMutex);
	}

	cout << "Thread end. ThreadID : " << GetCurrentThreadId() << endl;
	ReleaseMutex( g_hMutex );
	return 0;
}

int main()
{
	unsigned int uiThreadID = 0;
	g_hMutex = CreateMutex( NULL, FALSE, NULL );
	if( NULL == g_hMutex )
	{
		cout<<"CreateMutex FAILED : "<<GetLastError()<<endl;
	}

	HANDLE hThread_1 = (HANDLE)_beginthreadex( NULL, 0, ThreadFunc, NULL, CREATE_SUSPENDED, &uiThreadID );
	cout << "Create Thread ID : " << uiThreadID << endl;

	HANDLE hThread_2 = (HANDLE)_beginthreadex( NULL, 0, ThreadFunc, NULL, CREATE_SUSPENDED, &uiThreadID );
	cout << "Create Thread ID : " << uiThreadID << endl;

	if( hThread_1 == NULL || hThread_2 == NULL )
	{
		cout << "ERROR Thread NULL" << endl;
		return 0;
	}

	ResumeThread( hThread_1 );
	ResumeThread( hThread_2 );

	ReleaseMutex( g_hMutex );

	WaitForSingleObject( hThread_1, INFINITE );
	WaitForSingleObject( hThread_2, INFINITE );

	CloseHandle( g_hMutex );

	CloseHandle( hThread_1 );
	CloseHandle( hThread_2 );

	getchar();

	return 0;
}

