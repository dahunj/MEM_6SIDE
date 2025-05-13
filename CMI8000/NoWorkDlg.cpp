// NoWorkDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CMI8000.h"
#include "NoWorkDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "DataManager.h"
#include "Common.h"
#include "MESInterface.h"

// CNoWorkDlg 대화 상자입니다.
CNoWorkDlg g_dlgNoWork;


IMPLEMENT_DYNAMIC(CNoWorkDlg, CDialogEx)

CNoWorkDlg::CNoWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNoWorkDlg::IDD, pParent)
{

}

CNoWorkDlg::~CNoWorkDlg()
{
}

void CNoWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	DDX_Control(pDX, IDC_STC_NO_WORK_OP_ID, m_stcNoWorkOpId);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_NO_WORK_TIME_0 + i, m_stcNoWorkTime[i]);
	for (int i = 0; i < 24; i++) DDX_Control(pDX, IDC_BTN_STOP_REASON_0 + i, m_btnStopReason[i]);
	DDX_Control(pDX, IDC_STC_NO_WORK_EXIT, m_stcNoWorkExit);
}


BEGIN_MESSAGE_MAP(CNoWorkDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_STC_NO_WORK_OP_ID, &CNoWorkDlg::OnStnClickedStcNoWorkOpId)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_STOP_REASON_0, IDC_BTN_STOP_REASON_23, OnBtnStopReasonClick)
	ON_STN_CLICKED(IDC_STC_NO_WORK_EXIT, &CNoWorkDlg::OnStnClickedStcNoWorkExit)
END_MESSAGE_MAP()


// CNoWorkDlg 메시지 처리기입니다.

void CNoWorkDlg::Initial_Controls() 
{
	for (int i = 0; i < 3; i ++) m_Group[i].Init_Ctrl("Arial", 20, TRUE, RGB(0x10, 0x10, 0xD0), COLOR_DEFAULT);
	m_Label[0].Init_Ctrl("Arial", 30, TRUE,RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0xFF));
	for (int i = 1; i < 5; i++) m_Label[i].Init_Ctrl("Arial", 13, FALSE, RGB(0x10, 0x10, 0xD0), COLOR_DEFAULT);
	m_stcNoWorkOpId.Init_Ctrl("Segoe UI", 13, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	for (int i = 0; i < 3; i++) m_stcNoWorkTime[i].Init_Ctrl("Segoe UI", 13, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	for (int i = 0; i < 8; i++) m_btnStopReason[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0x60, 0xD0, 0x60), 0, 0);
	for (int i = 8; i < 16; i++) m_btnStopReason[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0xFF, 0x00, 0x00), 0, 0);
	for (int i = 16; i < 24; i++) m_btnStopReason[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0x60, 0xC0, 0xFF), 0, 0);
	m_stcNoWorkExit.Init_Ctrl("Arial", 12, TRUE,RGB(0xFF, 0xFF, 0xFF), RGB(0xF0, 0x60, 0xF0));
}


BOOL CNoWorkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Initial_Controls();

	if (!Read_StopLossList()) { EndDialog(IDCANCEL); return FALSE; }

	for (int i = 0; i < 24; i++) {
		m_btnStopReason[i].SetWindowText(m_strData[i][1]);
		if (m_strData[i][1] == "") m_btnStopReason[i].ShowWindow(SW_HIDE);
	}

	m_strOpId = m_strCode = m_strText = m_strNoWorkStart = "";

	return TRUE;
}


BOOL CNoWorkDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CNoWorkDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		m_strOpId = gData.sOperID;
		m_stcNoWorkOpId.SetWindowText(m_strOpId);
		m_stcNoWorkTime[0].SetWindowText(m_strNoWorkStart);
		m_strCode = "X11";
		m_strText = "Default";
		Set_NoWorkReport(TRUE);
		SetTimer(0, 500, NULL);
	} else {
		KillTimer(0);
		Set_NoWorkReport(FALSE);
		m_strOpId = m_strCode = m_strText = m_strNoWorkStart = "";
		m_stcNoWorkOpId.SetWindowText("");
		for (int i = 0; i < 3; i++)	m_stcNoWorkTime[i].SetWindowText("");
	}
}


void CNoWorkDlg::OnTimer(UINT_PTR nIDEvent)
{
	DWORD dwTotal = (GetTickCount() - m_dwNoWorkStart) / 1000;

	int nHour = dwTotal / 3600;
	int nMin  = (dwTotal - nHour * 3600) / 60;
	int nSec  = dwTotal - nHour * 3600 - nMin * 60;

	CString strTime;
	strTime.Format("%02d:%02d:%02d", nHour, nMin, nSec);
	m_stcNoWorkTime[2].SetWindowText(strTime);

	CDialogEx::OnTimer(nIDEvent);
}

void CNoWorkDlg::OnStnClickedStcNoWorkOpId()
{
	CString strKey;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;

	m_stcNoWorkOpId.SetWindowText(strKey);
	m_strOpId = strKey;
}

void CNoWorkDlg::OnBtnStopReasonClick(UINT nID)
{
	int ID = nID - IDC_BTN_STOP_REASON_0;
	if (m_strOpId == "") { AfxMessageBox("Operator ID 입력 후 선택 하십시오."); return; }

	m_strCode = m_strData[ID][0];
	m_strText = m_strData[ID][2];

	g_dlgNoWork.ShowWindow(SW_HIDE);
}

void CNoWorkDlg::OnStnClickedStcNoWorkExit()
{
	if (m_strCode == "" || m_strCode == "X11") { AfxMessageBox("비가동 사유를 선택하십시오."); return; }
	if (g_dlgNoWork.IsWindowVisible()) g_dlgNoWork.ShowWindow(SW_HIDE);
}


BOOL CNoWorkDlg::Read_StopLossList()
{
	CIniFileCS INI(gData.sEnvPath + "\\StopLoss.ini");
	if (!INI.Check_File()) { AfxMessageBox("StopLoss.ini File Not Found!!!"); return FALSE; }

	CString	strSection, strKey, strRead;
	char chSep = ',';

	for (int i = 0; i < 24; i++) {
		strSection = (i < 8 ? "PLAN" : (i < 16 ? "UNPLAN" : "IDLE"));
		strKey.Format("%02d", i % 8);
		strRead = INI.Get_String(strSection, strKey, "");
		AfxExtractSubString(m_strData[i][0], strRead, 0, chSep);
		AfxExtractSubString(m_strData[i][1], strRead, 1, chSep);
		AfxExtractSubString(m_strData[i][2], strRead, 2, chSep);
	}

	return TRUE;
}


void CNoWorkDlg::Set_NoWorkReport(BOOL bSet)
{
	CString strSet = (bSet ? "SET" : "RESET");

	DWORD dwTotal = (GetTickCount() - m_dwNoWorkStart) / 1000;
	int nHour = dwTotal / 3600;
	int nMin  = (dwTotal - nHour * 3600) / 60;
	int nSec  = dwTotal - nHour * 3600 - nMin * 60;

	CString strTime;
	strTime.Format("%02d:%02d:%02d", nHour, nMin, nSec);

	CString strLog;
	strLog.Format("Idle Report %s, ID: %s, Start Time : %s, Down Time: %s, Code: %s, Reason: %s", 
		strSet, m_strOpId, m_strNoWorkStart, strTime, m_strCode, m_strText);
	g_objLogFile.Save_HandlerLog(strLog);


	SYSTEMTIME time;
	GetLocalTime(&time);

	CString eventTime;
	eventTime.Format("%04d/%02d/%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	
	if(!bSet) g_objMES.Set_IDLEReasonReport(eventTime, m_strOpId, m_strNoWorkStart, eventTime,m_strCode );

	
}

void CNoWorkDlg::Start_NoWork(BOOL bNow)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	int nOffSet = (bNow ? 0 : pEquipData->nNoWorkTime);

	m_dwNoWorkStart = GetTickCount() - nOffSet * 1000;
	CTime CurTime = CTime::GetCurrentTime() - CTimeSpan(0, 0, 0, nOffSet);
	m_strNoWorkStart = CurTime.Format("%Y/%m/%d %H:%M:%S");

	CString strLog;
	strLog.Format("비가동 집계 시작\t%s", m_strNoWorkStart);
	g_objLogFile.Save_HandlerLog(strLog);
}