// NoWorkDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "NoWorkDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "DataManager.h"
#include "Common.h"
#include "MesAgent.h"

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
	DDX_Control(pDX, IDC_LBL_NO_WORK_TITLE, m_lblNoWorkTitle);
	for (int i = 0; i < 24; i++) DDX_Control(pDX, IDC_BTN_STOP_REASON_0 + i, m_btnStopReason[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_LBL_NO_WORK_TIME_0 + i, m_lblNoWorkTime[i]);
	DDX_Control(pDX, IDC_STC_NO_WORK_USER, m_stcNoWorkUser);
	DDX_Control(pDX, IDC_STC_NO_WORK_START, m_stcNoWorkStart);
	DDX_Control(pDX, IDC_STC_NO_WORK_END, m_stcNoWorkEnd);
	DDX_Control(pDX, IDC_STC_NO_WORK_TERM, m_stcNoWorkTerm);
	DDX_Control(pDX, IDC_BTN_NO_WORK_EXIT, m_btnNoWorkExit);
}

BEGIN_MESSAGE_MAP(CNoWorkDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_STOP_REASON_0, IDC_BTN_STOP_REASON_23, OnBtnStopReasonClick)
	ON_STN_CLICKED(IDC_STC_NO_WORK_USER, &CNoWorkDlg::OnStnClickedStcNoWorkUser)
	ON_BN_CLICKED(IDC_BTN_NO_WORK_EXIT, &CNoWorkDlg::OnBnClickedBtnNoWorkExit)
END_MESSAGE_MAP()

// CNoWorkDlg 메시지 처리기입니다.

void CNoWorkDlg::Initial_Controls()
{
	for (int i = 0; i < 3; i ++) m_Group[i].Init_Ctrl("Arial", 20, TRUE, RGB(0x10, 0x10, 0xD0), COLOR_DEFAULT);
	m_lblNoWorkTitle.Init_Ctrl("Arial", 30, TRUE,RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0xFF));
	for (int i = 0; i < 8; i++) m_btnStopReason[i+ 0].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0x60, 0xD0, 0x60), 0, 0);
	for (int i = 0; i < 8; i++) m_btnStopReason[i+ 8].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0xFF, 0x00, 0x00), 0, 0);
	for (int i = 0; i < 8; i++) m_btnStopReason[i+16].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0x60, 0xC0, 0xFF), 0, 0);
	for (int i = 0; i < 4; i++) m_lblNoWorkTime[i].Init_Ctrl("Arial", 13, FALSE, RGB(0x10, 0x10, 0xD0), COLOR_DEFAULT);
	m_stcNoWorkUser.Init_Ctrl("Segoe UI", 13, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	m_stcNoWorkStart.Init_Ctrl("Segoe UI", 13, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	m_stcNoWorkEnd.Init_Ctrl("Segoe UI", 13, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	m_stcNoWorkTerm.Init_Ctrl("Segoe UI", 13, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	m_btnNoWorkExit.Init_Ctrl("Arial", 12, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0xF0, 0x60, 0xF0), 0, 0);
}

BOOL CNoWorkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Initial_Controls();

	if (!Read_StopLossList()) return FALSE;

	for (int i = 0; i < 24; i++) {
		if (m_strData[i][1] == "") { m_btnStopReason[i].ShowWindow(SW_HIDE); continue; }
		m_btnStopReason[i].SetWindowText(m_strData[i][1]);
	}

	m_bNoWorkAuto = m_bNoWorkStart = FALSE;
	m_strUser = m_strCode = m_strText = m_strTimeS = m_strTimeE = "";

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
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
		m_strUser = gData.sOperID;
		m_stcNoWorkUser.SetWindowText(m_strUser);

		if (m_bNoWorkAuto) {
			m_btnNoWorkExit.EnableWindow(FALSE);
			NoWork_Start();
			SetTimer(0, 500, NULL);
		}
	}
	else
	{
		m_bNoWorkAuto = m_bNoWorkStart = FALSE;
		m_strUser = m_strCode = m_strText = m_strTimeS = m_strTimeE = "";

		m_stcNoWorkUser.SetWindowText("");
		m_stcNoWorkStart.SetWindowText("");
		m_stcNoWorkEnd.SetWindowText("");
		m_stcNoWorkTerm.SetWindowText("");

		for(int i = 0; i < 24; ++i) m_btnStopReason[i].EnableWindow(TRUE);
		m_btnNoWorkExit.EnableWindow(TRUE);
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
	m_stcNoWorkTerm.SetWindowText(strTime);

	CDialogEx::OnTimer(nIDEvent);
}

void CNoWorkDlg::OnBtnStopReasonClick(UINT nID)
{
	if (m_strUser == "") { AfxMessageBox("Operator ID 입력 후 선택 하십시오."); return; }

	if (m_bNoWorkAuto) {
		KillTimer(0);
		NoWork_End();

		int ID = nID - IDC_BTN_STOP_REASON_0;
		m_strCode = m_strData[ID][0];
		m_strText = m_strData[ID][2];
		Auto_NoWorkReport();

		g_dlgNoWork.ShowWindow(SW_HIDE);

	} else {
		if (m_bNoWorkStart) return;

		int ID = nID - IDC_BTN_STOP_REASON_0;
		for(int i = 0; i < 24; ++i) m_btnStopReason[i].EnableWindow(i == ID);

		NoWork_Start();
		m_strCode = m_strData[ID][0];
		m_strText = m_strData[ID][2];
		Set_NoWorkReport();

		SetTimer(0, 500, NULL);
	}
}

void CNoWorkDlg::OnStnClickedStcNoWorkUser()
{
	CString strKey;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;
	m_strUser = strKey;
	m_stcNoWorkUser.SetWindowText(strKey);
}

void CNoWorkDlg::OnBnClickedBtnNoWorkExit()
{
	if (m_bNoWorkStart) {
		KillTimer(0);
		NoWork_End();
		Reset_NoWorkReport();
	}
	g_dlgNoWork.ShowWindow(SW_HIDE);
}

///////////////////////////////////////////////////////////////////////////////

BOOL CNoWorkDlg::Read_StopLossList()
{
	CIniFileCS INI(gsCurrentDir + "\\System\\StopLoss.ini");
	if (!INI.Check_File()) { AfxMessageBox("StopLoss.ini File Not Found!!!"); return FALSE; }

	CString	strSection, strKey, strRead;
	char chSep = ',';

	for (int i = 0; i < 24; i++) {
		strSection = (i < 8) ? "PLAN" : (i < 16) ? "UNPLAN" : "IDLE";
		strKey.Format("%02d", i % 8);
		strRead = INI.Get_String(strSection, strKey, "");
		AfxExtractSubString(m_strData[i][0], strRead, 0, chSep);
		AfxExtractSubString(m_strData[i][1], strRead, 1, chSep);
		AfxExtractSubString(m_strData[i][2], strRead, 2, chSep);
	}

	return TRUE;
}

void CNoWorkDlg::Set_NoWorkReport()
{
	CString strLog;
	g_objMesAgent.Set_IdleSet(m_strUser, m_strCode);
	strLog.Format("비가동 집계 시작 : %s, %s, %s, %s", m_strTimeS, m_strUser, m_strCode, m_strText);
	g_objLogFile.Save_HandlerLog(strLog);
}

void CNoWorkDlg::Reset_NoWorkReport()
{
	CString strLog;
	g_objMesAgent.Set_IdleReset(m_strUser, m_strCode);
	strLog.Format("비가동 집계 종료 : %s, %s, %s, %s", m_strTimeE, m_strUser, m_strCode, m_strText);
	g_objLogFile.Save_HandlerLog(strLog);
}

void CNoWorkDlg::Auto_NoWorkReport()
{
	CString strLog;
	g_objMesAgent.Set_IdleReport(m_strUser, m_strCode, m_strText, m_strTimeS, m_strTimeE);
	strLog.Format("비가동 집계 Auto : %s, %s, %s, %s", m_strTimeS, m_strTimeE, m_strUser, m_strCode, m_strText);
	g_objLogFile.Save_HandlerLog(strLog);
}

void CNoWorkDlg::NoWork_Start()
{
	m_bNoWorkStart = TRUE;
	m_dwNoWorkStart = GetTickCount();

	CTime CurTime = CTime::GetCurrentTime();
	m_strTimeS = CurTime.Format("%Y%m%d%H%M%S");
	m_stcNoWorkStart.SetWindowText(CurTime.Format("%Y-%m-%d %H:%M:%S"));
}

void CNoWorkDlg::NoWork_End()
{
	m_bNoWorkStart = FALSE;

	CTime CurTime = CTime::GetCurrentTime();
	m_strTimeE = CurTime.Format("%Y%m%d%H%M%S");
	m_stcNoWorkEnd.SetWindowText(CurTime.Format("%Y-%m-%d %H:%M:%S"));
}

///////////////////////////////////////////////////////////////////////////////
