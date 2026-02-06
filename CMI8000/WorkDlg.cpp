// WorkDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "WorkDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "Common.h"

#include "Inspector.h"
#include "Dispatcher.h"
#include "CapAttach.h"
#include "BarcodeLot.h"
#include "MESInterface.h"
#include "SequenceInit.h"
#include "SequenceMain.h"

#include "OperatorDlg.h"
#include "CMI8000Dlg.h"
#include "NoWorkDlg.h"

#include "LaserComm.h"
#include "SetupDlg.h"



// CWorkDlg 대화 상자입니다.
CWorkDlg g_dlgWork;

IMPLEMENT_DYNAMIC(CWorkDlg, CDialogEx)

CWorkDlg::CWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWorkDlg::IDD, pParent)
{
}

CWorkDlg::~CWorkDlg()
{
}

void CWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_GRP_LOT + i, m_grpLot[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_LBL_LOT_0 + i, m_lblLot[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_BTN_LOTID_0 + i, m_btnLotID[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_LOT_ID_0 + i, m_stcLotId[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_TRAY_COUNT_0 + i, m_stcTrayCount[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_CM_COUNT_0 + i, m_stcCmCount[i]);
	DDX_Control(pDX, IDC_IMG_EQUIPMENT, m_imgEquipment);
	DDX_Control(pDX, IDC_PIC_UPH_BACK, m_picUphBack);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LED_EQUIP_OPTION_0 + i, m_ledEquipOption[i]);

	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_STC_B1_NO_0 + i, m_stcB1No[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_STC_INSP1_NO_0 + i, m_stcInsp1No[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_STC_INSP2_NO_0 + i, m_stcInsp2No[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_STC_INSP3_NO_0 + i, m_stcInsp3No[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_STC_B2_NO_0 + i, m_stcB2No[i]);
	for (int i = 0; i <	 5; i++) DDX_Control(pDX, IDC_STC_S1_NO_0 + i, m_stcS1No[i]);
	for (int i = 0; i <	 5; i++) DDX_Control(pDX, IDC_STC_S2_NO_0 + i, m_stcS2No[i]);
	DDX_Control(pDX, IDC_STC_CYCLE_TIME, m_stcCycleTime);

	DDX_Control(pDX, IDC_RDO_WORK_START, m_rdoWorkStart);
	DDX_Control(pDX, IDC_RDO_WORK_STOP, m_rdoWorkStop);
	DDX_Control(pDX, IDC_LED_INIT_COMPLETE, m_ledInitComplete);
	DDX_Control(pDX, IDC_LED_LOT_LOAD_ENABLE, m_ledLotLoadEnable);
	DDX_Control(pDX, IDC_CHK_CYCLE_STOP, m_chkCycleStop);
	DDX_Control(pDX, IDC_CHK_MES_USE, m_chkMesUse);
	DDX_Control(pDX, IDC_CHK_PULLFORCE, m_chkPullForce);
	DDX_Control(pDX, IDC_CHK_LOTID_INSERT, m_chkLotIDInsert);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_PIC_TRAY_BACK_0 + i, m_picTrayBack[i]);

	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_ANGLE_TRAY_COUNT_0 + i, m_stcAngleTrayCount[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_BUFFER_TRAY_COUNT_0 + i, m_stcBufferTrayCount[i]);
	DDX_Control(pDX, IDC_STC_GOOD_TRAY_COUNT, m_stcGoodTrayCount);

	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_GRD_ANGLE_TRAY_0+ i, m_grdAngleTray[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_GRD_NG_TRAY_0 + i, m_grdNgTray[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_GRD_BUFFER_TRAY_0 + i , m_grdBufferTray[i]);
	DDX_Control(pDX, IDC_GRD_GOOD_TRAY, m_grdGoodTray);

	for (int i = 0; i < 20; i++) DDX_Control(pDX, IDC_STC_WORK_CASE_0 + i, m_stcWorkCase[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_TAKT_0 + i, m_stcTakt[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_UPH_0 + i, m_stcUph[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_DAY_0 + i, m_stcDay[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_LED_VISION_STATUS_0 + i, m_ledVisionStatus[i]);
	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_STC_VISION_RESULT_0 + i, m_stcVisionResult[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_LASER_DATA_0 + i, m_stcLaserResult[i]);

	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_NG_CLEAR_0 + i, m_btnNgClear[i]);
	DDX_Control(pDX, IDC_BTN_BUFFER_CHANGE, m_btnBuffChange);
	DDX_Control(pDX, IDC_BTN_BUFFER_COMPLETE, m_btnBuffComplete);
	DDX_Control(pDX, IDC_BUTTON1, m_Btn1);
	DDX_Control(pDX, IDC_BUTTON2, m_Btn2);
	DDX_Control(pDX, IDC_BUTTON3, m_Btn3);
	DDX_Control(pDX, IDC_BUTTON5, m_Btn5);
	DDX_Control(pDX, IDC_BUTTON4, m_Btn4);
	DDX_Control(pDX, IDC_BUTTON6, m_Btn6);
}

BEGIN_MESSAGE_MAP(CWorkDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LOT_ID_0, IDC_STC_LOT_ID_1, OnStcLotIdClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_CM_COUNT_0, IDC_STC_CM_COUNT_1, OnStcCmCountClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_B1_NO_0,IDC_STC_B1_NO_9, OnStcBtm1InfoClick)

	ON_BN_CLICKED(IDC_BTN_MES_ABORT, &CWorkDlg::OnBnClickedBtnMesAbort)
	ON_BN_CLICKED(IDC_BTN_MES_MANUAL, &CWorkDlg::OnBnClickedBtnMesManual)
	ON_BN_CLICKED(IDC_RDO_WORK_START, &CWorkDlg::OnBnClickedRdoWorkStart)
	ON_BN_CLICKED(IDC_RDO_WORK_STOP, &CWorkDlg::OnBnClickedRdoWorkStop)
	ON_BN_CLICKED(IDC_CHK_CYCLE_STOP, &CWorkDlg::OnBnClickedChkCycleStop)
	ON_BN_CLICKED(IDC_CHK_MES_USE, &CWorkDlg::OnBnClickedChkMesUse)
	ON_MESSAGE(UM_UPDATE_MODEL, &CWorkDlg::OnUpdateModel)
	ON_MESSAGE(UM_UPDATE_TRAY_INFO, &CWorkDlg::OnUpdateTrayInfo)
	ON_MESSAGE(UM_UPDATE_BARCODE, &CWorkDlg::OnUpdateBarcode)
	ON_MESSAGE(UM_RESET_CYCLE_STOP, &CWorkDlg::OnResetCycleStop)
	ON_MESSAGE(UM_LOT_START_END, &CWorkDlg::OnLotStartEnd)
	ON_MESSAGE(UM_UPDATE_UPH, &CWorkDlg::OnUpdateUph)
	ON_MESSAGE(UM_VISION_RESULT, &CWorkDlg::OnVisionResult)
	ON_MESSAGE(UM_LOT_INFO_CLEAR, &CWorkDlg::OnLotInfoClear)
	ON_MESSAGE(UM_MODEL_CHANGE, &CWorkDlg::OnModelChange)
	ON_MESSAGE(UM_SHOW_MSG, &CWorkDlg::OnShowMsg)
	ON_MESSAGE(UM_UPDATE_LASER, &CWorkDlg::OnUpdateLaser)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOTID_0, IDC_BTN_LOTID_1, OnBnClickedLotID)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_NG_CLEAR_0, IDC_BTN_NG_CLEAR_3, OnBnClickedNgClear)
	ON_BN_CLICKED(IDC_BTN_BUFFER_CHANGE, &CWorkDlg::OnBnClickedBtnBufferChange)
	ON_BN_CLICKED(IDC_BTN_BUFFER_COMPLETE, &CWorkDlg::OnBnClickedBtnBufferComplete)
	ON_BN_CLICKED(IDC_BTN_IDLE_REPORT, &CWorkDlg::OnBnClickedBtnIdleReport)
	ON_BN_CLICKED(IDC_BUTTON1, &CWorkDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWorkDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CWorkDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CWorkDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CWorkDlg::OnBnClickedButton5)

	
	ON_BN_CLICKED(IDC_BTN_PORTZ_INIT, &CWorkDlg::OnBnClickedBtnPortzInit)
	ON_BN_CLICKED(IDC_BTN_LASERCONNECT, &CWorkDlg::OnBnClickedBtnLaserconnect)
	ON_BN_CLICKED(IDC_BTN_LASER_GETDATA, &CWorkDlg::OnBnClickedBtnLaserGetdata)
	ON_BN_CLICKED(IDC_BUTTON6, &CWorkDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CWorkDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON9, &CWorkDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_CHK_PULLFORCE, &CWorkDlg::OnBnClickedChkPullforce)
END_MESSAGE_MAP()

// CWorkDlg 메시지 처리기입니다.

BOOL CWorkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 75, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();


#ifdef AJIN_BOARD_USE
	m_Btn1.ShowWindow(FALSE);
	m_Btn2.ShowWindow(FALSE);
	m_Btn3.ShowWindow(FALSE);
	m_Btn4.ShowWindow(FALSE);
	m_Btn5.ShowWindow(FALSE);
	m_Btn6.ShowWindow(FALSE);
#endif


	m_pWorkInfoDlg = new CWorkInfoDlg(this);
	m_pWorkInfoDlg->Create(IDD_WORK_INFO_DLG, this);

	m_bAutoRunning = FALSE;
	m_nPortIdx = 0;

	gData.bBuffTrayChange = FALSE;
	gData.bContinueLotEnd = FALSE;
	gData.bNgTrayEnd = FALSE;
	gData.bMesFirstLot = FALSE;
	gData.bLotEndBeep = FALSE;
	gData.nMesPortNo = 0;
	gData.bFirstLotStart = FALSE;

	m_btnBuffChange.ShowWindow(FALSE);
	m_btnBuffComplete.ShowWindow(FALSE);

	m_rdoWorkStop.SetCheck(TRUE);
	m_rdoWorkStop.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);

	m_toast.Create(IDD_TOASTMSG_DLG, this);
	m_toast.SetColors(RGB(20,20,120), RGB(255,220,120), RGB(120,120,120));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CWorkDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	if (pMsg->message == WM_MOUSEMOVE) {
		CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
		pMainDlg->Set_ScreenOn();
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CWorkDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	KillTimer(0);

	m_pWorkInfoDlg->DestroyWindow();
	delete m_pWorkInfoDlg;
	m_pWorkInfoDlg = NULL;
}

void CWorkDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		CString strText;
		
		m_pWorkInfoDlg->ShowWindow(SW_SHOW);

#ifndef DRY_RUN_TEST
		m_bmpEquipment.DeleteObject();
		if (pEquipData->bUseDoorLock) m_bmpEquipment.LoadBitmap(IDB_EQUIP_WORK);
		else m_bmpEquipment.LoadBitmap(IDB_EQUIP_DOOR);
		m_imgEquipment.SetBitmap(m_bmpEquipment);
#endif

		m_stcLotId[0].GetWindowText(strText);
		if (strText.GetLength() < 1) {
			if (gData.sLotID[0].GetLength() < 1) gData.sLotID[0] = "LOT_ID";
			m_stcLotId[0].SetWindowText(gData.sLotID[0]);
		}

		strText.Format("%d", gData.nTrayUseCount[0]);
		m_stcTrayCount[0].SetWindowText(strText);

		strText.Format("%d", gData.nCmUseCount[0]);
		m_stcCmCount[0].SetWindowText(strText);

		m_stcLotId[1].GetWindowText(strText);
		if (strText.GetLength() < 1) {
			if (gData.sLotID[1].GetLength() < 1) gData.sLotID[1] = "LOT_ID";
			m_stcLotId[1].SetWindowText(gData.sLotID[1]);
		}

		strText.Format("%d", gData.nTrayUseCount[1]);
		m_stcTrayCount[1].SetWindowText(strText);

		strText.Format("%d", gData.nCmUseCount[1]);
		m_stcCmCount[1].SetWindowText(strText);

		m_ledEquipOption[0].Set_On(pEquipData->bUseVisionAlign);
		m_ledEquipOption[1].Set_On(pEquipData->bUseInspectAngle);
		m_ledEquipOption[2].Set_On(pEquipData->bUseInspectBtm1Specular);
		m_ledEquipOption[3].Set_On(pEquipData->bUseInspectTop1);
		m_ledEquipOption[4].Set_On(pEquipData->bUseInspectTop2);
		m_ledEquipOption[5].Set_On(pEquipData->bUseInspectBtm2);
		m_ledEquipOption[6].Set_On(pEquipData->bUseDispatcher);
		m_ledEquipOption[7].Set_On(pEquipData->bUseMES);
		m_ledEquipOption[8].Set_On(pEquipData->bUseApdAlarm);
		m_ledEquipOption[9].Set_On(pEquipData->bUseInlineMode);

		m_chkMesUse.SetCheck(pEquipData->bUseMES);

		m_grpLot[0].SetWindowText("Port 1 Lot Info");
		m_grpLot[1].SetWindowText("Port 2 Lot Info");
		for (int i = 0; i < 2; i++) m_stcLotId[i].Set_Color(COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));
		for (int i = 0; i < 2; i++) m_stcCmCount[i].Set_Color(COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));

		g_objCommon.Locking_MainDoor(FALSE);

		m_btnBuffComplete.EnableWindow(FALSE);
		SetTimer(0, 100, NULL);
	} else {
		KillTimer(0);
		m_pWorkInfoDlg->ShowWindow(SW_HIDE);
	}
}

void CWorkDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(0);

	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	DX_DATA_14 *pDX14 = g_objAJinAXL.Get_pDX14();
	if (pDX14->iStartSw && !m_rdoWorkStart.GetCheck()) {
		int nNo = 0;
		if (gData.nLPNo > 0 || gData.nULPNo > 0) {
			if (gData.nULPNo > 0) nNo = gData.nULPNo;
			if (gData.nLPNo > 0)  nNo = gData.nLPNo;

		} else { nNo = 1;}
<<<<<<< HEAD

=======
>>>>>>> Release_MEM25/V3.5.1.20
		g_objMES.Set_Status(1);
		g_objLogFile.Save_HandlerLog("[Work Mode] START S/W push");
		m_rdoWorkStart.SetCheck(TRUE);
		pMainDlg->Set_LotErrorLog("START", 903, "Start");
		g_objLogFile.Save_EfficiencyLog(1, "Run", 903, "Run Start");
		SetTimer(0, 100, NULL); return;

	} else if (pDX14->iStopSw && !m_rdoWorkStop.GetCheck()) {
		int nNo = 0;
		if (gData.nLPNo > 0 || gData.nULPNo > 0) {
			if (gData.nULPNo > 0) nNo = gData.nULPNo;
			if (gData.nLPNo > 0)  nNo = gData.nLPNo;

		} else { nNo = 1;}
<<<<<<< HEAD

=======
>>>>>>> Release_MEM25/V3.5.1.20
		g_objMES.Set_Status(2);
		g_objLogFile.Save_HandlerLog("[Work Mode] STOP S/W push");
		MachineStopLog("STOP_BUTTON_PUSH");
		m_rdoWorkStop.SetCheck(TRUE);
		pMainDlg->Set_LotErrorLog("STOP", 904, "Stop");
		g_objLogFile.Save_EfficiencyLog(1, "Stop", 904, "Stop Button Push");
		SetTimer(0, 100, NULL); return;
	}

	if (pDX14->iResetSw) {
		g_objCommon.Show_Alarm("", FALSE);
	}

	Check_Lamp();	// Load1/2, NG, Good Port Lamp Switch
	Display_Status();

	if (m_rdoWorkStart.GetCheck()) {

		if (!g_objCommon.Check_ServoOn()){
			g_objCommon.Show_MsgBox(1, "서보 off 상태 입니다.");
			return;
		}
		if (!g_objCommon.Check_DirveAlarm()){
			g_objCommon.Show_MsgBox(1, "Drive 알람 상태 입니다.");
			return;
		}
		if (!g_objCommon.Check_EndLimit()){
			g_objCommon.Show_MsgBox(1, "limit sensor check 상태입니다.");
			return;
		}
			
		if (!g_objCommon.Check_HomeDone()) {
			g_objCommon.Show_MsgBox(1, "Home이 안잡혀 있습니다.");
			return;
		}


		if (!m_bAutoRunning) {		// First AutoRun
			if (!Work_Start()) { SetTimer(0, 100, NULL); m_rdoWorkStop.SetCheck(TRUE); m_rdoWorkStart.SetCheck(FALSE); return; }

			if (g_objSequenceInit.Get_InitComplete()) 
			{

				m_bAutoRunning = TRUE;
				BOOL bLoad1 = g_objSequenceMain.Get_LotLoadEnable(0);
				BOOL bLoad2 = g_objSequenceMain.Get_LotLoadEnable(1);
				if (!bLoad1)				g_objCommon.Locking_PortSlide(TRUE, 1);
				if (!bLoad2)				g_objCommon.Locking_PortSlide(TRUE, 2);
				if (!gData.bGoodTrayWait)	g_objCommon.Locking_PortSlide(TRUE, 4);
				if (!gData.bEmptyZWait)		g_objCommon.Locking_PortSlide(TRUE, 5);

				if (!gData.bNGTrayWait) {	// 시퀀스 번호 13번이면 Lock 걸어주면 안된다.
					int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_NG_TRAY);
					if (nCase == 13) g_objCommon.Locking_PortSlide(FALSE, 3);
					else			 g_objCommon.Locking_PortSlide(TRUE, 3);
				}
					
				if(pEquipData->bUseDoorLock) g_objCommon.Locking_MainDoor(TRUE, TRUE);
				pMainDlg->Enable_ModeButton(FALSE);
				if (gAlm.bBegin) Reset_AlarmLog();
				pMainDlg->Set_CurrentState(STATE_RUN);

				// 처음 시작할때만 MES 체크 해준다.
// 				if (!g_objMES.m_bMESUse || g_objMES.m_nMESSequence == 3 || (!gData.bMesFirstLot && g_objMES.m_bMesErr) ) {	// 연속 랏 중 MES 알람 났을때
				if (!g_objMES.m_bMESUse || g_objMES.m_nMESSequence == 3) {
// 					if (gData.bMesFirstLot) gData.bMesRegistered[0] = TRUE;
					g_objLogFile.Save_HandlerLog("[Work Mode] Main Thread Start");
					g_objSequenceMain.Begin_MainRunThread();
					g_objMES.Set_Status(1);
				} else if (g_objMES.m_nMESSequence == 4) {
					g_objCommon.Show_MsgBox(1, "MES 등록이 정상적으로 되지 않았습니다. 수동 착공해주시길 바랍니다.");
					pMainDlg->Enable_ModeButton(TRUE);
					SetTimer(0, 100, NULL); m_rdoWorkStop.SetCheck(TRUE); return;
				} else if (g_objMES.m_nMESSequence == 0 && !gData.bMesFirstLot) {
					gData.bMesFirstLot = TRUE;
					g_objMES.Clear_APDResultVar();	// 모듈 결과 APD 관련 변수 초기화.
					g_objMES.m_bMesStart = TRUE;
					g_objMES.Set_JobReady(gData.sLotID[0], gData.nCmUseCount[0], gData.sOperID, 1); //무조건 1번 포트 먼저 
					CString strLog;
					strLog.Format("[Work Timer] Set_JobReady. (LotID:%s, CmCnt:%d, Port:0)", gData.sLotID[0], gData.nCmUseCount[0]);
					g_objLogFile.Save_MesAgentLog(strLog);
				}
				pMainDlg->Set_EquipRunStart();
				MachineStopLog("RUN_START");
				
				g_objCommon.Btm1BlowSunctionOnOff(TRUE);
				g_objCommon.TopBlowSunctionOnOff(TRUE);
				

			} else {
				g_objCommon.Show_Error(40);		// 초기화 완료 에러
			}

		} 
		else 
		{				// Auto Running
			// MES 통신 완료일 때 장비 구동하기 위해...
			if (g_objMES.m_bMESUse && g_objMES.m_nMESSequence == 2) {
				g_objMES.m_nMESSequence = 3;
				if (!g_objSequenceMain.Is_MainThreadRun()) {
// 					if (gData.bMesFirstLot) gData.bMesRegistered[0] = TRUE;
					g_objLogFile.Save_HandlerLog("[Work Mode] Main Thread Start");
					g_objSequenceMain.Begin_MainRunThread();
					g_objMES.Set_Status(1);
				}
			}

			if (!g_objMES.m_bMESUse || g_objMES.m_nMESSequence == 3) {
				if (!g_objSequenceMain.Is_MainThreadRun()) {
					g_objLogFile.Save_HandlerLog("[Work Mode] Auto STOP");
					pMainDlg->Set_CurrentState(STATE_STOP);
					g_objMES.Set_Status(2);

					g_objCommon.Btm1BlowSunctionOnOff(FALSE);
					g_objCommon.TopBlowSunctionOnOff(FALSE);
				}
			}
			if(pEquipData->bUseDoorLock) g_objCommon.Locking_MainDoor(TRUE, TRUE);
			pMainDlg->Enable_ModeButton(FALSE);
			g_objCommon.Check_PortArea(TRUE);
		}

	} 
	else if (m_rdoWorkStop.GetCheck()) // First AutoStop
	{
		if (m_bAutoRunning) 
		{	
			m_bAutoRunning = FALSE;

			g_objLogFile.Save_HandlerLog("[Work Mode] Main Thread End");
			g_objSequenceMain.End_MainRunThread();

			g_objLogFile.Save_HandlerLog("[Work Mode] Main Thread End Complete");
			g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z);		// 강제 STOP
			g_objAJinAXL.Stop_Motion(AX_EMPTY_TRANS1_X);	// 강제 STOP
			// g_objMES.m_bMesStart = FALSE; // MES 정보를 읽는 Thread가 멈추지 않도록 해야 한다.

			int nState = theApp.Get_MainState();
			if (nState != STATE_ALARM && nState != STATE_ERROR) pMainDlg->Set_CurrentState(STATE_STOP);

			m_rdoWorkStart.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
			m_rdoWorkStop.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);

			pMainDlg->Enable_ModeButton(TRUE);
			g_objCommon.Locking_MainDoor(FALSE);
			g_objCommon.Locking_PortSlide(FALSE);

			pMainDlg->Save_EquipRunTime();
			g_objCommon.Save_MotionPos();

			g_objCommon.Btm1BlowSunctionOnOff(FALSE);
			g_objCommon.TopBlowSunctionOnOff(FALSE);
			

		} else {				// Stop
			int nState = theApp.Get_MainState();
			if (nState != STATE_ERROR) g_objCommon.Check_MainEmgAir();
		}
	}

	SetTimer(0, 100, NULL);
	CDialogEx::OnTimer(nIDEvent);
}

void CWorkDlg::OnBnClickedLotID(UINT nID)
{
	int nIndex = nID - IDC_BTN_LOTID_0;
	m_nPortIdx = nIndex;
}

void CWorkDlg::OnBnClickedNgClear(UINT nID)
{
	int nIndex = nID - IDC_BTN_NG_CLEAR_0;
	CString strMsg;

	if (nIndex == 0)
	{
		strMsg.Format("N1&&N2&&N3 Tray Clear 하시겠습니까?");

	}
	if (nIndex == 1) strMsg.Format("N4 Tray Clear 하시겠습니까?");
	//if (nIndex == 2) strMsg.Format("N1 && N3 Tray Clear 하시겠습니까?");
	//if (nIndex == 3) strMsg.Format("N2 Tray Clear 하시겠습니까?");

	if (g_objCommon.Show_MsgBox(2, strMsg) == IDOK) {
		g_objSequenceMain.Init_NgTray(nIndex);
		if (nIndex == 0)
		{
			gData.nTrayCntNG[0]++;
			gData.nTrayCntNG[2]++;
			g_objSequenceMain.Init_NgTray(2);	// NG 1 Clear 할 때 NG3도 Clear 한다.
		}
		if (nIndex == 1)
		{
			gData.nTrayCntNG[1]++;
			gData.nTrayCntNG[3]++;
			g_objSequenceMain.Init_NgTray(3);	// NG 1 Clear 할 때 NG3도 Clear 한다.
		}
	}	
}

void CWorkDlg::OnStcLotIdClick(UINT nID)
{
	int ID = nID - IDC_STC_LOT_ID_0;
	CString strKey;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;

	m_stcLotId[ID].SetWindowText(strKey);
	if (ID == 1) g_dlgOperator.m_stcOperLotId.SetWindowText(strKey);

	CString strLog;
	strLog.Format("[Work Dialog] Lot ID (%d) Click. (LotID:%s)", ID, strKey);
	g_objLogFile.Save_MesAgentLog(strLog);
}

void CWorkDlg::OnStcCmCountClick(UINT nID)
{
	if(!m_chkMesUse.GetCheck())
	{
		int ID = nID  - IDC_STC_CM_COUNT_0;

		CString strOld, strNew, strValue;

		m_stcCmCount[ID].GetWindowText(strOld);
		if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

		int nCmCnt = atoi(strNew);

		int nTrayUseCount = nCmCnt / gData.nCmMaxCount;
		if (nCmCnt % gData.nCmMaxCount) nTrayUseCount++;

		strValue.Format("%d", nTrayUseCount);
		m_stcTrayCount[ID].SetWindowText(strValue);
		if (ID == 1) g_dlgOperator.m_stcOperTrayCount.SetWindowText(strValue);

		strValue.Format("%d", nCmCnt);
		m_stcCmCount[ID].SetWindowText(strValue);
		if (ID == 1) g_dlgOperator.m_stcOperCmCount.SetWindowText(strValue);

		CString strLog;
		strLog.Format("[Work Dialog] CM Count (%d) Click. (CmCount:%d)", ID, nCmCnt);
		g_objLogFile.Save_MesAgentLog(strLog);
	}	
}

void CWorkDlg::OnStcBtm1InfoClick(UINT nID)
{
	int ID = nID - IDC_STC_B1_NO_0;

	if(g_objCommon.Show_MsgBox(2, "해당 모듈 Dummy 입니까?") != IDOK) return;
	
	//나중에 NG로 빠지게 하면 될듯 지금 그렇게 해줄 이유가 없음 
	//gData.nCNoBtm1Pick[ID] = 0; //gData.nTNoBtm1Pick[ID] = 0;
	//gData.InfoBtm1Pick[ID] = gData.nCNoBtm1Pick[ID];

	
	gData.nPNoMESNG[gLot.nMatchingNgCount[gData.nPNoBtm1Pick-1][0]] = gData.nPNoBtm1Pick;
	gData.nTNoMESNG[gLot.nMatchingNgCount[gData.nPNoBtm1Pick-1][0]] = gData.nTNoBtm1Pick[ID];
	gData.nCmNoMESNG[gLot.nMatchingNgCount[gData.nPNoBtm1Pick-1][0]] = gData.nCNoBtm1Pick[ID];
	gLot.nMatchingNgCount[gData.nPNoBtm1Pick-1][0]++;

	//m_stcB1No[ID].Init_Ctrl("바탕", 12, TRUE, RGB(0xFF, 0x00, 0x00), RGB(0x0F, 0x0F, 0x0F));
	m_stcB1No[ID].Set_Color(RGB(0xFF, 0x00, 0x00), RGB(0x0F, 0x0F, 0x0F));
	//m_stcB1No[ID].Set_Color(RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));

	CString strLog;

	strLog.Format("MES NG, Clicked, PortNo(%d), TrayNo(%d), CmNo(%d)", gData.nPNoBtm1Pick, gData.nTNoBtm1Pick[ID], gData.nCNoBtm1Pick[ID]);
	g_objLogFile.Save_HandlerLog(strLog);

	//CString strText;
	//strText.Format("%d-%d", gData.nTNoBtm1Pick[ID], gData.nCNoBtm1Pick[ID]); m_stcB1No[ID].Set_Text(strText);

}

void CWorkDlg::OnBnClickedChkMesUse()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (m_rdoWorkStart.GetCheck()) {
		g_objCommon.Show_MsgBox(1, "Can't change in Auto Run.");
		m_chkMesUse.SetCheck(pEquipData->bUseMES);

	} else {
		CIniFileCS INI(gData.sEnvPath + "\\EquipData.ini");
		if (!INI.Check_File()) { AfxMessageBox("EquipData.ini File Not Found!!!"); return; }

		INI.Set_Bool("OPTION", "MES_USE", m_chkMesUse.GetCheck());
		g_objDataManager.Read_EquipData();
		m_ledEquipOption[7].Set_On(pEquipData->bUseMES);
		g_objMES.Set_MESUse(pEquipData->bUseMES);
// 		if (pEquipData->bUseMES) gData.bMesFirstLot = FALSE;
	}
	CString strLog;
	strLog.Format("[Work Dialog] Check MES Use Click (%d).", (int)pEquipData->bUseMES);
	g_objLogFile.Save_MesAgentLog(strLog);
}

void CWorkDlg::OnBnClickedBtnMesAbort()
{
	if (m_rdoWorkStart.GetCheck()) { g_objCommon.Show_MsgBox(1, "You can't cancel the Lot during Run........."); return; }

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseMES) { g_objCommon.Show_MsgBox(1, "You can't cancel the MES No_Use........."); return; }

	g_objMES.Set_LotCancel(gData.sLotID[m_nPortIdx], gData.nCmUseCount[m_nPortIdx], gData.sOperID);
// 	gData.bMesRegistered[m_nPortIdx] = FALSE;

	CString strLog;
	strLog.Format("Lot Cancel OK...  => Lot[%s] 취소를 MES에 전송하였습니다.", gData.sLotID[m_nPortIdx]);
	AfxMessageBox(strLog);

	strLog.Format("[Work Dialog] MES Cancel Button Click. (LotID:%s, CmCnt:%d)", gData.sLotID[m_nPortIdx], gData.nCmUseCount[m_nPortIdx]);
	g_objLogFile.Save_MesAgentLog(strLog);
}

void CWorkDlg::OnBnClickedBtnMesManual()
{
	if (m_rdoWorkStart.GetCheck()) { g_objCommon.Show_MsgBox(1, "장비 가동 중에는 등록 할수 없습니다........."); return; }

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseMES) { g_objCommon.Show_MsgBox(1, "MES를 사용하지 않고 있습니다........."); return; }

	CString strLog;
// 	if (gData.bMesRegistered[m_nPortIdx]) {
// 		strLog.Format("Port[%d]번 이미 MES 연결 하였습니다.\n다시 연결하려면 MES Cancel 버튼을 누른후 다시 시도하십시오.", m_nPortIdx+1);
// 		g_objCommon.Show_MsgBox(1, strLog);
// 		return;
// 	}

	if (m_bMesManual) { g_objCommon.Show_MsgBox(1, "MES 연결중 입니다........."); return; }
	m_bMesManual = TRUE;

	Get_LotInfo(m_nPortIdx+1);
	g_objMES.m_bMesStart = TRUE;
	g_objMES.Set_JobReady(gData.sLotID[m_nPortIdx], gData.nCmUseCount[m_nPortIdx], gData.sOperID, m_nPortIdx+1);

	DWORD dwStart = GetTickCount();
	while(1) {	
// 		if (GetTickCount() - dwStart > 20000) { AfxMessageBox("[Time Over] MES 연결 실패 하였습니다. 확인하여주십시오."); break; }
		if (GetTickCount() - dwStart > 20000) { AfxMessageBox("[Time Over] MES 연결 실패 하였습니다. MES Cancel 버튼을 누른후 다시 시도하십시오."); break; }
		if (g_objMES.m_nMESSequence == 2 || g_objMES.m_nMESSequence == 4) break;
		theApp.DoEvents();
	}

	if (g_objMES.m_nMESSequence == 2) {
		strLog.Format("Lot[%s] MES 수동 착공 완료 하였습니다.", gData.sLotID[m_nPortIdx]);
// 		gData.bMesRegistered[m_nPortIdx] = TRUE;
		AfxMessageBox(strLog);
		g_objMES.m_nMESSequence = 3;	// Run
	} else if (g_objMES.m_nMESSequence == 4) {
		AfxMessageBox("[MES Error] MES 연결 실패 하였습니다. MES Cancel 버튼을 누른후 다시 시도하십시오.");
	}

	m_bMesManual = FALSE;
	strLog.Format("[Work Dialog] MES Manual Button Click. (LotID:%s, CmCnt:%d)", gData.sLotID[m_nPortIdx], gData.nCmUseCount[m_nPortIdx]);
	g_objLogFile.Save_MesAgentLog(strLog);
}

void CWorkDlg::OnBnClickedRdoWorkStart()
{
	g_objLogFile.Save_HandlerLog("[Work Mode] START button push");
	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_LotErrorLog("START", 903, "Start");
	g_objLogFile.Save_EfficiencyLog(1, "Run", 903, "Run Start");
}

void CWorkDlg::OnBnClickedRdoWorkStop()
{
	g_objMES.Set_Status(2);
	g_objLogFile.Save_HandlerLog("[Work Mode] STOP button push");
	MachineStopLog("STOP_BUTTON_PUSH");
	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_LotErrorLog("STOP", 904, "Stop");
	g_objLogFile.Save_EfficiencyLog(1, "Stop", 903, "Stop Button Push");
}

void CWorkDlg::OnBnClickedChkCycleStop()
{
	if (m_chkCycleStop.GetCheck()) {
		m_chkCycleStop.Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0xFF));
		if (g_objCommon.Show_MsgBox(2, "Are you sure you want to Cycle Stop?") == IDOK) {
			gData.bCycleStop = TRUE;
		} else {
			m_chkCycleStop.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xF0, 0xF0));
			m_chkCycleStop.SetCheck(FALSE);
		}
	} else {
		m_chkCycleStop.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xF0, 0xF0));
		gData.bCycleStop = FALSE;
	}
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

BOOL CWorkDlg::Work_Start()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	CString strTemp, strTemp2;

	int nState = theApp.Get_MainState();
	if (nState == STATE_ERROR || nState == STATE_ALARM) return FALSE;

	if (!g_objCommon.Check_MainDoor()) return FALSE;

	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09();
	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();

	if (gData.sOperID.GetLength() < 5) {
		m_rdoWorkStop.SetCheck(TRUE);
		g_objCommon.Show_MsgBox(1, "Please Check Operator ID.......");
		return FALSE;
	}

	// Down Time Report 화면 출력시 진행 안함.
	if (g_dlgNoWork.IsWindowVisible()) {
		g_objCommon.Show_MsgBox(1, "Please Close Down Time Report Window."); return FALSE; 
	}


#ifndef AJIN_BOARD_USE
	DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();
	pDY09->oBufferStage1Vacuum = pDX09->iBufferStage1Vacuum = TRUE;
	pDY09->oBufferStage2Vacuum = pDX09->iBufferStage2Vacuum = TRUE;	
#endif 

	if (m_bMesManual) { g_objCommon.Show_MsgBox(1, "MES 수동 착공 중입니다. 완료후 진행해 주십시오."); return FALSE; }
	if (gData.bBuffTrayChange) { g_objCommon.Show_MsgBox(1, "Buffer Tray 교체 작업 진행해 주십시오."); return FALSE; }

	if (!pDX09->iBufferStage1Vacuum) {g_objCommon.Show_MsgBox(1, "Check Buffer1 Tray Vacuum On."); return FALSE; }
	if (!pDX09->iBufferStage2Vacuum) {g_objCommon.Show_MsgBox(1, "Check Buffer2 Tray Vacuum On."); return FALSE; }

	if (!pDX00->iLoadPort1SlideClose) { g_objCommon.Show_MsgBox(1, "Load Port 1번 Slide Close 센서가 감지 되지 않습니다. Slide를 끝까지 밀어주십시오."); return FALSE; }
	if (!pDX00->iLoadPort2SlideClose) { g_objCommon.Show_MsgBox(1, "Load Port 2번 Slide Close 센서가 감지 되지 않습니다. Slide를 끝까지 밀어주십시오."); return FALSE; }
	if (!pDX09->iNgPortSlideClose)	  { g_objCommon.Show_MsgBox(1, "NG Port Slide Close 센서가 감지 되지 않습니다. Slide를 끝까지 밀어주십시오."); return FALSE; }
	if (!pDX12->iGoodPortSlideClose)  { g_objCommon.Show_MsgBox(1, "Good Port Slide Close 센서가 감지 되지 않습니다. Slide를 끝까지 밀어주십시오."); return FALSE; }
#ifdef EDITION_2ND
	if (!pDX01->iEmptyPortSlideClose) { g_objCommon.Show_MsgBox(1, "Empty Tray Port Slide Close 센서가 감지 되지 않습니다. Slide를 끝까지 밀어주십시오."); return FALSE; }
#else
	if (!pDX07->iEmptyPortSlideClose) { g_objCommon.Show_MsgBox(1, "Empty Tray Port Slide Close 센서가 감지 되지 않습니다. Slide를 끝까지 밀어주십시오."); return FALSE; }
#endif

	int nMotionNo = g_objCommon.Check_MotionPos();
	if (nMotionNo < 99) {
		double dCurrentPos = g_objAJinAXL.Get_Position(nMotionNo);
		CString strName = g_objAJinAXL.Get_AxisName(nMotionNo);
		strTemp.Format("Motion(%s) 위치를 Check 하세요.\n이전위치(%0.3lf) != 현재위치(%0.3lf)", strName, gAlm.dMotionPos[nMotionNo], dCurrentPos);
		g_objLogFile.Save_HandlerLog(strTemp);

		g_objCommon.Show_MsgBox(1, strTemp);
		m_rdoWorkStop.SetCheck(TRUE);
		return FALSE;
	}

	//buffer stage 이전 상태 기억 및 체크 

	else if((gAlm.bBufferUpStatus[0] != pDX09->iBufferStage1Up) || (gAlm.bBufferUpStatus[1] != pDX09->iBufferStage2Up)
		|| (gAlm.bBufferDownStatus[0] != pDX09->iBufferStage1Down) || (gAlm.bBufferDownStatus[1] != pDX09->iBufferStage2Down))
	{
		strTemp.Format("Buffer Stage 상태 체크 하세요 1Up :%d, 1Down:%d, 2Up:%d, 2Down:%d", gAlm.bBufferUpStatus[0],gAlm.bBufferDownStatus[0],gAlm.bBufferUpStatus[1],gAlm.bBufferDownStatus[1] );
		g_objLogFile.Save_HandlerLog(strTemp);

		g_objCommon.Show_MsgBox(1, strTemp);

	}



	g_objInspector.Set_FOBRequest(INSPECTOR_ALL);	// 시작전 FOB 모드 확인

#ifndef DRY_RUN_TEST
	if (!pEquipData->bUseInspectAngle || !pEquipData->bUseInspectBtm1Specular || !pEquipData->bUseInspectBtm1Angle || !pEquipData->bUseInspectBtm13D
		|| !pEquipData->bUseInspectTop1 || !pEquipData->bUseInspectTop2 || !pEquipData->bUseInspectBtm2) {

		if (!pEquipData->bResultTestUse) {
			if (g_objCommon.Show_MsgBox(2, "Vision Option을 끄고 진행하시겠습니까?") != IDOK) return FALSE;
		}

		if (gData.bCycleStop) {
			if (!pEquipData->bUseInspectBtm1Specular || !pEquipData->bUseInspectBtm1Angle || !pEquipData->bUseInspectBtm13D) g_objInspector.Set_CycleStop(INSPECTOR_PC1);
			if (!pEquipData->bUseInspectTop1 || !pEquipData->bUseInspectAngle) g_objInspector.Set_CycleStop(INSPECTOR_PC2);
			if (!pEquipData->bUseInspectTop2) g_objInspector.Set_CycleStop(INSPECTOR_PC3);
			if (!pEquipData->bUseInspectBtm2) g_objInspector.Set_CycleStop(INSPECTOR_PC4);
		}
	}

	int nTimeOut = 0;
		while(gData.nVisionFOBMode == -1) {
			nTimeOut++;
			if (nTimeOut > 30) break;	//Time Out 3초
			g_objCommon.uSleep(100);
		}
		if (gData.nVisionFOBMode == 1) {
			if (g_objCommon.Show_MsgBox(2,"Vision FOB Mode 상태 입니다. 진행 하시겠습니까?") != IDOK) {
				m_rdoWorkStop.SetCheck(TRUE);
				return FALSE;
			} else {
				// FOB 모드일때 시작하면 MES 꺼주고 시작한다.
				CIniFileCS INI(gData.sEnvPath + "\\EquipData.ini");
				if (!INI.Check_File()) {
					AfxMessageBox("EquipData.ini File Not Found!!!");
					return FALSE;
				}
				INI.Set_Bool("OPTION", "MES_USE", FALSE);
				g_objDataManager.Read_EquipData();
				g_objMES.Set_MESUse(FALSE);				
				m_chkMesUse.SetCheck(FALSE);
				m_ledEquipOption[7].Set_On(FALSE);
			}
		}

		if (!pEquipData->bUseMES) {
			if (g_objCommon.Show_MsgBox(2, "MES Option을 끄고 진행하시겠습니까?") != IDOK) return FALSE;
		}
#endif

#ifndef AJIN_BOARD_USE
		m_stcCmCount[0].GetWindowText(strTemp);	// CM 수량
		int nTemp = atoi(strTemp);
		if (nTemp > 1 || nTemp < 2400) pDX00->iLoadPort1Bottom = TRUE;

		m_stcCmCount[1].GetWindowText(strTemp2);	// CM 수량
		int nTemp2 = atoi(strTemp2);
		gData.nCmUseCount[1] = nTemp2;
		if (nTemp2 > 1 || nTemp2 < 2400) pDX00->iLoadPort2Bottom = TRUE;
#endif 

	if (g_objSequenceMain.Get_IsAutoRun()) return TRUE;	// Auto Run이면 스킵

	int nLoadTray = g_objSequenceMain.Get_MainRunCase(AUTO_LOAD_TRAY);
	if (nLoadTray == 0 && !pDX00->iLoadPort1Bottom && !pDX00->iLoadPort2Bottom && gData.nLoadTrayCount[0] == 0) {
		g_objCommon.Show_MsgBox(1, "Please Check the Load Tray."); return FALSE;
	}

	// 모든 Lot이 종료되고 다시 시작할때 Good Port에 있는 Tray 제거해줘 Cap Attach Tray Picker Z축 Servo Off 알람이 나지 않게 해줘야한다.
	if (pDX12->iGoodPortBottom) {
		g_objCommon.Show_MsgBox(1, "Good Port에 있는 Tray를 제거해 주십시오."); return FALSE;
	}

	g_objSequenceMain.Set_ClearRunData(1);	// 시점 변경 (LotEnd->LotStart)
	

	gData.bFirstLotStart = TRUE;

	m_stcLotId[0].GetWindowText(strTemp);		// Lot ID
	if (strTemp.GetLength() < 2) { g_objCommon.Show_MsgBox(1, "Port1번에 LOT ID를 입력하여 주십시오."); return FALSE; }
	gData.sLotID[0] = strTemp;

	if(!m_chkMesUse.GetCheck())
	{
		m_stcCmCount[0].GetWindowText(strTemp);	// CM 수량
		int nTempCnt = atoi(strTemp);
		if (nTempCnt < 1 || nTempCnt > 2400) {	g_objCommon.Show_MsgBox(1, "Port1번의 CM 수량을 확인하여 주십시오. (1 ~ 2400)"); return FALSE; }
		gData.nCmUseCount[0] = nTempCnt;

		m_stcTrayCount[0].GetWindowText(strTemp);	// Tray Count
		gData.nTrayUseCount[0] = atoi(strTemp);
	}

	m_stcLotId[1].GetWindowText(strTemp2);		// Lot ID
	gData.sLotID[1] = strTemp2;

	m_stcCmCount[1].GetWindowText(strTemp2);	// CM 수량
	int nTempCnt2 = atoi(strTemp2);
	gData.nCmUseCount[1] = nTempCnt2;

	m_stcTrayCount[1].GetWindowText(strTemp2);	// Tray Count
	gData.nTrayUseCount[1] = atoi(strTemp2);

	if (strTemp2.GetLength() > 0 && nTempCnt2 > 0) { Enable_UserInput(2, TRUE); }
	else { gData.bLoadLampOn[1] = TRUE; g_objSequenceMain.Set_LotLoadEnable(TRUE, 1); }

	return TRUE;
}

void CWorkDlg::Initial_Controls() 
{
	CString strText;
	for (int i = 0; i < 6; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 7; i++) m_Label[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x80, 0x80, 0x80));		// Tray
	for (int i = 0; i < 2; i++) m_grpLot[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 3; i++) m_lblLot[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x20, 0x20, 0x80));		// Lot Run
	for (int i = 3; i < 6; i++) m_lblLot[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x00, 0x80));		// Lot Standby
	for (int i = 0; i < 2; i++) m_stcLotId[i].Init_Ctrl("Arial", 8, TRUE, COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));
	for (int i = 0; i < 2; i++) m_stcTrayCount[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	for (int i = 0; i < 2; i++) m_stcCmCount[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));
	m_bmpEquipment.LoadBitmap(IDB_EQUIP_WORK);
	m_imgEquipment.SetBitmap(m_bmpEquipment);
	m_picUphBack.Init_Ctrl(COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 10; i++) m_ledEquipOption[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emBlue, CLedCS::em16);
	for (int i = 0; i < 10; i++) m_stcB1No[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 10; i++) m_stcB2No[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 10; i++) m_stcInsp1No[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 10; i++) m_stcInsp2No[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 10; i++) m_stcInsp3No[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i <  5; i++) m_stcS1No[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i <  5; i++) m_stcS2No[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
	m_stcCycleTime.Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
	m_rdoWorkStart.Init_Ctrl("바탕", 20, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoWorkStop.Init_Ctrl("바탕", 20, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_ledInitComplete.Init_Ctrl("바탕", 9, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	m_ledLotLoadEnable.Init_Ctrl("바탕", 9, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	m_chkCycleStop.Init_Ctrl("바탕", 9, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_chkMesUse.Init_Ctrl("Arial", 10, TRUE, RGB(0xFF, 0xFF, 0x00), RGB(0xC0, 0x10, 0x30), CCheckCS::emRed, CCheckCS::emRight);
	m_chkPullForce.Init_Ctrl("Arial", 10, TRUE, RGB(0xFF, 0xFF, 0x00), RGB(0xC0, 0x10, 0x30), CCheckCS::emRed, CCheckCS::emRight);
	m_chkLotIDInsert.Init_Ctrl("Arial", 10, TRUE, RGB(0xFF, 0xFF, 0x00), RGB(0x30, 0x70, 0xFF), CCheckCS::emRed, CCheckCS::emRight);
	for (int i = 0; i < 6; i++) m_picTrayBack[i].Set_Color(COLOR_DEFAULT, RGB(0xF0, 0xF0, 0x80));
	for (int i = 0; i < 2; i++) m_stcAngleTrayCount[i].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 2; i++) m_stcBufferTrayCount[i].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	m_stcGoodTrayCount.Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 2; i++)Initial_Grid(&m_grdAngleTray[i], gData.nTrayY, gData.nTrayX, 0);
	for (int i = 0; i < 4; i++)Initial_Grid(&m_grdNgTray[i], gData.nTrayY, gData.nTrayX);
	for (int i = 0; i < 2; i++)Initial_Grid(&m_grdBufferTray[i], gData.nTrayY, gData.nTrayX);
	Initial_Grid(&m_grdGoodTray, gData.nTrayY, gData.nTrayX);
	for (int i = 0; i < 20; i++) m_stcWorkCase[i].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 4; i++) m_stcTakt[i].Init_Ctrl("Arial", 9, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 4; i++) m_stcUph[i].Init_Ctrl("Arial", 9, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 2; i++) m_stcDay[i].Init_Ctrl("Arial", 9, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 7; i++) m_ledVisionStatus[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 16; i++) m_stcVisionResult[i].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 2; i++) m_stcLaserResult[i].Init_Ctrl("바탕", 9, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 2; i++) m_btnLotID[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF),  RGB(0x40, 0x00, 0x80), 0, 0);
	for (int i = 0; i < 4; i++) m_btnNgClear[i].Init_Ctrl("Arial", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_btnBuffChange.Init_Ctrl("Arial", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_btnBuffComplete.Init_Ctrl("Arial", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
}

void CWorkDlg::Initial_Grid(CGridCS *pGrid, int nRows, int nCols, int nType)
{
	pGrid->Set_RowCount(nRows);
	pGrid->Set_ColCount(nCols);
	pGrid->Set_FixRowCount(0);
	pGrid->Set_FixColCount(0);

	long lTotalW = pGrid->Get_GridWidth();
	long lTotalH = pGrid->Get_GridHeight();
	long lCellW = lTotalW / nCols;
	long lCellH = lTotalH / nRows;

	CString str;
	if (nType == 0) {
		for (int i = 0; i < nRows; i++) {
			pGrid->Set_RowHeight(i, lCellH);

			for (int j = 0; j < nCols; j++) {
				if (i == 0) pGrid->Set_ColWidth(j, lCellW);

				pGrid->Set_CellAlign(i, j, CELL_ALIGN_CENTER);
				str.Format(_T("%d"), (nRows - 1 - i) * nCols + j + 1);
				pGrid->Set_CellText(i, j, str);
			}
		}
	} else {
		for (int i = 0; i < nRows; i++) {
			pGrid->Set_RowHeight(i, lCellH);

			for (int j = 0; j < nCols; j++) {
				if (i == 0) pGrid->Set_ColWidth(j, lCellW);

				pGrid->Set_CellAlign(i, j, CELL_ALIGN_CENTER);
				str.Format(_T("%d"), i * nCols + j + 1);
				pGrid->Set_CellText(i, j, str);
			}
		}
	}
}

void CWorkDlg::Check_Lamp()
{
	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00(); DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01(); DY_DATA_01 *pDY01 = g_objAJinAXL.Get_pDY01();
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07(); DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09(); DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();
	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12(); DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();
	DX_DATA_14 *pDX14 = g_objAJinAXL.Get_pDX14(); DY_DATA_14 *pDY14 = g_objAJinAXL.Get_pDY14();
	DX_DATA_15 *pDX15 = g_objAJinAXL.Get_pDX15(); DY_DATA_15 *pDY15 = g_objAJinAXL.Get_pDY15();

	BOOL bLoad1 = g_objSequenceMain.Get_LotLoadEnable(0);
	BOOL bLoad2 = g_objSequenceMain.Get_LotLoadEnable(1);

	if (g_objSequenceMain.Get_IsAutoRun()) {	// m_rdoWorkStart.GetCheck()
		// Load Port1
		if (pDX14->iLoad1Sw && bLoad1) {
			if (pDX00->iLoadPort1SlideClose && (!pEquipData->bUseDoorLock || !pDX15->iDoor02Unlock)) {	// 안전 확인.
				
				CString sTemp;				
				m_stcLotId[0].GetWindowText(sTemp);		// Lot ID				
				// Port1 CM 수량 확인 및 LotID 확인
				if (gLot.sLotID[0] != sTemp && gLot.sLotID[1] != sTemp) 
				{
					
					gData.bLoadLampOn[0] = FALSE;
					pMainDlg->Set_LampFlicker_Load1(FALSE);
					g_objSequenceMain.Set_LotLoadEnable(FALSE, 0);
					pDY00->oLoadPort1SlideLock = TRUE; pDY00->oLoadPort1SlideUnlock = FALSE;
					g_objAJinAXL.Write_Output(0);
					if (pEquipData->bUseDoorLock) pDY15->oDoor02Unlock = FALSE; 
					g_objAJinAXL.Write_Output(15);
				} 
				else 
				{
					#ifndef AJIN_BOARD_USE
						pDX14->iLoad1Sw = FALSE;
					#endif
					g_objCommon.Show_MsgBox(1, "Port1 Lot 정보를 확인해 주십시오.");
				}
			}
		}
		// Load Port2
		if (pDX14->iLoad2Sw && bLoad2) {
			if (pDX00->iLoadPort2SlideClose && (!pEquipData->bUseDoorLock || !pDX15->iDoor03Unlock)) {	// 안전 확인.
				CString sTemp;
				
				m_stcLotId[1].GetWindowText(sTemp);		// Lot ID
				
				// Port1 CM 수량 확인 및 LotID 확인
				if ( gLot.sLotID[0] != sTemp && gLot.sLotID[1] != sTemp) 				
				{										
					gData.bLoadLampOn[1] = FALSE;
					pMainDlg->Set_LampFlicker_Load2(FALSE);
					g_objSequenceMain.Set_LotLoadEnable(FALSE, 1);
					pDY00->oLoadPort2SlideLock = TRUE; pDY00->oLoadPort2SlideUnlock = FALSE;
					g_objAJinAXL.Write_Output(0);
					if (pEquipData->bUseDoorLock) pDY15->oDoor03Unlock = FALSE; 
					g_objAJinAXL.Write_Output(15);
				} 
				else 
				{
					#ifndef AJIN_BOARD_USE
						pDX14->iLoad2Sw = FALSE;
					#endif
					g_objCommon.Show_MsgBox(1, "Port2 Lot 정보를 확인해 주십시오.");
				}
			}
		}
		// NG Tray
		if (pDX14->iNgSw && gData.bNGTrayWait) {
			if (pDX09->iNgPortSlideClose && (!pEquipData->bUseDoorLock || !pDX15->iDoor04Unlock)) {	// 안전 확인.
				pMainDlg->Set_LampFlicker_Ng(FALSE);
				pDY09->oNgPortSlideLock = TRUE; pDY09->oNgPortSlideUnlock = FALSE;
				g_objAJinAXL.Write_Output(9);
				if (pEquipData->bUseDoorLock) pDY15->oDoor04Unlock = FALSE; 
				g_objAJinAXL.Write_Output(15);

				if (gData.bContinueLotEnd) 
				{
					gData.bContinueLotEnd = FALSE;
					for (int i = 0; i < 4; i++) g_objSequenceMain.Init_NgTray(i);
				} 
				else 
				{
					if (gData.InfoNgTray[0][gData.nTrayY-1][gData.nTrayX-1] > 0) //N123
					{ 
						gData.nTrayCntNG[0]++;
						gData.nTrayCntNG[2]++;
						g_objSequenceMain.Init_NgTray(0); g_objSequenceMain.Init_NgTray(2); 
					}	
					if (gData.InfoNgTray[1][gData.nTrayY-1][gData.nTrayX-1] > 0) // N4
					{ 
						gData.nTrayCntNG[1]++;
						gData.nTrayCntNG[3]++;
						g_objSequenceMain.Init_NgTray(1); g_objSequenceMain.Init_NgTray(3); 
					}	
					//if (gData.InfoNgTray[3][gData.nTrayY-1][gData.nTrayX-1] > 0) 
				}
				gData.bNGTrayWait = FALSE;

				// NG Tray Start
				g_objSequenceMain.Set_MainRunLoop(AUTO_NG_TRAY, 5000);
				int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_NG_TRAY);
				if (nCase == 0) g_objSequenceMain.Set_MainRunCase(AUTO_NG_TRAY, 1);
			}
		}
		// Good Tray
		if (pDX14->iGoodSw && gData.bGoodTrayWait) {
			if (pDX12->iGoodPortSlideClose && (!pEquipData->bUseDoorLock || !pDX15->iDoor05Unlock)) {	// 안전 확인.
				pMainDlg->Set_LampFlicker_Good(FALSE);
				pDY12->oGoodPortSlideLock = TRUE; pDY12->oGoodPortSlideUnlock = FALSE;
				g_objAJinAXL.Write_Output(12);
				if (pEquipData->bUseDoorLock) pDY15->oDoor05Unlock = FALSE; 
				g_objAJinAXL.Write_Output(15);
				gData.bGoodTrayWait = FALSE;
				gData.bGoodTrayLotEnd[0] = FALSE;
				gData.bGoodTrayLotEnd[1] = FALSE;
				//gData.nGoodTrayCount = 0;
			}
		}
		// Empty Port
		if (pDX14->iEmptySw && gData.bEmptyZWait) {
#ifdef EDITION_2ND
			if (pDX01->iEmptyPortSlideClose && (!pEquipData->bUseDoorLock || !pDX15->iDoor10Unlock)) {	// 안전 확인.
#else
			if (pDX07->iEmptyPortSlideClose && (!pEquipData->bUseDoorLock || !pDX15->iDoor10Unlock)) {	// 안전 확인.
#endif
				pMainDlg->Set_LampFlicker_Empty(FALSE);
				if (pEquipData->bUseDoorLock) pDY15->oDoor10Unlock = FALSE;
				g_objAJinAXL.Write_Output(15);
				gData.bEmptyZWait = FALSE;
			}
		}
	}
}

void CWorkDlg::Display_Status()
{
	CString strTemp, strText;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

// 	if (g_objMesAgent.Is_Connected()) { m_stcMesConnect.Set_Text("Connected"); m_stcMesConnect.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0x00, 0xFF, 0x00)); }
// 	else { m_stcMesConnect.Set_Text("Disconnected"); m_stcMesConnect.Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00)); }
// 
// 	if (g_objMesAgent.Is_HostOnline()) { m_stcMesOnline.Set_Text("Online"); m_stcMesOnline.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0x00, 0xFF, 0x00)); }
// 	else { m_stcMesOnline.Set_Text("Offline"); m_stcMesOnline.Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00)); }

	BOOL bInitComplete = g_objSequenceInit.Get_InitComplete();
	m_ledInitComplete.Set_On(bInitComplete);
	
	BOOL bLotLoadEnable = g_objSequenceMain.Get_LotLoadEnable(0);
	m_ledLotLoadEnable.Set_On(bLotLoadEnable);

	m_grpLot[0].Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_grpLot[1].Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_grpLot[m_nPortIdx].Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);

	for (int i = 0; i < 10; i++) { strText.Format("%d-%d", gData.nTNoBtm1Pick[i], gData.nCNoBtm1Pick[i]); m_stcB1No[i].Set_Text(strText); }
	for (int i = 0; i < 10; i++) { strText.Format("%d-%d", gData.nTNoBtm2Pick[i], gData.nCNoBtm2Pick[i]); m_stcB2No[i].Set_Text(strText); }
	for (int i = 0; i < 10; i++) { strText.Format("%d-%d", gData.nTNoInspect[0][i], gData.nCNoInspect[0][i]); m_stcInsp1No[i].Set_Text(strText); }
	for (int i = 0; i < 10; i++) { strText.Format("%d-%d", gData.nTNoInspect[1][i], gData.nCNoInspect[1][i]); m_stcInsp2No[i].Set_Text(strText); }
	for (int i = 0; i < 10; i++) { strText.Format("%d-%d", gData.nTNoInspect[2][i], gData.nCNoInspect[2][i]); m_stcInsp3No[i].Set_Text(strText); }
	for (int i = 0; i <  5; i++) { strText.Format("%d-%d", gData.nTNoSortPick[0][i], gData.nCNoSortPick[0][i]); m_stcS1No[i].Set_Text(strText); }
	for (int i = 0; i <  5; i++) { strText.Format("%d-%d", gData.nTNoSortPick[1][i], gData.nCNoSortPick[1][i]); m_stcS2No[i].Set_Text(strText); }

	m_ledVisionStatus[0].Set_On(g_objInspector.Get_VisionStatus(INSPECTOR_PC1));
	m_ledVisionStatus[1].Set_On(g_objInspector.Get_VisionStatus(INSPECTOR_PC2));
	m_ledVisionStatus[2].Set_On(g_objInspector.Get_VisionStatus(INSPECTOR_PC3));
	m_ledVisionStatus[3].Set_On(g_objInspector.Get_VisionStatus(INSPECTOR_PC4));
	m_ledVisionStatus[4].Set_On(pEquipData->bUseDispatcher && g_objDispatcher.Is_Connected());
	m_ledVisionStatus[5].Set_On(pEquipData->bUseInlineMode && g_objCapAttach.Is_Opened());
	m_ledVisionStatus[6].Set_On(g_objLaserComm.getConnection());

	int *pCase = g_objSequenceMain.Get_pMainRunCase();
	for (int i = 0; i < AUTO_COUNT; i++) { strText.Format("%02d", *(pCase + i)); m_stcWorkCase[i].Set_Text(strText); }
}

void CWorkDlg::ResetInfoDisplay()
{
	for (int i = 0; i < 10; i++) m_stcB1No[i].Set_Color(RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
}


void CWorkDlg::Reset_AlarmLog()
{
	CString strLog, strErrNo;
	SYSTEMTIME time;

	gAlm.bBegin = FALSE;
	GetLocalTime(&time);

	gAlm.dwEndTime = GetTickCount();
	gAlm.sEndTime.Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	gAlm.dwProcTime = gAlm.dwEndTime - gAlm.dwStartTime;
	
	gLot.dwErrorTime[PORT1] += gAlm.dwProcTime; 
	gLot.dwErrorTime[PORT2] += gAlm.dwProcTime;
		
	gLot.nErrorCount[PORT1]++;
	gLot.nErrorCount[PORT2]++;

	strLog.Format("%s,%04d,%s,%s,%s,%d", gAlm.sLotID, gAlm.nAlmNo, gAlm.sAlmMsg, gAlm.sStartTime, gAlm.sEndTime, gAlm.dwProcTime);
	g_objLogFile.Save_AlarmResetLog(strLog);	// Alarm Reset

	strErrNo.Format("%04d", gAlm.nAlmNo);
//	g_objMesAgent.Set_ErrorUpdate(0, strErrNo);

	g_objLogFile.Save_ECMLog(1, strLog);
}

void CWorkDlg::MachineStopLog(CString sType)
{
	CString strLog, strErrNo;
	SYSTEMTIME time;

	GetLocalTime(&time);

	if (sType == "LOT_END" || sType == "STOP_BUTTON_PUSH" || sType == "CAP_ATTACH_STOP" || sType == "ALARM" || sType == "PROGRAM_BEGIN") {
		if (gData.dwMStopStartTime > 1) {
			gData.dwMStopEndTime = GetTickCount();
			gData.dwMStopProcTime = gData.dwMStopEndTime - gData.dwMStopStartTime;

			gData.dwMStopStartTime = GetTickCount();
			if (sType == "ALARM")	strLog.Format("%s: %04d(%s),%d", sType, gAlm.nAlmNo, gAlm.sAlmMsg, gData.dwMStopProcTime);
			else					strLog.Format("%s,%d", sType, gData.dwMStopProcTime);		

		} else {
			gData.dwMStopStartTime = GetTickCount();
			if (sType == "ALARM")	strLog.Format("%s: %04d(%s)", sType, gAlm.nAlmNo, gAlm.sAlmMsg);
			else					strLog.Format("%s", sType);
		}
	} else if (sType == "RUN_START" || sType == "PROGRAM_EXIT") {
		gData.dwMStopEndTime = GetTickCount();
		if (gData.dwMStopStartTime < 1) gData.dwMStopProcTime = 0;
		else							gData.dwMStopProcTime = gData.dwMStopEndTime - gData.dwMStopStartTime;

		strLog.Format("%s,%d", sType, gData.dwMStopProcTime);

		gData.dwMStopStartTime = 0;
	}
	
	g_objLogFile.Save_MachineStopLog(strLog);
}

void CWorkDlg::Set_AutoRun(BOOL bAutoRun)
{
	if (bAutoRun) m_rdoWorkStart.SetCheck(TRUE);
	else m_rdoWorkStop.SetCheck(TRUE);
}

void CWorkDlg::Set_State(int nState)
{
	switch (nState) {
	case STATE_NONE:
	case STATE_INIT:
	case STATE_STOP:	// Stop
		m_rdoWorkStart.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
		m_rdoWorkStop.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
		m_rdoWorkStop.SetCheck(TRUE);
		m_chkCycleStop.EnableWindow(TRUE);
		break;
	case STATE_RUN:		// Start
		m_rdoWorkStart.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
		m_rdoWorkStop.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
		m_chkCycleStop.EnableWindow(FALSE);
		break;
	case STATE_ALARM:
	case STATE_ERROR:	// Error
		m_rdoWorkStart.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
		m_rdoWorkStop.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
		m_rdoWorkStop.SetCheck(TRUE);
		break;
	}
}

void CWorkDlg::Enable_UserInput(int nNo, BOOL bEnable)
{
	if (nNo < 1 || nNo > 2) return;

	m_stcLotId[nNo-1].EnableWindow(bEnable);
	m_stcTrayCount[nNo-1].EnableWindow(bEnable);
	m_stcCmCount[nNo-1].EnableWindow(bEnable);
}

void CWorkDlg::Get_LotInfo(int nNo)
{
	if (nNo < 1 || nNo > 2) return;

	CString strTemp;

	m_stcLotId[nNo-1].GetWindowText(strTemp);		// Lot ID
	gData.sLotID[nNo-1] = strTemp;

	m_stcCmCount[nNo-1].GetWindowText(strTemp);	// CM 수량
	gData.nCmUseCount[nNo-1] = atoi(strTemp);

	m_stcTrayCount[nNo-1].GetWindowText(strTemp);	// Tray Count
	gData.nTrayUseCount[nNo-1] = atoi(strTemp);
}

///////////////////////////////////////////////////////////////////////////////

void CWorkDlg::OnBnClickedBtnBufferChange()
{
	if (!g_objSequenceInit.Get_InitComplete()) {
		g_objCommon.Show_MsgBox(1, "초기화 진행 먼저 해주십시오");
		return;
	}

	if (g_objSequenceMain.Get_IsAutoRun()) {
		g_objCommon.Show_MsgBox(1, "자동검사 중에는 교체 할 수 없습니다.");
		return;
	}

	int nBuffTray1Case = g_objSequenceMain.Get_MainRunCase(AUTO_BUFFER1);
	int nBuffTray2Case = g_objSequenceMain.Get_MainRunCase(AUTO_BUFFER2);
	if (nBuffTray1Case != 0 && nBuffTray2Case != 50) {
		g_objCommon.Show_MsgBox(1, "Buffer Stage 시퀀스 번호가 맞지 않습니다. 초기화 진행 후 작업 해주십시오");
		return;
	}

	DWORD dwStart;
	CWaitCursor wait;

	gData.bBuffTrayChange = TRUE;	// Change 버튼만 누르고 장비 시작하면 안되기 때문에 TRUE로 바꿔준다.
	DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09(); DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();

	if ((pDX09->iBufferStage1Up && !pDX09->iBufferStage1Down && pDX09->iBufferStage2Up && !pDX09->iBufferStage2Down) ||	// All Up
		(!pDX09->iBufferStage1Up && pDX09->iBufferStage1Down && !pDX09->iBufferStage2Up && pDX09->iBufferStage2Down)) {	// All Down
			pDY09->oBufferStage1Up = TRUE; pDY09->oBufferStage1Down = FALSE;	// Stage1 Up
			pDY09->oBufferStage2Up = FALSE; pDY09->oBufferStage2Down = TRUE;	// Stage2 Down
			g_objAJinAXL.Write_Output(9);
	}

	dwStart = GetTickCount();
	while (TRUE) {
		if ((pDX09->iBufferStage1Up && !pDX09->iBufferStage1Down && !pDX09->iBufferStage2Up && pDX09->iBufferStage2Down)) { break; }
		if (GetTickCount() - dwStart > 10000) { AfxMessageBox("Stage Up/Down Time Over."); wait.Restore(); return; }
	}

	g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 4);
	g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 4);

	dwStart = GetTickCount();
	while (TRUE) {
		if (g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 4) && g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 4)) { break; }
		if (GetTickCount() - dwStart > 10000) { AfxMessageBox("Stage Move Time Over."); wait.Restore(); return; }
	}

	pDY09->oBufferStage1Vacuum = FALSE; 
	pDY09->oBufferStage2Vacuum = FALSE;
	g_objAJinAXL.Write_Output(9);

	wait.Restore();

	g_objCommon.Show_MsgBox(1, "Buffer Stage Change Position Move Complete.");
	m_btnBuffComplete.EnableWindow(TRUE);
	
}


void CWorkDlg::OnBnClickedBtnBufferComplete()
{
	DWORD dwStart;
	CWaitCursor wait;
	DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09(); DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();

	pDY09->oBufferStage1Vacuum = TRUE; 
	pDY09->oBufferStage2Vacuum = TRUE;
	g_objAJinAXL.Write_Output(9);

	dwStart = GetTickCount();
	while (TRUE) {
		if ((pDX09->iBufferStage1Vacuum && pDX09->iBufferStage2Vacuum)) { break; }
		if (GetTickCount() - dwStart > 10000) { AfxMessageBox("Stage Vacuum Time Over."); wait.Restore(); return;}
	}

	if (!pDX09->iBufferStage1Up && !pDX09->iBufferStage2Down) {	AfxMessageBox("Stage Up/Down 상태 확인해 주십시오."); wait.Restore(); return; }
	
	g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 0);
	g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 3);

	dwStart = GetTickCount();
	while (TRUE) {
		if (g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 3)) { break; }
		if (GetTickCount() - dwStart > 10000) { AfxMessageBox("Stage Move Time Over."); wait.Restore(); return; }
	}

	gData.bBuffTrayChange = FALSE;
	wait.Restore();

	g_objCommon.Show_MsgBox(1, "Buffer Stage Work Position Move Complete.");
	m_btnBuffComplete.EnableWindow(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// Message

LRESULT CWorkDlg::OnUpdateModel(WPARAM wParam, LPARAM lParam)
{
	CString strText;
	for (int i = 0; i < 2; i++) { strText.Format("%d", gData.nTrayUseCount[i]); m_stcTrayCount[i].SetWindowText(strText); }
	for (int i = 0; i < 2; i++) { strText.Format("%d", gData.nCmUseCount[i]); m_stcCmCount[i].SetWindowText(strText); }
	return 0;
}

LRESULT CWorkDlg::OnUpdateTrayInfo(WPARAM nTray, LPARAM lParam)
{
	CString strText;
	int nIdx = (int)lParam;

	if (nTray == 0 || nTray == 1) {		// Angle Port
		strText.Format("%d", gData.nTNoAnglePort[0]);
		m_stcAngleTrayCount[0].SetWindowText(strText);

		for (int i = 0; i < gData.nTrayY; i++) {
			for (int j = 0; j < gData.nTrayX; j++) {
				if		(gData.InfoAnglePortTray[i][j] == 9) m_grdAngleTray[0].Set_CellBackClr(i, j, RGB(0x00, 0xFF, 0xFF));	// Reserve
				else if (gData.InfoAnglePortTray[i][j] == 0) m_grdAngleTray[0].Set_CellBackClr(i, j, RGB(0xFF, 0xFF, 0xFF));	// Empty
				else										 m_grdAngleTray[0].Set_CellBackClr(i, j, RGB(0x80, 0x80, 0x80));	// Error
			}
		}
		g_dlgOperator.Update_TrayInfo(nTray);
	}

	if (nTray == 0 || nTray == 2) {		// Angle Tray
		strText.Format("%d", gData.nTNoAngleTray[nIdx]);
		m_stcAngleTrayCount[1].SetWindowText(strText);

		for (int i = 0; i < gData.nTrayY; i++) {
			for (int j = 0; j < gData.nTrayX; j++) {
				if		(gData.InfoAngleTray[nIdx][i][j] == 9) m_grdAngleTray[1].Set_CellBackClr(i, j, RGB(0x00, 0xFF, 0xFF));	// Reserve
				else if (gData.InfoAngleTray[nIdx][i][j] == 0) m_grdAngleTray[1].Set_CellBackClr(i, j, RGB(0xFF, 0xFF, 0xFF));	// Empty
				else									       m_grdAngleTray[1].Set_CellBackClr(i, j, RGB(0x80, 0x80, 0x80));	// Error
			}
		}
		g_dlgOperator.Update_TrayInfo(nTray, nIdx);
	}

	if (nTray == 0 || nTray == 3) {		// Buffer Tray
		strText.Format("%d", gData.nTNoBuffTray[nIdx][0][0]);
		m_stcBufferTrayCount[nIdx].SetWindowText(strText);
		
		for (int s = 0; s < 2; s++) {
			for (int i = 0; i < gData.nTrayY; i++) {
				for (int j = 0; j < gData.nTrayX; j++) {
					if		(gData.InfoBuffTray[s][i][j] == 1) m_grdBufferTray[s].Set_CellBackClr(i, j, RGB(0x00, 0xFF, 0x00));	// Good
					else if (gData.InfoBuffTray[s][i][j] == 0) m_grdBufferTray[s].Set_CellBackClr(i, j, RGB(0xFF, 0xFF, 0xFF));	// Empty
					else									   m_grdBufferTray[s].Set_CellBackClr(i, j, RGB(0x80, 0x80, 0x80));	// Error
				}
			}
		}
		g_dlgOperator.Update_TrayInfo(nTray);
	}

	if (nTray == 0 || nTray == 4) {		// Good Tray
		strText.Format("%d", gData.nGoodTrayCount);
		m_stcGoodTrayCount.SetWindowText(strText);

		for (int i = 0; i < gData.nTrayY; i++) {
			for (int j = 0; j < gData.nTrayX; j++) {
				if		(gData.InfoGoodTray[i][j] == 1) m_grdGoodTray.Set_CellBackClr(i, j, RGB(0x00, 0xFF, 0x00));	// Good
				else if (gData.InfoGoodTray[i][j] == 0) m_grdGoodTray.Set_CellBackClr(i, j, RGB(0xFF, 0xFF, 0xFF));	// Empty
				else								    m_grdGoodTray.Set_CellBackClr(i, j, RGB(0x80, 0x80, 0x80));	// Error
			}
		}
		g_dlgOperator.Update_TrayInfo(nTray);
	}

	if (nTray == 0 || nTray == 5) {		// NG Tray
		//m_stcGoodTrayCount.SetWindowText(strText);
		for (int s = 0; s < 4; s++) {
			for (int i = 0; i < gData.nTrayY; i++) {
				for (int j = 0; j < gData.nTrayX; j++) {
					if		(gData.InfoNgTray[s][i][j] == 2)  m_grdNgTray[s].Set_CellBackClr(i, j, RGB(0xFF, 0x00, 0x00));	// Normal
					else if	(gData.InfoNgTray[s][i][j] == 4)  m_grdNgTray[s].Set_CellBackClr(i, j, RGB(0xF0, 0xF0, 0x00));	// N1
					else if (gData.InfoNgTray[s][i][j] == 5)  m_grdNgTray[s].Set_CellBackClr(i, j, RGB(0xFF, 0x60, 0xFF));	// N2
					else if (gData.InfoNgTray[s][i][j] == 6)  m_grdNgTray[s].Set_CellBackClr(i, j, RGB(0xFF, 0x80, 0x00));	// N3
					else if (gData.InfoNgTray[s][i][j] == 7)  m_grdNgTray[s].Set_CellBackClr(i, j, RGB(0xFF, 0xE0, 0x00));	// B
					else if (gData.InfoNgTray[s][i][j] == 8)  m_grdNgTray[s].Set_CellBackClr(i, j, RGB(0xFF, 0xD0, 0xFF));	// N4
					else if (gData.InfoNgTray[s][i][j] == 0)  m_grdNgTray[s].Set_CellBackClr(i, j, RGB(0xFF, 0xFF, 0xFF));	// Empty
					else									  m_grdNgTray[s].Set_CellBackClr(i, j, RGB(0x80, 0x80, 0x80));	// Error
				}
			}
		}
		g_dlgOperator.Update_TrayInfo(nTray);
	}

	return 0;
}

LRESULT CWorkDlg::OnUpdateBarcode(WPARAM wParam, LPARAM lParam)
{
	CString strTemp, sBarcode, strLog;
	CString sData = g_objBarcodeLot.Get_BarcodeLot(); 
	if (sData.GetLength() < 1) return 0;
	/*
	sData.Replace("\r","");
	sData.Replace("\n","");

	char chSep = '/';
	CString strPartNumber, strModuleCount, strLotID;

	AfxExtractSubString(strPartNumber, sData, 0, chSep);
	AfxExtractSubString(strModuleCount, sData, 1, chSep);
	AfxExtractSubString(strLotID, sData, 2, chSep);

	//'/' 구분 바코드 자릿수 체크 
	if(strPartNumber.GetLength() < 10) { // 바코드 part number 자릿수 10 이하 체크 
		g_objCommon.Show_MsgBox(1, "바코드 part number 자릿수를 체크해 주십시오\n");
		return 0;
	}
	if(strModuleCount.GetLength() < 3) { // 모듈 수량 자릿수 3 이하 체크 
		g_objCommon.Show_MsgBox(1, "바코드 모듈 수량 자릿수를 체크해 주십시오\n %s");
		return 0;
	}
	if(strLotID.GetLength() < 12) { // 모듈 수량 자릿수 3 이하 체크 
		g_objCommon.Show_MsgBox(1, "바코드 Lot ID 자릿수를 체크해 주십시오\n");
		return 0;
	}

	BOOL ret = g_objLogFile.Check_BarcodeLog(sData);
	if(!ret) g_objCommon.Show_MsgBox(1, "2주 내에 같은 바코드를 사용하였습니다.\n 주의 바랍니다.");
	*/
	if (!m_stcLotId[m_nPortIdx].IsWindowEnabled()) return 0;

	if (m_chkLotIDInsert.GetCheck()) sBarcode.Format("%sT1", sData);
	else sBarcode.Format("%s", sData);

	m_stcLotId[m_nPortIdx].GetWindowText(strTemp);
	if (strTemp == sBarcode) return 0;

	m_stcLotId[m_nPortIdx].SetWindowText(sBarcode);
	g_dlgOperator.m_stcOperLotId.SetWindowText(sBarcode);

	strLog.Format("[Work Dialog] OnUpdateBarcode. (PortNo:%d, Barcode:%s)", m_nPortIdx+1, sBarcode);
	g_objLogFile.Save_MesAgentLog(strLog);

	//2주내 중복 바코드 체크 위한 저장 
	//g_objLogFile.Save_BarcodeChkLog(sBarcode);

	return 0;
}

LRESULT CWorkDlg::OnUpdateLaser(WPARAM wParam, LPARAM lParam)
{
	m_stcLaserResult[0].SetWindowText(gData.sLaserData1);
	m_stcLaserResult[1].SetWindowText(gData.sLaserData2);

	double dLaser1 = atof(gData.sLaserData1);
	double dLaser2 = atof(gData.sLaserData2);

	double diff = abs(gData.sLaserData1 - gData.sLaserData2);
	if(diff > 2 ) g_objCommon.Show_MsgBox(1, "Stage 높이 편차가 2mm 이상입니다.");

	return 0;
}

LRESULT CWorkDlg::OnResetCycleStop(WPARAM wParam, LPARAM lParam)
{
	gData.bCycleStop = FALSE;
	m_chkCycleStop.SetCheck(FALSE);
	m_chkCycleStop.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xF0, 0xF0));
	//if (wParam == 1) g_objSequenceInit.Set_InitComplete(FALSE);	// 초기화 필요

	return 0;
}

LRESULT CWorkDlg::OnUpdateUph(WPARAM wParam, LPARAM lParam)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	int nHour = time.wHour;
	double	dDispTakt[4] = { 0.0 };
	double	dCycleTime = 0.0;
	CString strText;

	if (g_objSequenceMain.m_dwULCycleTime != 0) {
//		dCycleTime = g_objSequenceMain.m_dwULCycleTime / (double)(gData.nTrayX * gData.nTrayY) / 1000.0;
		dCycleTime = g_objSequenceMain.m_dwULCycleTime / (double)(8.0) / 1000.0;
	}
	if (dCycleTime == 0.0) strText = " Cycle Time :";
	else strText.Format(" Cycle Time : %0.5f", dCycleTime);
	m_stcCycleTime.SetWindowText(strText);
	strText.Format("LotID,%s,Cycle Time,%0.6lf", gLot.sLotID[gData.nULPNo-1], dCycleTime);
	g_objLogFile.Save_HandlerLog(strText);
	if (wParam == 1) return 0;

	dDispTakt[0] = gUph.dTaktTime;		// 현재 Lot

	int nCount = gUph.nLotCount[nHour];
	for (int i = 0; i < nCount; i++) dDispTakt[1] += gUph.dTakt[nHour][i];
	dDispTakt[1] /= nCount;				// 1시간

	int nTotalCount = 0;
	if (nHour >= 7 && nHour < 19) {
		for (int i = 7; i < 19; i++) {
			nCount = gUph.nLotCount[i];
			for (int j = 0; j < nCount; j++) dDispTakt[2] += gUph.dTakt[i][j];
			nTotalCount += nCount;
		}
	} else {
		for (int i = 0; i < 7; i++) {
			nCount = gUph.nLotCount[i];
			for (int j = 0; j < nCount; j++) dDispTakt[2] += gUph.dTakt[i][j];
			nTotalCount += nCount;
		}

		for (int i = 19; i < 24; i++) {
			nCount = gUph.nLotCount[i];
			for (int j = 0; j < nCount; j++) dDispTakt[2] += gUph.dTakt[i][j];
			nTotalCount += nCount;
		}
	}
	dDispTakt[2] /= nTotalCount;		// 12시간

	nTotalCount = 0;
	for (int i = 0; i < 24; i++) {
		nCount = gUph.nLotCount[i];
		for (int j = 0; j < nCount; j++) dDispTakt[3] += gUph.dTakt[i][j];
		nTotalCount += nCount;
	}
	dDispTakt[3] /= nTotalCount;		// 1일

	// Takt & UPH
	for (int i = 0; i < 4; i++) {
		if (dDispTakt[i] == 0) strText = "";
		else strText.Format("%0.5f", dDispTakt[i]);
		m_stcTakt[i].SetWindowText(strText);

		if (dDispTakt[i] == 0) strText = "";
		else strText.Format("%d", int(3600 / dDispTakt[i]));
		m_stcUph[i].SetWindowText(strText);
	}

	//생산량
	for (int i = 0; i < 2; i++) {
		if (gUph.nCmCount[i] == 0) strText = "";
		else strText.Format("%d", gUph.nCmCount[i]);
		m_stcDay[i].SetWindowText(strText);
	}

	return 0;
}

LRESULT CWorkDlg::OnVisionResult(WPARAM wParam, LPARAM lParam)
{
	CString strText;
	if (wParam == 1 || wParam == 0) {
		int nTotal = gLot.nGoodCount[0] + gLot.nNgCount[0];
		strText.Format("%d", nTotal);				m_stcVisionResult[1].SetWindowText(strText);
		strText.Format("%d", gLot.nGoodCount[0]);	m_stcVisionResult[2].SetWindowText(strText);
		strText.Format("%d", gLot.nSNgCount[0][1]);	m_stcVisionResult[3].SetWindowText(strText);
		strText.Format("%d", gLot.nSNgCount[0][2]);	m_stcVisionResult[4].SetWindowText(strText);
		strText.Format("%d", gLot.nSNgCount[0][3]);	m_stcVisionResult[5].SetWindowText(strText);
		strText.Format("%d", gLot.nSNgCount[0][5]);	m_stcVisionResult[6].SetWindowText(strText);

		m_stcVisionResult[0].Init_Ctrl("바탕", 11, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
		m_stcVisionResult[8].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
		m_stcVisionResult[0].SetWindowText(gLot.sLotID[0]);
		m_stcVisionResult[8].SetWindowText(gLot.sLotID[1]);

		if (nTotal == 0) strText.Format("0.0");
		else strText.Format("%0.1lf", gLot.nGoodCount[0] * 100.0 / nTotal);
		m_stcVisionResult[7].SetWindowText(strText);
	}

	if (wParam == 2 || wParam == 0) {
		int nTotal = gLot.nGoodCount[1] + gLot.nNgCount[1];
		strText.Format("%d", nTotal);				m_stcVisionResult[9].SetWindowText(strText);
		strText.Format("%d", gLot.nGoodCount[1]);	m_stcVisionResult[10].SetWindowText(strText);
		strText.Format("%d", gLot.nSNgCount[1][1]);	m_stcVisionResult[11].SetWindowText(strText);
		strText.Format("%d", gLot.nSNgCount[1][2]);	m_stcVisionResult[12].SetWindowText(strText);
		strText.Format("%d", gLot.nSNgCount[1][3]);	m_stcVisionResult[13].SetWindowText(strText);
		strText.Format("%d", gLot.nSNgCount[1][5]);	m_stcVisionResult[14].SetWindowText(strText);

		m_stcVisionResult[0].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
		m_stcVisionResult[8].Init_Ctrl("바탕", 11, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
		m_stcVisionResult[0].SetWindowText(gLot.sLotID[0]);
		m_stcVisionResult[8].SetWindowText(gLot.sLotID[1]);

		if (nTotal == 0) strText.Format("0.0");
		else strText.Format("%0.1lf", gLot.nGoodCount[1] * 100.0 / nTotal);
		m_stcVisionResult[15].SetWindowText(strText);
	}
	return 0;
}

LRESULT CWorkDlg::OnLotStartEnd(WPARAM wParam, LPARAM lParam)
{
	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	int nNo = lParam;
	if (wParam == 1) {
		pMainDlg->Set_LotErrorLog("LOT START", 901, "Lot Start", nNo);
		g_objLogFile.Save_EfficiencyLog(1, "Start", 901, "Lot Start");	// Lot Start
	} 
	else if (wParam == 2) 
	{
		
		pMainDlg->Set_LotErrorLog("LOT END", 902, "Lot End", nNo);
		g_objLogFile.Save_EfficiencyLog(1, "End", 902, "Lot End");
	}
	return 0;
}

LRESULT CWorkDlg::OnLotInfoClear(WPARAM wParam, LPARAM lParam)
{
	int nPNo = wParam;
	m_stcLotId[nPNo].SetWindowText("");
	m_stcCmCount[nPNo].SetWindowText("0");
	m_stcTrayCount[nPNo].SetWindowText("0");
	Enable_UserInput(nPNo+1, TRUE);

	return 0;
}

LRESULT CWorkDlg::OnModelChange(WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < 2; i++)Initial_Grid(&m_grdAngleTray[i], gData.nTrayY, gData.nTrayX, 0);
	for (int i = 0; i < 4; i++)Initial_Grid(&m_grdNgTray[i], gData.nTrayY, gData.nTrayX);
	for (int i = 0; i < 2; i++)Initial_Grid(&m_grdBufferTray[i], gData.nTrayY, gData.nTrayX);
	Initial_Grid(&m_grdGoodTray, gData.nTrayY, gData.nTrayX);

	g_dlgOperator.Set_GridModelChange();

	return 0;
}

LRESULT CWorkDlg::OnShowMsg(WPARAM wParam, LPARAM lParam)
{
	//g_dlgWork.PostMessage(UM_LOT_END_MSG, NULL, NULL);
	if		(wParam == 1) g_objCommon.Show_MsgBox(1, "NG Tray Full 상태입니다.\nNG Tray 교체 해주십시오.");
	else if	(wParam == 2) g_objCommon.Show_MsgBox(1, "현재 Lot이 종료 되었습니다. NG Tray 교체 해주십시오.");
	else if	(wParam == 3) g_objCommon.Show_MsgBox(1, "Cap Attach 설비가 정지하였습니다.");
	else if	(wParam == 4) g_objCommon.Show_MsgBox(1, "MES 수량 대비 실물 수량 부족. 확인바랍니다.");
	else if	(wParam == 5) g_objCommon.Show_MsgBox(1, "CAP Vision Alarm 입니다. 참고 바랍니다.");
	else 				  g_objCommon.Show_MsgBox(1, "Lot의 마지막 트레이 작업 중입니다.\n배출 준비 해주십시오.");

	return 0;
}

///////////////////////////////////////////////////////////////////////////////



void CWorkDlg::OnBnClickedBtnIdleReport()
{
	if (g_dlgNoWork.IsWindowVisible()) return;
	g_dlgNoWork.Start_NoWork(TRUE);
	g_dlgNoWork.ShowWindow(SW_SHOW);
}


void CWorkDlg::OnBnClickedButton1()
{
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	DX_DATA_14 *pDX14 = g_objAJinAXL.Get_pDX14();
	DX_DATA_15 *pDX15 = g_objAJinAXL.Get_pDX15();
	pDX14->iLoad1Sw = TRUE;
	pDX00->iLoadPort1Bottom = TRUE; // 1번 포트에 트레이 넣는다고 가정(1포트 시뮬레이션 위해)
	pDX15->iDoor02Unlock = FALSE; //문이 락 걸려 있다고 가정 


	
	//PostMessage(UM_UPDATE_BARCODE, NULL, NULL);
	//BOOL ret = g_objLogFile.Check_BarcodeLog("CPP00034A/560/DVRI1144N014");
	//BOOL ret = g_objMES.Read_APDResult("APDTEST7");
}


void CWorkDlg::OnBnClickedButton2()
{
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	DX_DATA_14 *pDX14 = g_objAJinAXL.Get_pDX14();
	DX_DATA_15 *pDX15 = g_objAJinAXL.Get_pDX15();
	pDX14->iLoad2Sw = TRUE;
	pDX00->iLoadPort2Bottom = TRUE;//pDX00->iLoadPort2Bottom = TRUE; // 2번 포트에 트레이 넣는다고 가정(2포트 시뮬레이션 위해)
	pDX15->iDoor03Unlock = FALSE; //문이 락 걸려 있다고 가정 
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CWorkDlg::OnBnClickedButton3()
{
	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();
	pDX12->iGoodPortBottom = FALSE;

	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CWorkDlg::OnBnClickedButton4()
{
	DX_DATA_14 *pDX14 = g_objAJinAXL.Get_pDX14();
	pDX14->iLoad2Sw = FALSE;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
}


void CWorkDlg::OnBnClickedButton5()
{
	gData.bNGTrayWait = FALSE;
	
	/*ULONGLONG size = 0;
	CString strRAM;

	g_objCommon.Get_HardInfo(strRAM);
	AfxMessageBox(strRAM);*/
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CWorkDlg::OnBnClickedButton6()
{
	
	
	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();
	DY_DATA_15 *pDY15 = g_objAJinAXL.Get_pDY15();

	pDY12->oGoodPortSlideLock = TRUE; pDY12->oGoodPortSlideUnlock = FALSE;
	g_objAJinAXL.Write_Output(12);

	
	gData.bGoodTrayWait = FALSE;
	gData.bGoodTrayLotEnd[0] = FALSE;
	gData.bGoodTrayLotEnd[1] = FALSE;
}





void CWorkDlg::WriteCMCount(int nPortNo)
{
	CString strNew, strValue;

	int nCmCnt = gData.nCmUseCount[nPortNo];

	int nTrayUseCount = nCmCnt / gData.nCmMaxCount;
	if (nCmCnt % gData.nCmMaxCount) nTrayUseCount++;

	gData.nTrayUseCount[nPortNo] = nTrayUseCount;

	strValue.Format("%d", nTrayUseCount);
	m_stcTrayCount[nPortNo].SetWindowText(strValue);
	if (nPortNo == 1) g_dlgOperator.m_stcOperTrayCount.SetWindowText(strValue);

	strValue.Format("%d", nCmCnt);
	m_stcCmCount[nPortNo].SetWindowText(strValue);
	if (nPortNo == 1) g_dlgOperator.m_stcOperCmCount.SetWindowText(strValue);

	

}


void CWorkDlg::FakeLoadPortSensor()
{
	CString strTemp, strTemp2;

	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	
	m_stcCmCount[0].GetWindowText(strTemp);	// CM 수량
	int nTemp = atoi(strTemp);
	if (nTemp > 1 || nTemp < 2400) pDX00->iLoadPort1Bottom = TRUE;

	m_stcCmCount[1].GetWindowText(strTemp2);	// CM 수량
	int nTemp2 = atoi(strTemp2);
	gData.nCmUseCount[1] = nTemp2;
	if (nTemp2 > 1 || nTemp2 < 2400) pDX00->iLoadPort2Bottom = TRUE;

}




void CWorkDlg::OnBnClickedBtnPortzInit()
{
	gData.dEmptyPort_Z_Limit = g_objDataManager.Get_pMoveData()->dEmptyPortZ[2];
}


void CWorkDlg::OnBnClickedBtnLaserconnect()
{
	g_objLaserComm.Connect();
}


void CWorkDlg::OnBnClickedBtnLaserGetdata()
{
	g_objLaserComm.Set_M0();// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
}




void CWorkDlg::OnBnClickedButton7()
{
	
	

	gData.dwSkipTime_Sort1 = GetTickCount();
}


void CWorkDlg::OnBnClickedButton9()
{
	gData.dwSkipTime_Sort2 = GetTickCount();
	DWORD dwTemp = (gData.dwSkipTime_Sort2 - gData.dwSkipTime_Sort1);

	if(dwTemp > 30000)
	{
		AfxMessageBox("good");
	}
}


void CWorkDlg::OnBnClickedChkPullforce()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CIniFileCS INI(gData.sEnvPath + "\\EquipData.ini");
	if (!INI.Check_File()) { AfxMessageBox("EquipData.ini File Not Found!!!"); return; }

	CString strTemp;

	SYSTEMTIME time;
	GetLocalTime(&time);

	
	gData.bPullForce = m_chkPullForce.GetCheck();

	if(gData.bPullForce )
	{
		m_toast.ShowToast("Pull Force Mode", 120,0);

		pEquipData->bUseInlineMode = TRUE;
		INI.Set_Bool("OPTION", "INLINE_MODE", pEquipData->bUseInlineMode);
		INI.Set_Bool("OPTION", "MES_USE", FALSE);
		m_chkMesUse.SetCheck(FALSE);
		g_objCapAttach.Set_VisionAlarmOff();

		strTemp.Format("%04d%02d%02d%02d:%02d",time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);
		m_stcLotId[0].SetWindowText(strTemp);

		pEquipData->bUseVisionAlign = TRUE;
		INI.Set_Bool("OPTION", "VISION_ALIGN", TRUE);
		pEquipData->bUseInspectAngle = TRUE;
		INI.Set_Bool("OPTION", "INSPECT_ANGLE", TRUE);
		pEquipData->bUseInspectBtm1Specular = TRUE;
		INI.Set_Bool("OPTION", "INSPECT_BTM_1_SP", TRUE);
		pEquipData->bUseInspectBtm1Angle = TRUE;
		INI.Set_Bool("OPTION", "INSPECT_BTM_1_AG", TRUE);
		pEquipData->bUseInspectBtm13D = TRUE;
		INI.Set_Bool("OPTION", "INSPECT_BTM_1_3D", TRUE);
		pEquipData->bUseInspectTop1 = TRUE;
		INI.Set_Bool("OPTION", "INSPECT_TOP_1", TRUE);
		pEquipData->bUseInspectTop2 = TRUE;
		INI.Set_Bool("OPTION", "INSPECT_TOP_2", TRUE);
		pEquipData->bUseInspectBtm2 = TRUE;
		INI.Set_Bool("OPTION", "INSPECT_BTM_2", TRUE);
		g_dlgSetup.m_pSetupEquipDlg->Cancel_EquipData();
	}
	else
	{
		m_toast.ShowWindow(SW_HIDE);

		pEquipData->bUseInlineMode = TRUE;
		INI.Set_Bool("OPTION", "INLINE_MODE", pEquipData->bUseInlineMode);
		INI.Set_Bool("OPTION", "MES_USE", TRUE);
		m_chkMesUse.SetCheck(TRUE);

		g_objCapAttach.Set_VisionAlarmOn();
		m_stcLotId[0].SetWindowText("");
		m_stcLotId[1].SetWindowText("");

		pEquipData->bUseVisionAlign = TRUE;
		INI.Set_Bool("OPTION", "VISION_ALIGN", TRUE);
		pEquipData->bUseInspectAngle = TRUE;
		INI.Set_Bool("OPTION", "INSPECT_ANGLE", TRUE);
		pEquipData->bUseInspectBtm1Specular = TRUE;
		INI.Set_Bool("OPTION", "INSPECT_BTM_1_SP", TRUE);
		pEquipData->bUseInspectBtm1Angle = TRUE;
		INI.Set_Bool("OPTION", "INSPECT_BTM_1_AG", TRUE);
		pEquipData->bUseInspectBtm13D = TRUE;
		INI.Set_Bool("OPTION", "INSPECT_BTM_1_3D", TRUE);
		pEquipData->bUseInspectTop1 = TRUE;
		INI.Set_Bool("OPTION", "INSPECT_TOP_1", TRUE);
		pEquipData->bUseInspectTop2 = TRUE;
		INI.Set_Bool("OPTION", "INSPECT_TOP_2", TRUE);
		pEquipData->bUseInspectBtm2 = TRUE;
		INI.Set_Bool("OPTION", "INSPECT_BTM_2", TRUE);

		g_dlgSetup.m_pSetupEquipDlg->Cancel_EquipData();
	}
}

void CWorkDlg::Set_PullForce(BOOL bCheck)
{
	if(!bCheck) m_chkPullForce.SetCheck(FALSE);
	else m_chkPullForce.SetCheck(TRUE);
}
