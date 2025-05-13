// ManualBtm2Dlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "ManualBtm2Dlg.h"
#include "afxdialogex.h"
#include "math.h"

#include "LogFile.h"
#include "AJinAXL.h"
#include "DataManager.h"
#include "Common.h"
#include "SequenceMain.h"

// CManualBtm2Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualBtm2Dlg, CDialogEx)

CManualBtm2Dlg::CManualBtm2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualBtm2Dlg::IDD, pParent)
{
}

CManualBtm2Dlg::~CManualBtm2Dlg()
{
}

void CManualBtm2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 17; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 12; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 12; i++) DDX_Control(pDX, IDC_STC_AXIS_POS_0 + i, m_stcAxisPos[i]);
	for (int i = 0; i <  8; i++) DDX_Control(pDX, IDC_BTN_BTM2_PICKER_X_0 + i, m_btnBtm2PickerX[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_BTM2_PICKER_Z_0 + i, m_btnBtm2PickerZ[i]);
	for (int i = 0; i <  3; i++) DDX_Control(pDX, IDC_BTN_BTM2_PICKER_P1_0 + i, m_btnBtm2PickerP1[i]);
	for (int i = 0; i <  3; i++) DDX_Control(pDX, IDC_BTN_BTM2_PICKER_P2_0 + i, m_btnBtm2PickerP2[i]);
	for (int i = 0; i < 54; i++) DDX_Control(pDX, IDC_BTN_BTM2_PICKER_IO_0 + i, m_btnBtm2PickerIo[i]);
	for (int i = 0; i < 40; i++) DDX_Control(pDX, IDC_LED_BTM2_PICKER_IO_0 + i, m_ledBtm2PickerIo[i]);
	for (int i = 0; i <  6; i++) DDX_Control(pDX, IDC_BTN_BUFFER_STAGE1_Y_0 + i, m_btnBufferStage1Y[i]);
	for (int i = 0; i <  1; i++) DDX_Control(pDX, IDC_CHK_BUFFER_STAGE1_IO_0 + i, m_chkBufferStage1Io[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_BUFFER_STAGE1_IO_0 + i, m_btnBufferStage1Io[i]);
	for (int i = 0; i <  5; i++) DDX_Control(pDX, IDC_LED_BUFFER_STAGE1_IO_0 + i, m_ledBufferStage1Io[i]);
	for (int i = 0; i <  6; i++) DDX_Control(pDX, IDC_BTN_BUFFER_STAGE2_Y_0 + i, m_btnBufferStage2Y[i]);
	for (int i = 0; i <  1; i++) DDX_Control(pDX, IDC_CHK_BUFFER_STAGE2_IO_0 + i, m_chkBufferStage2Io[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_BUFFER_STAGE2_IO_0 + i, m_btnBufferStage2Io[i]);
	for (int i = 0; i <  5; i++) DDX_Control(pDX, IDC_LED_BUFFER_STAGE2_IO_0 + i, m_ledBufferStage2Io[i]);
	for (int i = 0; i <  9; i++) DDX_Control(pDX, IDC_BTN_SORT_PICKER1_X_0 + i, m_btnSortPicker1X[i]);
	for (int i = 0; i <  6; i++) DDX_Control(pDX, IDC_BTN_SORT_PICKER1_Z_0 + i, m_btnSortPicker1Z[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_BTN_SORT_PICKER1_P_0 + i, m_btnSortPicker1P[i]);
	for (int i = 0; i < 24; i++) DDX_Control(pDX, IDC_BTN_SORT_PICKER1_IO_0 + i, m_btnSortPicker1Io[i]);
	for (int i = 0; i < 20; i++) DDX_Control(pDX, IDC_LED_SORT_PICKER1_IO_0 + i, m_ledSortPicker1Io[i]);
	for (int i = 0; i <  9; i++) DDX_Control(pDX, IDC_BTN_SORT_PICKER2_X_0 + i, m_btnSortPicker2X[i]);
	for (int i = 0; i <  6; i++) DDX_Control(pDX, IDC_BTN_SORT_PICKER2_Z_0 + i, m_btnSortPicker2Z[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_BTN_SORT_PICKER2_P_0 + i, m_btnSortPicker2P[i]);
	for (int i = 0; i < 24; i++) DDX_Control(pDX, IDC_BTN_SORT_PICKER2_IO_0 + i, m_btnSortPicker2Io[i]);
	for (int i = 0; i < 20; i++) DDX_Control(pDX, IDC_LED_SORT_PICKER2_IO_0 + i, m_ledSortPicker2Io[i]);
}

BEGIN_MESSAGE_MAP(CManualBtm2Dlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_BTM2_PICKER_X_0, IDC_BTN_BTM2_PICKER_X_7, OnBtnBtm2PickerXClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_BTM2_PICKER_Z_0, IDC_BTN_BTM2_PICKER_Z_3, OnBtnBtm2PickerZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_BTM2_PICKER_P1_0, IDC_BTN_BTM2_PICKER_P1_2, OnBtnBtm2PickerP1Click)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_BTM2_PICKER_P2_0, IDC_BTN_BTM2_PICKER_P2_2, OnBtnBtm2PickerP2Click)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_BTM2_PICKER_IO_0, IDC_BTN_BTM2_PICKER_IO_53, OnBtnBtm2PickerIoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_BUFFER_STAGE1_Y_0, IDC_BTN_BUFFER_STAGE1_Y_5, OnBtnBufferStage1YClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_BUFFER_STAGE1_IO_0, IDC_CHK_BUFFER_STAGE1_IO_0, OnChkBufferStage1IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_BUFFER_STAGE1_IO_0, IDC_BTN_BUFFER_STAGE1_IO_3, OnBtnBufferStage1IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_BUFFER_STAGE2_Y_0, IDC_BTN_BUFFER_STAGE2_Y_5, OnBtnBufferStage2YClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_BUFFER_STAGE2_IO_0, IDC_CHK_BUFFER_STAGE2_IO_0, OnChkBufferStage2IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_BUFFER_STAGE2_IO_0, IDC_BTN_BUFFER_STAGE2_IO_3, OnBtnBufferStage2IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_SORT_PICKER1_X_0, IDC_BTN_SORT_PICKER1_X_8, OnBtnSortPicker1XClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_SORT_PICKER1_Z_0, IDC_BTN_SORT_PICKER1_Z_5, OnBtnSortPicker1ZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_SORT_PICKER1_P_0, IDC_BTN_SORT_PICKER1_P_1, OnBtnSortPicker1PClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_SORT_PICKER1_IO_0, IDC_BTN_SORT_PICKER1_IO_23, OnBtnSortPicker1IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_SORT_PICKER2_X_0, IDC_BTN_SORT_PICKER2_X_8, OnBtnSortPicker2XClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_SORT_PICKER2_Z_0, IDC_BTN_SORT_PICKER2_Z_5, OnBtnSortPicker2ZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_SORT_PICKER2_P_0, IDC_BTN_SORT_PICKER2_P_1, OnBtnSortPicker2PClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_SORT_PICKER2_IO_0, IDC_BTN_SORT_PICKER2_IO_23, OnBtnSortPicker2IoClick)
END_MESSAGE_MAP()

// CManualBtm2Dlg 메시지 처리기입니다.

BOOL CManualBtm2Dlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CManualBtm2Dlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CManualBtm2Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Display_Status();

	DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();

	m_chkBufferStage1Io[0].SetCheck(pDY09->oBufferStage1Vacuum);
	m_chkBufferStage2Io[0].SetCheck(pDY09->oBufferStage2Vacuum);

	m_strLog.Format("[Manual Trans] Show Window");
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::Initial_Controls() 
{
	for (int i = 0; i < 5; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT);
	for (int i = 5; i < 9; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x30, 0x80, 0x00), COLOR_DEFAULT);
	for (int i = 9; i < 17; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0x10), COLOR_DEFAULT);
	m_Group[11].Init_Ctrl("Arial", 10, TRUE, RGB(0xD0, 0x10, 0x10), COLOR_DEFAULT);
	m_Group[15].Init_Ctrl("Arial", 10, TRUE, RGB(0xD0, 0x10, 0x10), COLOR_DEFAULT);
	for (int i = 0; i < 12; i++) m_Label[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, RGB(0xB0, 0xB0, 0xB0));
	for (int i = 0; i < 12; i++) m_stcAxisPos[i].Init_Ctrl("Arial", 10, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x10, 0x10, 0x60));
	for (int i = 0; i <  8; i++)  m_btnBtm2PickerX[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++)  m_btnBtm2PickerZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  3; i++)  m_btnBtm2PickerP1[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  3; i++)  m_btnBtm2PickerP2[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 54; i++)  m_btnBtm2PickerIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 40; i++)  m_ledBtm2PickerIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  6; i++)  m_btnBufferStage1Y[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  1; i++)  m_chkBufferStage1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++)  m_btnBufferStage1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  5; i++)  m_ledBufferStage1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  6; i++)  m_btnBufferStage2Y[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  1; i++)  m_chkBufferStage2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++)  m_btnBufferStage2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  5; i++)  m_ledBufferStage2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  9; i++)  m_btnSortPicker1X[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  6; i++)  m_btnSortPicker1Z[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++)  m_btnSortPicker1P[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 24; i++)  m_btnSortPicker1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 20; i++)  m_ledSortPicker1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  9; i++)  m_btnSortPicker2X[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  6; i++)  m_btnSortPicker2Z[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++)  m_btnSortPicker2P[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 24; i++)  m_btnSortPicker2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 20; i++)  m_ledSortPicker2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
}

void CManualBtm2Dlg::Display_Status()
{
	CString strPos;
	for (int i = 0; i < 12; i++) {
		double dPos = g_objAJinAXL.Get_Position(AX_BTM2_PICKER_X + i);
		strPos.Format("%0.3lf", dPos);
		m_stcAxisPos[i].SetWindowText(strPos);
	}

	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
	DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09();
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();

#ifdef EDITION_2ND
	m_ledBtm2PickerIo[0].Set_On(pDX07->iBtm2PickerOpen01);
	m_ledBtm2PickerIo[1].Set_On(pDX07->iBtm2PickerOpen02);
	m_ledBtm2PickerIo[2].Set_On(pDX07->iBtm2PickerOpen03);
	m_ledBtm2PickerIo[3].Set_On(pDX07->iBtm2PickerOpen04);
	m_ledBtm2PickerIo[4].Set_On(pDX07->iBtm2PickerOpen05);
	m_ledBtm2PickerIo[5].Set_On(pDX07->iBtm2PickerOpen06);
	m_ledBtm2PickerIo[6].Set_On(pDX07->iBtm2PickerOpen07);
	m_ledBtm2PickerIo[7].Set_On(pDX07->iBtm2PickerOpen08);

	m_ledBtm2PickerIo[ 8].Set_On(pDX07->iBtm2PickerExist01);
	m_ledBtm2PickerIo[ 9].Set_On(pDX07->iBtm2PickerExist02);
	m_ledBtm2PickerIo[10].Set_On(pDX07->iBtm2PickerExist03);
	m_ledBtm2PickerIo[11].Set_On(pDX07->iBtm2PickerExist04);
	m_ledBtm2PickerIo[12].Set_On(pDX07->iBtm2PickerExist05);
	m_ledBtm2PickerIo[13].Set_On(pDX07->iBtm2PickerExist06);
	m_ledBtm2PickerIo[14].Set_On(pDX07->iBtm2PickerExist07);
	m_ledBtm2PickerIo[15].Set_On(pDX07->iBtm2PickerExist08);

	m_ledBtm2PickerIo[16].Set_On(pDX07->iBtm2PickerUp01);
	m_ledBtm2PickerIo[17].Set_On(pDX07->iBtm2PickerUp02);
	m_ledBtm2PickerIo[18].Set_On(pDX07->iBtm2PickerUp03);
	m_ledBtm2PickerIo[19].Set_On(pDX07->iBtm2PickerUp04);
	m_ledBtm2PickerIo[20].Set_On(pDX08->iBtm2PickerUp05);
	m_ledBtm2PickerIo[21].Set_On(pDX08->iBtm2PickerUp06);
	m_ledBtm2PickerIo[22].Set_On(pDX08->iBtm2PickerUp07);
	m_ledBtm2PickerIo[23].Set_On(pDX08->iBtm2PickerUp08);

	m_ledBtm2PickerIo[24].Set_On(pDX07->iBtm2PickerDown01);
	m_ledBtm2PickerIo[25].Set_On(pDX07->iBtm2PickerDown02);
	m_ledBtm2PickerIo[26].Set_On(pDX07->iBtm2PickerDown03);
	m_ledBtm2PickerIo[27].Set_On(pDX07->iBtm2PickerDown04);
	m_ledBtm2PickerIo[28].Set_On(pDX08->iBtm2PickerDown05);
	m_ledBtm2PickerIo[29].Set_On(pDX08->iBtm2PickerDown06);
	m_ledBtm2PickerIo[30].Set_On(pDX08->iBtm2PickerDown07);
	m_ledBtm2PickerIo[31].Set_On(pDX08->iBtm2PickerDown08);

	m_ledBtm2PickerIo[32].Set_On(pDX08->iBtm2PickerVac01);
	m_ledBtm2PickerIo[33].Set_On(pDX08->iBtm2PickerVac02);
	m_ledBtm2PickerIo[34].Set_On(pDX08->iBtm2PickerVac03);
	m_ledBtm2PickerIo[35].Set_On(pDX08->iBtm2PickerVac04);
	m_ledBtm2PickerIo[36].Set_On(pDX08->iBtm2PickerVac05);
	m_ledBtm2PickerIo[37].Set_On(pDX08->iBtm2PickerVac06);
	m_ledBtm2PickerIo[38].Set_On(pDX08->iBtm2PickerVac07);
	m_ledBtm2PickerIo[39].Set_On(pDX08->iBtm2PickerVac08);
#else
	// 추후 구형 1호기 I/O Map에 따라 수정해줘야한다.
#endif	

	m_ledBufferStage1Io[0].Set_On(pDX09->iBufferStage1Vacuum);
	m_ledBufferStage1Io[1].Set_On(pDX09->iBufferStage1Normal);
	m_ledBufferStage1Io[2].Set_On(pDX09->iBufferStage1Rotate);
	m_ledBufferStage1Io[3].Set_On(pDX09->iBufferStage1Up);
	m_ledBufferStage1Io[4].Set_On(pDX09->iBufferStage1Down);

	m_ledBufferStage2Io[0].Set_On(pDX09->iBufferStage2Vacuum);
	m_ledBufferStage2Io[1].Set_On(pDX09->iBufferStage2Normal);
	m_ledBufferStage2Io[2].Set_On(pDX09->iBufferStage2Rotate);
	m_ledBufferStage2Io[3].Set_On(pDX09->iBufferStage2Up);
	m_ledBufferStage2Io[4].Set_On(pDX09->iBufferStage2Down);

	m_ledSortPicker1Io[ 0].Set_On(pDX10->iSortPicker1Up1);
	m_ledSortPicker1Io[ 1].Set_On(pDX10->iSortPicker1Up2);
	m_ledSortPicker1Io[ 2].Set_On(pDX10->iSortPicker1Up3);
	m_ledSortPicker1Io[ 3].Set_On(pDX10->iSortPicker1Up4);
// 	m_ledSortPicker1Io[ 4].Set_On(pDX10->i1008);
	m_ledSortPicker1Io[ 5].Set_On(pDX10->iSortPicker1Down1);
	m_ledSortPicker1Io[ 6].Set_On(pDX10->iSortPicker1Down2);
	m_ledSortPicker1Io[ 7].Set_On(pDX10->iSortPicker1Down3);
	m_ledSortPicker1Io[ 8].Set_On(pDX10->iSortPicker1Down4);
// 	m_ledSortPicker1Io[ 9].Set_On(pDX10->i1009);
	m_ledSortPicker1Io[10].Set_On(pDX10->iSortPicker1Open1);
	m_ledSortPicker1Io[11].Set_On(pDX10->iSortPicker1Open2);
	m_ledSortPicker1Io[12].Set_On(pDX10->iSortPicker1Open3);
	m_ledSortPicker1Io[13].Set_On(pDX10->iSortPicker1Open4);
//	m_ledSortPicker1Io[14].Set_On(pDX10->i1024);
	m_ledSortPicker1Io[15].Set_On(pDX10->iSortPicker1Exist1);
	m_ledSortPicker1Io[16].Set_On(pDX10->iSortPicker1Exist2);
	m_ledSortPicker1Io[17].Set_On(pDX10->iSortPicker1Exist3);
	m_ledSortPicker1Io[18].Set_On(pDX10->iSortPicker1Exist4);
//	m_ledSortPicker1Io[19].Set_On(pDX10->i1025);

	m_ledSortPicker2Io[ 0].Set_On(pDX11->iSortPicker2Up1);
	m_ledSortPicker2Io[ 1].Set_On(pDX11->iSortPicker2Up2);
	m_ledSortPicker2Io[ 2].Set_On(pDX11->iSortPicker2Up3);
	m_ledSortPicker2Io[ 3].Set_On(pDX11->iSortPicker2Up4);
// 	m_ledSortPicker2Io[ 4].Set_On(pDX11->i1108);
	m_ledSortPicker2Io[ 5].Set_On(pDX11->iSortPicker2Down1);
	m_ledSortPicker2Io[ 6].Set_On(pDX11->iSortPicker2Down2);
	m_ledSortPicker2Io[ 7].Set_On(pDX11->iSortPicker2Down3);
	m_ledSortPicker2Io[ 8].Set_On(pDX11->iSortPicker2Down4);
// 	m_ledSortPicker2Io[ 9].Set_On(pDX11->i1109);
	m_ledSortPicker2Io[10].Set_On(pDX11->iSortPicker2Open1);
	m_ledSortPicker2Io[11].Set_On(pDX11->iSortPicker2Open2);
	m_ledSortPicker2Io[12].Set_On(pDX11->iSortPicker2Open3);
	m_ledSortPicker2Io[13].Set_On(pDX11->iSortPicker2Open4);
// 	m_ledSortPicker2Io[14].Set_On(pDX11->i1124);
	m_ledSortPicker2Io[15].Set_On(pDX11->iSortPicker2Exist1);
	m_ledSortPicker2Io[16].Set_On(pDX11->iSortPicker2Exist2);
	m_ledSortPicker2Io[17].Set_On(pDX11->iSortPicker2Exist3);
	m_ledSortPicker2Io[18].Set_On(pDX11->iSortPicker2Exist4);
// 	m_ledSortPicker2Io[19].Set_On(pDX11->i1125);
}

///////////////////////////////////////////////////////////////////////////////

void CManualBtm2Dlg::OnBtnBtm2PickerXClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_BTM2_PICKER_X_0;

	if (!g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0)) { AfxMessageBox("Btm2 Picker Z축 Ready Up 상태를 확인 후 진행하세요."); return; }

	if (nIndex == 6 || nIndex == 7) {	// Pitch Move
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		int nDir = (nIndex == 6 ? -1 : 1);
		double dMove = pEquipData->dTrayPitchX * nDir;
		g_objAJinAXL.Move_Relative(AX_BTM2_PICKER_X, dMove);
	} else {
		g_objCommon.Move_Position(AX_BTM2_PICKER_X, nIndex);
	}

	m_strLog.Format("[Manual Btm2] Btm2 Picker X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnBtm2PickerZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_X)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_BTM2_PICKER_Z_0;

	int nPosX = -1;
	for (int i = 0; i < 6; i++) { if (g_objCommon.Check_Position(AX_BTM2_PICKER_X, i)) { nPosX = i; break; } }

	if (nIndex == 1 && nPosX != 0) { AfxMessageBox("Btm2 Picker X축이 Inspect 위치인지 확인 후 진행하세요."); return; }
	if (nIndex == 2 && nPosX != 1) { AfxMessageBox("Btm2 Picker X축이 BTM2 위치인지 확인 후 진행하세요."); return; }
	if (nIndex == 3 && nPosX <= 1) { AfxMessageBox("Btm2 Picker X축이 Buffer 위치인지 확인 후 진행하세요."); return; }

	g_objCommon.Move_Position(AX_BTM2_PICKER_Z, nIndex);

	m_strLog.Format("[Manual Btm2] Btm2 Picker Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnBtm2PickerP1Click(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_P1)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_BTM2_PICKER_P1_0;

	if (!g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0)) { AfxMessageBox("Btm2 Picker Z축 Ready Up 상태를 확인 후 진행하세요."); return; }

	if (nIndex == 2) {
		g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_P1, -2.5);
	} else {
// 		if (!gData.bR04A && !g_objCommon.Get_Btm2PickerNormal()) { AfxMessageBox("Btm2 Picker Normal 상태 확인 후 진행하세요."); return; }
		g_objCommon.Move_Position(AX_BTM2_PICKER_P1, nIndex);
	}

	m_strLog.Format("[Manual Btm2] Btm2 Picker P1 (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnBtm2PickerP2Click(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_P2)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_BTM2_PICKER_P2_0;

	if (!g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0)) { AfxMessageBox("Btm2 Picker Z축 Ready Up 상태를 확인 후 진행하세요."); return; }

	if (nIndex == 2) {
		g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_P2, -2.5);
	} else {
// 		if (!gData.bR04A && !g_objCommon.Get_Btm2PickerNormal()) { AfxMessageBox("Btm2 Picker Normal 상태 확인 후 진행하세요."); return; }
		g_objCommon.Move_Position(AX_BTM2_PICKER_P2, nIndex);
	}

	m_strLog.Format("[Manual Btm2] Btm2 Picker P2 (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnBtm2PickerIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_BTM2_PICKER_IO_0;

	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();

#ifdef EDITION_2ND
	if (nIndex == 48 || nIndex ==  0) { pDY07->oBtm2PickerOpen01 = TRUE; pDY07->oBtm2PickerClose01 = FALSE; }	// Open
	if (nIndex == 48 || nIndex ==  1) { pDY07->oBtm2PickerOpen02 = TRUE; pDY07->oBtm2PickerClose02 = FALSE; }
	if (nIndex == 48 || nIndex ==  2) { pDY07->oBtm2PickerOpen03 = TRUE; pDY07->oBtm2PickerClose03 = FALSE; }
	if (nIndex == 48 || nIndex ==  3) { pDY07->oBtm2PickerOpen04 = TRUE; pDY07->oBtm2PickerClose04 = FALSE; }
	if (nIndex == 48 || nIndex ==  4) { pDY07->oBtm2PickerOpen05 = TRUE; pDY07->oBtm2PickerClose05 = FALSE; }
	if (nIndex == 48 || nIndex ==  5) { pDY07->oBtm2PickerOpen06 = TRUE; pDY07->oBtm2PickerClose06 = FALSE; }
	if (nIndex == 48 || nIndex ==  6) { pDY07->oBtm2PickerOpen07 = TRUE; pDY07->oBtm2PickerClose07 = FALSE; }
	if (nIndex == 48 || nIndex ==  7) { pDY07->oBtm2PickerOpen08 = TRUE; pDY07->oBtm2PickerClose08 = FALSE; }

	if (nIndex == 49 || nIndex ==  8) { pDY07->oBtm2PickerOpen01 = FALSE; pDY07->oBtm2PickerClose01 = TRUE; }	// Close
	if (nIndex == 49 || nIndex ==  9) { pDY07->oBtm2PickerOpen02 = FALSE; pDY07->oBtm2PickerClose02 = TRUE; }
	if (nIndex == 49 || nIndex == 10) { pDY07->oBtm2PickerOpen03 = FALSE; pDY07->oBtm2PickerClose03 = TRUE; }
	if (nIndex == 49 || nIndex == 11) { pDY07->oBtm2PickerOpen04 = FALSE; pDY07->oBtm2PickerClose04 = TRUE; }
	if (nIndex == 49 || nIndex == 12) { pDY07->oBtm2PickerOpen05 = FALSE; pDY07->oBtm2PickerClose05 = TRUE; }
	if (nIndex == 49 || nIndex == 13) { pDY07->oBtm2PickerOpen06 = FALSE; pDY07->oBtm2PickerClose06 = TRUE; }
	if (nIndex == 49 || nIndex == 14) { pDY07->oBtm2PickerOpen07 = FALSE; pDY07->oBtm2PickerClose07 = TRUE; }
	if (nIndex == 49 || nIndex == 15) { pDY07->oBtm2PickerOpen08 = FALSE; pDY07->oBtm2PickerClose08 = TRUE; }

	if (nIndex == 50 || nIndex == 16) pDY07->oBtm2PickerDown01 = FALSE;	// Up
	if (nIndex == 50 || nIndex == 17) pDY07->oBtm2PickerDown02 = FALSE;
	if (nIndex == 50 || nIndex == 18) pDY07->oBtm2PickerDown03 = FALSE;
	if (nIndex == 50 || nIndex == 19) pDY07->oBtm2PickerDown04 = FALSE;
	if (nIndex == 50 || nIndex == 20) pDY08->oBtm2PickerDown05 = FALSE;
	if (nIndex == 50 || nIndex == 21) pDY08->oBtm2PickerDown06 = FALSE;
	if (nIndex == 50 || nIndex == 22) pDY08->oBtm2PickerDown07 = FALSE;
	if (nIndex == 50 || nIndex == 23) pDY08->oBtm2PickerDown08 = FALSE;

	if (nIndex == 51 || nIndex == 24) pDY07->oBtm2PickerDown01 = TRUE;	// Down
	if (nIndex == 51 || nIndex == 25) pDY07->oBtm2PickerDown02 = TRUE;
	if (nIndex == 51 || nIndex == 26) pDY07->oBtm2PickerDown03 = TRUE;
	if (nIndex == 51 || nIndex == 27) pDY07->oBtm2PickerDown04 = TRUE;
	if (nIndex == 51 || nIndex == 28) pDY08->oBtm2PickerDown05 = TRUE;
	if (nIndex == 51 || nIndex == 29) pDY08->oBtm2PickerDown06 = TRUE;
	if (nIndex == 51 || nIndex == 30) pDY08->oBtm2PickerDown07 = TRUE;
	if (nIndex == 51 || nIndex == 31) pDY08->oBtm2PickerDown08 = TRUE;

#else
// 추후 구형 1호기 I/O Map에 따라 수정해줘야한다.
#endif

	if (nIndex == 52 || nIndex == 32) { pDY08->oBtm2PickerVac01 = TRUE; pDY08->oBtm2PickerAir01 = FALSE; }	// Vac On
	if (nIndex == 52 || nIndex == 33) { pDY08->oBtm2PickerVac02 = TRUE; pDY08->oBtm2PickerAir02 = FALSE; }
	if (nIndex == 52 || nIndex == 34) { pDY08->oBtm2PickerVac03 = TRUE; pDY08->oBtm2PickerAir03 = FALSE; }
	if (nIndex == 52 || nIndex == 35) { pDY08->oBtm2PickerVac04 = TRUE; pDY08->oBtm2PickerAir04 = FALSE; }
	if (nIndex == 52 || nIndex == 36) { pDY08->oBtm2PickerVac05 = TRUE; pDY08->oBtm2PickerAir05 = FALSE; }
	if (nIndex == 52 || nIndex == 37) { pDY08->oBtm2PickerVac06 = TRUE; pDY08->oBtm2PickerAir06 = FALSE; }
	if (nIndex == 52 || nIndex == 38) { pDY08->oBtm2PickerVac07 = TRUE; pDY08->oBtm2PickerAir07 = FALSE; }
	if (nIndex == 52 || nIndex == 39) { pDY08->oBtm2PickerVac08 = TRUE; pDY08->oBtm2PickerAir08 = FALSE; }

	if (nIndex == 53 || nIndex == 40) { pDY08->oBtm2PickerVac01 = FALSE; pDY08->oBtm2PickerAir01 = TRUE; }	// Vac Off
	if (nIndex == 53 || nIndex == 41) { pDY08->oBtm2PickerVac02 = FALSE; pDY08->oBtm2PickerAir02 = TRUE; }
	if (nIndex == 53 || nIndex == 42) { pDY08->oBtm2PickerVac03 = FALSE; pDY08->oBtm2PickerAir03 = TRUE; }
	if (nIndex == 53 || nIndex == 43) { pDY08->oBtm2PickerVac04 = FALSE; pDY08->oBtm2PickerAir04 = TRUE; }
	if (nIndex == 53 || nIndex == 44) { pDY08->oBtm2PickerVac05 = FALSE; pDY08->oBtm2PickerAir05 = TRUE; }
	if (nIndex == 53 || nIndex == 45) { pDY08->oBtm2PickerVac06 = FALSE; pDY08->oBtm2PickerAir06 = TRUE; }
	if (nIndex == 53 || nIndex == 46) { pDY08->oBtm2PickerVac07 = FALSE; pDY08->oBtm2PickerAir07 = TRUE; }
	if (nIndex == 53 || nIndex == 47) { pDY08->oBtm2PickerVac08 = FALSE; pDY08->oBtm2PickerAir08 = TRUE; }

	if (nIndex == 53 || (nIndex >= 40 && nIndex < 48)) { g_objAJinAXL.Write_Output(8); Sleep(500); }

	if (nIndex == 53 || nIndex == 40) pDY08->oBtm2PickerAir01 = FALSE;		// Air Off
	if (nIndex == 53 || nIndex == 41) pDY08->oBtm2PickerAir02 = FALSE;
	if (nIndex == 53 || nIndex == 42) pDY08->oBtm2PickerAir03 = FALSE;
	if (nIndex == 53 || nIndex == 43) pDY08->oBtm2PickerAir04 = FALSE;
	if (nIndex == 53 || nIndex == 44) pDY08->oBtm2PickerAir05 = FALSE;
	if (nIndex == 53 || nIndex == 45) pDY08->oBtm2PickerAir06 = FALSE;
	if (nIndex == 53 || nIndex == 46) pDY08->oBtm2PickerAir07 = FALSE;
	if (nIndex == 53 || nIndex == 47) pDY08->oBtm2PickerAir08 = FALSE;

	g_objAJinAXL.Write_Output(7);
	g_objAJinAXL.Write_Output(8);

	m_strLog.Format("[Manual Btm2] Btm2 Picker IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnBufferStage1YClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_BUFFER_STAGE1_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_BUFFER_STAGE1_Y_0;

	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();

	double dBtm2X = g_objAJinAXL.Get_Position(AX_BTM2_PICKER_X);
	if (!g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) &&
		dBtm2X > pMoveData->dBtm2PickerX[0] + 0.5 && dBtm2X < pMoveData->dBtm2PickerX[1] - 0.5) {
		AfxMessageBox("Btm2 Picker Z Up 위치 확인 후 진행하세요."); return;
	}

	if (!g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER1_X, 0)) {
		AfxMessageBox("Sort Picker1 Z Up 위치 확인 후 진행하세요."); return;
	}

	if (!g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER2_X, 0)) {
		AfxMessageBox("Sort Picker2 Z Up 위치 확인 후 진행하세요."); return;
	}

	DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09();
	if (!pDX09->iBufferStage1Up && pDX09->iBufferStage1Down) {
		if (!pDX09->iBufferStage2Up || pDX09->iBufferStage2Down) { AfxMessageBox("Buffer Stage2 Up 상태 확인 후 진행하세요."); return; }
		if (nIndex < 4) g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, nIndex);	// L1, L2, Sort1, Sort2

	} else if (pDX09->iBufferStage1Up && !pDX09->iBufferStage1Down) {
		if (pDX09->iBufferStage2Up || !pDX09->iBufferStage2Down) { AfxMessageBox("Buffer Stage2 Down 상태 확인 후 진행하세요."); return; }
		if (nIndex == 4 || nIndex == 5) {	// Pitch Move
			EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
			int nDir = (nIndex == 4 ? 1 : -1);
			double dMove = pEquipData->dTrayPitchY * nDir;
			g_objAJinAXL.Move_Relative(AX_BUFFER_STAGE1_Y, dMove);
		} else {
			g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, nIndex);	// L1, L2, Sort1, Sort2
		}
	}

	m_strLog.Format("[Manual Btm2] Buffer Stage1 Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnChkBufferStage1IoClick(UINT nID)
{
// 	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_CHK_BUFFER_STAGE1_IO_0;

	BOOL bOn = m_chkBufferStage1Io[nIndex].GetCheck();
	DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();
	pDY09->oBufferStage1Vacuum = bOn;
	g_objAJinAXL.Write_Output(9);

	m_strLog.Format("[Manual Btm2] Buffer Stage1 Vacuum Click (%d, %d)", nIndex, bOn);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnBufferStage1IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_BUFFER_STAGE1_IO_0;

	if (nIndex == 0 || nIndex == 1) {
		DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09();
		if (pDX09->iBufferStage1Up || !pDX09->iBufferStage1Down) {
			if (!g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0)) { AfxMessageBox("Btm2 Picker Z축 Ready Up 상태 확인 후 진행하세요."); return; }
			if (!g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0)) { AfxMessageBox("Sort Picker1 Z축 Ready Up 상태 확인 후 진행하세요."); return; }
			if (!g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0)) { AfxMessageBox("Sort Picker2 Z축 Ready Up 상태 확인 후 진행하세요."); return; }
		}
	} else if (nIndex == 2 || nIndex == 3) {
		double dY1 = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE1_Y);
		double dY2 = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE2_Y);
		if (fabs(dY1 - dY2) < 300.0) { AfxMessageBox("Buffer Stage 2 Y축 위치 확인 후 진행하세요."); return; }
	}

	DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();
	if (nIndex == 0) pDY09->oBufferStage1Rotate = FALSE;
	if (nIndex == 1) pDY09->oBufferStage1Rotate = TRUE;
	if (nIndex == 2) { pDY09->oBufferStage1Up = TRUE;  pDY09->oBufferStage1Down = FALSE; }
	if (nIndex == 3) { pDY09->oBufferStage1Up = FALSE; pDY09->oBufferStage1Down = TRUE; }
	g_objAJinAXL.Write_Output(9);

	m_strLog.Format("[Manual Btm2] Buffer Stage1 Io (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnBufferStage2YClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_BUFFER_STAGE2_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_BUFFER_STAGE2_Y_0;

	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();

	double dBtm2X = g_objAJinAXL.Get_Position(AX_BTM2_PICKER_X);
	if (!g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) &&
		dBtm2X > pMoveData->dBtm2PickerX[0] + 0.5 && dBtm2X < pMoveData->dBtm2PickerX[1] - 0.5) {
		AfxMessageBox("Btm2 Picker Z Up 위치 확인 후 진행하세요."); return;
	}

	if (!g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER1_X, 0)) {
		AfxMessageBox("Sort Picker1 Z Up 위치 확인 후 진행하세요."); return;
	}

	if (!g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER2_X, 0)) {
		AfxMessageBox("Sort Picker2 Z Up 위치 확인 후 진행하세요."); return;
	}

	DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09();
	if (!pDX09->iBufferStage2Up && pDX09->iBufferStage2Down) {
		if (!pDX09->iBufferStage1Up || pDX09->iBufferStage1Down) { AfxMessageBox("Buffer Stage1 Up 상태 확인 후 진행하세요."); return; }
		if (nIndex < 4) g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, nIndex);	// L1, L2, Sort1, Sort2

	} else if (pDX09->iBufferStage2Up && !pDX09->iBufferStage2Down) {
		if (pDX09->iBufferStage1Up || !pDX09->iBufferStage1Down) { AfxMessageBox("Buffer Stage1 Down 상태 확인 후 진행하세요."); return; }
		if (nIndex == 4 || nIndex == 5) {	// Pitch Move
			EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
			int nDir = (nIndex == 4 ? 1 : -1);
			double dMove = pEquipData->dTrayPitchY * nDir;
			g_objAJinAXL.Move_Relative(AX_BUFFER_STAGE2_Y, dMove);
		} else {
			g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, nIndex);	// L1, L2, Sort1, Sort2
		}
	}

	m_strLog.Format("[Manual Btm2] Buffer Stage2 Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnChkBufferStage2IoClick(UINT nID)
{
// 	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_CHK_BUFFER_STAGE2_IO_0;

	BOOL bOn = m_chkBufferStage2Io[nIndex].GetCheck();
	DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();
	pDY09->oBufferStage2Vacuum = bOn;
	g_objAJinAXL.Write_Output(9);

	m_strLog.Format("[Manual Btm2] Buffer Stage2 Vacuum Click (%d, %d)", nIndex, bOn);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnBufferStage2IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_BUFFER_STAGE2_IO_0;

	if (nIndex == 0 || nIndex == 1) {
		DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09();
		if (pDX09->iBufferStage2Up || !pDX09->iBufferStage2Down) {
			if (!g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0)) { AfxMessageBox("Btm2 Picker Z축 Ready Up 상태 확인 후 진행하세요."); return; }
			if (!g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0)) { AfxMessageBox("Sort Picker1 Z축 Ready Up 상태 확인 후 진행하세요."); return; }
			if (!g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0)) { AfxMessageBox("Sort Picker2 Z축 Ready Up 상태 확인 후 진행하세요."); return; }
		}
	} else if (nIndex == 2 || nIndex == 3) {
		double dY2 = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE2_Y);
		double dY1 = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE1_Y);
		if (fabs(dY2 - dY1) < 300.0) { AfxMessageBox("Buffer Stage 1 Y축 위치 확인 후 진행하세요."); return; }
	}

	DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();
	if (nIndex == 0) pDY09->oBufferStage2Rotate = FALSE;
	if (nIndex == 1) pDY09->oBufferStage2Rotate = TRUE;
	if (nIndex == 2) { pDY09->oBufferStage2Up = TRUE;  pDY09->oBufferStage2Down = FALSE; }
	if (nIndex == 3) { pDY09->oBufferStage2Up = FALSE; pDY09->oBufferStage2Down = TRUE; }
	g_objAJinAXL.Write_Output(9);

	m_strLog.Format("[Manual Btm2] Buffer Stage2 Io (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnSortPicker1XClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_SORT_PICKER1_X_0;

	if (!g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0)) { AfxMessageBox("Sort Picker1 Z축 Ready Up 위치 확인 후 진행하세요."); return; }

	if (nIndex == 7 || nIndex == 8) {	// Pitch Move
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		int nDir = (nIndex == 7 ? -1 : 1);
		double dMove = pEquipData->dTrayPitchX * nDir;
		g_objAJinAXL.Move_Relative(AX_SORT_PICKER1_X, dMove);
	} else {
		g_objCommon.Move_Position(AX_SORT_PICKER1_X, nIndex);
	}

	m_strLog.Format("[Manual Btm2] Sort Picker1 X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnSortPicker1ZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_X)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_SORT_PICKER1_Z_0;

	int nPosX = -1;
	for (int i = 0; i < 7; i++) { if (g_objCommon.Check_Position(AX_SORT_PICKER1_X, i)) { nPosX = i; break; } }

	if (nIndex == 1 && nPosX != 0)				 { AfxMessageBox("Sort Picker1 X축이 Buffer 1 위치인지 확인 후 진행하세요."); return; }
	if (nIndex == 2 && nPosX != 2 && nPosX != 3) { AfxMessageBox("Sort Picker1 X축이 Good 위치인지 확인 후 진행하세요."); return; }
	if (nIndex == 3 && nPosX != 4 && nPosX != 5) { AfxMessageBox("Sort Picker1 X축이 NG 위치인지 확인 후 진행하세요."); return; }
	if (nIndex == 4 && nPosX != 6)				 { AfxMessageBox("Sort Picker1 X축이 NG Buffer 위치인지 확인 후 진행하세요."); return; }
	if (nIndex == 5 && nPosX != 1)				 { AfxMessageBox("Sort Picker1 X축이 Buffer 2 위치인지 확인 후 진행하세요."); return; }

	g_objCommon.Move_Position(AX_SORT_PICKER1_Z, nIndex);

	m_strLog.Format("[Manual Btm2] Sort Picker1 Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnSortPicker1PClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_P)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER1_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_SORT_PICKER1_P_0;

	if (!g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0)) { AfxMessageBox("Sort Picker Z축 Ready Up 위치 확인 후 진행하세요."); return; }

	g_objCommon.Move_Position(AX_SORT_PICKER1_P, nIndex);

	m_strLog.Format("[Manual Btm2] Sort Picker1 P (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnSortPicker1IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_SORT_PICKER1_IO_0;

	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();

	if (nIndex == 20 || nIndex == 0) pDY10->oSortPicker1Down1 = FALSE;		// Up
	if (nIndex == 20 || nIndex == 1) pDY10->oSortPicker1Down2 = FALSE;
	if (nIndex == 20 || nIndex == 2) pDY10->oSortPicker1Down3 = FALSE;
	if (nIndex == 20 || nIndex == 3) pDY10->oSortPicker1Down4 = FALSE;
// 	if (nIndex == 20 || nIndex == 4) pDY10->o1009 = FALSE;

	if (nIndex == 21 || nIndex == 5) pDY10->oSortPicker1Down1 = TRUE;		// Down
	if (nIndex == 21 || nIndex == 6) pDY10->oSortPicker1Down2 = TRUE;
	if (nIndex == 21 || nIndex == 7) pDY10->oSortPicker1Down3 = TRUE;
	if (nIndex == 21 || nIndex == 8) pDY10->oSortPicker1Down4 = TRUE;
// 	if (nIndex == 21 || nIndex == 9) pDY10->o1009 = TRUE;

	if (nIndex == 22 || nIndex == 10) { pDY10->oSortPicker1Open1 = TRUE; pDY10->oSortPicker1Close1 = FALSE;	}	// Open
	if (nIndex == 22 || nIndex == 11) { pDY10->oSortPicker1Open2 = TRUE; pDY10->oSortPicker1Close2 = FALSE;	}
	if (nIndex == 22 || nIndex == 12) { pDY10->oSortPicker1Open3 = TRUE; pDY10->oSortPicker1Close3 = FALSE;	}
	if (nIndex == 22 || nIndex == 13) { pDY10->oSortPicker1Open4 = TRUE; pDY10->oSortPicker1Close4 = FALSE;	}
// 	if (nIndex == 22 || nIndex == 14) pDY10->o1024 = TRUE;

	if (nIndex == 23 || nIndex == 15) { pDY10->oSortPicker1Open1 = FALSE; pDY10->oSortPicker1Close1 = TRUE;	}	// Close
	if (nIndex == 23 || nIndex == 16) { pDY10->oSortPicker1Open2 = FALSE; pDY10->oSortPicker1Close2 = TRUE;	}
	if (nIndex == 23 || nIndex == 17) { pDY10->oSortPicker1Open3 = FALSE; pDY10->oSortPicker1Close3 = TRUE;	}
	if (nIndex == 23 || nIndex == 18) { pDY10->oSortPicker1Open4 = FALSE; pDY10->oSortPicker1Close4 = TRUE;	}
// 	if (nIndex == 23 || nIndex == 19) pDY10->o1025 = FALSE;

	g_objAJinAXL.Write_Output(10);

	m_strLog.Format("[Manual Btm2] Sort Picker1 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnSortPicker2XClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_SORT_PICKER2_X_0;

	if (!g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0)) { AfxMessageBox("Sort Picker2 Z축 Ready Up 위치 확인 후 진행하세요."); return; }

	if (nIndex == 7 || nIndex == 8) {	// Pitch Move
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		int nDir = (nIndex == 7 ? -1 : 1);
		double dMove = pEquipData->dTrayPitchX * nDir;
		g_objAJinAXL.Move_Relative(AX_SORT_PICKER2_X, dMove);
	} else {
		g_objCommon.Move_Position(AX_SORT_PICKER2_X, nIndex);
	}

	m_strLog.Format("[Manual Btm2] Sort Picker2 X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnSortPicker2ZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_X)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_SORT_PICKER2_Z_0;

	int nPosX = -1;
	for (int i = 0; i < 7; i++) { if (g_objCommon.Check_Position(AX_SORT_PICKER2_X, i)) { nPosX = i; break; } }

	if (nIndex == 1 && nPosX != 0)				 { AfxMessageBox("Sort Picker2 X축이 Buffer 1 위치인지 확인 후 진행하세요."); return; }
	if (nIndex == 2 && nPosX != 2 && nPosX != 3) { AfxMessageBox("Sort Picker2 X축이 Good 위치인지 확인 후 진행하세요."); return; }
	if (nIndex == 3 && nPosX != 4 && nPosX != 5) { AfxMessageBox("Sort Picker2 X축이 NG 위치인지 확인 후 진행하세요."); return; }
	if (nIndex == 4 && nPosX != 6)				 { AfxMessageBox("Sort Picker2 X축이 NG Buffer 위치인지 확인 후 진행하세요."); return; }
	if (nIndex == 5 && nPosX != 1)				 { AfxMessageBox("Sort Picker2 X축이 Buffer 2 위치인지 확인 후 진행하세요."); return; }

	g_objCommon.Move_Position(AX_SORT_PICKER2_Z, nIndex);

	m_strLog.Format("[Manual Btm2] Sort Picker2 Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnSortPicker2PClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_P)) return;
	if (!g_objAJinAXL.Is_Home(AX_SORT_PICKER2_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_SORT_PICKER2_P_0;

	if (!g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0)) { AfxMessageBox("Sort Picker Z축 Ready Up 위치 확인 후 진행하세요."); return; }

	g_objCommon.Move_Position(AX_SORT_PICKER2_P, nIndex);

	m_strLog.Format("[Manual Btm2] Sort Picker2 P (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm2Dlg::OnBtnSortPicker2IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_SORT_PICKER2_IO_0;

	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();

	if (nIndex == 20 || nIndex == 0) pDY11->oSortPicker2Down1 = FALSE;		// Up
	if (nIndex == 20 || nIndex == 1) pDY11->oSortPicker2Down2 = FALSE;
	if (nIndex == 20 || nIndex == 2) pDY11->oSortPicker2Down3 = FALSE;
	if (nIndex == 20 || nIndex == 3) pDY11->oSortPicker2Down4 = FALSE;
// 	if (nIndex == 20 || nIndex == 4) pDY11->o1109 = FALSE;

	if (nIndex == 21 || nIndex == 5) pDY11->oSortPicker2Down1 = TRUE;		// Down
	if (nIndex == 21 || nIndex == 6) pDY11->oSortPicker2Down2 = TRUE;
	if (nIndex == 21 || nIndex == 7) pDY11->oSortPicker2Down3 = TRUE;
	if (nIndex == 21 || nIndex == 8) pDY11->oSortPicker2Down4 = TRUE;
// 	if (nIndex == 21 || nIndex == 9) pDY11->o1109 = TRUE;

	if (nIndex == 22 || nIndex == 10) { pDY11->oSortPicker2Open1 = TRUE; pDY11->oSortPicker2Close1 = FALSE;	}	// Open
	if (nIndex == 22 || nIndex == 11) { pDY11->oSortPicker2Open2 = TRUE; pDY11->oSortPicker2Close2 = FALSE;	}
	if (nIndex == 22 || nIndex == 12) { pDY11->oSortPicker2Open3 = TRUE; pDY11->oSortPicker2Close3 = FALSE;	}
	if (nIndex == 22 || nIndex == 13) { pDY11->oSortPicker2Open4 = TRUE; pDY11->oSortPicker2Close4 = FALSE;	}
// 	if (nIndex == 22 || nIndex == 14) pDY11->o1124 = TRUE;

	if (nIndex == 23 || nIndex == 15) { pDY11->oSortPicker2Open1 = FALSE; pDY11->oSortPicker2Close1 = TRUE;	}	// Close
	if (nIndex == 23 || nIndex == 16) { pDY11->oSortPicker2Open2 = FALSE; pDY11->oSortPicker2Close2 = TRUE;	}
	if (nIndex == 23 || nIndex == 17) { pDY11->oSortPicker2Open3 = FALSE; pDY11->oSortPicker2Close3 = TRUE;	}
	if (nIndex == 23 || nIndex == 18) { pDY11->oSortPicker2Open4 = FALSE; pDY11->oSortPicker2Close4 = TRUE;	}
// 	if (nIndex == 23 || nIndex == 19) pDY11->o1125 = FALSE;

	g_objAJinAXL.Write_Output(11);

	m_strLog.Format("[Manual Btm2] Sort Picker2 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

///////////////////////////////////////////////////////////////////////////////
