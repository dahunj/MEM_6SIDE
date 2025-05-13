// LaserComm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CMI8000.h"
#include "LaserComm.h"
#include "WorkDlg.h"

#define IL065_LASER_DISTANCE_IP		"192.168.0.110"


#define IL065_LASER_DISTANCE_PORT		64000


// CLaserComm

IMPLEMENT_DYNAMIC(CLaserComm, CWnd)

CLaserComm g_objLaserComm;

CLaserComm::CLaserComm()
{

}

CLaserComm::~CLaserComm()
{
}


BEGIN_MESSAGE_MAP(CLaserComm, CWnd)
	ON_MESSAGE(UM_TCP_RECEIVE, OnTcpReceive)
END_MESSAGE_MAP()


void CLaserComm::Initialize()
{
	m_client.Create();	
	m_client.Init(this->m_hWnd);
}

void CLaserComm::Connect()
{
	m_bConnection = m_client.Connect(IL065_LASER_DISTANCE_IP, IL065_LASER_DISTANCE_PORT);
}

void CLaserComm::Terminate()
{
	m_client.Close();

}

BOOL CLaserComm::getConnection()
{
	return m_bConnection;
}

void CLaserComm::Set_M0()
{
	CString strTemp = "M0\r\n";
	strTemp.Format("%s",strTemp);
		

	char chSend[1024] = { 0 };
	int nLength = strTemp.GetLength();
	memcpy(chSend, (LPSTR)(LPCSTR)strTemp, nLength);

	m_client.Send((BYTE*)chSend,nLength);
}

// CLaserComm 메시지 처리기입니다.

LPARAM CLaserComm::OnTcpReceive(UINT wParam, LPARAM lParam)
{
	TCHAR temp[1024];
	CString strTemp;
	memset(temp, NULL, 1024);

	m_client.Receive(temp, 1024);
	strTemp.Format("%s", temp);

	AfxExtractSubString(gData.sLaserData1,strTemp, 1, ',');
	AfxExtractSubString(gData.sLaserData2,strTemp, 2, ',');

	gData.sLaserData2.Trim("\r\n");

	gData.sLaserData1.Insert(7,".");
	gData.sLaserData2.Insert(7,".");

	gData.sLaserData1.Delete(0,4);
	gData.sLaserData2.Delete(0,4);

	g_dlgWork.PostMessage(UM_UPDATE_LASER, NULL, NULL);

	return TRUE;
}
