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
	//������ ����� ������.
	WSACleanup();
}

bool cSocket::InitSocket()
{
	WSADATA wsaData;

	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != nRet)
	{
		cout << "WSAStartup ���� : " << nRet << endl;

		return false;
	}

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_socket)
	{
		cout << "socket ����" << endl;
		return false;
	}

	cout << "���� �ʱ�ȭ ����" << endl;
	
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

	//���ڰ迭���� ��Ʋ����� 3 = 03 00
	stServerAddr.sin_family = AF_INET;

	//��Ʈ
	stServerAddr.sin_port = htons(nBindPort);
	//� �ּҵ� �޾Ƶ��̰ڴ�.
	stServerAddr.sin_addr.s_addr = htons(INADDR_ANY);

	int nRet = bind(m_socket, (SOCKADDR*)&stServerAddr, sizeof(stServerAddr));
	if (0 != nRet)
	{
		cout << "bind ����" << endl;
		return false;
	}

	nRet = listen(m_socket, 5);
	if (0 != nRet)
	{
		cout << "listen ����" << endl;
		return false;
	}

	cout << "���� ��� ����" << endl;
	return true;
}

bool cSocket::StartServer()
{
	char szOutStr[1024];

	// Ŭ������ ������ ����ü
	SOCKADDR_IN stClientAddr;
	int nAddrLen = sizeof(SOCKADDR_IN);

	cout << "���� ����..." << endl;

	//Ŭ���û���

	m_socketConnect = accept(m_socket, (SOCKADDR*)&stClientAddr, &nAddrLen);
	if (INVALID_SOCKET == m_socketConnect)
	{

		cout << "accept ����" << endl;
		return false;
	}

	sprintf(szOutStr, "Ŭ���̾�Ʈ ���� : IP(%s) SOCKET(%d)", inet_ntoa(stClientAddr.sin_addr), m_socketConnect);
	
	cout << szOutStr << endl;

	while (true)
	{
		int nRecvLen = recv(m_socketConnect, m_szSocketBuf, 1024, 0);
		if (0 == nRecvLen)
		{
			cout << "Ŭ��� ���� ����" << endl;
			CloseSocket(m_socketConnect);

			//�����ϰ� �ٽ� ���� ���� ����
			StartServer();
			return false;
		}
		else if (-1 == nRecvLen) 
		{
			cout << "recv ����" << endl;
			CloseSocket(m_socketConnect);

			//�����ϰ� �ٽ� ���� ���� ����
			StartServer();
			return false;
		}

		m_szSocketBuf[nRecvLen] = NULL;
		cout << "�޼��� ���� : ���� bytes[" << nRecvLen << "], ���� : [" << m_szSocketBuf << "]" << endl;

		int nSendLen = send(m_socketConnect, m_szSocketBuf, nRecvLen, 0);
		if (0 == nRecvLen)
		{
			cout << "send ����" << endl;
			CloseSocket(m_socketConnect);

			//�����ϰ� �ٽ� ���� ���� ����
			StartServer();
			return false;
		}

		cout << "�޼��� �۽� : �۽� bytes[" << nRecvLen << "], ���� : [" << m_szSocketBuf << "]" << endl;
	}


	CloseSocket(m_socketConnect);
	CloseSocket(m_socket);

	cout << "���� ���� ����" << endl;
	return true;
}

bool cSocket::Connect(char * szIP, int nPort)
{
	SOCKADDR stServerAddr;

	char szOutMsg[1024];
	stServerAddr.sa_family = 
}