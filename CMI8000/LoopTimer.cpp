// LoopTimer.cpp : 구현 파일
//
#include "StdAfx.h"
#include "LoopTimer.h"

#include "LogFile.h"

CLoopTimer::CLoopTimer()
{
	m_dwLoopStart = 0;
	m_dwLoopTerm = 0;

	m_bWaitingRun = FALSE;
	m_dwWaitingStart = 0;
}

CLoopTimer::~CLoopTimer()
{
}

void CLoopTimer::Set_LoopTime(DWORD msec)
{
	m_dwLoopStart = GetTickCount();
	m_dwLoopTerm = msec;
}

BOOL CLoopTimer::Over_LoopTime()
{
//	return FALSE;	// No Timeout Error

	DWORD dwTerm = GetTickCount() - m_dwLoopStart;
	if (dwTerm >= m_dwLoopTerm)
		return TRUE;
	else
		return FALSE;
}

BOOL CLoopTimer::Delay_LoopTime(DWORD msec)
{
	DWORD dwTerm = GetTickCount() - m_dwLoopStart;
	if (dwTerm >= msec) return TRUE;
	else return FALSE;
}

BOOL CLoopTimer::Waiting_Time(DWORD msec)
{
	if (!m_bWaitingRun) {
		m_bWaitingRun = TRUE;
		m_dwWaitingStart = GetTickCount();
	}

	DWORD dwTerm = GetTickCount() - m_dwWaitingStart;
	if (dwTerm < msec) return FALSE;
	
	m_bWaitingRun = FALSE;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

void CLoopTimer::Takt_Start(int nZone, int nCase, BOOL bFirst)
{
	m_dwTakt = GetTickCount();

	// Log 표준화
	CString strZone, strMsg;

	g_objLogFile.Get_ZoneMsg(nZone, nCase, strZone, strMsg);

	if (bFirst) g_objLogFile.Save_StdMotionLog("Y", nZone, nCase, 1, strZone, strMsg);
	g_objLogFile.Save_StdMotionLog("X", nZone, nCase, 1, strZone, strMsg);
}

void CLoopTimer::Takt_End(int nZone, int nCase, BOOL bLast)
{
	CString strLog, strZone, strMsg;

	g_objLogFile.Get_ZoneMsg(nZone, nCase, strZone, strMsg);
		
	strLog.Format("MCC,(%02d) %s,(%02d) %s,%0.3lf",nZone, strZone, nCase, strMsg, (GetTickCount() - m_dwTakt) / 1000.0);
	g_objLogFile.Save_MCCLog(strLog);
	
	// Log 표준화
	g_objLogFile.Save_StdMotionLog("X", nZone, nCase, 0, strZone, strMsg);
	if (bLast) g_objLogFile.Save_StdMotionLog("Y", nZone, nCase, 0, strZone, strMsg);
}

///////////////////////////////////////////////////////////////////////////////
