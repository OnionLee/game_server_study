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
	g_bIsRun = false;
	//쓰레드가 종료 될 때까지 기다린다.
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
    return 0;
}

