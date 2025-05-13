// SetupMoveTab1Dlg.cpp : ±∏«ˆ ∆ƒ¿œ¿‘¥œ¥Ÿ.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "SetupMoveTab1Dlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "DataManager.h"
#include "Common.h"
#include "MESInterface.h"

#include "AJinDefine.h"

// CSetupMoveTab1Dlg ¥Î»≠ ªÛ¿⁄¿‘¥œ¥Ÿ.

IMPLEMENT_DYNAMIC(CSetupMoveTab1Dlg, CDialogEx)

CSetupMoveTab1Dlg::CSetupMoveTab1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMoveTab1Dlg::IDD, pParent)
{
}

CSetupMoveTab1Dlg::~CSetupMoveTab1Dlg()
{
}

void CSetupMoveTab1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 17; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 61; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_LOAD_STAGE_X_0 + i, m_stcLoadStageX[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_LOAD_STAGE_Z_0 + i, m_stcLoadStageZ[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_LOAD_PICKER_Y_0 + i, m_stcLoadPickerY[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_ANGLE_UNIT_Y_0 + i, m_stcAngleUnitY[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_ANGLE_UNIT_X_0 + i, m_stcAngleUnitX[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_ANGLE_UNIT_R_0 + i, m_stcAngleUnitR[i]);
	for (int i = 0; i < 1; i++) DDX_Control(pDX, IDC_STC_VISION_ANGLE_Z_0 + i, m_stcVisionAngleZ[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_VISION_ALIGN_Z_0 + i, m_stcVisionAlignZ[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_STC_ANGLE_STAGE1_Y_0 + i, m_stcAngleStage1Y[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_ANGLE_STAGE1_Z_0 + i, m_stcAngleStage1Z[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_STC_ANGLE_STAGE2_Y_0 + i, m_stcAngleStage2Y[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_ANGLE_STAGE2_Z_0 + i, m_stcAngleStage2Z[i]);
	for (int i = 0; i < 9; i++) DDX_Control(pDX, IDC_STC_BTM1_PICKER_X_0 + i, m_stcBtm1PickerX[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_BTM1_PICKER_Z_0 + i, m_stcBtm1PickerZ[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_BTM1_PICKER_P1_0 + i, m_stcBtm1PickerP1[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_BTM1_PICKER_P2_0 + i, m_stcBtm1PickerP2[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_BTM1_SHIFT_Y_0 + i, m_stcBtm1ShiftY[i]);
	
}

BEGIN_MESSAGE_MAP(CSetupMoveTab1Dlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LOAD_STAGE_X_0, IDC_STC_LOAD_STAGE_X_2, OnStcLoadStageXClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LOAD_STAGE_Z_0, IDC_STC_LOAD_STAGE_Z_3, OnStcLoadStageZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LOAD_PICKER_Y_0, IDC_STC_LOAD_PICKER_Y_1, OnStcLoadPickerYClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_ANGLE_UNIT_Y_0, IDC_STC_ANGLE_UNIT_Y_1, OnStcAngleUnitYClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_ANGLE_UNIT_X_0, IDC_STC_ANGLE_UNIT_X_1, OnStcAngleUnitXClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_ANGLE_UNIT_R_0, IDC_STC_ANGLE_UNIT_R_1, OnStcAngleUnitRClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_VISION_ANGLE_Z_0, IDC_STC_VISION_ANGLE_Z_0, OnStcVisionAngleZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_VISION_ALIGN_Z_0, IDC_STC_VISION_ALIGN_Z_1, OnStcVisionAlignZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_ANGLE_STAGE1_Y_0, IDC_STC_ANGLE_STAGE1_Y_4, OnStcAngleStage1YClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_ANGLE_STAGE1_Z_0, IDC_STC_ANGLE_STAGE1_Z_5, OnStcAngleStage1ZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_ANGLE_STAGE2_Y_0, IDC_STC_ANGLE_STAGE2_Y_4, OnStcAngleStage2YClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_ANGLE_STAGE2_Z_0, IDC_STC_ANGLE_STAGE2_Z_5, OnStcAngleStage2ZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_BTM1_PICKER_X_0, IDC_STC_BTM1_PICKER_X_8, OnStcBtm1PickerXClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_BTM1_PICKER_Z_0, IDC_STC_BTM1_PICKER_Z_5, OnStcBtm1PickerZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_BTM1_PICKER_P1_0, IDC_STC_BTM1_PICKER_P1_1, OnStcBtm1PickerP1Click)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_BTM1_PICKER_P2_0, IDC_STC_BTM1_PICKER_P2_1, OnStcBtm1PickerP2Click)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_BTM1_SHIFT_Y_0, IDC_STC_BTM1_SHIFT_Y_1, OnStcBtm1ShiftYClick)
END_MESSAGE_MAP()

// CSetupMoveTab1Dlg ∏ﬁΩ√¡ˆ √≥∏Æ±‚¿‘¥œ¥Ÿ.

BOOL CSetupMoveTab1Dlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  ø©±‚ø° √ﬂ∞° √ ±‚»≠ ¿€æ˜¿ª √ﬂ∞°«’¥œ¥Ÿ.
	SetWindowPos(NULL, 5, 65, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// øπø‹: OCX º”º∫ ∆‰¿Ã¡ˆ¥¬ FALSE∏¶ π›»Ø«ÿæﬂ «’¥œ¥Ÿ.
}

BOOL CSetupMoveTab1Dlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupMoveTab1Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Display_MoveData();

	g_objLogFile.Save_HandlerLog("[Setup Move] Show Window - Tab1");
}

void CSetupMoveTab1Dlg::OnStcLoadStageXClick(UINT nID)
{
	int ID = nID - IDC_STC_LOAD_STAGE_X_0;

	CString strOld, strNew;
	m_stcLoadStageX[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_LOAD_STAGE_X) != IDOK) return;

	m_stcLoadStageX[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcLoadStageZClick(UINT nID)
{
	int ID = nID - IDC_STC_LOAD_STAGE_Z_0;

	CString strOld, strNew;
	m_stcLoadStageZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_LOAD_STAGE_Z) != IDOK) return;

	m_stcLoadStageZ[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcLoadPickerYClick(UINT nID)
{
	int ID = nID - IDC_STC_LOAD_PICKER_Y_0;

	CString strOld, strNew;
	m_stcLoadPickerY[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_LOAD_PICKER_Y) != IDOK) return;

	m_stcLoadPickerY[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcAngleUnitYClick(UINT nID)
{
	int ID = nID - IDC_STC_ANGLE_UNIT_Y_0;

	CString strOld, strNew;
	m_stcAngleUnitY[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_ANGLE_UNIT_Y) != IDOK) return;

	m_stcAngleUnitY[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcAngleUnitXClick(UINT nID)
{
	int ID = nID - IDC_STC_ANGLE_UNIT_X_0;

	CString strOld, strNew;
	m_stcAngleUnitX[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_ANGLE_UNIT_X) != IDOK) return;

	m_stcAngleUnitX[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcAngleUnitRClick(UINT nID)
{
	int ID = nID - IDC_STC_ANGLE_UNIT_R_0;

	CString strOld, strNew;
	m_stcAngleUnitR[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_ANGLE_UNIT_R) != IDOK) return;

	m_stcAngleUnitR[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcVisionAngleZClick(UINT nID)
{
	int ID = nID - IDC_STC_VISION_ANGLE_Z_0;

	CString strOld, strNew;
	m_stcVisionAngleZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_VISION_ANGLE_Z) != IDOK) return;

	m_stcVisionAngleZ[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcVisionAlignZClick(UINT nID)
{
	int ID = nID - IDC_STC_VISION_ALIGN_Z_0;

	CString strOld, strNew;
	m_stcVisionAlignZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_VISION_ALIGN_Z) != IDOK) return;

	m_stcVisionAlignZ[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcAngleStage1YClick(UINT nID)
{
	int ID = nID - IDC_STC_ANGLE_STAGE1_Y_0;

	CString strOld, strNew;
	m_stcAngleStage1Y[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_ANGLE_STAGE1_Y) != IDOK) return;

	m_stcAngleStage1Y[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcAngleStage1ZClick(UINT nID)
{
	int ID = nID - IDC_STC_ANGLE_STAGE1_Z_0;

	CString strOld, strNew;
	m_stcAngleStage1Z[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_ANGLE_STAGE1_Z) != IDOK) return;

	m_stcAngleStage1Z[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcAngleStage2YClick(UINT nID)
{
	int ID = nID - IDC_STC_ANGLE_STAGE2_Y_0;

	CString strOld, strNew;
	m_stcAngleStage2Y[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_ANGLE_STAGE2_Y) != IDOK) return;

	m_stcAngleStage2Y[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcAngleStage2ZClick(UINT nID)
{
	int ID = nID - IDC_STC_ANGLE_STAGE2_Z_0;

	CString strOld, strNew;
	m_stcAngleStage2Z[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_ANGLE_STAGE2_Z) != IDOK) return;

	m_stcAngleStage2Z[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcBtm1PickerXClick(UINT nID)
{
	int ID = nID - IDC_STC_BTM1_PICKER_X_0;

	CString strOld, strNew;
	m_stcBtm1PickerX[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_BTM1_PICKER_X) != IDOK) return;

	m_stcBtm1PickerX[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcBtm1PickerZClick(UINT nID)
{
	int ID = nID - IDC_STC_BTM1_PICKER_Z_0;

	CString strOld, strNew;
	m_stcBtm1PickerZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_BTM1_PICKER_Z) != IDOK) return;

	m_stcBtm1PickerZ[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcBtm1PickerP1Click(UINT nID)
{
	int ID = nID - IDC_STC_BTM1_PICKER_P1_0;

	CString strOld, strNew;
	m_stcBtm1PickerP1[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_BTM1_PICKER_P1) != IDOK) return;

	m_stcBtm1PickerP1[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcBtm1PickerP2Click(UINT nID)
{
	int ID = nID - IDC_STC_BTM1_PICKER_P2_0;

	CString strOld, strNew;
	m_stcBtm1PickerP2[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_BTM1_PICKER_P2) != IDOK) return;

	m_stcBtm1PickerP2[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcBtm1ShiftYClick(UINT nID)
{
	int ID = nID - IDC_STC_BTM1_SHIFT_Y_0;

	CString strOld, strNew;
	m_stcBtm1ShiftY[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_BTM1_SHIFT_Y) != IDOK) return;

	m_stcBtm1ShiftY[ID].SetWindowText(strNew);
}


///////////////////////////////////////////////////////////////////////////////
// User Functions

void CSetupMoveTab1Dlg::Initial_Controls() 
{
	for (int i = 0; i < 17; i++) m_Group[i].Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 61; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	for (int i = 0; i < 3; i++)  m_stcLoadStageX[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 4; i++)  m_stcLoadStageZ[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 2; i++)  m_stcLoadPickerY[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 2; i++)  m_stcAngleUnitY[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 2; i++)  m_stcAngleUnitX[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 2; i++)  m_stcAngleUnitR[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 1; i++)  m_stcVisionAngleZ[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 2; i++)  m_stcVisionAlignZ[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 5; i++)  m_stcAngleStage1Y[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 6; i++)  m_stcAngleStage1Z[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 5; i++)  m_stcAngleStage2Y[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 6; i++)  m_stcAngleStage2Z[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 9; i++)  m_stcBtm1PickerX[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 6; i++)  m_stcBtm1PickerZ[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 2; i++)  m_stcBtm1PickerP1[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 2; i++)  m_stcBtm1PickerP2[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 2; i++)  m_stcBtm1ShiftY[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
}

void CSetupMoveTab1Dlg::Display_MoveData()
{
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	CString strData;

	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dLoadStageX[i]);		m_stcLoadStageX[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%0.3lf", pMoveData->dLoadStageZ[i]);		m_stcLoadStageZ[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dLoadPickerY[i]);		m_stcLoadPickerY[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dAngleUnitY[i]);		m_stcAngleUnitY[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dAngleUnitX[i]);		m_stcAngleUnitX[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dAngleUnitR[i]);		m_stcAngleUnitR[i].SetWindowText(strData); }
	for (int i = 0; i < 1; i++) { strData.Format("%0.3lf", pMoveData->dVisionAngleZ[i]);	m_stcVisionAngleZ[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dVisonAlignZ[i]);		m_stcVisionAlignZ[i].SetWindowText(strData); }
	for (int i = 0; i < 5; i++) { strData.Format("%0.3lf", pMoveData->dAngleStage1Y[i]);	m_stcAngleStage1Y[i].SetWindowText(strData); }
	for (int i = 0; i < 6; i++) { strData.Format("%0.3lf", pMoveData->dAngleStage1Z[i]);	m_stcAngleStage1Z[i].SetWindowText(strData); }
	for (int i = 0; i < 5; i++) { strData.Format("%0.3lf", pMoveData->dAngleStage2Y[i]);	m_stcAngleStage2Y[i].SetWindowText(strData); }
	for (int i = 0; i < 6; i++) { strData.Format("%0.3lf", pMoveData->dAngleStage2Z[i]);	m_stcAngleStage2Z[i].SetWindowText(strData); }
	for (int i = 0; i < 9; i++) { strData.Format("%0.3lf", pMoveData->dBtm1PickerX[i]);		m_stcBtm1PickerX[i].SetWindowText(strData); }
	for (int i = 0; i < 6; i++) { strData.Format("%0.3lf", pMoveData->dBtm1PickerZ[i]);		m_stcBtm1PickerZ[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dBtm1PickerP1[i]);	m_stcBtm1PickerP1[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dBtm1PickerP2[i]);	m_stcBtm1PickerP2[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dBtm1ShiftY[i]);	m_stcBtm1ShiftY[i].SetWindowText(strData); }
}

void CSetupMoveTab1Dlg::Save_MoveData()
{
 	CIniFileCS INI(gData.sEnvPath + "\\MoveData.ini");
	if (!INI.Check_File()) { AfxMessageBox("MoveData.ini File Not Found!!!"); return; }

	g_objCommon.Backup_File(gData.sEnvPath, "MoveData");

	CString strKey, strData;
	double dData;

	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcLoadStageX[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("00_LOAD_STAGE_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i);	m_stcLoadStageZ[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("01_LOAD_STAGE_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcLoadPickerY[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("02_LOAD_PICKER_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcAngleUnitY[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("03_ANGLE_UNIT_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcAngleUnitX[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("04_ANGLE_UNIT_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcAngleUnitR[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("05_ANGLE_UNIT_R", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 1; i++) { strKey.Format("%02d", i);	m_stcVisionAngleZ[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("06_VISION_ANGLE_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcVisionAlignZ[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("07_VISION_ALIGN_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i);	m_stcAngleStage1Y[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("08_ANGLE_STAGE1_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcAngleStage1Z[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("09_ANGLE_STAGE1_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i);	m_stcAngleStage2Y[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("10_ANGLE_STAGE2_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcAngleStage2Z[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("11_ANGLE_STAGE2_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 9; i++) { strKey.Format("%02d", i);	m_stcBtm1PickerX[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("12_BTM1_PICKER_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcBtm1PickerP1[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("14_BTM1_PICKER_P1", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcBtm1PickerP2[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("15_BTM1_PICKER_P2", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcBtm1ShiftY[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("16_BTM1_SHIFT_Y", strKey, dData, "%0.3lf"); }

	for (int i = 0; i < 6; i++) {
		strKey.Format("%02d", i);	m_stcBtm1PickerZ[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("13_BTM1_PICKER_Z", strKey, dData, "%0.3lf");

		// RMS «◊∏Ò ¿˙¿Â
		if (i == 1) g_objMES.Save_AviRmsData("BTM1 Picker Z Angle Stage Down Position", strData);
		if (i == 3) g_objMES.Save_AviRmsData("BTM1 Picker Z Inspect Down Position", strData);
	}

	g_objLogFile.Save_HandlerLog("[Setup - MoveTab Dialog 1] save");

	Cancel_MoveData();
}


void CSetupMoveTab1Dlg::Save_MoveDataForAll()
{
	CIniFileCS INI(gData.sEnvPath + "\\AllParam.ini");
	if (!INI.Check_File()) { AfxMessageBox("AllParam.ini File Not Found!!!"); return; }

	//g_objCommon.Backup_File(gData.sEnvPath, "MoveData");

	CString strKey, strData;
	double dData;

	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcLoadStageX[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("00_LOAD_STAGE_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i);	m_stcLoadStageZ[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("01_LOAD_STAGE_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcLoadPickerY[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("02_LOAD_PICKER_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcAngleUnitY[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("03_ANGLE_UNIT_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcAngleUnitX[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("04_ANGLE_UNIT_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcAngleUnitR[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("05_ANGLE_UNIT_R", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 1; i++) { strKey.Format("%02d", i);	m_stcVisionAngleZ[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("06_VISION_ANGLE_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcVisionAlignZ[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("07_VISION_ALIGN_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i);	m_stcAngleStage1Y[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("08_ANGLE_STAGE1_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcAngleStage1Z[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("09_ANGLE_STAGE1_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i);	m_stcAngleStage2Y[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("10_ANGLE_STAGE2_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcAngleStage2Z[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("11_ANGLE_STAGE2_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 9; i++) { strKey.Format("%02d", i);	m_stcBtm1PickerX[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("12_BTM1_PICKER_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcBtm1PickerP1[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("14_BTM1_PICKER_P1", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcBtm1PickerP2[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("15_BTM1_PICKER_P2", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcBtm1ShiftY[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("16_BTM1_SHIFT_Y", strKey, dData, "%0.3lf"); }

	
}

void CSetupMoveTab1Dlg::Cancel_MoveData()
{
	g_objDataManager.Read_MoveData();
	Display_MoveData();
}

///////////////////////////////////////////////////////////////////////////////
