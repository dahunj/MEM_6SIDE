// SetupEquipDlg.cpp : ±∏«ˆ ∆ƒ¿œ¿‘¥œ¥Ÿ.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "SetupEquipDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "DataManager.h"
#include "Common.h"

#include "SetupDlg.h"
#include "Inspector.h"
#include "SequenceMain.h"
#include "SequenceInit.h"
#include "WorkDlg.h"

#include "Dispatcher.h"
#include "CapAttach.h"

#include "CMI8000Dlg.h"

// CSetupEquipDlg ¥Î»≠ ªÛ¿⁄¿‘¥œ¥Ÿ.

IMPLEMENT_DYNAMIC(CSetupEquipDlg, CDialogEx)

CSetupEquipDlg::CSetupEquipDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupEquipDlg::IDD, pParent)
{
}

CSetupEquipDlg::~CSetupEquipDlg()
{
}

void CSetupEquipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 39; i++) DDX_Control(pDX, IDC_LABEL_0 + i,  m_Label[i]);
	DDX_Control(pDX, IDC_STC_EQUIP_NAME, m_stcEquipName);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_RDO_MODEL_0 + i, m_rdoModel[i]);
	DDX_Control(pDX, IDC_CBO_LOT_BARCODE_PORT, m_cboLotBarcodePort);
	DDX_Control(pDX, IDC_STC_SCREEN_OFF, m_stcScreenOff);
	DDX_Control(pDX, IDC_STC_NOWORK_TIME, m_stcNoWorkTime);
	DDX_Control(pDX, IDC_STC_MOTION_CHECK, m_stcMotionCheck);
	DDX_Control(pDX, IDC_STC_LOG_DATE, m_stcLogDate);
	DDX_Control(pDX, IDC_CHK_MANUAL_TAKT_TEST, m_chkManualTaktTest);
	DDX_Control(pDX, IDC_LBL_DOOR_LOCK, m_lblDoorLock);
	DDX_Control(pDX, IDC_LBL_DOOR_LOCK2, m_lblDoorLockTime);
	DDX_Control(pDX, IDC_STC_DOORLOCK_TIME, m_stcDoorLockTime);
	
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_RDO_DOOR_LOCK_0 + i, m_rdoDoorLock[i]);
	DDX_Control(pDX, IDC_CHK_USE_VISION_ALIGN, m_chkUseVisionAlign);
	DDX_Control(pDX, IDC_CHK_USE_INSPECT_ANGLE, m_chkUseInspectAngle);
	DDX_Control(pDX, IDC_CHK_USE_INSPECT_BTM_1_SP, m_chkUseInspectBtm1_SP);
	DDX_Control(pDX, IDC_CHK_USE_INSPECT_BTM_1_AG, m_chkUseInspectBtm1_AG);
	DDX_Control(pDX, IDC_CHK_USE_INSPECT_BTM_1_3D, m_chkUseInspectBtm1_3D);
	DDX_Control(pDX, IDC_CHK_USE_INSPECT_TOP_1, m_chkUseInspectTop1);
	DDX_Control(pDX, IDC_CHK_USE_INSPECT_TOP_2, m_chkUseInspectTop2);
	DDX_Control(pDX, IDC_CHK_USE_INSPECT_BTM_2, m_chkUseInspectBtm2);
	DDX_Control(pDX, IDC_CHK_USE_DISPATCHER, m_chkUseDispatcher);
	DDX_Control(pDX, IDC_CHK_USE_INLINE, m_chkUseInline);
	DDX_Control(pDX, IDC_CHK_USE_MES_APD, m_chkUseMesApd);
	DDX_Control(pDX, IDC_CHK_USE_BTM2, m_chkUseBtm2);
	DDX_Control(pDX, IDC_CHK_USE_PM_TRIGGER, m_chkUsePMTrigger);
	DDX_Control(pDX, IDC_CHK_USE_SECOND_AVI, m_chkUse2ndAVI);
	DDX_Control(pDX, IDC_CHK_USE_DRY_RUN, m_chkUseDryRun);

	DDX_Control(pDX, IDC_CHK_USE_ROS_SKIP, m_chkUseRosSkip);
	DDX_Control(pDX, IDC_CHK_USE_INSP_SKIP, m_chkUseInspectSkip);
	
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_TRAY_DATA_0 + i, m_stcTrayData[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_STC_INSPECT_DATA_0 + i, m_stcInspectData[i]);
	for (int i = 0; i < 1; i++) DDX_Control(pDX, IDC_STC_NG_BUFFER_DATA_0 + i, m_stcNgBufferData[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_VAC_OFF_DELAY_0 + i, m_stcVacOffDelay[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_VAC_OFF_REPEAT_0 + i, m_stcVacOffRepeat[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_STC_DELAY_ADD_0 + i, m_stcDelayAdd[i]);
	DDX_Control(pDX, IDC_GRP_ROS, m_grpRos);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_LBL_ROS_0 + i, m_lblRos[i]);
	DDX_Control(pDX, IDC_IPA_DISPATCHER_IP, m_ipaDispatcherIp);
	DDX_Control(pDX, IDC_IPA_HANDLER_IP, m_ipaHandlerIp);
	DDX_Control(pDX, IDC_BTN_ROS_CONNECT, m_btnRosConnect);
	DDX_Control(pDX, IDC_BTN_ROS_DISCONNECT, m_btnRosDisconnect);
	DDX_Control(pDX, IDC_STC_JUDGE_TIME_OVER, m_stcJudgeTimeOver);
	DDX_Control(pDX, IDC_CHK_JUDGE_SPECIA_NG, m_chkJudgeSpecialNg);
	DDX_Control(pDX, IDC_GRP_CAP, m_grpCap);
	DDX_Control(pDX, IDC_LBL_CAP_0, m_lblCap);
	DDX_Control(pDX, IDC_IPA_CAP_IP, m_ipaCapAttachIp);
	DDX_Control(pDX, IDC_BTN_CAP_CONNECT, m_btnCapConnect);
	DDX_Control(pDX, IDC_BTN_CAP_DISCONNECT, m_btnCapDisconnect);

	DDX_Control(pDX, IDC_GRP_TOWER, m_grpTower);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LBL_TOWER_0 + i,  m_lblTower[i]);
	for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) DDX_Control(pDX, IDC_CHK_TOWER_0_0 + i * 4 + j,  m_chkTower[i][j]);
	DDX_Control(pDX, IDC_GRP_BUZZER, m_grpBuzzer);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_LBL_BUZZER_0 + i,  m_lblBuzzer[i]);
	for (int i = 0; i < 2; i++) for (int j = 0; j < 6; j++) DDX_Control(pDX, IDC_CHK_BUZZER_0_0 + i * 6 + j,  m_chkBuzzer[i][j]);
	DDX_Control(pDX, IDC_GRP_HIDDEN, m_grpHidden);
	DDX_Control(pDX, IDC_LBL_PASSWORD_MT, m_lblPasswordMt);
	DDX_Control(pDX, IDC_STC_PASSWORD_MT, m_stcPasswordMt);
	DDX_Control(pDX, IDC_LBL_PASSWORD_SI, m_lblPasswordSi);
	DDX_Control(pDX, IDC_EDT_PASSWORD_SI, m_edtPasswordSi);
	DDX_Control(pDX, IDC_GRP_RESULT_TEST, m_grpResultTest);
	DDX_Control(pDX, IDC_CHK_RESULT_TEST_USE, m_chkResultTestUse);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_LBL_RESULT_TEST_0 + i, m_lblResultTest[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_EDT_RESULT_TEST_0 + i, m_edtResultTest[i]);
	DDX_Control(pDX, IDC_STC_VIS_PROG_VER, m_stcVisProgVer);
	DDX_Control(pDX, IDC_STC_VIS_PARA_VER, m_stcVisParaVer);
	DDX_Control(pDX, IDC_CHK_USE_FOCSU_LOG, m_chkUseFocusLog);
	DDX_Control(pDX, IDC_STC_TMR_FOCUS_LOG, m_stcTmrFocusLog);
	DDX_Control(pDX, IDC_STC_JIG_NG_RATE, m_stcJigNgRate);
	DDX_Control(pDX, IDC_STC_JIG_NG_CNT, m_stcJigNgCnt);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_TRIGGER_DATA_0 + i, m_stcTriggerData[i]);
}

BEGIN_MESSAGE_MAP(CSetupEquipDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_STN_CLICKED(IDC_STC_EQUIP_NAME, &CSetupEquipDlg::OnStnClickedStcEquipName)
	ON_STN_CLICKED(IDC_STC_SCREEN_OFF, &CSetupEquipDlg::OnStnClickedStcScreenOff)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_TRAY_DATA_0, IDC_STC_TRAY_DATA_3, OnStcTrayDataClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_INSPECT_DATA_0, IDC_STC_INSPECT_DATA_7, OnStcInspectDataClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_NG_BUFFER_DATA_0, IDC_STC_NG_BUFFER_DATA_0, OnStcNgBufferDataClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_VAC_OFF_DELAY_0, IDC_STC_VAC_OFF_DELAY_3, OnStcVacOffDelayClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_VAC_OFF_REPEAT_0, IDC_STC_VAC_OFF_REPEAT_3, OnStcVacOffRepeatClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_DELAY_ADD_0, IDC_STC_DELAY_ADD_4, OnStcDelayAddClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_TRIGGER_DATA_0, IDC_STC_TRIGGER_DATA_3, OnStcTriggerDataClick)
	ON_BN_CLICKED(IDC_BTN_ROS_CONNECT, &CSetupEquipDlg::OnBnClickedBtnRosConnect)
	ON_BN_CLICKED(IDC_BTN_ROS_DISCONNECT, &CSetupEquipDlg::OnBnClickedBtnRosDisconnect)
	ON_BN_CLICKED(IDC_BTN_CAP_CONNECT, &CSetupEquipDlg::OnBnClickedBtnCapConnect)
	ON_BN_CLICKED(IDC_BTN_CAP_DISCONNECT, &CSetupEquipDlg::OnBnClickedBtnCapDisconnect)
	ON_STN_CLICKED(IDC_STC_JUDGE_TIME_OVER, &CSetupEquipDlg::OnStnClickedStcJudgeTimeOver)
	ON_STN_CLICKED(IDC_STC_SHOW_HIDDEN, &CSetupEquipDlg::OnStnClickedStcShowHidden)
	ON_STN_CLICKED(IDC_STC_PASSWORD_MT, &CSetupEquipDlg::OnStnClickedStcPasswordMt)
	ON_STN_CLICKED(IDC_STC_NOWORK_TIME, &CSetupEquipDlg::OnStnClickedStcNoworkTime)
	ON_STN_CLICKED(IDC_STC_LOG_DATE, &CSetupEquipDlg::OnStnClickedStcLogDate)
	ON_STN_CLICKED(IDC_STC_MOTION_CHECK, &CSetupEquipDlg::OnStnClickedStcMotionCheck)
	ON_STN_CLICKED(IDC_STC_VIS_PROG_VER, &CSetupEquipDlg::OnStnClickedStcVisProgVer)
	ON_STN_CLICKED(IDC_STC_VIS_PARA_VER, &CSetupEquipDlg::OnStnClickedStcVisParaVer)
	ON_STN_CLICKED(IDC_STC_TMR_FOCUS_LOG, &CSetupEquipDlg::OnStnClickedStcTmrFocusLog)
	ON_STN_CLICKED(IDC_STC_JIG_NG_RATE, &CSetupEquipDlg::OnStnClickedStcJigNgRate)
	ON_STN_CLICKED(IDC_STC_JIG_NG_CNT, &CSetupEquipDlg::OnStnClickedStcJigNgCnt)
	
	ON_STN_CLICKED(IDC_STC_DOORLOCK_TIME, &CSetupEquipDlg::OnStnClickedStcDoorlockTime)
	ON_BN_CLICKED(IDC_CHK_USE_VISION_ALIGN, &CSetupEquipDlg::OnBnClickedChkUseVisionAlign)
	ON_BN_CLICKED(IDC_CHK_USE_INSPECT_ANGLE, &CSetupEquipDlg::OnBnClickedChkUseInspectAngle)
	ON_BN_CLICKED(IDC_CHK_USE_INSPECT_BTM_1_SP, &CSetupEquipDlg::OnBnClickedChkUseInspectBtm1Sp)
	ON_BN_CLICKED(IDC_CHK_USE_INSPECT_BTM_1_AG, &CSetupEquipDlg::OnBnClickedChkUseInspectBtm1Ag)
	ON_BN_CLICKED(IDC_CHK_USE_INSPECT_BTM_1_3D, &CSetupEquipDlg::OnBnClickedChkUseInspectBtm13d)
	ON_BN_CLICKED(IDC_CHK_USE_INSPECT_TOP_1, &CSetupEquipDlg::OnBnClickedChkUseInspectTop1)
	ON_BN_CLICKED(IDC_CHK_USE_INSPECT_TOP_2, &CSetupEquipDlg::OnBnClickedChkUseInspectTop2)
	ON_BN_CLICKED(IDC_CHK_USE_INSPECT_BTM_2, &CSetupEquipDlg::OnBnClickedChkUseInspectBtm2)
	ON_BN_CLICKED(IDC_CHK_USE_INLINE, &CSetupEquipDlg::OnBnClickedChkUseInline)
	ON_BN_CLICKED(IDC_CHK_USE_BTM2, &CSetupEquipDlg::OnBnClickedChkUseBtm2)
	ON_BN_CLICKED(IDC_RDO_DOOR_LOCK_0, &CSetupEquipDlg::OnBnClickedRdoDoorLock0)
	ON_BN_CLICKED(IDC_RDO_DOOR_LOCK_1, &CSetupEquipDlg::OnBnClickedRdoDoorLock1)

	ON_BN_CLICKED(IDC_CHK_USE_DRY_RUN, &CSetupEquipDlg::OnBnClickedChkUseDryRun)
END_MESSAGE_MAP()

// CSetupEquipDlg ∏ﬁΩ√¡ˆ √≥∏Æ±‚¿‘¥œ¥Ÿ.

BOOL CSetupEquipDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  ø©±‚ø° √ﬂ∞° √ ±‚»≠ ¿€æ˜¿ª √ﬂ∞°«’¥œ¥Ÿ.
	SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// øπø‹: OCX º”º∫ ∆‰¿Ã¡ˆ¥¬ FALSE∏¶ π›»Ø«ÿæﬂ «’¥œ¥Ÿ.
}

BOOL CSetupEquipDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupEquipDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Display_EquipData();

	EQUIP_DATA *m_pEquipData = g_objDataManager.Get_pEquipData();

	CIniFileCS INI(gData.sEnvPath + "\\EquipData.ini");
	if (!INI.Check_File()) { AfxMessageBox("EquipData.ini File Not Found!!!"); return; }


	int nTempSy = atoi(m_pEquipData->sPasswordSi);

	if(gData.nLogInLevel == 9300)
	{
		m_lblDoorLock.ShowWindow(SW_SHOW);
		m_rdoDoorLock[0].ShowWindow(SW_SHOW);
		m_rdoDoorLock[1].ShowWindow(SW_SHOW);
	}
	else if(gData.nLogInLevel == nTempSy)
	{
		m_lblDoorLock.ShowWindow(SW_SHOW);
		m_rdoDoorLock[0].ShowWindow(SW_SHOW);
		m_rdoDoorLock[1].ShowWindow(SW_SHOW);
	}
	else
	{		
		m_pEquipData->bUseDoorLock = TRUE;
		INI.Set_Bool("EQUIPMENT", "DOOR_LOCK", TRUE);
		m_lblDoorLock.ShowWindow(SW_HIDE);
		m_rdoDoorLock[0].ShowWindow(SW_HIDE);
		m_rdoDoorLock[1].ShowWindow(SW_HIDE);
	}

	m_grpHidden.ShowWindow(SW_HIDE);
	m_lblPasswordMt.ShowWindow(SW_HIDE);
	m_stcPasswordMt.ShowWindow(SW_HIDE);
	m_lblPasswordSi.ShowWindow(SW_HIDE);
	m_edtPasswordSi.ShowWindow(SW_HIDE);
	m_chkUseDryRun.ShowWindow(SW_HIDE);

	if(gData.bUseDryRun)
	{
		m_grpResultTest.ShowWindow(SW_SHOW);
		m_chkResultTestUse.ShowWindow(SW_HIDE);
		for (int i = 0; i < 4; i++) m_lblResultTest[i].ShowWindow(SW_SHOW);
		for (int i = 0; i < 4; i++) m_edtResultTest[i].ShowWindow(SW_SHOW);
	}
	else
	{
		m_grpResultTest.ShowWindow(SW_HIDE);
		m_chkResultTestUse.ShowWindow(SW_HIDE);
		for (int i = 0; i < 4; i++) m_lblResultTest[i].ShowWindow(SW_HIDE);
		for (int i = 0; i < 4; i++) m_edtResultTest[i].ShowWindow(SW_HIDE);
	}
}

void CSetupEquipDlg::OnStnClickedStcEquipName()
{
	CString strKey;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;

	m_stcEquipName.SetWindowText(strKey);
}

void CSetupEquipDlg::OnStnClickedStcScreenOff()
{
	CString strOld, strNew;
	m_stcScreenOff.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcScreenOff.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStnClickedStcNoworkTime()
{
	CString strOld, strNew;
	m_stcNoWorkTime.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcNoWorkTime.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStnClickedStcLogDate()
{
	CString strOld, strNew;
	m_stcLogDate.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcLogDate.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStnClickedStcMotionCheck()
{
	CString strOld, strNew;
	m_stcMotionCheck.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcMotionCheck.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcTrayDataClick(UINT nID)
{
	int ID = nID - IDC_STC_TRAY_DATA_0;

	CString strOld, strNew;
	m_stcTrayData[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcTrayData[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcInspectDataClick(UINT nID)
{
	int ID = nID - IDC_STC_INSPECT_DATA_0;

	CString strOld, strNew;
	m_stcInspectData[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcInspectData[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcNgBufferDataClick(UINT nID)
{
	int ID = nID - IDC_STC_NG_BUFFER_DATA_0;

	CString strOld, strNew;
	m_stcNgBufferData[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcNgBufferData[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcVacOffDelayClick(UINT nID)
{
	int ID = nID - IDC_STC_VAC_OFF_DELAY_0;

	CString strOld, strNew;
	m_stcVacOffDelay[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcVacOffDelay[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcVacOffRepeatClick(UINT nID)
{
	int ID = nID - IDC_STC_VAC_OFF_REPEAT_0;

	CString strOld, strNew;
	m_stcVacOffRepeat[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcVacOffRepeat[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcDelayAddClick(UINT nID)
{
	int ID = nID - IDC_STC_DELAY_ADD_0;

	CString strOld, strNew;
	m_stcDelayAdd[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcDelayAdd[ID].SetWindowText(strNew);
}


void CSetupEquipDlg::OnStcTriggerDataClick(UINT nID)
{
	int ID = nID - IDC_STC_TRIGGER_DATA_0;

	CString strOld, strNew;
	m_stcTriggerData[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcTriggerData[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnBnClickedBtnRosConnect()
{
	g_objLogFile.Save_DispatcherLog("[Ros Connect] Click");

	CString strIp;
	m_ipaDispatcherIp.GetWindowText(strIp);

	g_objDispatcher.Set_DispatcherIp(strIp);
	if (!g_objDispatcher.Initialize()) return;
	Disable_RosCtrl(TRUE);

	DWORD dwStart = GetTickCount();
	while (!g_objDispatcher.Is_Connected()) {
		if (GetTickCount() - dwStart > 1500) {	// 1.5√ 
			g_objLogFile.Save_DispatcherLog("[Ros Connect] Fail");
			Disable_RosCtrl(FALSE); break;
		}
		theApp.DoEvents();
	}
}

void CSetupEquipDlg::OnBnClickedBtnRosDisconnect()
{
	g_objLogFile.Save_DispatcherLog("[Ros Disconnect] Click");

	g_objDispatcher.Set_ConnectEnd();
	Disable_RosCtrl(FALSE);
}

void CSetupEquipDlg::OnStnClickedStcJudgeTimeOver()
{
	CString strOld, strNew;
	m_stcJudgeTimeOver.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcJudgeTimeOver.SetWindowText(strNew);
}

void CSetupEquipDlg::OnBnClickedBtnCapConnect()
{
	if (!g_objCapAttach.Initialize()) return;
	m_ipaCapAttachIp.EnableWindow(FALSE);
	g_objLogFile.Save_DispatcherLog("[Cap Connect] Click");
}

void CSetupEquipDlg::OnBnClickedBtnCapDisconnect()
{
	g_objCapAttach.Terminate();
	m_ipaCapAttachIp.EnableWindow(TRUE);
	m_ipaHandlerIp.EnableWindow(TRUE);
	g_objLogFile.Save_DispatcherLog("[Cap Disconnect] Click");
}

void CSetupEquipDlg::OnStnClickedStcPasswordMt()
{
	CString strTemp, strData;

	if (g_objCommon.Show_NumPad("", strData) != IDOK) return;

	int nLen = strData.GetLength();
	if (nLen < 1) return;

	for (int i = 0; i < nLen; i++ ) {
		strTemp = strData.Mid(i, 1);
		if (strTemp < "0" || strTemp > "9") { AfxMessageBox("Password Input Error......"); return; }
	}
	m_stcPasswordMt.SetWindowText(strData);
}

void CSetupEquipDlg::OnStnClickedStcShowHidden()
{
	if (m_grpHidden.IsWindowVisible()) 
	{
		m_grpHidden.ShowWindow(SW_HIDE);
		m_lblPasswordMt.ShowWindow(SW_HIDE);
		m_stcPasswordMt.ShowWindow(SW_HIDE);
		m_chkUseDryRun.ShowWindow(SW_HIDE);
		if (g_dlgSetup.Get_LoginUser() != 2) return;
		m_lblPasswordSi.ShowWindow(SW_HIDE);
		m_edtPasswordSi.ShowWindow(SW_HIDE);		
	}
	else
	{
		m_grpHidden.ShowWindow(SW_SHOW);
		m_lblPasswordMt.ShowWindow(SW_SHOW);
		m_stcPasswordMt.ShowWindow(SW_SHOW);
		m_chkUseDryRun.ShowWindow(SW_SHOW);
		if (g_dlgSetup.Get_LoginUser() != 2) return;
		m_lblPasswordSi.ShowWindow(SW_SHOW);
		m_edtPasswordSi.ShowWindow(SW_SHOW);
	}

	if(!gData.bUseDryRun)
	{
		if (m_grpResultTest.IsWindowVisible()) 
		{
			m_grpResultTest.ShowWindow(SW_HIDE);
			m_chkResultTestUse.ShowWindow(SW_HIDE);
			for (int i = 0; i < 4; i++) m_lblResultTest[i].ShowWindow(SW_HIDE);
			for (int i = 0; i < 4; i++) m_edtResultTest[i].ShowWindow(SW_HIDE);
		} 
		else
		{
			m_grpResultTest.ShowWindow(SW_SHOW);
			m_chkResultTestUse.ShowWindow(SW_SHOW);
			for (int i = 0; i < 4; i++) m_lblResultTest[i].ShowWindow(SW_SHOW);
			for (int i = 0; i < 4; i++) m_edtResultTest[i].ShowWindow(SW_SHOW);
		}
	}


}

void CSetupEquipDlg::OnStnClickedStcVisProgVer()
{
	CString strOld, strNew;
	m_stcVisProgVer.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;
	
	int nData = atoi(strNew); strNew.Format("%04d", nData);
	m_stcVisProgVer.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStnClickedStcVisParaVer()
{
	CString strOld, strNew;
	m_stcVisParaVer.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	int nData = atoi(strNew); strNew.Format("%04d", nData);
	m_stcVisParaVer.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStnClickedStcTmrFocusLog()
{
	CString strOld, strNew;
	m_stcTmrFocusLog.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcTmrFocusLog.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStnClickedStcJigNgRate()
{
	CString strOld, strNew;
	m_stcJigNgRate.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcJigNgRate.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStnClickedStcJigNgCnt()
{
	CString strOld, strNew;
	m_stcJigNgCnt.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcJigNgCnt.SetWindowText(strNew);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CSetupEquipDlg::Initial_Controls() 
{
	CString strText;
	for (int i = 0; i < 8; i++) m_Group[i].Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i =  0; i <  6; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x10, 0xC0));	// Equip Data
	for (int i =  6; i <  8; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x10, 0xC0));	// Version
	for (int i =  8; i < 10; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0xB0, 0xFF));	// Jig NG Rate & Count
	for (int i = 10; i < 14; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x20, 0x50, 0x00));	// Tray Data
	for (int i = 14; i < 22; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x40, 0x80));	// Inspection Stage
	m_Label[22].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0xB0, 0xFF));								// NG Buffer STage
	for (int i = 23; i < 25; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));	// Vac Label
	for (int i = 25; i < 29; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x80, 0x00, 0x00));	// Vacuum Off
	for (int i = 29; i < 34; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x60, 0x40));	// Delay Add
	m_Label[34].Init_Ctrl("πŸ≈¡", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xA0, 0x00));	// Model
	for (int i = 35; i < 39; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x60, 0x40));	// Trigger

	m_stcEquipName.Init_Ctrl("πŸ≈¡", 15, TRUE, RGB(0x00, 0x00, 0x80), RGB(0xE0, 0xFF, 0xE0));
	for (int i = 0; i < 2; i++) m_rdoModel[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xF0, 0xE0, 0x00), CRadioCS::emRed, 0);
	for (int i = 0; i < 4; i++) { strText.Format("COM%d", i + 1); m_cboLotBarcodePort.AddString(strText); }
	m_cboLotBarcodePort.Init_Ctrl("πŸ≈¡", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xE0, 0x00));
	m_stcScreenOff.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0x00, 0x00, 0x80), RGB(0xF0, 0xE0, 0x00));
	m_stcNoWorkTime.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0x00, 0x00, 0x80), RGB(0xF0, 0xE0, 0x00));
	m_stcLogDate.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0x00, 0x00, 0x80), RGB(0xF0, 0xE0, 0x00));
	m_chkManualTaktTest.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xC0, 0xC0), CCheckCS::emBlue, 0);
	m_stcMotionCheck.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0x00, 0x00, 0x80), RGB(0xF0, 0xE0, 0x00));
	m_lblDoorLock.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	m_lblDoorLockTime.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	m_stcDoorLockTime.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 0; i < 2; i++) m_rdoDoorLock[i].Init_Ctrl("πŸ≈¡", 11, FALSE, COLOR_DEFAULT, RGB(0xC0, 0xC0, 0xC0), CRadioCS::emRed, 0);
	m_chkUseVisionAlign.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseInspectAngle.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseInspectBtm1_SP.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseInspectBtm1_AG.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseInspectBtm1_3D.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseInspectTop1.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseInspectTop2.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseInspectBtm2.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseDispatcher.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x90, 0x90, 0xF0), CCheckCS::emRed, 0);
	m_chkUseInline.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0x80, 0x20), CCheckCS::emRed, 0);
	m_chkUseMesApd.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x90, 0x90, 0xF0), CCheckCS::emRed, 0);
	m_chkUsePMTrigger.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x90, 0x90, 0xF0), CCheckCS::emRed, 0);
	m_chkUse2ndAVI.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x90, 0x90, 0xF0), CCheckCS::emRed, 0);
	m_chkUseDryRun.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);

	m_chkUseBtm2.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xA0, 0xF0), CCheckCS::emRed, 0);
	m_chkUseRosSkip.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xC0, 0x20), CCheckCS::emRed, 0);
	m_chkUseInspectSkip.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xC0, 0x20), CCheckCS::emRed, 0);

	for (int i = 0; i < 2; i++) m_stcTrayData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	for (int i = 2; i < 4; i++) m_stcTrayData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 0; i < 2; i++) m_stcInspectData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	for (int i = 2; i < 6; i++) m_stcInspectData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 6; i < 8; i++) m_stcInspectData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 0; i < 1; i++) m_stcNgBufferData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 0; i < 4; i++) m_stcVacOffDelay[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 4; i++) m_stcVacOffRepeat[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 5; i++) m_stcDelayAdd[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 0; i < 4; i++) m_stcTriggerData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	m_grpRos.Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 3; i++) m_lblRos[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60)); // ROS Data
	m_btnRosConnect.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_btnRosDisconnect.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_stcJudgeTimeOver.Init_Ctrl("πŸ≈¡", 10, TRUE, RGB(0x00, 0x00, 0xFF), RGB(0xE0, 0xFF, 0xE0));
	m_chkJudgeSpecialNg.Init_Ctrl("πŸ≈¡", 10, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60), CCheckCS::emBlue, 0);
	m_grpCap.Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_lblCap.Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60)); // ROS Data
	m_btnCapConnect.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_btnCapDisconnect.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_grpTower.Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 10; i++) m_lblTower[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) m_chkTower[i][j].Init_Ctrl("πŸ≈¡", 11, FALSE, COLOR_DEFAULT, RGB(0x00, 0xF0, 0x00), CCheckCS::emRed, 0);
	m_grpBuzzer.Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 8; i++) m_lblBuzzer[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 2; i++) for (int j = 0; j < 6; j++) m_chkBuzzer[i][j].Init_Ctrl("πŸ≈¡", 11, FALSE, COLOR_DEFAULT, RGB(0x00, 0xE0, 0xF0), CCheckCS::emRed, 0);
	m_grpHidden.Init_Ctrl("πŸ≈¡", 10, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_lblPasswordMt.Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	m_stcPasswordMt.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0xC0, 0xC0, 0xC0));
	m_lblPasswordSi.Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	m_edtPasswordSi.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_grpResultTest.Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_chkResultTestUse.Init_Ctrl("πŸ≈¡", 10, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60), CCheckCS::emRed, 0);
	for (int i = 0; i < 4; i++) m_lblResultTest[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	for (int i = 0; i < 4; i++) m_edtResultTest[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_stcVisProgVer.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	m_stcVisParaVer.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	m_chkUseFocusLog.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x10, 0xC0), CCheckCS::emRed, 0);
	m_stcTmrFocusLog.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	m_stcJigNgRate.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	m_stcJigNgCnt.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
}

void CSetupEquipDlg::Display_EquipData()
{
	CString strData;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	m_chkUseDryRun.SetCheck(pEquipData->bUseDryRun);

	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg *)AfxGetApp()->GetMainWnd();
	if(gData.bUseDryRun)
	{
		pMainDlg->m_stcMainEquip.Init_Ctrl("Segoe UI", 14, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0x00, 0x00));
		pMainDlg->m_stcMainEquip.Init_Ctrl("Segoe UI", 14, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0x00, 0x00));
	}
	else
	{
		pMainDlg->m_stcMainEquip.Init_Ctrl("Segoe UI", 14, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xE6, 0xE6, 0xE6));
		pMainDlg->m_stcMainEquip.Init_Ctrl("Segoe UI", 14, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xE6, 0xE6, 0xE6));
	}


	m_stcEquipName.SetWindowText(pEquipData->sEquipName);
	gData.sRecipe == "R54B" ? m_rdoModel[1].SetCheck(TRUE) : m_rdoModel[0].SetCheck(TRUE);
	m_cboLotBarcodePort.SetCurSel(pEquipData->nLotBarcodePort - 1);
	strData.Format("%d", pEquipData->nScreenOff); m_stcScreenOff.SetWindowText(strData);
	strData.Format("%d", pEquipData->nNoWorkTime); m_stcNoWorkTime.SetWindowText(strData);
	strData.Format("%d", pEquipData->nLogDate); m_stcLogDate.SetWindowText(strData);
	strData.Format("%0.3lf", gAlm.dMotionChkPos);	 m_stcMotionCheck.SetWindowText(strData);
	m_chkManualTaktTest.SetCheck(pEquipData->bManualTaktTest);
	pEquipData->bUseDoorLock ? m_rdoDoorLock[1].SetCheck(TRUE) : m_rdoDoorLock[0].SetCheck(TRUE);
	strData.Format("%d", gData.nDoorLockTime);
	m_stcDoorLockTime.SetWindowText(strData);

	m_chkUseVisionAlign.SetCheck(pEquipData->bUseVisionAlign);
	m_chkUseInspectAngle.SetCheck(pEquipData->bUseInspectAngle);
	m_chkUseInspectBtm1_SP.SetCheck(pEquipData->bUseInspectBtm1Specular);
	m_chkUseInspectBtm1_AG.SetCheck(pEquipData->bUseInspectBtm1Angle);
	m_chkUseInspectBtm1_3D.SetCheck(pEquipData->bUseInspectBtm13D);
	m_chkUseInspectTop1.SetCheck(pEquipData->bUseInspectTop1);
	m_chkUseInspectTop2.SetCheck(pEquipData->bUseInspectTop2);
	m_chkUseInspectBtm2.SetCheck(pEquipData->bUseInspectBtm2);
	m_chkUseDispatcher.SetCheck(pEquipData->bUseDispatcher);
	m_chkUseInline.SetCheck(pEquipData->bUseInlineMode);
	m_chkUseMesApd.SetCheck(pEquipData->bUseApdAlarm);
	m_chkUsePMTrigger.SetCheck(pEquipData->bUsePMTrigger);
	m_chkUse2ndAVI.SetCheck(pEquipData->bUse2ndAVI);
	m_chkUseBtm2.SetCheck(pEquipData->bUseBtm2PickUpDown);
	m_chkUseRosSkip.SetCheck(pEquipData->bUseRosSkip);
	m_chkUseInspectSkip.SetCheck(pEquipData->bUseInspectSkip);
	

	strData.Format("%s", pEquipData->sVisionProgVer); m_stcVisProgVer.SetWindowText(strData);
	strData.Format("%s", pEquipData->sVisionParaVer); m_stcVisParaVer.SetWindowText(strData);
	m_chkUseFocusLog.SetCheck(pEquipData->bUseFocusLog);
	strData.Format("%d", pEquipData->nTmrFocusLog); m_stcTmrFocusLog.SetWindowText(strData);
	strData.Format("%d", pEquipData->nJigNgRate); m_stcJigNgRate.SetWindowText(strData);
	strData.Format("%d", pEquipData->nJigNgCnt); m_stcJigNgCnt.SetWindowText(strData);

	strData.Format("%0.2lf", pEquipData->dTriggerStart); m_stcTriggerData[0].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dTriggerEnd); m_stcTriggerData[1].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dTriggerPeriod); m_stcTriggerData[2].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dTriggerVel); m_stcTriggerData[3].SetWindowText(strData);

	strData.Format("%0.2lf", pEquipData->dTrayPitchX); m_stcTrayData[2].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dTrayPitchY); m_stcTrayData[3].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dInspectPitchX); m_stcInspectData[2].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dInspectPitchY); m_stcInspectData[3].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dInspectALOffset); m_stcInspectData[4].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dInspectALBackOffset); m_stcInspectData[5].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dInspectTop2ShiftX); m_stcInspectData[6].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dInspectBTM1ShiftX); m_stcInspectData[7].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dNgBufferPitchX); m_stcNgBufferData[0].SetWindowText(strData);

	for (int i = 0; i < 4; i++) { strData.Format("%d", pEquipData->nVacOffDelay[i]); m_stcVacOffDelay[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%d", pEquipData->nVacOffRepeat[i]); m_stcVacOffRepeat[i].SetWindowText(strData); }
	for (int i = 0; i < 5; i++) { strData.Format("%d", pEquipData->nDelayAdd[i]); m_stcDelayAdd[i].SetWindowText(strData); }

	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	m_ipaDispatcherIp.SetWindowText(pRosData->sDispatcherIp);
	m_ipaHandlerIp.SetWindowText(pRosData->sHandlerIp);
	BOOL bConnected = g_objDispatcher.Is_Connected();
	m_ipaDispatcherIp.EnableWindow(!bConnected);
	m_ipaHandlerIp.EnableWindow(!bConnected);
	m_btnRosConnect.EnableWindow(!bConnected);
	m_btnRosDisconnect.EnableWindow(bConnected);
	strData.Format("%d", pRosData->nJudgeTimeOver); m_stcJudgeTimeOver.SetWindowText(strData);
	m_chkJudgeSpecialNg.SetCheck(pRosData->bJudgeSpecialNg);

	m_ipaCapAttachIp.EnableWindow(!g_objCapAttach.Is_Opened());
	m_ipaCapAttachIp.SetWindowText(pEquipData->sCapAttachIp);
	for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) m_chkTower[i][j].SetCheck(pEquipData->bTower[i][j]);
	for (int i = 0; i < 2; i++) for (int j = 0; j < 6; j++) m_chkBuzzer[i][j].SetCheck(pEquipData->bBuzzer[i][j]);
	m_stcPasswordMt.SetWindowText(pEquipData->sPasswordMt);
	m_edtPasswordSi.SetWindowText(pEquipData->sPasswordSi);
	m_chkResultTestUse.SetCheck(pEquipData->bResultTestUse);
	strData.Format("%d", pEquipData->nResultTestN1); m_edtResultTest[0].SetWindowText(strData);
	strData.Format("%d", pEquipData->nResultTestN2); m_edtResultTest[1].SetWindowText(strData);
	strData.Format("%d", pEquipData->nResultTestN3); m_edtResultTest[2].SetWindowText(strData);
	strData.Format("%d", pEquipData->nResultTestN4); m_edtResultTest[3].SetWindowText(strData);

	// ∫Ø∞Ê ∫“∞°
	strData.Format("%d", gData.nTrayX); m_stcTrayData[0].SetWindowText(strData);		// Tray Count X
	strData.Format("%d", gData.nTrayY);	m_stcTrayData[1].SetWindowText(strData);		// Tray Count Y

	strData.Format("%d", gData.nBtmPickQt );m_stcInspectData[0].SetWindowText(strData);	// Inspect Count X
	m_stcInspectData[1].SetWindowText("2");												// Inspect Count Y
}

void CSetupEquipDlg::Save_EquipData()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CString strKey, strData;
	int nData;
	double dData;

	CIniFileCS INI(gData.sEnvPath + "\\EquipData.ini");
	if (!INI.Check_File()) { AfxMessageBox("EquipData.ini File Not Found!!!"); return; }

	g_objCommon.Backup_File(gData.sEnvPath, "EquipData");

	m_stcEquipName.GetWindowText(strData); INI.Set_String("EQUIPMENT", "NAME", strData);

	gData.sRecipe = (m_rdoModel[1].GetCheck() ? "R54B" : "R53B");
	g_objDataManager.Save_ModelData();

	nData = m_cboLotBarcodePort.GetCurSel(); INI.Set_Integer("EQUIPMENT", "LOT_BARCODE", nData + 1);
	m_stcScreenOff.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("EQUIPMENT", "SCREEN_OFF", nData);
	m_stcNoWorkTime.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("EQUIPMENT", "NOWORK_TIME", nData);
	m_stcLogDate.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("EQUIPMENT", "LOG_SAVE_DATE", nData);
	INI.Set_Bool("EQUIPMENT", "MANUAL_TAKT_TEST", m_chkManualTaktTest.GetCheck());
	INI.Set_Bool("EQUIPMENT", "DOOR_LOCK", m_rdoDoorLock[1].GetCheck());

	if(m_rdoDoorLock[1].GetCheck()) pEquipData->bUseDoorLock = TRUE;
	else	pEquipData->bUseDoorLock = FALSE;

	//Save «“∂ß door interlock Ω√¿€ Ω√∞£ π◊ ≥° Ω√∞£ ∞ËªÍ 
	if(m_rdoDoorLock[1].GetCheck() && gData.bWasUnlock)
	{
		gData.tDoorUnlockEnd = CTime::GetCurrentTime();
		gData.tsDoorUnlockTotal += gData.tDoorUnlockEnd - gData.tDoorUnlockStart;
		gData.tDoorUnlockEnd = CTime();
		gData.tDoorUnlockStart = CTime();
		gData.bWasUnlock = FALSE;

		SaveLog_DoorInterlock("A2", "OK");
	}
	else if(!m_rdoDoorLock[1].GetCheck())
	{
		if(!gData.bWasUnlock){
			gData.bWasUnlock = TRUE;
			gData.tDoorUnlockStart = CTime::GetCurrentTime();
			SaveLog_DoorInterlock("F", "NG");
		}
	}

	m_stcDoorLockTime.GetWindowText(strData);
	gData.nDoorLockTime = atoi(strData);
	INI.Set_Integer("EQUIPMENT", "DOOR_LOCK_TIME", gData.nDoorLockTime);

	m_stcMotionCheck.GetWindowText(strData); dData = atof(strData); INI.Set_Double("EQUIPMENT", "MOTION_CHECK", dData, "%0.3lf");


	if(!gData.bUseDryRun)
	{
		INI.Set_Bool("OPTION", "VISION_ALIGN", m_chkUseVisionAlign.GetCheck());
		INI.Set_Bool("OPTION", "INSPECT_ANGLE", m_chkUseInspectAngle.GetCheck());
		INI.Set_Bool("OPTION", "INSPECT_BTM_1_SP", m_chkUseInspectBtm1_SP.GetCheck());
		INI.Set_Bool("OPTION", "INSPECT_BTM_1_AG", m_chkUseInspectBtm1_AG.GetCheck());
		INI.Set_Bool("OPTION", "INSPECT_BTM_1_3D", m_chkUseInspectBtm1_3D.GetCheck());
		INI.Set_Bool("OPTION", "INSPECT_TOP_1", m_chkUseInspectTop1.GetCheck());
		INI.Set_Bool("OPTION", "INSPECT_TOP_2", m_chkUseInspectTop2.GetCheck());
		INI.Set_Bool("OPTION", "INSPECT_BTM_2", m_chkUseInspectBtm2.GetCheck());
		INI.Set_Bool("OPTION", "DISPATCHER", m_chkUseDispatcher.GetCheck());
		INI.Set_Bool("OPTION", "INLINE_MODE", m_chkUseInline.GetCheck());
		INI.Set_Bool("OPTION", "MES_APD", m_chkUseMesApd.GetCheck());
		INI.Set_Bool("OPTION", "PM_TRIGGER", m_chkUsePMTrigger.GetCheck());
		INI.Set_Bool("OPTION", "SECOND_AVI", m_chkUse2ndAVI.GetCheck());	
	}
	
	INI.Set_Bool("OPTION", "DRY_RUN", m_chkUseDryRun.GetCheck());

	INI.Set_Bool("OPTION", "BTM2_PICK_UPDOWN", m_chkUseBtm2.GetCheck());
	INI.Set_Bool("OPTION", "ROS_SKIP", m_chkUseRosSkip.GetCheck());
	INI.Set_Bool("OPTION", "INSPECT_SKIP", m_chkUseInspectSkip.GetCheck());

	m_stcVisProgVer.GetWindowText(strData); INI.Set_String("OPTION", "VIS_PROG_VER", strData);
	m_stcVisParaVer.GetWindowText(strData); INI.Set_String("OPTION", "VIS_PARA_VER", strData);
	INI.Set_Bool("OPTION", "USE_FOCUS_LOG", m_chkUseFocusLog.GetCheck());
	m_stcTmrFocusLog.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("OPTION", "TMR_FOCUS_LOG", nData);
	m_stcJigNgRate.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("OPTION", "JIG_NG_RATE", nData);
	m_stcJigNgCnt.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("OPTION", "JIG_NG_CNT", nData);

	m_stcTrayData[2].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("TRAY", "PITCH_X", dData, "%0.2lf");
	m_stcTrayData[3].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("TRAY", "PITCH_Y", dData, "%0.2lf");
	m_stcInspectData[2].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "PITCH_X", dData, "%0.2lf");
	m_stcInspectData[3].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "PITCH_Y", dData, "%0.2lf");
	m_stcInspectData[4].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "AL_OFFSET", dData, "%0.2lf");
	m_stcInspectData[5].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "AL_BACK_OFFSET", dData, "%0.2lf");
	m_stcInspectData[6].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "TOP2_SHIFT_X", dData, "%0.2lf");
	m_stcInspectData[7].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "BTM1_SHIFT_X", dData, "%0.2lf");

	m_stcNgBufferData[0].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("NG_BUFFER", "PITCH_X", dData, "%0.2lf");

	for (int i = 0; i < 4; i++) { strKey.Format("%d", i); m_stcVacOffDelay[i].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("VAC_OFF_DELAY", strKey, nData); }
	for (int i = 0; i < 4; i++) { strKey.Format("%d", i); m_stcVacOffRepeat[i].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("VAC_OFF_REPEAT", strKey, nData); }
	for (int i = 0; i < 5; i++) { strKey.Format("%d", i); m_stcDelayAdd[i].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("DELAY_ADD", strKey, nData); }
	for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) { strKey.Format("%d%d", i, j); INI.Set_Bool("TOWER", strKey, m_chkTower[i][j].GetCheck()); }
	for (int i = 0; i < 2; i++) for (int j = 0; j < 6; j++) { strKey.Format("%d%d", i, j); INI.Set_Bool("BUZZER", strKey, m_chkBuzzer[i][j].GetCheck()); }

	m_stcPasswordMt.GetWindowText(strData); INI.Set_String("HIDDEN", "PASSWORD_MT", strData);
	m_edtPasswordSi.GetWindowText(strData); INI.Set_String("HIDDEN", "PASSWORD_SI", strData);

	m_ipaCapAttachIp.GetWindowText(strData); INI.Set_String("CAPATTACH", "CAP_IP", strData);

	m_stcTriggerData[0].GetWindowText(strData); INI.Set_String("TRIGGER", "START", strData);
	m_stcTriggerData[1].GetWindowText(strData); INI.Set_String("TRIGGER", "END", strData);
	m_stcTriggerData[2].GetWindowText(strData); INI.Set_String("TRIGGER", "PERIOD", strData);
	m_stcTriggerData[3].GetWindowText(strData); INI.Set_String("TRIGGER", "WIDTH", strData);


	pEquipData->bResultTestUse = m_chkResultTestUse.GetCheck();
	m_edtResultTest[0].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("RESULT_TEST", "N1", nData);
	m_edtResultTest[1].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("RESULT_TEST", "N2", nData);
	m_edtResultTest[2].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("RESULT_TEST", "N3", nData);
	m_edtResultTest[3].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("RESULT_TEST", "N4", nData);

	g_objLogFile.Save_HandlerLog("[Setup Equip] Save Click");

	CIniFileCS INI2(gData.sEnvPath + "\\RosData.ini");
	if (!INI2.Check_File()) { AfxMessageBox("RosData.ini File Not Found!!!"); return; }

	m_ipaDispatcherIp.GetWindowText(strData); INI2.Set_String("DISPATCHER", "DISPATCHER_IP", strData);
	m_ipaHandlerIp.GetWindowText(strData); INI2.Set_String("DISPATCHER", "HANDLER_IP", strData);
	m_stcJudgeTimeOver.GetWindowText(strData); nData = atoi(strData); INI2.Set_Integer("DISPATCHER", "TIME_OVER", nData);
	INI2.Set_Bool("DISPATCHER", "SPECIAL_NG", m_chkJudgeSpecialNg.GetCheck());

	Cancel_EquipData();
}


void CSetupEquipDlg::Save_EquipDataForAllParam()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CString strKey, strData;
	int nData;
	double dData;

	CIniFileCS INI(gData.sEnvPath + "\\AllParam.ini");
	if (!INI.Check_File()) { AfxMessageBox("AllParam.ini File Not Found!!!"); return; }

	g_objCommon.Backup_File(gData.sEnvPath, "EquipData");

	m_stcEquipName.GetWindowText(strData); INI.Set_String("EQUIPMENT", "NAME", strData);

	gData.sRecipe = (m_rdoModel[1].GetCheck() ? "R54B" : "R53B");
	g_objDataManager.Save_ModelData();

	nData = m_cboLotBarcodePort.GetCurSel(); INI.Set_Integer("EQUIPMENT", "LOT_BARCODE", nData + 1);
	m_stcScreenOff.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("EQUIPMENT", "SCREEN_OFF", nData);
	m_stcNoWorkTime.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("EQUIPMENT", "NOWORK_TIME", nData);
	m_stcLogDate.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("EQUIPMENT", "LOG_SAVE_DATE", nData);
	INI.Set_Bool("EQUIPMENT", "MANUAL_TAKT_TEST", m_chkManualTaktTest.GetCheck());
	INI.Set_Bool("EQUIPMENT", "DOOR_LOCK", m_rdoDoorLock[1].GetCheck());
	m_stcMotionCheck.GetWindowText(strData); dData = atof(strData); INI.Set_Double("EQUIPMENT", "MOTION_CHECK", dData, "%0.3lf");

	if(!gData.bUseDryRun)
	{
		INI.Set_Bool("OPTION", "VISION_ALIGN", m_chkUseVisionAlign.GetCheck());
		INI.Set_Bool("OPTION", "INSPECT_ANGLE", m_chkUseInspectAngle.GetCheck());
		INI.Set_Bool("OPTION", "INSPECT_BTM_1_SP", m_chkUseInspectBtm1_SP.GetCheck());
		INI.Set_Bool("OPTION", "INSPECT_BTM_1_AG", m_chkUseInspectBtm1_AG.GetCheck());
		INI.Set_Bool("OPTION", "INSPECT_BTM_1_3D", m_chkUseInspectBtm1_3D.GetCheck());
		INI.Set_Bool("OPTION", "INSPECT_TOP_1", m_chkUseInspectTop1.GetCheck());
		INI.Set_Bool("OPTION", "INSPECT_TOP_2", m_chkUseInspectTop2.GetCheck());
		INI.Set_Bool("OPTION", "INSPECT_BTM_2", m_chkUseInspectBtm2.GetCheck());
		INI.Set_Bool("OPTION", "DISPATCHER", m_chkUseDispatcher.GetCheck());
		INI.Set_Bool("OPTION", "INLINE_MODE", m_chkUseInline.GetCheck());
		INI.Set_Bool("OPTION", "MES_APD", m_chkUseMesApd.GetCheck());
		INI.Set_Bool("OPTION", "PM_TRIGGER", m_chkUsePMTrigger.GetCheck());
		INI.Set_Bool("OPTION", "SECOND_AVI", m_chkUse2ndAVI.GetCheck());	
	}
	
	INI.Set_Bool("OPTION", "BTM2_PICK_UPDOWN", m_chkUseBtm2.GetCheck());
	INI.Set_Bool("OPTION", "ROS_SKIP", m_chkUseRosSkip.GetCheck());
	INI.Set_Bool("OPTION", "INSPECT_SKIP", m_chkUseInspectSkip.GetCheck());

	m_stcVisProgVer.GetWindowText(strData); INI.Set_String("OPTION", "VIS_PROG_VER", strData);
	m_stcVisParaVer.GetWindowText(strData); INI.Set_String("OPTION", "VIS_PARA_VER", strData);
	INI.Set_Bool("OPTION", "USE_FOCUS_LOG", m_chkUseFocusLog.GetCheck());
	m_stcTmrFocusLog.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("OPTION", "TMR_FOCUS_LOG", nData);
	m_stcJigNgRate.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("OPTION", "JIG_NG_RATE", nData);
	m_stcJigNgCnt.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("OPTION", "JIG_NG_CNT", nData);

	m_stcTrayData[2].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("TRAY", "PITCH_X", dData, "%0.2lf");
	m_stcTrayData[3].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("TRAY", "PITCH_Y", dData, "%0.2lf");
	m_stcInspectData[2].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "PITCH_X", dData, "%0.2lf");
	m_stcInspectData[3].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "PITCH_Y", dData, "%0.2lf");
	m_stcInspectData[4].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "AL_OFFSET", dData, "%0.2lf");
	m_stcInspectData[5].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "AL_BACK_OFFSET", dData, "%0.2lf");
	m_stcInspectData[6].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "TOP2_SHIFT_X", dData, "%0.2lf");
	m_stcInspectData[7].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "BTM1_SHIFT_X", dData, "%0.2lf");

	m_stcNgBufferData[0].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("NG_BUFFER", "PITCH_X", dData, "%0.2lf");

	for (int i = 0; i < 4; i++) { strKey.Format("%d", i); m_stcVacOffDelay[i].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("VAC_OFF_DELAY", strKey, nData); }
	for (int i = 0; i < 4; i++) { strKey.Format("%d", i); m_stcVacOffRepeat[i].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("VAC_OFF_REPEAT", strKey, nData); }
	for (int i = 0; i < 5; i++) { strKey.Format("%d", i); m_stcDelayAdd[i].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("DELAY_ADD", strKey, nData); }
	for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) { strKey.Format("%d%d", i, j); INI.Set_Bool("TOWER", strKey, m_chkTower[i][j].GetCheck()); }
	for (int i = 0; i < 2; i++) for (int j = 0; j < 6; j++) { strKey.Format("%d%d", i, j); INI.Set_Bool("BUZZER", strKey, m_chkBuzzer[i][j].GetCheck()); }

	m_stcPasswordMt.GetWindowText(strData); INI.Set_String("HIDDEN", "PASSWORD_MT", strData);
	m_edtPasswordSi.GetWindowText(strData); INI.Set_String("HIDDEN", "PASSWORD_SI", strData);

	m_ipaCapAttachIp.GetWindowText(strData); INI.Set_String("CAPATTACH", "CAP_IP", strData);

	m_stcTriggerData[0].GetWindowText(strData); INI.Set_String("TRIGGER", "START", strData);
	m_stcTriggerData[1].GetWindowText(strData); INI.Set_String("TRIGGER", "END", strData);
	m_stcTriggerData[2].GetWindowText(strData); INI.Set_String("TRIGGER", "PERIOD", strData);
	m_stcTriggerData[3].GetWindowText(strData); INI.Set_String("TRIGGER", "WIDTH", strData);


	pEquipData->bResultTestUse = m_chkResultTestUse.GetCheck();
	m_edtResultTest[0].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("RESULT_TEST", "N1", nData);
	m_edtResultTest[1].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("RESULT_TEST", "N2", nData);
	m_edtResultTest[2].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("RESULT_TEST", "N3", nData);
	m_edtResultTest[3].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("RESULT_TEST", "N4", nData);

	g_objLogFile.Save_HandlerLog("[Setup Equip] Save Click");
	
}

void CSetupEquipDlg::Cancel_EquipData()
{
 	g_objDataManager.Read_ModelData();
	g_objDataManager.Read_EquipData();
	g_objDataManager.Read_IdleData();
	g_objDataManager.Read_RosData();

	Display_EquipData();

	g_objLogFile.Save_HandlerLog("[Setup Equip] Cancel Click");
}

void CSetupEquipDlg::Disable_RosCtrl(BOOL bDisable)
{
	m_ipaDispatcherIp.EnableWindow(!bDisable);
	m_ipaHandlerIp.EnableWindow(!bDisable);
	m_btnRosConnect.EnableWindow(!bDisable);
	m_btnRosDisconnect.EnableWindow(bDisable);
}

///////////////////////////////////////////////////////////////////////////////

void CSetupEquipDlg::SaveLog_DoorInterlock(CString resultCode, CString doorLockUse)
{
	int nLotNo = 0;
	
	CString strInterlockLog, strSite, strLine, strResultCode, strStateOfDoors, strLotID;
	strResultCode = resultCode; //∏≈ ¡§Ω√ ≥≤±‚¥¬ ∑Œ±◊ : 'A1',  off : F, off-->On : A2
	strSite = "Gumi C3"; strLine = "C3";

	EQUIP_DATA *m_pEquipData = g_objDataManager.Get_pEquipData();
	//CString strDoorLockUse = m_pEquipData->bUseDoorLock ? "OK" : "NG";
	
	CString strState = theApp.Get_MainState() == 0 ? "None" : 
		(theApp.Get_MainState() == 1 ? "Init":
		(theApp.Get_MainState() == 2 ? "Stop":
		(theApp.Get_MainState() == 3 ? "Running":
		(theApp.Get_MainState() == 4 ? "Alarm":
		(theApp.Get_MainState() == 5 ? "Error": "None")))));
		
	nLotNo = gData.nLPNo;
	if(gData.nLPNo == 0) nLotNo = 1;

	strLotID = gData.sLotID[nLotNo];
	if(gData.sLotID[nLotNo] == "LOT_ID") strLotID = "";

	strStateOfDoors = g_objCommon.Get_StateOfDoors();
	strInterlockLog.Format("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,,,%s", strLotID,"" ,MAIN_VERSION, strState,  
		strSite, strLine, m_pEquipData->sEquipName, gData.sOperID,strResultCode, doorLockUse, strStateOfDoors );

	g_objLogFile.Save_DoorInterlock(nLotNo, strInterlockLog, FALSE);
}

void CSetupEquipDlg::OnStnClickedStcDoorlockTime()
{
	CString strOld, strNew;
	m_stcDoorLockTime.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) == IDOK)
	{
		int nCnt = atoi(strNew);
		if (nCnt < 0) {
			AfxMessageBox("Door Lock Time input Error.....");
			return;
		}
	}

	m_stcDoorLockTime.SetWindowText(strNew);
}


void CSetupEquipDlg::OnBnClickedChkUseVisionAlign()
{
	m_strLog.Format("Vision Align Checked : %d", m_chkUseVisionAlign.GetCheck());
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupEquipDlg::OnBnClickedChkUseDryRun()
{
	m_strLog.Format("Dry Run Checked : %d", m_chkUseDryRun.GetCheck());
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupEquipDlg::OnBnClickedChkUseInspectAngle()
{
	m_strLog.Format("Angle Vision Checked : %d", m_chkUseInspectAngle.GetCheck());
	g_objLogFile.Save_HandlerLog(m_strLog);
}


void CSetupEquipDlg::OnBnClickedChkUseInspectBtm1Sp()
{
	m_strLog.Format("Btm1SP Checked : %d", m_chkUseInspectAngle.GetCheck());
	g_objLogFile.Save_HandlerLog(m_strLog);
}


void CSetupEquipDlg::OnBnClickedChkUseInspectBtm1Ag()
{
	m_strLog.Format("Btm1AG Checked : %d", m_chkUseInspectAngle.GetCheck());
	g_objLogFile.Save_HandlerLog(m_strLog);
}


void CSetupEquipDlg::OnBnClickedChkUseInspectBtm13d()
{
	m_strLog.Format("Btm13D Checked : %d", m_chkUseInspectAngle.GetCheck());
	g_objLogFile.Save_HandlerLog(m_strLog);
}


void CSetupEquipDlg::OnBnClickedChkUseInspectTop1()
{
	m_strLog.Format("Top1Vision Checked : %d", m_chkUseInspectAngle.GetCheck());
	g_objLogFile.Save_HandlerLog(m_strLog);
}


void CSetupEquipDlg::OnBnClickedChkUseInspectTop2()
{
	m_strLog.Format("Top2Vision Checked : %d", m_chkUseInspectAngle.GetCheck());
	g_objLogFile.Save_HandlerLog(m_strLog);
}


void CSetupEquipDlg::OnBnClickedChkUseInspectBtm2()
{
	m_strLog.Format("Btm2Vision Checked : %d", m_chkUseInspectAngle.GetCheck());
	g_objLogFile.Save_HandlerLog(m_strLog);
}


void CSetupEquipDlg::OnBnClickedChkUseInline()
{
	m_strLog.Format("UseInline Checked : %d", m_chkUseInspectAngle.GetCheck());
	g_objLogFile.Save_HandlerLog(m_strLog);
}


void CSetupEquipDlg::OnBnClickedChkUseBtm2()
{
	m_strLog.Format("PickUpOption Checked : %d", m_chkUseInspectAngle.GetCheck());
	g_objLogFile.Save_HandlerLog(m_strLog);
}


void CSetupEquipDlg::OnBnClickedRdoDoorLock0()
{
	m_strLog.Format("DoorLockNotUse Checked : %d", m_chkUseInspectAngle.GetCheck());
	g_objLogFile.Save_HandlerLog(m_strLog);
}


void CSetupEquipDlg::OnBnClickedRdoDoorLock1()
{
	m_strLog.Format("DoorLockUse Checked : %d", m_chkUseInspectAngle.GetCheck());
	g_objLogFile.Save_HandlerLog(m_strLog);
}
