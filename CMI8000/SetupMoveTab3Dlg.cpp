// SetupMoveTab3Dlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "SetupMoveTab3Dlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "DataManager.h"
#include "Common.h"
#include "MESInterface.h"

#include "AJinDefine.h"

// CSetupMoveTab3Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupMoveTab3Dlg, CDialogEx)

CSetupMoveTab3Dlg::CSetupMoveTab3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMoveTab3Dlg::IDD, pParent)
{
}

CSetupMoveTab3Dlg::~CSetupMoveTab3Dlg()
{
}

void CSetupMoveTab3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 14; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 63; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_STC_SORT_PICKER1_X_0 + i, m_stcSortPicker1X[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_SORT_PICKER1_Z_0 + i, m_stcSortPicker1Z[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_SORT_PICKER1_P_0 + i, m_stcSortPicker1P[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_STC_SORT_PICKER2_X_0 + i, m_stcSortPicker2X[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_SORT_PICKER2_Z_0 + i, m_stcSortPicker2Z[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_SORT_PICKER2_P_0 + i, m_stcSortPicker2P[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_GOOD_STAGE1_Y_0 + i, m_stcGoodStage1Y[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_STC_GOOD_STAGE1_Z_0 + i, m_stcGoodStage1Z[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_GOOD_STAGE2_Y_0 + i, m_stcGoodStage2Y[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_STC_GOOD_STAGE2_Z_0 + i, m_stcGoodStage2Z[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_STC_NG_STAGE_Y_0 + i, m_stcNgStageY[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_EMPTY_TRANS1_X_0 + i, m_stcEmptyTrans1X[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_EMPTY_PORT_Z_0 + i, m_stcEmptyPortZ[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_EMPTY_TRANS2_Y_0 + i, m_stcEmptyTrans2Y[i]);
}

BEGIN_MESSAGE_MAP(CSetupMoveTab3Dlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_SORT_PICKER1_X_0, IDC_STC_SORT_PICKER1_X_6, OnStcSortPicker1XClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_SORT_PICKER1_Z_0, IDC_STC_SORT_PICKER1_Z_5, OnStcSortPicker1ZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_SORT_PICKER1_P_0, IDC_STC_SORT_PICKER1_P_1, OnStcSortPicker1PClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_SORT_PICKER2_X_0, IDC_STC_SORT_PICKER2_X_6, OnStcSortPicker2XClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_SORT_PICKER2_Z_0, IDC_STC_SORT_PICKER2_Z_5, OnStcSortPicker2ZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_SORT_PICKER2_P_0, IDC_STC_SORT_PICKER2_P_1, OnStcSortPicker2PClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_GOOD_STAGE1_Y_0, IDC_STC_GOOD_STAGE1_Y_3, OnStcGoodStage1YClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_GOOD_STAGE1_Z_0, IDC_STC_GOOD_STAGE1_Z_4, OnStcGoodStage1ZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_GOOD_STAGE2_Y_0, IDC_STC_GOOD_STAGE2_Y_3, OnStcGoodStage2YClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_GOOD_STAGE2_Z_0, IDC_STC_GOOD_STAGE2_Z_4, OnStcGoodStage2ZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_NG_STAGE_Y_0, IDC_STC_NG_STAGE_Y_4, OnStcNgStageYClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_EMPTY_TRANS1_X_0, IDC_STC_EMPTY_TRANS1_X_3, OnStcEmptyTrans1XClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_EMPTY_PORT_Z_0, IDC_STC_EMPTY_PORT_Z_2, OnStcEmptyPortZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_EMPTY_TRANS2_Y_0, IDC_STC_EMPTY_TRANS2_Y_2, OnStcEmptyTrans2YClick)
END_MESSAGE_MAP()

// CSetupMoveTab3Dlg 메시지 처리기입니다.

// CSetupMoveTab3Dlg 메시지 처리기입니다.

BOOL CSetupMoveTab3Dlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 5, 65, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CSetupMoveTab3Dlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupMoveTab3Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Display_MoveData();

	g_objLogFile.Save_HandlerLog("[Setup Move] Show Window - Tab3");
}

void CSetupMoveTab3Dlg::OnStcSortPicker1XClick(UINT nID)
{
	int ID = nID - IDC_STC_SORT_PICKER1_X_0;

	CString strOld, strNew;
	m_stcSortPicker1X[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_SORT_PICKER1_X) != IDOK) return;

	m_stcSortPicker1X[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcSortPicker1ZClick(UINT nID)
{
	int ID = nID - IDC_STC_SORT_PICKER1_Z_0;

	CString strOld, strNew;
	m_stcSortPicker1Z[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_SORT_PICKER1_Z) != IDOK) return;

	m_stcSortPicker1Z[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcSortPicker1PClick(UINT nID)
{
	int ID = nID - IDC_STC_SORT_PICKER1_P_0;

	CString strOld, strNew;
	m_stcSortPicker1P[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_SORT_PICKER1_P) != IDOK) return;

	m_stcSortPicker1P[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcSortPicker2XClick(UINT nID)
{
	int ID = nID - IDC_STC_SORT_PICKER2_X_0;

	CString strOld, strNew;
	m_stcSortPicker2X[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_SORT_PICKER2_X) != IDOK) return;

	m_stcSortPicker2X[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcSortPicker2ZClick(UINT nID)
{
	int ID = nID - IDC_STC_SORT_PICKER2_Z_0;

	CString strOld, strNew;
	m_stcSortPicker2Z[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_SORT_PICKER2_Z) != IDOK) return;

	m_stcSortPicker2Z[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcSortPicker2PClick(UINT nID)
{
	int ID = nID - IDC_STC_SORT_PICKER2_P_0;

	CString strOld, strNew;
	m_stcSortPicker2P[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_SORT_PICKER2_P) != IDOK) return;

	m_stcSortPicker2P[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcGoodStage1YClick(UINT nID)
{
	int ID = nID - IDC_STC_GOOD_STAGE1_Y_0;

	CString strOld, strNew;
	m_stcGoodStage1Y[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_GOOD_STAGE1_Y) != IDOK) return;

	m_stcGoodStage1Y[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcGoodStage1ZClick(UINT nID)			
{
	int ID = nID - IDC_STC_GOOD_STAGE1_Z_0;

	CString strOld, strNew;
	m_stcGoodStage1Z[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_GOOD_STAGE1_Z) != IDOK) return;

	m_stcGoodStage1Z[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcGoodStage2YClick(UINT nID)			
{
	int ID = nID - IDC_STC_GOOD_STAGE2_Y_0;

	CString strOld, strNew;
	m_stcGoodStage2Y[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_GOOD_STAGE2_Y) != IDOK) return;

	m_stcGoodStage2Y[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcGoodStage2ZClick(UINT nID)			
{
	int ID = nID - IDC_STC_GOOD_STAGE2_Z_0;

	CString strOld, strNew;
	m_stcGoodStage2Z[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_GOOD_STAGE2_Z) != IDOK) return;

	m_stcGoodStage2Z[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcNgStageYClick(UINT nID)			
{
	int ID = nID - IDC_STC_NG_STAGE_Y_0;

	CString strOld, strNew;
	m_stcNgStageY[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_NG_STAGE_Y) != IDOK) return;

	m_stcNgStageY[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcEmptyTrans1XClick(UINT nID)			
{
	int ID = nID - IDC_STC_EMPTY_TRANS1_X_0;

	CString strOld, strNew;
	m_stcEmptyTrans1X[ID].GetWindowText(strOld);
 	if (g_objCommon.Show_NumPad(strOld, strNew, AX_EMPTY_TRANS1_X) != IDOK) return;

	m_stcEmptyTrans1X[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcEmptyPortZClick(UINT nID)			
{
	int ID = nID - IDC_STC_EMPTY_PORT_Z_0;

	CString strOld, strNew;
	m_stcEmptyPortZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_EMPTY_PORT_Z) != IDOK) return;

	m_stcEmptyPortZ[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcEmptyTrans2YClick(UINT nID)			
{
	int ID = nID - IDC_STC_EMPTY_TRANS2_Y_0;

	CString strOld, strNew;
	m_stcEmptyTrans2Y[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_EMPTY_TRANS2_Y) != IDOK) return;

	m_stcEmptyTrans2Y[ID].SetWindowText(strNew);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CSetupMoveTab3Dlg::Initial_Controls() 
{
	for (int i = 0; i < 14; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 63; i++) m_Label[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	for (int i = 0; i < 7; i++) m_stcSortPicker1X[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 6; i++) m_stcSortPicker1Z[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 2; i++) m_stcSortPicker1P[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 7; i++) m_stcSortPicker2X[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 6; i++) m_stcSortPicker2Z[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 2; i++) m_stcSortPicker2P[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 4; i++) m_stcGoodStage1Y[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 5; i++) m_stcGoodStage1Z[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 4; i++) m_stcGoodStage2Y[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 5; i++) m_stcGoodStage2Z[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 5; i++) m_stcNgStageY[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 4; i++) m_stcEmptyTrans1X[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 3; i++) m_stcEmptyPortZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 3; i++) m_stcEmptyTrans2Y[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
}

void CSetupMoveTab3Dlg::Display_MoveData()
{
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	CString strData;

	for (int i = 0; i < 7; i++) { strData.Format("%0.3lf", pMoveData->dSortPicker1X[i]);	m_stcSortPicker1X[i].SetWindowText(strData); }
	for (int i = 0; i < 6; i++) { strData.Format("%0.3lf", pMoveData->dSortPicker1Z[i]);	m_stcSortPicker1Z[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dSortPicker1P[i]);	m_stcSortPicker1P[i].SetWindowText(strData); }
	for (int i = 0; i < 7; i++) { strData.Format("%0.3lf", pMoveData->dSortPicker2X[i]);	m_stcSortPicker2X[i].SetWindowText(strData); }
	for (int i = 0; i < 6; i++) { strData.Format("%0.3lf", pMoveData->dSortPicker2Z[i]);	m_stcSortPicker2Z[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dSortPicker2P[i]);	m_stcSortPicker2P[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%0.3lf", pMoveData->dGoodStage1Y[i]);		m_stcGoodStage1Y[i].SetWindowText(strData); }
	for (int i = 0; i < 5; i++) { strData.Format("%0.3lf", pMoveData->dGoodStage1Z[i]);		m_stcGoodStage1Z[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%0.3lf", pMoveData->dGoodStage2Y[i]);		m_stcGoodStage2Y[i].SetWindowText(strData); }
	for (int i = 0; i < 5; i++) { strData.Format("%0.3lf", pMoveData->dGoodStage2Z[i]);		m_stcGoodStage2Z[i].SetWindowText(strData); }
	for (int i = 0; i < 5; i++) { strData.Format("%0.3lf", pMoveData->dNgStageY[i]);		m_stcNgStageY[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%0.3lf", pMoveData->dEmptyTrans1X[i]);	m_stcEmptyTrans1X[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dEmptyPortZ[i]);		m_stcEmptyPortZ[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dEmptyTrans2Y[i]);	m_stcEmptyTrans2Y[i].SetWindowText(strData); }
}

void CSetupMoveTab3Dlg::Save_MoveData()
{
	CIniFileCS INI(gData.sEnvPath + "\\MoveData.ini");
	if (!INI.Check_File()) { AfxMessageBox("MoveData.ini File Not Found!!!"); return; }

	g_objCommon.Backup_File(gData.sEnvPath, "MoveData");

	CString strKey, strData;
	double dData;

	for (int i = 0; i < 7; i++) { strKey.Format("%02d", i);	m_stcSortPicker1X[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("32_SORT_PICKER1_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcSortPicker1P[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("34_SORT_PICKER1_P", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 7; i++) { strKey.Format("%02d", i);	m_stcSortPicker2X[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("35_SORT_PICKER2_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcSortPicker2P[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("37_SORT_PICKER2_P", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i);	m_stcGoodStage1Y[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("38_GOOD_STAGE1_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i);	m_stcGoodStage1Z[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("39_GOOD_STAGE1_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i);	m_stcGoodStage2Y[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("40_GOOD_STAGE2_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i);	m_stcGoodStage2Z[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("41_GOOD_STAGE2_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i);	m_stcNgStageY[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("42_NG_STAGE_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i);	m_stcEmptyTrans1X[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("43_EMPTY_TRANS1_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcEmptyPortZ[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("44_EMPTY_PORT_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcEmptyTrans2Y[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("45_EMPTY_TRANS2_Y", strKey, dData, "%0.3lf"); }

	for (int i = 0; i < 6; i++) {
		strKey.Format("%02d", i);	m_stcSortPicker1Z[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("33_SORT_PICKER1_Z", strKey, dData, "%0.3lf");

		// RMS 항목 저장
		if (i == 1) g_objMES.Save_AviRmsData("Sort Picker1 Z Buffer1 Down Position", strData);
		if (i == 2) g_objMES.Save_AviRmsData("Sort Picker1 Z Good Down Position", strData);
		if (i == 3) g_objMES.Save_AviRmsData("Sort Picker1 Z NG Down Position", strData);
		if (i == 4) g_objMES.Save_AviRmsData("Sort Picker1 Z NG Buffer Down Position", strData);
		if (i == 5) g_objMES.Save_AviRmsData("Sort Picker1 Z Buffer2 Down Position", strData);
	}
	for (int i = 0; i < 6; i++) {
		strKey.Format("%02d", i);	m_stcSortPicker2Z[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("36_SORT_PICKER2_Z", strKey, dData, "%0.3lf");

		// RMS 항목 저장
		if (i == 1) g_objMES.Save_AviRmsData("Sort Picker2 Z Buffer1 Down Position", strData);
		if (i == 2) g_objMES.Save_AviRmsData("Sort Picker2 Z Good Down Position", strData);
		if (i == 3) g_objMES.Save_AviRmsData("Sort Picker2 Z NG Down Position", strData);
		if (i == 4) g_objMES.Save_AviRmsData("Sort Picker2 Z NG Buffer Down Position", strData);
		if (i == 5) g_objMES.Save_AviRmsData("Sort Picker2 Z Buffer2 Down Position", strData);
	}

	g_objLogFile.Save_HandlerLog("[Setup - MoveTab Dialog 3] save");

	Cancel_MoveData();
}

void CSetupMoveTab3Dlg::Save_MoveDataForAll()
{
	CIniFileCS INI(gData.sEnvPath + "\\AllParam.ini");
	if (!INI.Check_File()) { AfxMessageBox("AllParam.ini File Not Found!!!"); return; }
	
	CString strKey, strData;
	double dData;

	for (int i = 0; i < 7; i++) { strKey.Format("%02d", i);	m_stcSortPicker1X[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("32_SORT_PICKER1_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcSortPicker1P[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("34_SORT_PICKER1_P", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 7; i++) { strKey.Format("%02d", i);	m_stcSortPicker2X[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("35_SORT_PICKER2_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcSortPicker2P[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("37_SORT_PICKER2_P", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i);	m_stcGoodStage1Y[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("38_GOOD_STAGE1_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i);	m_stcGoodStage1Z[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("39_GOOD_STAGE1_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i);	m_stcGoodStage2Y[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("40_GOOD_STAGE2_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i);	m_stcGoodStage2Z[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("41_GOOD_STAGE2_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i);	m_stcNgStageY[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("42_NG_STAGE_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i);	m_stcEmptyTrans1X[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("43_EMPTY_TRANS1_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcEmptyPortZ[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("44_EMPTY_PORT_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcEmptyTrans2Y[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("45_EMPTY_TRANS2_Y", strKey, dData, "%0.3lf"); }

}
void CSetupMoveTab3Dlg::Cancel_MoveData()
{
	g_objDataManager.Read_MoveData();
	Display_MoveData();
}

///////////////////////////////////////////////////////////////////////////////
