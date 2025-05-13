// ManualNgDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "ManualUnloadDlg.h"
#include "afxdialogex.h"
#include "math.h"

#include "LogFile.h"
#include "AJinAXL.h"
#include "DataManager.h"
#include "Common.h"

// CManualNgDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualUnloadDlg, CDialogEx)

CManualUnloadDlg::CManualUnloadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualUnloadDlg::IDD, pParent)
{
}

CManualUnloadDlg::~CManualUnloadDlg()
{
}

void CManualUnloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 17; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_STC_AXIS_POS_0 + i, m_stcAxisPos[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_BTN_GOOD_STAGE1_Y_0 + i, m_btnGoodStage1Y[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_BTN_GOOD_STAGE1_Z_0 + i, m_btnGoodStage1Z[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_BTN_GOOD_STAGE1_IO_0 + i, m_btnGoodStage1Io[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_GOOD_STAGE1_IO_0 + i, m_ledGoodStage1Io[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_BTN_GOOD_STAGE2_Y_0 + i, m_btnGoodStage2Y[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_BTN_GOOD_STAGE2_Z_0 + i, m_btnGoodStage2Z[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_BTN_GOOD_STAGE2_IO_0 + i, m_btnGoodStage2Io[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_GOOD_STAGE2_IO_0 + i, m_ledGoodStage2Io[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_BTN_GOOD_PORT_IO_0 + i, m_btnGoodPortIo[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_LED_GOOD_PORT_IO_0 + i, m_ledGoodPortIo[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_BTN_NG_STAGE_Y_0 + i, m_btnNgStageY[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_LED_NG_STAGE_IO_0 + i, m_ledNgStageIo[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_BTN_NG_PORT_IO_0 + i, m_btnNgPortIo[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_NG_PORT_IO_0 + i, m_ledNgPortIo[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_BTN_EMPTY_TRANS1_X_0 + i, m_btnEmptyTrans1X[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_BTN_EMPTY_TRANS1_IO_0 + i, m_btnEmptyTrans1Io[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_LED_EMPTY_TRANS1_IO_0 + i, m_ledEmptyTrans1Io[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_BTN_EMPTY_PORT_Z_0 + i, m_btnEmptyPortZ[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_EMPTY_PORT_IO_0 + i, m_ledEmptyPortIo[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_BTN_EMPTY_TRANS2_Y_0 + i, m_btnEmptyTrans2Y[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_BTN_EMPTY_TRANS2_IO_0 + i, m_btnEmptyTrans2Io[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_LED_EMPTY_TRANS2_IO_0 + i, m_ledEmptyTrans2Io[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_BTN_GOOD_BUFFER_IO_0 + i, m_btnGoodTrayBufferIo[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_GOOD_BUFFER_IO_0 + i, m_ledGoodTrayBufferIo[i]);
	for (int i = 0; i < 24; i++) DDX_Control(pDX, IDC_BTN_NG_BUFFER_IO_0 + i, m_btnNgBufferIo[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LED_NG_BUFFER_IO_0 + i, m_ledNgBufferIo[i]);

	
	DDX_Control(pDX, IDC_RDO_SKIPON, m_rdoSkipOn);
	DDX_Control(pDX, IDC_RDO_SKIPOFF, m_rdoSkipOff);
}

BEGIN_MESSAGE_MAP(CManualUnloadDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_GOOD_STAGE1_Y_0, IDC_BTN_GOOD_STAGE1_Y_5, OnBtnGoodStage1YClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_GOOD_STAGE1_Z_0, IDC_BTN_GOOD_STAGE1_Z_4, OnBtnGoodStage1ZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_GOOD_STAGE1_IO_0, IDC_BTN_GOOD_STAGE1_IO_3, OnBtnGoodStage1IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_GOOD_STAGE2_Y_0, IDC_BTN_GOOD_STAGE2_Y_5, OnBtnGoodStage2YClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_GOOD_STAGE2_Z_0, IDC_BTN_GOOD_STAGE2_Z_4, OnBtnGoodStage2ZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_GOOD_STAGE2_IO_0, IDC_BTN_GOOD_STAGE2_IO_3, OnBtnGoodStage2IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_GOOD_PORT_IO_0, IDC_BTN_GOOD_PORT_IO_1, OnBtnGoodPortIoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_NG_STAGE_Y_0, IDC_BTN_NG_STAGE_Y_4, OnBtnNgStageYClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_NG_PORT_IO_0, IDC_BTN_NG_PORT_IO_1, OnBtnNgPortIoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_EMPTY_TRANS1_X_0, IDC_BTN_EMPTY_TRANS1_X_1, OnBtnEmptyTrans1XClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_EMPTY_TRANS1_IO_0, IDC_BTN_EMPTY_TRANS1_IO_5, OnBtnEmptyTrans1IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_EMPTY_PORT_Z_0, IDC_BTN_EMPTY_PORT_Z_7, OnBtnEmptyPortZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_EMPTY_TRANS2_Y_0, IDC_BTN_EMPTY_TRANS2_Y_1, OnBtnEmptyTrans2YClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_EMPTY_TRANS2_IO_0, IDC_BTN_EMPTY_TRANS2_IO_5, OnBtnEmptyTrans2IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_GOOD_BUFFER_IO_0, IDC_BTN_GOOD_BUFFER_IO_1, OnBtnGoodBufferIoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_NG_BUFFER_IO_0, IDC_BTN_NG_BUFFER_IO_23, OnBtnNgBufferIoClick)

	ON_BN_CLICKED(IDC_RDO_SKIPON, &CManualUnloadDlg::OnBnClickedRdoSkipon)
	ON_BN_CLICKED(IDC_RDO_SKIPOFF, &CManualUnloadDlg::OnBnClickedRdoSkipoff)
END_MESSAGE_MAP()

// CManualUnloadDlg 메시지 처리기입니다.

BOOL CManualUnloadDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CManualUnloadDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CManualUnloadDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) {
		g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z);
		return;
	}

// 	if (gData.bR13A) {
// 		m_btnNgBufferIo[1].EnableWindow(FALSE);
// 		m_btnNgBufferIo[3].EnableWindow(FALSE);
// 		m_btnNgBufferIo[6].EnableWindow(FALSE);
// 		m_btnNgBufferIo[8].EnableWindow(FALSE);
// 
// 		m_btnNgBufferIo[11].EnableWindow(FALSE);
// 		m_btnNgBufferIo[13].EnableWindow(FALSE);
// 		m_btnNgBufferIo[16].EnableWindow(FALSE);
// 		m_btnNgBufferIo[18].EnableWindow(FALSE);
// 
// 	} else {
// 		m_btnNgBufferIo[1].EnableWindow(TRUE);
// 		m_btnNgBufferIo[3].EnableWindow(TRUE);
// 		m_btnNgBufferIo[6].EnableWindow(TRUE);
// 		m_btnNgBufferIo[8].EnableWindow(TRUE);
// 
// 		m_btnNgBufferIo[11].EnableWindow(FALSE);
// 		m_btnNgBufferIo[13].EnableWindow(FALSE);
// 		m_btnNgBufferIo[16].EnableWindow(FALSE);
// 		m_btnNgBufferIo[18].EnableWindow(FALSE);
// 	}

	Display_Status();

	m_strLog.Format("[Manual Unload] Show Window");
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::Initial_Controls() 
{
	for (int i = 0; i < 7; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x10, 0x10, 0xD0), COLOR_DEFAULT);
	for (int i = 7; i < 9; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0x10), COLOR_DEFAULT);
	for (int i = 9; i < 11; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x30, 0x80, 0x00), COLOR_DEFAULT);
	for (int i = 11; i < 13; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0xD0), COLOR_DEFAULT);
	for (int i = 13; i < 15; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x30, 0x80, 0x00), COLOR_DEFAULT);
	for (int i = 15; i < 16; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x10, 0x10, 0xD0), COLOR_DEFAULT);
	for (int i = 16; i < 17; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0x10), COLOR_DEFAULT);
	for (int i = 0; i < 8; i++) m_Label[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, RGB(0xB0, 0xB0, 0xB0));
	for (int i = 0; i < 8; i++) m_stcAxisPos[i].Init_Ctrl("Arial", 10, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x10, 0x10, 0x60));
	for (int i = 0; i < 6; i++) m_btnGoodStage1Y[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 5; i++) m_btnGoodStage1Z[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 4; i++) m_btnGoodStage1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 5; i++) m_ledGoodStage1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 6; i++) m_btnGoodStage2Y[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 5; i++) m_btnGoodStage2Z[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 4; i++) m_btnGoodStage2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 5; i++) m_ledGoodStage2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 2; i++) m_btnGoodPortIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 8; i++) m_ledGoodPortIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 5; i++) m_btnNgStageY[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 6; i++) m_ledNgStageIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 2; i++) m_btnNgPortIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 5; i++) m_ledNgPortIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 2; i++) m_btnEmptyTrans1X[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 6; i++) m_btnEmptyTrans1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 7; i++) m_ledEmptyTrans1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 8; i++) m_btnEmptyPortZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 5; i++) m_ledEmptyPortIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 2; i++) m_btnEmptyTrans2Y[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 6; i++) m_btnEmptyTrans2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 7; i++) m_ledEmptyTrans2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 2; i++) m_btnGoodTrayBufferIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 5; i++) m_ledGoodTrayBufferIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 24; i++) m_btnNgBufferIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 10; i++) m_ledNgBufferIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
}

void CManualUnloadDlg::Display_Status()
{
	CString strPos;
	for (int i = 0; i < 8; i++) {
		double dPos = g_objAJinAXL.Get_Position(AX_GOOD_STAGE1_Y + i);
		strPos.Format("%0.3lf", dPos);
		m_stcAxisPos[i].SetWindowText(strPos);
	}

	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09();
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();
	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();
	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();

	m_ledGoodStage1Io[0].Set_On(pDX12->iGoodStage1Exist);
	m_ledGoodStage1Io[1].Set_On(pDX12->iGoodStage1MasterIn);
	m_ledGoodStage1Io[2].Set_On(pDX12->iGoodStage1MasterOut);
	m_ledGoodStage1Io[3].Set_On(pDX12->iGoodStage1SlaveIn);
	m_ledGoodStage1Io[4].Set_On(pDX12->iGoodStage1SlaveOut);

	m_ledGoodStage2Io[0].Set_On(pDX12->iGoodStage2Exist);
	m_ledGoodStage2Io[1].Set_On(pDX12->iGoodStage2MasterIn);
	m_ledGoodStage2Io[2].Set_On(pDX12->iGoodStage2MasterOut);
	m_ledGoodStage2Io[3].Set_On(pDX12->iGoodStage2SlaveIn);
	m_ledGoodStage2Io[4].Set_On(pDX12->iGoodStage2SlaveOut);

	m_ledGoodPortIo[0].Set_On(pDX12->iGoodPortUpper);
	m_ledGoodPortIo[1].Set_On(pDX12->iGoodPortLower);
	m_ledGoodPortIo[2].Set_On(pDX12->iGoodPortBottom);
	m_ledGoodPortIo[3].Set_On(pDX12->iGoodPortSlideLock);
	m_ledGoodPortIo[4].Set_On(pDX12->iGoodPortSlideUnlock);
	m_ledGoodPortIo[5].Set_On(pDX12->iGoodPortSlideClose);
	m_ledGoodPortIo[6].Set_On(pDX12->iGoodPortSlideOpen);
	m_ledGoodPortIo[7].Set_On(pDX12->iGoodPortAreaCheck);

	m_ledNgStageIo[0].Set_On(!pDX09->iNgStageExist1);
	m_ledNgStageIo[1].Set_On(!pDX09->iNgStageExist2);
	m_ledNgStageIo[2].Set_On(!pDX09->iNgStageExist3);
	m_ledNgStageIo[3].Set_On(!pDX09->iNgStageExist4);

	m_ledNgPortIo[0].Set_On(pDX09->iNgPortSlideLock);
	m_ledNgPortIo[1].Set_On(pDX09->iNgPortSlideUnlock);
	m_ledNgPortIo[2].Set_On(pDX09->iNgPortSlideClose);
	m_ledNgPortIo[3].Set_On(pDX09->iNgPortSlideOpen);
	m_ledNgPortIo[4].Set_On(pDX09->iNgPortAreaCheck);

	m_ledEmptyTrans1Io[0].Set_On(pDX13->iEmptyTrans1Exist);
	m_ledEmptyTrans1Io[1].Set_On(pDX13->iEmptyTrans1Up);
	m_ledEmptyTrans1Io[2].Set_On(pDX13->iEmptyTrans1Down);
	m_ledEmptyTrans1Io[3].Set_On(pDX13->iEmptyTrans1MasterIn);
	m_ledEmptyTrans1Io[4].Set_On(pDX13->iEmptyTrans1MasterOut);
	m_ledEmptyTrans1Io[5].Set_On(pDX13->iEmptyTrans1SlaveIn);
	m_ledEmptyTrans1Io[6].Set_On(pDX13->iEmptyTrans1SlaveOut);

#ifdef EDITION_2ND
	m_ledEmptyPortIo[0].Set_On(pDX01->iEmptyPortExist);
	m_ledEmptyPortIo[1].Set_On(pDX01->iEmptyPortSlideClose);
	m_ledEmptyPortIo[2].Set_On(pDX01->iEmptyPortSlideOpen);
	m_ledEmptyPortIo[3].Set_On(pDX01->iEmptyPortTopCheck);
	m_ledEmptyPortIo[4].Set_On(pDX01->iEmptyPortAreaCheck);
#else
	m_ledEmptyPortIo[0].Set_On(pDX07->iEmptyPortExist);
	m_ledEmptyPortIo[1].Set_On(pDX07->iEmptyPortSlideClose);
	m_ledEmptyPortIo[2].Set_On(pDX07->iEmptyPortSlideOpen);
	m_ledEmptyPortIo[3].Set_On(pDX07->iEmptyPortTopCheck);
	m_ledEmptyPortIo[4].Set_On(pDX07->iEmptyPortAreaCheck);
#endif

	m_ledEmptyTrans2Io[0].Set_On(pDX13->iEmptyTrans2Exist);
	m_ledEmptyTrans2Io[1].Set_On(pDX13->iEmptyTrans2Up);
	m_ledEmptyTrans2Io[2].Set_On(pDX13->iEmptyTrans2Down);
	m_ledEmptyTrans2Io[3].Set_On(pDX13->iEmptyTrans2MasterIn);
	m_ledEmptyTrans2Io[4].Set_On(pDX13->iEmptyTrans2MasterOut);
	m_ledEmptyTrans2Io[5].Set_On(pDX13->iEmptyTrans2SlaveIn);
	m_ledEmptyTrans2Io[6].Set_On(pDX13->iEmptyTrans2SlaveOut);

	m_ledGoodTrayBufferIo[0].Set_On(pDX12->iGoodTrayBufferBottom);
	m_ledGoodTrayBufferIo[1].Set_On(pDX12->iGoodTrayBuffSupport1In);
	m_ledGoodTrayBufferIo[2].Set_On(pDX12->iGoodTrayBuffSupport2In);
	m_ledGoodTrayBufferIo[3].Set_On(pDX12->iGoodTrayBuffSupport1Out);
	m_ledGoodTrayBufferIo[4].Set_On(pDX12->iGoodTrayBuffSupport2Out);

	m_ledNgBufferIo[0].Set_On(pDX10->iNgBufferVac01);
	m_ledNgBufferIo[1].Set_On(pDX10->iNgBufferVac02);
	m_ledNgBufferIo[2].Set_On(pDX10->iNgBufferVac03);
	m_ledNgBufferIo[3].Set_On(pDX10->iNgBufferVac04);
// 	m_ledNgBufferIo[4].Set_On(pDX10->i1031);
	m_ledNgBufferIo[5].Set_On(pDX11->iNgBufferVac05);
	m_ledNgBufferIo[6].Set_On(pDX11->iNgBufferVac06);
	m_ledNgBufferIo[7].Set_On(pDX11->iNgBufferVac07);
	m_ledNgBufferIo[8].Set_On(pDX11->iNgBufferVac08);
// 	m_ledNgBufferIo[9].Set_On(pDX11->i1131);

}

///////////////////////////////////////////////////////////////////////////////

void CManualUnloadDlg::OnBtnGoodStage1YClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_GOOD_STAGE1_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_GOOD_STAGE1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_GOOD_STAGE2_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_EMPTY_TRANS2_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_GOOD_STAGE1_Y_0;

	if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 0)) {			// Moving Down
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 0)) { AfxMessageBox("Good Stage2 Z축 위치 확인 후 진행하세요."); return; }
		if (nIndex < 4) g_objCommon.Move_Position(AX_GOOD_STAGE1_Y, nIndex);	// Load, Sort1, Sort2, Unload

	} else if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1)) {	// Moving Up
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1)) { AfxMessageBox("Good Stage2 Z축 위치 확인 후 진행하세요."); return; }

		DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0) && !pDX13->iEmptyTrans2Up && pDX13->iEmptyTrans2Down) {
			AfxMessageBox("Empty Trans2 Up 상태 확인 후 진행하세요."); return;
		}

		if (!g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER1_X, 1)) {
			AfxMessageBox("Sort Picker1 Z Up 위치 확인 후 진행하세요."); return;
		}

		if (!g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER2_X, 1)) {
			AfxMessageBox("Sort Picker2 Z Up 위치 확인 후 진행하세요."); return;
		}

		if (nIndex == 4 || nIndex == 5) {	// Pitch Move
			EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
			int nDir = (nIndex == 4 ? 1 : -1);
			double dMove = pEquipData->dTrayPitchY * nDir;
			g_objAJinAXL.Move_Relative(AX_GOOD_STAGE1_Y, dMove);
		} else {
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Y, nIndex);	// Load, Sort1, Sort2, Unload
		}
	} else {
		AfxMessageBox("Good Stage1 Z축 위치 확인 후 진행하세요."); return;
	}

	m_strLog.Format("[Manual Unload] Good Stage1 Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnGoodStage1ZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_GOOD_STAGE1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_GOOD_STAGE1_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_GOOD_STAGE2_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_GOOD_STAGE2_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_GOOD_STAGE1_Z_0;

	int nPosY1 = -1, nPosY2 = -1;
	for (int i = 0; i < 4; i++) { if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, i)) { nPosY1 = i; break; } }
	for (int i = 0; i < 4; i++) { if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, i)) { nPosY2 = i; break; } }

	if (nPosY1 == 0 && nPosY2 == 0) { AfxMessageBox("Good Stage2 Y축 위치 확인 후 진행하세요."); return; }
	if ((nPosY1 == 1 || nPosY1 == 2) && (nPosY2 == 1 || nPosY2 == 2)) { AfxMessageBox("Good Stage2 Y축 위치 확인 후 진행하세요."); return; }
	if (nPosY1 == 3 && nPosY2 == 3) { AfxMessageBox("Good Stage2 Y축 위치 확인 후 진행하세요."); return; }

	if ((nIndex == 2 || nIndex == 4) && nPosY1 != 0) { AfxMessageBox("Good Stage 1 Y축이 Load 위치인지 확인 후 진행하세요."); return; }
	if (nIndex == 3 && nPosY1 != 3) { AfxMessageBox("Good Stage 1 Y축이 Unload 위치인지 확인 후 진행하세요."); return; }

	double dY1 = g_objAJinAXL.Get_Position(AX_GOOD_STAGE1_Y);
	double dY2 = g_objAJinAXL.Get_Position(AX_GOOD_STAGE2_Y);
	if (fabs(dY1 - dY2) < 280.0) { AfxMessageBox("Good Stage 2 Y축 위치 확인 후 진행하세요."); return; }
	
	g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, nIndex);	// Moving Down, Moving Up, Load Up, Unload Up

	m_strLog.Format("[Manual Unload] Good Stage1 Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnGoodStage1IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_GOOD_STAGE1_IO_0;

	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();

	if (nIndex == 0) pDY12->oGoodStage1MasterIn = TRUE;
	if (nIndex == 1) pDY12->oGoodStage1MasterIn = FALSE;
	if (nIndex == 2) pDY12->oGoodStage1SlaveIn = TRUE;
	if (nIndex == 3) pDY12->oGoodStage1SlaveIn = FALSE;

	g_objAJinAXL.Write_Output(12);

	m_strLog.Format("[Manual Unload] Good Stage1 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnGoodStage2YClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_GOOD_STAGE2_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_GOOD_STAGE2_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_GOOD_STAGE1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_EMPTY_TRANS2_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_GOOD_STAGE2_Y_0;

	if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 0)) {			// Moving Down
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 0)) { AfxMessageBox("Good Stage1 Z축 위치 확인 후 진행하세요."); return; }
		if (nIndex < 4) g_objCommon.Move_Position(AX_GOOD_STAGE2_Y, nIndex);	// Load, Sort1, Sort2, Unload

	} else if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1)) {	// Moving Up
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1)) { AfxMessageBox("Good Stage1 Z축 위치 확인 후 진행하세요."); return; }

		DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0) && !pDX13->iEmptyTrans2Up && pDX13->iEmptyTrans2Down) {
			AfxMessageBox("Empty Trans2 Up 상태 확인 후 진행하세요."); return;
		}

		if (!g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER1_X, 1)) {
			AfxMessageBox("Sort Picker1 Z Up 위치 확인 후 진행하세요."); return;
		}

		if (!g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER2_X, 1)) {
			AfxMessageBox("Sort Picker2 Z Up 위치 확인 후 진행하세요."); return;
		}

		if (nIndex == 4 || nIndex == 5) {	// Pitch Move
			EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
			int nDir = (nIndex == 4 ? 1 : -1);
			double dMove = pEquipData->dTrayPitchY * nDir;
			g_objAJinAXL.Move_Relative(AX_GOOD_STAGE2_Y, dMove);
		} else {
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Y, nIndex);	// Load, Sort1, Sort2, Unload
		}
	} else {
		AfxMessageBox("Good Stage2 Z축 위치 확인 후 진행하세요."); return;
	}

	m_strLog.Format("[Manual Unload] Good Stage2 Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnGoodStage2ZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_GOOD_STAGE2_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_GOOD_STAGE2_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_GOOD_STAGE1_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_GOOD_STAGE1_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_GOOD_STAGE2_Z_0;

	int nPosY2 = -1, nPosY1 = -1;
	for (int i = 0; i < 4; i++) { if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, i)) { nPosY2 = i; break; } }
	for (int i = 0; i < 4; i++) { if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, i)) { nPosY1 = i; break; } }

	if (nPosY2 == 0 && nPosY1 == 0) { AfxMessageBox("Good Stage1 Y축 위치 확인 후 진행하세요."); return; }
	if ((nPosY2 == 1 || nPosY2 == 2) && (nPosY1 == 1 || nPosY1 == 2)) { AfxMessageBox("Good Stage1 Y축 위치 확인 후 진행하세요."); return; }
	if (nPosY2 == 3 && nPosY1 == 3) { AfxMessageBox("Good Stage1 Y축 위치 확인 후 진행하세요."); return; }

	if ((nIndex == 2 || nIndex == 4) && nPosY2 != 0) { AfxMessageBox("Good Stage 2 Y축이 Load 위치인지 확인 후 진행하세요."); return; }
	if (nIndex == 3 && nPosY2 != 3) { AfxMessageBox("Good Stage 2 Y축이 Unload 위치인지 확인 후 진행하세요."); return; }

	double dY2 = g_objAJinAXL.Get_Position(AX_GOOD_STAGE2_Y);
	double dY1 = g_objAJinAXL.Get_Position(AX_GOOD_STAGE1_Y);
	if (fabs(dY2 - dY1) < 280.0) { AfxMessageBox("Good Stage 1 Y축 위치 확인 후 진행하세요."); return; }

	g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, nIndex);	// Moving Down, Moving Up, Load Up, Unload Up

	m_strLog.Format("[Manual Unload] Good Stage2 Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnGoodStage2IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_GOOD_STAGE2_IO_0;

	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();

	if (nIndex == 0) pDY12->oGoodStage2MasterIn = TRUE;
	if (nIndex == 1) pDY12->oGoodStage2MasterIn = FALSE;
	if (nIndex == 2) pDY12->oGoodStage2SlaveIn = TRUE;
	if (nIndex == 3) pDY12->oGoodStage2SlaveIn = FALSE;

	g_objAJinAXL.Write_Output(12);

	m_strLog.Format("[Manual Unload] Good Stage2 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnGoodPortIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_GOOD_PORT_IO_0;

	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();

	if (nIndex == 0) { pDY12->oGoodPortSlideLock = TRUE;  pDY12->oGoodPortSlideUnlock = FALSE; }
	if (nIndex == 1) { pDY12->oGoodPortSlideLock = FALSE; pDY12->oGoodPortSlideUnlock = TRUE; }

	g_objAJinAXL.Write_Output(12);

	m_strLog.Format("[Manual Unload] Good Port IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnNgStageYClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_NG_STAGE_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_NG_STAGE_Y_0;

	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();

	double dX1 = g_objAJinAXL.Get_Position(AX_SORT_PICKER1_X);
	if (!g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && (dX1 < pMoveData->dSortPicker1X[0] - 1)) {
		AfxMessageBox("Sort Picker1 Z Up 위치 확인 후 진행하세요."); return;
	}

	double dX2 = g_objAJinAXL.Get_Position(AX_SORT_PICKER2_X);
	if (!g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && (dX2 < pMoveData->dSortPicker2X[0] - 1)) {
		AfxMessageBox("Sort Picker2 Z Up 위치 확인 후 진행하세요."); return;
	}

	g_objCommon.Move_Position(AX_NG_STAGE_Y, nIndex);	// Load, Sort1, Sort2, Unload

	m_strLog.Format("[Manual Unload] NG Stage Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnNgPortIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_NG_PORT_IO_0;

	DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();

	if (nIndex == 0) { pDY09->oNgPortSlideLock = TRUE;  pDY09->oNgPortSlideUnlock = FALSE; }
	if (nIndex == 1) { pDY09->oNgPortSlideLock = FALSE; pDY09->oNgPortSlideUnlock = TRUE; }

	g_objAJinAXL.Write_Output(9);

	m_strLog.Format("[Manual Unload] NG Port IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnEmptyTrans1XClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_EMPTY_TRANS1_X)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_EMPTY_TRANS1_X_0;

	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();
	if (!pDX13->iEmptyTrans1Up || pDX13->iEmptyTrans1Down) { AfxMessageBox("Empty Trans1 Up 상태 확인 후 진행하세요."); return; }

	g_objCommon.Move_Position(AX_EMPTY_TRANS1_X, nIndex);

	m_strLog.Format("[Manual Unload] Empty Trans1 X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnEmptyTrans1IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_EMPTY_TRANS1_IO_0;

	DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();

	if (nIndex == 0) { pDY13->oEmptyTrans1Up = TRUE;        pDY13->oEmptyTrans1Down = FALSE; }
	if (nIndex == 1) { pDY13->oEmptyTrans1Up = FALSE;       pDY13->oEmptyTrans1Down = TRUE; }
	if (nIndex == 2) { pDY13->oEmptyTrans1MasterIn = TRUE;  pDY13->oEmptyTrans1MasterOut = FALSE; }
	if (nIndex == 3) { pDY13->oEmptyTrans1MasterIn = FALSE; pDY13->oEmptyTrans1MasterOut = TRUE; }
	if (nIndex == 4) { pDY13->oEmptyTrans1SlaveIn = TRUE;   pDY13->oEmptyTrans1SlaveOut = FALSE; }
	if (nIndex == 5) { pDY13->oEmptyTrans1SlaveIn = FALSE;  pDY13->oEmptyTrans1SlaveOut = TRUE; }

	g_objAJinAXL.Write_Output(13);

	m_strLog.Format("[Manual Unload] Empty Trans1 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnEmptyPortZClick(UINT nID)
{
	int nIndex = nID - IDC_BTN_EMPTY_PORT_Z_0;

	if (nIndex != 5 && !g_objAJinAXL.Is_Home(AX_EMPTY_PORT_Z)) return;
	if (nIndex != 5 && !g_objCommon.Check_MainDoor()) return;

	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (nIndex == 0 || nIndex == 1 || nIndex == 2) {	// Base Down, Ready Down, Limit Up
		g_objCommon.Move_Position(AX_EMPTY_PORT_Z, nIndex);
	}
	else if (nIndex == 3) {		// Slow Up
#ifdef EDITION_2ND
		if (pDX01->iEmptyPortTopCheck) { AfxMessageBox("이미 Top Check 센서를 감지하고 있습니다."); return; }

		CWaitCursor wait;

		g_objAJinAXL.Move_AbsSlow(AX_EMPTY_PORT_Z, pMoveData->dEmptyPortZ[2]);

		DWORD dwStart = GetTickCount();
		while (!pDX01->iEmptyPortTopCheck) {
			if (GetTickCount() - dwStart > 10000) { g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z); AfxMessageBox("Time Over."); wait.Restore(); return; }
			theApp.DoEvents();
		}

		wait.Restore();
#else
		if (pDX07->iEmptyPortTopCheck) { AfxMessageBox("이미 Top Check 센서를 감지하고 있습니다."); return; }

		CWaitCursor wait;

		g_objAJinAXL.Move_AbsSlow(AX_EMPTY_PORT_Z, pMoveData->dEmptyPortZ[2]);

		DWORD dwStart = GetTickCount();
		while (!pDX07->iEmptyPortTopCheck) {
			if (GetTickCount() - dwStart > 10000) { g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z); AfxMessageBox("Time Over."); wait.Restore(); return; }
			theApp.DoEvents();
		}

		wait.Restore();
#endif
		g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z);
	}
	else if (nIndex == 4) {		// Slow Down
#ifdef EDITION_2ND
		if (!pDX01->iEmptyPortExist) { AfxMessageBox("Empty Port Tray 유무를 확인 후 진행하세요."); return; }
		if (!pDX01->iEmptyPortTopCheck) { AfxMessageBox("이미 Top Check 센서를 감지하지 않고 있습니다."); return; }

		CWaitCursor wait;

		g_objAJinAXL.Move_AbsSlow(AX_EMPTY_PORT_Z, pMoveData->dEmptyPortZ[1]);

		DWORD dwStart = GetTickCount();
		while (pDX01->iEmptyPortTopCheck) {
			if (GetTickCount() - dwStart > 10000) { g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z); AfxMessageBox("Time Over."); wait.Restore(); return; }
			theApp.DoEvents();
		}

		wait.Restore();
#else
		if (!pDX07->iEmptyPortExist) { AfxMessageBox("Empty Port Tray 유무를 확인 후 진행하세요."); return; }
		if (!pDX07->iEmptyPortTopCheck) { AfxMessageBox("이미 Top Check 센서를 감지하지 않고 있습니다."); return; }

		CWaitCursor wait;

		g_objAJinAXL.Move_AbsSlow(AX_EMPTY_PORT_Z, pMoveData->dEmptyPortZ[1]);

		DWORD dwStart = GetTickCount();
		while (pDX07->iEmptyPortTopCheck) {
			if (GetTickCount() - dwStart > 10000) { g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z); AfxMessageBox("Time Over."); wait.Restore(); return; }
			theApp.DoEvents();
		}

		wait.Restore();
#endif
		g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z);
	}
	else if (nIndex == 5) {		// Stop
		g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z);
	}
	else if (nIndex == 6) {		// Trans1 Offset
		g_objAJinAXL.Move_Relative(AX_EMPTY_PORT_Z, -pMoveData->dEmptyTrans1X[2]);
	}
	else if (nIndex == 7) {		// Trans2 Offset
		g_objAJinAXL.Move_Relative(AX_EMPTY_PORT_Z, pMoveData->dEmptyTrans2Y[2]);
	}

	m_strLog.Format("[Manual Unload] Empty Port Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnEmptyTrans2YClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_EMPTY_TRANS2_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_EMPTY_TRANS2_Y_0;

	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();
	if (!pDX13->iEmptyTrans2Up || pDX13->iEmptyTrans2Down) { AfxMessageBox("Empty Trans2 Up 상태 확인 후 진행하세요."); return; }

	g_objCommon.Move_Position(AX_EMPTY_TRANS2_Y, nIndex);

	m_strLog.Format("[Manual Unload] Empty Trans2 Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnEmptyTrans2IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_EMPTY_TRANS2_IO_0;

	DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();

	if (nIndex == 0) { pDY13->oEmptyTrans2Up = TRUE;        pDY13->oEmptyTrans2Down = FALSE; }
	if (nIndex == 1) { pDY13->oEmptyTrans2Up = FALSE;       pDY13->oEmptyTrans2Down = TRUE; }
	if (nIndex == 2) { pDY13->oEmptyTrans2MasterIn = TRUE;  pDY13->oEmptyTrans2MasterOut = FALSE; }
	if (nIndex == 3) { pDY13->oEmptyTrans2MasterIn = FALSE; pDY13->oEmptyTrans2MasterOut = TRUE; }
	if (nIndex == 4) { pDY13->oEmptyTrans2SlaveIn = TRUE;   pDY13->oEmptyTrans2SlaveOut = FALSE; }
	if (nIndex == 5) { pDY13->oEmptyTrans2SlaveIn = FALSE;  pDY13->oEmptyTrans2SlaveOut = TRUE; }

	g_objAJinAXL.Write_Output(13);

	m_strLog.Format("[Manual Unload] Empty Trans2 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnGoodBufferIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_GOOD_BUFFER_IO_0;

	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();

	if (nIndex == 0) { pDY12->oGoodTrayBuffSupport1In = TRUE;        pDY12->oGoodTrayBuffSupport2In = TRUE; }
	if (nIndex == 1) { pDY12->oGoodTrayBuffSupport1In = FALSE;       pDY12->oGoodTrayBuffSupport2In = FALSE; }

	g_objAJinAXL.Write_Output(12);

	m_strLog.Format("[Manual Unload] Good Buffer IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnNgBufferIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_NG_BUFFER_IO_0;

	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();

	// Vac On
	if (nIndex ==  20 || nIndex ==  0) { pDY10->oNgBufferVac01 = TRUE; }
	if (nIndex ==  20 || nIndex ==  1) { pDY10->oNgBufferVac02 = TRUE; }
	if (nIndex ==  20 || nIndex ==  2) { pDY10->oNgBufferVac03 = TRUE; }
	if (nIndex ==  20 || nIndex ==  3) { pDY10->oNgBufferVac04 = TRUE; }
// 	if (nIndex ==  20 || nIndex ==  4) { pDY10->o1031 = TRUE; }
	if (nIndex ==  21 || nIndex ==  5) { pDY11->oNgBufferVac05 = TRUE; }
	if (nIndex ==  21 || nIndex ==  6) { pDY11->oNgBufferVac06 = TRUE; }
	if (nIndex ==  21 || nIndex ==  7) { pDY11->oNgBufferVac07 = TRUE; }
	if (nIndex ==  21 || nIndex ==  8) { pDY11->oNgBufferVac08 = TRUE; }
// 	if (nIndex ==  21 || nIndex ==  9) { pDY11->o1131 = TRUE; }

	// Vac Off
	if (nIndex ==  22 || nIndex == 10) { pDY10->oNgBufferVac01 = FALSE; }
	if (nIndex ==  22 || nIndex == 11) { pDY10->oNgBufferVac02 = FALSE; }
	if (nIndex ==  22 || nIndex == 12) { pDY10->oNgBufferVac03 = FALSE; }
	if (nIndex ==  22 || nIndex == 13) { pDY10->oNgBufferVac04 = FALSE; }
// 	if (nIndex ==  22 || nIndex == 14) { pDY10->o1031 = FALSE; }
	if (nIndex ==  23 || nIndex == 15) { pDY11->oNgBufferVac05 = FALSE; }
	if (nIndex ==  23 || nIndex == 16) { pDY11->oNgBufferVac06 = FALSE; }
	if (nIndex ==  23 || nIndex == 17) { pDY11->oNgBufferVac07 = FALSE; }
	if (nIndex ==  23 || nIndex == 18) { pDY11->oNgBufferVac08 = FALSE; }
// 	if (nIndex ==  23 || nIndex == 19) { pDY11->o1131 = FALSE; }

	g_objAJinAXL.Write_Output(10);
	g_objAJinAXL.Write_Output(11);

	m_strLog.Format("[Manual Unload] NG Buffer IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

///////////////////////////////////////////////////////////////////////////////







void CManualUnloadDlg::OnBnClickedRdoSkipon()
{
	m_rdoSkipOff.SetCheck(FALSE);

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CIniFileCS INI(gData.sEnvPath + "\\EquipData.ini");
	if (!INI.Check_File()) { AfxMessageBox("EquipData.ini File Not Found!!!"); return; }

	INI.Set_Bool("OPTION", "INSPECT_SKIP", TRUE);

	g_objDataManager.Read_EquipData();
}


void CManualUnloadDlg::OnBnClickedRdoSkipoff()
{
	m_rdoSkipOn.SetCheck(FALSE);

		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CIniFileCS INI(gData.sEnvPath + "\\EquipData.ini");
	if (!INI.Check_File()) { AfxMessageBox("EquipData.ini File Not Found!!!"); return; }

	INI.Set_Bool("OPTION", "INSPECT_SKIP", FALSE);

	g_objDataManager.Read_EquipData();
}
