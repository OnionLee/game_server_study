#include "stdafx.h"

volatile bool g_bIsRun = true;

unsigned int WINAPI ThreadFunc(LPVOID lpVoid)
{
	while (g_bIsRun)
	{
		cout << "ThreadFunc..." << endl;
	}
	cout << "쓰레드 종료" << endl;

	return 0;
}

int main()
{
	unsigned int uiThreadID;

	//쓰레드 생성
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &uiThreadID);

	if (hThread == NULL)
	{
		cout << "쓰레드 생성 실패" << endl;
		return 0;
	}
	Sleep(500);
	
	DWORD dwExitCode = 0;
	//쓰레드를 강제 종료하기 위해 종료 코드값을 얻어온다.
	GetExitCodeThread(hThread, &dwExitCode);
	//쓰레를 강제 종료 시킨다.
	TerminateThread(hThread, dwExitCode);

	//쓰레드가 종료 될때까지 기다린다.
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}

