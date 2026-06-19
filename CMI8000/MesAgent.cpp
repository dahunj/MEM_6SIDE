// MesAgent.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CMI8000.h"
#include "MesAgent.h"

#include "LogFile.h"
#include "Common.h"
#include "DataManager.h"
#include "Inspector.h"

#define	MES_AGENT_IP	"127.0.0.1"
#define MES_AGENT_PORT	10000		// MesAgent Handler Port


// CMesAgent

IMPLEMENT_DYNAMIC(CMesAgent, CWnd)

CMesAgent g_objMesAgent;

CMesAgent::CMesAgent()
{
	m_bConnected = FALSE;
	m_bHostOnline = FALSE;
	m_strRecvCmd = "";
}

CMesAgent::~CMesAgent()
{
}


BEGIN_MESSAGE_MAP(CMesAgent, CWnd)
	ON_WM_TIMER()
	ON_MESSAGE(UM_CLIENT_CONNECT, OnClientConnect)
	ON_MESSAGE(UM_CLIENT_RECEIVE, OnClientReceive)
	ON_MESSAGE(UM_CLIENT_CLOSE, OnClientClose)
END_MESSAGE_MAP()



// CMesAgent 메시지 처리기입니다.


void CMesAgent::Initialize()
{
	if (m_bConnected) return;

	BOOL ret = m_Client.Open_Socket(MES_AGENT_IP, MES_AGENT_PORT, this);	
	theApp.uSleep(2000);

	if(ret)
	{
		//pass
	}
	else
	{
		m_Client.Close_Socket();
		AfxMessageBox("Connect Fail");
	}


	/*CString sLog, sKey;
	sLog.Format("MesAgent Initialize");
	g_objLogFile.Save_MesAgentLog(sLog);*/
}

void CMesAgent::Terminate()
{
	m_bConnected = FALSE;
	m_bHostOnline = FALSE;
	m_Client.Close_Socket();

	g_objLogFile.Save_MesAgentLog("MesAgent Terminate.");	theApp.uSleep(500);
}



LRESULT CMesAgent::OnClientConnect(WPARAM wConnect, LPARAM lParam)
{
	m_bConnected = (BOOL)wConnect;
	if (!m_bConnected) return 0;

	Set_OperUpdate(gData.sOperID);
	Set_EquipState(2);	//Idle
	g_objLogFile.Save_MesAgentLog("MesAgent Connected");
	return 0;
}

LRESULT CMesAgent::OnClientClose(WPARAM wParam, LPARAM lParam)
{
	m_bConnected = FALSE;
	m_bHostOnline = FALSE;
	m_Client.Close_Socket();
	g_objLogFile.Save_MesAgentLog("MesAgent Disconnected");
	return 0;
}

LRESULT CMesAgent::OnClientReceive(WPARAM wParam, LPARAM lParam)
{
	BYTE byRecv[1025] = { 0 };	// Buffer 1024, Last 0x00
	int nLen = m_Client.Read_Socket(byRecv);

	CString strRecvSocket, strLog;
	strRecvSocket.Format("%s", byRecv);
	m_strRecvCmd += strRecvSocket;

	while (!m_strRecvCmd.IsEmpty()) {
		int nStart = m_strRecvCmd.Find("@");
		int nEnd = m_strRecvCmd.Find("\n");

		if (nEnd < 0) break;	// 버퍼에 들어오는 중...

		if (nStart < 0 || nStart > nEnd) {
			strLog.Format("[<-] : <<Error>> %s : Start(%d), End(%d)", m_strRecvCmd, nStart, nEnd);
			g_objLogFile.Save_MesAgentLog(strLog);
			m_strRecvCmd.Delete(0, nEnd + 1);	// 쓰레기값이 채워져 있어서...
			continue;
		}

		CString strRecv = m_strRecvCmd.Mid(nStart + 1, nEnd - nStart - 1);
		m_strRecvCmd.Delete(0, nEnd + 1);

		// Inspector Log ////////////////////////////////////////////////////////////
		strLog.Format("[<-] : %s", strRecv);
		g_objLogFile.Save_MesAgentLog(strLog);
		/////////////////////////////////////////////////////////////////////////////

		char chSep = ',';
		CString strCmd, strOp;

		AfxExtractSubString(strCmd, strRecv, 0, chSep);
		AfxExtractSubString(strOp, strRecv, 1, chSep);
				
		

		CString strArg[10];
		for (int i = 0; i < 5; i++) AfxExtractSubString(strArg[i], strRecv, i + 2, chSep);

		if (strCmd == "CONTROL") {
			if (strOp == "STATE") Get_ControlState(strArg[0]);

		} else if (strCmd == "LOT") {
			if (strOp == "START")  Get_LotStart(strArg[0], strArg[1], strArg[2]);
			if (strOp == "FAIL") Get_LotIDFail(strArg[0], strArg[1], strArg[2], strArg[3], strArg[4]);

		} else if (strCmd == "TIME") {
			if (strOp == "UPDATE") Get_TimeSync();

		} 
		else if(strCmd == "PP")
		{
			if (strOp == "SELECT")	Get_PPSelect(strArg[0], strArg[1], strArg[2]);
			//if (strOp == "CONFIRM") Get_PPUpload_Confirm(strArg[0]);
			//if (strOp == "FAIL") Get_PPUpload_Fail(strArg[0], strArg[1], strArg[2]);
		}
		
	}
	return 0;
}


void CMesAgent::Send_Command(CString sSend)
{	
	CString strSendSocket, strLog;
	//#ifdef AJIN_BOARD_USE
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseMES) return;	

	if (!m_bConnected) return;
	if (sSend.Left(11) != "OPER,UPDATE" && sSend.Left(7) != "CONTROL" && sSend.Left(9) != "LOT,ABORT") {
		if (!m_bHostOnline) return;
	}	

	strSendSocket.Format("@%s\n", sSend);

	char chSend[1001] = { 0 };	// Buffer 1000, Last 0x00
	int nLength = strSendSocket.GetLength();
	memcpy(chSend, (LPSTR)(LPCSTR)strSendSocket, nLength);

	if (!m_Client.Write_Socket((BYTE*)chSend, nLength)) return;
	//#endif
	// Host Log ////////////////////////////////////////////
	strLog.Format("[->] : %s", sSend);
	g_objLogFile.Save_MesAgentLog(strLog);
	///////////////////////////////////////////////////////
}

void CMesAgent::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(0);
	if (GetTickCount() - gMes.dwMesTime > MES_WAITTIME) {
		g_objCommon.Show_Error(9000);
		KillTimer(0);
		return;
	}
	SetTimer(0, 100, NULL);
	CWnd::OnTimer(nIDEvent);
}

//get

void CMesAgent::Get_TimeSync()
{
	//g_objInspector.Set_TimeUpdate(VISION_PC1);
}

void CMesAgent::Get_ControlState(CString sFlag)
{
	int nOnline = atoi(sFlag);	// 1:Online, 2:Offline
	// 	if (m_bHostOnline && nOnline == 2) g_objCommon.Show_Error(9006);	// Agent 에서 Offline 변경
	if (nOnline == 1 && m_bHostOnline == FALSE) Set_OperUpdate(gData.sOperID);
	m_bHostOnline = (nOnline == 1 ? TRUE : FALSE);
}

void CMesAgent::Get_LotStart(CString sLotId, CString sRecipe, CString sCMCount)
{
	gMes.bPPSelected = TRUE;
	gMes.sHostLotID = sLotId;
	gMes.sHostRecipe = sRecipe;
	gMes.nHostCount = atoi(sCMCount);
}

void CMesAgent::Get_LotIDFail(CString sLotId, CString sRTSTID, CString sLabelType, CString sCode, CString sText)
{
	gMes.sHostFailLotId = sLotId;

	gMes.sRTSTID = sRTSTID;
	gMes.sLabelType = sLabelType;

	gMes.sHostFailCode = sCode;
	gMes.sHostFailText = sText;
	g_objCommon.Show_Error(9032);
}


void CMesAgent::Get_PPSelect(CString sLotId, CString sRecipe, CString sOperID)
{
	gMes.sHostLotID = sLotId;
	gMes.sHostRecipe = sRecipe;

	if (gMes.sHostLotID.GetLength() < 5 || gMes.sHostRecipe.GetLength() < 1) 
	{
		g_objCommon.Show_Error(9004); return;
	}	
	gMes.bLotReported = TRUE;	
}



//
//void CMesAgent::Get_PPUpload_Confirm(CString sRecipeID)
//{
//	gMes.sHostRecipe[gMes.nElevPos] = sRecipeID;
//	gMes.bPPConfirm = TRUE;
//}

//void CMesAgent::Get_PPUpload_Fail(CString sRecipeID, CString sFailCode, CString sFailText)
//{
//	gMes.sHostRecipe[gMes.nElevPos] = sRecipeID;
//	gMes.bPPConfirm = FALSE;
//	gMes.sHostCancelCode = sFailCode;
//	gMes.sHostCancelText = sFailText;
//	g_objCommon.Show_Error(9031);
//}



//Set

void CMesAgent::Set_EquipState(int nFlag)
{
	// MES : Init, idle, Setup, Ready, Executing(=Run), Paused(=Down)
	CString strSend;
	strSend.Format("EQUIP,STATE,%d,%s", nFlag, gData.sOperID);	// 1:Init, 2:Idle, 3:Setup, 4:Ready, 5:Run(=Executing), 6;Pause(=Down)
	Send_Command(strSend);
}

void CMesAgent::Set_ErrorUpdate(int nFlag, CString sErrNo)
{
	CString strSend;
	strSend.Format("ERROR,UPDATE,%d,%s", nFlag, sErrNo);
	Send_Command(strSend);
}

void CMesAgent::Set_ControlState(int nFlag, CString sOperId)
{
	CString strSend;
	strSend.Format("CONTROL,STATE,%d,%s", nFlag, sOperId);
	Send_Command(strSend);
	if (nFlag == 2) m_bHostOnline = FALSE;	// 사용자 Offline
}

void CMesAgent::Set_OperUpdate(CString sOperId)
{
	if (sOperId.GetLength() < 4) return;
	CString strSend;
	strSend.Format("OPER,UPDATE,%s", sOperId);
	Send_Command(strSend);
}

void CMesAgent::Set_IdleReport(CString sOperId, CString sSTime, CString sETime, CString sCode, CString sType)
{
	CString strSend;
	strSend.Format("IDLE,REPORT,%s,%s,%s,%s,%s", sOperId, sSTime, sETime, sCode, sType);
	Send_Command(strSend);
}


void CMesAgent::Set_LotIDReport(int nType, CString sLotID, int nPortNo, CString sRecipe)
{
	CString strSend; 
	strSend.Format("LOT,REPORT,%d,%s,%d,%s", nType, sLotID, nPortNo, gData.sRecipe);
	Send_Command(strSend);
}


void CMesAgent::Set_PPSelectedReport(CString sLotId, CString sRecipeId)
{
	CString strSend; 
	strSend.Format("PP,SELECTED,%s,%s", sLotId, sRecipeId);
	Send_Command(strSend);
}

//void CMesAgent::Set_PPUploadCompletedReport(CString sLotId, CString sMGZId, CString sRecipeId)
//{
//	CString strSend; 
//	strSend.Format("PP,COMPLETED,%s,%s,%s", sLotId, sMGZId, sRecipeId);
//	Send_Command(strSend);
//}


void CMesAgent::Set_LotStartedReport(CString sOperID, CString sLotId, CString sRecipe, CString sCMCount)
{
	CString strSend, strLogID;
	
	strSend.Format("LOT,START,%s,%s,%s,%s", sOperID, sLotId, sRecipe, sCMCount);
	Send_Command(strSend);
}


void CMesAgent::Set_ProductCompletedReport(CString sOperID, CString sLotID, int nTrayNo, int nCMNo,  CString sResult, CString sReasonCode, CString sCMBarcode, int UnitNo)
{
	CString strSend, strLogID;

	strSend.Format("PRODUCT,COMPLETED,%s,%s,%d,%d,%s,%s,%s,%d", sOperID, sLotID,nTrayNo,nCMNo,sResult,sReasonCode,sCMBarcode, UnitNo);
	Send_Command(strSend);
}