#include "stdafx.h"
#include "cRingBuffer.h"

cRingBuffer::cRingBuffer()
{
	m_pRingBuffer = NULL;
	m_pBeginMark = NULL;
	m_pEndMark = NULL;
	m_pCurrentMark = NULL;
	m_pGettedBufferMark = NULL;
	m_pLastMoveMark = NULL;
	m_nUsedBufferSize = 0;
	m_uiAllUserBufSize = 0;
}


cRingBuffer::~cRingBuffer()
{
	if (NULL != m_pBeginMark)
		delete[] m_pBeginMark;
}

bool cRingBuffer::Create(int nBufferSize)
{
	if (NULL != m_pBeginMark)
		delete[] m_pBeginMark;

	m_pBeginMark = new char[nBufferSize];

	if (NULL == m_pBeginMark)
		return false;

	m_pEndMark = m_pBeginMark + nBufferSize - 1;
	m_nBufferSize = nBufferSize;

	Initialize();
	return false;
}

bool cRingBuffer::Initialize()
{
	cMonitor::Owner lock(m_csRingBuffer);
	{
		m_nUsedBufferSize = 0;
		m_pCurrentMark = m_pBeginMark;
		m_pGettedBufferMark = m_pBeginMark;
		m_pLastMoveMark = m_pEndMark;
		m_uiAllUserBufSize = 0;
	}
	return true;
}

char * cRingBuffer::ForwardMark(int nForwardLength)
{
	char* pPreCurrentMark = NULL;
	cMonitor::Owner lock(m_csRingBuffer);
	{
		//링 버퍼 오버플로 체크
		if (m_nUsedBufferSize + nForwardLength > m_nBufferSize)
			return NULL;

		if ((m_pEndMark - m_pCurrentMark) >= nForwardLength)
		{
			pPreCurrentMark = m_pCurrentMark;
			m_pCurrentMark += nForwardLength;
		}
		else
		{
			//순환되기 전 마지막 좌표를 저장
			m_pLastMoveMark = m_pCurrentMark;
			m_pCurrentMark = m_pBeginMark + nForwardLength;
			pPreCurrentMark = m_pBeginMark;
		}

		m_nUsedBufferSize += nForwardLength;
		m_uiAllUserBufSize += nForwardLength;
	}

	return m_pCurrentMark;
}

char * cRingBuffer::ForwardMark(int nForwardLength, int nNextLength, DWORD dwRemainLength)
{
	cMonitor::Owner lock(m_csRingBuffer);
	{
		//링 버퍼 오버플로 체크
		if (m_nUsedBufferSize + nForwardLength + nNextLength > m_nBufferSize)
			return NULL;

		if ((m_pEndMark - m_pCurrentMark) >= (nNextLength + nForwardLength))
		{
			m_pCurrentMark += nForwardLength;
		}
		else
		{
			//순환되기 전 마지막 좌표를 저장
			m_pLastMoveMark = m_pCurrentMark;
			CopyMemory(m_pBeginMark, m_pCurrentMark - (dwRemainLength - nForwardLength), dwRemainLength);
			m_pCurrentMark = m_pBeginMark + dwRemainLength;
		}

		m_nUsedBufferSize += nForwardLength;
		m_uiAllUserBufSize += nForwardLength;
	}

	return m_pCurrentMark;
}

void cRingBuffer::RelesaseBuffer(int nReleaseSize)
{
	cMonitor::Owner lock(m_csRingBuffer);
	{
		m_nUsedBufferSize -= nReleaseSize;
	}
}

char * cRingBuffer::GetBuffer(int nReadSize, int * pReadSize)
{
	char* pRet = NULL;
	cMonitor::Owner lock(m_csRingBuffer);
	{
		//마지막까지 다 읽었다면 그 읽어드릴 버퍼의 포인터는 맨 앞으로 옮긴다.
		if (m_pLastMoveMark == m_pGettedBufferMark)
		{
			m_pGettedBufferMark = m_pBeginMark;
			m_pLastMoveMark = m_pEndMark;
		}

		//현재 버퍼에 있는 size가 읽어드릴 size보다 크다면
		if (m_nUsedBufferSize > nReadSize)
		{
			//링 버퍼의 끝인지 판단.
			if ((m_pLastMoveMark - m_pGettedBufferMark) >= nReadSize)
			{
				*pReadSize = nReadSize;
				pRet = m_pGettedBufferMark;
				m_pGettedBufferMark += nReadSize;
			}
			else
			{
				*pReadSize = (int)(m_pLastMoveMark - m_pGettedBufferMark);
				pRet = m_pGettedBufferMark;
				m_pGettedBufferMark += *pReadSize;
			}
		}
		else if (m_nUsedBufferSize > 0)
		{
			if ((m_pLastMoveMark - m_pGettedBufferMark) >= m_nUsedBufferSize)
			{
				*pReadSize = m_nUsedBufferSize;
				pRet = m_pGettedBufferMark;
				m_pGettedBufferMark += m_nUsedBufferSize;
			}
			else
			{
				*pReadSize = (int)(m_pLastMoveMark - m_pGettedBufferMark);
				pRet = m_pGettedBufferMark;
				m_pGettedBufferMark += *pReadSize;
			}
		}
	}
	return pRet;
}