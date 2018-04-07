#pragma once
class NETLIB_API cMonitor
{
public:

	class NETLIB_API Owner
	{
	public:
		Owner(cMonitor &crit);
		~Owner();

	private:
		cMonitor & m_csSyncObject;

		Owner(const Owner &rhs);
		Owner &operator=(const Owner &rhs);
	};

	cMonitor();
	virtual ~cMonitor();

#if(_WIN32_WINNT >= 0x0400)
	BOOL TryEnter();
#endif
	void Enter();
	void Leave();

private:

	CRITICAL_SECTION m_csSyncObject;

	cMonitor(const cMonitor &rhs);
	cMonitor &operator=(const cMonitor &rhs);
};





