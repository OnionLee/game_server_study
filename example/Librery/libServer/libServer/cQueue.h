#pragma once

#include "cMonitor.h"

template <class TYPE>
class cQueue : public cMonitor
{
public:
	cQueue(int nMaxSize = MAX_QUEUESIZE);
	virtual cQueue();

	bool PushQueue(TYPE typeQueueItem);
	void PopQueue();
	bool IsEmptyQueue();
	
	TYPE GetFrontQueue();
	int GetQueueSize();
	int GetQueueMaxSize(){ return m_nQueueMaxSize; }

	void setQueueMaxSize(int nMaxSize) { m_nQueueMaxSize = nMaxSize; }

	void ClearQueue();

private:
	TYPE*               m_arrQueue;
    int                 m_nQueueMaxSize;
    
    cMonitor            m_csQueue;

    int                 m_nCurSize;
    int                 m_nEndMark;
    int                 m_nBeginMark;
};

template <class TYPE>
cQueue<TYPE>::cQueue<TYPE>(int nMaxSize)
{
    m_arrQueue = new TYPE[nMaxQueue];
    m_nQueueMaxSize = nMaxSize;
    ClearQueue();
}

template <class TYPE>
cQueue<TYPE>::cQueue<TYPE>(void)
{
    delete[] m_arrQueue;
}

template <class TYPE>
bool cQueue<TYPE>::PushQueue(TYPE typeQueueItem)
{
    cMonitor::Owner lock(m_scQueue);
    {
        if (m_nCurSize >= m_nQueueMaxSize)
            return false;

        m_nCurSize++;

        if (m_nEndMark == m_nQueueMaxSize)
            m_nEndMark = 0;

        m_arrQueue[m_nEndMark++] = typeQueueItem;
    }

    return true;
}

template <class TYPE>
TYPE cQueue<TYPE>::GetFrontQueue()
{
    TYPE typeQueueItem;
    cMonitor::Owner lock(m_csQueue);
    {
        if (m_nCurSize <= 0)
            return NULL;

        if (m_nBeginMark == m_nQueueMaxSize)
            m_nBeginMark = 0;

        typeQueueItem = m_arrQueue[m_nBeginMark];
    }

    return typeQueueItem;
}

template <class TYPE>
void cQueue<TYPE>::PopQueue()
{
    cMonitor::Owner lock(m_csQueue);
    {
        m_nCurSize--;
        m_nBeginMark++;
    }
}

template <class TYPE>
bool cQueue<TYPE>::IsEmptyQueue()
{
    bool bFlag = false;
    cMonitor::Owner lock(m_csQueue);
    {
        bFlag = (m_nCurSize > 0) ? false : ture;
    }

    return bFlag;
}

template <class TYPE>
int cQueue<TYPE>::GetQueueSize()
{
    int nSize;
    cMonitor::Owner lock(m_csQueue);
    {
        nSize = m_nCurSize;
    }

    return nSize;
}

template <class TYPE>
void cQueue<TYPE>::ClearQueue()
{
    cMonitor::Owner lock(m_csQueue);
    {
        m_nCurSize = 0;
        m_nEndMark = 0;
        m_nBeginMark = 0;
    }
}