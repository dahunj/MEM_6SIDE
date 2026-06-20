// Equip.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MesEmulator.h"
#include "Equip.h"
#include "LogFile.h"
#include "MesEmulatorDlg.h"


#define	EQUIP_IP	"127.0.0.1"
#define EQUIP_PORT	10001		// Equip Handler Port

const char STX = 0x02;
const char ETX = 0x03;
const CString CRLF = "\r\n";

CEquip g_objEquip;
// CEquip

IMPLEMENT_DYNAMIC(CEquip, CWnd)

CEquip::CEquip()
{
	m_bConnected = FALSE;
	m_bEquipmentOnline = FALSE;
	m_strRecvCmd = "";
}

CEquip::~CEquip()
{
}


BEGIN_MESSAGE_MAP(CEquip, CWnd)
	ON_MESSAGE(UM_CLIENT_CONNECT, OnClientConnect)
	ON_MESSAGE(UM_CLIENT_RECEIVE, OnClientReceive)
	ON_MESSAGE(UM_CLIENT_CLOSE, OnClientClose)
END_MESSAGE_MAP()



// CEquip 메시지 처리기입니다.


void CEquip::Initialize()
{
	if (m_bConnected) return;
	
	m_bConnected = m_Client.Open_Socket(EQUIP_IP, EQUIP_PORT, this);	
	Sleep(1000);

}

void CEquip::Terminate()
{
	m_bConnected = FALSE;
	m_bEquipmentOnline = FALSE;
	m_Client.Close_Socket();

	//g_objLogFile.Save_MesAgentLog("MesAgent Terminate.");	Sleep(500);
}


LRESULT CEquip::OnClientConnect(WPARAM wConnect, LPARAM lParam)
{
	m_bConnected = (BOOL)wConnect;
	if (!m_bConnected) return 0;

	//
	g_objLogFile.Save_EquipLog("Equip Connected");
	return 0;
}

LRESULT CEquip::OnClientClose(WPARAM wParam, LPARAM lParam)
{
	m_bConnected = FALSE;
	m_bEquipmentOnline = FALSE;
	m_Client.Close_Socket();
	g_objLogFile.Save_EquipLog("Equip Disconnected");
	return 0;
}

LRESULT CEquip::OnClientReceive(WPARAM wParam, LPARAM lParam)
{
	BYTE byRecv[1025] = { 0 };	// Buffer 1024, Last 0x00
	int nLen = m_Client.Read_Socket(byRecv);

	CString strRecvSocket, strLog;
	strRecvSocket.Format("%s", byRecv);
	m_strRecvCmd += strRecvSocket;

	while (!m_strRecvCmd.IsEmpty()) 
	{
		int nStart = m_strRecvCmd.Find(STX);
		int nEnd = m_strRecvCmd.Find(ETX);

		if (nEnd < 0) break;	// 버퍼에 들어오는 중...

		if (nStart < 0 || nStart > nEnd) 
		{
			strLog.Format("[OnClientReceive] <<Error>> - Start(%d), End(%d).\n%s", nStart, nEnd, m_strRecvCmd);
			g_objLogFile.Save_EquipLog(strLog);
			m_strRecvCmd.Delete(0, nEnd + 1);	// 쓰레기값이 채워져 있어서...
			continue;
		}
		
		CString strRecv = m_strRecvCmd.Mid(nStart + 1, nEnd - nStart - 1);
		m_strRecvCmd.Delete(0, nEnd + 1);

		// Inspector Log ////////////////////////////////////////////////////////////
		strLog.Format("[<-] : %s", strRecv);
		g_objLogFile.Save_EquipLog(strLog);
		/////////////////////////////////////////////////////////////////////////////

		m_nRecvCmdCount = atoi(strRecv.Mid(8, 4));	// 4Byte

		CString strXml = strRecv.Right(strRecv.GetLength() - 13);
		if (!Extract_Xml(strXml)) return 0;

	}

	return 0;
}



BOOL CEquip::Extract_Xml(CString sXmlData)
{
	int k=0;
	m_strStFn = m_strRcmd = "";	// 초기화

	CMesEmulatorDlg *pMainDlg = (CMesEmulatorDlg*)AfxGetMainWnd();
	if (!m_xml.LoadXml(sXmlData) ) 
	{
		CString strLog, strMsg;

		strMsg.Format("[Extract_Xml] CXml Data Load Fail.");
		//pMainDlg->Set_HostMsg(strMsg);

		strLog.Format("%s\n%s", strMsg, sXmlData);
		g_objLogFile.Save_EquipLog(strLog);

		return FALSE;
	}

	CXmlNode node = m_xml.GetRoot();
	m_strStFn = node.GetAttribute("ID");

	if (m_strStFn == "S6F11")
	{
		CXmlNode node = m_xml.GetRoot()->GetChild("ITEM")->GetChild("CEID");
		m_strRcmd = node.GetAttribute("VALUE", "");
		
		if(m_strRcmd == "20106")
		{
			CXmlNodes nodes = m_xml.GetRoot()->GetChild("ITEM")->GetChild("DVLIST")->GetChildren();
			int nCount = nodes.GetCount();

			//제대로 받는지 내일 해보자 
			//CString sOperID = nodes[0]->GetAttribute("VALUE", "");
			CString sLotID =  nodes[3]->GetAttribute("VALUE", "");
			CString sRecipe = nodes[4]->GetAttribute("VALUE", "");

			Set_S2F49_PP_SELECT(gData.sOperId ,sLotID, sRecipe);				
		}

		if(m_strRcmd == "40102") //PP Selected Report 
		{
			CXmlNodes nodes = m_xml.GetRoot()->GetChild("ITEM")->GetChild("DVLIST")->GetChildren();
			int nCount = nodes.GetCount();

			//제대로 받는지 내일 해보자
			gData.sHostLotID = nodes[2]->GetAttribute("VALUE","");
			gData.sHostRecipeID = nodes[3]->GetAttribute("VALUE", "");

			Set_S2F49_LOT_START();


			//Set_S7F25();
		}

		//if(m_strRcmd == "40103") //PP Upload Completed Report 
		//{
		//	CXmlNodes nodes = m_xml.GetRoot()->GetChild("ITEM")->GetChild("DVLIST")->GetChildren();
		//	int nCount = nodes.GetCount();

		//	gData.sHostLotID = nodes[1]->GetAttribute("VALUE","");
		//	gData.sHostMGZID = nodes[2]->GetAttribute("VALUE", "");
		//	gData.sHostRecipeID = nodes[3]->GetAttribute("VALUE", "");

		//	Set_S2F49_LOT_START();
		//}

		
	}	
	if(m_strStFn == "S7F26")
	{
		//Set_S2F49_PP_UPLOAD_CONFIRM();//Set_S2F49_PP_UPLOAD_FAIL();
	}

	m_xml.Close();
	return TRUE;
}




void CEquip::Set_S2F3_LINK_REQUEST()
{
	gData.sEquipId = "AVI-TEST";

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S2F3\" NAME=\"Link Test Request\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;	
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S6F12");
}



void CEquip::Set_S2F49_PP_SELECT(CString sOperID, CString sLotID, CString sRecipe)
{
	gData.sEquipId = "AVI-TEST";
	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S2F49\" NAME=\"Enhanced Remote Command\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <RCMDCP>" + CRLF;
	strSend += "      <RCMD NAME=\"RCMD\" VALUE=\"PP_SELECT\" />" + CRLF;
	strSend += "      <CPLIST COUNT=\"6\">" + CRLF;
	strSend += "        <CP>" + CRLF;
	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"TIME\" />" + CRLF;
	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"20251215000000\" />" + CRLF;
	strSend += "        </CP>" + CRLF;
	strSend += "        <CP>" + CRLF;
	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"LOTID\" />" + CRLF;
	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"TESTLOT\" />" + CRLF;
	strSend += "        </CP>" + CRLF;
	strSend += "        <CP>" + CRLF;
	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"RECIPEID\" />" + CRLF;
	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"R53B\" />" + CRLF;
	strSend += "        </CP>" + CRLF;
	strSend += "        <CP>" + CRLF;
	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"OPERATORID\" />" + CRLF;
	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\""+ sOperID + "\" />" + CRLF;
	strSend += "        </CP>" + CRLF;
	strSend += "      </CPLIST>" + CRLF;	  
	strSend += "    </RCMDCP>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S2F49");
}


//
//void CEquip::Set_S2F49_PP_UPLOAD_CONFIRM()
//{
//	gData.sEquipId = "AVI-TEST";
//	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;
//
//	strSend += "<EIF VERSION=\"2.0\" ID=\"S2F49\" NAME=\"Enhanced Remote Command\">" + CRLF;
//	strSend += "  <ELEMENT>" + CRLF;
//	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
//	strSend += "  </ELEMENT>" + CRLF;
//	strSend += "  <ITEM>" + CRLF;
//	strSend += "    <RCMDCP>" + CRLF;
//	strSend += "      <RCMD NAME=\"RCMD\" VALUE=\"PP_UPLOAD_CONFIRM\" />" + CRLF;
//	strSend += "      <CPLIST COUNT=\"5\">" + CRLF;
//	strSend += "        <CP>" + CRLF;
//	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"TIME\" />" + CRLF;
//	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"20251215000000\" />" + CRLF;
//	strSend += "        </CP>" + CRLF;
//	strSend += "        <CP>" + CRLF;
//	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"LOTID\" />" + CRLF;
//	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"" + gData.sHostLotID +"\" />" + CRLF;
//	strSend += "        </CP>" + CRLF;
//	strSend += "        <CP>" + CRLF;
//	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"MGZID\" />" + CRLF;
//	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"" + gData.sHostMGZID +"\" />" + CRLF;
//	strSend += "        </CP>" + CRLF;
//	strSend += "        <CP>" + CRLF;
//	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"RECIPEID\" />" + CRLF;
//	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\""+ gData.sHostRecipeID + "\" />" + CRLF;
//	strSend += "        </CP>" + CRLF;
//	strSend += "        <CP>" + CRLF;
//	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"OPERATORID\" />" + CRLF;
//	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\""+ gData.sHostOperID + "\" />" + CRLF;
//	strSend += "        </CP>" + CRLF;
//	strSend += "      </CPLIST>" + CRLF;	  
//	strSend += "      <RESULT>" + CRLF;
//	strSend += "        <CODE NAME=\"CODE\" VALUE=\"\" />" + CRLF;
//	strSend += "        <TEXT VALUE=\"CODE\" />" + CRLF;
//	strSend += "      </RESULT>" + CRLF;
//	strSend += "    </RCMDCP>" + CRLF;
//	strSend += "  </ITEM>" + CRLF;
//	strSend += "</EIF>";
//
//	Send_Command(strSend, FALSE, "S2F49");
//}
//
//
//
//void CEquip::Set_S2F49_PP_UPLOAD_FAIL()
//{
//	gData.sEquipId = "AVI-TEST";
//	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;
//
//	strSend += "<EIF VERSION=\"2.0\" ID=\"S2F49\" NAME=\"Enhanced Remote Command\">" + CRLF;
//	strSend += "  <ELEMENT>" + CRLF;
//	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
//	strSend += "  </ELEMENT>" + CRLF;
//	strSend += "  <ITEM>" + CRLF;
//	strSend += "    <RCMDCP>" + CRLF;
//	strSend += "      <RCMD NAME=\"RCMD\" VALUE=\"PP_UPLOAD_FAIL\" />" + CRLF;
//	strSend += "      <CPLIST COUNT=\"3\">" + CRLF;
//	strSend += "        <CP>" + CRLF;
//	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"TIME\" />" + CRLF;
//	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"20251215000000\" />" + CRLF;
//	strSend += "        </CP>" + CRLF;
//	strSend += "        <CP>" + CRLF;
//	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"LOTID\" />" + CRLF;
//	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"" + gData.sHostLotID +"\" />" + CRLF;
//	strSend += "        </CP>" + CRLF;
//	strSend += "        <CP>" + CRLF;
//	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"MGZID\" />" + CRLF;
//	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"" + gData.sHostMGZID +"\" />" + CRLF;
//	strSend += "        </CP>" + CRLF;
//	strSend += "        <CP>" + CRLF;
//	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"RECIPEID\" />" + CRLF;
//	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"" + gData.sHostRecipeID +"\" />" + CRLF;
//	strSend += "        </CP>" + CRLF;
//	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"OPERATORID\" />" + CRLF;
//	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\""+ gData.sHostOperID + "\" />" + CRLF;
//	strSend += "        </CP>" + CRLF;
//	strSend += "      </CPLIST>" + CRLF;	  
//	strSend += "      <RESULT>" + CRLF;
//	strSend += "        <CODE VALUE=\"112\" />" + CRLF;
//	strSend += "        <TEXT VALUE=\"PP_UPLOAD_FAIL\" />" + CRLF;
//	strSend += "      </RESULT>" + CRLF;
//	strSend += "    </RCMDCP>" + CRLF;
//	strSend += "  </ITEM>" + CRLF;
//	strSend += "</EIF>";
//
//	Send_Command(strSend, FALSE, "S2F49");
//}


void CEquip::Set_S2F49_LOT_START()
{
	gData.sEquipId = "AVI-TEST";
	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S2F49\" NAME=\"Enhanced Remote Command\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <RCMDCP>" + CRLF;
	strSend += "      <RCMD NAME=\"RCMD\" VALUE=\"LOT_START\" />" + CRLF;
	strSend += "      <CPLIST COUNT=\"5\">" + CRLF;
	strSend += "        <CP>" + CRLF;
	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"TIME\" />" + CRLF;
	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"20251215000000\" />" + CRLF;
	strSend += "        </CP>" + CRLF;
	strSend += "        <CP>" + CRLF;
	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"LOTID\" />" + CRLF;
	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"" + gData.sHostLotID +"\" />" + CRLF;
	strSend += "        </CP>" + CRLF;
	strSend += "        <CP>" + CRLF;
	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"RECIPEID\" />" + CRLF;
	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"" + gData.sHostRecipeID +"\" />" + CRLF;
	strSend += "        </CP>" + CRLF;
	strSend += "        <CP>" + CRLF;
	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"TOTALQTY\" />" + CRLF;
	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"99\" />" + CRLF;
	strSend += "        </CP>" + CRLF;
	strSend += "        <CP>" + CRLF;
	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"OPERATORID\" />" + CRLF;
	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\""+ gData.sHostOperID + "\" />" + CRLF;
	strSend += "        </CP>" + CRLF;
	strSend += "      </CPLIST>" + CRLF;	  
	strSend += "      <RESULT>" + CRLF;
	strSend += "        <CODE NAME=\"CODE\" VALUE=\"\" />" + CRLF;
	strSend += "        <TEXT VALUE=\"CODE\" />" + CRLF;
	strSend += "      </RESULT>" + CRLF;
	strSend += "    </RCMDCP>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S2F49");
}




void CEquip::Set_S2F49_LOT_ID_FAIL()
{
	gData.sEquipId = "AVI-TEST";
	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S2F49\" NAME=\"Enhanced Remote Command\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <RCMDCP>" + CRLF;
	strSend += "      <RCMD NAME=\"RCMD\" VALUE=\"LOT_ID_FAIL\" />" + CRLF;
	strSend += "      <CPLIST COUNT=\"5\">" + CRLF;
	strSend += "        <CP>" + CRLF;
	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"TIME\" />" + CRLF;
	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"20251215000000\" />" + CRLF;
	strSend += "        </CP>" + CRLF;
	strSend += "        <CP>" + CRLF;
	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"LOTID\" />" + CRLF;
	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"" + gData.sHostLotID +"\" />" + CRLF;
	strSend += "        </CP>" + CRLF;
	strSend += "        <CP>" + CRLF;
	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"OPERATORID\" />" + CRLF;
	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\""+ gData.sHostOperID + "\" />" + CRLF;
	strSend += "        </CP>" + CRLF;
	strSend += "        <CP>" + CRLF;
	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"RTSTID\" />" + CRLF;
	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"RTSTID\" />" + CRLF;
	strSend += "        </CP>" + CRLF;
	strSend += "        <CP>" + CRLF;
	strSend += "          <CPNAME NAME=\"CPNAME\" VALUE=\"LABELTYPE\" />" + CRLF;
	strSend += "          <CPVAL NAME=\"CPVAL\" VALUE=\"LABELTYPE\" />" + CRLF;
	strSend += "        </CP>" + CRLF;
	strSend += "      </CPLIST>" + CRLF;	  
	strSend += "      <RESULT>" + CRLF;
	strSend += "        <CODE VALUE=\"1245\" />" + CRLF;
	strSend += "        <TEXT VALUE=\"TRAY_CANCEL\" />" + CRLF;
	strSend += "      </RESULT>" + CRLF;
	strSend += "    </RCMDCP>" + CRLF;
	strSend += "  </ITEM>" + CRLF;
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S2F49");
}


void CEquip::Set_S7F25()
{
	gData.sEquipId = "AVI-TEST";

	CString strSend = "<?xml version=\"1.0\" encoding=\"utf-16\"?>" + CRLF;

	strSend += "<EIF VERSION=\"2.0\" ID=\"S7F25\" NAME=\"Formatted Process Program Request\">" + CRLF;
	strSend += "  <ELEMENT>" + CRLF;
	strSend += "    <EQPID VALUE=\"" + gData.sEquipId + "\" />" + CRLF;
	strSend += "  </ELEMENT>" + CRLF;	
	strSend += "  <ITEM>" + CRLF;
	strSend += "    <PPID NAME=\"PPID\" VALUE=\"" + gData.sHostRecipeID + "\" />" + CRLF;
	strSend += "    <LOTID NAME=\"LOTID\" VALUE=\"" + gData.sHostLotID + "\" />" + CRLF;
	strSend += "  </ITEM>" + CRLF;	
	strSend += "</EIF>";

	Send_Command(strSend, FALSE, "S7F25");
}


void CEquip::Send_Command(CString sSend, BOOL bReply, CString sStFn, CString sRcmd)
{
	CString strLog, strMsg, strSendSocket, strTemp;

	int nLen = sSend.GetLength();

	if (!bReply) m_nSendCmdCount < 9999 ? m_nSendCmdCount++ : m_nSendCmdCount = 1;
	int nCount = (bReply ? m_nRecvCmdCount : m_nSendCmdCount);

	strSendSocket.Format("%c%08d%04d1%s%c", STX, nLen, nCount, sSend, ETX);

	char chSend[2000] = { 0 };	// Max 2000
	int nLength = strSendSocket.GetLength();

	if (nLength > 2000) 
	{
		int nSendCount = strSendSocket.GetLength() / 2000 + 1;
		for (int i = 0; i < nSendCount; i++)
		{
			strTemp = strSendSocket.Mid(i * 2000, 2000);
			memset(chSend, 0x00, sizeof(char) * 2000);
			memcpy(chSend, strTemp, strTemp.GetLength());
			int nLenTemp = strTemp.GetLength();
			if (!m_Client.Write_Socket((BYTE*)chSend, nLenTemp)) return;
		}
	} 
	else
	{
		memcpy(chSend, (LPSTR)(LPCSTR)strSendSocket, nLength);
		if (!m_Client.Write_Socket((BYTE*)chSend, nLength)) return;
	}

	// Host Log //////////////////////////////////////////////////////////////////
	strLog.Format("[->] %08d%04d1%s", nLen, nCount, sSend);
	g_objLogFile.Save_EquipLog(strLog);

	strMsg.Format("%s : %s,%s", strLog.Left(18), sStFn, sRcmd);
	CMesEmulatorDlg *pMainDlg = (CMesEmulatorDlg*)AfxGetMainWnd();
	//pMainDlg->Set_HostMsg(strMsg);
}