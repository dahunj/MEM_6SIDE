// ErrorDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "ErrorDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "Common.h"
#include "MesAgent.h"

#include "SequenceInit.h"
#include "SequenceMain.h"
#include "Dispatcher.h"

#include "WorkDlg.h"
#include "ManualDlg.h"
#include "CMI8000Dlg.h"

// CErrorDlg 대화 상자입니다.
CErrorDlg g_dlgError;

IMPLEMENT_DYNAMIC(CErrorDlg, CDialogEx)

CErrorDlg::CErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CErrorDlg::IDD, pParent)
{
}

CErrorDlg::~CErrorDlg()
{
}

void CErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_ERR_BACK_0 + i, m_stcErrBack[i]);
	DDX_Control(pDX, IDC_STC_ERR_TITLE, m_stcErrTitle);
	DDX_Control(pDX, IDC_IMAGE_0, m_Image);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_LED_MAIN_AIR_0 + i, m_ledMainAir[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_LED_EMG_SW_0 + i, m_ledEmgSw[i]);
	for (int i = 0; i < 19; i++) DDX_Control(pDX, IDC_LED_DOOR_OPEN_0 + i, m_ledDoorOpen[i]);
	for (int i = 0; i < 14; i++) DDX_Control(pDX, IDC_STC_ERR_POS_0 + i, m_stcErrPos[i]);
	DDX_Control(pDX, IDC_STC_ERR_NO, m_stcErrNo);
	DDX_Control(pDX, IDC_STC_MSG_BACK, m_stcMsgBack);
	DDX_Control(pDX, IDC_STC_ERR_MSG, m_stcErrMsg);
	DDX_Control(pDX, IDC_BTN_ERR_BUZZ_OFF, m_btnErrBuzzOff);
	DDX_Control(pDX, IDC_BTN_ERR_SKIP, m_btnErrSkip);
	DDX_Control(pDX, IDC_BTN_ERR_RETRY, m_btnErrRetry);
	DDX_Control(pDX, IDC_BTN_ERR_OK, m_btnErrOK);
	DDX_Control(pDX, IDC_BTN_ERR_SYSTEM_EXIT, m_btnErrSystemExit);
	DDX_Control(pDX, IDC_BTN_ERR_TO_MANUAL, m_btnErrToManual);
}

BEGIN_MESSAGE_MAP(CErrorDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_ERR_BUZZ_OFF, &CErrorDlg::OnBnClickedBtnErrBuzzOff)
	ON_BN_CLICKED(IDC_BTN_ERR_SKIP, &CErrorDlg::OnBnClickedBtnErrSkip)
	ON_BN_CLICKED(IDC_BTN_ERR_RETRY, &CErrorDlg::OnBnClickedBtnErrRetry)
	ON_BN_CLICKED(IDC_BTN_ERR_OK, &CErrorDlg::OnBnClickedBtnErrOk)
	ON_BN_CLICKED(IDC_BTN_ERR_SYSTEM_EXIT, &CErrorDlg::OnBnClickedBtnErrSystemExit)
	ON_BN_CLICKED(IDC_BTN_ERR_TO_MANUAL, &CErrorDlg::OnBnClickedBtnErrToManual)
END_MESSAGE_MAP()

// CErrorDlg 메시지 처리기입니다.

BOOL CErrorDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	Initial_Controls();

	m_nErrNo = 0;
	m_nBackColorLoop = 0;
	m_nErrorPos = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CErrorDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CErrorDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	int nNo = 0;
	if (gData.nLPNo > 0 || gData.nULPNo > 0) {
		if (gData.nULPNo > 0) nNo = gData.nULPNo - 1;
		if (gData.nLPNo > 0)  nNo = gData.nLPNo - 1;

	} else { nNo = 0;}

	if (bShow) {
		BringWindowToTop();	// 화면 위로...

		CString strErrNo, strErrMsg, strShow, strLog;

		pMainDlg->Set_CurrentState(STATE_ERROR);

		strErrNo.Format("%04d", m_nErrNo);
		m_stcErrNo.SetWindowText(strErrNo);
		
		CIniFileCS INI(gData.sEnvPath + "\\ErrorList.ini");
		if (!INI.Check_File()) { AfxMessageBox("ErrorList.ini File Not Found!!!"); return; }
		strErrMsg = INI.Get_String("ERROR", strErrNo, "");

		CString strErrPick = "";
		if (m_nErrNo==2401) {
// 			if (gData.nErrIndxNo == 1) strErrPick.Format(" ==> Load Index Z2축 Ready Up 후 진행하세요.");
// 			if (gData.nErrIndxNo == 2) strErrPick.Format(" ==> Vision Top1 Z축 Ready Up 후 진행하세요.");
// 			if (gData.nErrIndxNo == 3) strErrPick.Format(" ==> Vision Top2 Z축 Ready Up 후 진행하세요.");
// 			if (gData.nErrIndxNo == 4) strErrPick.Format(" ==> Vision Top3 Z축 Ready Up 후 진행하세요.");
// 			if (gData.nErrIndxNo == 7) strErrPick.Format(" ==> Top2 Prism Z축 Ready Up 후 진행하세요.");
// 			if (gData.nErrIndxNo == 8) strErrPick.Format(" ==> Trans Index Z1축 Ready Up 후 진행하세요.");
// 			if (gData.nErrIndxNo == 9) strErrPick.Format(" ==> Jig Align Master/Slave Out 후 진행하세요.");
// 			if (gData.nErrIndxNo == 11) strErrPick.Format(" ==> Load Index Z1축 Ready Up 후 진행하세요.");
// 			if (gData.nErrIndxNo == 12) strErrPick.Format(" ==> Vision BTM1 Z축 Ready Up 후 진행하세요.");
// 			if (gData.nErrIndxNo == 13) strErrPick.Format(" ==> Load Index Z2축 Ready Up 후 진행하세요.");
// 			if (gData.nErrIndxNo == 14) strErrPick.Format(" ==> Trans Index Z1축 Ready Up 후 진행하세요.");
// 			if (gData.nErrIndxNo == 15) strErrPick.Format(" ==> Vision BTM2 Z축 Ready Up 후 진행하세요.");
// 			if (gData.nErrIndxNo == 16) strErrPick.Format(" ==> BTM2 Prism Z축 Ready Down 후 진행하세요.");
// 			if (gData.nErrIndxNo == 17) strErrPick.Format(" ==> Trans Index Z2축 Ready Up 후 진행하세요.");
		}

		if (m_nErrNo == 6101 || m_nErrNo == 6102 || m_nErrNo == 6103 || m_nErrNo == 6104 || m_nErrNo == 6110 ||
			m_nErrNo == 6111 || m_nErrNo == 6120 || m_nErrNo == 6121 || m_nErrNo == 4306 || m_nErrNo == 4406) {
			
				strErrMsg += m_strErrSubMsg;	// 검사완료 Error

		} else if (m_nErrNo == 3315 || m_nErrNo == 3610 || m_nErrNo == 3710 || m_nErrNo == 3720 ||
				   m_nErrNo == 3810 || m_nErrNo == 3820 || m_nErrNo == 3910 || m_nErrNo == 3920 || m_nErrNo == 4010) {

				strErrMsg += m_strErrSubMsg;	// 스캔완료 Error

		} else if (m_nErrNo == 6150) {

			strErrMsg += m_strErrSubMsg;	// 동일 불량 알람. LOT ID가 뒤에 LOT ID로 표시되지 않게 해준다.

		} else if (m_nErrNo == 6155) {
			strErrMsg += "#==>" + g_objCommon.Get_JigErrMessage();

		} else if (m_nErrNo == 6180) {		// Barcode Shift Alarm
			strErrMsg += m_strErrSubMsg;
		}

		if (m_nErrNo == 3601 || m_nErrNo == 3602 || m_nErrNo == 3607) {
			strErrPick = g_objCommon.Get_Btm1PickerCloseChkErr();
		}

		if(m_nErrNo == 3623){
			strErrPick = g_objCommon.Get_Btm1PickerDownChkErr();
		}

		if(m_nErrNo == 3624){
			strErrPick = g_objCommon.Get_Btm1PickerOpenChkErr();
		}

		if(m_nErrNo == 3701 || m_nErrNo == 3705 || m_nErrNo == 3707 || m_nErrNo == 3708 || m_nErrNo == 3719){
			strErrPick = g_objCommon.Get_InspectVacOnChkErr(1);
		}

		if( m_nErrNo == 3801 || m_nErrNo == 3805 || m_nErrNo == 3807 || m_nErrNo == 3808 || m_nErrNo == 3819){
			strErrPick = g_objCommon.Get_InspectVacOnChkErr(2);
		}

		if( m_nErrNo == 3901 || m_nErrNo == 3905 || m_nErrNo == 3907 || m_nErrNo == 3908 || m_nErrNo == 3919){
			strErrPick = g_objCommon.Get_InspectVacOnChkErr(3);
		}

		/*if (m_nErrNo == 3621) {
			strErrPick = g_objCommon.Get_Btm1PickerVacErr();
		}*/

		if(m_nErrNo == 4004)
		{
			strErrPick = g_objCommon.Get_Btm2PickerCloseChkErr();
		}

		if (m_nErrNo == 4005 || m_nErrNo == 4007 || m_nErrNo == 4008 || m_nErrNo == 4009 || m_nErrNo == 4014) {
			strErrPick = g_objCommon.Get_Btm2PickerVacErr();
		}

		if (m_nErrNo == 4305 || m_nErrNo == 4316 || m_nErrNo == 4326 || m_nErrNo == 4344 || m_nErrNo == 4354) {
			strErrPick = g_objCommon.Get_Sort1PickerCheckErr();
		}

		if (m_nErrNo == 4405 || m_nErrNo == 4416 || m_nErrNo == 4426 || m_nErrNo == 4444 || m_nErrNo == 4454) {
			strErrPick = g_objCommon.Get_Sort2PickerCheckErr();
		}

		if (m_nErrNo == 4364 || m_nErrNo == 4464) {
			strErrPick = gData.sAPDNgItem;
		}
		if (m_nErrNo == 5007) {	// Empty Tray 수량 부족, 추가 투입 요청
			g_objSequenceMain.Set_MainRunCase(AUTO_EMPTY_TRAY_Y, 5);
		}

		if(m_nErrNo == 7000)
		{			
			strErrPick.Format("= 1-up : %d, 1-Down : %d, 2-up : %d, 2-Down : %d", gAlm.bBufferUpStatus[0], gAlm.bBufferDownStatus[0], gAlm.bBufferUpStatus[1], gAlm.bBufferDownStatus[1]);
		}

// 		if (m_nErrNo == 9001) {
// 			strErrPick.Format("#==> Start LotID(%s) 총수량(%d)#==> MES LotID(%s) 총수량(%d) Recipe(%s) Model(%s)", gData.sLotID[nNo], gData.nCmUseCount[nNo], gMes.sHostLotID, gMes.nHostCount, gMes.sHostRecipe, gMes.sHostModel);
// 		}
// 		if (m_nErrNo == 9002) {
// 			strErrPick.Format("#==> MES Lot Cancel_Code(%s) 사유(%s)", gMes.sHostCancelCode, gMes.sHostCancelText);
// 
// 			g_objMesAgent.Set_LotCancel(gData.sLotID[nNo], "H");
// 			m_btnErrToManual.SetWindowText("Retry");
// 		}

		/*if(m_nErrNo > 990 && m_nErrNo < 995) {
			if(m_nErrNo == 991) strErrPick.Format("#==> User_LotID[%s]. User_Count[%d]", g_objMES.m_sOperLotID, g_objMES.m_nOperCount);
			if(m_nErrNo == 992) strErrPick.Format("#==> MES_LotID[%s]. MES_Count[%d]. Code[%s]. Text[%s]", g_objMES.m_sMESLotID, g_objMES.m_nMESCount, g_objMES.m_sReasonCode, g_objMES.m_sReasonText);
			if(m_nErrNo == 993) strErrPick.Format("#==> User_LotID[%s]. MES_LotID[%s]", g_objMES.m_sOperLotID, g_objMES.m_sMESLotID);
			if(m_nErrNo == 994) strErrPick.Format("#==> User_Count[%d]. MES_Count[%d]", g_objMES.m_nOperCount, g_objMES.m_nMESCount);
		}*/
		m_btnErrRetry.ShowWindow(SW_HIDE);

		if(m_nErrNo == 9004)
		{
			m_btnErrRetry.ShowWindow(SW_SHOW);
			
		}
		if(m_nErrNo == 9005)
		{
			m_btnErrRetry.ShowWindow(SW_SHOW);
			strErrPick.Format(", Port1: %s, Port2: %s, Host %s,", gData.sLotID[0], gData.sLotID[1], gMes.sHostLotID);

		}
		if(m_nErrNo == 9006)
		{
			m_btnErrRetry.ShowWindow(SW_SHOW);
			strErrPick.Format(", Equip: %s, Host %s,", gData.sRecipe, gMes.sHostRecipe);
			
		}

		if(m_nErrNo == 9007)
		{
			m_btnErrRetry.ShowWindow(SW_SHOW);
			strErrPick.Format(", Port1: %s, Port2: %s, Host %s,", gData.sLotID[0], gData.sLotID[1], gMes.sHostLotID);

		}
		if(m_nErrNo == 9008)
		{
			m_btnErrRetry.ShowWindow(SW_SHOW);
			strErrPick.Format(", Equip: %s, Host %s,", gData.sRecipe, gMes.sHostRecipe);

		}

		if(m_nErrNo == 3141)
		{
			m_btnErrRetry.ShowWindow(SW_SHOW);
			strErrPick.Format(", Port1: %s, Port2: %s, Host %s,", gData.sLotID[0], gData.sLotID[1], gMes.sHostLotID);
			
			
		}
		if(m_nErrNo == 3142)
		{
			m_btnErrRetry.ShowWindow(SW_SHOW);
			strErrPick.Format(", Equip: %s, Host %s,", gData.sRecipe, gMes.sHostRecipe);
			
			
		}

		if(m_nErrNo == 9032)
		{
			m_btnErrRetry.ShowWindow(SW_SHOW);

			strErrPick.Format(", Fail ID: %s, Code: %s, Text: %s", gMes.sHostFailLotId, gMes.sHostFailCode, gMes.sHostFailText);
		
		}


		if (m_nErrNo > 2 && m_nErrNo < 9) g_objSequenceInit.Set_InitComplete(FALSE);	// 3,4,5,6

		m_strErrMsg = strErrMsg + strErrPick;

		strShow = m_strErrMsg;
		if (strShow.Left(1) == "#") strShow.Delete(0);
		strShow.Replace("#", "\n\n");
		m_stcErrMsg.SetWindowText(strShow);

		SYSTEMTIME time;
		GetLocalTime(&time);
		for (int i = 2; i > 0; i--) gData.sAlarmTime[i] = gData.sAlarmTime[i - 1];
		for (int i = 2; i > 0; i--) gData.sAlarmList[i] = gData.sAlarmList[i - 1];
		gData.sAlarmTime[0].Format("%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond);
		gData.sAlarmList[0].Format("[%s] %s", strErrNo, m_strErrMsg);

		if (m_nErrNo == 6101 || m_nErrNo == 6102 || m_nErrNo == 6103 || m_nErrNo == 6104 || m_nErrNo == 6110 ||
			m_nErrNo == 6111 || m_nErrNo == 6120 || m_nErrNo == 6121 || m_nErrNo == 6150)
		{
			//Vision에서 보내준 LotID로 설정해준다.
			strLog.Format("%s,%s,%s", gData.sErrVisionLotID, strErrNo, m_strErrMsg);
		} else {
			strLog.Format("%s,%s,%s", gData.sLotID[nNo], strErrNo, m_strErrMsg);
		}
		g_objLogFile.Save_AlarmLog(strLog);
		
		int *pCase = g_objSequenceMain.Get_pMainRunCase();
		strLog.Format("[Error Mode] ErrorNo:%s SeqNo:%d-%d-%d-%d-%d  %d-%d-%d-%d-%d  %d-%d-%d-%d-%d  %d-%d-%d-%d-%d", strErrNo,
			*(pCase+ 0), *(pCase+ 1), *(pCase+2),  *(pCase+ 3),  *(pCase+ 4),
			*(pCase+ 5), *(pCase+ 6), *(pCase+7),  *(pCase+ 8),  *(pCase+ 9),
			*(pCase+10), *(pCase+11), *(pCase+12), *(pCase+13),  *(pCase+14),
			*(pCase+15), *(pCase+16), *(pCase+17), *(pCase+18),  *(pCase+19));
		g_objLogFile.Save_HandlerLog(strLog);

		
		int nCategory = 33;	// 11:비가동중 알람, 21:경알람, 31:품질관련, 32:생산관련, 33:설비문제
		g_objMesAgent.Set_AlarmLog(m_nErrNo, m_strErrMsg, nCategory);

		Set_AlarmLog(m_nErrNo, m_strErrMsg);
		g_objDispatcher.Set_StatusUpdate(2);

		strLog.Format("[Error Mode] Error No ==> %s", strErrNo);
		g_objLogFile.Save_HandlerLog(strLog);
			
		pMainDlg->Set_LotErrorLog("ERROR", m_nErrNo, m_strErrMsg);

		int nZoneNo = (m_nErrNo - 3000) / 100;
		if (nZoneNo > 0 && nZoneNo < 21) g_objLogFile.Save_EfficiencyLog(nZoneNo, "Down", m_nErrNo, m_strErrMsg);

		m_nBackColorLoop = 0;
		if		(m_nErrNo > 3100 && m_nErrNo < 3200) m_nErrorPos =  1;	//  1.Load Tray
		else if (m_nErrNo > 3200 && m_nErrNo < 3300) m_nErrorPos =  2;	//  2.Load Picker
		else if (m_nErrNo > 3300 && m_nErrNo < 3600) m_nErrorPos =  3;	//  3.Angle Tray
		else if (m_nErrNo > 3600 && m_nErrNo < 3700) m_nErrorPos =  4;	//  4.Btm1 Picker

		else if (m_nErrNo > 3700 && m_nErrNo < 4000) m_nErrorPos =  5;	//  5.Inspect Stage
		
		else if (m_nErrNo > 4000 && m_nErrNo < 4100) m_nErrorPos =  6;	//  6.Btm2 Picker
		else if (m_nErrNo > 4100 && m_nErrNo < 4300) m_nErrorPos =  7;	//  7.Buffer Tray
		else if (m_nErrNo > 4300 && m_nErrNo < 4500) m_nErrorPos =  8;	//  8.Sort Picker
		
		else if (m_nErrNo > 4500 && m_nErrNo < 4700) m_nErrorPos =  9;	//  9.Good Tray
		else if (m_nErrNo > 4700 && m_nErrNo < 4800) m_nErrorPos = 10;	// 10.NG Tray

		else if (m_nErrNo > 4800 && m_nErrNo < 4900) m_nErrorPos = 11;	// 11.Empty X
		else if (m_nErrNo > 4900 && m_nErrNo < 5000) m_nErrorPos = 12;	// 12.Empty Z
		else if (m_nErrNo > 5000 && m_nErrNo < 5100) m_nErrorPos = 13;	// 13.Empty Y
		else										 m_nErrorPos = 0;
		m_stcErrPos[m_nErrorPos].Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0x00, 0x00));

		SetTimer(0, 100, NULL);

	} else {
		KillTimer(0);

		for (int i = 0; i < 14; i++) m_stcErrPos[i].Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));

		pMainDlg->Set_BuzzerFlicker(FALSE);
		if (g_objSequenceInit.Get_InitComplete()) pMainDlg->Set_CurrentState(STATE_STOP);
		else pMainDlg->Set_CurrentState(STATE_NONE);
		//g_objMES.Set_Alarm(2, m_nErrNo, m_strErrMsg);

		g_objLogFile.Save_HandlerLog("[Error Mode] Close Error");

		pMainDlg->Set_LotErrorLog("RESET", 905, "Error Reset");

		int nZoneNo = (m_nErrNo - 3000) / 100;
		if (nZoneNo > 0 && nZoneNo < 21) g_objLogFile.Save_EfficiencyLog(nZoneNo, "Reset", m_nErrNo, m_strErrMsg);
	}
}

void CErrorDlg::OnTimer(UINT_PTR nIDEvent) 
{
	KillTimer(0);

	DX_DATA_14 *pDX14 = g_objAJinAXL.Get_pDX14();

	if (pDX14->iResetSw) {
		OnBnClickedBtnErrOk();
		return;
	}

	if (m_nBackColorLoop == 1) {
		for (int i = 0; i < 4; i++) m_stcErrBack[i].Set_Color(COLOR_DEFAULT, RGB(0x00, 0x00, 0xFF));
	} else if (m_nBackColorLoop == 6) {
		for (int i = 0; i < 4; i++) m_stcErrBack[i].Set_Color(COLOR_DEFAULT, RGB(0xFF, 0xFF, 0x00));
	} else if (m_nBackColorLoop == 10) m_nBackColorLoop = 0;
	m_nBackColorLoop++;

	m_ledMainAir[0].Set_On(pDX14->iMainAir1);
	m_ledMainAir[1].Set_On(pDX14->iMainAir2);
	m_ledMainAir[2].Set_On(pDX14->iMainAir2);

	m_ledEmgSw[0].Set_On(pDX14->iEmgSw1);
	m_ledEmgSw[1].Set_On(pDX14->iEmgSw2);
	m_ledEmgSw[2].Set_On(pDX14->iEmgSw3);
	m_ledEmgSw[3].Set_On(pDX14->iEmgSw4);
	m_ledEmgSw[4].Set_On(pDX14->iEmgSw5);
	m_ledEmgSw[5].Set_On(pDX14->iEmgSw6);

	for (int i = 0; i < 19; i++) m_ledDoorOpen[i].Set_On((pDX14->nValue >> (i + 18)) & 1);

	SetTimer(0, 100, NULL);
	CDialogEx::OnTimer(nIDEvent);
}

void CErrorDlg::OnBnClickedBtnErrBuzzOff()
{
	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_BuzzerFlicker(FALSE);
}

void CErrorDlg::OnBnClickedBtnErrSkip()
{
// 	g_objLogFile.Save_HandlerLog("[Error Mode] SKIP button push");
// 
// 	if (m_nErrNo == 3325) {		// Inspection Not Complete
// 	}
// 
// 	g_dlgWork.Set_AutoRun(TRUE);
// 
// 	ShowWindow(SW_HIDE);
}

void CErrorDlg::OnBnClickedBtnErrRetry()
{
	g_objLogFile.Save_HandlerLog("[Error Mode] Retry button push");

	switch (m_nErrNo) 
	{
	case 9004:
		g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_TRAY, 40);
		break;

	case 3141:
		{
			//int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_LOAD_TRAY);
			g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_TRAY, 40);
		}
		break;
	case 3142:
		{
			g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_TRAY, 45);
		}
		break;
	case 9005:
		{
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_LOAD_TRAY);
			if(nCase == 41) g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_TRAY, 40);
			else if(nCase == 42) g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_TRAY, 45);
		}
		break;
	case 9006:
		{
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_LOAD_TRAY);
			g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_TRAY, 40);
			
		}
		break;
	case 9007:
		{
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_LOAD_TRAY);
			
			g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_TRAY, 45);
		}
		break;
	case 9008:
		{
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_LOAD_TRAY);
			if(nCase == 41) g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_TRAY, 40);
			else if(nCase == 42) g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_TRAY, 45);
		}
		break;
	case 9032:
		{
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_LOAD_TRAY);
			if(nCase == 41) g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_TRAY, 40);
			else if(nCase == 42) g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_TRAY, 45);
		}
		break;


	}

	ShowWindow(SW_HIDE);
	g_dlgWork.Set_AutoRun(TRUE);
}

void CErrorDlg::OnBnClickedBtnErrOk()
{
// 	if (m_nErrNo == 9002) {
// 		int nNo = gData.nLPNo-1;
// 		if (nNo < 0) nNo = 0;
// 		g_objMesAgent.Set_LotCancel(gData.sLotID[nNo], "O");
// 		m_btnErrToManual.SetWindowText("To Manual");
// 	}
	g_objLogFile.Save_HandlerLog("[Error Mode] OK button push");
	ShowWindow(SW_HIDE);
}

void CErrorDlg::OnBnClickedBtnErrSystemExit()
{
	if (g_objCommon.Show_MsgBox(2, "Do you want to exit the program?") != IDOK) return;
	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Exit_System(EXIT_SYSTEM_AJIN);
}

void CErrorDlg::OnBnClickedBtnErrToManual()
{
	int nManualPos = 0;
	if (m_nErrorPos >  0 && m_nErrorPos <=   3) nManualPos = 1;	// Load
	if (m_nErrorPos >  3 && m_nErrorPos <=   5) nManualPos = 2;	// Index
	if (m_nErrorPos >  5 && m_nErrorPos <=   8) nManualPos = 3;	// Trans
	if (m_nErrorPos >  8 && m_nErrorPos <=  13) nManualPos = 4;	// Unload

	if (nManualPos == 0 && m_nErrNo != 9002) return;

	CString strLog;
	if (m_nErrNo == 9002) {	// Lot Cancel시 Retry 기능 >> 단순 Start 시키는 구조.
		ShowWindow(SW_HIDE);
		m_btnErrToManual.SetWindowText("To Manual");

		CString strLotId, strTrayCnt, strCmCnt;

		g_dlgWork.m_stcLotId[0].GetWindowText(strLotId);
// 
		g_dlgWork.m_stcTrayCount[0].GetWindowText(strTrayCnt);
// 
		g_dlgWork.m_stcCmCount[0].GetWindowText(strCmCnt);

		strLog.Format("[Error Mode] Manual button push - LotID[%s], TrayCnt[%s], CmCnt[%s]", strLotId, strTrayCnt, strCmCnt);
		g_objLogFile.Save_MesAgentLog(strLog);
	} else {
		ShowWindow(SW_HIDE);

		g_dlgManual.Set_ManualPos(nManualPos);
		CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
		pMainDlg->Set_CurrentMode(MODE_MANUAL);

		strLog.Format("[Error Mode] Manual button push - Pos[%d]", nManualPos);
		g_objLogFile.Save_HandlerLog(strLog);
	}
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CErrorDlg::Set_AlarmLog(int nErrNo, CString sErrMsg)
{
	SYSTEMTIME time;
	int nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;
	gAlm.bBegin = TRUE;

	if (nErrNo == 6101 || nErrNo == 6102 || nErrNo == 6103 || nErrNo == 6104 || nErrNo == 6110 ||
		nErrNo == 6111 || nErrNo == 6120 || nErrNo == 6121 || nErrNo == 6150)
	{
		gAlm.sLotID = gData.sErrVisionLotID;

	} else {
		gAlm.sLotID = gData.sLotID[nNo];
	}
	gAlm.nAlmNo = nErrNo;
	gAlm.sAlmMsg = sErrMsg;
	GetLocalTime(&time);
	gAlm.dwStartTime = GetTickCount();
	gAlm.sStartTime.Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	// 작업자 기인 알람으로 설비 멈췄을때.
	// Door, Empty Tray Z축 상한/하한, MES 알람, Empty Tray Full
	if ((m_nErrNo > 10 && m_nErrNo < 30) || nErrNo == 344 || nErrNo == 444 || (m_nErrNo > 990 && m_nErrNo < 995) || nErrNo == 4926 || nErrNo == 4932) {
		g_dlgWork.MachineStopLog("ALARM");
	}
}

void CErrorDlg::Initial_Controls() 
{
	for (int i = 0; i < 4; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT);
	for (int i = 0; i < 8; i++) m_Label[i].Init_Ctrl("바탕", 20, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	for (int i = 0; i < 4; i++) m_stcErrBack[i].Set_Color(COLOR_DEFAULT, RGB(0x00, 0x00, 0xFF));

	m_stcErrTitle.Init_Ctrl("바탕", 24, TRUE, RGB(0xFF, 0xFF, 0xFF),RGB(0xFF, 0x00, 0x00));

	m_bmpImage.LoadBitmap(IDB_EQUIP_ERROR);
	m_Image.SetBitmap(m_bmpImage);
// 	m_Image.SetWindowPos(NULL, 0, 0, 1060, 600, SWP_NOZORDER | SWP_NOMOVE);	// 원본 960x540 (1680x945)
	m_Image.SetWindowPos(NULL, 0, 0, 933, 677, SWP_NOZORDER | SWP_NOMOVE);	// 533x387 : 원본 800x580 (933x677)

	for (int i = 0; i < 3; i++) m_ledMainAir[i].Init_Ctrl("바탕", 11, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em24);
	for (int i = 0; i < 6; i++) m_ledEmgSw[i].Init_Ctrl("바탕", 11, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emRed, CLedCS::em24);
	for (int i = 0; i < 19; i++) m_ledDoorOpen[i].Init_Ctrl("바탕", 11, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em24);

	for (int i = 0; i < 14; i++) m_stcErrPos[i].Init_Ctrl("바탕", 12, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));

	m_stcErrNo.Init_Ctrl("바탕", 30, TRUE, RGB(0x00, 0x00, 0xFF), RGB(0xFF, 0xF0, 0xE0));
	m_stcMsgBack.Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xE0, 0xF0, 0xF0));
	m_stcErrMsg.Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xE0, 0xF0, 0xF0));

	m_btnErrBuzzOff.Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, 0, 0);
	m_btnErrSkip.Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, 0, 0);
	m_btnErrRetry.Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, 0, 0);
	m_btnErrOK.Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, IDI_OK, CButtonCS::emLeft);
	m_btnErrSystemExit.Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, 0, 0);
	m_btnErrToManual.Init_Ctrl("바탕", 18, TRUE, RGB(0xFF, 0x00, 0xFF), COLOR_DEFAULT, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////
