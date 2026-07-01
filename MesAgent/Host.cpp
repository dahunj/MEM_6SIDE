// Host.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "MesAgent.h"
#include "Host.h"

#include "Common.h"
#include "LogFile.h"
#include "MesAgentDlg.h"
#include "Handler.h"

IMPLEMENT_DYNAMIC(CHost, CWnd)

CHost g_objHost;

const char STX = 0x02;
const char ETX = 0x03;
const CString CRLF = "\r\n";

// CHost

CHost::CHost()
{
	m_bConnected = FALSE;
	m_bHostOnline = FALSE;
	m_strRecvCmd = "";

	m_strStFn = "";
	m_strRcmd = "";
	m_dwLastTime = GetTickCount();


	for(int i = 0; i < 20; i++)
	{
		 gMes.dwCTStart[i] = 0 ;
		 gMes.bCTTickStarted[i] = FALSE;	
	}

	
}

CHost::~CHost()
{
}

BEGIN_MESSAGE_MAP(CHost, CWnd)
	ON_MESSAGE(UM_SERVER_ACCEPT, &CHost::OnServerAccept)
	ON_MESSAGE(UM_SERVER_RECEIVE, &CHost::OnServerReceive)
	ON_MESSAGE(UM_SERVER_REMOVE, &CHost::OnServerRemove)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CHost 메시지 처리기입니다.

void CHost::Initialize()
{
	m_bHostOnline = FALSE;
	m_nSendCmdCount = 0;
	m_nLPort = gData.nHostPort;
	m_Server.Listen_Socket(m_nLPort, this);

	SetTimer(0, 100, NULL);
}

void CHost::Terminate()
{
	m_bHostOnline = FALSE;
	m_Server.Close_Socket();
	if (g_objHandler.Is_Connected()) g_objHandler.Set_ControlState(2);	// 1:Online, 2:Offline
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CHost::OnServerAccept(WPARAM wLocalPort, LPARAM lClientIdx)
{
	UINT nPort = (UINT)wLocalPort;
	int nClient = (int)lClientIdx;

	if (nPort != m_nLPort) { g_objLogFile.Save_HostLog("OnServerAccept - Local Port Mismatch"); return 0; }

	if (lClientIdx > 0) { for (int i = 0; i < m_Server.Get_ClientCount()-1; i++) m_Server.Close_Client(i); }
	m_bConnected = TRUE;

	CString strHostIP = "", strLog;
	UINT nHostPort = 0;
	if (!m_Server.Get_ClientInfo(0, strHostIP, nHostPort)) return 0;

	strLog.Format("Host Connected. IP(%s), Port, %d", strHostIP, nHostPort);
	g_objLogFile.Save_HostLog(strLog);

	CMesAgentDlg *pMainDlg = (CMesAgentDlg*)AfxGetMainWnd();
	pMainDlg->Set_HostConnect(TRUE, strHostIP, nHostPort);

	m_dwLastTime = GetTickCount();

	if (g_objHandler.Is_Connected()) Set_S6F11_ControlState(1);	//1:Online, 2:Offline

	Set_S1F1_Ready();

	return 0;
}

LRESULT CHost::OnServerReceive(WPARAM wLocalPort, LPARAM lClientIdx)
{
	UINT nPort = (UINT)wLocalPort;
	int nClient = (int)lClientIdx;

	if (nPort != m_nLPort) { g_objLogFile.Save_HostLog("OnServerReceive - Local Port Mismatch"); return 0; }

	BYTE byRecv[8193] = { 0 };	// 마지막 0x00
	int nLen = m_Server.Read_Socket(0, byRecv);
	if (nLen < 1) { g_objLogFile.Save_HostLog("OnServerReceive - Data Zero"); return 0; }

	char *pRecv = (char*)byRecv;
	CString strRecvSocket = CString(pRecv);
	m_strRecvCmd += strRecvSocket;

	CMesAgentDlg *pMainDlg = (CMesAgentDlg*)AfxGetMainWnd();
	CString strLog, strMsg;

	while (!m_strRecvCmd.IsEmpty()) {
		int nStart = m_strRecvCmd.Find(STX);
		int nEnd = m_strRecvCmd.Find(ETX);

		if (nEnd < 0) break;	// 버퍼에 들어오는 중...

		if (nStart < 0 || nStart > nEnd) {
			strLog.Format("[OnServerReceive] <<Error>> - Start(%d), End(%d).\n%s", nStart, nEnd, m_strRecvCmd);
			g_objLogFile.Save_HostLog(strLog);
			m_strRecvCmd.Delete(0, nEnd + 1);	// 쓰레기값이 채워져 있어서...
			continue;
		}

		m_dwLastTime = GetTickCount();	// 시간 갱신

		CString strRecv = m_strRecvCmd.Mid(nStart + 1, nEnd - nStart - 1);
		m_strRecvCmd.Delete(0, nEnd + 1);

		// Host Log /////////////////////////////////////////////////////////////////
		strLog.Format("[<-] %s", strRecv);
		g_objLogFile.Save_HostLog(strLog);

		m_nRecvCmdCount = atoi(strRecv.Mid(8, 4));	// 4Byte

		if (strRecv.GetAt(12) == '0') {		// Heart Beat
			strMsg.Format("%s : [HeartBeat]", strLog.Left(18));
			pMainDlg->Set_HostMsg(strMsg);
			Reply_HeartBeat();

		} else {
			CString strXml = strRecv.Right(strRecv.GetLength() - 13);
			if (!Extract_Xml(strXml)) return 0;

			strMsg.Format("%s : %s,%s", strLog.Left(18), m_strStFn, m_strRcmd); 
			pMainDlg->Set_HostMsg(strMsg);

			if 		(m_strStFn == "S1F1")  Get_S1F1_Ready();	// Are You There Request
			else if (m_strStFn == "S1F3")  Get_S1F3_State();	// Equip Status Request
			else if (m_strStFn == "S2F3")  Get_S2F3_Link();		// Link Test Request
			else if (m_strStFn == "S2F31") Get_S2F31_Time();	// Date and Time Set Request
			else if (m_strStFn == "S2F49" && m_strRcmd == "PP_SELECT")			 Get_S2F49_PPSelect();			
			//else if (m_strStFn == "S2F49" && m_strRcmd == "PP_UPLOAD_CONFIRM")	 Get_S2F49_PP_UPLOAD_CONFIRM();
			//else if (m_strStFn == "S2F49" && m_strRcmd == "PP_UPLOAD_FAIL")		 Get_S2F49_PP_UPLOAD_FAIL();
			else if (m_strStFn == "S2F49" && m_strRcmd == "LOT_START")			 Get_S2F49_LOT_START();
			else if (m_strStFn == "S2F49" && m_strRcmd == "LOT_ID_FAIL")		 Get_S2F49_LOT_ID_FAIL();
			
			//else if (m_strStFn == "S5F2")  Get_S5F2_AlarmAck();	// Alarm Report Acknowledge
			//else if (m_strStFn == "S10F3") Get_S10F3_Display();
		}
	}

	return 0;
}

LRESULT CHost::OnServerRemove(WPARAM wLocalPort, LPARAM lClientIdx)
{
	UINT nPort = (UINT)wLocalPort;
	int nClient = (int)lClientIdx;

	if (nPort != m_nLPort) { g_objLogFile.Save_HostLog("OnServerRemove - Local Port Mismatch"); return 0; }
	m_bConnected = FALSE;
	m_bHostOnline = FALSE;

	CString strLog = "Host Disconnected.";
	g_objLogFile.Save_HostLog(strLog);

	CMesAgentDlg *pMainDlg = (CMesAgentDlg*)AfxGetMainWnd();
	pMainDlg->Set_HostConnect(FALSE, "0.0.0.0", 0);

	if (g_objHandler.Is_Connected()) g_objHandler.Set_ControlState(2);	// 1:Online, 2:Offline

	return 0;
}

BOOL CHost::Extract_Xml(CString sXmlData)
{
	m_strStFn = m_strRcmd = "";	// 초기화

	CMesAgentDlg *pMainDlg = (CMesAgentDlg*)AfxGetMainWnd();
	if (!m_xml.LoadXml(sXmlData) ) 
	{
		CString strLog, strMsg;

		strMsg.Format("[Extract_Xml] CXml Data Load Fail.");
		pMainDlg->Set_HostMsg(strMsg);

		strLog.Format("%s\n%s", strMsg, sXmlData);
		g_objLogFile.Save_HostLog(strLog);

		return FALSE;
	}

	CXmlNode node = m_xml.GetRoot();
	m_strStFn = node.GetAttribute("ID");

	if (m_strStFn == "S2F31") {
		CXmlNode nodeTime = m_xml.GetRoot()->GetChild("ITEM")->GetChild("TIME");
		m_strSetTime = nodeTime.GetAttribute("VALUE", "");
	} 
	else if (m_strStFn == "S2F49")
	{
		CXmlNode nodeE = m_xml.GetRoot()->GetChild("ITEM")->GetChild("RCMDCP")->GetChild("RCMD");
		m_strRcmd = nodeE.GetAttribute("VALUE", "");
		
		 if (m_strRcmd == "PP_SELECT") 
		 {
			CXmlNodes nodes = m_xml.GetRoot()->GetChild("ITEM")->GetChild("RCMDCP")->GetChild("CPLIST")->GetChildren();
			int nCount = nodes.GetCount();

			for (int i = 0; i < nCount; i++) 
			{
				CString strName = nodes[i]->GetChild("CPNAME")->GetAttribute("VALUE");
				CString strData = nodes[i]->GetChild("CPVAL")->GetAttribute("VALUE");

				if (strName == "LOTID")			gMes.sHostLotId = strData;
				if (strName == "RECIPEID")		gMes.sHostRecipe = strData;
				if (strName == "OPERATORID")	gData.sOperId = strData;
			}
		 }	
		 if (m_strRcmd == "LOT_ID_FAIL") 
		 {
			 CXmlNodes nodes = m_xml.GetRoot()->GetChild("ITEM")->GetChild("RCMDCP")->GetChild("CPLIST")->GetChildren();
			 int nCount = nodes.GetCount();

			 for (int i = 0; i < nCount; i++) 
			 {
				 CString strName = nodes[i]->GetChild("CPNAME")->GetAttribute("VALUE");
				 CString strData = nodes[i]->GetChild("CPVAL")->GetAttribute("VALUE");

				 if (strName == "LOTID")			gMes.sHostLotId = strData;
				 if (strName == "OPERATORID")		gData.sOperId = strData;
				/* if (strName == "RTSTID")			gMes.sRTSTID = strData;
				 if (strName == "LABELTYPE")		gMes.sLabelType = strData;*/
			 }

			 nodeE = m_xml.GetRoot()->GetChild("ITEM")->GetChild("RCMDCP")->GetChild("RESULT");
			 gMes.sFailCode = nodeE.GetChild("CODE")->GetAttribute("VALUE");
			 gMes.sFailText = nodeE.GetChild("TEXT")->GetAttribute("VALUE");
		 }	

		 if (m_strRcmd == "LOT_START") 
		 {
			 CXmlNodes nodes = m_xml.GetRoot()->GetChild("ITEM")->GetChild("RCMDCP")->GetChild("CPLIST")->GetChildren();
			 int nCount = nodes.GetCount();

			 for (int i = 0; i < nCount; i++) 
			 {
				 CString strName = nodes[i]->GetChild("CPNAME")->GetAttribute("VALUE");
				 CString strData = nodes[i]->GetChild("CPVAL")->GetAttribute("VALUE");

				 if (strName == "LOTID")		gMes.sHostLotId = strData;
				 if (strName == "RECIPEID")		gMes.sHostRecipe = strData;
				 if (strName == "TOTALQTY")		gMes.sHostCMCount = strData;
				 								
			 }		
		 }	


		 //if (m_strRcmd == "MGZ_CANCEL") 
		 //{
			// CXmlNodes nodes = m_xml.GetRoot()->GetChild("ITEM")->GetChild("RCMDCP")->GetChild("CPLIST")->GetChildren();
			// int nCount = nodes.GetCount();

			// for (int i = 0; i < nCount; i++) {
			//	 CString strName = nodes[i]->GetChild("CPNAME")->GetAttribute("VALUE");
			//	 CString strData = nodes[i]->GetChild("CPVAL")->GetAttribute("VALUE");

			//	 if (strName == "MGZID")			gMes.sHostLdMGZId = strData;				
			// }

			// nodeE = m_xml.GetRoot()->GetChild("ITEM")->GetChild("RCMDCP")->GetChild("RESULT");
			// gMes.sCancelCode = nodeE.GetChild("CODE")->GetAttribute("VALUE");
			// gMes.sCancelText = nodeE.GetChild("TEXT")->GetAttribute("VALUE");
		 //}	
		 //
		 //if (m_strRcmd == "MGZ_ID_CONFIRM") 
		 //{
			// CXmlNodes nodes = m_xml.GetRoot()->GetChild("ITEM")->GetChild("RCMDCP")->GetChild("CPLIST")->GetChildren();
			// int nCount = nodes.GetCount();

			// for (int i = 0; i < nCount; i++) {
			//	 CString strName = nodes[i]->GetChild("CPNAME")->GetAttribute("VALUE");
			//	 CString strData = nodes[i]->GetChild("CPVAL")->GetAttribute("VALUE");

			//	 if (strName == "MGZID")		gMes.sHostUldMGZId = strData;				
			// }
		 //}	

		 //if (m_strRcmd == "PP_UPLOAD_CONFIRM") 
		 //{
			// CXmlNodes nodes = m_xml.GetRoot()->GetChild("ITEM")->GetChild("RCMDCP")->GetChild("CPLIST")->GetChildren();
			// int nCount = nodes.GetCount();

			// for (int i = 0; i < nCount; i++) 
			// {
			//	 CString strName = nodes[i]->GetChild("CPNAME")->GetAttribute("VALUE");
			//	 CString strData = nodes[i]->GetChild("CPVAL")->GetAttribute("VALUE");

			//	 if (strName == "LOTID")		gMes.sHostLotId = strData;
			//	 if (strName == "MGZID")		gMes.sHostLdMGZId = strData;
			//	 if (strName == "RECIPEID")		gMes.sHostRecipe = strData;							
			// }		
		 //}		

		 //if (m_strRcmd == "PP_UPLOAD_FAIL") 
		 //{
			// CXmlNodes nodes = m_xml.GetRoot()->GetChild("ITEM")->GetChild("RCMDCP")->GetChild("CPLIST")->GetChildren();
			// int nCount = nodes.GetCount();

			// for (int i = 0; i < nCount; i++) {
			//	 CString strName = nodes[i]->GetChild("CPNAME")->GetAttribute("VALUE");
			//	 CString strData = nodes[i]->GetChild("CPVAL")->GetAttribute("VALUE");

			//	 if (strName == "RECIPEID")			gMes.sHostRecipe = strData;				
			// }

			// nodeE = m_xml.GetRoot()->GetChild("ITEM")->GetChild("RCMDCP")->GetChild("RESULT");
			// gMes.sFailCode = nodeE.GetChild("CODE")->GetAttribute("VALUE");
			// gMes.sFailText = nodeE.GetChild("TEXT")->GetAttribute("VALUE");
		 //}	
		

		

	} 
	else if(m_strStFn == "S7F25")
	{
		Set_S7F26();
	}
	else if (m_strStFn == "S10F3")
	{
		m_strDisplay = m_xml.GetRoot()->GetChild("ITEM")->GetChild("TEXT")->GetAttribute("VALUE");
		g_objHandler.Set_TerminalDisplay(m_strDisplay);
	}

	m_xml.Close();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Get Command

void CHost::Get_S1F1_Ready()
{
	CString strControl, strEqiup;

	strControl = (g_objHandler.Is_Connected() ? "1" : "2");		// 1:Online, 2:Offline
	strEqiup.Format("%d", gData.nCurEquipState);

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"1.4\" ID=\"S1F2\" NAME=\"Are You There Data\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <SOFTREV NAME=\"SOFTREV\" VALUE=\"" + (CString)MAIN_VERSION + "\" />" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, TRUE, "S1F2");	// Are You There Data => S1F1 응답
}

void CHost::Get_S1F3_State()
{
	CString strControl, strEqiup;

	strControl = (g_objHandler.Is_Connected() ? "1" : "2");		// 1:Online, 2:Offline
	strEqiup.Format("%d", gData.nCurEquipState);

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"1.4\" ID=\"S1F4\" NAME=\"Selected Equipment Status Data\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <SVLIST COUNT = \"3\">" + CRLF;
	strSend += "      <SV NAME=\"ControlState\" VALUE=\"" + strControl + "\" />" + CRLF;
	strSend += "      <SV NAME=\"EquipmentState\" VALUE=\"" + strEqiup + "\" />" + CRLF;
	strSend += "      <SV NAME=\"SWVersion\" VALUE=\"" + (CString)MAIN_VERSION + "\" />" + CRLF;
	strSend += "    </SVLIST>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, TRUE, "S1F4");	// Equip Status Response => S1F3 응답
}

void CHost::Get_S2F3_Link()
{
	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S2F4\" NAME=\"Link Test Response\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, TRUE, "S2F4");	// Link Test Response => S2F3 응답
}

void CHost::Get_S2F31_Time()
{
	if (m_strSetTime.GetLength() < 14) {
		CString strLog;
		strLog.Format("[Get_S2F31] Time Value Error => Time [%s]", m_strSetTime);
		g_objLogFile.Save_HostLog(strLog);
		return;
	}

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	sysTime.wHour = atoi(m_strSetTime.Mid(8, 2));
	sysTime.wMinute = atoi(m_strSetTime.Mid(10, 2));
	sysTime.wSecond = atoi(m_strSetTime.Mid(12, 2));
	sysTime.wYear = atoi(m_strSetTime.Mid(0, 4));
	sysTime.wMonth = atoi(m_strSetTime.Mid(4, 2));
	sysTime.wDay = atoi(m_strSetTime.Mid(6, 2));

	SetLocalTime(&sysTime);	// 사용프로잭트속성.구성속성.링커.매니페스트파일(asInvoker->highestAvailable)

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S2F32\" NAME=\"Date and Time Set Acknowledge\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <ACKC NAME=\"ACKC\" VALUE=\"" + m_strSetTime +"\" />" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, TRUE, "S2F32");	// Date and Time Set Acknowledge => S1F31 응답

	g_objHandler.Set_TimeSync();
}


///////////////////////////////////////////////////////////////////////////////
// Get Command 

void CHost::Get_S2F49_PPSelect()
{
	Set_S2F50_PPSelect(0);
	g_objHandler.Set_PPSelect();
}

//void CHost::Get_S2F49_PP_UPLOAD_CONFIRM()
//{
//	Set_S2F50_PP_UPLOAD_CONFIRM();
//	g_objHandler.Set_PP_Upload_Confirm();
//}
//
//
//void CHost::Get_S2F49_PP_UPLOAD_FAIL()
//{
//	Set_S2F50_PP_UPLOAD_FAIL();
//	g_objHandler.Set_PP_Upload_Fail();
//}

void CHost::Get_S2F49_LOT_START()
{
	Set_S2F50_LOT_START();
	g_objHandler.Set_Lot_Start();
}

void CHost::Get_S2F49_LOT_ID_FAIL()
{
	Set_S2F50_LOT_ID_FAIL();
	g_objHandler.Set_Lot_ID_Fail();
}




///////////////////////////////////////////////////////////////////////////////
// Set Command

void CHost::Set_S7F26()
{
	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	CString sBodyName, sBodyData;

	sBodyName = "Name";
	sBodyData = "Data";

	strSend += "<EIF VERSION=\"2.0\" ID=\"S7F26\" NAME=\"Formatted Process Program Data\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <PPID VALUE=\"" + gMes.sHostRecipe + "\" />" + CRLF;
	strSend += "    <MDLN VALUE=\"0\"/>" + CRLF;
	strSend += "    <SOFTREV VALUE=\"" + gData.sVersion + "\" />" + CRLF;
	strSend += "    <LOTID VALUE=\"" + gMes.sHostLotId + "\" />" + CRLF;
	strSend += "    <PCLIST COUNT=\"1\">" + CRLF;
	strSend += "      <LIST>" + CRLF;
	strSend += "        <CCODE VALUE=\"" + sBodyName + "\" />" + CRLF;
	strSend += "        <PPARM VALUE=\"" + sBodyData + "\" />" + CRLF;
	strSend += "      </LIST>" + CRLF;
	strSend += "    </PCLIST>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, TRUE, "S7F26");
}

void CHost::Set_S1F1_Ready()
{
	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"1.4\" ID=\"S1F1\" NAME=\"Are You There Request\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S1F1");
}

void CHost::Set_S5F1_AlarmReport(int nFlag, CString sErrNo, CString sErrMsg)
{
	int	 nAlCD = (nFlag == 1 ? 161 : 33);
	BYTE cAlCD = (nFlag == 1 ? 128 : 48);

	CString strAlCD;
	strAlCD.Format("%d", nAlCD);

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"1.4\" ID=\"S5F1\" NAME=\"Alarm Report Send\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
    strSend += "  <ITEM>" + CRLF;
	strSend += "    <ALCD VALUE=\"" + strAlCD + "\" />" + CRLF;
	strSend += "    <ALID VALUE=\"" + sErrNo + "\" />" + CRLF;
	strSend += "    <ALTX VALUE=\"" + sErrMsg + "\" />" + CRLF;
    strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S5F1");
}

void CHost::Set_S6F11_ControlState(int nState)
{
	CString strState;
	strState.Format("%d", nState);

	CString strUnitNo = "1";	// 1: AVI, 4:CAP

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d%02d%02d%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S6F11\" NAME=\"Event Report\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <CEID NAME=\"CEID\" VALUE=\"10101\" />" + CRLF;
	strSend += "    <RPTID NAME=\"RPTID\" VALUE=\"10101\" />" + CRLF;
	strSend += "    <DVLIST COUNT=\"5\">" + CRLF;
	strSend += "      <DV NAME=\"OPERATORID\" VALUE=\"" + gData.sOperId + "\" />" + CRLF;	
	strSend += "      <DV NAME=\"TIME\" VALUE=\"" + strTime + "\" />" + CRLF;
	strSend += "      <DV NAME=\"CONTROLSTATE\" VALUE=\"" + strState + "\" />" + CRLF;
	strSend += "      <DV NAME=\"REASONTEXT\" VALUE=\"\"/>" + CRLF;	
	strSend += "      <DV NAME=\"UNITNO\" VALUE=\"" + strUnitNo + "\" />" + CRLF;
	strSend += "    </DVLIST>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S6F11", "10101");

	if (nState == 1) g_objHandler.Set_ControlState(1);	// 1:Online, 2:Offline
	m_bHostOnline = (nState == 1 ? TRUE : FALSE);
}

void CHost::Set_S6F11_EquipState(int nState, CString sErrNo, CString sCategory, CString sErrMsg)
{
	gData.nCurEquipState = nState;
	if (gData.nCurEquipState == gData.nPreEquipState) return;

	CString strState;
	strState.Format("%d", nState);

	CString strUnitNo = "1"; // 1: AVI, 4: CAP 
		
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d%02d%02d%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S6F11\" NAME=\"Event Report\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <CEID NAME=\"CEID\" VALUE=\"10108\" />" + CRLF;
	strSend += "    <RPTID NAME=\"RPTID\" VALUE=\"10108\" />" + CRLF;
	strSend += "    <DVLIST COUNT=\"11\">" + CRLF;
	strSend += "      <DV NAME=\"OPERATORID\" VALUE=\"" + gData.sOperId + "\" />" + CRLF;	
	strSend += "      <DV NAME=\"TIME\" VALUE=\"" + strTime + "\" />" + CRLF;
	strSend += "      <DV NAME=\"PROCESSSTATE\" VALUE=\"" + strState + "\" />" + CRLF;
	strSend += "      <DV NAME=\"ALMLISTQTY\" VALUE=\"3\" />" + CRLF;
	strSend += "      <DV NAME=\"DATANAME1\" VALUE=\"Alarm ID\" />" + CRLF;
	strSend += "      <DV NAME=\"DATAVALUE1\" VALUE=\"" + sErrNo + "\" />" + CRLF;
	strSend += "      <DV NAME=\"DATANAME2\" VALUE=\"Alarm Category\" />" + CRLF;
	strSend += "      <DV NAME=\"DATAVALUE2\" VALUE=\"33\" />" + CRLF;
	strSend += "      <DV NAME=\"DATANAME3\" VALUE=\"Alarm Text\" />" + CRLF;
	strSend += "      <DV NAME=\"DATAVALUE3\" VALUE=\"" + sErrMsg + "\" />" + CRLF;	
	strSend += "      <DV NAME=\"UNITNO\" VALUE=\"" + strUnitNo + "\" />" + CRLF;
	strSend += "    </DVLIST>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S6F11", "10108");

	gData.nPreEquipState = gData.nCurEquipState;
}


void CHost::Set_S6F11_LotIDReport(CString sType, CString sLotID, CString sPortNo, CString sRecipe)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime, sPortID;
	strTime.Format("%04d%02d%02d%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S6F11\" NAME=\"Event Report\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <CEID NAME=\"CEID\" VALUE=\"20106\" />" + CRLF;
	strSend += "    <RPTID NAME=\"RPTID\" VALUE=\"20106\" />" + CRLF;
	strSend += "    <DVLIST COUNT=\"5\">" + CRLF;
	strSend += "      <DV NAME=\"OPERATORID\" VALUE=\"" + gData.sOperId + "\" />" + CRLF;
	strSend += "      <DV NAME=\"TIME\" VALUE=\""		+ strTime + "\" />" + CRLF;
	strSend += "      <DV NAME=\"PORTNO\" VALUE=\""		+ sPortNo +"\" />" + CRLF;
	strSend += "      <DV NAME=\"LOTID\" VALUE=\""		+ sLotID + "\" />" + CRLF;
	strSend += "      <DV NAME=\"RECIPEID\" VALUE=\""	+ sRecipe + "\" />" + CRLF;
	strSend += "    </DVLIST>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	gMes.bCTTickStarted[eCT::LOT_REPORT] = TRUE;
	Send_Command(strSend, FALSE, "S6F11", "20106");
}



void CHost::Set_S6F11_LotStartedReport(CString sOperID, CString sLotID, CString sRecipe, CString sCMCount)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d%02d%02d%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S6F11\" NAME=\"Event Report\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <CEID NAME=\"CEID\" VALUE=\"20101\" />" + CRLF;
	strSend += "    <RPTID NAME=\"RPTID\" VALUE=\"20101\" />" + CRLF;
	strSend += "    <DVLIST COUNT=\"5\">" + CRLF;
	strSend += "      <DV NAME=\"OPERATORID\" VALUE=\"" + gData.sOperId + "\" />" + CRLF;
	strSend += "      <DV NAME=\"TIME\" VALUE=\"" + strTime + "\" />" + CRLF;
	strSend += "      <DV NAME=\"LOTID\" VALUE=\""+ sLotID + "\" />" + CRLF;
	strSend += "      <DV NAME=\"RECIPEID\" VALUE=\"" + sRecipe + "\" />" + CRLF;
	strSend += "      <DV NAME=\"TOTALQTY\" VALUE=\"" + sCMCount + "\" />" + CRLF;
	strSend += "    </DVLIST>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S6F11", "20101");
}

void CHost::Set_S6F11_PPSelectedReport(CString sLotId, CString sRecipeId)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d%02d%02d%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S6F11\" NAME=\"Event Report\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <CEID NAME=\"CEID\" VALUE=\"40102\" />" + CRLF;
	strSend += "    <RPTID NAME=\"RPTID\" VALUE=\"40102\" />" + CRLF;
	strSend += "    <DVLIST COUNT=\"4\">" + CRLF;
	strSend += "      <DV NAME=\"OPERATORID\" VALUE=\"" + gData.sOperId + "\" />" + CRLF;
	strSend += "      <DV NAME=\"TIME\" VALUE=\"" + strTime + "\" />" + CRLF;
	strSend += "      <DV NAME=\"LOTID\" VALUE=\"" + sLotId + "\" />" + CRLF;
	strSend += "      <DV NAME=\"RECIPEID\" VALUE=\"" + sRecipeId + "\"/>" + CRLF;	
	strSend += "    </DVLIST>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	gMes.bCTTickStarted[eCT::PP_SELECTED] = TRUE;
	Send_Command(strSend, FALSE, "S6F11", "40102");
}

void CHost::Set_S6F11_ProductCompletedReport(CString sOperID, CString sLotID, CString sTrayNo, CString sCMNo, CString sResult, CString sReasonCode, CString sBarcode, CString sUnitNo)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d%02d%02d%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S6F11\" NAME=\"Event Report\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <CEID NAME=\"CEID\" VALUE=\"20401\" />" + CRLF;
	strSend += "    <RPTID NAME=\"RPTID\" VALUE=\"20401\" />" + CRLF;
	strSend += "    <DVLIST COUNT=\"12\">" + CRLF;
	strSend += "      <DV NAME=\"OPERATORID\" VALUE=\"" + gData.sOperId + "\" />" + CRLF;
	strSend += "      <DV NAME=\"TIME\" VALUE=\"" + strTime + "\" />" + CRLF;
	strSend += "      <DV NAME=\"LOTID\" VALUE=\"" + sLotID + "\" />" + CRLF;
	strSend += "      <DV NAME=\"TOTRAYID\" VALUE=\"" + sTrayNo + "\" />" + CRLF;
	strSend += "      <DV NAME=\"TOPOCKETID\" VALUE=\"" + sCMNo + "\" />" + CRLF;
	strSend += "      <DV NAME=\"RESULT\" VALUE=\"" + sResult + "\" />" + CRLF;
	strSend += "      <DV NAME=\"REASONCODE\" VALUE=\"" + sReasonCode + "\" />" + CRLF;
	strSend += "      <DV NAME=\"MODULEID\" VALUE=\"" + sBarcode + "\"/>" + CRLF;
	strSend += "      <DV NAME=\"UNITNO\" VALUE=\"" + sUnitNo + "\" />" + CRLF;
	strSend += "      <DV NAME=\"DATAQTY\" VALUE=\"1\" />" + CRLF;
	strSend += "      <DV NAME=\"DATANAME1\" VALUE=\"NAME1\" />" + CRLF;
	strSend += "      <DV NAME=\"DATAVALUE1\" VALUE=\"VALUE1\" />" + CRLF;
	strSend += "    </DVLIST>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S6F11", "20401");
}

void CHost::Set_S6F11_LotEnd(CString sLotId, CString sRecipe, int nCount, int nOk, int nNg, int nBNg)
{
	CString strTotal, strReal, strGood, strBad, strVNg, strBNg;
	strTotal.Format("%d", nCount);
	strReal.Format("%d", nOk + nNg);
	strGood.Format("%d", nOk);
	strVNg.Format("%d", nNg);	// AVI NG
	strBNg.Format("%d", nBNg);			// Barcode NG

	CString strEndPort = "2";	//1: Loader, 2:Unloader(AVI only), 3:CapAttach-Inline
	CString strRsnCode = "AVING";

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d%02d%02d%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S6F11\" NAME=\"Event Report\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <CEID NAME=\"CEID\" VALUE=\"20102\" />" + CRLF;
	strSend += "    <RPTID NAME=\"RPTID\" VALUE=\"20102\" />" + CRLF;
	strSend += "    <DVLIST COUNT=\"14\">" + CRLF;
	strSend += "      <DV NAME=\"OPERATORID\" VALUE=\"" + gData.sOperId + "\" />" + CRLF;
	strSend += "      <DV NAME=\"TIME\" VALUE=\"" + strTime + "\" />" + CRLF;
	strSend += "      <DV NAME=\"PORTNO\" VALUE=\"" + strEndPort + "\" />" + CRLF;
	strSend += "      <DV NAME=\"LOTID\" VALUE=\"" + sLotId + "\" />" + CRLF;
	strSend += "      <DV NAME=\"RECIPEID\" VALUE=\"" + sRecipe + "\" />" + CRLF;
	strSend += "      <DV NAME=\"LOTCOUNT\" VALUE=\"" + strTotal + "\" />" + CRLF;
	strSend += "      <DV NAME=\"LOTREALCOUNT\" VALUE=\"" + strReal + "\" />" + CRLF;
	strSend += "      <DV NAME=\"LOTGOODCOUNT\" VALUE=\"" + strGood + "\" />" + CRLF;
	strSend += "      <DV NAME=\"LOTBADCOUNT\" VALUE=\"" + strBad + "\" />" + CRLF;
	strSend += "      <DV NAME=\"REASONCODEQTY\" VALUE=\"2\" />" + CRLF;
	strSend += "      <DV NAME=\"REASONCODE#1\" VALUE=\"" + strRsnCode + "\" />" + CRLF;
	strSend += "      <DV NAME=\"REASONCODECOUNT#1\" VALUE=\"" + strVNg + "\" />" + CRLF;
	strSend += "      <DV NAME=\"REASONCODE#2\" VALUE=\"BLNG\" />" + CRLF;
	strSend += "      <DV NAME=\"REASONCODECOUNT#2\" VALUE=\"" + strBNg + "\" />" + CRLF;
	strSend += "    </DVLIST>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S6F11", "20102");
}


void CHost::Set_S6F11_LotAbort(CString sLotId, CString sRecipe)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d%02d%02d%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S6F11\" NAME=\"Event Report\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <CEID NAME=\"CEID\" VALUE=\"20104\" />" + CRLF;
	strSend += "    <RPTID NAME=\"RPTID\" VALUE=\"20104\" />" + CRLF;
	strSend += "    <DVLIST COUNT=\"4\">" + CRLF;
	strSend += "      <DV NAME=\"OPERATORID\" VALUE=\"" + gData.sOperId + "\" />" + CRLF;
	strSend += "      <DV NAME=\"TIME\" VALUE=\"" + strTime + "\" />" + CRLF;
	strSend += "      <DV NAME=\"LOTID\" VALUE=\"" + sLotId + "\" />" + CRLF;
	strSend += "      <DV NAME=\"RECIPEID\" VALUE=\"" + sRecipe + "\" />" + CRLF;
	strSend += "    </DVLIST>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S6F11", "20104");
}

void CHost::Set_S6F11_IdleSet()
{
	CString strUnitNo = "1";	// 1: AVI, 4:CAP

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d%02d%02d%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S6F11\" NAME=\"Event Report\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <CEID NAME=\"CEID\" VALUE=\"50102\" />" + CRLF;
	strSend += "    <RPTID NAME=\"RPTID\" VALUE=\"50102\" />" + CRLF;
	strSend += "    <DVLIST COUNT=\"4\">" + CRLF;
	strSend += "      <DV NAME=\"OPERATORID\" VALUE=\"" + gData.sOperId + "\" />" + CRLF;
	strSend += "      <DV NAME=\"TIME\" VALUE=\"" + strTime + "\" />" + CRLF;
	strSend += "      <DV NAME=\"IDLEREASONCODE\" VALUE=\"" + gIdle.sIdleCode + "\" />" + CRLF;
	strSend += "      <DV NAME=\"UNITNO\" VALUE=\"" + strUnitNo + "\" />" + CRLF;
	strSend += "    </DVLIST>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S6F11", "50102");
}

void CHost::Set_S6F11_IdleReset()
{
	CString strUnitNo = "1";	// 1: AVI, 4:CAP

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d%02d%02d%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S6F11\" NAME=\"Event Report\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <CEID NAME=\"CEID\" VALUE=\"50103\" />" + CRLF;
	strSend += "    <RPTID NAME=\"RPTID\" VALUE=\"50103\" />" + CRLF;
	strSend += "    <DVLIST COUNT=\"4\">" + CRLF;
	strSend += "      <DV NAME=\"OPERATORID\" VALUE=\"" + gData.sOperId + "\" />" + CRLF;
	strSend += "      <DV NAME=\"TIME\" VALUE=\"" + strTime + "\" />" + CRLF;
	strSend += "      <DV NAME=\"IDLEREASONCODE\" VALUE=\"" + gIdle.sIdleCode + "\" />" + CRLF;
	strSend += "      <DV NAME=\"UNITNO\" VALUE=\"" + strUnitNo + "\" />" + CRLF;
	strSend += "    </DVLIST>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S6F11", "50103");
}

void CHost::Set_S6F11_IdleReport()
{
	CString strUnitNo = "1";	// 1: AVI, 4:CAP

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d%02d%02d%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S6F11\" NAME=\"Event Report\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <CEID NAME=\"CEID\" VALUE=\"50104\" />" + CRLF;
	strSend += "    <RPTID NAME=\"RPTID\" VALUE=\"50104\" />" + CRLF;
	strSend += "    <DVLIST COUNT=\"7\">" + CRLF;
	strSend += "      <DV NAME=\"OPERATORID\" VALUE=\"" + gData.sOperId + "\" />" + CRLF;
	strSend += "      <DV NAME=\"TIME\" VALUE=\"" + strTime + "\" />" + CRLF;
	strSend += "      <DV NAME=\"IDLEREASONCODE\" VALUE=\"" + gIdle.sIdleCode + "\" />" + CRLF;
	strSend += "      <DV NAME=\"REASONTEXT\" VALUE=\"" + gIdle.sIdleText + "\" />" + CRLF;
	strSend += "      <DV NAME=\"STARTTIME\" VALUE=\"" + gIdle.sIdleSTime + "\" />" + CRLF;
	strSend += "      <DV NAME=\"ENDTIME\" VALUE=\"" + gIdle.sIdleETime + "\" />" + CRLF;
	strSend += "      <DV NAME=\"UNITNO\" VALUE=\"" + strUnitNo + "\" />" + CRLF;
	strSend += "    </DVLIST>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S6F11", "50104");
}

//
//void CHost::Set_S6F11_PPUploadCompleted(CString sLotId, CString sMGZId, CString sRecipeId)
//{
//	SYSTEMTIME time;
//	GetLocalTime(&time);
//
//	CString strTime;
//	strTime.Format("%04d%02d%02d%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
//
//	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;
//
//	strSend += "<EIF VERSION=\"2.0\" ID=\"S6F11\" NAME=\"Event Report\">" + CRLF;
//	strSend += "  <ELEMENT>" + CRLF;
//	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
//	strSend += "  </ELEMENT>" + CRLF;
//	strSend += "  <ITEM>" + CRLF;
//	strSend += "    <CEID NAME=\"CEID\" VALUE=\"40103\" />" + CRLF;
//	strSend += "    <RPTID NAME=\"RPTID\" VALUE=\"40103\" />" + CRLF;
//	strSend += "    <DVLIST COUNT=\"5\">" + CRLF;
//	strSend += "      <DV NAME=\"TIME\" VALUE=\"" + strTime + "\" />" + CRLF;
//	strSend += "      <DV NAME=\"LOTID\" VALUE=\"" + sLotId + "\" />" + CRLF;
//	strSend += "      <DV NAME=\"MGZID\" VALUE=\"" + sMGZId + "\" />" + CRLF;
//	strSend += "      <DV NAME=\"RECIPEID\" VALUE=\"" + sRecipeId + "\" />" + CRLF;
//	strSend += "      <DV NAME=\"OPERATORID\" VALUE=\"" + gData.sOperId + "\" />" + CRLF;
//	strSend += "    </DVLIST>" + CRLF;
//	strSend += "  </ITEM>" + CRLF;
//	strSend += "</EIF>";
//
//	Send_Command(strSend, FALSE, "S6F11", "40103");
//}


void CHost::Set_S2F50_PPSelect(int nFail)
{
	CString sFail;

	sFail.Format("%d", nFail);  // 1: Fail, 0 :OK 

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S2F50\" NAME=\"Enhanced Remote Command Acknowledge\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <RCMDCP>" + CRLF;
	strSend += "      <RCMD NAME=\"RCMD\" VALUE=\"PP_SELECT\" />" + CRLF;
	strSend += "    </RCMDCP>" + CRLF;
	strSend += "    <HCACK NAME=\"HCACK\" VALUE=\""+ sFail + "\" />" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, TRUE, "S2F50", "PP_SELECT");
}

//
//void CHost::Set_S2F50_PP_UPLOAD_CONFIRM()
//{
//	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;
//
//	strSend += "<EIF VERSION=\"2.0\" ID=\"S2F50\" NAME=\"Enhanced Remote Command Acknowledge\">" + CRLF;
//	strSend += "  <ELEMENT>" + CRLF;
//	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
//	strSend += "  </ELEMENT>" + CRLF;
//	strSend += "  <ITEM>" + CRLF;
//	strSend += "    <RCMDCP>" + CRLF;
//	strSend += "      <RCMD NAME=\"RCMD\" VALUE=\"PP_UPLOAD_CONFIRM\" />" + CRLF;
//	strSend += "    </RCMDCP>" + CRLF;
//	strSend += "    <HCACK NAME=\"HCACK\" VALUE=\"0\" />" + CRLF;
//	strSend += "  </ITEM>" + CRLF;
//	strSend += "</EIF>";
//
//	Send_Command(strSend, TRUE, "S2F50", "PP_UPLOAD_CONFIRM");
//}
//
//
//void CHost::Set_S2F50_PP_UPLOAD_FAIL()
//{
//	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;
//
//	strSend += "<EIF VERSION=\"2.0\" ID=\"S2F50\" NAME=\"Enhanced Remote Command Acknowledge\">" + CRLF;
//	strSend += "  <ELEMENT>" + CRLF;
//	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
//	strSend += "  </ELEMENT>" + CRLF;
//	strSend += "  <ITEM>" + CRLF;
//	strSend += "    <RCMDCP>" + CRLF;
//	strSend += "      <RCMD NAME=\"RCMD\" VALUE=\"PP_UPLOAD_FAIL\" />" + CRLF;
//	strSend += "    </RCMDCP>" + CRLF;
//	strSend += "    <HCACK NAME=\"HCACK\" VALUE=\"0\" />" + CRLF;
//	strSend += "  </ITEM>" + CRLF;
//	strSend += "</EIF>";
//
//	Send_Command(strSend, TRUE, "S2F50", "PP_UPLOAD_FAIL");
//}


void CHost::Set_S2F50_LOT_START()
{
	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S2F50\" NAME=\"Enhanced Remote Command Acknowledge\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <RCMDCP>" + CRLF;
	strSend += "      <RCMD NAME=\"RCMD\" VALUE=\"LOT_START\" />" + CRLF;
	strSend += "    </RCMDCP>" + CRLF;
	strSend += "    <HCACK NAME=\"HCACK\" VALUE=\"0\" />" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, TRUE, "S2F50", "LOT_START");
}


void CHost::Set_S2F50_LOT_ID_FAIL()
{
	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S2F50\" NAME=\"Enhanced Remote Command Acknowledge\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <RCMDCP>" + CRLF;
	strSend += "      <RCMD NAME=\"RCMD\" VALUE=\"LOT_ID_FAIL\" />" + CRLF;
	strSend += "    </RCMDCP>" + CRLF;
	strSend += "    <HCACK NAME=\"HCACK\" VALUE=\"0\" />" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, TRUE, "S2F50", "LOT_ID_FAIL");
}





void CHost::Set_S9F13_Timeout()	// Conversation Timeout
{
	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"1.4\" ID=\"S9F13\" NAME=\"ConversationTimeout\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S9F13");
}

void CHost::Reply_HeartBeat()
{
	CString strLog, strMsg, strSendSocket;

	strSendSocket.Format("%c%08d%04d0%c", STX, 0, m_nRecvCmdCount, ETX);

	char chSend[16] = { 0 };	// 마지막 0x00
	int nLength = strSendSocket.GetLength();
	memcpy(chSend, (LPSTR)(LPCSTR)strSendSocket, nLength);

	if (!m_Server.Write_Socket(0, (BYTE*)chSend, nLength)) return;

	// Host Log //////////////////////////////////////////////////////////////////
	strLog.Format("[->] %08d%04d0", 0, m_nRecvCmdCount);
	g_objLogFile.Save_HostLog(strLog);

	strMsg.Format("%s : [HeartBeat]", strLog);
	CMesAgentDlg *pMainDlg = (CMesAgentDlg*)AfxGetMainWnd();
	pMainDlg->Set_HostMsg(strMsg);
}

///////////////////////////////////////////////////////////////////////////////

void CHost::Send_Command(CString sSend, BOOL bReply, CString sStFn, CString sRcmd)
{
	CString strLog, strMsg, strSendSocket;

	int nLen = sSend.GetLength();

	if (!bReply) m_nSendCmdCount < 9999 ? m_nSendCmdCount++ : m_nSendCmdCount = 1;
	int nCount = (bReply ? m_nRecvCmdCount : m_nSendCmdCount);

	strSendSocket.Format("%c%08d%04d1%s%c", STX, nLen, nCount, sSend, ETX);

	char chSend[8192] = { 0 };
	int nLength = strSendSocket.GetLength();
	memcpy(chSend, (LPSTR)(LPCSTR)strSendSocket, nLength);

	if (!m_Server.Write_Socket(0, (BYTE*)chSend, nLength)) return;

	// Host Log //////////////////////////////////////////////////////////////////
	strLog.Format("[->] %08d%04d1%s", nLen, nCount, sSend);
	g_objLogFile.Save_HostLog(strLog);

	strMsg.Format("%s : %s,%s", strLog.Left(18), sStFn, sRcmd);
	CMesAgentDlg *pMainDlg = (CMesAgentDlg*)AfxGetMainWnd();
	pMainDlg->Set_HostMsg(strMsg);
}

///////////////////////////////////////////////////////////////////////////////

void CHost::Test_Command()
{
	CString strFile = "Test_Command.txt";

	CFile file;
	if (!file.Open(strFile, CFile::modeRead)) { AfxMessageBox("File Open Fail."); return; }

	int nSize = (int)file.GetLength();
	char *pBuff = new char[nSize + 1];
	pBuff[nSize] = '\0';

	if (file.Read(pBuff, nSize) == 0) { AfxMessageBox("File Read Fail."); return; }
	CString strRecv = (CString)pBuff;

	file.Close();
	delete pBuff;

	CString strXml = strRecv.Right(strRecv.GetLength() - 13);
	if (!Extract_Xml(strXml)) { AfxMessageBox("Extract XML Fail."); return; }

	AfxMessageBox("Test Command Sucess.");
}


void CHost::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	for(int i = 0; i < 20; i++)
	{
		if(GetTickCount() - gMes.dwCTStart[i] > 3000 && gMes.bCTTickStarted[i])
		{
			gMes.bCTTickStarted[i] = FALSE;
			Set_S9F13_Timeout();
		}
	}


	CWnd::OnTimer(nIDEvent);
}
