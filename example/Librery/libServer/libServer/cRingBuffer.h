#pragma once
#include "cMonitor.h"
#define MAX_RINGBUFSIZE 1024  

class NETLIB_API cRingBuffer :
	public cMonitor
{
public:
	cRingBuffer();
	~cRingBuffer();

	//��	 ���� �޸� �Ҵ�
	bool Create(int nBufferSize = MAX_RINGBUFSIZE);
	//�ʱ�ȭ
	bool Initialize();
	//�Ҵ�� ���� ũ�⸦ ��ȯ�Ѵ�.
	inline int GetBufferSize() { return m_nBufferSize; }
	
	//�ش��ϴ� ���� ������ �����͸� ��ȯ
	inline char* GetBeingMark() { return m_pBeginMark; }
	inline char* GetCurrentMark() { return m_pCurrentMark; }
	inline char* GetEndMark() { return m_pEndMark; }

	//���� ������ ���� �����͸� ����
	char* ForwardMark(int nForwardLength);
	char* ForwardMark(int nForwardLength, int nNextLength, DWORD dwRemainLength);

	//���� ���� ���� ����
	void RelesaseBuffer(int nReleaseSize);
	//���� ���� ���� ũ�� ��ȯ
	int GetUsedBufferSize() { return m_nUsedBufferSize; }
	//���� ���� ���� ��ȯ
	int GetAllUsedBufferSize() { return m_uiAllUserBufSize; }

	//���� ���� ������ �о ��ȯ
	char* GetBuffer(int nReadSize, int* pReadSize);

private:
	char* m_pRingBuffer;				//���� �����͸� �����ϴ� ���� ������
	
	char* m_pBeginMark;					//������ ó���κ��� ����Ű�� �ִ� ������
	char* m_pEndMark;					//������ �������κ��� ����Ű�� �ִ� ������
	char* m_pCurrentMark;				//������ ������� ���� �κ��� ����Ű�� �ִ� ������
	char* m_pGettedBufferMark;			//������� �����͸� ���� ���� ������
	char* m_pLastMoveMark;				//recycle�Ǳ� ���� ������ ������

	int m_nBufferSize;					//���� ������ �� ũ��
	int m_nUsedBufferSize;				//���� ���� ���� ������ ũ��
	unsigned int m_uiAllUserBufSize;	//�� ó���� �����;�

	cMonitor m_csRingBuffer;			//����ȭ ��ü

private:
	cRingBuffer(const cRingBuffer &rhs);
	cRingBuffer &operator=(const cRingBuffer &rhs);
};