// 1_echo.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include <iostream>

using namespace std;

#include "cSocket.h"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8000

#include <windows.h>
#include <process.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cout << "usage : client.exe [option : [/server][/clinet]]" << endl;
	}

	//소켓 객체 생성
	cSocket Socket;
	//
    return 0;
}

