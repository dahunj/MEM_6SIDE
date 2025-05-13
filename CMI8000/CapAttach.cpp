// CapAttach.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "CapAttach.h"

#include "LogFile.h"
#include "Common.h"
#include "SequenceMain.h"
#include "MESInterface.h"

#define UDP_CAPATTACH_LPORT	8050
#define UDP_CAPATTACH_HPORT	8050

IMPLEMENT_DYNAMIC(CCapAttach, CWnd)

CCapAttach g_objCapAttach;

CCriticalSection g_csCapAttach;	// Send_Command 문제 해결하기 위함

// CCapAttach

CCapAttach::CCapAttach()
{
	m_strRecvCmd = "";
	m_bOpened = FALSE;
	m_bConnected = FALSE;
	m_nStatusCapAttach = 0;	// 상태 (0:Not Ready, 1:Ready)
}

CCapAttach::~CCapAttach()
{
}

BEGIN_MESSAGE_MAP(CCapAttach, CWnd)
	ON_MESSAGE(UM_UDP_RECEIVE, OnUdpReceive)
END_MESSAGE_MAP()

// CCapAttacher 메시지 처리기입니다.

BOOL CCapAttach::Initialize()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseInlineMode) return FALSE;
	if (m_bOpened) return TRUE;

	m_bOpened = m_UdpCapAttach.Open_Socket(UDP_CAPATTACH_LPORT, UDP_CAPATTACH_HPORT, pEquipData->sCapAttachIp, this);
	if (!m_bOpened) return FALSE;
		
	Set_ConnectRequest();

	CString strLog;
	strLog.Format("Initialize : IP(%s)", pEquipData->sCapAttachIp);
	g_objLogFile.Save_CapAttachLog(strLog);

	return TRUE;
}

void CCapAttach::Terminate()
{
	Set_ConnectEnd();
	m_bOpened = FALSE;
	m_UdpCapAttach.Close_Socket();
	g_objLogFile.Save_CapAttachLog("Terminate");
}

///////////////////////////////////////////////////////////////////////////////
// UDP Socket Message

LRESULT CCapAttach::OnUdpReceive(WPARAM wLocalPort, LPARAM lParam)
{
	UINT nPort = (UINT)wLocalPort;
	BYTE byRecv[1024] = { 0 };
	CString strLog;

	int nLen = m_UdpCapAttach.Read_Socket(byRecv);

	if (nLen < 1) {
		strLog.Format("[H<-C] : Local Port (%d) Mismatch or Receive Data Zero (%d)", nPort, nLen);
		g_objLogFile.Save_CapAttachLog(strLog);
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
			strLog.Format("[H<-C] : <<Error>> %s : Start(%d), End(%d)", m_strRecvCmd, nStart, nEnd);
			g_objLogFile.Save_CapAttachLog(strLog);
			m_strRecvCmd.Delete(0, nEnd + 1);	// 쓰레기값이 채워져 있어서...
			continue;
		}

		CString strRecv = m_strRecvCmd.Mid(nStart + 1, nEnd - nStart - 1);
		m_strRecvCmd.Delete(0, nEnd + 1);

		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		// if (!pEquipData->bUseInlineMode) return 0;

		char chSep = ',';
		CString strCmd, strOp;

		AfxExtractSubString(strCmd, strRecv, 0, chSep);
		AfxExtractSubString(strOp, strRecv, 1, chSep);

		// CapAttach Log /////////////////////////////////////////////////////
		strLog.Format("[H<-C] : %s", strRecv);
		g_objLogFile.Save_CapAttachLog(strLog);
		///////////////////////////////////////////////////////////////////////

		CString strArg[1];
		for (int i = 0; i < 1; i++) AfxExtractSubString(strArg[i], strRecv, i + 2, chSep);

		if (strCmd == "CONNECT") {
			if (strOp == "REQUEST")	Get_ConnectRequest();
			else if (strOp == "REPLY") Get_ConnectReply();
			else if (strOp == "END") Get_ConnectEnd();

		} else if (strCmd == "STATUS") {
			if (strOp == "REQUEST")	Get_StatusRequest();
			else if (strOp == "REPLY") Get_StatusReply(strArg[0]);
			else if (strOp == "UPDATE") Get_StatusUpdate(strArg[0]);

		} else if (strCmd == "TRAY") {
			if (strOp == "UNLOAD") Get_TrayUnload();

		} else if (strCmd == "APD") {
			if (strOp == "REPLY") Get_ApdReply(strArg[0]);
		}
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Get Command

void CCapAttach::Get_ConnectRequest()
{
	m_bConnected = TRUE;
	Set_ConnectReply();
}

void CCapAttach::Get_ConnectReply()
{
	m_bConnected = TRUE;
}

void CCapAttach::Get_ConnectEnd()
{
	m_bConnected = FALSE;
}

void CCapAttach::Get_StatusRequest()
{
	BOOL bStatus = g_objSequenceMain.Is_MainThreadRun();
	if (bStatus) Set_StatusReply(1);
	else Set_StatusReply(0);
}

void CCapAttach::Get_StatusReply(CString sStatus)
{
	m_nStatusCapAttach = atoi(sStatus);
}

void CCapAttach::Get_StatusUpdate(CString sStatus)
{
	m_nStatusCapAttach = atoi(sStatus);
}

void CCapAttach::Get_TrayUnload()
{
	gData.bCapTrayLoad = FALSE;
}

void CCapAttach::Get_ApdReply(CString sCapForceAvg)
{
	gData.dAssyLoadCellAvg = atof(sCapForceAvg);
}

/////////////////////////////////////////////////////////////////////////////
// Set Command

void CCapAttach::Set_ConnectRequest()
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	strSendCmd.Format("CONNECT,REQUEST,%s,%d", pEquipData->sCapAttachIp, UDP_CAPATTACH_LPORT);
	Send_Command(strSendCmd);
}

void CCapAttach::Set_ConnectReply()
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	strSendCmd.Format("CONNECT,REPLY,%s,%d", pEquipData->sCapAttachIp, UDP_CAPATTACH_LPORT);
	Send_Command(strSendCmd);
}

void CCapAttach::Set_ConnectEnd()
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	strSendCmd.Format("CONNECT,END,%s,%d", pEquipData->sCapAttachIp, UDP_CAPATTACH_LPORT);
	Send_Command(strSendCmd);
	m_bConnected = FALSE;
}

void CCapAttach::Set_StatusRequest()
{
	m_nStatusCapAttach = 0;
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	strSendCmd.Format("STATUS,REQUEST,%s,%d", pEquipData->sCapAttachIp, UDP_CAPATTACH_LPORT);
	Send_Command(strSendCmd);
}

void CCapAttach::Set_StatusReply(int nStatus)
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	strSendCmd.Format("STATUS,REPLY,%d,%s,%d", nStatus, pEquipData->sCapAttachIp, UDP_CAPATTACH_LPORT);
	Send_Command(strSendCmd);
}

void CCapAttach::Set_StatusUpdate(int nStatus)
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	strSendCmd.Format("STATUS,UPDATE,%d,%s,%d", nStatus, pEquipData->sCapAttachIp, UDP_CAPATTACH_LPORT);
	Send_Command(strSendCmd);
}

void CCapAttach::Set_LotStart(int nPortNo)
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CString strLotId = gData.sLotID[nPortNo-1];
	int nTrayUseCnt = gData.nTrayUseCount[nPortNo-1];
	int nCmUseCnt = gData.nCmUseCount[nPortNo-1];

	strSendCmd.Format("LOT,START,%s,%d,%d,%d,%s", strLotId, nPortNo, nTrayUseCnt, nCmUseCnt, gData.sRecipe);
	Send_Command(strSendCmd);
}

void CCapAttach::Set_LotEnd(int nPortNo)
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CString strLotId = gData.sLotID[nPortNo-1];

	strSendCmd.Format("LOT,END,%s,%d", strLotId, nPortNo);
	Send_Command(strSendCmd);
}

void CCapAttach::Set_TrayLoad(int nPortNo, int nSNo)
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CString strLotId = gData.sCLotID[nSNo-1];
	int nTrayNo = gData.nCGoodTrayCount[nSNo-1];
	int nCmCnt = gData.nCTrayCmCnt[nSNo-1];

	strSendCmd.Format("TRAY,LOAD,%s,%d,%d,%d", strLotId, nTrayNo, nCmCnt, nPortNo);
	Send_Command(strSendCmd);
}

void CCapAttach::Set_TrayEnd(int nPortNo)
{
	CString	strSendCmd;
	int nTrayCnt = gData.nLastTrayNo;

	strSendCmd.Format("TRAY,END,%d,%d", nTrayCnt, nPortNo);
	Send_Command(strSendCmd);
}

void CCapAttach::Set_BarcodeUpdate(int nPortNo, int nTrayNo, int nCmNo, CString sBarcode)
{
	CString	strSendCmd;
	strSendCmd.Format("BARCODE,UPDATE,%d,%d,%d,%s", nPortNo, nTrayNo, nCmNo, sBarcode);
	Send_Command(strSendCmd);
}

void CCapAttach::Set_ApdRequest()
{
	CString	strSendCmd;
	gData.dAssyLoadCellAvg = -1.0;
	strSendCmd.Format("APD,REQUEST");
	Send_Command(strSendCmd);
}

void CCapAttach::Set_TimeUpdate()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d %03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	strSendCmd.Format("TIME,UPDATE,%s,%s,%d", strTime, pEquipData->sCapAttachIp, UDP_CAPATTACH_LPORT);
	Send_Command(strSendCmd);
}

/////////////////////////////////////////////////////////////////////////////
// UDP Socket Send Message

void CCapAttach::Send_Command(CString strSend)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseInlineMode) return;

	CString strLog;
	strLog.Format("[H->C] : %s", strSend);
	g_objLogFile.Save_CapAttachLog(strLog);

	CString strSendSocket;
	strSendSocket.Format("@%s\n", strSend);

	char chSend[1024] = { 0 };
	int nLength = strSendSocket.GetLength();
	memcpy(chSend, (LPSTR)(LPCSTR)strSendSocket, nLength);

	g_csCapAttach.Lock();	// Critical Section

	m_UdpCapAttach.Write_Socket((BYTE*)chSend, nLength);

	g_csCapAttach.Unlock();	// Critical Section
}

/////////////////////////////////////////////////////////////////////////////

void CCapAttach::Test_Command(int nNo)
{
}
