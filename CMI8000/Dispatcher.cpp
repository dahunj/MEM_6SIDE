// Dispatcher.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "Dispatcher.h"

#include "LogFile.h"
#include "Common.h"
#include "SequenceMain.h"
#include "SetupDlg.h"

#define UDP_DISPATCHER_LPORT	8020
#define UDP_DISPATCHER_HPORT	8020

IMPLEMENT_DYNAMIC(CDispatcher, CWnd)

CDispatcher g_objDispatcher;

CCriticalSection g_csDispatcher;	// Send_Command 문제 해결하기 위함

// CDispatcher

CDispatcher::CDispatcher()
{
	m_bConnected = FALSE;
	m_strRecvCmd = "";
	Reset_JudgeData(0);
}

CDispatcher::~CDispatcher()
{
}

BEGIN_MESSAGE_MAP(CDispatcher, CWnd)
	ON_MESSAGE(UM_UDP_CLIENT_RECEIVE, OnUdpClientReceive)
END_MESSAGE_MAP()

// CDispatcherer 메시지 처리기입니다.

BOOL CDispatcher::Initialize()
{
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	BOOL bOpenRecv = m_UdpRecv.Open_Socket(UDP_DISPATCHER_LPORT, this);
	BOOL bOpenSend = m_UdpSend.Open_Socket(this);
	if (!bOpenRecv || !bOpenSend) return FALSE;

	CString strSpecialNg = (pRosData->bJudgeSpecialNg ? "TRUE" : "FALSE");
	CString strLog;
	strLog.Format("Initialize : Dispatcher IP(%s), Handler IP(%s), TimeOver(%d msec), SpecialNg(%s)",
		pRosData->sDispatcherIp, pRosData->sHandlerIp, pRosData->nJudgeTimeOver, strSpecialNg);
	g_objLogFile.Save_DispatcherLog(strLog);

	Set_ConnectRequest();

	return TRUE;
}

void CDispatcher::Terminate()
{
	Set_ConnectEnd();
	m_UdpRecv.Close_Socket();
	m_UdpSend.Close_Socket();
	g_objLogFile.Save_DispatcherLog("Terminate");
}

void CDispatcher::Reset_JudgeData(int nPortNo)
{
	if (nPortNo == 0) {
		memset(m_bJudgeReq, 0x00, sizeof(BOOL) * 2 * 30 * 40);
		memset(m_bJudgeDone, 0x00, sizeof(BOOL) * 2 * 30 * 40);
		memset(m_dwReqStart, 0x00, sizeof(DWORD) * 2 * 30 * 40);
	} else {
		int nPx = nPortNo - 1;
		memset(m_bJudgeReq[nPx], 0x00, sizeof(BOOL) * 30 * 40);
		memset(m_bJudgeDone[nPx], 0x00, sizeof(BOOL) * 30 * 40);
		memset(m_dwReqStart[nPx], 0x00, sizeof(DWORD) * 30 * 40);
	}
}

///////////////////////////////////////////////////////////////////////////////
// UDP Socket Message

LRESULT CDispatcher::OnUdpClientReceive(WPARAM wLocalPort, LPARAM lParam)
{
	UINT nPort = (UINT)wLocalPort;
	BYTE byRecv[1025] = { 0 };
	CString strLog;

	int nLen = m_UdpRecv.Read_Socket(byRecv);

	if (nLen < 1) {
		strLog.Format("[H<-J] : Local Port (%d) Mismatch or Receive Data Zero (%d)", nPort, nLen);
		g_objLogFile.Save_DispatcherLog(strLog);
		return 0;
	}

	CString strRecvSocket;
	strRecvSocket.Format("%s", byRecv);
	m_strRecvCmd += strRecvSocket;

	while (!m_strRecvCmd.IsEmpty()) {
		int nStart = m_strRecvCmd.Find("@");
		int nEnd = m_strRecvCmd.Find("\n");

		if (nEnd < 0) break;	// 버퍼에 들어오는 중...

		if (nStart < 0 || nStart > nEnd) {
			strLog.Format("[H<-J] : <<Error>> %s : Start(%d), End(%d)", m_strRecvCmd, nStart, nEnd);
			g_objLogFile.Save_DispatcherLog(strLog);
			m_strRecvCmd.Delete(0, nEnd + 1);	// 쓰레기값이 채워져 있어서...
			continue;
		}

		CString strRecv = m_strRecvCmd.Mid(nStart + 1, nEnd - nStart - 1);
		m_strRecvCmd.Delete(0, nEnd + 1);

		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		if (!pEquipData->bUseDispatcher) return 0;

		char chSep = ',';
		CString strCmd, strOp, strIp;

		AfxExtractSubString(strCmd, strRecv, 0, chSep);
		AfxExtractSubString(strOp, strRecv, 1, chSep);
		AfxExtractSubString(strIp, strRecv, 2, chSep);	// Recieve MDJ IP

		// Dispatcher Log /////////////////////////////////////////////////////
		strLog.Format("[H<-J] : %s", strRecv);
		g_objLogFile.Save_DispatcherLog(strLog);
		///////////////////////////////////////////////////////////////////////

		CString strArg[8];
		for (int i = 0; i < 8; i++) AfxExtractSubString(strArg[i], strRecv, i + 3, chSep);

		if (strCmd == "CONNECT") {
			if (strOp == "REQUEST")	Get_ConnectRequest(strIp);
			else if (strOp == "REPLY") Get_ConnectReply();
			else if (strOp == "END") Get_ConnectEnd();

		} else if (strCmd == "STATUS") {
			if (strOp == "REQUEST")	Get_StatusRequest();
			else if (strOp == "REPLY") Get_StatusReply(strArg[0]);
			else if (strOp == "UPDATE") Get_StatusUpdate(strArg[0]);

		} else if (strCmd == "JUDGE") {
			if (strOp == "REPLY") Get_JudgeReply(strArg[0], strArg[1], strArg[2], strArg[3], strArg[4], strArg[5], strArg[6], strArg[7]);

		} else if (strCmd == "INFO") {
			if (strOp == "REQUEST")	Get_InfoRequest();
		}
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Get Command

void CDispatcher::Get_ConnectRequest(CString sIp)
{
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	if (sIp != pRosData->sDispatcherIp) {
		Set_ConnectEnd();
		Set_DispatcherIp(sIp);
		if (g_dlgSetup.m_pSetupEquipDlg->IsWindowVisible()) g_dlgSetup.m_pSetupEquipDlg->Update_Ip(sIp);
	}
	m_bConnected = TRUE;
	Set_ConnectReply();
}

void CDispatcher::Get_ConnectReply()
{
	m_bConnected = TRUE;
}

void CDispatcher::Get_ConnectEnd()
{
	m_bConnected = FALSE;
}

void CDispatcher::Get_StatusRequest()
{
	int nEqStatus = (int)g_objSequenceMain.Is_MainThreadRun();
	Set_StatusReply(nEqStatus);
}

void CDispatcher::Get_StatusReply(CString sStatus)
{
	int nRosStatus = atoi(sStatus);
}

void CDispatcher::Get_StatusUpdate(CString sStatus)
{
	int nRosStatus = atoi(sStatus);
}

void CDispatcher::Get_InfoRequest()
{
	Set_InfoReply();
}

void CDispatcher::Get_JudgeReply(CString sModel, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sBarcode, CString sCode, CString sJudge)
{
	CString strLog;
	int	nPx = atoi(sPortNo) - 1;	// Port Index
	int	nTx = atoi(sTrayNo) - 1;	// Tray Index
	int	nCx = atoi(sCmNo) - 1;		// CM Index

	if (sLotId != gData.sLotID[nPx] || !m_bJudgeReq[nPx][nTx][nCx] || nTx < 0 || nTx > 29 || nCx < 0 || nCx > 39) {
		strLog.Format("[Get_JudgeReply] Receive Data Error ==> LotId(%s),JudgeReq(%d),PortNo(%d),TrayNo(%d),CmNo(%d),Judge(%s)", sLotId, (int)m_bJudgeReq, nPx+1, nTx+1, nCx+1, sJudge);
		g_objLogFile.Save_DispatcherLog(strLog);
		return;
	}
	
	if (((gData.byInspectDone[nPx][nTx][nCx] >> 7) & 1) == 1) {
		strLog.Format("[Get_JudgeReply] Already been judged,LotId(%s),PortNo(%d),TrayNo(%d),CmNo(%d),Judge(%s)", sLotId, nPx+1, nTx+1, nCx+1, sJudge);
		g_objLogFile.Save_DispatcherLog(strLog);
		return;
	}

	if (sJudge == "G") { gData.nInspectInfo[nPx][nTx][nCx] = 1; gLot.nRosGood[nPx]++; }		// Good
	if (sJudge == "N") { gData.nInspectInfo[nPx][nTx][nCx] = 2; gLot.nRosNg[nPx]++; }		// ROS NG
	if (sJudge == "R") {										gLot.nRosRepair[nPx]++; }	// Normal NG

	m_bJudgeDone[nPx][nTx][nCx] = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Set Command

void CDispatcher::Set_ConnectRequest()
{
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	CString	strSendCmd;
	strSendCmd.Format("CONNECT,REQUEST,%s", pRosData->sHandlerIp);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_ConnectReply()
{
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	CString	strSendCmd;
	strSendCmd.Format("CONNECT,REPLY,%s", pRosData->sHandlerIp);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_ConnectEnd()
{
	m_bConnected = FALSE;

	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	CString	strSendCmd;
	strSendCmd.Format("CONNECT,END,%s", pRosData->sHandlerIp);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_StatusRequest()
{
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	CString	strSendCmd;
	strSendCmd.Format("STATUS,REQUEST,%s", pRosData->sHandlerIp);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_StatusReply(int nStatus)
{
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	CString	strSendCmd;
	strSendCmd.Format("STATUS,REPLY,%s,%d", pRosData->sHandlerIp, nStatus);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_StatusUpdate(int nStatus)
{
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	CString	strSendCmd;
	strSendCmd.Format("STATUS,UPDATE,%s,%d", pRosData->sHandlerIp, nStatus);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_LotStart(int nPortNo)
{
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();

	CString strLotId = gData.sLotID[nPortNo-1];
	int nTrayUseCnt = gData.nTrayUseCount[nPortNo-1];
	int nCmUseCnt = gData.nCmUseCount[nPortNo-1];

	CString	strSendCmd;
	strSendCmd.Format("LOT,START,%s,%s,%s,%d,%d,%d", pRosData->sHandlerIp, gData.sRecipe, strLotId, nPortNo, nTrayUseCnt, nCmUseCnt);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_LotEnd(int nPortNo)
{
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();

	CString strLotId = gData.sLotID[nPortNo-1];

	CString	strSendCmd;
	strSendCmd.Format("LOT,END,%s,%s,%s,%d", pRosData->sHandlerIp, gData.sRecipe, strLotId, nPortNo);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_InfoReply()
{
	if (gData.nLPNo == 0) return; 

	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();

	CString strLotId = gData.sLotID[gData.nLPNo-1];
	int nTrayUseCnt = gData.nTrayUseCount[gData.nLPNo-1];
	int nCmUseCnt = gData.nCmUseCount[gData.nLPNo-1];

	CString	strSendCmd;
	strSendCmd.Format("INFO,REPLY,%s,%s,%s,%d,%d,%d", pRosData->sHandlerIp, gData.sRecipe, strLotId, gData.nLPNo, nTrayUseCnt, nCmUseCnt);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_JudgeRequest(int nPortNo, int nTrayNo, int nCmNo)
{
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();

	CString strLotId = gData.sLotID[nPortNo-1];
	CString strBarcode = gMes.sBarID[nPortNo-1][nTrayNo-1][nCmNo-1];
	CString strNgCode = gMes.sNGCode[nPortNo-1][nTrayNo-1][nCmNo-1];

	CString	strSendCmd;
	strSendCmd.Format("JUDGE,REQUEST,%s,%s,%s,%d,%d,%d,%s,%s",
		pRosData->sHandlerIp, gData.sRecipe, strLotId, nPortNo, nTrayNo, nCmNo, strBarcode, strNgCode);
	Send_Command(strSendCmd);

	gLot.nRosRequest[nPortNo-1]++;
	m_dwReqStart[nPortNo-1][nTrayNo-1][nCmNo-1] = GetTickCount();
	m_bJudgeReq[nPortNo-1][nTrayNo-1][nCmNo-1] = TRUE;
}

void CDispatcher::Set_TimeUpdate()
{
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d,%02d,%02d,%02d,%02d,%02d,%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	CString	strSendCmd;
	strSendCmd.Format("TIME,UPDATE,%s,%s", pRosData->sHandlerIp, strTime);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_DispatcherIp(CString sIp)
{
	CIniFileCS INI(gData.sEnvPath + "\\RosData.ini");
	INI.Set_String("DISPATCHER", "DISPATCHER_IP", sIp);

	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	pRosData->sDispatcherIp = sIp;
}

/////////////////////////////////////////////////////////////////////////////
// UDP Socket Send Message

void CDispatcher::Send_Command(CString strSend)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseDispatcher) return;

	CString strLog;
	strLog.Format("[H->J] : %s", strSend);
	g_objLogFile.Save_DispatcherLog(strLog);

	CString strSendSocket;
	strSendSocket.Format("@%s\n", strSend);

	char chSend[1025] = { 0 };
	int nLength = strSendSocket.GetLength();
	memcpy(chSend, (LPSTR)(LPCSTR)strSendSocket, nLength);

	g_csDispatcher.Lock();		// Critical Section
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	m_UdpSend.Write_Socket(pRosData->sDispatcherIp, UDP_DISPATCHER_HPORT, (BYTE*)chSend, nLength);
	g_csDispatcher.Unlock();	// Critical Section
}

///////////////////////////////////////////////////////////////////////////////

BOOL CDispatcher::Is_JudgeDone(int nPortNo, int nTrayNo, int nCmNo)
{
	if (nPortNo == 0 || nTrayNo == 0 || nCmNo == 0 || gData.bCycleStop) return TRUE;
	int nPx = nPortNo - 1;	// Port Index
	int	nTx = nTrayNo - 1;	// Tray Index
	int	nCx = nCmNo - 1;	// CM Index

	if (m_bJudgeDone[nPx][nTx][nCx]) return TRUE;

	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	int nTerm = (int)(GetTickCount() - m_dwReqStart[nPx][nTx][nCx]);

	if (m_dwReqStart[nPx][nTx][nCx] > 0 && nTerm > pRosData->nJudgeTimeOver) {		// Time Over
		CString strLog;
		strLog.Format("Judge Time Over(%d msec), PortNo(%d),TrayNo(%d), CmNo(%d)", pRosData->nJudgeTimeOver, nPortNo, nTrayNo, nCmNo);
		g_objLogFile.Save_DispatcherLog(strLog);
		m_bJudgeDone[nPx][nTx][nCx] = TRUE;
		gLot.nRosTimeOut[nPx]++;
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

void CDispatcher::Test_Command(int nNo)
{
}
