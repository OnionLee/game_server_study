#include <iostream>

using namespace std;

#include "cSocket.h"
cSocket::cSocket(void)
{
	this->m_socket = INVALID_SOCKET;
	this->m_socketConnect = INVALID_SOCKET;
	ZeroMemory(m_szSocketBuf, 1024);
}

cSocket::~cSocket(void)
{
	//윈속의 사용을 끝낸다.
	WSACleanup();
}

bool cSocket::InitSocket()
{
	WSADATA wsaData;

	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != nRet)
	{
		cout << "WSAStartup 에러 : " << nRet << endl;

		return false;
	}

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_socket)
	{
		cout << "socket 에러" << endl;
		return false;
	}

	cout << "소켓 초기화 성공" << endl;
	
	return true;
}

void cSocket::CloseSocket(SOCKET socketClose, bool bIsForce)
{
	struct linger stLinger = { 0, 0 }; // SO_DONTLINGER;
	
	if (true == bIsForce)
	{
		stLinger.l_onoff = 1;
	}

	shutdown(socketClose, SD_BOTH);
	setsockopt(socketClose, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));
	closesocket(socketClose);

	socketClose = INVALID_SOCKET;
}

bool cSocket::BindandListen(int nBindPort)
{
	SOCKADDR_IN stServerAddr;

	//인텔계열에선 리틀엔디안 3 = 03 00
	stServerAddr.sin_family = AF_INET;

	//포트
	stServerAddr.sin_port = htons(nBindPort);
	//어떤 주소든 받아들이겠다.
	stServerAddr.sin_addr.s_addr = htons(INADDR_ANY);

	int nRet = bind(m_socket, (SOCKADDR*)&stServerAddr, sizeof(stServerAddr));
	if (0 != nRet)
	{
		cout << "bind 에러" << endl;
		return false;
	}

	nRet = listen(m_socket, 5);
	if (0 != nRet)
	{
		cout << "listen 에러" << endl;
		return false;
	}

	cout << "서버 등록 성공" << endl;
	return true;
}

bool cSocket::StartServer()
{
	char szOutStr[1024];

	// 클라정보 저장할 구조체
	SOCKADDR_IN stClientAddr;
	int nAddrLen = sizeof(SOCKADDR_IN);

	cout << "서버 시작..." << endl;

	//클라요청대기

	m_socketConnect = accept(m_socket, (SOCKADDR*)&stClientAddr, &nAddrLen);
	if (INVALID_SOCKET == m_socketConnect)
	{

		cout << "accept 에러" << endl;
		return false;
	}

	sprintf(szOutStr, "클라이언트 접속 : IP(%s) SOCKET(%d)", inet_ntoa(stClientAddr.sin_addr), m_socketConnect);
	
	cout << szOutStr << endl;

	while (true)
	{
		int nRecvLen = recv(m_socketConnect, m_szSocketBuf, 1024, 0);
		if (0 == nRecvLen)
		{
			cout << "클라와 연결 종료" << endl;
			CloseSocket(m_socketConnect);

			//종료하고 다시 서버 접속 받음
			StartServer();
			return false;
		}
		else if (-1 == nRecvLen) 
		{
			cout << "recv 에러" << endl;
			CloseSocket(m_socketConnect);

			//종료하고 다시 서버 접속 받음
			StartServer();
			return false;
		}

		m_szSocketBuf[nRecvLen] = NULL;
		cout << "메세지 수신 : 수신 bytes[" << nRecvLen << "], 내용 : [" << m_szSocketBuf << "]" << endl;

		int nSendLen = send(m_socketConnect, m_szSocketBuf, nRecvLen, 0);
		if (0 == nRecvLen)
		{
			cout << "send 에러" << endl;
			CloseSocket(m_socketConnect);

			//종료하고 다시 서버 접속 받음
			StartServer();
			return false;
		}

		cout << "메세지 송신 : 송신 bytes[" << nRecvLen << "], 내용 : [" << m_szSocketBuf << "]" << endl;
	}


	CloseSocket(m_socketConnect);
	CloseSocket(m_socket);

	cout << "서버 정상 종료" << endl;
	return true;
}

bool cSocket::Connect(char * szIP, int nPort)
{
	SOCKADDR stServerAddr;

	char szOutMsg[1024];
	stServerAddr.sa_family = 
}