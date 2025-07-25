// VisionCommManager.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Main.h"
#include "VisionCommManager.h"


// CVisionCommManager
#define UDP_PC1_HOST_IP		"127.0.0.1"
#define UDP_PC2_HOST_IP		"127.0.0.1"
#define UDP_PC3_HOST_IP		"127.0.0.1"
#define UDP_PC4_HOST_IP		"127.0.0.1"

#define UDP_PC1_LPORT		10001
#define UDP_PC1_HPORT		10000
#define UDP_PC2_LPORT		11001
#define UDP_PC2_HPORT		11000
#define UDP_PC3_LPORT		12001
#define UDP_PC3_HPORT		12000
#define UDP_PC4_LPORT		13001
#define UDP_PC4_HPORT		13000

CCriticalSection g_csInspector;	// Send_Command 문제 해결하기 위함

CVisionCommManager g_objVIsionManager;

IMPLEMENT_DYNAMIC(CVisionCommManager, CWnd)

CVisionCommManager::CVisionCommManager()
{
	m_strRecvCmd = "";

	m_bConnectPC1 = FALSE;
	m_bConnectPC2 = FALSE;
	m_bConnectPC3 = FALSE;
	m_bConnectPC4 = FALSE;

	m_nStatusPC1 = 0;		// Vision PC1 상태 (0:Not Ready, 1:Ready)
	m_nStatusPC2 = 0;		// Vision PC2 상태 (0:Not Ready, 1:Ready)
	m_nStatusPC3 = 0;		// Vision PC3 상태 (0:Not Ready, 1:Ready)
	m_nStatusPC4 = 0;		// Vision PC4 상태 (0:Not Ready, 1:Ready)

	m_bLotReady1 = FALSE;
	m_bLotReady2 = FALSE;
	m_bLotReady3 = FALSE;
	m_bLotReady4 = FALSE;


}

CVisionCommManager::~CVisionCommManager()
{
}


BEGIN_MESSAGE_MAP(CVisionCommManager, CWnd)
	ON_WM_TIMER()
	ON_MESSAGE(UM_UDP_RECEIVE, OnUdpReceive)
END_MESSAGE_MAP()


void CVisionCommManager::Initialize()
{
	BOOL bOpenedPC1 = m_UdpVisionPC1.Open_Socket(10001, 10000, "127.0.0.1", this);
	BOOL bOpenedPC2 = m_UdpVisionPC2.Open_Socket(11001, 11000, "127.0.0.1", this);
	BOOL bOpenedPC3 = m_UdpVisionPC3.Open_Socket(12001, 12000, "127.0.0.1", this);
	BOOL bOpenedPC4 = m_UdpVisionPC4.Open_Socket(13001, 13000, "127.0.0.1", this);
	
}

void CVisionCommManager::Terminate()
{
	m_UdpVisionPC1.Close_Socket();
	m_UdpVisionPC2.Close_Socket();
	m_UdpVisionPC3.Close_Socket();
	m_UdpVisionPC4.Close_Socket();
}



LRESULT CVisionCommManager::OnUdpReceive(WPARAM wLocalPort, LPARAM lParam)
{
	UINT nPort = (UINT)wLocalPort;
	int nInspector = 0, nLen = 0;
	BYTE byRecv[1024] = { 0 };
	CString strLog;

	if (nPort == 10000) { nInspector = INSPECTOR_PC1; nLen = m_UdpVisionPC1.Read_Socket(byRecv); }
	if (nPort == 11000) { nInspector = INSPECTOR_PC2; nLen = m_UdpVisionPC2.Read_Socket(byRecv); }
	if (nPort == 12000) { nInspector = INSPECTOR_PC3; nLen = m_UdpVisionPC3.Read_Socket(byRecv); }
	if (nPort == 13000) { nInspector = INSPECTOR_PC4; nLen = m_UdpVisionPC4.Read_Socket(byRecv); }

	if (nInspector == 0 || nLen < 1) {
		strLog.Format("[H<-V%d] : Local Port (%d) Mismatch or Receive Data Zero (%d)", nInspector, nPort, nLen);
		//g_objLogFile.Save_InspectorLog(strLog);
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
			strLog.Format("[H<-V%d] : <<Error>> %s : Start(%d), End(%d)", nInspector, m_strRecvCmd, nStart, nEnd);
			//g_objLogFile.Save_InspectorLog(strLog);
			m_strRecvCmd.Delete(0, nEnd + 1);	// 쓰레기값이 채워져 있어서...
			continue;
		}

		CString strRecv = m_strRecvCmd.Mid(nStart + 1, nEnd - nStart - 1);
		m_strRecvCmd.Delete(0, nEnd + 1);

		char chSep = ',';
		CString strCmd, strOp;

		AfxExtractSubString(strCmd, strRecv, 0, chSep);
		AfxExtractSubString(strOp, strRecv, 1, chSep);

		// Inspector Log ////////////////////////////////////////
		if (strCmd != "HEART" && strOp != "BEAT") {
			strLog.Format("[H<-V%d] : %s", nInspector, strRecv);
			//g_objLogFile.Save_InspectorLog(strLog);
		}
		/////////////////////////////////////////////////////////
		
		CString strArg[7];
		for (int i = 0; i < 7; i++) AfxExtractSubString(strArg[i], strRecv, i + 2, chSep);

		if (strCmd == "CONNECT") {
			if (strOp == "REQUEST")	Get_ConnectRequest(nInspector);
			else if (strOp == "REPLY") Get_ConnectReply(nInspector);
			else if (strOp == "END") Get_ConnectEnd(nInspector);

		} else if (strCmd == "STATUS") {
			if (strOp == "REQUEST")	Get_StatusRequest(nInspector);
			else if (strOp == "REPLY") Get_StatusReply(nInspector, strArg[0]);
			else if (strOp == "UPDATE") Get_StatusUpdate(nInspector, strArg[0]);

		} 
	}
	return 1;
}


void CVisionCommManager::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	switch (nIDEvent) {
	case INSPECTOR_PC1:	m_nStatusPC1 = 0; break;
	case INSPECTOR_PC2:	m_nStatusPC2 = 0; break;
	case INSPECTOR_PC3:	m_nStatusPC3 = 0; break;
	case INSPECTOR_PC4:	m_nStatusPC4 = 0; break;
	}
	CWnd::OnTimer(nIDEvent);
}

void CVisionCommManager::DoEvents(int nSleep)
{
	MSG msg;
	if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (nSleep > 0) Sleep(nSleep);
}


void CVisionCommManager::Send_Command(int nInspector, CString strSend)
{
	// Inspector Log //////////////////////////////////////
	CString strLog;
	strLog.Format("[H->V%d] : %s", nInspector, strSend);
	//g_objLogFile.Save_InspectorLog(strLog);
	///////////////////////////////////////////////////////

	g_csInspector.Lock();	// Critical Section

	CString strSendSocket;
	strSendSocket.Format("@%s\n", strSend);

	char chSend[1024] = { 0 };
	int nLength = strSendSocket.GetLength();
	memcpy(chSend, (LPSTR)(LPCSTR)strSendSocket, nLength);

	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC1) m_UdpVisionPC1.Write_Socket((BYTE*)chSend, nLength);
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC2) m_UdpVisionPC2.Write_Socket((BYTE*)chSend, nLength);
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC3) m_UdpVisionPC3.Write_Socket((BYTE*)chSend, nLength);
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC4) m_UdpVisionPC4.Write_Socket((BYTE*)chSend, nLength);

	g_csInspector.Unlock();	// Critical Section
}


///////////////////////////////////////////////////////////////////////////////
// Get Command

void CVisionCommManager::Get_ConnectRequest(int nInspector)
{
	if (nInspector == INSPECTOR_PC1) m_bConnectPC1 = TRUE;
	if (nInspector == INSPECTOR_PC2) m_bConnectPC2 = TRUE;
	if (nInspector == INSPECTOR_PC3) m_bConnectPC3 = TRUE;
	if (nInspector == INSPECTOR_PC4) m_bConnectPC4 = TRUE;
	//Set_ConnectReply(nInspector);
}

void CVisionCommManager::Get_ConnectReply(int nInspector)
{
	if (nInspector == INSPECTOR_PC1) m_bConnectPC1 = TRUE;
	if (nInspector == INSPECTOR_PC2) m_bConnectPC2 = TRUE;
	if (nInspector == INSPECTOR_PC3) m_bConnectPC3 = TRUE;
	if (nInspector == INSPECTOR_PC4) m_bConnectPC4 = TRUE;
}

void CVisionCommManager::Get_ConnectEnd(int nInspector)
{
	if (nInspector == INSPECTOR_PC1) m_bConnectPC1 = FALSE;
	if (nInspector == INSPECTOR_PC2) m_bConnectPC2 = FALSE;
	if (nInspector == INSPECTOR_PC3) m_bConnectPC3 = FALSE;
	if (nInspector == INSPECTOR_PC4) m_bConnectPC4 = FALSE;
}

void CVisionCommManager::Get_StatusRequest(int nInspector)
{
	//BOOL bStatus = g_objSequenceMain.Is_MainThreadRun();
	//if (bStatus) Set_StatusReply(nInspector, 1);
	//else Set_StatusReply(nInspector, 0);
}

void CVisionCommManager::Get_StatusReply(int nInspector, CString sStatus)
{
	if (nInspector == INSPECTOR_PC1) m_nStatusPC1 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC2) m_nStatusPC2 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC3) m_nStatusPC3 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC4) m_nStatusPC4 = atoi(sStatus);
}

void CVisionCommManager::Get_StatusUpdate(int nInspector, CString sStatus)
{
	KillTimer(nInspector);
	if (nInspector == INSPECTOR_PC1) m_nStatusPC1 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC2) m_nStatusPC2 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC3) m_nStatusPC3 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC4) m_nStatusPC4 = atoi(sStatus);
	SetTimer(nInspector, 15000, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// Set Command

void CVisionCommManager::Set_ConnectRequest(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("CONNECT,REQUEST");
	Send_Command(nInspector, strSendCmd);
}

void CVisionCommManager::Set_ConnectReply(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("CONNECT,REPLY");
	Send_Command(nInspector, strSendCmd);
}

void CVisionCommManager::Set_ConnectEnd(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("CONNECT,END");
	Send_Command(nInspector, strSendCmd);

	if (nInspector == INSPECTOR_PC1) m_bConnectPC1 = FALSE;
	if (nInspector == INSPECTOR_PC2) m_bConnectPC2 = FALSE;
	if (nInspector == INSPECTOR_PC3) m_bConnectPC3 = FALSE;
	if (nInspector == INSPECTOR_PC4) m_bConnectPC4 = FALSE;
}

void CVisionCommManager::Set_StatusRequest(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("STATUS,REQUEST");
	Send_Command(nInspector, strSendCmd);
}

void CVisionCommManager::Set_StatusReply(int nInspector, int nStatus)
{
	CString	strSendCmd;
	strSendCmd.Format("STATUS,REPLY,%d", nStatus);
	Send_Command(nInspector, strSendCmd);
}

void CVisionCommManager::Set_StatusUpdate(int nInspector, int nStatus)
{
	CString	strSendCmd;
	strSendCmd.Format("STATUS,UPDATE,%d", nStatus);
	Send_Command(nInspector, strSendCmd);
}

// CVisionCommManager 메시지 처리기입니다.

