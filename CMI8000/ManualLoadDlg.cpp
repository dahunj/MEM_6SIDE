// ManualLoadDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "ManualLoadDlg.h"
#include "afxdialogex.h"
#include "math.h"

#include "LogFile.h"
#include "AJinAXL.h"
#include "DataManager.h"
#include "Common.h"
#include "SequenceMain.h"

// CManualLoadDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualLoadDlg, CDialogEx)

CManualLoadDlg::CManualLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualLoadDlg::IDD, pParent)
{
}

CManualLoadDlg::~CManualLoadDlg()
{
}

void CManualLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 19; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 12; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 12; i++) DDX_Control(pDX, IDC_STC_AXIS_POS_0 + i, m_stcAxisPos[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_BTN_LOAD_PORT_IO_0 + i, m_btnLoadPortIo[i]);
	for (int i = 0; i < 23; i++) DDX_Control(pDX, IDC_LED_LOAD_PORT_IO_0 + i, m_ledLoadPortIo[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_BTN_LOAD_STAGE_X_0 + i, m_btnLoadStageX[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_BTN_LOAD_STAGE_Z_0 + i, m_btnLoadStageZ[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_BTN_LOAD_STAGE_IO_0 + i, m_btnLoadStageIo[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_LOAD_STAGE_IO_0 + i, m_ledLoadStageIo[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_BTN_LOAD_PICKER_Y_0 + i, m_btnLoadPickerY[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_BTN_LOAD_PICKER_IO_0 + i, m_btnLoadPickerIo[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_LED_LOAD_PICKER_IO_0 + i, m_ledLoadPickerIo[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_BTN_ANGLE_UNIT_Y_0 + i, m_btnAngleUnitY[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_BTN_ANGLE_UNIT_X_0 + i, m_btnAngleUnitX[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_BTN_ANGLE_UNIT_R_0 + i, m_btnAngleUnitR[i]);
	for (int i = 0; i < 1; i++) DDX_Control(pDX, IDC_BTN_VISION_ANGLE_Z_0 + i, m_btnVisionAngleZ[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_BTN_VISION_ALIGN_Z_0 + i, m_btnVisionAlignZ[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_BTN_ANGLE_PORT_IO_0 + i, m_btnAnglePortIo[i]);
	for (int i = 0; i < 9; i++) DDX_Control(pDX, IDC_LED_ANGLE_PORT_IO_0 + i, m_ledAnglePortIo[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_BTN_ANGLE_STAGE1_Y_0 + i, m_btnAngleStage1Y[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_BTN_ANGLE_STAGE1_Z_0 + i, m_btnAngleStage1Z[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_BTN_ANGLE_STAGE1_IO_0 + i, m_btnAngleStage1Io[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_ANGLE_STAGE1_IO_0 + i, m_ledAngleStage1Io[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_BTN_ANGLE_STAGE2_Y_0 + i, m_btnAngleStage2Y[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_BTN_ANGLE_STAGE2_Z_0 + i, m_btnAngleStage2Z[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_BTN_ANGLE_STAGE2_IO_0 + i, m_btnAngleStage2Io[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_ANGLE_STAGE2_IO_0 + i, m_ledAngleStage2Io[i]);
}

BEGIN_MESSAGE_MAP(CManualLoadDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_PORT_IO_0, IDC_BTN_LOAD_PORT_IO_7, OnBtnLoadPortIoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_STAGE_X_0, IDC_BTN_LOAD_STAGE_X_2, OnBtnLoadStageXClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_STAGE_Z_0, IDC_BTN_LOAD_STAGE_Z_3, OnBtnLoadStageZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_STAGE_IO_0, IDC_BTN_LOAD_STAGE_IO_3, OnBtnLoadStageIoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_PICKER_Y_0, IDC_BTN_LOAD_PICKER_Y_1, OnBtnLoadPickerYClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_PICKER_IO_0, IDC_BTN_LOAD_PICKER_IO_5, OnBtnLoadPickerIoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ANGLE_UNIT_Y_0, IDC_BTN_ANGLE_UNIT_Y_3, OnBtnAngleUnitYClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ANGLE_UNIT_X_0, IDC_BTN_ANGLE_UNIT_X_3, OnBtnAngleUnitXClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ANGLE_UNIT_R_0, IDC_BTN_ANGLE_UNIT_R_1, OnBtnAngleUnitRClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_VISION_ANGLE_Z_0, IDC_BTN_VISION_ANGLE_Z_0, OnBtnVisionAngleZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_VISION_ALIGN_Z_0, IDC_BTN_VISION_ALIGN_Z_1, OnBtnVisionAlignZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ANGLE_PORT_IO_0, IDC_BTN_ANGLE_PORT_IO_5, OnBtnAnglePortIoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ANGLE_STAGE1_Y_0, IDC_BTN_ANGLE_STAGE1_Y_6, OnBtnAngleStage1YClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ANGLE_STAGE1_Z_0, IDC_BTN_ANGLE_STAGE1_Z_4, OnBtnAngleStage1ZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ANGLE_STAGE1_IO_0, IDC_BTN_ANGLE_STAGE1_IO_3, OnBtnAngleStage1IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ANGLE_STAGE2_Y_0, IDC_BTN_ANGLE_STAGE2_Y_6, OnBtnAngleStage2YClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ANGLE_STAGE2_Z_0, IDC_BTN_ANGLE_STAGE2_Z_4, OnBtnAngleStage2ZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ANGLE_STAGE2_IO_0, IDC_BTN_ANGLE_STAGE2_IO_3, OnBtnAngleStage2IoClick)
END_MESSAGE_MAP()

// CManualLoadDlg 메시지 처리기입니다.

BOOL CManualLoadDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CManualLoadDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CManualLoadDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Display_Status();

	m_strLog.Format("[Manual Load] Show Window");
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::Initial_Controls() 
{
	for (int i = 0; i < 2; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x30, 0x80, 0x00), COLOR_DEFAULT);
	for (int i = 2; i < 5; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0x10), COLOR_DEFAULT);
	for (int i = 5; i < 7; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0xD0), COLOR_DEFAULT);
	for (int i = 7; i < 12; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT);
	for (int i = 12; i < 13; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x30, 0x80, 0x00), COLOR_DEFAULT);
	for (int i = 13; i < 16; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0x10), COLOR_DEFAULT);
	for (int i = 16; i < 19; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0xF0), COLOR_DEFAULT);
	for (int i = 0; i < 12; i++) m_Label[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, RGB(0xB0, 0xB0, 0xB0));
	for (int i = 0; i < 12; i++) m_stcAxisPos[i].Init_Ctrl("Arial", 10, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x10, 0x10, 0x60));
	for (int i = 0; i < 8; i++) m_btnLoadPortIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 23; i++) m_ledLoadPortIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 3; i++) m_btnLoadStageX[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 4; i++) m_btnLoadStageZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 4; i++) m_btnLoadStageIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 5; i++) m_ledLoadStageIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 2; i++) m_btnLoadPickerY[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 6; i++) m_btnLoadPickerIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 7; i++) m_ledLoadPickerIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 4; i++) m_btnAngleUnitY[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 4; i++) m_btnAngleUnitX[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 2; i++) m_btnAngleUnitR[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 1; i++) m_btnVisionAngleZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 2; i++) m_btnVisionAlignZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 6; i++) m_btnAnglePortIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 9; i++) m_ledAnglePortIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 7; i++) m_btnAngleStage1Y[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 5; i++) m_btnAngleStage1Z[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 4; i++) m_btnAngleStage1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 5; i++) m_ledAngleStage1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 7; i++) m_btnAngleStage2Y[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 5; i++) m_btnAngleStage2Z[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 4; i++) m_btnAngleStage2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 5; i++) m_ledAngleStage2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
}

void CManualLoadDlg::Display_Status()
{
	CString strPos;
	for (int i = 0; i < 12; i++) {
		double dPos = g_objAJinAXL.Get_Position(AX_LOAD_STAGE_X + i);
		strPos.Format("%0.3lf", dPos);
		m_stcAxisPos[i].SetWindowText(strPos);
	}

	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	DX_DATA_02 *pDX02 = g_objAJinAXL.Get_pDX02();

// 	m_ledLoadPortIo[0].Set_On(pDX00->iLoadPort1Upper);
// 	m_ledLoadPortIo[1].Set_On(pDX00->iLoadPort1Lower);
	m_ledLoadPortIo[2].Set_On(pDX00->iLoadPort1Bottom);
	m_ledLoadPortIo[3].Set_On(pDX00->iLoadPort1Support1In);
	m_ledLoadPortIo[4].Set_On(pDX00->iLoadPort1Support1Out);
	m_ledLoadPortIo[5].Set_On(pDX00->iLoadPort1Support2In);
	m_ledLoadPortIo[6].Set_On(pDX00->iLoadPort1Support2Out);
	m_ledLoadPortIo[7].Set_On(pDX00->iLoadPort1SlideLock);
	m_ledLoadPortIo[8].Set_On(pDX00->iLoadPort1SlideUnlock);
	m_ledLoadPortIo[9].Set_On(pDX00->iLoadPort1SlideClose);
	m_ledLoadPortIo[10].Set_On(pDX00->iLoadPort1SlideOpen);

	m_ledLoadPortIo[11].Set_On(pDX00->iLoadPortAreaCheck);

// 	m_ledLoadPortIo[12].Set_On(pDX00->iLoadPort2Upper);
// 	m_ledLoadPortIo[13].Set_On(pDX00->iLoadPort2Lower);
	m_ledLoadPortIo[14].Set_On(pDX00->iLoadPort2Bottom);
	m_ledLoadPortIo[15].Set_On(pDX00->iLoadPort2Support1In);
	m_ledLoadPortIo[16].Set_On(pDX00->iLoadPort2Support1Out);
	m_ledLoadPortIo[17].Set_On(pDX00->iLoadPort2Support2In);
	m_ledLoadPortIo[18].Set_On(pDX00->iLoadPort2Support2Out);
	m_ledLoadPortIo[19].Set_On(pDX00->iLoadPort2SlideLock);
	m_ledLoadPortIo[20].Set_On(pDX00->iLoadPort2SlideUnlock);
	m_ledLoadPortIo[21].Set_On(pDX00->iLoadPort2SlideClose);
	m_ledLoadPortIo[22].Set_On(pDX00->iLoadPort2SlideOpen);

	m_ledLoadStageIo[0].Set_On(pDX01->iLoadStageExist);
	m_ledLoadStageIo[1].Set_On(pDX01->iLoadStageMasterIn);
	m_ledLoadStageIo[2].Set_On(pDX01->iLoadStageMasterOut);
	m_ledLoadStageIo[3].Set_On(pDX01->iLoadStageSlaveIn);
	m_ledLoadStageIo[4].Set_On(pDX01->iLoadStageSlaveOut);

	m_ledLoadPickerIo[0].Set_On(pDX01->iLoadPickerExist);
	m_ledLoadPickerIo[1].Set_On(pDX01->iLoadPickerUp);
	m_ledLoadPickerIo[2].Set_On(pDX01->iLoadPickerDown);
	m_ledLoadPickerIo[3].Set_On(pDX01->iLoadPickerMasterIn);
	m_ledLoadPickerIo[4].Set_On(pDX01->iLoadPickerMasterOut);
	m_ledLoadPickerIo[5].Set_On(pDX01->iLoadPickerSlaveIn);
	m_ledLoadPickerIo[6].Set_On(pDX01->iLoadPickerSlaveOut);

	m_ledAnglePortIo[0].Set_On(pDX02->iAnglePortExist);
	m_ledAnglePortIo[1].Set_On(pDX02->iAnglePortSupport1In);
	m_ledAnglePortIo[2].Set_On(pDX02->iAnglePortSupport1Out);
	m_ledAnglePortIo[3].Set_On(pDX02->iAnglePortSupport2In);
	m_ledAnglePortIo[4].Set_On(pDX02->iAnglePortSupport2Out);
	m_ledAnglePortIo[5].Set_On(pDX02->iAnglePortMasterIn);
	m_ledAnglePortIo[6].Set_On(pDX02->iAnglePortMasterOut);
	m_ledAnglePortIo[7].Set_On(pDX02->iAnglePortSlaveIn);
	m_ledAnglePortIo[8].Set_On(pDX02->iAnglePortSlaveOut);

	m_ledAngleStage1Io[0].Set_On(pDX02->iAngleStage1Normal);
	m_ledAngleStage1Io[1].Set_On(pDX02->iAngleStage1Rotate);
	m_ledAngleStage1Io[2].Set_On(pDX02->iAngleStage1AlignIn);
	m_ledAngleStage1Io[3].Set_On(pDX02->iAngleStage1AlignOut);
	m_ledAngleStage1Io[4].Set_On(pDX02->iAngleStage1Exist);

	m_ledAngleStage2Io[0].Set_On(pDX02->iAngleStage2Normal);
	m_ledAngleStage2Io[1].Set_On(pDX02->iAngleStage2Rotate);
	m_ledAngleStage2Io[2].Set_On(pDX02->iAngleStage2AlignIn);
	m_ledAngleStage2Io[3].Set_On(pDX02->iAngleStage2AlignOut);
	m_ledAngleStage2Io[4].Set_On(pDX02->iAngleStage2Exist);
}

///////////////////////////////////////////////////////////////////////////////

void CManualLoadDlg::OnBtnLoadPortIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_PORT_IO_0;
	
	DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
		
	if (nIndex == 0) { pDY00->oLoadPort1SupportIn = TRUE;  pDY00->oLoadPort1SupportOut = FALSE;	}
	if (nIndex == 1) { 
		if(pDX00->iLoadPort1Bottom){
			AfxMessageBox("Load port에 Tray가 있습니다. Port #1"); return;
		}
		pDY00->oLoadPort1SupportIn = FALSE; pDY00->oLoadPort1SupportOut = TRUE; 
	}
	if (nIndex == 2) { pDY00->oLoadPort1SlideLock = TRUE;  pDY00->oLoadPort1SlideUnlock = FALSE; }
	if (nIndex == 3) { pDY00->oLoadPort1SlideLock = FALSE; pDY00->oLoadPort1SlideUnlock = TRUE; }
	if (nIndex == 4) { pDY00->oLoadPort2SupportIn = TRUE;  pDY00->oLoadPort2SupportOut = FALSE; }
	if (nIndex == 5) { 
		if(pDX00->iLoadPort2Bottom){
			AfxMessageBox("Load port에 Tray가 있습니다. Port #1"); return;
		}
		pDY00->oLoadPort2SupportIn = FALSE; pDY00->oLoadPort2SupportOut = TRUE; 
	}
	if (nIndex == 6) { pDY00->oLoadPort2SlideLock = TRUE;  pDY00->oLoadPort2SlideUnlock = FALSE; }
	if (nIndex == 7) { pDY00->oLoadPort2SlideLock = FALSE; pDY00->oLoadPort2SlideUnlock = TRUE; }
	g_objAJinAXL.Write_Output(0);

	m_strLog.Format("[Manual Load] Load Port IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnLoadStageXClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE_Z)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_STAGE_X_0;

	if (!g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 0)) {	// Ready Down
		AfxMessageBox("Load Stage Z축 위치 확인 후 진행하세요."); return;
	}

	g_objCommon.Move_Position(AX_LOAD_STAGE_X, nIndex);

	m_strLog.Format("[Manual Load] Load Stage X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnLoadStageZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE_X)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_STAGE_Z_0;

	double dZ = g_objAJinAXL.Get_Position(AX_LOAD_STAGE_Z);
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();

	int nPosX = -1;
	for (int i = 0; i < 3; i++) { if (g_objCommon.Check_Position(AX_LOAD_STAGE_X, i)) { nPosX = i; break; } }
	if (nPosX < 0) { AfxMessageBox("Load Stage X축 위치 확인 후 진행하세요."); return; }

	switch (nIndex) {
	case 0:		// Ready Down
		if (dZ > pMoveData->dLoadStageZ[2] - 0.5) {
			if (nPosX == 0 && !g_objCommon.Get_LoadPortSupportOut(1) && !g_objCommon.Get_LoadTrayMasterSlaveOut()) {
				AfxMessageBox("Load Port1 Support Out 또는 Load Stage Master Slave Out 상태를 확인 후 진행하세요."); return;
			}
			if (nPosX == 1 && !g_objCommon.Get_LoadPortSupportOut(2) && !g_objCommon.Get_LoadTrayMasterSlaveOut()) {
				AfxMessageBox("Load Port2 Support Out 또는 Load Stage Master Slave Out 상태를 확인 후 진행하세요."); return;
			}
		}
		break;
	case 1:		// Support Up
		if (nPosX == 0 && !g_objCommon.Get_LoadPortSupportOut(1)) {
			if (pDX01->iLoadStageExist) { AfxMessageBox("Load Stage Tray를 제거 후 진행하세요."); return; }
			if (!g_objCommon.Get_LoadTrayMasterSlaveOut()) { AfxMessageBox("Load Stage Master Slave Out 상태를 확인 후 진행하세요."); return; }
		}
		if (nPosX == 1 && !g_objCommon.Get_LoadPortSupportOut(2)) {
			if (pDX01->iLoadStageExist) { AfxMessageBox("Load Stage Tray를 제거 후 진행하세요."); return; }
			if (!g_objCommon.Get_LoadTrayMasterSlaveOut()) { AfxMessageBox("Load Stage Master Slave Out 상태를 확인 후 진행하세요."); return; }
		}
		if (nPosX == 2) { AfxMessageBox("Load Stage X축 위치 확인 후 진행하세요."); return; }
		break;
	case 2:		// Support Down
		if (dZ > pMoveData->dLoadStageZ[2] - 0.5) {
			if (nPosX == 0 && !g_objCommon.Get_LoadPortSupportOut(1) && !g_objCommon.Get_LoadTrayMasterSlaveOut()) {
				AfxMessageBox("Load Port1 Support Out 또는 Load Stage Master Slave Out 상태를 확인 후 진행하세요."); return;
			}
			if (nPosX == 1 && !g_objCommon.Get_LoadPortSupportOut(2) && !g_objCommon.Get_LoadTrayMasterSlaveOut()) {
				AfxMessageBox("Load Port2 Support Out 또는 Load Stage Master Slave Out 상태를 확인 후 진행하세요."); return;
			}
		}
		if (nPosX == 2) { AfxMessageBox("Load Stage X축 위치 확인 후 진행하세요."); return; }
		break;
	case 3:		// Unload Up
		if (nPosX != 2) { AfxMessageBox("Load Stage X축 위치 확인 후 진행하세요."); return; }
		break;
	}

	g_objCommon.Move_Position(AX_LOAD_STAGE_Z, nIndex);

	m_strLog.Format("[Manual Load] Load Stage Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnLoadStageIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_STAGE_IO_0;

	DY_DATA_01 *pDY01 = g_objAJinAXL.Get_pDY01();

	if (nIndex == 0) pDY01->oLoadStageMasterIn = TRUE;
	if (nIndex == 1) pDY01->oLoadStageMasterIn = FALSE;
	if (nIndex == 2) pDY01->oLoadStageSlaveIn = TRUE;
	if (nIndex == 3) pDY01->oLoadStageSlaveIn = FALSE;

	g_objAJinAXL.Write_Output(1);

	m_strLog.Format("[Manual Load] Load Stage IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnLoadPickerYClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_LOAD_PICKER_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_UNIT_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_UNIT_R)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_PICKER_Y_0;

	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	if (!pDX01->iLoadPickerUp || pDX01->iLoadPickerDown) { AfxMessageBox("Load Picker Z Up 상태를 확인 후 진행하세요."); return; }

	if (!g_objCommon.Check_Position(AX_ANGLE_UNIT_X, 0) || !g_objCommon.Check_Position(AX_ANGLE_UNIT_R, 0)) {
		AfxMessageBox("Angle Unit X축, R축 Ready 위치 확인 후 진행하세요."); return;
	}

	g_objCommon.Move_Position(AX_LOAD_PICKER_Y, nIndex);

	m_strLog.Format("[Manual Load] Load Picker Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnLoadPickerIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_PICKER_IO_0;

	DY_DATA_01 *pDY01 = g_objAJinAXL.Get_pDY01();

	if (nIndex == 0) { pDY01->oLoadPickerUp = TRUE;        pDY01->oLoadPickerDown = FALSE; }
	if (nIndex == 1) { pDY01->oLoadPickerUp = FALSE;       pDY01->oLoadPickerDown = TRUE; }
	if (nIndex == 2) { pDY01->oLoadPickerMasterIn = TRUE;  pDY01->oLoadPickerMasterOut = FALSE; }
	if (nIndex == 3) { pDY01->oLoadPickerMasterIn = FALSE; pDY01->oLoadPickerMasterOut = TRUE; }
	if (nIndex == 4) { pDY01->oLoadPickerSlaveIn = TRUE;   pDY01->oLoadPickerSlaveOut = FALSE; }
	if (nIndex == 5) { pDY01->oLoadPickerSlaveIn = FALSE;  pDY01->oLoadPickerSlaveOut = TRUE; }

	g_objAJinAXL.Write_Output(1);

	m_strLog.Format("[Manual Load] Load Picker IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnAngleUnitYClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_UNIT_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_PICKER_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_ANGLE_UNIT_Y_0;

	if (!g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0)) { AfxMessageBox("Load Picker Y축 위치 확인 후 진행하세요."); return; }

	if (nIndex == 2 || nIndex == 3) {	// Pitch Move
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		int nDir = (nIndex == 2 ? 1 : -1);
		double dMove = pEquipData->dTrayPitchY * nDir;
		g_objAJinAXL.Move_Relative(AX_ANGLE_UNIT_Y, dMove);
	} else {
		g_objCommon.Move_Position(AX_ANGLE_UNIT_Y, nIndex);
	}

	m_strLog.Format("[Manual Load] Angle Unit Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnAngleUnitXClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_UNIT_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_PICKER_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_ANGLE_UNIT_X_0;

	if (!g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0)) { AfxMessageBox("Load Picker Y축 위치 확인 후 진행하세요."); return; }

	if (nIndex == 2 || nIndex == 3) {	// Pitch Move
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		int nDir = (nIndex == 2 ? 1 : -1);
		double dMove = pEquipData->dTrayPitchX * nDir;
		g_objAJinAXL.Move_Relative(AX_ANGLE_UNIT_X, dMove);
	} else {
		g_objCommon.Move_Position(AX_ANGLE_UNIT_X, nIndex);
	}

	m_strLog.Format("[Manual Load] Angle Unit X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnAngleUnitRClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_UNIT_R)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_PICKER_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_ANGLE_UNIT_R_0;

	if (!g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0)) { AfxMessageBox("Load Picker Y축 위치 확인 후 진행하세요."); return; }

	g_objCommon.Move_Position(AX_ANGLE_UNIT_R, nIndex);

	m_strLog.Format("[Manual Load] Angle Unit R (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnVisionAngleZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_VISION_ANGLE_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_VISION_ANGLE_Z_0;

	g_objCommon.Move_Position(AX_VISION_ANGLE_Z, nIndex);

	m_strLog.Format("[Manual Load] Vision Angle Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnVisionAlignZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_VISION_ALIGN_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_VISION_ALIGN_Z_0;

	g_objCommon.Move_Position(AX_VISION_ALIGN_Z, nIndex);

	m_strLog.Format("[Manual Load] Vision Align Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnAnglePortIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_ANGLE_PORT_IO_0;

	DY_DATA_02 *pDY02 = g_objAJinAXL.Get_pDY02();

	if (nIndex == 0) { pDY02->oAnglePortSupportIn = TRUE;  pDY02->oAnglePortSupportOut = FALSE; }
	if (nIndex == 1) { pDY02->oAnglePortSupportIn = FALSE; pDY02->oAnglePortSupportOut = TRUE; }
	if (nIndex == 2) { pDY02->oAnglePortMasterIn = TRUE;   pDY02->oAnglePortMasterOut = FALSE; }
	if (nIndex == 3) { pDY02->oAnglePortMasterIn = FALSE;  pDY02->oAnglePortMasterOut = TRUE; }
	if (nIndex == 4) { pDY02->oAnglePortSlaveIn = TRUE;    pDY02->oAnglePortSlaveOut = FALSE; }
	if (nIndex == 5) { pDY02->oAnglePortSlaveIn = FALSE;   pDY02->oAnglePortSlaveOut = TRUE; }

	g_objAJinAXL.Write_Output(2);

	m_strLog.Format("[Manual Load] Angle Port IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnAngleStage1YClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE1_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE2_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_ANGLE_STAGE1_Y_0;

	if (g_objCommon.Check_Position(AX_BTM1_PICKER_X, 0) && !g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0)) {
		AfxMessageBox("Btm1 Picker X축 Z축 위치 확인 후 진행하세요."); return;
	}

	int nPosZ1 = -1, nPosZ2 = -1;
	for (int i = 0; i < 3; i++) { if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, i)) { nPosZ1 = i; break; } }
	for (int i = 0; i < 3; i++) { if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, i)) { nPosZ2 = i; break; } }
	if (nPosZ1 < 0 || nPosZ2 < 0) { AfxMessageBox("Angle Stage1 Z축과 Angle Stage2 Z축 위치 확인 후 진행하세요."); return; }

	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	double dY1 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE1_Y);
	if (nPosZ1 == 0) {
		if (nPosZ2 == 0) { AfxMessageBox("Angle Stage2 Z축 위치 확인 후 진행하세요."); return; }
		if (nIndex < 5) g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, nIndex);	// Load, Ready, L1, L2, Unload

	} else if (nPosZ1 == 1 || nPosZ1 == 2) {
		if (nPosZ2 == 1 || nPosZ2 == 2) { AfxMessageBox("Angle Stage2 Z축 위치 확인 후 진행하세요."); return; }
		if (dY1 < pMoveData->dAngleStage1Y[1] - 0.5) { AfxMessageBox("Angle Stage1 Y축 위치 확인 후 진행하세요."); return; }
		if (nIndex > 0 && nIndex < 5) g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, nIndex);	// Ready, L1, L2, Unload
		else if (nIndex == 5 || nIndex == 6) {	// Pitch Move
			EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
			int nDir = (nIndex == 5 ? 1 : -1);
 			double dMove = pEquipData->dTrayPitchY * nDir;
			g_objAJinAXL.Move_Relative(AX_ANGLE_STAGE1_Y, dMove);
		}
	}

	m_strLog.Format("[Manual Load] Angle Stage1 Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnAngleStage1ZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE1_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE2_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_ANGLE_STAGE1_Z_0;

	int nPosY1 = -1, nPosY2 = -1;
	for (int i = 0; i < 5; i++) { if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, i)) { nPosY1 = i; break; } }
	for (int i = 0; i < 5; i++) { if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, i)) { nPosY2 = i; break; } }
	if (nPosY1 < 0 || nPosY2 < 0) { AfxMessageBox("Angle Stage1 Y축과 Angle Stage2 Y축 위치 확인 후 진행하세요."); return; }

// 	if (nPosY1 == 0 && nPosY2 == 0) { AfxMessageBox("Angle Stage2 Y축 위치 확인 후 진행하세요."); return; }
// 	if (nPosY1 == 1 && nPosY2 == 1) { AfxMessageBox("Angle Stage2 Y축 위치 확인 후 진행하세요."); return; }
// 	if ((nPosY1 == 2 || nPosY1 == 3) && (nPosY2 == 2 || nPosY2 == 3)) { AfxMessageBox("Angle Stage2 Y축 위치 확인 후 진행하세요."); return; }
// 	if (nPosY1 == 4 && nPosY2 == 4) { AfxMessageBox("Angle Stage2 Y축 위치 확인 후 진행하세요."); return; }

	double dY1 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE1_Y);
	double dY2 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE2_Y);
	if (fabs(dY1 - dY2) < 330.0) { AfxMessageBox("Angle Stage2 Y축 위치 확인 후 진행하세요."); return; }

	if (nIndex == 2 && nPosY1 == 0) { AfxMessageBox("Angle Stage1 Y축과 위치 확인 후 진행하세요."); return; }
	if ((nIndex == 1 || nIndex == 3 || nIndex == 4) && nPosY1 != 0) { AfxMessageBox("Angle Stage1 Y축과 위치 확인 후 진행하세요.(Load Position)"); return; }

	g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, nIndex);

	m_strLog.Format("[Manual Load] Angle Stage1 Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnAngleStage1IoClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE1_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_EMPTY_TRANS1_X)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_ANGLE_STAGE1_IO_0;

	double dY = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE1_Y);
	double dZ = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE1_Z);
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();

	if (nIndex == 0 || nIndex == 1) {
		if (dZ > pMoveData->dAngleStage1Z[0] + 1.0) {
			if (dY < pMoveData->dAngleStage1Y[1] - 1.0) { AfxMessageBox("Angle Stage1 Y축 위치 확인 후 진행하세요."); return; }
			if (g_objCommon.Check_Position(AX_BTM1_PICKER_X, 0) && !g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0)) {
				AfxMessageBox("Btm1 Picker X축 Z축 위치 확인 후 진행하세요."); return;
			}
			DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();
			if (g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 0) && (!pDX13->iEmptyTrans1Up || pDX13->iEmptyTrans1Down)) {
				AfxMessageBox("Empty Trans1 X축과 Up상태 확인 후 진행하세요."); return;
			}
		}
	}

	DY_DATA_02 *pDY02 = g_objAJinAXL.Get_pDY02();
	if (nIndex == 0) pDY02->oAngleStage1Rotate = FALSE;
	if (nIndex == 1) pDY02->oAngleStage1Rotate = TRUE;
	if (nIndex == 2) pDY02->oAngleStage1AlignIn= TRUE;
	if (nIndex == 3) pDY02->oAngleStage1AlignIn= FALSE;
	g_objAJinAXL.Write_Output(2);
}

void CManualLoadDlg::OnBtnAngleStage2YClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE2_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE2_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_ANGLE_STAGE2_Y_0;

	if (g_objCommon.Check_Position(AX_BTM1_PICKER_X, 0) && !g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0)) {
		AfxMessageBox("Btm1 Picker X축 Z축 위치 확인 후 진행하세요."); return;
	}

	int nPosZ2 = -1, nPosZ1 = -1;
	for (int i = 0; i < 3; i++) { if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, i)) { nPosZ2 = i; break; } }
	for (int i = 0; i < 3; i++) { if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, i)) { nPosZ1 = i; break; } }
	if (nPosZ2 < 0 || nPosZ1 < 0) { AfxMessageBox("Angle Stage2 Z축과 Angle Stage1 Z축 위치 확인 후 진행하세요."); return; }

	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	double dY2 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE2_Y);
	if (nPosZ2 == 0) {
		if (nPosZ1 == 0) { AfxMessageBox("Angle Stage1 Z축 위치 확인 후 진행하세요."); return; }
		if (nIndex < 5) g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, nIndex);	// Load, Ready, L1, L2, Unload

	} else if (nPosZ2 == 1 || nPosZ2 == 2) {
		if (nPosZ1 == 1 || nPosZ1 == 2) { AfxMessageBox("Angle Stage1 Z축 위치 확인 후 진행하세요."); return; }
		if (dY2 < pMoveData->dAngleStage2Y[1] - 0.5) { AfxMessageBox("Angle Stage2 Y축 위치 확인 후 진행하세요."); return; }
		if (nIndex > 0 && nIndex < 5) g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, nIndex);	// Ready, L1, L2, Unload
		else if (nIndex == 5 || nIndex == 6) {	// Pitch Move
			EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
			int nDir = (nIndex == 5 ? 1 : -1);
			double dMove = pEquipData->dTrayPitchY * nDir;
			g_objAJinAXL.Move_Relative(AX_ANGLE_STAGE2_Y, dMove);
		}
	}

	m_strLog.Format("[Manual Load] Angle Stage2 Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnAngleStage2ZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE2_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE2_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE1_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_ANGLE_STAGE2_Z_0;

	int nPosY2 = -1, nPosY1 = -1;
	for (int i = 0; i < 5; i++) { if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, i)) { nPosY2 = i; break; } }
	for (int i = 0; i < 5; i++) { if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, i)) { nPosY1 = i; break; } }
	if (nPosY2 < 0 || nPosY1 < 0) { AfxMessageBox("Angle Stage1 Y축과 Angle Stage1 Y축 위치 확인 후 진행하세요."); return; }

// 	if (nPosY2 == 0 && nPosY1 == 0) { AfxMessageBox("Angle Stage1 Y축 위치 확인 후 진행하세요."); return; }
// 	if (nPosY2 == 1 && nPosY1 == 1) { AfxMessageBox("Angle Stage1 Y축 위치 확인 후 진행하세요."); return; }
// 	if ((nPosY2 == 2 || nPosY2 == 3) && (nPosY1 == 2 || nPosY1 == 3)) { AfxMessageBox("Angle Stage1 Y축 위치 확인 후 진행하세요."); return; }
// 	if (nPosY2 == 4 && nPosY1 == 4) { AfxMessageBox("Angle Stage1 Y축 위치 확인 후 진행하세요."); return; }

	double dY2 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE2_Y);
	double dY1 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE1_Y);
	if (fabs(dY2 - dY1) < 330.0) { AfxMessageBox("Angle Stage1 Y축 위치 확인 후 진행하세요."); return; }

	if (nIndex == 2 && nPosY2 == 0) { AfxMessageBox("Angle Stage2 Y축과 위치 확인 후 진행하세요."); return; }
	if ((nIndex == 1 || nIndex == 3 || nIndex == 4) && nPosY2 != 0) { AfxMessageBox("Angle Stage2 Y축과 위치 확인 후 진행하세요.(Load Position)"); return; }

	g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, nIndex);

	m_strLog.Format("[Manual Load] Angle Stage2 Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnBtnAngleStage2IoClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE2_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_ANGLE_STAGE2_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_EMPTY_TRANS1_X)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_ANGLE_STAGE2_IO_0;

	double dY = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE2_Y);
	double dZ = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE2_Z);
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();

	if (nIndex == 0 || nIndex == 1) {
		if (dZ > pMoveData->dAngleStage2Z[0] + 1.0) {
			if (dY < pMoveData->dAngleStage2Y[1] - 1.0) { AfxMessageBox("Angle Stage2 Y축 위치 확인 후 진행하세요."); return; }
			if (g_objCommon.Check_Position(AX_BTM1_PICKER_X, 0) && !g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0)) {
				AfxMessageBox("Btm1 Picker X축 Z축 위치 확인 후 진행하세요."); return;
			}
			DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();
			if (g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 0) && (!pDX13->iEmptyTrans1Up || pDX13->iEmptyTrans1Down)) {
				AfxMessageBox("Empty Trans1 X축과 Up상태 확인 후 진행하세요."); return;
			}
		}
	}

	DY_DATA_02 *pDY02 = g_objAJinAXL.Get_pDY02();
	if (nIndex == 0) pDY02->oAngleStage2Rotate = FALSE;
	if (nIndex == 1) pDY02->oAngleStage2Rotate = TRUE;
	if (nIndex == 2) pDY02->oAngleStage2AlignIn= TRUE;
	if (nIndex == 3) pDY02->oAngleStage2AlignIn= FALSE;
	g_objAJinAXL.Write_Output(2);
}

///////////////////////////////////////////////////////////////////////////////
