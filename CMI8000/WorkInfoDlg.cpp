// WorkInfoDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "WorkInfoDlg.h"
#include "afxdialogex.h"

#include "AJinAXL.h"

// CWorkInfoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CWorkInfoDlg, CDialogEx)

CWorkInfoDlg::CWorkInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWorkInfoDlg::IDD, pParent)
{
}

CWorkInfoDlg::~CWorkInfoDlg()
{
}

void CWorkInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i <  3; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i <  9; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LED_BTM1_PICK_INFO_0 + i,	m_ledBtm1PickInfo[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LED_BTM2_PICK_INFO_0 + i, m_ledBtm2PickInfo[i]);
	for (int i = 0; i <  5; i++) DDX_Control(pDX, IDC_LED_SORT_PICK1_INFO_0 + i, m_ledSortPick1Info[i]);
	for (int i = 0; i <  5; i++) DDX_Control(pDX, IDC_LED_SORT_PICK2_INFO_0 + i, m_ledSortPick2Info[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LED_INSPECT1_INFO_0 + i, m_ledInspect1Info[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LED_INSPECT2_INFO_0 + i, m_ledInspect2Info[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LED_INSPECT3_INFO_0 + i, m_ledInspect3Info[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_NG_BUFFER_INFO_0 + i, m_ledNgBuffer1Info[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_NG_BUFFER_INFO_5 + i, m_ledNgBuffer2Info[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_VIS_SCAN_DONE_0 + i, m_ledVisScanDone[i]);
}

BEGIN_MESSAGE_MAP(CWorkInfoDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CWorkInfoDlg 메시지 처리기입니다.

BOOL CWorkInfoDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 180, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CWorkInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CWorkInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) SetTimer(0, 100, NULL);
	else KillTimer(0);
}

void CWorkInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(0);

	Display_Status();

	SetTimer(0, 100, NULL);
	CDialogEx::OnTimer(nIDEvent);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CWorkInfoDlg::Initial_Controls() 
{
	for (int i = 0; i <  3; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i <  9; i++) m_Label[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 10; i++) m_ledBtm1PickInfo[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 10; i++) m_ledBtm2PickInfo[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 10; i++) m_ledInspect1Info[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 10; i++) m_ledInspect2Info[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 10; i++) m_ledInspect3Info[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i <  5; i++) m_ledNgBuffer1Info[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i <  5; i++) m_ledNgBuffer2Info[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i <  5; i++) m_ledSortPick1Info[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i <  5; i++) m_ledSortPick2Info[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
 	for (int i = 0; i <  5; i++) m_ledVisScanDone[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
}

void CWorkInfoDlg::Display_Status()
{
	for (int i = 0; i < 10; i++) gData.InfoBtm1Pick[i] > 0 ? m_ledBtm1PickInfo[i].On() : m_ledBtm1PickInfo[i].Off();
	for (int i = 0; i < 10; i++) gData.InfoBtm2Pick[i] > 0 ? m_ledBtm2PickInfo[i].On() : m_ledBtm2PickInfo[i].Off();
	for (int i = 0; i < 10; i++) gData.InfoInspect[0][i] > 0 ? m_ledInspect1Info[i].On() : m_ledInspect1Info[i].Off();
	for (int i = 0; i < 10; i++) gData.InfoInspect[1][i] > 0 ? m_ledInspect2Info[i].On() : m_ledInspect2Info[i].Off();
	for (int i = 0; i < 10; i++) gData.InfoInspect[2][i] > 0 ? m_ledInspect3Info[i].On() : m_ledInspect3Info[i].Off();
	for (int i = 0; i <  5; i++) gData.InfoNgBuffer[0][i] > 0 ? m_ledNgBuffer1Info[i].On() : m_ledNgBuffer1Info[i].Off();
	for (int i = 0; i <  5; i++) gData.InfoNgBuffer[1][i] > 0 ? m_ledNgBuffer2Info[i].On() : m_ledNgBuffer2Info[i].Off();
	for (int i = 0; i <  5; i++) gData.InfoSortPick[0][i] > 0 ? m_ledSortPick1Info[i].On() : m_ledSortPick1Info[i].Off();
	for (int i = 0; i <  5; i++) gData.InfoSortPick[1][i] > 0 ? m_ledSortPick2Info[i].On() : m_ledSortPick2Info[i].Off();
	for (int i = 0; i < 5; i++) {if(i==1)continue; m_ledVisScanDone[i].Set_On(gData.bScanDone[i]);}
	m_ledVisScanDone[1].Set_On(gData.bScanDone[6]);
}

///////////////////////////////////////////////////////////////////////////////
