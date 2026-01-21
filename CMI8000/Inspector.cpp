// Inspector.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "Inspector.h"

#include "LogFile.h"
#include "Common.h"
#include "DataManager.h"

#include "SequenceMain.h"
#include "MESInterface.h"
#include "Dispatcher.h"

#define UDP_PC1_HOST_IP		"192.168.0.21"
#define UDP_PC2_HOST_IP		"192.168.0.22"
#define UDP_PC3_HOST_IP		"192.168.0.23"
#define UDP_PC4_HOST_IP		"192.168.0.24"

#define UDP_PC1_LPORT		20001
#define UDP_PC1_HPORT		20001
#define UDP_PC2_LPORT		20002
#define UDP_PC2_HPORT		20002
#define UDP_PC3_LPORT		20003
#define UDP_PC3_HPORT		20003
#define UDP_PC4_LPORT		20004
#define UDP_PC4_HPORT		20004

IMPLEMENT_DYNAMIC(CInspector, CWnd)

CInspector g_objInspector;

CCriticalSection g_csInspector;	// Send_Command 문제 해결하기 위함

// CInspector

CInspector::CInspector()
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

	for (int i = 0; i < 4; i++) for (int j = 0; j < 2; j++) m_bHddVision[i][j] = FALSE;

	m_nB1_SPScanReq = 0;
	m_nB1_AGScanReq = 0;
	m_nT1ScanReq = 0;
	m_nT2ScanReq = 0;
	m_nB2ScanReq = 0;
	m_nB1_3DScanReq = 0;
	m_nB1_SPScanCnt = 0;
	m_nB1_AGScanCnt = 0;
	m_nT1ScanCnt = 0;
	m_nT2ScanCnt = 0;
	m_nB2ScanCnt = 0;
	m_nB1_3DScanCnt = 0;
}

CInspector::~CInspector()
{
}

BEGIN_MESSAGE_MAP(CInspector, CWnd)
	ON_WM_TIMER()
	ON_MESSAGE(UM_UDP_RECEIVE, OnUdpReceive)
END_MESSAGE_MAP()

// CInspector 메시지 처리기입니다.

void CInspector::Initialize()
{

#ifndef AJIN_BOARD_USE
	BOOL bOpenedPC1 = m_UdpVisionPC1.Open_Socket(10000, 10001, "127.0.0.1", this);
	BOOL bOpenedPC2 = m_UdpVisionPC2.Open_Socket(11000, 11001, "127.0.0.1", this);
	BOOL bOpenedPC3 = m_UdpVisionPC3.Open_Socket(12000, 12001, "127.0.0.1", this);
	BOOL bOpenedPC4 = m_UdpVisionPC4.Open_Socket(13000, 13001, "127.0.0.1", this);

	if (bOpenedPC1) Set_ConnectRequest(INSPECTOR_PC1);
	if (bOpenedPC2) Set_ConnectRequest(INSPECTOR_PC2);
	if (bOpenedPC3) Set_ConnectRequest(INSPECTOR_PC3);
	if (bOpenedPC4) Set_ConnectRequest(INSPECTOR_PC4);

#else
	BOOL bOpenedPC1 = m_UdpVisionPC1.Open_Socket(UDP_PC1_LPORT, UDP_PC1_HPORT, UDP_PC1_HOST_IP, this);
	BOOL bOpenedPC2 = m_UdpVisionPC2.Open_Socket(UDP_PC2_LPORT, UDP_PC2_HPORT, UDP_PC2_HOST_IP, this);
	BOOL bOpenedPC3 = m_UdpVisionPC3.Open_Socket(UDP_PC3_LPORT, UDP_PC3_HPORT, UDP_PC3_HOST_IP, this);
	BOOL bOpenedPC4 = m_UdpVisionPC4.Open_Socket(UDP_PC4_LPORT, UDP_PC4_HPORT, UDP_PC4_HOST_IP, this);

	if (bOpenedPC1) Set_ConnectRequest(INSPECTOR_PC1);
	if (bOpenedPC2) Set_ConnectRequest(INSPECTOR_PC2);
	if (bOpenedPC3) Set_ConnectRequest(INSPECTOR_PC3);
	if (bOpenedPC4) Set_ConnectRequest(INSPECTOR_PC4);

#endif


}

void CInspector::Terminate()
{
	Set_ConnectEnd(INSPECTOR_PC1);
	Set_ConnectEnd(INSPECTOR_PC2);
	Set_ConnectEnd(INSPECTOR_PC3);
	Set_ConnectEnd(INSPECTOR_PC4);

	m_UdpVisionPC1.Close_Socket();
	m_UdpVisionPC2.Close_Socket();
	m_UdpVisionPC3.Close_Socket();
	m_UdpVisionPC4.Close_Socket();
}

///////////////////////////////////////////////////////////////////////////////
// UDP Socket Message

void CInspector::OnTimer(UINT_PTR nIDEvent)
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

LRESULT CInspector::OnUdpReceive(WPARAM wLocalPort, LPARAM lParam)
{
	UINT nPort = (UINT)wLocalPort;
	int nInspector = 0, nLen = 0;
	BYTE byRecv[1024] = { 0 };
	CString strLog;

#ifdef AJIN_BOARD_USE
	if (nPort == UDP_PC1_HPORT) { nInspector = INSPECTOR_PC1; nLen = m_UdpVisionPC1.Read_Socket(byRecv); }
	if (nPort == UDP_PC2_HPORT) { nInspector = INSPECTOR_PC2; nLen = m_UdpVisionPC2.Read_Socket(byRecv); }
	if (nPort == UDP_PC3_HPORT) { nInspector = INSPECTOR_PC3; nLen = m_UdpVisionPC3.Read_Socket(byRecv); }
	if (nPort == UDP_PC4_HPORT) { nInspector = INSPECTOR_PC4; nLen = m_UdpVisionPC4.Read_Socket(byRecv); }
#else
	if (nPort == 10001) { nInspector = INSPECTOR_PC1; nLen = m_UdpVisionPC1.Read_Socket(byRecv); }
	if (nPort == 11001) { nInspector = INSPECTOR_PC2; nLen = m_UdpVisionPC2.Read_Socket(byRecv); }
	if (nPort == 12001) { nInspector = INSPECTOR_PC3; nLen = m_UdpVisionPC3.Read_Socket(byRecv); }
	if (nPort == 13001) { nInspector = INSPECTOR_PC4; nLen = m_UdpVisionPC4.Read_Socket(byRecv); }
#endif



	if (nInspector == 0 || nLen < 1) {
		strLog.Format("[H<-V%d] : Local Port (%d) Mismatch or Receive Data Zero (%d)", nInspector, nPort, nLen);
		g_objLogFile.Save_InspectorLog(strLog);
		return 0;
	}

	CString strRecvSocket;
	strRecvSocket.Format("%s", byRecv);
	m_strRecvCmd += strRecvSocket;

	while (!m_strRecvCmd.IsEmpty()) 
	{
		int nStart = m_strRecvCmd.Find("@");
		int nEnd = m_strRecvCmd.Find("\n");

		if (nEnd < 0) break;	// 버퍼에 들어오는 중...

		if (nStart < 0 || nStart > nEnd) {
			strLog.Format("[H<-V%d] : <<Error>> %s : Start(%d), End(%d)", nInspector, m_strRecvCmd, nStart, nEnd);
			g_objLogFile.Save_InspectorLog(strLog);
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
			g_objLogFile.Save_InspectorLog(strLog);
		}
		/////////////////////////////////////////////////////////

		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		if (nInspector == INSPECTOR_PC1 && !pEquipData->bUseInspectBtm1Specular && !pEquipData->bUseInspectBtm1Angle && !pEquipData->bUseInspectBtm13D) return 0;
		if (nInspector == INSPECTOR_PC2 && !pEquipData->bUseInspectTop1 && !pEquipData->bUseVisionAlign && !pEquipData->bUseInspectAngle) return 0;
		if (nInspector == INSPECTOR_PC3 && !pEquipData->bUseInspectTop2) return 0;
		if (nInspector == INSPECTOR_PC4 && !pEquipData->bUseInspectBtm2) return 0;

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

		} else if (strCmd == "LOT") {
			if (strOp == "READY") Get_LotReady(nInspector, strArg[0], strArg[1], strArg[2], strArg[3]);

		} else if (strCmd == "RELOAD") {
			if (strOp == "REQUEST") Get_ReloadRequest(nInspector, strArg[0], strArg[1]);

		} else if (strCmd == "SCAN") {
			if (strOp == "COMPLETE") Get_ScanComplete(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4]);

		} else if (strCmd == "INSPECT") {
			if (strOp == "COMPLETE") Get_InspectComplete(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4], strArg[5], strArg[6]);

		} else if (strCmd == "AMOVE") {
			if (strOp == "REQUEST") Get_AMoveRequest(nInspector, strArg[0], strArg[1]);

		} else if (strCmd == "POSITION") {
			if (strOp == "REQUEST") Get_PositionRequest(nInspector, strArg[0]);

		} else if (strCmd == "ALIGN") {
			int nStrCnt = 0;
			CString str[7];
			for (int i = 0; i < 7; i++) {
				AfxExtractSubString(str[i], strRecv, i, chSep);
				nStrCnt += (str[i].GetLength() + 1);
			}
			CString strResult = strRecv.Mid(nStrCnt, strRecv.GetLength() - nStrCnt);
			if (strOp == "COMPLETE") Get_AlignComplete(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4], strResult);
			else if (strOp == "REPLY") gData.bAlignReply = TRUE;

		} else if (strCmd == "SHIFT") {
			if (strOp == "REQUEST") Get_ShiftRequest(nInspector, strArg[0]);

		} else if (strCmd == "SMOVE") {
			if (strOp == "REQUEST") Get_SMoveRequest(nInspector, strArg[0], strArg[1]);

		} else if (strCmd == "BARCODE") {
			if (strOp == "RESULT") Get_BarcodeResult(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4]);

		} else if (strCmd == "TIME") {
			if (strOp == "UPDATE") Get_TimeUpdate(nInspector, strArg[0]);

		} else if (strCmd == "ERROR") {
			if (strOp == "REQUEST") Get_ErrorRequest(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4], strArg[5]);
		
		} else if (strCmd == "ALARM") {
			// int nInspector, CString sGbn, CString sLotId, CString sPortNo, CString sErrNo, CString sInfo1, CString sInfo2, CString sInfo3
			if (strOp == "REQUEST") Get_AlarmRequest(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4], strArg[5], strArg[6]);

		} else if (strCmd == "FOB") {
			if (strOp == "REPLY") Get_FOBReply(nInspector, strArg[0]);
			else if (strOp == "UPDATE") Get_FOBUpdate(nInspector, strArg[0]);

		} else if (strCmd == "APD") {
			if (strOp == "REPLY") Get_APDReply(nInspector, strRecv);
		
		}  else if (strCmd == "HEART") {
			if (strOp == "BEAT") Get_HeartBeat(nInspector);
		} else if (strCmd == "3DSTART"){
			if (strOp =="REQUEST") Get_3DStart(nInspector);
		}
		
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Get Command

void CInspector::Get_ConnectRequest(int nInspector)
{
	if (nInspector == INSPECTOR_PC1) m_bConnectPC1 = TRUE;
	if (nInspector == INSPECTOR_PC2) m_bConnectPC2 = TRUE;
	if (nInspector == INSPECTOR_PC3) m_bConnectPC3 = TRUE;
	if (nInspector == INSPECTOR_PC4) m_bConnectPC4 = TRUE;
	Set_ConnectReply(nInspector);
}

void CInspector::Get_ConnectReply(int nInspector)
{
	if (nInspector == INSPECTOR_PC1) m_bConnectPC1 = TRUE;
	if (nInspector == INSPECTOR_PC2) m_bConnectPC2 = TRUE;
	if (nInspector == INSPECTOR_PC3) m_bConnectPC3 = TRUE;
	if (nInspector == INSPECTOR_PC4) m_bConnectPC4 = TRUE;
}

void CInspector::Get_ConnectEnd(int nInspector)
{
	if (nInspector == INSPECTOR_PC1) m_bConnectPC1 = FALSE;
	if (nInspector == INSPECTOR_PC2) m_bConnectPC2 = FALSE;
	if (nInspector == INSPECTOR_PC3) m_bConnectPC3 = FALSE;
	if (nInspector == INSPECTOR_PC4) m_bConnectPC4 = FALSE;
}

void CInspector::Get_StatusRequest(int nInspector)
{
	BOOL bStatus = g_objSequenceMain.Is_MainThreadRun();
	if (bStatus) Set_StatusReply(nInspector, 1);
	else Set_StatusReply(nInspector, 0);
}

void CInspector::Get_StatusReply(int nInspector, CString sStatus)
{
	if (nInspector == INSPECTOR_PC1) m_nStatusPC1 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC2) m_nStatusPC2 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC3) m_nStatusPC3 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC4) m_nStatusPC4 = atoi(sStatus);
}

void CInspector::Get_StatusUpdate(int nInspector, CString sStatus)
{
	KillTimer(nInspector);
	if (nInspector == INSPECTOR_PC1) m_nStatusPC1 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC2) m_nStatusPC2 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC3) m_nStatusPC3 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC4) m_nStatusPC4 = atoi(sStatus);
	SetTimer(nInspector, 15000, NULL);
}

void CInspector::Get_LotReady(int nInspector, CString sLotId, CString sPortNo, CString sProgVer, CString sParaVer)
{
	int nPortNo = atoi(sPortNo);
	if (sLotId != gData.sLotID[nPortNo - 1]) return;
	if (nInspector == INSPECTOR_PC1) m_bLotReady1 = TRUE;
	if (nInspector == INSPECTOR_PC2) m_bLotReady2 = TRUE;
	if (nInspector == INSPECTOR_PC3) m_bLotReady3 = TRUE;
	if (nInspector == INSPECTOR_PC4) m_bLotReady4 = TRUE;

	CString strLog;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if ((pEquipData->sVisionProgVer == "0000") || (pEquipData->sVisionParaVer == "0000")) return;
	if (sProgVer != pEquipData->sVisionProgVer || sParaVer != pEquipData->sVisionParaVer) {
		strLog.Format("Vision PC %d 의 프로그램 버전 또는 파라메터 버전을 확인하여 주십시오.", nInspector);
		g_objLogFile.Save_InspectorLog(strLog);

		gData.bVisionVerErr = TRUE; 
		m_bLotReady1 = FALSE; m_bLotReady2 = FALSE; m_bLotReady3 = FALSE; m_bLotReady4 = FALSE;

		// 비전 프로그램 완료되면 아래 주석 풀것. 2020.08.27
		if (nInspector == INSPECTOR_PC1) g_objCommon.Show_Error(6141);
		if (nInspector == INSPECTOR_PC2) g_objCommon.Show_Error(6142);
		if (nInspector == INSPECTOR_PC3) g_objCommon.Show_Error(6143);
		if (nInspector == INSPECTOR_PC4) g_objCommon.Show_Error(6144);
	}

	Set_LotRdyCom(nInspector, sLotId);
}

void CInspector::Get_ReloadRequest(int nInspector, CString sPc, CString sVision)
{
	// bScanDone[6] ==> 0:Angle, 1:Btm1_SP, 2:Top1, 3:Top2, 4:Btm2 5:Btm1_AG
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (nInspector == INSPECTOR_PC1) 
	{			
		if (sPc != "PC1") return;
		Set_ReloadComplete(INSPECTOR_PC1, sPc);
		
		if (pEquipData->bUseInspectBtm1Specular && !gData.bScanDone[1]) {	// Btm1_SP
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM1);
			if (nCase >= 10 && nCase < 15) { gData.bReload[1] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 15); }
		}

		if (pEquipData->bUseInspectBtm1Angle && !gData.bScanDone[5]) {	// Btm1_AG
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM1);
			if (nCase >= 30 && nCase < 35) { gData.bReload[5] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 35); }
		}

		if (pEquipData->bUseInspectBtm13D && !gData.bScanDone[6]) {	// Btm1_3D
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM1);
			if (nCase >= 42 && nCase < 46) { gData.bReload[6] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 40); }
		}

		int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER1);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(1, gData.nPNoSortPick[0], gData.nTNoSortPick[0][i], gData.nCNoSortPick[0][i]); }
		}

		nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER2);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(1, gData.nPNoSortPick[1], gData.nTNoSortPick[1][i], gData.nCNoSortPick[1][i]); }
		}		

	} else if (nInspector == INSPECTOR_PC2) // Top1
	{	
		if (sPc != "PC2") return;
		Set_ReloadComplete(INSPECTOR_PC2, sPc);
		
		if (pEquipData->bUseVisionAlign && sVision == "ALIGN") 
		{	// Align
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_ANGLE);
			if (nCase == 5) g_objSequenceMain.Set_MainRunCase(AUTO_VISION_ANGLE, 4);
		}

		if (pEquipData->bUseInspectAngle && !gData.bScanDone[0] && sVision == "AG") {	// Angle
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_ANGLE);
			if (nCase == 15 && nCase < 19) g_objSequenceMain.Set_MainRunCase(AUTO_VISION_ANGLE, 13);
		}

		if (pEquipData->bUseInspectTop1 && !gData.bScanDone[2] && sVision == "T1") 
		{	// Top1
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			if (nCase >= 10 && nCase < 15) { gData.bReload[2] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION1, 15); }

			nCase = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			if (nCase >= 10 && nCase < 15) { gData.bReload[2] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION2, 15); }

			nCase = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase >= 10 && nCase < 15) { gData.bReload[2] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION3, 15); }
		}

		int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER1);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(2, gData.nPNoSortPick[0], gData.nTNoSortPick[0][i], gData.nCNoSortPick[0][i]); }
		}

		nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER2);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(2, gData.nPNoSortPick[1], gData.nTNoSortPick[1][i], gData.nCNoSortPick[1][i]); }
		}

	} else if (nInspector == INSPECTOR_PC3) {	// Top2
		if (sPc != "PC3") return;
		Set_ReloadComplete(INSPECTOR_PC3, sPc);

		if (pEquipData->bUseInspectTop2 && !gData.bScanDone[3]) {	// Top2
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			if (nCase >= 20 && nCase < 25) { gData.bReload[3] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION1, 25); }

			nCase = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			if (nCase >= 20 && nCase < 25) { gData.bReload[3] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION2, 25); }

			nCase = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase >= 20 && nCase < 25) { gData.bReload[3] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION3, 25); }
		}

		int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER1);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(3, gData.nPNoSortPick[0], gData.nTNoSortPick[0][i], gData.nCNoSortPick[0][i]); }
		}

		nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER2);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(3, gData.nPNoSortPick[1], gData.nTNoSortPick[1][i], gData.nCNoSortPick[1][i]); }
		}

	} else if (nInspector == INSPECTOR_PC4) {	// BTM2
		if (sPc != "PC4") return;
		Set_ReloadComplete(INSPECTOR_PC4, sPc);

		if (pEquipData->bUseInspectAngle && !gData.bScanDone[4]) {	// BTM2
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM2);
			if (nCase >= 10 && nCase < 14) { gData.bReload[4] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM2, 15); }
		}

		int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER1);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(4, gData.nPNoSortPick[0], gData.nTNoSortPick[0][i], gData.nCNoSortPick[0][i]); }
		}

		nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER2);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(4, gData.nPNoSortPick[1], gData.nTNoSortPick[1][i], gData.nCNoSortPick[1][i]); }
		}
	}
}

void CInspector::Get_ScanComplete(int nInspector, CString sGbn, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo)
{
	int nPx = atoi(sPortNo) - 1;
	int	nTx = atoi(sTrayNo) - 1;
	int	nCx = atoi(sCmNo) - 1;
	if (nPx < 0 || nPx > 1 || nTx < 0 || nTx > 29 || nCx < 0 || nCx > 39) { g_objCommon.Show_Error(6101); return; }

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	// 0:Angle, 1:Btm1_SP, 2:Top1, 3:Top2, 4:Btm2, 5:Btm1_AG, 6:Btm1_3D 
	if (sGbn == "AG") {
		int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_ANGLE);
		if (nCase != 15) { Exception_Log("Scan Complete", sGbn, nCase); return; }
		gData.bScanDone[0] = TRUE;
		int nY = gData.nTrayY - (nCx / gData.nTrayX) - 1;	// 하단부터 적재 해준다.
		int nX = nCx % gData.nTrayX;
		if (gData.InfoAnglePortTray[nY][nX] == 9) gData.InfoAnglePortTray[nY][nX] = 1;	//Scan Done
		if (gData.InfoAnglePortTray[nY][nX] == 0) gData.InfoAnglePortTray[nY][nX] = 2;	//Scan Done
		g_objSequenceMain.Set_MainRunCase(AUTO_VISION_ANGLE, 19);

	} else if (sGbn == "B1SP") {
		int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM1);
		if (nCase != 10) { Exception_Log("Scan Complete", sGbn, nCase); return; }
		m_nB1_SPScanCnt++;
		if (m_nB1_SPScanCnt < m_nB1_SPScanReq) return;
		gData.bScanDone[1] = TRUE; 
		g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 15);
	
	} else if (sGbn == "B1AG") {
		int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM1);
		if (nCase != 30) { Exception_Log("Scan Complete", sGbn, nCase); return; }
		m_nB1_AGScanCnt++;
		if (m_nB1_AGScanCnt < m_nB1_AGScanReq) return;
		gData.bScanDone[5] = TRUE;
		g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 35);

	} else if (sGbn == "B13D"){
		/*int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM1);
		m_nB1_3DScanCnt++;
		if (m_nB1_3DScanCnt < m_nB1_3DScanReq) return;
		if (nCase != 42 && nCase != 45) { Exception_Log("Scan Complete", sGbn, nCase); return; }
		gData.bScanDone[6] = TRUE;
		g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 47);*/

	} else if (sGbn == "T1") {
		int nCase = 0;
		int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
		int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
		int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
		
		if (nCase1 == 10) nCase = 1;
		if (nCase2 == 10) nCase = 2;
		if (nCase3 == 10) nCase = 3;
		if (nCase == 0) { Exception_Log("Scan Complete", sGbn, nCase); return; }
		m_nT1ScanCnt++;
		if (m_nT1ScanCnt < m_nT1ScanReq) return;
		gData.bScanDone[2] = TRUE;

		if (nCase == 1) {
			//g_objLogFile.VisionTakt_Save(AUTO_INSPECTION1, 22, TOP1); 
		g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION1, 15); }
		if (nCase == 2) { 
			//g_objLogFile.VisionTakt_Save(AUTO_INSPECTION2, 22, TOP1); 
			g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION2, 15); }
		if (nCase == 3) { 
			//g_objLogFile.VisionTakt_Save(AUTO_INSPECTION3, 22, TOP1); 
			g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION3, 15); }

 	} else if (sGbn == "T2") {
		int nCase = 0;
		int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
		int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
		int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);

		if (nCase1 == 20) nCase = 1;
		if (nCase2 == 20) nCase = 2;
		if (nCase3 == 20) nCase = 3;
		if (nCase == 0) { Exception_Log("Scan Complete", sGbn, nCase); return; }
		m_nT2ScanCnt++;
		if (m_nT2ScanCnt < m_nT2ScanReq) return;
		gData.bScanDone[3] = TRUE;

		if (nCase == 1) { 
			//g_objLogFile.VisionTakt_Save(AUTO_INSPECTION1, 24, TOP2);
			g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION1, 25); }
		if (nCase == 2) { 
			//g_objLogFile.VisionTakt_Save(AUTO_INSPECTION2, 24, TOP2); 
			g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION2, 25); }
		if (nCase == 3) { 
			//g_objLogFile.VisionTakt_Save(AUTO_INSPECTION3, 24, TOP2);
			g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION3, 25); }

	} else if (sGbn == "B2") {
		int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM2);
		if (nCase != 10) { Exception_Log("Scan Complete", sGbn, nCase); return; }
		m_nB2ScanCnt++;
		if (m_nB2ScanCnt < m_nB2ScanReq) return;
		gData.bScanDone[4] = TRUE;
		//g_objLogFile.VisionTakt_Save(AUTO_VISION_BTM2, 21, BTM2);
		g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM2, 15);

	} else {
		g_objCommon.Show_Error(6102);
	}
}

void CInspector::Get_InspectComplete(int nInspector, CString sGbn, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sJudge, CString sNGCode)
{
	int nPx = atoi(sPortNo) - 1;
	int	nTx = atoi(sTrayNo) - 1;
	int	nCx = atoi(sCmNo) - 1;
	if (nPx < 0 || nPx > 1 || nTx < 0 || nTx > 29 || nCx < 0 || nCx > 39) { g_objCommon.Show_Error(6101); return; }

	// byInspectDone[2][30][40] : 검사완료 (5bit 사용 ==> 0:Angle, 1:Btm1, 2:Top1, 3:Top2, 4:Btm2)
	int nV = (sGbn == "AG" ? 0 : (sGbn == "B1SP" ? 1 : (sGbn == "T1" ? 2 : (sGbn == "T2" ? 3 : (sGbn == "B2" ? 4 :(sGbn == "B1AG" ? 5 : (sGbn == "B13D" ? 6 : -1)) )))));
	if (nV == -1) { g_objCommon.Show_Error(6102); return; }

	if(nPx==0){
		gData.cJudgeCode1[nTx][nCx][nV] = *(LPSTR)(LPCTSTR)sJudge;
	}
	if(nPx==1){
		gData.cJudgeCode2[nTx][nCx][nV] = *(LPSTR)(LPCTSTR)sJudge;
	}

	

	if (sJudge != "G" && sNGCode.GetLength() < 2) sNGCode = "NON";	// Good 일때 NG Code는 Space(" ")

	if (sJudge == "N1" || sJudge == "N2" || sJudge == "N3" || sJudge == "N4") {
		gMes.sJudge[nPx][nTx][nCx] = sJudge;
		gMes.sNGCode[nPx][nTx][nCx] = gData.sNGData[nPx][nTx][nCx][nV] = sNGCode;

		// 지그별 NG 수량
		int nJigNo, nStageNo;
		if (nV == 1) {	// BTM1_SP
			nJigNo = gData.nCmJigNo[nPx][nTx][nCx][BTM1_PICK] - 1;
			gData.nJigNgCnt[nPx][0][nJigNo]++;

		} else if (nV == 5){ //BTM1_AG
			nJigNo = gData.nCmJigNo[nPx][nTx][nCx][BTM1_PICK] - 1;
			gData.nJigNgCnt[nPx][0][nJigNo]++;
		
		} else if (nV == 6){ // BTM1_3D
			nJigNo = gData.nCmJigNo[nPx][nTx][nCx][BTM1_PICK] - 1;
			gData.nJigNgCnt[nPx][0][nJigNo]++;

		} else if (nV == 2) {	// TOP1 (1, 2, 3)
			nJigNo   = gData.nCmJigNo[nPx][nTx][nCx][INSP_JIG_NO] - 1;
			nStageNo = gData.nCmJigNo[nPx][nTx][nCx][INSP_STAGE_NO];
			gData.nJigNgCnt[nPx][nStageNo][nJigNo]++;

		} else if (nV == 3) {	// TOP2 (4, 5, 6)
			nJigNo   = gData.nCmJigNo[nPx][nTx][nCx][INSP_JIG_NO] - 1;
			nStageNo = gData.nCmJigNo[nPx][nTx][nCx][INSP_STAGE_NO] + 3;
			gData.nJigNgCnt[nPx][nStageNo][nJigNo]++;

		} else if (nV == 4) {	// BTM2
			nJigNo = gData.nCmJigNo[nPx][nTx][nCx][BTM2_PICK] - 1;
			gData.nJigNgCnt[nPx][7][nJigNo]++;
		}
	}

	int nMode = theApp.Get_MainMode();
	int nPreInfo = gData.nInspectInfo[nPx][nTx][nCx];
// 	if (nPreInfo == 0) {
// 		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) { g_objCommon.Show_Error(6103); return; }
// 	}

	// 우선순위 : N4(8) -> B(7) -> N3(6) -> N2(5) -> N1(4)
	if		(sJudge == "N4") { if (nPreInfo < 8 || nPreInfo > 8) gData.nInspectInfo[nPx][nTx][nCx] = 8; }	// N4
	else if	(sJudge == "B")  { if (nPreInfo < 7 || nPreInfo > 8) gData.nInspectInfo[nPx][nTx][nCx] = 7; }	// BS
	else if	(sJudge == "N3") { if (nPreInfo < 6 || nPreInfo > 8) gData.nInspectInfo[nPx][nTx][nCx] = 6; }	// N3
	else if	(sJudge == "N2") { if (nPreInfo < 5 || nPreInfo > 8) gData.nInspectInfo[nPx][nTx][nCx] = 5; }	// N2
	else if (sJudge == "N1") { if (nPreInfo < 4 || nPreInfo > 8) gData.nInspectInfo[nPx][nTx][nCx] = 4; }	// N1
	else if (sJudge == "G")	 
	{ 
		if (nPreInfo < 2 || nPreInfo > 8) gData.nInspectInfo[nPx][nTx][nCx] = 9; 
		if ( gMes.sNGCode[nPx][nTx][nCx] == "MC") gData.nInspectInfo[nPx][nTx][nCx] = 9; 
	}	// Good
	else if (sJudge != "G")  
	{ 
		if (nPreInfo < 2 || nPreInfo > 8) gData.nInspectInfo[nPx][nTx][nCx] = 2;  // Normal (20180831 유출 때문에 수정.)
		//if ( gMes.sNGCode[nPx][nTx][nCx] == "MC") gData.nInspectInfo[nPx][nTx][nCx] = 9; 
	}	

	if (sJudge == "B") gLot.nBsNgCount[nPx]++;	// Normal (Barcode Shift도 8로 분류해서 색상다르게 1차로 빼준다.)

	gData.byInspectDone[nPx][nTx][nCx] |= (1 << nV);

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseDispatcher) return;

	if (pEquipData->bUseInspectAngle && ((gData.byInspectDone[nPx][nTx][nCx] >> 0) & 1) == 0) return;	// Angle
	if (pEquipData->bUseInspectBtm1Specular  && ((gData.byInspectDone[nPx][nTx][nCx] >> 1) & 1) == 0) return;	// Btm1_Specular
	if (pEquipData->bUseInspectTop1  && ((gData.byInspectDone[nPx][nTx][nCx] >> 2) & 1) == 0) return;	// Top1
	if (pEquipData->bUseInspectTop2  && ((gData.byInspectDone[nPx][nTx][nCx] >> 3) & 1) == 0) return;	// Top2
	if (pEquipData->bUseInspectBtm2  && ((gData.byInspectDone[nPx][nTx][nCx] >> 4) & 1) == 0) return;	// Btm2
	if (pEquipData->bUseInspectBtm1Angle  && ((gData.byInspectDone[nPx][nTx][nCx] >> 5) & 1) == 0) return;	// Btm1_Angle
	if (pEquipData->bUseInspectBtm13D  && ((gData.byInspectDone[nPx][nTx][nCx] >> 6) & 1) == 0) return;	// Btm1_3D

	if (gData.nInspectInfo[nPx][nTx][nCx] == 4 || gData.nInspectInfo[nPx][nTx][nCx] == 5) {
		g_objDispatcher.Set_JudgeRequest(nPx+1, nTx+1, nCx+1);	// N1(4), N2(5)
	} else {
		g_objDispatcher.Set_JudgeDone(nPx+1, nTx+1, nCx+1);	// ROS 판정 완료
	}
}

void CInspector::Get_AMoveRequest(int nInspector, CString sGbn, CString sZ)
{
	double dZ = atof(sZ);
	CString strLog = "";

	int nMode = theApp.Get_MainMode();
	if (sGbn == "B1SP") {
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM1);
			if (nCase != 10) { Exception_Log("AMove Request", sGbn, nCase); return; }
			g_objSequenceMain.m_dBtm1Z = dZ;
			g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 11);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z) || dZ < 0.0) return;
			g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_Z, dZ);
			DWORD dwStart = GetTickCount();
			while (!g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_Z, dZ)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_MoveComplete(nInspector, sGbn);
		}
	} else if (sGbn == "B1AG"){
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM1);
			if (nCase != 30) { Exception_Log("AMove Request", sGbn, nCase); return; }
			g_objSequenceMain.m_dBtm1Z = dZ;
			g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 31);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z) || dZ < 0.0) return;
			g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_Z, dZ);
			DWORD dwStart = GetTickCount();
			while (!g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_Z, dZ)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_MoveComplete(nInspector, sGbn);
		}
	}else if (sGbn == "B13D"){
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM1);
			if (nCase != 42) { Exception_Log("AMove Request", sGbn, nCase); return; }
			g_objSequenceMain.m_dBtm1Z = dZ;
			g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 43);
		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z) || dZ < 0.0) return;
			g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_Z, dZ);
			DWORD dwStart = GetTickCount();
			while (!g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_Z, dZ)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_MoveComplete(nInspector, sGbn);
		}
	} else if (sGbn == "T1") {
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase1 != 10 && nCase2 != 10 && nCase3 != 10) { Exception_Log("AMove Request", sGbn, nCase1); return; }
			g_objSequenceMain.m_dTop1Z = dZ;
			int nAutoRun = (nCase3 == 10 ? AUTO_INSPECTION3 : (nCase2 == 10 ? AUTO_INSPECTION2 : AUTO_INSPECTION1));
			g_objSequenceMain.Set_MainRunCase(nAutoRun, 11);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_VISION_TOP1_Z) || dZ < 0.0) return;
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, dZ);
			DWORD dwStart = GetTickCount();
			while (!g_objAJinAXL.Is_MoveDone(AX_VISION_TOP1_Z, dZ)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_MoveComplete(nInspector, sGbn);
		}

	} else if (sGbn == "T2" || sGbn == "T3") {
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase1 != 20 && nCase2 != 20 && nCase3 != 20) { Exception_Log("AMove Request", sGbn, nCase1); return; }
			g_objSequenceMain.m_dTop2Z = dZ;
			int nAutoRun = (nCase3 == 20 ? AUTO_INSPECTION3 : (nCase2 == 20 ? AUTO_INSPECTION2 : AUTO_INSPECTION1));
			g_objSequenceMain.Set_MainRunCase(nAutoRun, 21);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_VISION_TOP2_Z) || dZ < 0.0) return;
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, dZ);
			DWORD dwStart = GetTickCount();
			while (!g_objAJinAXL.Is_MoveDone(AX_VISION_TOP2_Z, dZ)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_MoveComplete(nInspector, sGbn);
		}

	} else if (sGbn == "B2" || sGbn == "B3") {
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM2);
			if (nCase != 10) { Exception_Log("AMove Request", sGbn, nCase); return; }
			g_objSequenceMain.m_dBtm2Z = dZ;
			g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM2, 11);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_Z) || dZ < 0.0) return;
			g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_Z, dZ);
			DWORD dwStart = GetTickCount();
			while (!g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_Z, dZ)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_MoveComplete(nInspector, sGbn);
		}
	} else {
		g_objCommon.Show_Error(6102);
	}
}

void CInspector::Get_PositionRequest(int nInspector, CString sGbn)
{
	double dZ = 0.0;

	if (sGbn=="AG") { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_VISION_ANGLE_Z); dZ = pStatusZ->dPos; }
	if (sGbn=="B1SP") { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_BTM1_PICKER_Z); dZ = pStatusZ->dPos; }
	if (sGbn=="B1AG") { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_BTM1_PICKER_Z); dZ = pStatusZ->dPos; }
	if (sGbn=="T1") { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_VISION_TOP1_Z); dZ = pStatusZ->dPos; }
	if (sGbn=="T2" || sGbn=="T3") { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_VISION_TOP2_Z); dZ = pStatusZ->dPos; }
	if (sGbn=="B2" || sGbn=="B3") { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_BTM2_PICKER_Z); dZ = pStatusZ->dPos; }

	Set_PositionReply(nInspector, sGbn, dZ);
}

void CInspector::Get_AlignComplete(int nInspector, CString sLotId, CString sPortNo, CString sTrayNo, CString sSeqNo, CString sCmCnt, CString sResult)
{
	int	nTrayNo = atoi(sTrayNo);
	int nCmCount = atoi(sCmCnt);
	if (nTrayNo < 1 || nTrayNo > 40 || nCmCount < 0 || nCmCount > 32) { g_objCommon.Show_Error(6101); return; }

	CString strTemp;
	char chSep = ',';
	for (int i = 0; i < nCmCount; i++) {
		AfxExtractSubString(strTemp, sResult, i * 3 + 0, chSep); m_dAlignX[i] = atof(strTemp);
		AfxExtractSubString(strTemp, sResult, i * 3 + 1, chSep); m_dAlignY[i] = atof(strTemp);
		AfxExtractSubString(strTemp, sResult, i * 3 + 2, chSep); m_dAlignQ[i] = atof(strTemp);
	}

	int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_ANGLE);
	if (nCase != 5) { Exception_Log("Align Complete", "AL", nCase); return; }
	g_objSequenceMain.Set_MainRunCase(AUTO_VISION_ANGLE, 8);
}

void CInspector::Get_ShiftRequest(int nInspector, CString sGbn)
{
	CString strLog = "";

	int nMode = theApp.Get_MainMode();

	 if (sGbn == "T1") {		// Top1 Mirror Z축
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase1 != 10 && nCase2 != 10 && nCase3 != 10) { Exception_Log("AMove Request", sGbn, nCase1); return; }
			int nAutoRun = (nCase3 == 10 ? AUTO_INSPECTION3 : (nCase2 == 10 ? AUTO_INSPECTION2 : AUTO_INSPECTION1));
			g_objSequenceMain.Set_MainRunCase(nAutoRun, 13);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_TOP1_MIRROR_Z)) return;
			g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 1);
			DWORD dwStart = GetTickCount();
			while (!g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 1)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_ShiftComplete(nInspector, sGbn);
		}

	} else if (sGbn == "T2") {		// Top2 Shift Y축
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase1 != 20 && nCase2 != 20 && nCase3 != 20) { Exception_Log("AMove Request", sGbn, nCase1); return; }
			int nAutoRun = (nCase3 == 20 ? AUTO_INSPECTION3 : (nCase2 == 20 ? AUTO_INSPECTION2 : AUTO_INSPECTION1));
			g_objSequenceMain.Set_MainRunCase(nAutoRun, 23);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_TOP2_SHIFT_Y)) return;
			g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 1);
			DWORD dwStart = GetTickCount();
			while (!g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 1)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_ShiftComplete(nInspector, sGbn);
		}

	} else {
		g_objCommon.Show_Error(6102);
	}
}

void CInspector::Get_SMoveRequest(int nInspector, CString sGbn, CString sZ)
{
	double dZ = atof(sZ);
	CString strLog = "";

	int nMode = theApp.Get_MainMode();

	if (sGbn == "B1AG") {
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM1);
			if (nCase != 30) { Exception_Log("SMove Request", sGbn, nCase); return; }
			g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 33);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_BTM1_SHIFT_Y) || !g_objCommon.Check_Position(AX_BTM1_SHIFT_Y, 1)) return;
			if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z) ) return;

			g_objCommon.Move_Position(AX_BTM1_SHIFT_Y, 1);
			Set_SMoveComplete(nInspector, sGbn);
		}
	}
	else if (sGbn == "T1") {
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase1 != 10 && nCase2 != 10 && nCase3 != 10) { Exception_Log("SMove Request", sGbn, nCase1); return; }
			g_objSequenceMain.m_dTop1Z = dZ;
			int nAutoRun = (nCase3 == 10 ? AUTO_INSPECTION3 : (nCase2 == 10 ? AUTO_INSPECTION2 : AUTO_INSPECTION1));
			g_objSequenceMain.Set_MainRunCase(nAutoRun, 13);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_TOP1_MIRROR_Z)) return;
			if (!g_objAJinAXL.Is_Home(AX_VISION_TOP1_Z) || dZ < 0.0) return;

			g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 1);
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, dZ);
			DWORD dwStart = GetTickCount();
			while (!g_objAJinAXL.Is_MoveDone(AX_VISION_TOP1_Z, dZ) || !g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 1)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_SMoveComplete(nInspector, sGbn);
		}

	} else if (sGbn == "T2" || sGbn == "T3") {
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase1 != 20 && nCase2 != 20 && nCase3 != 20) { Exception_Log("SMove Request", sGbn, nCase1); return; }
			g_objSequenceMain.m_dTop2Z = dZ;
			int nAutoRun = (nCase3 == 20 ? AUTO_INSPECTION3 : (nCase2 == 20 ? AUTO_INSPECTION2 : AUTO_INSPECTION1));
			g_objSequenceMain.Set_MainRunCase(nAutoRun, 23);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_TOP2_SHIFT_Y)) return;
			if (!g_objAJinAXL.Is_Home(AX_VISION_TOP2_Z) || dZ < 0.0) return;

			g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 1);
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, dZ);
			DWORD dwStart = GetTickCount();
			while (!g_objAJinAXL.Is_MoveDone(AX_VISION_TOP2_Z, dZ) || !g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 1)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_SMoveComplete(nInspector, sGbn);
		}

	} else {
		g_objCommon.Show_Error(6102);
	}
}

void CInspector::Get_BarcodeResult(int nInspector, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sBarcode)
{
	int nPx = atoi(sPortNo) - 1;
	int	nTx = atoi(sTrayNo) - 1;
	int	nCx = atoi(sCmNo) - 1;
	if (nPx < 0 || nPx > 1 || nTx < 0 || nTx > 29 || nCx < 0 || nCx > 39) { g_objCommon.Show_Error(6101); return; }

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	gMes.sBarID[nPx][nTx][nCx] = sBarcode;
	Set_BarcodeUpdate(INSPECTOR_PC2, sLotId, nPx+1, nTx+1, nCx+1, sBarcode);
	Set_BarcodeUpdate(INSPECTOR_PC3, sLotId, nPx+1, nTx+1, nCx+1, sBarcode);
	Set_BarcodeUpdate(INSPECTOR_PC4, sLotId, nPx+1, nTx+1, nCx+1, sBarcode);
//	g_objMesAgent.Set_CmRequest(nTx+1, nCx+1, gLot.sLotID[nPx], sBarcode);

	if (sBarcode == "NOREAD"/* && atoi(sMainNo) != 0 */) {
		gData.nInspectInfo[nPx][nTx][nCx] = 4;	// N1
		
		CString strLog;
		strLog.Format("[Barcode NoRead] LotId(%s), PortNo(%d), TrayNo(%d), CmNo(%d)", gLot.sLotID[nPx], nPx+1, nTx+1, nCx+1);
		g_objLogFile.Save_HandlerLog(strLog);
	}
}

void CInspector::Get_ErrorRequest(int nInspector, CString sGbn, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sErrNo)
{
	int nPx = atoi(sPortNo) - 1;
	int	nTx = atoi(sTrayNo) - 1;
	int	nCx = atoi(sCmNo) - 1;
	if (nPx < 0 || nPx > 1 || nTx < 0 || nTx > 29 || nCx < 0 || nCx > 39) { g_objCommon.Show_Error(6101); return; }

	int nErrNo = atoi(sErrNo);	// 6120 : Mirror Z Axis Error, 6130: Btm2 Picker Module Error

	CString strErrMsg;
	strErrMsg.Format(" #==> Vision:%s. LotID:%s. PortNo:%d. Tray:%d. CM:%d", sGbn, sLotId, nPx+1, nTx+1, nCx+1);

	gData.sErrVisionLotID = sLotId;
	if (nErrNo == 6150) g_objCommon.Set_ErrorSubMessage("");
	else				g_objCommon.Set_ErrorSubMessage(strErrMsg);

	int nMode = theApp.Get_MainMode();
	if (nErrNo != 6120 && nErrNo != 6121 && (nMode == MODE_WORK || nMode == MODE_OPERATOR)) g_objCommon.Show_Error(nErrNo);

	if (nErrNo == 6120 || nErrNo == 6121) {	// Mirror Z Axis Error
		gData.nTop1MirrorErrNo = nErrNo;
		int nInsp1Case = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
		int nInsp2Case = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
		int nInsp3Case = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
		if (nInsp1Case == 10) { gData.bTop1MirrorErr[0] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION1, 8); }
		if (nInsp2Case == 10) { gData.bTop1MirrorErr[1] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION2, 8);	}
		if (nInsp3Case == 10) { gData.bTop1MirrorErr[2] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION3, 8);	}
	}
	//HDD 용량 확인 후 85% 차면, 알람 번호에 맞는 플래그 True
	if(nErrNo == 6301){m_bHddVision[0][1] = TRUE;}
	if(nErrNo == 6302){m_bHddVision[1][1] = TRUE;}
	if(nErrNo == 6303){m_bHddVision[2][1] = TRUE;}
	if(nErrNo == 6304){m_bHddVision[3][1] = TRUE;}

	//3D Scanner Grab Fail 알람
	if(nErrNo == 6200){gData.b3DGrabFailErr = TRUE;}
}

void CInspector::Get_AlarmRequest(int nInspector, CString sGbn, CString sLotId, CString sPortNo, CString sErrNo, CString sInfo1, CString sInfo2, CString sInfo3)
{
	int nPNo = atoi(sPortNo);
	if (nPNo < 1 || nPNo > 2) { g_objCommon.Show_Error(6101); return; }
	CString strErrMsg;

	int nErrNo = atoi(sErrNo);

	if (nErrNo == 6150) {
		// 6150 : 다발불량
		// sInfo1: NG Code, sInfo2: NG Count
		int nNGCnt = atoi(sInfo2);
		strErrMsg.Format(" #==> Vision:%s. LotID:%s. NG Code:%s. NG Count:%d. Port No:%d", sGbn, sLotId, sInfo1, nNGCnt, nPNo);

		gData.sErrVisionLotID = sLotId;
		g_objCommon.Set_ErrorSubMessage(strErrMsg);

		int nMode = theApp.Get_MainMode();
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) g_objCommon.Show_Error(nErrNo);

	} else if (nErrNo == 6160){
		// Alarm 처리 하려했으나 아직 필요없어 정보 받기만 한다.
		// Barcode Shift 불량
		// sInfo1: X, sInfo2: Y, sInfo3: R
		double dShiftX = atof(sInfo1);
		double dShiftY = atof(sInfo2);
		double dShiftR = atof(sInfo3);
		strErrMsg.Format(" #==> Vision:%s. LotID:%s. X: %0.3lf Y: %0.3lf R: %0.3lf Port No:%d", sGbn, sLotId, dShiftX, dShiftY, dShiftR, nPNo);
	}
}

void CInspector::Get_FOBUpdate(int nInspector, CString sStatus)
{
	int nMode = atoi(sStatus);

	// 기존 모드와 다를 경우에만 Request로 보내준다.
	// -1이면 Request 후 대기중인 상태
	if (gData.nVisionFOBMode != nMode && gData.nVisionFOBMode != -1) {
		Set_FOBRequest(INSPECTOR_ALL);
	}
}

void CInspector::Get_FOBReply(int nInspector, CString sStatus)
{
	m_nFOBModeCnt++;
	if (nInspector == INSPECTOR_PC1) m_nFOBMode[0] = atoi(sStatus);	// 1:FOB Mode On
	if (nInspector == INSPECTOR_PC2) m_nFOBMode[1] = atoi(sStatus);
	if (nInspector == INSPECTOR_PC3) m_nFOBMode[2] = atoi(sStatus);
	if (nInspector == INSPECTOR_PC4) m_nFOBMode[3] = atoi(sStatus);
	if (m_nFOBModeCnt < 4) return;

	if (m_nFOBMode[0] == 1 || m_nFOBMode[1] == 1 || m_nFOBMode[2] == 1 || m_nFOBMode[3] == 1) {
		gData.nVisionFOBMode = 1;

	} else { gData.nVisionFOBMode = 0; }

	if (gData.nVisionFOBMode) {
		int nMode = theApp.Get_MainMode();
		if (nMode == MODE_WORK && g_objSequenceMain.Get_IsAutoRun()) g_objCommon.Show_Error(6170);
	}
}

void CInspector::Get_APDReply(int nInspector, CString strRecv)
{
	char chSep = ',';
	CString strGbn, strLotId, strPortNo, strCount;
	CString strGV[40];

	//@APD,REPLY,I,TESTLOT,0,4,111.10,222.20,33.30,44.40
	AfxExtractSubString(strGbn, strRecv, 2, chSep);
	AfxExtractSubString(strLotId, strRecv, 3, chSep);
	AfxExtractSubString(strPortNo, strRecv, 4, chSep);
	AfxExtractSubString(strCount, strRecv, 5, chSep);	

	int nGbn = (strGbn == "AG" ? 0 : (strGbn == "B1" ? 1 : (strGbn == "T1" ? 2 : (strGbn == "T2" ? 3 : (strGbn == "B2" ? 4 : -1)))));
	int nPNo = atoi(strPortNo) - 1;
	int nCount = atoi(strCount);	// strCount는 이미지 GV값 개수
	gData.nAviApdCnt[nGbn] = nCount;
	gData.bAviApdReceive[nGbn] = TRUE;

	for (int i = 0; i < nCount; i++) {
		AfxExtractSubString(strGV[i], strRecv, 6 + i, chSep);
		gData.sVisionGV[nGbn][i] = strGV[i];
	}
}

void CInspector::Get_TimeUpdate(int nInspector, CString sTime)
{
	// sTime Format : "2000-01-01 12:30:30 000"
	CString strTemp;
	SYSTEMTIME time;

	strTemp = sTime.Mid(0, 4);  time.wYear = atoi(strTemp);			// Year
	strTemp = sTime.Mid(5, 2);  time.wMonth = atoi(strTemp);		// Month
	strTemp = sTime.Mid(8, 2);  time.wDay = atoi(strTemp);			// Day
	strTemp = sTime.Mid(11, 2); time.wHour = atoi(strTemp);			// Hour
	strTemp = sTime.Mid(14, 2); time.wMinute = atoi(strTemp);		// Minute
	strTemp = sTime.Mid(17, 2); time.wSecond = atoi(strTemp);		// Second
	strTemp = sTime.Mid(20, 3); time.wMilliseconds = atoi(strTemp);	// MilliSecond

	SetLocalTime(&time);
}


void CInspector::Get_HeartBeat(int nInspector)
{
	KillTimer(nInspector);
	if (nInspector == INSPECTOR_PC1) m_nStatusPC1 = 1;
	if (nInspector == INSPECTOR_PC2) m_nStatusPC2 = 1;
	if (nInspector == INSPECTOR_PC3) m_nStatusPC3 = 1;
	if (nInspector == INSPECTOR_PC4) m_nStatusPC4 = 1;
	SetTimer(nInspector, 15000, NULL);
}

void CInspector::Get_3DStart(int nInspector)
{
	gData.b3DStart = TRUE;
	g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 45);
}

void CInspector::Exception_Log(CString sFunc, CString sGbn, int nCase)
{
	CString strLog;
	strLog.Format("%s Exception ==> Vision:%s, Case:%d", sFunc, sGbn, nCase);
	g_objLogFile.Save_InspectorLog(strLog);
}

/////////////////////////////////////////////////////////////////////////////
// Set Command

void CInspector::Set_ConnectRequest(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("CONNECT,REQUEST");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_ConnectReply(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("CONNECT,REPLY");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_ConnectEnd(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("CONNECT,END");
	Send_Command(nInspector, strSendCmd);

	if (nInspector == INSPECTOR_PC1) m_bConnectPC1 = FALSE;
	if (nInspector == INSPECTOR_PC2) m_bConnectPC2 = FALSE;
	if (nInspector == INSPECTOR_PC3) m_bConnectPC3 = FALSE;
	if (nInspector == INSPECTOR_PC4) m_bConnectPC4 = FALSE;
}

void CInspector::Set_StatusRequest(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("STATUS,REQUEST");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_StatusReply(int nInspector, int nStatus)
{
	CString	strSendCmd;
	strSendCmd.Format("STATUS,REPLY,%d", nStatus);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_StatusUpdate(int nInspector, int nStatus)
{
	CString	strSendCmd;
	strSendCmd.Format("STATUS,UPDATE,%d", nStatus);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_LotStart(int nInspector, CString sLotId, int nPortNo, CString sProgVer, CString sParaVer)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	m_bLotReady1 = m_bLotReady2 = m_bLotReady3 = m_bLotReady4 = FALSE;
	CString	strSendCmd;
	strSendCmd.Format("LOT,START,%s,%d,%d,%d,%s,%s,%s", sLotId, nPortNo, gData.nTrayUseCount[nPortNo-1], gData.nCmUseCount[nPortNo-1], gData.sRecipe, sProgVer, sParaVer);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_LotEnd(int nInspector, CString sLotId, int nPortNo)
{
	CString	strSendCmd;
	strSendCmd.Format("LOT,END,%s,%d", sLotId, nPortNo);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_LotRdyCom(int nInspector, CString sLotId)
{
	CString	strSendCmd;
	strSendCmd.Format("LOT,RDYCOM,%s", sLotId);
	Send_Command(nInspector, strSendCmd);
}


void CInspector::Set_LoadComplete(int nInspector, CString sGbn, CString sLotId, int nPortNo, int nTNo1, int nTNo2, int nTNo3, int nTNo4, int nCNo1, int nCNo2, int nCNo3, int nCNo4)
{
	CString	strSendCmd, strTemp;

	if (sGbn == "B1SP" || sGbn == "B1AG" || sGbn == "T1" || sGbn == "T2" || sGbn == "B2") {
		int nScanReq = 0;
		if (nCNo1 > 0) nScanReq++;
		if (nCNo2 > 0) nScanReq++;
		if (nCNo3 > 0) nScanReq++;
		if (nCNo4 > 0) nScanReq++;

		
		if (nCNo1 == 0) { nCNo1 = -1;	nTNo1 = -1; }
		if (nCNo2 == 0) { nCNo2 = -1;	nTNo2 = -1; }
		if (nCNo3 == 0) { nCNo3 = -1;	nTNo3 = -1; }
		if (nCNo4 == 0) { nCNo4 = -1;	nTNo4 = -1; }

		if (nTNo1 == 0) nTNo1 = -1;
		if (nTNo2 == 0) nTNo2 = -1;
		if (nTNo3 == 0) nTNo3 = -1;
		if (nTNo4 == 0) nTNo4 = -1;

		if (sGbn == "B1SP") { m_nB1_SPScanReq = nScanReq; m_nB1_SPScanCnt = 0; gData.bScanDone[1] = FALSE; }
		if (sGbn == "B1AG") { m_nB1_AGScanReq = nScanReq; m_nB1_AGScanCnt = 0; gData.bScanDone[5] = FALSE; }
		if (sGbn == "T1") { m_nT1ScanReq = nScanReq; m_nT1ScanCnt = 0; gData.bScanDone[2] = FALSE; }
		if (sGbn == "T2") { m_nT2ScanReq = nScanReq; m_nT2ScanCnt = 0; gData.bScanDone[3] = FALSE; }
		if (sGbn == "B2") { m_nB2ScanReq = nScanReq; m_nB2ScanCnt = 0; gData.bScanDone[4] = FALSE; }

	}
	strSendCmd.Format("LOAD,COMPLETE,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d", sGbn, sLotId, nPortNo, nTNo1, nTNo2, nTNo3, nTNo4, nCNo1, nCNo2, nCNo3, nCNo4);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_3DLoadComplete(int nInspector, CString sGbn, CString sLotId, int nPortNo, int nTNo1, int nTNo2, int nTNo3, int nTNo4, int nTNo5, int nTNo6, int nTNo7, int nTNo8,
	int nCNo1, int nCNo2, int nCNo3, int nCNo4, int nCNo5, int nCNo6, int nCNo7, int nCNo8)
{
	CString	strSendCmd, strTemp;

	int nScanReq = 0;
	if (nCNo1 > 0) nScanReq++;
	if (nCNo2 > 0) nScanReq++;
	if (nCNo3 > 0) nScanReq++;
	if (nCNo4 > 0) nScanReq++;
	if (nCNo5 > 0) nScanReq++;
	if (nCNo6 > 0) nScanReq++;
	if (nCNo7 > 0) nScanReq++;
	if (nCNo8 > 0) nScanReq++;

	if (nCNo1 == 0) { nCNo1 = -1;	nTNo1 = -1; }
	if (nCNo2 == 0) { nCNo2 = -1;	nTNo2 = -1; }
	if (nCNo3 == 0) { nCNo3 = -1;	nTNo3 = -1; }
	if (nCNo4 == 0) { nCNo4 = -1;	nTNo4 = -1; }
	if (nCNo5 == 0) { nCNo5 = -1;	nTNo5 = -1; }
	if (nCNo6 == 0) { nCNo6 = -1;	nTNo6 = -1; }
	if (nCNo7 == 0) { nCNo7 = -1;	nTNo7 = -1; }
	if (nCNo8 == 0) { nCNo8 = -1;	nTNo8 = -1; }

	if (nTNo1 == 0) nTNo1 = -1;
	if (nTNo2 == 0) nTNo2 = -1;
	if (nTNo3 == 0) nTNo3 = -1;
	if (nTNo4 == 0) nTNo4 = -1;
	if (nTNo5 == 0) nTNo5 = -1;
	if (nTNo6 == 0) nTNo6 = -1;
	if (nTNo7 == 0) nTNo7 = -1;
	if (nTNo8 == 0) nTNo8 = -1;
	
	if (sGbn == "B13D") { m_nB1_3DScanReq = nScanReq; m_nB1_3DScanCnt = 0; gData.bScanDone[6] = FALSE; }

	strSendCmd.Format("3DLOAD,COMPLETE,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", sGbn, sLotId, nPortNo, nTNo1, nTNo2, nTNo3, nTNo4, nTNo5, nTNo6, nTNo7, nTNo8, 
		nCNo1, nCNo2, nCNo3, nCNo4, nCNo5, nCNo6, nCNo7, nCNo8);
	Send_Command(nInspector, strSendCmd);

}

void CInspector::Set_MoveComplete(int nInspector, CString sGbn)
{
	CString	strSendCmd;
	strSendCmd.Format("MOVE,COMPLETE,%s", sGbn);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_PositionReply(int nInspector, CString sGbn, double dZ)
{
	CString	strSendCmd;
	strSendCmd.Format("POSITION,REPLY,%s,%0.3lf", sGbn, dZ);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_AlignRequest(int nInspector, CString sLotId, int nPortNo, int nTrayNo, int nSeqNo)
{
	CString	strSendCmd;
	strSendCmd.Format("ALIGN,REQUEST,%s,%d,%d,%d", sLotId, nPortNo, nTrayNo, nSeqNo);
	Send_Command(nInspector, strSendCmd);

	memset(m_dAlignX, 0x00, sizeof(double) * 32);
	memset(m_dAlignY, 0x00, sizeof(double) * 32);
	memset(m_dAlignQ, 0x00, sizeof(double) * 32);
}

void CInspector::Set_ShiftComplete(int nInspector, CString sGbn)
{
	CString	strSendCmd;
	strSendCmd.Format("SHIFT,COMPLETE,%s", sGbn);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_SMoveComplete(int nInspector, CString sGbn)
{
	CString	strSendCmd;
	strSendCmd.Format("SMOVE,COMPLETE,%s", sGbn);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_BarcodeUpdate(int nInspector, CString sLotId, int nPortNo, int nTrayNo, int nCmNo, CString sBarcode)
{
	CString	strSendCmd;
	strSendCmd.Format("BARCODE,UPDATE,%s,%d,%d,%d,%s", sLotId, nPortNo, nTrayNo, nCmNo, sBarcode);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_InitialRequest(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("INITIAL,REQUEST");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_ReloadComplete(int nInspector, CString sPc)
{
	CString	strSendCmd;
	strSendCmd.Format("RELOAD,COMPLETE,%s", sPc);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_CycleStop(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("CYCLE,STOP");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_TimeUpdate(int nInspector)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d %03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	CString	strSendCmd;
	strSendCmd.Format("TIME,UPDATE,%s", strTime);
	Send_Command(nInspector, strSendCmd);
}



void CInspector::Set_FOBRequest(int nInspector)
{
	m_nFOBModeCnt = 0;
	gData.nVisionFOBMode = -1;
	CString strSendCmd;
	strSendCmd.Format("FOB,REQUEST");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_ApdRequest(int nInspector, CString sGbn, int nPortNo)
{
	CString sSendData;
	if (sGbn == "AG") { gData.bAviApdReceive[0] = FALSE; }
	if (sGbn == "B1") { gData.bAviApdReceive[1] = FALSE; }
	if (sGbn == "T1") { gData.bAviApdReceive[2] = FALSE; }
	if (sGbn == "T2") { gData.bAviApdReceive[3] = FALSE; }
	if (sGbn == "B2") { gData.bAviApdReceive[4] = FALSE; }
	sSendData.Format("APD,REQUEST,%s,%s,%d", sGbn, gData.sLotID[nPortNo-1], nPortNo);
	Send_Command(nInspector, sSendData);
}

void CInspector::Set_3DENDComplete(int nInspector)
{
	CString sSendData;
	sSendData.Format("3DEND,COMPLETE");
	Send_Command(nInspector, sSendData);
}

///////////////////////////////////////////////////////////////////////////////

int CInspector::Get_VisionStatus(int nInspector)
{
	switch (nInspector) {
	case INSPECTOR_PC1:	return m_nStatusPC1;
	case INSPECTOR_PC2:	return m_nStatusPC2;
	case INSPECTOR_PC3:	return m_nStatusPC3;
	case INSPECTOR_PC4:	return m_nStatusPC4;
	default:			return 0;
	}
}

BOOL CInspector::Check_LotReady()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
#ifdef DRY_RUN_TEST
	m_bLotReady1 = TRUE;
	m_bLotReady2 = TRUE;
	m_bLotReady3 = TRUE;
	m_bLotReady4 = TRUE;
#else
	if (!pEquipData->bUseInspectBtm1Specular && !pEquipData->bUseInspectBtm1Angle && !pEquipData->bUseInspectBtm13D ) m_bLotReady1 = TRUE;
	if (!pEquipData->bUseInspectTop1  && !pEquipData->bUseInspectAngle && !pEquipData->bUseVisionAlign) m_bLotReady2 = TRUE;
	if (!pEquipData->bUseInspectTop2) m_bLotReady3 = TRUE;
	if (!pEquipData->bUseInspectBtm2) m_bLotReady4 = TRUE;
#endif

	return (m_bLotReady1 && m_bLotReady2 && m_bLotReady3 && m_bLotReady4);
}



void CInspector::Set_InspectResult(int nPC, int nPortNo, int nTrayNo, int nCmNo)
{
	int nPx = nPortNo - 1;
	int nTx = nTrayNo - 1;
	int nCx = nCmNo - 1;
	if (nPx < 0 || nPx > 1 || nTx < 0 || nTx > 29 || nCx < 0 || nCx > 39) return;

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	// byInspectDone[2][30][40] : 검사완료 (5bit 사용 ==> 0:Angle, 1:Btm1_SP, 2:top1, 3:Top2, 4:Btm2, 5:Btm1_AG, 6:Btm1_3D)
	if	(nPC == 2 && pEquipData->bUseInspectAngle && ((gData.byInspectDone[nPx][nTx][nCx] >> 0) & 1) == 0) { gData.byInspectDone[nPx][nTx][nCx] |= (1 << 0); gData.nInspectInfo[nPx][nTx][nCx] = 4;}	// Angle
	if	(nPC == 1 && pEquipData->bUseInspectBtm1Specular  && ((gData.byInspectDone[nPx][nTx][nCx] >> 1) & 1) == 0) { gData.byInspectDone[nPx][nTx][nCx] |= (1 << 1); gData.nInspectInfo[nPx][nTx][nCx] = 4;}	// Btm1_SP
	if	(nPC == 2 && pEquipData->bUseInspectTop1  && ((gData.byInspectDone[nPx][nTx][nCx] >> 2) & 1) == 0) { gData.byInspectDone[nPx][nTx][nCx] |= (1 << 2); gData.nInspectInfo[nPx][nTx][nCx] = 4;}	// Top1
	if	(nPC == 3 && pEquipData->bUseInspectTop2  && ((gData.byInspectDone[nPx][nTx][nCx] >> 3) & 1) == 0) { gData.byInspectDone[nPx][nTx][nCx] |= (1 << 3); gData.nInspectInfo[nPx][nTx][nCx] = 4;}	// Top2
	if	(nPC == 4 && pEquipData->bUseInspectBtm2  && ((gData.byInspectDone[nPx][nTx][nCx] >> 4) & 1) == 0) { gData.byInspectDone[nPx][nTx][nCx] |= (1 << 4); gData.nInspectInfo[nPx][nTx][nCx] = 4;}	// Btm2
	if	(nPC == 1 && pEquipData->bUseInspectBtm1Angle  && ((gData.byInspectDone[nPx][nTx][nCx] >> 5) & 1) == 0) { gData.byInspectDone[nPx][nTx][nCx] |= (1 << 5); gData.nInspectInfo[nPx][nTx][nCx] = 4;}	//Btm1_AG
	if	(nPC == 1 && pEquipData->bUseInspectBtm13D  && ((gData.byInspectDone[nPx][nTx][nCx] >> 6) & 1) == 0) { gData.byInspectDone[nPx][nTx][nCx] |= (1 << 6); gData.nInspectInfo[nPx][nTx][nCx] = 4;}	//Btm1_AG
}

/////////////////////////////////////////////////////////////////////////////
// UDP Socket Send Message

void CInspector::Send_Command(int nInspector, CString strSend)
{
	// Inspector Log //////////////////////////////////////
	CString strLog;
	strLog.Format("[H->V%d] : %s", nInspector, strSend);
	g_objLogFile.Save_InspectorLog(strLog);
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

void CInspector::DoEvents(int nSleep)
{
	MSG msg;
	if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (nSleep > 0) Sleep(nSleep);
}

/////////////////////////////////////////////////////////////////////////////

void CInspector::Test_Command()
{
}
