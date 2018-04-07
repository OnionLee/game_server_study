#include "cSingleton.h"
#include "CommonDef.h"

#pragma once
class NETLIB_API cVBuffer : public cSingleton
{
	DECLEAR_SINGLETON(cVBuffer);
public:
	cVBuffer(int nMaxBufSize = MAX_VBUFFER_SIZE);
	~cVBuffer(void);

	void GetChar(char& cCh);
	void GetShort(short& nNum);
	void GetInterger(int& nNum);
	void GetString(char * pszBuffer);
	void GetStream(char * pszBuffer, short sLen);
	void SetInterger(int nI);
	void SetShort(short sShort);
	void SetChar(char cCh);
	void SetString(char * pszBuffer);
	void SetStream(char * pszBuffer, short sLen);

	void SetBuffer(char * pVBuffer);

	inline int GetMaxBufSize() { return m_nMaxBufSize; }
	inline int GetCurBufSize() { return m_nCurBufSize; }
	inline char * GetCurMark() { return m_pCurMark; }
	inline char * GetBeginMark() { return m_pszVBuffer; }

	bool CopyBuffer(char * pDestBuffer);
	void Init();

private:

	char * m_pszVBuffer;
	char * m_pCurMark;

	int m_nMaxBufSize;

	int m_nCurBufSize;

	cVBuffer(const cVBuffer &rhs);
	cVBuffer &operator=(const cVBuffer &rhs);
};

CREATE_FUNCTION(cVBuffer, VBuffer)

