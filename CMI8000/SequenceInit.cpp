// SequenceInit.cpp : 구현 파일
//
#include "stdafx.h"
#include "CMI8000.h"
#include "SequenceInit.h"
#include "math.h"

#include "LogFile.h"
#include "Common.h"
#include "SequenceMain.h"
#include "MESInterface.h"
#include "WorkDlg.h"

const double ANGLE_STAGE_LENGTH = 250.0;	// Good Tray 1/2 간섭없는 간격

CSequenceInit g_objSequenceInit;

CSequenceInit::CSequenceInit()
{
	for (int i = 0; i < AXIS_COUNT; i++) m_pStatus[i] = g_objAJinAXL.Get_pStatus(i);
	for (int i = 0; i < AXIS_COUNT; i++) m_pParam[i] = g_objAJinAXL.Get_pParam(i);
	
	m_pDX00 = g_objAJinAXL.Get_pDX00();
	m_pDX01 = g_objAJinAXL.Get_pDX01();
	m_pDX02 = g_objAJinAXL.Get_pDX02();
	m_pDX03 = g_objAJinAXL.Get_pDX03();
	m_pDX04 = g_objAJinAXL.Get_pDX04();
	m_pDX05 = g_objAJinAXL.Get_pDX05();
	m_pDX06 = g_objAJinAXL.Get_pDX06();
	m_pDX07 = g_objAJinAXL.Get_pDX07();
	m_pDX08 = g_objAJinAXL.Get_pDX08();
	m_pDX09 = g_objAJinAXL.Get_pDX09();
	m_pDX10 = g_objAJinAXL.Get_pDX10();
	m_pDX11 = g_objAJinAXL.Get_pDX11();
	m_pDX12 = g_objAJinAXL.Get_pDX12();
	m_pDX13 = g_objAJinAXL.Get_pDX13();
	m_pDX14 = g_objAJinAXL.Get_pDX14();
	m_pDX15 = g_objAJinAXL.Get_pDX15();

	m_pDY00 = g_objAJinAXL.Get_pDY00();
	m_pDY01 = g_objAJinAXL.Get_pDY01();
	m_pDY02 = g_objAJinAXL.Get_pDY02();
	m_pDY03 = g_objAJinAXL.Get_pDY03();
	m_pDY04 = g_objAJinAXL.Get_pDY04();
	m_pDY05 = g_objAJinAXL.Get_pDY05();
	m_pDY06 = g_objAJinAXL.Get_pDY06();
	m_pDY07 = g_objAJinAXL.Get_pDY07();
	m_pDY08 = g_objAJinAXL.Get_pDY08();
	m_pDY09 = g_objAJinAXL.Get_pDY09();
	m_pDY10 = g_objAJinAXL.Get_pDY10();
	m_pDY11 = g_objAJinAXL.Get_pDY11();
	m_pDY12 = g_objAJinAXL.Get_pDY12();
	m_pDY13 = g_objAJinAXL.Get_pDY13();
	m_pDY14 = g_objAJinAXL.Get_pDY14();
	m_pDY15 = g_objAJinAXL.Get_pDY15();

	m_pEquipData = g_objDataManager.Get_pEquipData();
	m_pMoveData = g_objDataManager.Get_pMoveData();
	
	m_bThreadInitial = FALSE;
	m_pThreadInitial = NULL;
	
	Reset_InitialCase();

	m_bInitComplete = FALSE;
}

CSequenceInit::~CSequenceInit()
{
	if (m_pThreadInitial) {
		m_bThreadInitial = FALSE;
		WaitForSingleObject(m_pThreadInitial->m_hThread, INFINITE);
	}
}

int *CSequenceInit::Get_InitialCase()
{
	static int nCase[14] = { 0 };

	nCase[0]  = m_niMainInitCase;	//  0. (Error : 1000)
	nCase[1]  = m_niLoadPickCase;	//  1. (Error : 1100)
	nCase[2]  = m_niAngleUnitCase;	//  2. (Error : 1200)
	nCase[3]  = m_niBtm1PickCase;	//  3. (Error : 1300)
	nCase[4]  = m_niBtm2PickCase;	//  4. (Error : 1400)
	nCase[5]  = m_niSortPickCase;	//  5. (Error : 1500)
	nCase[6]  = m_niLoadStageCase;	//  6. (Error : 1600)
	nCase[7]  = m_niAngleStageCase; //  7. (Error : 1700)
	nCase[8]  = m_niInspStageCase;	//  8. (Error : 1800)
	nCase[9]  = m_niBuffStageCase;	//  9. (Error : 1900)
	nCase[10] = m_niGoodStageCase;	// 10. (Error : 2000)
	nCase[11] = m_niNgStageCase;	// 11. (Error : 2100)
	nCase[12] = m_niEmpTrans1Case;	// 12. (Error : 2200)
	nCase[13] = m_niEmpTrans2Case;	// 13. (Error : 2300)

	return nCase;
}

void CSequenceInit::Begin_InitialThread()
{
	m_bInitComplete = FALSE;

	Init_Replay();

	if (m_niMainInitCase == 0) m_niMainInitCase = 1;

	if (m_pThreadInitial) End_InitialThread();
	m_bThreadInitial = TRUE;
	m_pThreadInitial = AfxBeginThread(Thread_Initial, NULL);
}

void CSequenceInit::End_InitialThread()
{
	if (m_pThreadInitial) {
		m_bThreadInitial = FALSE;
		WaitForSingleObject(m_pThreadInitial->m_hThread, INFINITE);
	}
}

void CSequenceInit::Init_Replay()
{
	m_tiMainInitLoop.Set_LoopTime(5000);	//  0. (Error : 1000)
	m_tiLoadPickLoop.Set_LoopTime(5000);	//  1. (Error : 1100)
	m_tiAngleUnitLoop.Set_LoopTime(5000);	//  2. (Error : 1200)
	m_tiBtm1PickLoop.Set_LoopTime(5000);	//  3. (Error : 1300)
	m_tiBtm2PickLoop.Set_LoopTime(5000);	//  4. (Error : 1400)
	m_tiSortPickLoop.Set_LoopTime(5000);	//  5. (Error : 1500)
	m_tiLoadStageLoop.Set_LoopTime(5000);	//  6. (Error : 1600)
	m_tiAngleStageLoop.Set_LoopTime(5000);	//  7. (Error : 1700)
	m_tiInspStageLoop.Set_LoopTime(5000);	//  8. (Error : 1800)
	m_tiBuffStageLoop.Set_LoopTime(5000);	//  9. (Error : 1900)
	m_tiGoodStageLoop.Set_LoopTime(5000);	// 10. (Error : 2000)
	m_tiNgStageLoop.Set_LoopTime(5000);		// 11. (Error : 2100)
	m_tiEmpTrans1Loop.Set_LoopTime(5000);	// 12. (Error : 2200)
	m_tiEmpTrans2Loop.Set_LoopTime(5000);	// 13. (Error : 2300)
}

void CSequenceInit::Reset_InitialCase()
{
	m_niMainInitCase	= 0;	//  0. (Error : 1000)
	m_niLoadPickCase	= 0;	//  1. (Error : 1100)
	m_niAngleUnitCase	= 0;	//  2. (Error : 1200)
	m_niBtm1PickCase	= 0;	//  3. (Error : 1300)
	m_niBtm2PickCase	= 0;	//  4. (Error : 1400)
	m_niSortPickCase	= 0;	//  5. (Error : 1500)
	m_niLoadStageCase	= 0;	//  6. (Error : 1600)
	m_niAngleStageCase	= 0;	//  7. (Error : 1700)
	m_niInspStageCase	= 0;	//  8. (Error : 1800)
	m_niBuffStageCase	= 0;	//  9. (Error : 1900)
	m_niGoodStageCase	= 0;	// 10. (Error : 2000)
	m_niNgStageCase		= 0;	// 11. (Error : 2100)
	m_niEmpTrans1Case	= 0;	// 12. (Error : 2200)
	m_niEmpTrans2Case	= 0;	// 13. (Error : 2300)
}


BOOL CSequenceInit::Check_Mode()
{
	if(theApp.Get_MainMode() == MODE_MANUAL
		|| theApp.Get_MainMode() == MODE_SETUP
		|| theApp.Get_MainMode() == MODE_PARAM
		|| theApp.Get_MainMode() == MODE_PROHIBIT
		) return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Initial Thread Function 

UINT CSequenceInit::Thread_Initial(LPVOID lpVoid)
{
	while (g_objSequenceInit.m_bThreadInitial) {
		if (!g_objSequenceInit.Check_Mode()) break;
		if (!g_objCommon.Check_MainEmgAir()) break;
		if (!g_objCommon.Check_MainDoor()) break;
		if (!g_objCommon.Check_TrayFull()) break;
		if (!g_objCommon.Check_ServoOn()) break;
		if (!g_objCommon.Check_DirveAlarm()) break;

		if (!g_objSequenceInit.Initial_MainInit()) break;		//  0. (Error : 1000)
		if (!g_objSequenceInit.Initial_LoadPicker()) break;		//  1. (Error : 1100)
		if (!g_objSequenceInit.Initial_AngleUnit()) break;		//  2. (Error : 1200)
		if (!g_objSequenceInit.Initial_Btm1Picker()) break;		//  3. (Error : 1300)
		if (!g_objSequenceInit.Initial_Btm2Picker()) break;		//  4. (Error : 1400)
		if (!g_objSequenceInit.Initial_SortPicker()) break;		//  5. (Error : 1500)
		if (!g_objSequenceInit.Initial_LoadStage()) break;		//  6. (Error : 1600)
		if (!g_objSequenceInit.Initial_AngleStage()) break;		//  7. (Error : 1700)
		if (!g_objSequenceInit.Initial_InspectStage()) break;	//  8. (Error : 1800)
		if (!g_objSequenceInit.Initial_BufferStage()) break;	//  9. (Error : 1900)
		if (!g_objSequenceInit.Initial_GoodStage()) break;		// 10. (Error : 2000)
		if (!g_objSequenceInit.Initial_NgStage()) break;		// 11. (Error : 2100)
		if (!g_objSequenceInit.Initial_EmptyTrans1()) break;	// 12. (Error : 2200)
		if (!g_objSequenceInit.Initial_EmptyTrans2()) break;	// 13. (Error : 2300)

		if (!g_objSequenceInit.Initial_Simulation()) break;

		Sleep(10);
	}
	g_objSequenceInit.m_bThreadInitial = FALSE;
	g_objSequenceInit.m_pThreadInitial = NULL;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 0. (Error : 1000)
BOOL CSequenceInit::Initial_MainInit()
{
	switch (m_niMainInitCase) {
	case 0:		// Wait
		return TRUE;

	case 1:		// Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] MainInit Start");
		gLot.sLotID[0] = gLot.sLotID[1] = "";
		m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		break;

	case 2:		// Load Stage Exist Check
		if (!m_pDX01->iLoadStageExist) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Load Picker Exist Check
		if (!m_pDX01->iLoadPickerExist) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Angle Stage1 Exist Check
		if (!m_pDX02->iAngleStage1Exist) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Angle Stage2 Exist Check
		if (!m_pDX02->iAngleStage2Exist) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 6:		// Good Tray Buffer Exist Check
		//if (!m_pDX12->iGoodTrayBufferBottom) {
			m_niMainInitCase = 11; m_tiMainInitLoop.Set_LoopTime(5000);
		//}
		break;

	case 11:	// Btm1 Picker Exist Check
		if (g_objCommon.Get_Btm1PickerVacOff(0) && g_objCommon.Get_Btm1PickerAllNotChk()) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 12:		// Inspect Stage1 Exist Check
		if (g_objCommon.Get_InspectStage1VacOff(0)) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 13:		// Inspect Stage2 Exist Check
		if (g_objCommon.Get_InspectStage2VacOff(0)) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 14:	// Inspect Stage3 Exist Check
		if (g_objCommon.Get_InspectStage3VacOff(0)) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 15:	// Btm2 Picker Exist Check
		if (g_objCommon.Get_Btm2PickerVacOff(0) && g_objCommon.Get_Btm2PickerAllNotChk()) {
			m_niMainInitCase = 21; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;

	case 21:	// Buffer Stage1 Exist Check
//		if (!m_pDX09->iBufferStage1Vacuum) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
//		}
		break;
	case 22:	// Buffer Stage2 Exist Check
//		if (!m_pDX09->iBufferStage2Vacuum) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
//		}
		break;
	case 23:	// Sort Picker1 Exist Check
		if (g_objCommon.Get_SortPicker1AllNotChk()) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 24:	// Sort Picker2 Exist Check
		if (g_objCommon.Get_SortPicker2AllNotChk()) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 25:	// Good Stage1 Exist Check
		if (!m_pDX12->iGoodStage1Exist) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 26:	// Good Stage2 Exist Check
		if (!m_pDX12->iGoodStage2Exist) {
			m_niMainInitCase = 31; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;

	case 31:	// Empty Trans1 Exist Check
		if (!m_pDX13->iEmptyTrans1Exist) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 32:	// Empty Trans2 Exist Check
		if (!m_pDX13->iEmptyTrans2Exist) {
			m_niMainInitCase = 50; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;

	case 50:	// 각 파트 초기화 진행
		if (m_niLoadPickCase == 0)	m_niLoadPickCase = 1;	//  1. (Error : 1100)
		if (m_niAngleUnitCase == 0)	m_niAngleUnitCase = 1;	//  2. (Error : 1200)
		if (m_niBtm1PickCase == 0)	m_niBtm1PickCase = 1;	//  3. (Error : 1300)
		if (m_niBtm2PickCase == 0)	m_niBtm2PickCase = 1;	//  4. (Error : 1400)
		if (m_niSortPickCase == 0)	m_niSortPickCase = 1;	//  5. (Error : 1500)
		if (m_niEmpTrans1Case == 0)	m_niEmpTrans1Case = 1;	// 12. (Error : 2200)
		if (m_niEmpTrans2Case == 0)	m_niEmpTrans2Case = 1;	// 13. (Error : 2300)
		m_niMainInitCase++;
		break;

	case 51:	// Wait 
		if (m_niLoadPickCase == 90 && m_niAngleUnitCase == 90 && m_niBtm1PickCase == 90 &&
			m_niBtm2PickCase == 90 && m_niSortPickCase == 90 && m_niLoadStageCase == 90 &&
			m_niAngleStageCase == 90 && m_niInspStageCase == 90 && m_niBuffStageCase == 90 &&
			m_niGoodStageCase == 90 && m_niNgStageCase == 90 && m_niEmpTrans1Case == 90 && m_niEmpTrans2Case == 90) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		return TRUE;
	case 52:	// Angle Port Exist Check
		if (!m_pDX02->iAnglePortExist) {
			m_pDY14->oLoad1Lamp = FALSE;
			m_pDY14->oLoad2Lamp = FALSE;
			m_pDY14->oNgLamp = FALSE;
			m_pDY14->oGoodLamp = FALSE;
			m_pDY14->oEmptyLamp = FALSE;
			g_objAJinAXL.Write_Output(14);
			m_niMainInitCase = 90;
		}
		break;
		
	case 90:	// Initial Complete
		m_bInitComplete = TRUE;

		Reset_InitialCase();

		// 변수 초기화
		g_objSequenceMain.Reset_MainRunCase();
		g_objSequenceMain.Set_ClearRunData(0);
		if (gData.bCycleStop) g_dlgWork.PostMessage(UM_RESET_CYCLE_STOP, NULL, NULL);
		//g_objMesAgent.Set_EquipState(1);	// 1:Idle, 2:Run, 3:Down
		g_objMES.Initialize(m_pEquipData->bUseMES);

		gMes.nLotStatus = 0;

		g_objLogFile.Save_HandlerLog("[Initial Sequence] MainInit Complete");
		m_niMainInitCase = 0;
		return FALSE;	// Thread_Initial 종료
	}

	// 1. (Error : 1000)
	if (m_tiMainInitLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1000 + m_niMainInitCase);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 1. (Error : 1100)
BOOL CSequenceInit::Initial_LoadPicker()
{
	static double dILpPickY = 0.0;	// Load Picker Y

	switch (m_niLoadPickCase) {
	case 0:		// Wait
		return TRUE;

	case 1:		// Load Picker Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Load Picker Start");
		m_niLoadPickCase++; m_tiLoadPickLoop.Set_LoopTime(5000);
		break;
	case 2:		// Picker Master/Slave Out
		if (!m_pDX01->iLoadPickerExist) {
			m_pDY01->oLoadPickerMasterIn = FALSE; m_pDY01->oLoadPickerMasterOut = TRUE;
			m_pDY01->oLoadPickerSlaveIn = FALSE;  m_pDY01->oLoadPickerSlaveOut = TRUE;
			g_objAJinAXL.Write_Output(1);
			m_niLoadPickCase++; m_tiLoadPickLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Picker Up
		if (g_objCommon.Get_LoadPickerMasterSlaveOut()) {
			g_objCommon.Set_LoadPickerUp();
			m_niLoadPickCase++; m_tiLoadPickLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Check Picker Up
		if (g_objCommon.Get_LoadPickerUp()) {
			m_niLoadPickCase = 11; m_tiLoadPickLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Picker Y Home
		if (g_objCommon.Get_LoadPickerUp()) {
			g_objAJinAXL.Home_Search(AX_LOAD_PICKER_Y);
			m_niLoadPickCase++; m_tiLoadPickLoop.Set_LoopTime(20000);
		}
		break;
	case 12:	// Picker Y Move to Load Position
		if (g_objAJinAXL.Is_Home(AX_LOAD_PICKER_Y)) {
			if (!m_tiLoadPickLoop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_LOAD_PICKER_Y, 0);	// Load
			m_niLoadPickCase = 13; m_tiLoadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Check PickerZ Ready Up Position
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0)) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Load Picker Complete");
			m_niLoadPickCase = 90; m_tiLoadPickLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 1. (Error : 1100)
	if (m_tiLoadPickLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1100 + m_niLoadPickCase);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 2. (Error : 1200)
BOOL CSequenceInit::Initial_AngleUnit()
{
	switch (m_niAngleUnitCase) {
	case 0:		// Wait
		return TRUE;

	case 1:		// Angle Unit Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Angle Unit Start");
		m_niAngleUnitCase++; m_tiAngleUnitLoop.Set_LoopTime(5000);
		break;
	case 2:		// Vision Angle Z & Vision Align Z Home
		g_objAJinAXL.Home_Search(AX_VISION_ANGLE_Z);
		g_objAJinAXL.Home_Search(AX_VISION_ALIGN_Z);
		m_niAngleUnitCase++; m_tiAngleUnitLoop.Set_LoopTime(10000);
		break;
	case 3:		// Check Vision Angle Z & Vision Align Z Home
		if (g_objAJinAXL.Is_Home(AX_VISION_ANGLE_Z) && g_objAJinAXL.Is_Home(AX_VISION_ALIGN_Z)) {
			m_niAngleUnitCase = 11; m_tiAngleUnitLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Angle Unit Y, X, R Home
		if (g_objAJinAXL.Is_Home(AX_VISION_ANGLE_Z) && g_objAJinAXL.Is_Home(AX_VISION_ALIGN_Z)) {
			g_objAJinAXL.Home_Search(AX_ANGLE_UNIT_Y);
			g_objAJinAXL.Home_Search(AX_ANGLE_UNIT_X);
			g_objAJinAXL.Home_Search(AX_ANGLE_UNIT_R);
			m_niAngleUnitCase++; m_tiAngleUnitLoop.Set_LoopTime(20000);
		}
		break;
	case 12:	// Vision Angle Z & Vision Align Z Move to Ready Position
		if (g_objAJinAXL.Is_Home(AX_ANGLE_UNIT_Y) && g_objAJinAXL.Is_Home(AX_ANGLE_UNIT_X) && g_objAJinAXL.Is_Home(AX_ANGLE_UNIT_R)) {
			g_objCommon.Move_Position(AX_VISION_ANGLE_Z, 0);
			g_objCommon.Move_Position(AX_VISION_ALIGN_Z, 0);
			m_niAngleUnitCase++; m_tiAngleUnitLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Angle Unit Y, X, R Move to Ready Position
		if (g_objCommon.Check_Position(AX_VISION_ANGLE_Z, 0) && g_objCommon.Check_Position(AX_VISION_ALIGN_Z, 0)) {
			g_objCommon.Move_Position(AX_ANGLE_UNIT_Y, 0);
			g_objCommon.Move_Position(AX_ANGLE_UNIT_X, 0);
			g_objCommon.Move_Position(AX_ANGLE_UNIT_R, 0);
			m_niAngleUnitCase++; m_tiAngleUnitLoop.Set_LoopTime(10000);
		}
		break;
	case 14:	// Check Angle Unit Y, X, R Ready Position
		if (g_objCommon.Check_Position(AX_ANGLE_UNIT_Y, 0) &&
			g_objCommon.Check_Position(AX_ANGLE_UNIT_X, 0) &&
			g_objCommon.Check_Position(AX_ANGLE_UNIT_R, 0)) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Angle Unit Complete");
			m_niAngleUnitCase = 90; m_tiAngleUnitLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 2. (Error : 1200)
	if (m_tiAngleUnitLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1200 + m_niAngleUnitCase);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 3. (Error : 1300)
BOOL CSequenceInit::Initial_Btm1Picker()
{
	switch (m_niBtm1PickCase) {
	case 0:		// Wait
		return TRUE;

	case 1:		// Btm1 Picker Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Btm1 Picker Start");
		m_niBtm1PickCase++; m_tiBtm1PickLoop.Set_LoopTime(5000);
		break;
	case 2:		// Picker 1~10 Vacuum Off
		g_objCommon.Set_Btm1PickerVacOff(0);	// All Off
		g_objCommon.Set_Btm1PickerOpen(0);
		m_niBtm1PickCase++; m_tiBtm1PickLoop.Set_LoopTime(5000);
		break;
	case 3:		// Picker 1~10 Vacuum Air Off
		if (g_objCommon.Get_Btm1PickerVacOff(0) && g_objCommon.Get_Btm1PickerOpen(0)) {
			if (!m_tiBtm1PickLoop.Waiting_Time(500)) break;
			g_objCommon.Set_Btm1PickerAirOff(0);	// All Off
			m_niBtm1PickCase++; m_tiBtm1PickLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Picker Z & Btm1 Shift Y Home
		if (g_objCommon.Get_Btm1PickerVacOff(0) && g_objCommon.Get_Btm1PickerOpen(0)) {
			g_objAJinAXL.Home_Search(AX_BTM1_PICKER_Z);
			
			m_niBtm1PickCase = 11; m_tiBtm1PickLoop.Set_LoopTime(10000);
		}
		break;	

	case 11:	// Picker X, P1, P2 Home
		if (g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z)) {
			g_objAJinAXL.Home_Search(AX_BTM1_PICKER_X);
			g_objAJinAXL.Home_Search(AX_BTM1_PICKER_P1);
			g_objAJinAXL.Home_Search(AX_BTM1_PICKER_P2);
			
			m_niBtm1PickCase++; m_tiBtm1PickLoop.Set_LoopTime(50000);
		}
		break;
	case 12:
		if (g_objAJinAXL.Is_Home(AX_BTM1_PICKER_X) 
			&& g_objAJinAXL.Is_Home(AX_BTM1_PICKER_P1) 
			&& g_objAJinAXL.Is_Home(AX_BTM1_PICKER_P2) 
			)
		{
			m_niBtm1PickCase++; m_tiBtm1PickLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Picker 1~10 Down
		if (g_objAJinAXL.Is_Home(AX_BTM1_PICKER_X) &&
			g_objAJinAXL.Is_Home(AX_BTM1_PICKER_P1) &&
			g_objAJinAXL.Is_Home(AX_BTM1_PICKER_P2) )
		{
#ifdef EDITION_2ND
			m_pDY03->oBtm1PickerDown01 = m_pDY04->oBtm1PickerDown05 = TRUE;
			m_pDY03->oBtm1PickerDown02 = m_pDY04->oBtm1PickerDown06 = TRUE;
			m_pDY03->oBtm1PickerDown03 = m_pDY04->oBtm1PickerDown07 = TRUE;
			m_pDY03->oBtm1PickerDown04 = m_pDY04->oBtm1PickerDown08 = TRUE;
#else
			m_pDY03->oBtm1PickerDown01 = TRUE; m_pDY03->oBtm1PickerDown02 = TRUE;
			m_pDY03->oBtm1PickerDown06 = TRUE; m_pDY03->oBtm1PickerDown07 = TRUE;
#endif
			g_objAJinAXL.Write_Output(3);
			g_objAJinAXL.Write_Output(4);
			m_niBtm1PickCase++; m_tiBtm1PickLoop.Set_LoopTime(5000);
		}
		break;
	case 14:	// Picker Z Move to Ready Up Position
#ifdef EDITION_2ND
		if (g_objCommon.Get_Btm1PickerDown(0))
#else
 		if (m_tiBtm1PickLoop.Waiting_Time(1500))
#endif
		{
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 0);	// Ready Up
			
			m_niBtm1PickCase++; m_tiBtm1PickLoop.Set_LoopTime(10000);
		}
		break;
	case 15:	// Picker X, P1, P2 Move to Tray Position
		if (g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0)) {
			g_objCommon.Move_Position(AX_BTM1_PICKER_X, 0);
			
			g_objCommon.Move_Position(AX_BTM1_PICKER_P1, 0);
			g_objCommon.Move_Position(AX_BTM1_PICKER_P2, 0);
			m_niBtm1PickCase++; m_tiBtm1PickLoop.Set_LoopTime(10000);
		}
		break;
	case 16:	// Check Picker X, P1, P2 Ready Position
		if (g_objCommon.Check_Position(AX_BTM1_PICKER_X, 0) &&
			g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 0) &&
			g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 0)) {
			g_objAJinAXL.Home_Search(AX_BTM1_SHIFT_Y);
			m_niBtm1PickCase++; m_tiBtm1PickLoop.Set_LoopTime(10000);
		}
		break;

	case 17:		// Check Picker Z Home
		if (g_objAJinAXL.Is_Home(AX_BTM1_SHIFT_Y) ) {
			g_objCommon.Move_Position(AX_BTM1_SHIFT_Y, 0);
			m_niBtm1PickCase++; m_tiBtm1PickLoop.Set_LoopTime(10000);
		}
		break;
	case 18:		// Check Picker Z Home
		if ( g_objCommon.Check_Position(AX_BTM1_SHIFT_Y, 0)) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Btm1 Picker Complete");
			m_niBtm1PickCase = 90; m_tiBtm1PickLoop.Set_LoopTime(5000);
			
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 3. (Error : 1300)
	if (m_tiBtm1PickLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1300 + m_niBtm1PickCase);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 4. (Error : 1400)
BOOL CSequenceInit::Initial_Btm2Picker()
{
	switch (m_niBtm2PickCase) {
	case 0:		// Wait
		return TRUE;

	case 1:		// Btm2 Picker Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Btm2 Picker Start");
		m_niBtm2PickCase++; m_tiBtm2PickLoop.Set_LoopTime(5000);
		break;
	case 2:		// Picker 1~10 Vacuum Off
		g_objCommon.Set_Btm2PickerVacOff(0);	// All Off
		g_objCommon.Set_Btm2PickerOpen(0);		// All Open
		m_niBtm2PickCase++; m_tiBtm2PickLoop.Set_LoopTime(5000);
		break;
	case 3:		// Picker 1~10 Vacuum Air Off
		if (g_objCommon.Get_Btm2PickerVacOff(0) && g_objCommon.Get_Btm2PickerOpen(0)) {
			if (!m_tiBtm2PickLoop.Waiting_Time(500)) break;
			g_objCommon.Set_Btm2PickerAirOff(0);	// All Off
			m_niBtm2PickCase++; m_tiBtm2PickLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Picker Z Home
		if (g_objCommon.Get_Btm2PickerVacOff(0) && g_objCommon.Get_Btm2PickerOpen(0)) {
			g_objAJinAXL.Home_Search(AX_BTM2_PICKER_Z);
			m_niBtm2PickCase++; m_tiBtm2PickLoop.Set_LoopTime(10000);
		}
		break;
	case 5:		// Check Picker Z Home
		if (g_objAJinAXL.Is_Home(AX_BTM2_PICKER_Z)) {
			m_niBtm2PickCase = 10; m_tiBtm2PickLoop.Set_LoopTime(5000);
		}
		break;

	case 10:	// Top2 Shift Home wait
		if (m_niInspStageCase > 5) m_niBtm2PickCase++;
		return TRUE;

	case 11:	// Picker X, P1, P2 Home
		if (g_objAJinAXL.Is_Home(AX_BTM2_PICKER_Z)) {
			g_objAJinAXL.Home_Search(AX_BTM2_PICKER_X);
			g_objAJinAXL.Home_Search(AX_BTM2_PICKER_P1);
			g_objAJinAXL.Home_Search(AX_BTM2_PICKER_P2);
			m_niBtm2PickCase++; m_tiBtm2PickLoop.Set_LoopTime(50000);
		}
		break;
	case 12:	// Picker 1~10 Normal
		if (g_objAJinAXL.Is_Home(AX_BTM2_PICKER_X) &&
			g_objAJinAXL.Is_Home(AX_BTM2_PICKER_P1) &&
			g_objAJinAXL.Is_Home(AX_BTM2_PICKER_P2))
		{
			m_niBtm2PickCase++; m_tiBtm2PickLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Picker 1~10 Down
		if (g_objAJinAXL.Is_Home(AX_BTM2_PICKER_X) &&
			g_objAJinAXL.Is_Home(AX_BTM2_PICKER_P1) &&
			g_objAJinAXL.Is_Home(AX_BTM2_PICKER_P2))
		{
#ifdef EDITION_2ND
			m_pDY07->oBtm2PickerDown01 = m_pDY08->oBtm2PickerDown05 = TRUE;
			m_pDY07->oBtm2PickerDown02 = m_pDY08->oBtm2PickerDown06 = TRUE;
			m_pDY07->oBtm2PickerDown03 = m_pDY08->oBtm2PickerDown07 = TRUE;
			m_pDY07->oBtm2PickerDown04 = m_pDY08->oBtm2PickerDown08 = TRUE;
#else
			m_pDY07->oBtm2PickerDown01 = TRUE; m_pDY07->oBtm2PickerDown02 = TRUE;
			m_pDY07->oBtm2PickerDown06 = TRUE; m_pDY07->oBtm2PickerDown07 = TRUE;
#endif
			g_objAJinAXL.Write_Output(7);
			g_objAJinAXL.Write_Output(8);
			m_niBtm2PickCase++; m_tiBtm2PickLoop.Set_LoopTime(5000);
		}
		break;
	case 14:	// Picker Z Move to Ready Up Position
#ifdef EDITION_2ND
		if (g_objCommon.Get_Btm2PickerDown(0))
#else
		if (m_tiBtm2PickLoop.Waiting_Time(1500))
#endif
		{
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 0);	// Ready Up
			m_niBtm2PickCase++; m_tiBtm2PickLoop.Set_LoopTime(10000);
		}
		break;
	case 15:	// Picker Y Move to Tray Position
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0)) {
			g_objCommon.Move_Position(AX_BTM2_PICKER_X, 0);
			g_objCommon.Move_Position(AX_BTM2_PICKER_P1, 0);
			g_objCommon.Move_Position(AX_BTM2_PICKER_P2, 0);
			m_niBtm2PickCase++; m_tiBtm2PickLoop.Set_LoopTime(10000);
		}
		break;
	case 16:	// Check Picker X, P1, P2 Ready Check
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_X, 0) &&
			g_objCommon.Check_Position(AX_BTM2_PICKER_P1, 0) &&
			g_objCommon.Check_Position(AX_BTM2_PICKER_P2, 0)) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Btm2 Picker Complete");
			m_niBtm2PickCase = 90; m_tiBtm2PickLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 4. (Error : 1400)
	if (m_tiBtm2PickLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1400 + m_niBtm2PickCase);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 5. (Error : 1500)
BOOL CSequenceInit::Initial_SortPicker()
{
	switch (m_niSortPickCase) {
	case 0:		// Wait
		return TRUE;

	case 1:		// Sort Picker Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Sort Picker Start");
		m_niSortPickCase++; m_tiSortPickLoop.Set_LoopTime(5000);
		break;
	case 2:		// Picker1/2 1~5 Vacuum Off
		if (g_objCommon.Get_SortPicker1AllNotChk() && g_objCommon.Get_SortPicker2AllNotChk()) {
			g_objCommon.Set_SortPicker1Open(0);	// All Open
			g_objCommon.Set_SortPicker2Open(0);	// All Open
			g_objCommon.Set_NgBufferVacOff(1);
			g_objCommon.Set_NgBufferVacOff(2);
			m_niSortPickCase++; m_tiSortPickLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Picker1/2 1~5 Vacuum Air Off
		if (g_objCommon.Get_SortPicker1Open(0) && g_objCommon.Get_SortPicker2Open(0)) {
			if (!m_tiSortPickLoop.Waiting_Time(500)) break;
			m_niSortPickCase++; m_tiSortPickLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Picker1/2 1~6 Up
		if (g_objCommon.Get_SortPicker1Open(0) && g_objCommon.Get_SortPicker2Open(0)) {
			g_objCommon.Set_SortPicker1Up(0);	// All Up
			g_objCommon.Set_SortPicker2Up(0);	// All Up
			m_niSortPickCase++; m_tiSortPickLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Picker1/2 Z Home
		if (g_objCommon.Get_SortPicker1Up(0) && g_objCommon.Get_SortPicker2Up(0)) {
			g_objAJinAXL.Home_Search(AX_SORT_PICKER1_Z);
			g_objAJinAXL.Home_Search(AX_SORT_PICKER2_Z);
			m_niSortPickCase++; m_tiSortPickLoop.Set_LoopTime(10000);
		}
		break;
	case 6:		// Picker1/2 X Home
		if (g_objAJinAXL.Is_Home(AX_SORT_PICKER1_Z) && g_objAJinAXL.Is_Home(AX_SORT_PICKER2_Z)) {
			m_niSortPickCase = 11; m_tiSortPickLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Picker1/2 X, P Home
		if (g_objAJinAXL.Is_Home(AX_SORT_PICKER1_Z) && g_objAJinAXL.Is_Home(AX_SORT_PICKER2_Z)) {
			g_objAJinAXL.Home_Search(AX_SORT_PICKER1_X); g_objAJinAXL.Home_Search(AX_SORT_PICKER1_P);
			g_objAJinAXL.Home_Search(AX_SORT_PICKER2_X); g_objAJinAXL.Home_Search(AX_SORT_PICKER2_P);
			m_niSortPickCase++; m_tiSortPickLoop.Set_LoopTime(50000);
		}
		break;
	case 12:	// Picker1/2 Z Move to Ready Up Position
		if (g_objAJinAXL.Is_Home(AX_SORT_PICKER1_X) && g_objAJinAXL.Is_Home(AX_SORT_PICKER1_P) &&
			g_objAJinAXL.Is_Home(AX_SORT_PICKER2_X) && g_objAJinAXL.Is_Home(AX_SORT_PICKER2_P)) {
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
			m_niSortPickCase++; m_tiSortPickLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Picker1/2 X, P Move to Ready Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0)) {
			g_objCommon.Move_Position(AX_SORT_PICKER1_X, 0); g_objCommon.Move_Position(AX_SORT_PICKER1_P, 0);
			g_objCommon.Move_Position(AX_SORT_PICKER2_X, 0); g_objCommon.Move_Position(AX_SORT_PICKER2_P, 0);
			m_niSortPickCase++; m_tiSortPickLoop.Set_LoopTime(10000);
		}
		break;
	case 14:	// Check Picker X Ready Main Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_X, 0) && g_objCommon.Check_Position(AX_SORT_PICKER1_P, 0) &&
			g_objCommon.Check_Position(AX_SORT_PICKER2_X, 0) && g_objCommon.Check_Position(AX_SORT_PICKER2_P, 0)) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Sort Picker Complete");
			m_niSortPickCase = 90; m_tiSortPickLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 5. (Error : 1500)
	if (m_tiSortPickLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1500 + m_niSortPickCase);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 6. (Error : 1600)
BOOL CSequenceInit::Initial_LoadStage()
{
	switch (m_niLoadStageCase) {
	case 0:		// Wait for Load Picker
		if (m_niLoadPickCase > 10) m_niLoadStageCase++;
		return TRUE;

	case 1:		// Load Tray Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Load Stage Start");
		m_niLoadStageCase++; m_tiLoadStageLoop.Set_LoopTime(5000);
		break;
	case 2:		// Port1/2 Support In
		if (m_pDX00->iLoadPort1SlideClose && !m_pDX00->iLoadPort1SlideOpen &&
			m_pDX00->iLoadPort2SlideClose && !m_pDX00->iLoadPort2SlideOpen) {
			g_objCommon.Set_LoadPortSupportIn(1);
			g_objCommon.Set_LoadPortSupportIn(2);
			m_niLoadStageCase++; m_tiLoadStageLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Stage Master/Slave Out
		if (g_objCommon.Get_LoadPortSupportIn(1) && g_objCommon.Get_LoadPortSupportIn(2)) {
			m_pDY01->oLoadStageMasterIn = FALSE;
			m_pDY01->oLoadStageSlaveIn = FALSE;
			g_objAJinAXL.Write_Output(1);
			m_niLoadStageCase++; m_tiLoadStageLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Stage Z Home Search
		if (g_objCommon.Get_LoadTrayMasterSlaveOut()) {
			g_objAJinAXL.Home_Search(AX_LOAD_STAGE_Z);
			m_niLoadStageCase++; m_tiLoadStageLoop.Set_LoopTime(10000);
		}
		break;
	case 5:		// Check Stage Z Home
		if (g_objAJinAXL.Is_Home(AX_LOAD_STAGE_Z)) {
			m_niLoadStageCase = 11; m_tiLoadStageLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Stage X Home Search
		if (g_objAJinAXL.Is_MoveDone(AX_LOAD_STAGE_Z, 0.0)) {
			g_objAJinAXL.Home_Search(AX_LOAD_STAGE_X);
			m_niLoadStageCase++; m_tiLoadStageLoop.Set_LoopTime(20000);
		}
		break;
	case 12:	// Stage Z Move to Ready Down Position
		if (g_objAJinAXL.Is_Home(AX_LOAD_STAGE_X)) {
			g_objCommon.Move_Position(AX_LOAD_STAGE_Z, 0);	// Ready Down
			m_niLoadStageCase++; m_tiLoadStageLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Stage X Move to Load Position
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 0)) {
			g_objCommon.Move_Position(AX_LOAD_STAGE_X, 0);	// Load
			m_niLoadStageCase++; m_tiLoadStageLoop.Set_LoopTime(10000);
		}
		break;
	case 14:	// Load Port Slide Unlock
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_X, 0)) {
			m_pDY00->oLoadPort1SlideLock = FALSE; m_pDY00->oLoadPort1SlideUnlock = TRUE;
			m_pDY00->oLoadPort2SlideLock = FALSE; m_pDY00->oLoadPort2SlideUnlock = TRUE;
			g_objAJinAXL.Write_Output(0);
			m_niLoadStageCase++; m_tiLoadStageLoop.Set_LoopTime(5000);
		}
		break;
	case 15:	// Check Load Port Slide Unlock
		if (!m_pDX00->iLoadPort1SlideLock && m_pDX00->iLoadPort1SlideUnlock &&
			!m_pDX00->iLoadPort2SlideLock && m_pDX00->iLoadPort2SlideUnlock) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Load Stage Complete");
			m_niLoadStageCase = 90; m_tiLoadStageLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 6. (Error : 1600)
	if (m_tiLoadStageLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1600 + m_niLoadStageCase);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 7. (Error : 1700)
BOOL CSequenceInit::Initial_AngleStage()
{
	static double dIaY1 = 0.0, dIaZ1 = 0.0, dIaY2 = 0.0, dIaZ2 = 0.0;
	static int nIaAvoid = 0;	// 피하기 위한 위치 인덱스 (0:Load, 4:Avoid)

	switch (m_niAngleStageCase) {
	case 0:		// Wait for Good Picker Complete
		if (m_niBtm1PickCase > 10 && m_niEmpTrans1Case > 10) m_niAngleStageCase++;
		return TRUE;

	case 1:		// Angle Stage Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Angle Stage Start");
		m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(5000);
		break;
	case 2:		// Port Support In
		g_objCommon.Set_AnglePortSupportIn();
		m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(5000);
		break;
	case 3:		// Port Master/Slave Out
		if (g_objCommon.Get_AnglePortSupportIn()) {
			m_pDY02->oAnglePortMasterIn = FALSE; m_pDY02->oAnglePortMasterOut = TRUE;
			m_pDY02->oAnglePortSlaveIn = FALSE; m_pDY02->oAnglePortSlaveOut = TRUE;
			g_objAJinAXL.Write_Output(2);
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Stage1/2 Align Out
		if (g_objCommon.Get_AnglePortMasterSlaveOut()) {
			m_pDY02->oAngleStage1AlignIn = FALSE; m_pDY02->oAngleStage2AlignIn = FALSE;
			g_objAJinAXL.Write_Output(2);
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Check Stage1/2 Align Out & Tray Check
		if (!m_pDX02->iAngleStage1Exist && !m_pDX02->iAngleStage2Exist) {
			if (!m_pDX02->iAngleStage1AlignIn && m_pDX02->iAngleStage1AlignOut && 
				!m_pDX02->iAngleStage2AlignIn && m_pDX02->iAngleStage2AlignOut)
			{
				m_niAngleStageCase = 10; m_tiAngleStageLoop.Set_LoopTime(5000);
			}
		}
		break;

	case 10:	// Stage1/2 Z Check
		dIaY1 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE1_Y);
		dIaY2 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE2_Y);
		if (fabs(dIaY2 - dIaY1) < ANGLE_STAGE_LENGTH) {
			dIaZ1 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE1_Z);
			dIaZ2 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE2_Z);
			if (dIaZ1 < dIaZ2) {	// Stage1 Z Down, Stage2 Z Up
				m_niAngleStageCase = 11; m_tiAngleStageLoop.Set_LoopTime(5000);
			} else {				// Stage1 Z Up, Stage2 Z Down
				m_niAngleStageCase = 21; m_tiAngleStageLoop.Set_LoopTime(5000);
			}
		} else {					// Stage1/2 Z Up or Down
			m_niAngleStageCase = 31; m_tiAngleStageLoop.Set_LoopTime(5000);
		}
		break;

	// Stage1 Z Down, Stage2 Z Up 상태
	case 11:	// Stage1 Y Move to Unload or Avoid Position (Stage2 Y Check)
		dIaZ1 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE1_Z);
		dIaZ2 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE2_Z);
		if (dIaZ1 > m_pMoveData->dAngleStage1Z[0] + 0.5) break;	// 인터락
		if (dIaZ2 < m_pMoveData->dAngleStage2Z[1] - 0.5 && dIaZ2 < m_pMoveData->dAngleStage2Z[2] - 0.5) break;	// 인터락

		dIaY2 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE2_Y);
		nIaAvoid = (dIaY2 > m_pMoveData->dAngleStage2Y[1] - 0.5 ? 0 : 4);	// 0:Load, 4:Unload
		g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, nIaAvoid);
		m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(10000);
		break;
	case 12:	// Check Stage1 Y Load or Unload
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, nIaAvoid)) {
			m_niAngleStageCase = 31; m_tiAngleStageLoop.Set_LoopTime(5000);
		}
		break;

	// Stage1 Z Up, Stage2 Z Down 상태
	case 21:	// Stage2 Y Move to Unload or Avoid Position (Stage1 Y Check)
		dIaZ2 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE2_Z);
		dIaZ1 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE1_Z);
		if (dIaZ2 > m_pMoveData->dAngleStage2Z[0] + 0.5) break;	// 인터락
		if (dIaZ1 < m_pMoveData->dAngleStage1Z[1] - 0.5 && dIaZ1 < m_pMoveData->dAngleStage1Z[2] - 0.5) break;	// 인터락

		dIaY1 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE1_Y);
		nIaAvoid = (dIaY1 > m_pMoveData->dAngleStage1Y[1] - 0.5 ? 0 : 4);	// 0:Load, 4:Unload
		g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, nIaAvoid);
		m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(10000);
		break;
	case 22:	// Check Stage2 Y Load or Unload
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, nIaAvoid)) {
			m_niAngleStageCase = 31; m_tiAngleStageLoop.Set_LoopTime(5000);
		}
		break;

	// 공통 (Stage1/2 Z Homimg)
	case 31:	// Stage1/2 Z Home Search
		dIaY1 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE1_Y);
		dIaY2 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE2_Y);
		if (fabs(dIaY2 - dIaY1) < ANGLE_STAGE_LENGTH) break;	// 인터락 (Y1, Stage2 Y 간격 체크)

		g_objAJinAXL.Home_Search(AX_ANGLE_STAGE1_Z);
		g_objAJinAXL.Home_Search(AX_ANGLE_STAGE2_Z);
		m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(60000);
		break;
	case 32:	// Stage 1/2 Z Move to Ready Down Position
		if (g_objAJinAXL.Is_Home(AX_ANGLE_STAGE1_Z) && g_objAJinAXL.Is_Home(AX_ANGLE_STAGE2_Z)) {
			if (!m_tiAngleStageLoop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 0);	// Ready Down
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 0);	// Ready Down
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(10000);
		}
		break;
	case 33:	// Check Stage1/2 Z Ready Down Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 0)) {
			m_niAngleStageCase = 40; m_tiAngleStageLoop.Set_LoopTime(5000);
		}
		break;

	case 40:	// Stage1/2 Y Check
		dIaY1 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE1_Y);
		dIaY2 = g_objAJinAXL.Get_Position(AX_ANGLE_STAGE2_Y);
		m_niAngleStageCase = (dIaY1 < dIaY2 ? 41 : 51); m_tiAngleStageLoop.Set_LoopTime(5000);
		break;

	// Stage1 Y Front 상태
	case 41:	// Stage1 Y Move to Load Position & Stage2 Y Move to Ready Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 0)) {
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, 0);	// Load
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, 1);	// Ready
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(10000);
		}
		break;
	case 42:	// Stage1 Normal
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 0)) {
			if (!g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 0)) break;	// 인터락
			m_pDY02->oAngleStage1Rotate = FALSE;
			g_objAJinAXL.Write_Output(2);
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(5000);
		}
		break;
	case 43:	// Stage1 Y Home Search
		if (m_pDX02->iAngleStage1Normal && !m_pDX02->iAngleStage1Rotate) {
			if (!g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 0)) break;	// 인터락
			g_objAJinAXL.Home_Search(AX_ANGLE_STAGE1_Y);
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(20000);
		}
		break;
	case 44:	// Stage1 Y Move to Load Position
		if (g_objAJinAXL.Is_Home(AX_ANGLE_STAGE1_Y)) {
			if (!g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 0)) break;	// 인터락
			if (!m_tiAngleStageLoop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, 0);	// Load
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(10000);
		}
		break;
	case 45:	// Stage1 Z Move to Load Up Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 1)) {
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 1);	// Load Up
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(10000);
		}
		break;
	case 46:	// Stage2 Y Home Search
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 0)) {
			g_objAJinAXL.Home_Search(AX_ANGLE_STAGE2_Y);
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(20000);
		}
		break;
	case 47:	// Stage2 Y Move to Unload Position
		if (g_objAJinAXL.Is_Home(AX_ANGLE_STAGE2_Y) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 0)) {
			if (!g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 1)) break;	// 인터락
			if (!m_tiAngleStageLoop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, 4);	// Unload
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(10000);
		}
		break;
	case 48:	// Stage2 Z Move to Work Up & Stage1 Z Move to Ready Down Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 4) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 0)) {
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 2);	// Work Up
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 0);	// Ready Down
			m_niAngleStageCase = 61; m_tiAngleStageLoop.Set_LoopTime(10000);
		}
		break;

	// Stage2 Y Front 상태
	case 51:	// Stage2 Y Move to Load Position & Stage1 Y Move to Ready Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 0)) {
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, 0);		// Load
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, 1);		// Ready
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(10000);
		}
		break;
	case 52:	// Stage2 Y Home Search
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 0)) {
			if (!g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 0)) break;	// 인터락
			if (!m_tiAngleStageLoop.Waiting_Time(500)) break;
			g_objAJinAXL.Home_Search(AX_ANGLE_STAGE2_Y);
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(20000);
		}
		break;
	case 53:	// Stage1 Z Move to Work Up Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 1)) {
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 2);	// Work Up
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(10000);
		}
		break;
	case 54:	// Stage2 Y Move to Unload Position
		if (g_objAJinAXL.Is_Home(AX_ANGLE_STAGE2_Y) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 2)) {
			if (!g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 0)) break;	// 인터락
			if (!m_tiAngleStageLoop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, 4);	// Unload
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(10000);
		}
		break;
	case 55:	// Stage2 Z Move to Work Up & Stage1 Z Move to Ready Down Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 4) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 1)) {
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 2);	// Work Up
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 0);	// Ready Down
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(10000);
		}
		break;
	case 56:	// Stage1 Y Home Search
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 2) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 0)) {
			g_objAJinAXL.Home_Search(AX_ANGLE_STAGE1_Y);
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(20000);
		}
		break;
	case 57:	// Stage1 Y Move to Load Position
		if (g_objAJinAXL.Is_Home(AX_ANGLE_STAGE1_Y) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 0)) {
			if (!m_tiAngleStageLoop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, 0);		// Load
			m_niAngleStageCase = 61; m_tiAngleStageLoop.Set_LoopTime(10000);
		}
		break;

	case 61:	// Stage1,2 Normal
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 4) &&
			g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 2) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 0)) {
			m_pDY02->oAngleStage1Rotate = m_pDY02->oAngleStage2Rotate = FALSE;
			g_objAJinAXL.Write_Output(2);
			m_niAngleStageCase++; m_tiAngleStageLoop.Set_LoopTime(5000);
		}
		break;
	case 62:	// Check Stage1,2 Normal
		if (m_pDX02->iAngleStage1Normal && !m_pDX02->iAngleStage1Rotate && m_pDX02->iAngleStage2Normal && !m_pDX02->iAngleStage2Rotate) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Angle Stage Complete");
			m_niAngleStageCase = 90; m_tiAngleStageLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 17. (Error : 1700)
	if (m_tiAngleStageLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1700 + m_niAngleStageCase);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 8. (Error : 1800)
BOOL CSequenceInit::Initial_InspectStage()
{
	switch (m_niInspStageCase) {
	case 0:		// Wait for Load Picker & Unload Picker
		if (m_niBtm1PickCase > 10 && m_niBtm2PickCase >= 10) m_niInspStageCase++;
		return TRUE;

	case 1:		// Inspect Stage Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Inspect Stage Start");
		// CM Align Open
		m_pDY06->oCmAlignMasterOpen = TRUE; m_pDY06->oCmAlignMasterClose = FALSE;
		m_pDY06->oCmAlignSlaveOpen = TRUE;  m_pDY06->oCmAlignSlaveClose = FALSE;
		g_objAJinAXL.Write_Output(6);
		m_niInspStageCase++; m_tiInspStageLoop.Set_LoopTime(5000);
		break;
	case 2:		// Vision Top1 Z & Top1 Mirror Z & Top2 Shift Y & Vision Top2 Z Home
		if (m_pDX06->iCmAlignMasterOpen && !m_pDX06->iCmAlignMasterClose &&
			m_pDX06->iCmAlignSlaveOpen  && !m_pDX06->iCmAlignSlaveClose) {
			g_objAJinAXL.Home_Search(AX_VISION_TOP1_Z);
			g_objAJinAXL.Home_Search(AX_TOP1_MIRROR_Z);
			g_objAJinAXL.Home_Search(AX_TOP2_SHIFT_Y);
			g_objAJinAXL.Home_Search(AX_VISION_TOP2_Z);
			g_objAJinAXL.Home_Search(AX_MODULE_ALIGN_Y);
			g_objAJinAXL.Home_Search(AX_MODULE_ALIGN_Z);
			m_niInspStageCase++; m_tiInspStageLoop.Set_LoopTime(10000);
		}
		break;
	case 3:		// Vision Top1 Z & Top1 Mirror Z & Top2 Shift Y & Vision Top2 Z Move Ready
		if (g_objAJinAXL.Is_Home(AX_VISION_TOP1_Z) && g_objAJinAXL.Is_Home(AX_TOP1_MIRROR_Z) &&
			g_objAJinAXL.Is_Home(AX_TOP2_SHIFT_Y) && g_objAJinAXL.Is_Home(AX_VISION_TOP2_Z) &&
			g_objAJinAXL.Is_Home(AX_MODULE_ALIGN_Y) && g_objAJinAXL.Is_Home(AX_MODULE_ALIGN_Z)) {
			if (!m_tiInspStageLoop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_VISION_TOP1_Z, 0);
			g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 0);
			g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);
			g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);
			g_objCommon.Move_Position(AX_MODULE_ALIGN_Y, 0);
			g_objCommon.Move_Position(AX_MODULE_ALIGN_Z, 0);
			m_niInspStageCase++; m_tiInspStageLoop.Set_LoopTime(10000);
		}
		break;
	case 4:		// Stage1/2/3 Vacuum Off && Top1 Mirror Up(A) or Down(B,C)
		if (g_objCommon.Check_Position(AX_VISION_TOP1_Z, 0) && g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0) &&
			g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 0) && g_objCommon.Check_Position(AX_VISION_TOP2_Z, 0) &&
			g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0) && g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0)) {
			g_objCommon.Set_InspectStage1VacOff(0);
			g_objCommon.Set_InspectStage2VacOff(0);
			g_objCommon.Set_InspectStage3VacOff(0);

			// Mirror Up/Down
// 			if (gData.bR04B) { m_pDY06->oTop1Mirror12Up = FALSE; m_pDY06->oTop1Mirror12Down = TRUE; }
// 			else { m_pDY06->oTop1Mirror12Up = TRUE; m_pDY06->oTop1Mirror12Down = FALSE; }

			g_objAJinAXL.Write_Output(6);
			m_niInspStageCase++; m_tiInspStageLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Stage2/3 Backward
		if (g_objCommon.Get_InspectStage1VacOff(0) && g_objCommon.Get_InspectStage2VacOff(0) && g_objCommon.Get_InspectStage3VacOff(0)) {

			m_pDY05->oInspectStage2Fwd = FALSE; m_pDY05->oInspectStage2Bwd = TRUE;
			m_pDY05->oInspectStage3Fwd = FALSE; m_pDY05->oInspectStage3Bwd = TRUE;
			g_objAJinAXL.Write_Output(5);
			m_niInspStageCase++; m_tiInspStageLoop.Set_LoopTime(10000);

			// Mirror Up/Down Check
// 			if ((gData.bR04B && !m_pDX06->iTop1Mirror1Up && m_pDX06->iTop1Mirror1Down && !m_pDX06->iTop1Mirror2Up && m_pDX06->iTop1Mirror2Down) ||
// 				(!gData.bR04B && m_pDX06->iTop1Mirror1Up && !m_pDX06->iTop1Mirror1Down && m_pDX06->iTop1Mirror2Up && !m_pDX06->iTop1Mirror2Down)) {
// 
// 				m_pDY05->oInspectStage2Fwd = FALSE; m_pDY05->oInspectStage2Bwd = TRUE;
// 				m_pDY05->oInspectStage3Fwd = FALSE; m_pDY05->oInspectStage3Bwd = TRUE;
// 				g_objAJinAXL.Write_Output(5);
// 				m_niInspStageCase++; m_tiInspStageLoop.Set_LoopTime(10000);
// 			}
			
		}
		break;
	case 6:		// Stage2/3 Up or Down
		if (!m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd) {
			if (m_pDX05->iInspectStage1Up && !m_pDX05->iInspectStage1Down) {		// Up
				m_pDY05->oInspectStage2Up = FALSE; m_pDY05->oInspectStage2Down = TRUE;	// Down
				m_pDY05->oInspectStage3Up = FALSE; m_pDY05->oInspectStage3Down = TRUE;	// Down
				g_objAJinAXL.Write_Output(5);
				m_niInspStageCase = 11; m_tiInspStageLoop.Set_LoopTime(5000);
			} else if (!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down) {	// Down
				m_pDY05->oInspectStage2Up = TRUE; m_pDY05->oInspectStage2Down = FALSE;	// Up
				m_pDY05->oInspectStage3Up = TRUE; m_pDY05->oInspectStage3Down = FALSE;	// Up
				g_objAJinAXL.Write_Output(5);
				m_niInspStageCase = 21; m_tiInspStageLoop.Set_LoopTime(5000);
			}
		}
		break;

	case 11:	// Stage1/2/3 X Home Search
		if (m_pDX05->iInspectStage1Up && !m_pDX05->iInspectStage1Down &&
			!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down &&
			!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down) {	// Up(1), Down(2,3)
			if (m_pDX05->iInspectStage2Fwd || !m_pDX05->iInspectStage2Bwd) break;	// 인터락
			if (m_pDX05->iInspectStage3Fwd || !m_pDX05->iInspectStage3Bwd) break;	// 인터락
			g_objAJinAXL.Home_Search(AX_INSPECT_STAGE1_X);
			g_objAJinAXL.Home_Search(AX_INSPECT_STAGE2_X);
			g_objAJinAXL.Home_Search(AX_INSPECT_STAGE3_X);
			m_niInspStageCase++; m_tiInspStageLoop.Set_LoopTime(60000);
		}
		break;
	case 12:	// Stage1/2/3 X Move to Load/Load/Unload Position
		if (g_objAJinAXL.Is_Home(AX_INSPECT_STAGE1_X) && g_objAJinAXL.Is_Home(AX_INSPECT_STAGE2_X) && g_objAJinAXL.Is_Home(AX_INSPECT_STAGE3_X)) {
			if (!m_tiInspStageLoop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_INSPECT_STAGE1_X, 0);	// Load
			g_objCommon.Move_Position(AX_INSPECT_STAGE2_X, 0);	// Load
			g_objCommon.Move_Position(AX_INSPECT_STAGE3_X, 4);	// Unload
			m_niInspStageCase++; m_tiInspStageLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Check Stage1/2/3 X Position
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0) &&
			g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0) &&
			g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 4)) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Inspect Stage Complete");
			m_niInspStageCase = 90; m_tiInspStageLoop.Set_LoopTime(5000);
		}
		break;

	case 21:	// Stage1/2/3 X Home Search
		if (!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down &&
			m_pDX05->iInspectStage2Up && !m_pDX05->iInspectStage2Down &&
			m_pDX05->iInspectStage3Up && !m_pDX05->iInspectStage3Down) {	// Down(1), Up(2,3)
			if (m_pDX05->iInspectStage2Fwd || !m_pDX05->iInspectStage2Bwd) break;	// 인터락
			if (m_pDX05->iInspectStage3Fwd || !m_pDX05->iInspectStage3Bwd) break;	// 인터락
			g_objAJinAXL.Home_Search(AX_INSPECT_STAGE1_X);
			g_objAJinAXL.Home_Search(AX_INSPECT_STAGE2_X);
			g_objAJinAXL.Home_Search(AX_INSPECT_STAGE3_X);
			m_niInspStageCase++; m_tiInspStageLoop.Set_LoopTime(30000);
		}
		break;
	case 22:	// Stage1/2/3 X Move to Load/Align/Unload Position
		if (g_objAJinAXL.Is_Home(AX_INSPECT_STAGE1_X) && g_objAJinAXL.Is_Home(AX_INSPECT_STAGE2_X) && g_objAJinAXL.Is_Home(AX_INSPECT_STAGE3_X)) {
			if (!m_tiInspStageLoop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_INSPECT_STAGE1_X, 0);	// Load
			g_objCommon.Move_Position(AX_INSPECT_STAGE2_X, 2);	// Top1
			g_objCommon.Move_Position(AX_INSPECT_STAGE3_X, 4);	// Unload
			m_niInspStageCase++; m_tiInspStageLoop.Set_LoopTime(10000);
		}
		break;
	case 23:	// Stage1 Up, Stage2/3 Down
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0) &&
			g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 2) &&
			g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 4)) {
			m_pDY05->oInspectStage1Up = TRUE;  m_pDY05->oInspectStage1Down = FALSE;	// Up
			m_pDY05->oInspectStage2Up = FALSE; m_pDY05->oInspectStage2Down = TRUE;	// Down
			m_pDY05->oInspectStage3Up = FALSE; m_pDY05->oInspectStage3Down = TRUE;	// Down
			g_objAJinAXL.Write_Output(5);
			m_niInspStageCase++; m_tiInspStageLoop.Set_LoopTime(5000);
		}
		break;
	case 24:	// Stage2 X Move to Load Position
		if (m_pDX05->iInspectStage1Up && !m_pDX05->iInspectStage1Down &&
			!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down &&
			!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down) {	// Up(1), Down(2,3)
			if (m_pDX05->iInspectStage2Fwd || !m_pDX05->iInspectStage2Bwd) break;	// 인터락
			g_objCommon.Move_Position(AX_INSPECT_STAGE2_X, 0);	// Load
			m_niInspStageCase++; m_tiInspStageLoop.Set_LoopTime(10000);
		}
		break;
	case 25:	// Check Stage2 X Load Position
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0)) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Inspect Stage Complete");
			m_niInspStageCase = 90; m_tiInspStageLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 8. (Error : 1800)
	if (m_tiInspStageLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1800 + m_niInspStageCase);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 9. (Error : 1900)
BOOL CSequenceInit::Initial_BufferStage()
{
	switch (m_niBuffStageCase) {
	case 0:		// Wait for Load Picker && Index
		if (m_niBtm2PickCase > 10 && m_niSortPickCase > 10) m_niBuffStageCase++;
		return TRUE;

	case 1:		// Buffer Stage Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Buffer Stage Start");
		m_niBuffStageCase++; m_tiBuffStageLoop.Set_LoopTime(5000);
		break;
	case 2:		// Stage1/2 Vacuum Off
// 		m_pDY09->oBufferStage1Vacuum = FALSE; m_pDY09->oBufferStage2Vacuum = FALSE;
// 		g_objAJinAXL.Write_Output(9);
		m_niBuffStageCase++; m_tiBuffStageLoop.Set_LoopTime(5000);
		break;
	case 3:		// Stage1/2 Up or Down
// 		if (!m_pDX09->iBufferStage1Vacuum && !m_pDX09->iBufferStage2Vacuum) {
			m_niBuffStageCase = 11; m_tiBuffStageLoop.Set_LoopTime(5000);
// 		}
		break;

	case 11:	// Stage1/2 Up or Down
		if ((m_pDX09->iBufferStage1Up && !m_pDX09->iBufferStage1Down && m_pDX09->iBufferStage2Up && !m_pDX09->iBufferStage2Down) ||	// All Up
			(!m_pDX09->iBufferStage1Up && m_pDX09->iBufferStage1Down && !m_pDX09->iBufferStage2Up && m_pDX09->iBufferStage2Down)) {	// All Down
			m_pDY09->oBufferStage1Up = TRUE; m_pDY09->oBufferStage1Down = FALSE;	// Stage1 Up
			m_pDY09->oBufferStage2Up = FALSE; m_pDY09->oBufferStage2Down = TRUE;	// Stage2 Down
			g_objAJinAXL.Write_Output(9);
			m_niBuffStageCase++; m_tiBuffStageLoop.Set_LoopTime(5000);
		} else {
			m_niBuffStageCase++; m_tiBuffStageLoop.Set_LoopTime(5000);
		}
		break;
	case 12:	// Stage1/2 Y Home Search
		if ((m_pDX09->iBufferStage1Up && !m_pDX09->iBufferStage1Down && !m_pDX09->iBufferStage2Up && m_pDX09->iBufferStage2Down) ||	// 1 Up & 2 Down
			(!m_pDX09->iBufferStage1Up && m_pDX09->iBufferStage1Down && m_pDX09->iBufferStage2Up && !m_pDX09->iBufferStage2Down)) {	// 1 Down & 2 Up
			g_objAJinAXL.Home_Search(AX_BUFFER_STAGE1_Y);
			g_objAJinAXL.Home_Search(AX_BUFFER_STAGE2_Y);
			m_niBuffStageCase++; m_tiBuffStageLoop.Set_LoopTime(20000);
		}
		break;
	case 13:	// Move (Y1 : Btm2 Picker L1, Y2 : Sort Picker1)
		if (g_objAJinAXL.Is_Home(AX_BUFFER_STAGE1_Y) && g_objAJinAXL.Is_Home(AX_BUFFER_STAGE2_Y)) {
			if (!m_tiBuffStageLoop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 0);	// Btm Picker L1
			g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 3);	// Sort Picker2
			m_niBuffStageCase++; m_tiBuffStageLoop.Set_LoopTime(10000);
		}
		break;
	case 14:	// Stage1 Up and Stage2 Down
		if (g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 3)) {
			m_pDY09->oBufferStage1Up = TRUE; m_pDY09->oBufferStage1Down = FALSE;	// 1 Up
			m_pDY09->oBufferStage2Up = FALSE; m_pDY09->oBufferStage2Down = TRUE;	// 2 Down
			g_objAJinAXL.Write_Output(9);
			m_niBuffStageCase++; m_tiBuffStageLoop.Set_LoopTime(5000);
		}
		break;
	case 15:	// Stage1 Rotate, 2 Normal
		if (m_pDX09->iBufferStage1Up && !m_pDX09->iBufferStage1Down && !m_pDX09->iBufferStage2Up && m_pDX09->iBufferStage2Down) {
// 			if (gData.bR04A || gData.bR13A) {
// 				m_pDY09->oBufferStage1Rotate = TRUE;
// 				m_pDY09->oBufferStage2Rotate = TRUE;
// 			} else {
				m_pDY09->oBufferStage1Rotate = FALSE;
				m_pDY09->oBufferStage2Rotate = FALSE;
// 			}
			g_objAJinAXL.Write_Output(9);
			m_niBuffStageCase++; m_tiBuffStageLoop.Set_LoopTime(5000);
		}
		break;
	case 16:	// Check Stage1 Rotate, 2 Normal
// 		if (((gData.bR04A || gData.bR13A) && !m_pDX09->iBufferStage1Normal && m_pDX09->iBufferStage1Rotate && !m_pDX09->iBufferStage2Normal && m_pDX09->iBufferStage2Rotate) ||
// 			((!gData.bR04A && !gData.bR13A) && m_pDX09->iBufferStage1Normal && !m_pDX09->iBufferStage1Rotate && m_pDX09->iBufferStage2Normal && !m_pDX09->iBufferStage2Rotate)) {
		if (m_pDX09->iBufferStage1Normal && !m_pDX09->iBufferStage1Rotate && m_pDX09->iBufferStage2Normal && !m_pDX09->iBufferStage2Rotate) {

			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Buffer Stage Complete");
			m_niBuffStageCase = 90; m_tiBuffStageLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 9. (Error : 1900)
	if (m_tiBuffStageLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1900 + m_niBuffStageCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 10. (Error : 2000)
BOOL CSequenceInit::Initial_GoodStage()
{
	switch (m_niGoodStageCase) {
	case 0:		// Wait
		if (m_niSortPickCase > 10) m_niGoodStageCase++;
		return TRUE;

	case 1:		// Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Good Stage Start");
		m_niGoodStageCase++; m_tiGoodStageLoop.Set_LoopTime(5000);
		break;
	case 2:		// Stage1 Master/Slave In
		if (m_pDX12->iGoodPortSlideClose && !m_pDX12->iGoodPortSlideOpen) {
			m_pDY12->oGoodStage1MasterIn = TRUE;
			m_pDY12->oGoodStage1SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_niGoodStageCase++; m_tiGoodStageLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Stage2 Master/Slave In
		if (g_objCommon.Get_GoodTray1MasterSlaveIn()) {
			m_pDY12->oGoodStage2MasterIn = TRUE;
			m_pDY12->oGoodStage2SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_niGoodStageCase++; m_tiGoodStageLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Check Stage2 Master /Slave In
		if (g_objCommon.Get_GoodTray2MasterSlaveIn()) {
			m_niGoodStageCase = 11; m_tiGoodStageLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Stage1/2 Z Move Up or Down Position
		if (g_objAJinAXL.Get_Position(AX_GOOD_STAGE1_Z) < g_objAJinAXL.Get_Position(AX_GOOD_STAGE2_Z)) {
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 0);	// Down
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 1);	// Up
		} else {
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 1);	// Up
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 0);	// Down
		}
		m_niGoodStageCase++; m_tiGoodStageLoop.Set_LoopTime(10000);
		break;
	case 12:	// Stage1/2 X Home Search
		if ((g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1)) ||
			(g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 0))) {
			g_objAJinAXL.Home_Search(AX_GOOD_STAGE1_Y);
			g_objAJinAXL.Home_Search(AX_GOOD_STAGE2_Y);
			m_niGoodStageCase++; m_tiGoodStageLoop.Set_LoopTime(20000);
		}
		break;
	case 13:	// Stage X Move to Load Position and Stage X Move to Unload Position
		if (g_objAJinAXL.Is_Home(AX_GOOD_STAGE1_Y) && g_objAJinAXL.Is_Home(AX_GOOD_STAGE2_Y)) {
			if (!m_tiGoodStageLoop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Y, 0);	// Load
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Y, 3);	// Unload
			m_niGoodStageCase++; m_tiGoodStageLoop.Set_LoopTime(10000);
		}
		break;
	case 14:	// Stage1/2 Z Home Search
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 3)) {
			g_objAJinAXL.Home_Search(AX_GOOD_STAGE1_Z);
			g_objAJinAXL.Home_Search(AX_GOOD_STAGE2_Z);
			m_niGoodStageCase++; m_tiGoodStageLoop.Set_LoopTime(10000);
		}
		break;
	case 15:	// Stage1 Z Move to Up Position and Stage2 Z Move to Down Position
		if (g_objAJinAXL.Is_Home(AX_GOOD_STAGE1_Z) && g_objAJinAXL.Is_Home(AX_GOOD_STAGE2_Z)) {
			if (!m_tiGoodStageLoop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 1);	// Up
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 0);	// Down
			m_niGoodStageCase++; m_tiGoodStageLoop.Set_LoopTime(10000);
		}
		break;
	case 16:	// Stage1 Master/Slave Out
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 0)) {
			m_pDY12->oGoodStage1MasterIn = FALSE;
			m_pDY12->oGoodStage1SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_niGoodStageCase++; m_tiGoodStageLoop.Set_LoopTime(5000);
		}
		break;
	case 17:	// Stage2 Master/Slave Out
		if (g_objCommon.Get_GoodTray1MasterSlaveOut()) {
			m_pDY12->oGoodStage2MasterIn = FALSE;
			m_pDY12->oGoodStage2SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_niGoodStageCase++; m_tiGoodStageLoop.Set_LoopTime(5000);
		}
		break;
	case 18:	// Port Slide Unlock
		if (g_objCommon.Get_GoodTray2MasterSlaveOut()) {
			m_pDY12->oGoodPortSlideLock = FALSE; m_pDY12->oGoodPortSlideUnlock = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_niGoodStageCase++; m_tiGoodStageLoop.Set_LoopTime(5000);
		}
		break;
	case 19:	// Check Slide Unlock
		if (!m_pDX12->iGoodPortSlideLock && m_pDX12->iGoodPortSlideUnlock) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Good Stage Complete");
			m_niGoodStageCase = 90; m_tiGoodStageLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 10. (Error : 2000)
	if (m_tiGoodStageLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(2000 + m_niGoodStageCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 11. (Error : 2100)
BOOL CSequenceInit::Initial_NgStage()
{
	switch (m_niNgStageCase) {
	case 0:		// Wait for Tray Picker
		if (m_niSortPickCase > 10) m_niNgStageCase++;
		return TRUE;

	case 1:		// N-NG Tray Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - NG Stage Start");
		m_niNgStageCase++; m_tiNgStageLoop.Set_LoopTime(5000);
		break;
	case 2:		// Stage Y Home Search
		if (m_pDX09->iNgPortSlideClose && !m_pDX09->iNgPortSlideOpen) {
			g_objAJinAXL.Home_Search(AX_NG_STAGE_Y);
			m_niNgStageCase++; m_tiNgStageLoop.Set_LoopTime(20000);
		}
		break;
	case 3:		// Stage Y Btm1 Picker L1
		if (g_objAJinAXL.Is_Home(AX_NG_STAGE_Y)) {
			if (!m_tiNgStageLoop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_NG_STAGE_Y, 0);	// L1
			m_niNgStageCase++; m_tiNgStageLoop.Set_LoopTime(10000);
		}
		break;
	case 4:		// Port Slide Unlock
		if (g_objCommon.Check_Position(AX_NG_STAGE_Y, 0)) {
			m_pDY09->oNgPortSlideLock = FALSE; m_pDY09->oNgPortSlideUnlock = TRUE;
			g_objAJinAXL.Write_Output(9);
			m_niNgStageCase++; m_tiNgStageLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Check Port Slide Unlock
		if (!m_pDX09->iNgPortSlideLock && m_pDX09->iNgPortSlideUnlock) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - NG Tray Complete");
			m_niNgStageCase = 90; m_tiNgStageLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 11. (Error : 2100)
	if (m_tiNgStageLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(2100 + m_niNgStageCase);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 12. (Error : 2200)
BOOL CSequenceInit::Initial_EmptyTrans1()
{
	switch (m_niEmpTrans1Case) {
	case 0:		// Wait
		return TRUE;

	case 1:		// Empty Trans Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Empty Trans1 Start");
		m_niEmpTrans1Case++; m_tiEmpTrans1Loop.Set_LoopTime(5000);
		break;
	case 2:		// Trans1 Master/Slave Out
		if (!m_pDX13->iEmptyTrans1Exist) {
			m_pDY13->oEmptyTrans1MasterIn = FALSE; m_pDY13->oEmptyTrans1MasterOut = TRUE;
			m_pDY13->oEmptyTrans1SlaveIn = FALSE; m_pDY13->oEmptyTrans1SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(13);
			m_niEmpTrans1Case++; m_tiEmpTrans1Loop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Trans1 Up
		if (g_objCommon.Get_EmptyTrayXMasterSlaveOut()) {
			m_pDY13->oEmptyTrans1Up = TRUE; m_pDY13->oEmptyTrans1Down = FALSE;
			g_objAJinAXL.Write_Output(13);
			m_niEmpTrans1Case++; m_tiEmpTrans1Loop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Trans1 X Home
		if (m_pDX13->iEmptyTrans1Up && !m_pDX13->iEmptyTrans1Down) {
			g_objAJinAXL.Home_Search(AX_EMPTY_TRANS1_X);
			m_niEmpTrans1Case++; m_tiEmpTrans1Loop.Set_LoopTime(60000);
		}
		break;
	case 5:		// Trans1 X Move to Load Position
		if (g_objAJinAXL.Is_Home(AX_EMPTY_TRANS1_X)) {
			if (!m_tiEmpTrans1Loop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_EMPTY_TRANS1_X, 0);
			m_niEmpTrans1Case++; m_tiEmpTrans1Loop.Set_LoopTime(10000);
		}
		break;
	case 6:		// Check Trans1 X Load Position
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 0)) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Empty Trans1 Complete");
			m_niEmpTrans1Case = 90; m_tiEmpTrans1Loop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 12. (Error : 2200)
	if (m_tiEmpTrans1Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(2200 + m_niEmpTrans1Case);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 13. (Error : 2300)
BOOL CSequenceInit::Initial_EmptyTrans2()
{
	switch (m_niEmpTrans2Case) {
	case 0:		// Wait
		return TRUE;

	case 1:		// Empty Trans2 Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Empty Trans2 Start");
		m_niEmpTrans2Case++; m_tiEmpTrans2Loop.Set_LoopTime(5000);
		break;
	case 2:		// Port Z Home
#ifdef EDITION_2ND
		if (m_pDX01->iEmptyPortSlideClose && !m_pDX01->iEmptyPortSlideOpen)
#else
		if (m_pDX07->iEmptyPortSlideClose && !m_pDX07->iEmptyPortSlideOpen)
#endif
		{
			g_objAJinAXL.Home_Search(AX_EMPTY_PORT_Z);
			m_niEmpTrans2Case++; m_tiEmpTrans2Loop.Set_LoopTime(50000);
		}
		break;
	case 3:		// Port Z Move to Ready Position
		if (g_objAJinAXL.Is_Home(AX_EMPTY_PORT_Z)) {
			if (!m_tiEmpTrans2Loop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 0);
			m_niEmpTrans2Case++; m_tiEmpTrans2Loop.Set_LoopTime(10000);
		}
		break;
	case 4:		// Check Port Z Load Position
		if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 0)) {
			m_niEmpTrans2Case = 10; m_tiEmpTrans2Loop.Set_LoopTime(5000);
		}
		break;

	case 10:	// Wait for Btm1 Picker X Home
		if (m_niBtm2PickCase > 20) m_niEmpTrans2Case++;
		return TRUE;

	case 11:	// Trans2 Master/Slave Out
		if (!m_pDX13->iEmptyTrans2Exist/* && !m_pDX12->iGoodTrayBufferBottom*/) {
			m_pDY12->oGoodTrayBuffSupport1In = TRUE; m_pDY12->oGoodTrayBuffSupport2In = TRUE;
			g_objAJinAXL.Write_Output(12);

			m_pDY13->oEmptyTrans2MasterIn = FALSE; m_pDY13->oEmptyTrans2MasterOut = TRUE;
			m_pDY13->oEmptyTrans2SlaveIn = FALSE; m_pDY13->oEmptyTrans2SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(13);
			m_niEmpTrans2Case++; m_tiEmpTrans2Loop.Set_LoopTime(5000);
		}
		break;
	case 12:	// Trans2 Up
		if (g_objCommon.Get_EmptyTrayYMasterSlaveOut()) {
			m_pDY13->oEmptyTrans2Up = TRUE; m_pDY13->oEmptyTrans2Down = FALSE;
			g_objAJinAXL.Write_Output(13);
			m_niEmpTrans2Case++; m_tiEmpTrans2Loop.Set_LoopTime(5000);
		}
		break;
	case 13:	// Trans2 Y Home
		if (m_pDX13->iEmptyTrans2Up && !m_pDX13->iEmptyTrans2Down) {
			g_objAJinAXL.Home_Search(AX_EMPTY_TRANS2_Y);
			m_niEmpTrans2Case++; m_tiEmpTrans2Loop.Set_LoopTime(20000);
		}
		break;
	case 14:	// Trans2 Y Move to Load Position
		if (g_objAJinAXL.Is_Home(AX_EMPTY_TRANS2_Y)) {
			if (!m_tiEmpTrans2Loop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_EMPTY_TRANS2_Y, 0);
			m_niEmpTrans2Case++; m_tiEmpTrans2Loop.Set_LoopTime(10000);
		}
		break;
	case 15:	// Check Trans2 Y Load Position
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0)) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Empty Trans2 Complete");
			m_niEmpTrans2Case = 90; m_tiEmpTrans2Loop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 13. (Error : 2300)
	if (m_tiEmpTrans2Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(2300 + m_niEmpTrans2Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL CSequenceInit::Initial_Simulation()
{
#ifdef AJIN_BOARD_USE
	return TRUE;
#endif

	Sleep(SIM_WAITTIMES);

	if (m_niEmpTrans2Case == 2) 
	{ 
		m_pDX01->iEmptyPortSlideClose =TRUE ; m_pDX01->iEmptyPortSlideOpen = FALSE;
	}
	
	if(m_niLoadStageCase == 2)
	{ 
		m_pDX00->iLoadPort1SlideClose=TRUE; m_pDX00->iLoadPort1SlideOpen = FALSE; 
		m_pDX00->iLoadPort2SlideClose = TRUE; m_pDX00->iLoadPort2SlideOpen = FALSE;
	}

	if(m_niGoodStageCase == 2)
	{
		m_pDX12->iGoodPortSlideClose = TRUE;
		m_pDX12->iGoodPortSlideOpen = FALSE;
	}

	if(m_niNgStageCase == 2)
	{
		m_pDX09->iNgPortSlideClose = TRUE;
		m_pDX09->iNgPortSlideOpen = FALSE;
	}

	if(m_niAngleStageCase == 5)
	{
		m_pDX02->iAngleStage1Exist = FALSE;
		m_pDX02->iAngleStage2Exist = FALSE;

		g_objAJinAXL.Get_pStatus(AX_ANGLE_STAGE1_Y)->dPos =  m_pMoveData->dAngleStage1Y[0];
		g_objAJinAXL.Get_pStatus(AX_ANGLE_STAGE2_Y)->dPos =  m_pMoveData->dAngleStage2Y[4];
		g_objAJinAXL.Get_pStatus(AX_ANGLE_STAGE1_Z)->dPos =  m_pMoveData->dAngleStage1Z[0];
		g_objAJinAXL.Get_pStatus(AX_ANGLE_STAGE2_Z)->dPos =  m_pMoveData->dAngleStage2Z[1];
	}
	

	if(m_niInspStageCase == 6 || m_niInspStageCase == 11)
	{
		//1번스테이지 Up 상태로 가정 
		m_pDX05->iInspectStage1Up = TRUE; m_pDX05->iInspectStage1Down = FALSE;
	}

	if(m_niBuffStageCase == 12)
	{
		// 1: up 2: down 으로 가정 
		m_pDX09->iBufferStage1Up = TRUE; m_pDX09->iBufferStage1Down = FALSE;
		m_pDX09->iBufferStage2Up = FALSE; m_pDX09->iBufferStage2Down = TRUE;
	}
	
	return TRUE;
}
