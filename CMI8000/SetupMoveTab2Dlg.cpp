// SetupMoveTab2Dlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "SetupMoveTab2Dlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "DataManager.h"
#include "Common.h"
#include "MESInterface.h"

#include "AJinDefine.h"

// CSetupMoveTab2Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupMoveTab2Dlg, CDialogEx)

CSetupMoveTab2Dlg::CSetupMoveTab2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMoveTab2Dlg::IDD, pParent)
{
}

CSetupMoveTab2Dlg::~CSetupMoveTab2Dlg()
{
}

void CSetupMoveTab2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 15; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 55; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_INSPECT_STAGE1_X_0 + i, m_stcInspectStage1X[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_INSPECT_STAGE2_X_0 + i, m_stcInspectStage2X[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_INSPECT_STAGE3_X_0 + i, m_stcInspectStage3X[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_MODULE_ALIGN_Y_0 + i, m_stcModuleAlignY[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_MODULE_ALIGN_Z_0 + i, m_stcModuleAlignZ[i]);
	for (int i = 0; i < 1; i++) DDX_Control(pDX, IDC_STC_VISION_TOP1_Z_0 + i, m_stcVisionTop1Z[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_TOP1_MIRROR_Z_0 + i, m_stcTop1MirrorZ[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_TOP2_SHIFT_Y_0 + i, m_stcTop2ShiftY[i]);
	for (int i = 0; i < 1; i++) DDX_Control(pDX, IDC_STC_VISION_TOP2_Z_0 + i, m_stcVisionTop2Z[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_BTM2_PICKER_X_0 + i, m_stcBtm2PickerX[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_BTM2_PICKER_Z_0 + i, m_stcBtm2PickerZ[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_BTM2_PICKER_P1_0 + i, m_stcBtm2PickerP1[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_BTM2_PICKER_P2_0 + i, m_stcBtm2PickerP2[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_BUFFER_STAGE1_Y_0 + i, m_stcBufferStage1Y[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_BUFFER_STAGE2_Y_0 + i, m_stcBufferStage2Y[i]);
}

BEGIN_MESSAGE_MAP(CSetupMoveTab2Dlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_INSPECT_STAGE1_X_0, IDC_STC_INSPECT_STAGE1_X_5, OnStcInspectStage1XClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_INSPECT_STAGE2_X_0, IDC_STC_INSPECT_STAGE2_X_5, OnStcInspectStage2XClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_INSPECT_STAGE3_X_0, IDC_STC_INSPECT_STAGE3_X_5, OnStcInspectStage3XClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_MODULE_ALIGN_Y_0, IDC_STC_MODULE_ALIGN_Y_2, OnStcModuleAlignYClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_MODULE_ALIGN_Z_0, IDC_STC_MODULE_ALIGN_Z_1, OnStcModuleAlignZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_VISION_TOP1_Z_0, IDC_STC_VISION_TOP1_Z_0, OnStcVisionTop1ZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_TOP1_MIRROR_Z_0, IDC_STC_TOP1_MIRROR_Z_1, OnStcTop1MirrorZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_TOP2_SHIFT_Y_0, IDC_STC_TOP2_SHIFT_Y_1, OnStcTop2ShiftYClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_VISION_TOP2_Z_0, IDC_STC_VISION_TOP2_Z_0, OnStcVisionTop2ZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_BTM2_PICKER_X_0, IDC_STC_BTM2_PICKER_X_5, OnStcBtm2PickerXClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_BTM2_PICKER_Z_0, IDC_STC_BTM2_PICKER_Z_3, OnStcBtm2PickerZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_BTM2_PICKER_P1_0, IDC_STC_BTM2_PICKER_P1_1, OnStcBtm2PickerP1Click)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_BTM2_PICKER_P2_0, IDC_STC_BTM2_PICKER_P2_1, OnStcBtm2PickerP2Click)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_BUFFER_STAGE1_Y_0 , IDC_STC_BUFFER_STAGE1_Y_5 , OnStcBufferStage1YClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_BUFFER_STAGE2_Y_0 , IDC_STC_BUFFER_STAGE2_Y_5 , OnStcBufferStage2YClick)
END_MESSAGE_MAP()

// CSetupMoveTab2Dlg 메시지 처리기입니다.

BOOL CSetupMoveTab2Dlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 5, 65, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CSetupMoveTab2Dlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupMoveTab2Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Display_MoveData();

	g_objLogFile.Save_HandlerLog("[Setup Move] Show Window - Tab2");
}

void CSetupMoveTab2Dlg::OnStcInspectStage1XClick(UINT nID)
{
	int ID = nID - IDC_STC_INSPECT_STAGE1_X_0;

	CString strOld, strNew;
	m_stcInspectStage1X[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_INSPECT_STAGE1_X) != IDOK) return;

	m_stcInspectStage1X[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcInspectStage2XClick(UINT nID)
{
	int ID = nID - IDC_STC_INSPECT_STAGE2_X_0;

	CString strOld, strNew;
	m_stcInspectStage2X[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_INSPECT_STAGE2_X) != IDOK) return;

	m_stcInspectStage2X[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcInspectStage3XClick(UINT nID)
{
	int ID = nID - IDC_STC_INSPECT_STAGE3_X_0;

	CString strOld, strNew;
	m_stcInspectStage3X[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_INSPECT_STAGE3_X) != IDOK) return;

	m_stcInspectStage3X[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcModuleAlignYClick(UINT nID)			
{
	int ID = nID - IDC_STC_MODULE_ALIGN_Y_0;

	CString strOld, strNew;
	m_stcModuleAlignY[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_MODULE_ALIGN_Y) != IDOK) return;

	m_stcModuleAlignY[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcModuleAlignZClick(UINT nID)			
{
	int ID = nID - IDC_STC_MODULE_ALIGN_Z_0;

	CString strOld, strNew;
	m_stcModuleAlignZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_MODULE_ALIGN_Z) != IDOK) return;

	m_stcModuleAlignZ[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcVisionTop1ZClick(UINT nID)
{
	int ID = nID - IDC_STC_VISION_TOP1_Z_0;

	CString strOld, strNew;
	m_stcVisionTop1Z[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_VISION_TOP1_Z) != IDOK) return;

	m_stcVisionTop1Z[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcTop1MirrorZClick(UINT nID)
{
	int ID = nID - IDC_STC_TOP1_MIRROR_Z_0;

	CString strOld, strNew;
	m_stcTop1MirrorZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_TOP1_MIRROR_Z) != IDOK) return;

	m_stcTop1MirrorZ[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcTop2ShiftYClick(UINT nID)
{
	int ID = nID - IDC_STC_TOP2_SHIFT_Y_0;

	CString strOld, strNew;
	m_stcTop2ShiftY[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_TOP2_SHIFT_Y) != IDOK) return;

	m_stcTop2ShiftY[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcVisionTop2ZClick(UINT nID)
{
	int ID = nID - IDC_STC_VISION_TOP2_Z_0;

	CString strOld, strNew;
	m_stcVisionTop2Z[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_VISION_TOP2_Z) != IDOK) return;

	m_stcVisionTop2Z[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcBtm2PickerXClick(UINT nID)			
{
	int ID = nID - IDC_STC_BTM2_PICKER_X_0;

	CString strOld, strNew;
	m_stcBtm2PickerX[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_BTM2_PICKER_X) != IDOK) return;

	m_stcBtm2PickerX[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcBtm2PickerZClick(UINT nID)			
{
	int ID = nID - IDC_STC_BTM2_PICKER_Z_0;

	CString strOld, strNew;
	m_stcBtm2PickerZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_BTM2_PICKER_Z) != IDOK) return;

	m_stcBtm2PickerZ[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcBtm2PickerP1Click(UINT nID)			
{
	int ID = nID - IDC_STC_BTM2_PICKER_P1_0;

	CString strOld, strNew;
	m_stcBtm2PickerP1[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_BTM2_PICKER_P1) != IDOK) return;

	m_stcBtm2PickerP1[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcBtm2PickerP2Click(UINT nID)			
{
	int ID = nID - IDC_STC_BTM2_PICKER_P2_0;

	CString strOld, strNew;
	m_stcBtm2PickerP2[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_BTM2_PICKER_P2) != IDOK) return;

	m_stcBtm2PickerP2[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcBufferStage1YClick(UINT nID)			
{
	int ID = nID - IDC_STC_BUFFER_STAGE1_Y_0;

	CString strOld, strNew;
	m_stcBufferStage1Y[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_BUFFER_STAGE1_Y) != IDOK) return;

	if (ID == 5) {
		double dNewPos = atof(strNew);
		if (dNewPos < 580.0) {
			strNew = "580";
		}
	}

	m_stcBufferStage1Y[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcBufferStage2YClick(UINT nID)			
{
	int ID = nID - IDC_STC_BUFFER_STAGE2_Y_0;

	CString strOld, strNew;
	m_stcBufferStage2Y[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_BUFFER_STAGE2_Y) != IDOK) return;

	if (ID == 5) {
		double dNewPos = atof(strNew);
		if (dNewPos < 580.0) {
			strNew = "580";
		}
	}

	m_stcBufferStage2Y[ID].SetWindowText(strNew);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CSetupMoveTab2Dlg::Initial_Controls() 
{
	for (int i = 0; i < 15; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 55; i++) m_Label[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	for (int i = 0; i < 6; i++) m_stcInspectStage1X[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 6; i++) m_stcInspectStage2X[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 6; i++) m_stcInspectStage3X[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 3; i++) m_stcModuleAlignY[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 2; i++) m_stcModuleAlignZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 1; i++) m_stcVisionTop1Z[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 2; i++) m_stcTop1MirrorZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 2; i++) m_stcTop2ShiftY[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 1; i++) m_stcVisionTop2Z[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 6; i++) m_stcBtm2PickerX[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 4; i++) m_stcBtm2PickerZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 2; i++) m_stcBtm2PickerP1[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 2; i++) m_stcBtm2PickerP2[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 6; i++) m_stcBufferStage1Y[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 6; i++) m_stcBufferStage2Y[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
}

void CSetupMoveTab2Dlg::Display_MoveData()
{
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	CString strData;

	for (int i = 0; i < 6; i++) { strData.Format("%0.3lf", pMoveData->dInspectStage1X[i]);	m_stcInspectStage1X[i].SetWindowText(strData); }
	for (int i = 0; i < 6; i++) { strData.Format("%0.3lf", pMoveData->dInspectStage2X[i]);	m_stcInspectStage2X[i].SetWindowText(strData); }
	for (int i = 0; i < 6; i++) { strData.Format("%0.3lf", pMoveData->dInspectStage3X[i]);	m_stcInspectStage3X[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dModuleAlignY[i]);	m_stcModuleAlignY[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dModuleAlignZ[i]);	m_stcModuleAlignZ[i].SetWindowText(strData); }
	for (int i = 0; i < 1; i++) { strData.Format("%0.3lf", pMoveData->dVisionTop1Z[i]);		m_stcVisionTop1Z[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dTop1MirrorZ[i]);		m_stcTop1MirrorZ[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dTop2ShiftY[i]);		m_stcTop2ShiftY[i].SetWindowText(strData); }
	for (int i = 0; i < 1; i++) { strData.Format("%0.3lf", pMoveData->dVisionTop2Z[i]);		m_stcVisionTop2Z[i].SetWindowText(strData); }
	for (int i = 0; i < 6; i++) { strData.Format("%0.3lf", pMoveData->dBtm2PickerX[i]);		m_stcBtm2PickerX[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%0.3lf", pMoveData->dBtm2PickerZ[i]);		m_stcBtm2PickerZ[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dBtm2PickerP1[i]);	m_stcBtm2PickerP1[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dBtm2PickerP2[i]);	m_stcBtm2PickerP2[i].SetWindowText(strData); }
	for (int i = 0; i < 6; i++) { strData.Format("%0.3lf", pMoveData->dBufferStage1Y[i]);	m_stcBufferStage1Y[i].SetWindowText(strData); }
	for (int i = 0; i < 6; i++) { strData.Format("%0.3lf", pMoveData->dBufferStage2Y[i]);	m_stcBufferStage2Y[i].SetWindowText(strData); }
}

void CSetupMoveTab2Dlg::Save_MoveData()
{
	CIniFileCS INI(gData.sEnvPath + "\\MoveData.ini");
	if (!INI.Check_File()) { AfxMessageBox("MoveData.ini File Not Found!!!"); return; }

	g_objCommon.Backup_File(gData.sEnvPath, "MoveData");

	CString strKey, strData;
	double dData;

	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcInspectStage1X[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("17_INSPECT_STAGE1_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcInspectStage2X[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("18_INSPECT_STAGE2_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcInspectStage3X[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("19_INSPECT_STAGE3_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcModuleAlignY[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("20_MODULE_ALIGN_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcModuleAlignZ[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("21_MODULE_ALIGN_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 1; i++) { strKey.Format("%02d", i);	m_stcVisionTop1Z[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("22_VISION_TOP1_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcTop1MirrorZ[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("23_TOP1_MIRROR_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcTop2ShiftY[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("24_TOP2_SHIFT_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 1; i++) { strKey.Format("%02d", i);	m_stcVisionTop2Z[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("25_VISION_TOP2_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcBtm2PickerX[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("26_BTM2_PICKER_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcBtm2PickerP1[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("28_BTM2_PICKER_P1", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcBtm2PickerP2[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("29_BTM2_PICKER_P2", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcBufferStage1Y[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("30_BUFFER_STAGE1_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcBufferStage2Y[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("31_BUFFER_STAGE2_Y", strKey, dData, "%0.3lf"); }

	for (int i = 0; i < 4; i++) {
		strKey.Format("%02d", i);	m_stcBtm2PickerZ[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("27_BTM2_PICKER_Z", strKey, dData, "%0.3lf");

		// RMS 항목 저장
		if (i == 1) g_objMES.Save_AviRmsData("BTM2 Picker Z Inspect Down Position", strData);
		if (i == 3) g_objMES.Save_AviRmsData("BTM2 Picker Z Buffer Down Position", strData);
	}

	g_objLogFile.Save_HandlerLog("[Setup - MoveTab Dialog 2] save");

	Cancel_MoveData();
}

void CSetupMoveTab2Dlg::Save_MoveDataForAll()
{
	CIniFileCS INI(gData.sEnvPath + "\\AllParam.ini");
	if (!INI.Check_File()) { AfxMessageBox("AllParam.ini File Not Found!!!"); return; }

	CString strKey, strData;
	double dData;

	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcInspectStage1X[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("17_INSPECT_STAGE1_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcInspectStage2X[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("18_INSPECT_STAGE2_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcInspectStage3X[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("19_INSPECT_STAGE3_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcModuleAlignY[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("20_MODULE_ALIGN_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcModuleAlignZ[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("21_MODULE_ALIGN_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 1; i++) { strKey.Format("%02d", i);	m_stcVisionTop1Z[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("22_VISION_TOP1_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcTop1MirrorZ[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("23_TOP1_MIRROR_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcTop2ShiftY[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("24_TOP2_SHIFT_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 1; i++) { strKey.Format("%02d", i);	m_stcVisionTop2Z[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("25_VISION_TOP2_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcBtm2PickerX[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("26_BTM2_PICKER_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcBtm2PickerP1[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("28_BTM2_PICKER_P1", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcBtm2PickerP2[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("29_BTM2_PICKER_P2", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcBufferStage1Y[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("30_BUFFER_STAGE1_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcBufferStage2Y[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("31_BUFFER_STAGE2_Y", strKey, dData, "%0.3lf"); }

	
}

void CSetupMoveTab2Dlg::Cancel_MoveData()
{
	g_objDataManager.Read_MoveData();
	Display_MoveData();
}

///////////////////////////////////////////////////////////////////////////////
