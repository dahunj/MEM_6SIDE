// LoopTimer.h : 헤더 파일
//
#pragma once

class CLoopTimer
{
public:
	CLoopTimer();
	virtual ~CLoopTimer();

private:
	DWORD	m_dwLoopStart;
	DWORD	m_dwLoopTerm;
	DWORD	m_dwTakt;

	BOOL	m_bWaitingRun;
	DWORD	m_dwWaitingStart;

public:
	void Set_LoopTime(DWORD msec);
	BOOL Over_LoopTime();
	BOOL Delay_LoopTime(DWORD msec);
	BOOL Waiting_Time(DWORD msec);

	void Takt_Start(int nZone, int nCase, BOOL bFirst=FALSE);
	void Takt_End(int nZone, int nCase, BOOL bLast=FALSE);
};

///////////////////////////////////////////////////////////////////////////////
