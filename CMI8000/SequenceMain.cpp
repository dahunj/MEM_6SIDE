// SequenceMain.cpp : 구현 파일
//
#include "stdafx.h"
#include "CMI8000.h"
#include "CMI8000Dlg.h"
#include "SequenceMain.h"
#include <math.h>

#include "LogFile.h"
#include "Common.h"
#include "Inspector.h"
#include "MESInterface.h"
#include "Dispatcher.h"
#include "CapAttach.h"
#include "WorkDlg.h"

CSequenceMain g_objSequenceMain;

///////////////////////////////////////////////////////////////////////////////

CSequenceMain::CSequenceMain()
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
	m_pRosData = g_objDataManager.Get_pRosData();

	m_strLog = "";

	m_bThreadMainRun = FALSE;
	m_pThreadMainRun = NULL;

	m_nBtm2PickMultiCnt = 0;
	m_nSortPick1MultiCnt = 0;
	m_nSortPick2MultiCnt = 0;

	iGoodTrayBufferCount = 0;
	Reset_MainRunCase();

	gData.dEmptyPort_Z_Limit = 300;
}

CSequenceMain::~CSequenceMain()
{
	if (m_pThreadMainRun) {
		m_bThreadMainRun = FALSE;
		WaitForSingleObject(m_pThreadMainRun->m_hThread, INFINITE);
	}
}

void CSequenceMain::Reset_MainRunCase()
{
	m_nLoadTrayCase			= 0;		//  1. (Error : 3100)
	m_nLoadPickCase			= 0;		//  2. (Error : 3200)
	m_nVisAngleCase			= 0;		//  3. (Error : 3300)
	m_nAngleTray1Case		= 0;		//  4. (Error : 3400)
	m_nAngleTray2Case		= 50;		//  5. (Error : 3500)
	m_nBtm1PickCase			= 0;		//  6. (Error : 3600)
	m_nInspect1Case			= 0;		//  7. (Error : 3700) Center
	m_nInspect2Case			= 70;		//  8. (Error : 3800) Front
	m_nInspect3Case			= 50;		//  9. (Error : 3900) Rear
	m_nBtm2PickCase			= 0;		// 10. (Error : 4000)
	m_nBuffTray1Case		= 0;		// 11. (Error : 4100)
	m_nBuffTray2Case		= 50;		// 12. (Error : 4200)
	m_nSortPick1Case		= 0;		// 13. (Error : 4300) Rear
	m_nSortPick2Case		= 0;		// 14. (Error : 4400) Front
	m_nGoodTray1Case		= 0;		// 15. (Error : 4500)	
	m_nGoodTray2Case		= 50;		// 16. (Error : 4600) 
	m_nNgTrayCase			= 0;		// 17. (Error : 4700) 
	m_nEmptyTrayXCase		= 0;		// 18. (Error : 4800)
	m_nEmptyTrayElCase		= 0;		// 19. (Error : 4900)
	m_nEmptyTrayYCase		= 0;		// 20. (Error : 5000)
}

int *CSequenceMain::Get_pMainRunCase()
{
	static int nCase[20] = { 0 };

	nCase[0]  = m_nLoadTrayCase;		//  1. (Error : 3100)
	nCase[1]  = m_nLoadPickCase;		//  2. (Error : 3200)
	nCase[2]  = m_nVisAngleCase;		//  3. (Error : 3300)
	nCase[3]  = m_nAngleTray1Case;		//  4. (Error : 3400)
	nCase[4]  = m_nAngleTray2Case;		//  5. (Error : 3500)
	nCase[5]  = m_nBtm1PickCase;		//  6. (Error : 3600)
	nCase[6]  = m_nInspect1Case;		//  7. (Error : 3700)
	nCase[7]  = m_nInspect2Case;		//  8. (Error : 3800)
	nCase[8]  = m_nInspect3Case;		//  9. (Error : 3900)
	nCase[9]  = m_nBtm2PickCase;		// 10. (Error : 4000)
	nCase[10] = m_nBuffTray1Case;		// 11. (Error : 4100)
	nCase[11] = m_nBuffTray2Case;		// 12. (Error : 4200)
	nCase[12] = m_nSortPick1Case;		// 13. (Error : 4300)
	nCase[13] = m_nSortPick2Case;		// 14. (Error : 4400)
	nCase[14] = m_nGoodTray1Case;		// 15. (Error : 4500)
	nCase[15] = m_nGoodTray2Case;		// 16. (Error : 4600)
	nCase[16] = m_nNgTrayCase;			// 17. (Error : 4700)
	nCase[17] = m_nEmptyTrayXCase;		// 18. (Error : 4800)
	nCase[18] = m_nEmptyTrayElCase;		// 19. (Error : 4900)
	nCase[19] = m_nEmptyTrayYCase;		// 20. (Error : 5000)

	return nCase;
}

int CSequenceMain::Get_MainRunCase(int nRun)
{
	if		(nRun == AUTO_LOAD_TRAY)		return m_nLoadTrayCase;			//  1. (Error : 3100)
	else if (nRun == AUTO_LOAD_PICKER)		return m_nLoadPickCase;			//  2. (Error : 3200)
	else if (nRun == AUTO_VISION_ANGLE)		return m_nVisAngleCase;			//  3. (Error : 3300)
	else if (nRun == AUTO_ANGLE_TRAY1)		return m_nAngleTray1Case;		//  4. (Error : 3400)
	else if (nRun == AUTO_ANGLE_TRAY2)		return m_nAngleTray2Case;		//  5. (Error : 3500)
	else if (nRun == AUTO_VISION_BTM1)		return m_nBtm1PickCase;			//  6. (Error : 3600)
	else if (nRun == AUTO_INSPECTION1)		return m_nInspect1Case;			//  7. (Error : 3700)
	else if (nRun == AUTO_INSPECTION2)		return m_nInspect2Case;			//  8. (Error : 3800)
	else if (nRun == AUTO_INSPECTION3)		return m_nInspect3Case;			//  9. (Error : 3900)
	else if (nRun == AUTO_VISION_BTM2)		return m_nBtm2PickCase;			// 10. (Error : 4000)
	else if (nRun == AUTO_BUFFER1)			return m_nBuffTray1Case;		// 11. (Error : 4100)
	else if (nRun == AUTO_BUFFER2)			return m_nBuffTray2Case;		// 12. (Error : 4200)
	else if (nRun == AUTO_SORT_PICKER1)		return m_nSortPick1Case;		// 13. (Error : 4300)
	else if (nRun == AUTO_SORT_PICKER2)		return m_nSortPick2Case;		// 14. (Error : 4400)
	else if (nRun == AUTO_GOOD_TRAY1)		return m_nGoodTray1Case;		// 15. (Error : 4500)
	else if (nRun == AUTO_GOOD_TRAY2)		return m_nGoodTray2Case;		// 16. (Error : 4600)
	else if (nRun == AUTO_NG_TRAY)			return m_nNgTrayCase;			// 17. (Error : 4700)
	else if (nRun == AUTO_EMPTY_TRAY_X)		return m_nEmptyTrayXCase;		// 18. (Error : 4800)
	else if (nRun == AUTO_EMPTY_TRAY_EL)	return m_nEmptyTrayElCase;		// 19. (Error : 4900)
	else if (nRun == AUTO_EMPTY_TRAY_Y)		return m_nEmptyTrayYCase;		// 20. (Error : 5000)

	return 0;
}

void CSequenceMain::Set_MainRunCase(int nRun, int nCase)
{
	if		(nRun == AUTO_LOAD_TRAY)		m_nLoadTrayCase			= nCase;	//  1. (Error : 3100)
	else if (nRun == AUTO_LOAD_PICKER)		m_nLoadPickCase			= nCase;	//  2. (Error : 3200)
	else if (nRun == AUTO_VISION_ANGLE)		m_nVisAngleCase			= nCase;	//  3. (Error : 3300)
	else if (nRun == AUTO_ANGLE_TRAY1)		m_nAngleTray1Case		= nCase;	//  4. (Error : 3400)
	else if (nRun == AUTO_ANGLE_TRAY2)		m_nAngleTray2Case		= nCase;	//  5. (Error : 3500)
	else if (nRun == AUTO_VISION_BTM1)		m_nBtm1PickCase			= nCase;	//  6. (Error : 3600)
	else if (nRun == AUTO_INSPECTION1)		m_nInspect1Case			= nCase;	//  7. (Error : 3700)
	else if (nRun == AUTO_INSPECTION2)		m_nInspect2Case			= nCase;	//  8. (Error : 3800)
	else if (nRun == AUTO_INSPECTION3)		m_nInspect3Case			= nCase;	//  9. (Error : 3900)
	else if (nRun == AUTO_VISION_BTM2)		m_nBtm2PickCase			= nCase;	// 10. (Error : 4000)
	else if (nRun == AUTO_BUFFER1)			m_nBuffTray1Case		= nCase;	// 11. (Error : 4100)
	else if (nRun == AUTO_BUFFER2)			m_nBuffTray2Case		= nCase;	// 12. (Error : 4200)
	else if (nRun == AUTO_SORT_PICKER1)		m_nSortPick1Case		= nCase;	// 13. (Error : 4300)
	else if (nRun == AUTO_SORT_PICKER2)		m_nSortPick2Case		= nCase;	// 14. (Error : 4400)
	else if (nRun == AUTO_GOOD_TRAY1)		m_nGoodTray1Case		= nCase;	// 15. (Error : 4500)
	else if (nRun == AUTO_GOOD_TRAY2)		m_nGoodTray2Case		= nCase;	// 16. (Error : 4600)
	else if (nRun == AUTO_NG_TRAY)			m_nNgTrayCase			= nCase;	// 17. (Error : 4700)
	else if (nRun == AUTO_EMPTY_TRAY_X)		m_nEmptyTrayXCase		= nCase;	// 18. (Error : 4800)
	else if (nRun == AUTO_EMPTY_TRAY_EL)	m_nEmptyTrayElCase		= nCase;	// 19. (Error : 4900)
	else if (nRun == AUTO_EMPTY_TRAY_Y)		m_nEmptyTrayYCase		= nCase;	// 20. (Error : 5000)	
}

void CSequenceMain::Set_MainRunLoop(int nRun, int nLoop)
{
	if		(nRun == AUTO_LOAD_TRAY)		m_tLoadTrayLoop.Set_LoopTime(nLoop);	//  1. (Error : 3100)
	else if (nRun == AUTO_LOAD_PICKER)		m_tLoadPickLoop.Set_LoopTime(nLoop);	//  2. (Error : 3200)
	else if (nRun == AUTO_VISION_ANGLE)		m_tVisAngleLoop.Set_LoopTime(nLoop);	//  3. (Error : 3300)
	else if (nRun == AUTO_ANGLE_TRAY1)		m_tAngleTray1Loop.Set_LoopTime(nLoop);	//  4. (Error : 3400)
	else if (nRun == AUTO_ANGLE_TRAY2)		m_tAngleTray2Loop.Set_LoopTime(nLoop);	//  5. (Error : 3500)
	else if (nRun == AUTO_VISION_BTM1)		m_tBtm1PickLoop.Set_LoopTime(nLoop);	//  6. (Error : 3600)
	else if (nRun == AUTO_INSPECTION1)		m_tInspect1Loop.Set_LoopTime(nLoop);	//  7. (Error : 3700)
	else if (nRun == AUTO_INSPECTION2)		m_tInspect2Loop.Set_LoopTime(nLoop);	//  8. (Error : 3800)
	else if (nRun == AUTO_INSPECTION3)		m_tInspect3Loop.Set_LoopTime(nLoop);	//  9. (Error : 3900)
	else if (nRun == AUTO_VISION_BTM2)		m_tBtm2PickLoop.Set_LoopTime(nLoop);	// 10. (Error : 4000)
	else if (nRun == AUTO_BUFFER1)			m_tBuffTray1Loop.Set_LoopTime(nLoop);	// 11. (Error : 4100)
	else if (nRun == AUTO_BUFFER2)			m_tBuffTray2Loop.Set_LoopTime(nLoop);	// 12. (Error : 4200)
	else if (nRun == AUTO_SORT_PICKER1)		m_tSortPick1Loop.Set_LoopTime(nLoop);	// 13. (Error : 4300)
	else if (nRun == AUTO_SORT_PICKER2)		m_tSortPick2Loop.Set_LoopTime(nLoop);	// 14. (Error : 4400)
	else if (nRun == AUTO_GOOD_TRAY1)		m_tGoodTray1Loop.Set_LoopTime(nLoop);	// 15. (Error : 4500)
	else if (nRun == AUTO_GOOD_TRAY2)		m_tGoodTray2Loop.Set_LoopTime(nLoop);	// 16. (Error : 4600)
	else if (nRun == AUTO_NG_TRAY)			m_tNgTrayLoop.Set_LoopTime(nLoop);		// 17. (Error : 4700)
	else if (nRun == AUTO_EMPTY_TRAY_X)		m_tEmptyTrayXLoop.Set_LoopTime(nLoop);	// 18. (Error : 4800)
	else if (nRun == AUTO_EMPTY_TRAY_EL)	m_tEmptyTrayElLoop.Set_LoopTime(nLoop);	// 19. (Error : 4900)
	else if (nRun == AUTO_EMPTY_TRAY_Y)		m_tEmptyTrayYLoop.Set_LoopTime(nLoop);	// 20. (Error : 5000)
}

BOOL CSequenceMain::Get_IsAutoRun()
{
	int *pCase = Get_pMainRunCase();
	for (int i = 0; i < AUTO_COUNT; i++) {
		if (i == AUTO_ANGLE_TRAY1 && m_nAngleTray1Case == 50) continue;
		if (i == AUTO_ANGLE_TRAY2 && m_nAngleTray2Case == 50) continue;
		if (i == AUTO_INSPECTION1 && (m_nInspect1Case == 50 || m_nInspect1Case == 70)) continue;	// Center
		if (i == AUTO_INSPECTION2 && (m_nInspect2Case == 50 || m_nInspect2Case == 70)) continue;	// Front
		if (i == AUTO_INSPECTION3 && (m_nInspect3Case == 50 || m_nInspect3Case == 70)) continue;	// Rear
		if (i == AUTO_BUFFER1 && m_nBuffTray1Case == 50) continue;
		if (i == AUTO_BUFFER2 && m_nBuffTray2Case == 50) continue;
		if (i == AUTO_GOOD_TRAY1 && m_nGoodTray1Case == 50) continue;
		if (i == AUTO_GOOD_TRAY2 && m_nGoodTray2Case == 50) continue;
		if (*(pCase + i) > 0) return TRUE;
	}

	if (!Check_Btm1PickerEmpty()) return TRUE;
	if (!Check_InspetionEmpty(1)) return TRUE;
	if (!Check_InspetionEmpty(2)) return TRUE;
	if (!Check_InspetionEmpty(3)) return TRUE;
	if (!Check_Btm2PickerEmpty()) return TRUE;
	if (!Check_SortPickerEmpty(1)) return TRUE;
	if (!Check_SortPickerEmpty(2)) return TRUE;

	return FALSE;
}

void CSequenceMain::Begin_MainRunThread()
{
	Run_Replay();

	if (m_nLoadTrayCase == 0)	m_nLoadTrayCase = 1;

	if (m_nBuffTray1Case == 0)	m_nBuffTray1Case = 1;
	if (m_nBuffTray2Case == 0)	m_nBuffTray2Case = 1;

	if (m_nGoodTray1Case == 0)	m_nGoodTray1Case = 1;
	if (m_nGoodTray2Case == 0)	m_nGoodTray2Case = 1;

	if (m_nNgTrayCase == 0)		m_nNgTrayCase = 1;

	if (m_nEmptyTrayXCase == 0)		m_nEmptyTrayXCase = 1;
	if (m_nEmptyTrayYCase == 0)		m_nEmptyTrayYCase = 1;
	if (m_nEmptyTrayElCase == 0)	m_nEmptyTrayElCase = 1;

	g_objInspector.Set_StatusUpdate(INSPECTOR_ALL, 1);
	g_objDispatcher.Set_StatusUpdate(1);
	g_objCapAttach.Set_StatusUpdate(1);

	if (m_pThreadMainRun) End_MainRunThread();
	m_bThreadMainRun = TRUE;
	m_pThreadMainRun = AfxBeginThread(Thread_MainRun, NULL);
}

void CSequenceMain::End_MainRunThread()
{
	if (m_pThreadMainRun) {
		m_bThreadMainRun = FALSE;
		WaitForSingleObject(m_pThreadMainRun->m_hThread, INFINITE);
	}
}

void CSequenceMain::Run_Replay()
{
	for (int i = 0; i < AUTO_COUNT; i++) Set_MainRunLoop(i, 5000);

	if (m_nVisAngleCase == 15) m_tVisAngleLoop.Set_LoopTime(30000);
	if (m_nBtm1PickCase == 10) m_tBtm1PickLoop.Set_LoopTime(90000);
	if (m_nInspect1Case == 10 || m_nInspect1Case == 20)  m_tInspect1Loop.Set_LoopTime(90000);
	if (m_nInspect2Case == 10 || m_nInspect2Case == 20)  m_tInspect2Loop.Set_LoopTime(90000);
	if (m_nInspect3Case == 10 || m_nInspect3Case == 20)  m_tInspect3Loop.Set_LoopTime(90000);
	if (m_nBtm2PickCase == 10)  m_tBtm2PickLoop.Set_LoopTime(90000);

	// Empty Port Z축
	if (m_nEmptyTrayElCase == 6) {				// Slow Down
		g_objAJinAXL.Move_Absolute(AX_EMPTY_PORT_Z, m_pMoveData->dEmptyPortZ[0], 0.2);
		m_tEmptyTrayElLoop.Set_LoopTime(10000);	// 10초
	} else if (m_nEmptyTrayElCase == 21) {
		g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 1);
		m_tEmptyTrayElLoop.Set_LoopTime(10000);	// 10초
	} else if (m_nEmptyTrayElCase == 26) {
		g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 0);
		m_tEmptyTrayElLoop.Set_LoopTime(10000);	// 10초
	} else if (m_nEmptyTrayElCase == 52) {	// Ready Down
		g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 0);
		m_tEmptyTrayElLoop.Set_LoopTime(10000);	// 10초
	}

	// Empty Trans1 X축
	if (m_nEmptyTrayXCase == 9) {
		g_objCommon.Move_Position(AX_EMPTY_TRANS1_X, 3);
		m_tEmptyTrayXLoop.Set_LoopTime(90000);	// 속도 100일때 50초
	} else if (m_nEmptyTrayXCase == 12) {
		g_objCommon.Move_Position(AX_EMPTY_TRANS1_X, 1);
		m_tEmptyTrayXLoop.Set_LoopTime(90000);	// 속도 100일때 50초
	} else if (m_nEmptyTrayXCase == 21) {
		g_objCommon.Move_Position(AX_EMPTY_TRANS1_X, 0);
		m_tEmptyTrayXLoop.Set_LoopTime(90000);	// 속도 100일때 50초
	} else if (m_nEmptyTrayXCase == 13) {
		m_tEmptyTrayXLoop.Set_LoopTime(30000);	// Tray Full Alarm 상황일 수 있어 30초
	}

	// Empty Trans2 Y축
	if (m_nEmptyTrayYCase == 2) {
		m_tEmptyTrayYLoop.Set_LoopTime(60000);	// Tray 가져가기전 대기 시간 1분.
	} else if (m_nEmptyTrayYCase == 5) {
		m_tEmptyTrayYLoop.Set_LoopTime(30000);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Main Thread Function 
UINT CSequenceMain::Thread_MainRun(LPVOID lpVoid)
{
	while (g_objSequenceMain.m_bThreadMainRun) {
		if (!g_objCommon.Check_MainEmgAir()) break;
		if (!g_objCommon.Check_MainDoor(TRUE)) break;
		if (!g_objCommon.Check_PortFull()) break;
		//if (!g_objCommon.Check_PortArea(TRUE)) break;
		//if (!g_objCommon.Check_TrayFull()) break;
		if (!g_objCommon.Check_ServoOn()) break;
		if (!g_objCommon.Check_DirveAlarm()) break;
		if (!g_objCommon.Check_EndLimit()) break;
		if (!g_objCommon.Check_HomeDone()) break;
		// Cap Attach가 정지하면 AVI도 정지해준다.
		if (g_objSequenceMain.m_pEquipData->bUseInlineMode && g_objCapAttach.Is_StatusCapAttach() == 0) {
			g_dlgWork.MachineStopLog("CAP_ATTACH_STOP");	// Cap Attach 알람 또는 정지에 의한 설비 멈춤.
			g_dlgWork.PostMessage(UM_SHOW_MSG, 3, NULL);
			break;
		}

		//Vision PC HDD 용량 확인시 핸들러에서 알람 
		if(!g_objSequenceMain.Check_VisionHdd()) break;
		//3D grab fail error 
		//if (gData.b3DGrabFailErr) { gData.b3DGrabFailErr = FALSE; g_objCommon.Show_Error(6200);
		//	break;
		//}


		if (!g_objSequenceMain.LoadTray_Run()) break;			//  1. (Error : 3100)
		
		if (!g_objSequenceMain.LoadPicker_Run()) break;			//  2. (Error : 3200)
		
		if (!g_objSequenceMain.VisionAngle_Run()) break;		//  3. (Error : 3300)
		
		if (!g_objSequenceMain.AngleTray1_Run()) break;			//  4. (Error : 3400)
		if (!g_objCommon.Check_ServoOn()) break;
		if (!g_objSequenceMain.AngleTray2_Run()) break;			//  5. (Error : 3500)
		if (!g_objCommon.Check_ServoOn()) break;
		if (!g_objSequenceMain.Btm1Picker_Run()) break;			//  6. (Error : 3600)
		if (!g_objCommon.Check_ServoOn()) break;
		if (!g_objSequenceMain.Inspection1_Run()) break;		//  7. (Error : 3700) Center
		if (!g_objCommon.Check_ServoOn()) break;
		if (!g_objSequenceMain.Inspection2_Run()) break;		//  8. (Error : 3800) Front
		if (!g_objCommon.Check_ServoOn()) break;
		if (!g_objSequenceMain.Inspection3_Run()) break;		//  9. (Error : 3900) Rear
		if (!g_objCommon.Check_ServoOn()) break;
		if (!g_objSequenceMain.Btm2Picker_Run()) break;			// 10. (Error : 4000)
		
		if (!g_objSequenceMain.BufferTray1_Run()) break;		// 11. (Error : 4100)
		
		if (!g_objSequenceMain.BufferTray2_Run()) break;		// 12. (Error : 4200)

		if (!g_objSequenceMain.SortPicker1_Run()) break;		// 13. (Error : 4300) Rear
		
		if (!g_objSequenceMain.SortPicker2_Run()) break;		// 14. (Error : 4400) Front
		
		if (!g_objSequenceMain.GoodTray1_Run()) break;			// 15. (Error : 4500)
		
		if (!g_objSequenceMain.GoodTray2_Run()) break;			// 16. (Error : 4600)
		
		if (!g_objSequenceMain.NgTray_Run()) break;				// 17. (Error : 4700)
		
		if (!g_objSequenceMain.EmptyTrayX_Run()) break;			// 18. (Error : 4800)
		
		if (!g_objSequenceMain.EmptyTrayElevator_Run()) break;	// 19. (Error : 4900)
		
		if (!g_objSequenceMain.EmptyTrayY_Run()) break;			// 20. (Error : 5000)	
		

		if (!g_objSequenceMain.Run_Simulation()) break;		//  99.

		if (g_objSequenceMain.LotEnd_Run()) break;

		if (gData.bLotEndBeep) {
			if (g_objSequenceMain.m_pThreadBeep == NULL && g_objSequenceMain.m_pThreadNgFullBeep == NULL) {
				g_objSequenceMain.m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));
				gData.bLotEndBeep = FALSE;
			}
		}

		Sleep(5);
	}

	// Stop 버튼을 누른후에 마지막 프로세스 Case가 모션 이동 명령 일수 있어 이곳에도 Stop 추가. 
	g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z);		// 강제 STOP
#ifdef AJIN_BOARD_USE
	g_objInspector.Set_StatusUpdate(INSPECTOR_ALL, 0);
	g_objDispatcher.Set_StatusUpdate(0);
	g_objCapAttach.Set_StatusUpdate(0);
#endif
	g_objSequenceMain.m_bThreadMainRun = FALSE;
	g_objSequenceMain.m_pThreadMainRun = NULL;

	return 0;
}

BOOL CSequenceMain::Check_VisionHdd()
{
	if(g_objInspector.Get_HddStatus(1) == TRUE){
		g_objInspector.Set_HddStatus(1, FALSE);
		g_objCommon.Show_Error(6301);
		return FALSE;
	}
	if(g_objInspector.Get_HddStatus(2) == TRUE){
		g_objInspector.Set_HddStatus(2, FALSE);
		g_objCommon.Show_Error(6302);
		return FALSE;
	}
	if(g_objInspector.Get_HddStatus(3) == TRUE){
		g_objInspector.Set_HddStatus(3, FALSE);
		g_objCommon.Show_Error(6303);
		return FALSE;
	}
	if(g_objInspector.Get_HddStatus(4) == TRUE){
		g_objInspector.Set_HddStatus(4, FALSE);
		g_objCommon.Show_Error(6304);
		return FALSE;
	}
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// Picker Vacuum Thread Function

UINT CSequenceMain::Thread_Vacuum_B1p(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_Btm1Picker((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumB1p = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_B2p(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_Btm2Picker((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumB2p = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_Sp1(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_SortPicker1((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumSp1 = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_Sp2(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_SortPicker2((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumSp2 = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_B2p_Multi(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_Btm2PickerMulti((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumB2pMulti = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_Sp1_Multi(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_SortPicker1Multi((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumSp1Multi = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_Sp2_Multi(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_SortPicker2Multi((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumSp2Multi = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Beep(LPVOID lpVoid)
{
	g_objSequenceMain.Beep_Post(0, (int)lpVoid);
	g_objSequenceMain.m_pThreadBeep = NULL;
	return 0;
}

UINT CSequenceMain::Thread_NgFullBeep(LPVOID lpVoid)
{
	g_objSequenceMain.Beep_Post(1, (int)lpVoid);
	g_objSequenceMain.m_pThreadNgFullBeep = NULL;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

void CSequenceMain::Vacuum_Btm1Picker(int nSub)
{
	int nCount = 0;
	while (TRUE) {
		g_objCommon.Set_Btm1PickerVacOff(nSub);
		Sleep(m_pEquipData->nVacOffDelay[0]);
		
		g_objCommon.Set_Btm1PickerAirOff(nSub);
		if (++nCount >= m_pEquipData->nVacOffRepeat[0]) return;
		Sleep(m_pEquipData->nVacOffDelay[0]);
	}
}

void CSequenceMain::Vacuum_Btm2Picker(int nSub)
{
	int nCount = 0;
	while (TRUE) {
		g_objCommon.Set_Btm2PickerVacOff(nSub);
		Sleep(m_pEquipData->nVacOffDelay[1]);

		g_objCommon.Set_Btm2PickerAirOff(nSub);
		if (++nCount >= m_pEquipData->nVacOffRepeat[1]) return;
		Sleep(m_pEquipData->nVacOffDelay[1]);
	}
}

void CSequenceMain::Vacuum_SortPicker1(int nSub)
{

}

void CSequenceMain::Vacuum_SortPicker2(int nSub)
{

}

void CSequenceMain::Vacuum_Btm2PickerMulti(int nSub)
{
	int nCount = 0;
	while (TRUE) {
		g_objCommon.Set_Btm2PickerVacOffMulti(nSub, m_nBtm2PickMultiCnt, m_nBtm2PickRow);
		Sleep(m_pEquipData->nVacOffDelay[1]);

		g_objCommon.Set_Btm2PickerAirOffMulti(nSub, m_nBtm2PickMultiCnt, m_nBtm2PickRow);
		if (++nCount >= m_pEquipData->nVacOffRepeat[1]) return;
		Sleep(m_pEquipData->nVacOffDelay[1]);
	}
}

void CSequenceMain::Vacuum_SortPicker1Multi(int nSub)
{

}

void CSequenceMain::Vacuum_SortPicker2Multi(int nSub)
{
	
}

///////////////////////////////////////////////////////////////////////////////

BOOL CSequenceMain::LotEnd_Run()
{
//	if (!m_bLoadLotEnd) return FALSE;
	if (!m_bUnloadLotEnd) return FALSE;

	if (Get_IsAutoRun()) return FALSE;

	int nPx = gData.nULPNo - 1;	// 맨마지막 공정인 Good Tray Port No 활용.
	if (nPx < 0) nPx = 0;
//	Job_LotEnd(gData.nULPNo);
// 	Beep_Post(1000);
	if (gData.bCycleStop) g_dlgWork.PostMessage(UM_RESET_CYCLE_STOP, NULL, NULL);

	gMes.nLotStatus = 0;	
	gData.bMesFirstLot = FALSE;
	g_objMES.m_nMESSequence = 0;
// 	g_objMES.m_bMesErr = FALSE;
	g_objMES.m_bMesStart = FALSE;
	gData.nMesPortNo = 0;

	gData.bFirstLotStart = FALSE;

	gData.bNGTrayWait = FALSE;
	gData.bGoodTrayWait = FALSE;
	gData.bEmptyZWait = FALSE;
	gData.bLoadLampOn[0] = FALSE;
	gData.bLoadLampOn[1] = FALSE;

	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_LampFlicker_Load1(FALSE);
	pMainDlg->Set_LampFlicker_Load2(FALSE);
	pMainDlg->Set_LampFlicker_Ng(FALSE);
	pMainDlg->Set_LampFlicker_Good(FALSE);
	pMainDlg->Set_LampFlicker_Empty(FALSE);

	g_dlgWork.MachineStopLog("LOT_END");	// 뒤에 투입된 Lot이 없는 상태로 Lot End되어 설비 정지되었을때. 

	CString strMsg;
	int nSum = gLot.nGoodCount[nPx] + gLot.nNgCount[nPx];
	double dRate = (nSum == 0 ? 0.0 : gLot.nGoodCount[nPx] * 100.0 / nSum);
	strMsg.Format("Lot End ==> ID(%s), Total(%d), Good(%d), NG(%d), Rate(%0.1lf)", gLot.sLotID[nPx], nSum, gLot.nGoodCount[nPx], gLot.nNgCount[nPx], dRate);
	g_objLogFile.Save_HandlerLog(strMsg);

	strMsg.Format("Lot End.");
	g_objCommon.Show_Alarm(strMsg);

	strMsg.Format("Lot End.\n\nSpecial NG Count\n(N1:%d, N2:%d, N3:%d, N4:%d, MES:%d)",
		gLot.nSNgCount[nPx][1], gLot.nSNgCount[nPx][2], gLot.nSNgCount[nPx][3], gLot.nSNgCount[nPx][5], gLot.nSNgCount[nPx][0]);
	g_objCommon.Show_Alarm(strMsg);

	m_pEquipData->bResultTestUse = FALSE;	// LOT 끝나면 Reset
	gData.bNgTrayEnd = FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

void CSequenceMain::Set_ClearRunData(int nType)
{
	memset(gData.bReload, 0x00, sizeof(BOOL) * 7);
	memset(gData.bScanDone, 0x00, sizeof(BOOL) * 7);
	memset(gData.byInspectDone, 0x00, sizeof(BYTE) * 2 * 30 * 40);
	memset(gData.nInspectInfo, 0x00, sizeof(int) * 2 * 30 * 40);
	memset(gData.cJudgeCode1, 0x00, sizeof(char) *  30 * 40 * 7);
	memset(gData.cJudgeCode2, 0x00, sizeof(char) *  30 * 40 * 7);
	memset(gData.bRosDone, 0x00, sizeof(BOOL) * 2 * 30 * 40);
	memset(gData.nJigNgCnt, 0x00, sizeof(int) * 2 * 8 * 10 );	// Jig별 불량수량

	memset(gData.InfoAnglePortTray, 0x00, sizeof(int) * 8 * 5);
	memset(gData.InfoAngleTray, 0x00, sizeof(int) * 2 * 8 * 5);
	memset(gData.InfoBuffTray, 0x00, sizeof(int) * 2 * 8 * 5);
	memset(gData.InfoGoodTray, 0x00, sizeof(int) * 8 * 5);
	memset(gData.InfoNgTray, 0x00, sizeof(int) * 4 * 8 * 5);

	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 0, NULL);

	memset(gData.InfoBtm1Pick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.InfoInspect, 0x00, sizeof(int) * 3 * 2 * 5);
	memset(gData.InfoBtm2Pick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.InfoSortPick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.InfoNgBuffer, 0x00, sizeof(int) * 2 * 5);

	gData.nPNoTrayPick = gData.nPNoBtm1Pick = gData.nPNoBtm2Pick = 0;
	gData.nPNoNgTray = gData.nPNoGoodTray = 0;
	gData.nLPNo = gData.nULPNo = 0;
	memset(gData.nPNoAnglePort, 0x00, sizeof(int) * 2);
	memset(gData.nPNoAngleTray, 0x00, sizeof(int) * 2);
	memset(gData.nPNoInspect, 0x00, sizeof(int) * 3 );
	memset(gData.nPNoBuffTray, 0x00, sizeof(int) * 2);
	memset(gData.nPNoSortPick, 0x00, sizeof(int) * 2);
	memset(gData.nPNoNgBuffer, 0x00, sizeof(int) * 2);

	gData.nTNoTrayPick = 0;
	memset(gData.nTNoAnglePort, 0x00, sizeof(int) * 2);
	memset(gData.nTNoAngleTray, 0x00, sizeof(int) * 2);
	memset(gData.nTNoBtm1Pick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.nTNoInspect, 0x00, sizeof(int) * 3 * 2 * 5);
	memset(gData.nTNoBtm2Pick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.nTNoBuffTray, 0x00, sizeof(int) * 2 * 5 * 8);
	memset(gData.nTNoSortPick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.nTNoNgBuffer, 0x00, sizeof(int) * 2 * 5);

	memset(gData.nCNoBtm1Pick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.nCNoInspect, 0x00, sizeof(int) * 3 * 2 * 5);
	memset(gData.nCNoBtm2Pick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.nCNoBuffTray, 0x00, sizeof(int) * 2 * 5 * 8);
	memset(gData.nCNoSortPick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.nCNoNgBuffer, 0x00, sizeof(int) * 2 * 5);

	memset(gLot.dAverageCycle, 0x00, sizeof(double) * 20 * 2 * 15);

	// Process 변수 초기화
	m_bLoadLotEnd = FALSE;
	m_bUnloadLotEnd = FALSE;

	m_nAnglePortTrayCnt = 0;
	m_bAngleInspComplete = FALSE;

	//AHN
	m_dwFirstLoad = 0;
	m_dwLastUnLoad = 0;
	m_dwULCycleTime = 0;
	gData.dwGoodTray1LoadingTime = 0;
	gData.dwGoodTray1UnloadingTime = 0;
	gData.dwGoodTray2LoadingTime = 0;
	gData.dwGoodTray2UnloadingTime = 0;
	gLot.nErrorCount = 0;
	gLot.dwRunTime = gLot.dwErrorTime = gLot.dwStopTime = 0;
	gLot.bLotEndComplete[0] = FALSE;
	gLot.bLotEndComplete[1] = FALSE;
	gData.bFirstLotStart = FALSE;

	gData.nLoadTrayCount[0] = 0; gData.nLoadTrayCount[1] = 0;
	gData.nEmptyTrayCount = (m_nEmptyTrayElCase == 20 ? 1 : 0);
	gData.nGoodTrayCount = 0;
// 	gData.nNNgTrayCount = 0;
// 	gData.nSNgTrayCount = 0;



	gData.bNGTrayWait = FALSE;
	gData.bContinueLotEnd = FALSE;
	gData.bGoodTrayWait = FALSE;
	gData.bGoodTrayLotEnd[0] = FALSE;
	gData.bGoodTrayLotEnd[1] = FALSE;
	gData.bEmptyZWait = FALSE;

	gData.bLoadLampOn[0] = FALSE;
	gData.bLoadLampOn[1] = FALSE;

	gData.bVisionVerErr = FALSE;

	gData.bBuffStageMove = FALSE;

	// Mirror Down Error 변수 초기화
	gData.bTop1MirrorErr[0] = FALSE;
	gData.bTop1MirrorErr[1] = FALSE;
	gData.bTop1MirrorErr[2] = FALSE;
	gData.b3DGrabFailErr = FALSE;

	for (int p = 0; p < 2; p++) for (int i = 0; i < 6; i++) gLot.nSNgCount[p][i] = 0;	// Special NG
	gLot.nGoodCount[0] = gLot.nGoodCount[1] = gLot.nNgCount[0] = gLot.nNgCount[1] = 0;
	for (int p = 0; p < 2; p++) { gLot.nRosRequest[p] = gLot.nRosGood[p] = gLot.nRosNg[p] = gLot.nRosRepair[p] = gLot.nRosTimeOut[p] = 0; }
	g_dlgWork.PostMessage(UM_VISION_RESULT, NULL, NULL);

	for (int p = 0; p <  2; p++) for (int i = 0; i < 30; i++) for (int j = 0; j < 40; j++) gMes.sBarID[p][i][j] = gMes.sJudge[p][i][j] = gMes.sNGCode[p][i][j] = "";
	for (int p = 0; p <  2; p++) for (int i = 0; i < 30; i++) for (int j = 0; j < 40; j++) for (int k = 0; k < 7; k++) gData.sNGData[p][i][j][k] = "";

	m_bLotLoadEnable[0] = FALSE;
	m_bLotLoadEnable[1] = FALSE;

	g_dlgWork.Enable_UserInput(1, TRUE);
	g_dlgWork.Enable_UserInput(2, TRUE);

	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_LampFlicker_Load1(FALSE);
	pMainDlg->Set_LampFlicker_Load2(FALSE);
	pMainDlg->Set_LampFlicker_Ng(FALSE);
	pMainDlg->Set_LampFlicker_Good(FALSE);
	pMainDlg->Set_LampFlicker_Empty(FALSE);

	if (nType == 0) m_pEquipData->bResultTestUse = FALSE;	// LOT 끝나면 Reset
	gData.bCapTrayLoad = FALSE;
	gLot.nNGT = gLot.nNGC = gLot.nGDT = gLot.nG1DC = 0;

	g_objDispatcher.Reset_JudgeData(0);

	g_dlgWork.PostMessage(UM_UPDATE_MODEL, NULL, NULL);

	gData.b3DStart = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void CSequenceMain::Init_AnglePortTray(int nPNo)
{
	memset(gData.InfoAnglePortTray, 0x00, sizeof(int) * 8 * 5);	// Empty

	int nPx = nPNo - 1;
	if (nPx < 0) nPx = 0;

	int nCmMax = gData.nCmMaxCount; //한트레이에 들어가는 최대 모듈 수 
	int nTrayCnt = gData.nTNoAnglePort[0];
	int nTrayUse = gData.nTrayUseCount[nPx];
	int nCmUse   = gData.nCmUseCount[nPx];

	int nCmCnt = (nTrayCnt < nTrayUse ? nCmMax : (nCmUse - (nTrayCnt - 1) * nCmMax));
	int nTrayY, nTrayX;


	int fullLine_YCnt = gData.nTrayY - (nCmCnt / 4) - 1;
	int remainder_X = nCmCnt % 4;
	
 	for (int i = 0; i < nCmCnt; i++) {
		nTrayY = (gData.nTrayY-1) - (i/gData.nTrayX);	// 하단서부터 모듈정보를 채워준다.

		if(nTrayY == fullLine_YCnt)
		{
			nTrayX = (i % 4) +(gData.nTrayX-remainder_X); //마지막 라인 잔량은 오른쪽으로 붙이기
		}
		else{
			nTrayX = i % gData.nTrayX;
		}
		
		gData.InfoAnglePortTray[nTrayY][nTrayX] = 9;	// Not Use
	}
	
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, NULL);
}

void CSequenceMain::Init_AngleTray(int nIdx)
{
	for (int y = 0; y < gData.nTrayY; y++) {
		for (int x = 0; x < gData.nTrayX; x++) {
			if (gData.InfoAngleTray[nIdx][y][x] == 1) gData.InfoAngleTray[nIdx][y][x] = 9;
			else									  gData.InfoAngleTray[nIdx][y][x] = 0;
		}
	}

	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, nIdx);
}

void CSequenceMain::Init_BufferTray()
{
	memset(gData.InfoBuffTray, 0x00, sizeof(int) * 2 * 5 * 8);	// Empty
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 3, NULL);
}

void CSequenceMain::Init_GoodTray()
{
	memset(gData.InfoGoodTray, 0x00, sizeof(int) * 5 * 8);	// Empty
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 4, NULL);
}

void CSequenceMain::Init_NgTray(int nIdx)
{
	memset(gData.InfoNgTray[nIdx], 0x00, sizeof(int) * 5 * 8);	// Empty
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 5, NULL);
}

///////////////////////////////////////////////////////////////////////////////

BOOL CSequenceMain::Check_AnglePortTrayEmpty()
{
	for (int y = 0; y < gData.nTrayY; y++) {
		for (int x = 0; x < gData.nTrayX; x++) {
			if (gData.InfoAnglePortTray[y][x] > 0) return FALSE;
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_AngleTrayEmpty(int nNo)
{
	int nSNo = (nNo == 0 ? 0 : nNo-1);
	int nENo = (nNo == 0 ? 2 : nNo);
	for (int s = nSNo; s < nENo; s++) {
		for (int y = 0; y < gData.nTrayY; y++) {
			for (int x = 0; x < gData.nTrayX; x++) {
				if (gData.InfoAngleTray[s][y][x] > 0) return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_AngleTrayLineEmpty(int nNo, int nLine)
{	
	for (int x = 0; x < gData.nTrayX; x++) {
		if (gData.InfoAngleTray[nNo-1][nLine-1][x] > 0) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_Btm1PickerEmpty()
{
	for (int i = 0; i < 10; i++) { 
		if (gData.InfoBtm1Pick[i] > 0) return FALSE; 
	}
	return TRUE;
}

BOOL CSequenceMain::Check_InspetionEmpty(int nNo)
{
	for (int i = 0; i < 10; i++) {
		if (gData.InfoInspect[nNo-1][i] > 0) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_Btm2PickerEmpty()
{
	for (int i = 0; i < 10; i++) { 
		if (gData.InfoBtm2Pick[i] > 0) return FALSE; 
	}
	return TRUE;
}

BOOL CSequenceMain::Check_SortPickerEmpty(int nNo)
{
	for (int i = 0; i < 5; i++) {
		if (gData.InfoSortPick[nNo-1][i] > 0) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_BufferEmpty(int nNo)
{
	int nSNo = (nNo == 0 ? 0 : nNo-1);
	int nENo = (nNo == 0 ? 2 : nNo);
	for (int s = nSNo; s < nENo; s++) {
		for (int y = 0; y < gData.nTrayY; y++) {
			for (int x = 0; x < gData.nTrayX; x++) {
				if (gData.InfoBuffTray[s][y][x] > 0) return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_BufferTrayLineEmpty(int nNo, int nLine)
{	
	for (int x = 0; x < gData.nTrayX; x++) {
		if (gData.InfoBuffTray[nNo-1][nLine-1][x] > 0) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_GoodTrayEmpty()
{
	for (int y = 0; y < gData.nTrayY; y++) {
		for (int x = 0; x < gData.nTrayX; x++) {
			if (gData.InfoGoodTray[y][x] > 0) return FALSE;
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_Btm1PickerFull()
{
	// A모델은 90도 회전하기때문에 Full Picker가 안될수있다.
	// L2(5~10번)에 모듈이 있으면 Btm1 검사위치로 이동한다.
	for (int i = 4; i < 8; i++) {
		if (gData.InfoBtm1Pick[i] > 0) return TRUE; 
	}
	return FALSE;
}

BOOL CSequenceMain::Check_BufferFull(int nNo)
{
	// 10개씩 넘어가게 해준다.
	//if (gData.InfoBuffTray[nNo][gData.nTrayY - 2][gData.nTrayX - 1] > 0) return TRUE;
	// 잔량이 어디에 있을지 모르니 Tray 마지막 라인에 모듈 하나라도 있으면 넘어가게 해준다.
	for (int i = 0; i < gData.nTrayX; i++) {
		if (gData.InfoBuffTray[nNo][gData.nTrayY-2][i] > 0) return TRUE;
	}	
	return FALSE;
}

BOOL CSequenceMain::Check_GoodTrayFull()
{
	if (gData.InfoGoodTray[gData.nTrayY-1][gData.nTrayX-1] > 0) return TRUE;	
	return FALSE;
}

BOOL CSequenceMain::Check_NgTrayFull()
{
	if (gData.InfoNgTray[0][gData.nTrayY-1][gData.nTrayX-1] > 0) return TRUE;	// N1,N2,N3
	if (gData.InfoNgTray[1][gData.nTrayY-1][gData.nTrayX-1] > 0) return TRUE;	// 
	//if (gData.InfoNgTray[3][gData.nTrayY-1][gData.nTrayX-1] > 0) return TRUE;	// N4 2개 트레이 씀 
	return FALSE;
}

BOOL CSequenceMain::Check_NgBufferFull(int nNo)
{
	int nIdx = (nNo == 1 ? 1 : 0);
	if (gData.InfoNgBuffer[nIdx][gData.nSortPickQt-1] > 0) return TRUE;
	return FALSE;
}

BOOL CSequenceMain::Check_NgBufferEmpty(int nNo)
{
	int nIdx = (nNo == 1 ? 1 : 0);
	for (int i = 0; i < gData.nSortPickQt; i++) {
		if (gData.InfoNgBuffer[nIdx][i] > 0) return FALSE; 
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

BOOL CSequenceMain::Select_AngleScanPos(int &nAnglePosX, int &nAnglePosY)
{
	// Module만 Scan
	BOOL bScanLine = FALSE; 
	nAnglePosX = nAnglePosY = 0;
	for(int j=(gData.nTrayY-1); j>=0; j--) {
		// 잔량이 어디 있을지 몰라 Line에 한개 이상 모듈이 있으면 전체 Line Scan 해준다. 
		for(int i=0; i<gData.nTrayX; i++) {
			if (gData.InfoAnglePortTray[j][i] != 0) { bScanLine = TRUE; break;}
		}

		if (j==1 || j==3 || j==5 || j==7 || j==9 || j==11) {
			for(int i=(gData.nTrayX-1); i>=0; i--) {
				if ((gData.InfoAnglePortTray[j][i] == 0 || gData.InfoAnglePortTray[j][i] == 9) && bScanLine) {
					nAnglePosY = j + 1;
					nAnglePosX = i + 1;
					break;
				}
			}
		} else {
			for(int i=0; i<gData.nTrayX; i++) {
				if ((gData.InfoAnglePortTray[j][i] == 0 || gData.InfoAnglePortTray[j][i] == 9) && bScanLine) {
					nAnglePosY = j + 1;
					nAnglePosX = i + 1;
					break;
				}
			}
		}
		bScanLine = FALSE;
		if (nAnglePosY > 0) break;
	}

	if (nAnglePosY > gData.nTrayY) return FALSE;
	if (nAnglePosY == 0 || nAnglePosX ==0) return FALSE;
	return TRUE;
}

BOOL CSequenceMain::Select_BtmScanPos(int nPos, int nScanCnt)
{
	int *pInfo = NULL;
	if (nPos == AUTO_VISION_BTM1) pInfo = gData.InfoBtm1Pick;
	if (nPos == AUTO_VISION_BTM2) pInfo = gData.InfoBtm2Pick;
	if (pInfo == NULL) return FALSE;

	if (nPos == AUTO_VISION_BTM1) {
		/*if (nScanCnt == 0) { if (pInfo[3] == 9 || pInfo[7] == 9) return TRUE; }
		if (nScanCnt == 1) { if (pInfo[2] == 9 || pInfo[6] == 9) return TRUE; }
		if (nScanCnt == 2) { if (pInfo[1] == 9 || pInfo[5] == 9) return TRUE; }
		if (nScanCnt == 3) { if (pInfo[0] == 9 || pInfo[4] == 9) return TRUE; }*/

		if (nScanCnt == 0) { if (pInfo[4] == 9 || pInfo[0] == 9) return TRUE; }
		if (nScanCnt == 1) { if (pInfo[5] == 9 || pInfo[1] == 9) return TRUE; }
		if (nScanCnt == 2) { if (pInfo[6] == 9 || pInfo[2] == 9) return TRUE; }
		if (nScanCnt == 3) { if (pInfo[7] == 9 || pInfo[3] == 9) return TRUE; }
	}

	if (nPos == AUTO_VISION_BTM2) {
		if (nScanCnt == 0) { if (pInfo[3] == 9 || pInfo[7] == 9) return TRUE; }
		if (nScanCnt == 1) { if (pInfo[2] == 9 || pInfo[6] == 9) return TRUE; }
		if (nScanCnt == 2) { if (pInfo[1] == 9 || pInfo[5] == 9) return TRUE; }
		if (nScanCnt == 3) { if (pInfo[0] == 9 || pInfo[4] == 9) return TRUE; }
	}
	return FALSE;
}

BOOL CSequenceMain::Select_Top1ScanPos(int nPos, int nScanCnt)
{
	int *pInfo = NULL;
	if (nPos == AUTO_INSPECTION1) pInfo = gData.InfoInspect[0];
	if (nPos == AUTO_INSPECTION2) pInfo = gData.InfoInspect[1];
	if (nPos == AUTO_INSPECTION3) pInfo = gData.InfoInspect[2];
	if (pInfo == NULL) return FALSE;

	if (nScanCnt == 0) { if (pInfo[3] == 9 || pInfo[7] == 9) return TRUE; }
	if (nScanCnt == 1) { if (pInfo[2] == 9 || pInfo[6] == 9) return TRUE; }
	if (nScanCnt == 2) { if (pInfo[1] == 9 || pInfo[5] == 9) return TRUE; }
	if (nScanCnt == 3) { if (pInfo[0] == 9 || pInfo[4] == 9) return TRUE; }
	return FALSE;
}

BOOL CSequenceMain::Select_Top2ScanPos(int nPos, int nScanCnt)
{
	int *pInfo = NULL;
	if (nPos == AUTO_INSPECTION1) pInfo = gData.InfoInspect[0];
	if (nPos == AUTO_INSPECTION2) pInfo = gData.InfoInspect[1];
	if (nPos == AUTO_INSPECTION3) pInfo = gData.InfoInspect[2];
	if (pInfo == NULL) return FALSE;

	if (nScanCnt == 0) { if (pInfo[3] == 9 || pInfo[7] == 9) return TRUE; }
	if (nScanCnt == 1) { if (pInfo[2] == 9 || pInfo[6] == 9) return TRUE; }
	if (nScanCnt == 2) { if (pInfo[1] == 9 || pInfo[5] == 9) return TRUE; }
	if (nScanCnt == 3) { if (pInfo[0] == 9 || pInfo[4] == 9) return TRUE; }
	return FALSE;
}

BOOL CSequenceMain::Select_AngleTrayPos(int nNo, int &nTrayPosY, int &nRow)
{
	nTrayPosY = nRow = -1;
	if (Check_AngleTrayEmpty()) return FALSE;

	for (int y = 0; y <gData.nTrayY; y++) {
		if (gData.InfoAngleTray[nNo-1][y][3] > 0) { nTrayPosY = y; nRow = y%2; break;}
	}
	if (nTrayPosY == -1 || nRow == -1) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Select_Btm1PickPos(int &nPos)
{
	//for (int i = 0; i < 8; i++) { if (gData.InfoBtm1Pick[i] == 0) { nPos = i; return TRUE; } }	// 1,2,3,4,5 순서로...
	int nPick = -1;
	for (int i = 0; i <  4; i++) { if (gData.InfoBtm1Pick[i] > 0) nPick = i; }	// 모듈을 들고 있으면 4~8번 Picker가 집는다.
	if (nPick == -1) { nPos = 0; return TRUE; }

	nPick = -1;
	for (int i = 4; i < 8; i++) { if (gData.InfoBtm1Pick[i] > 0) nPick = i; }
	if (nPick == -1) { nPos = 4; return TRUE; }
	return FALSE;
}

BOOL CSequenceMain::Select_Btm2PickPos(int &nPos)
{
	//for (int i = 0; i < 8; i++) { if (gData.InfoBtm2Pick[i] > 0) { nPos = i; return TRUE; } }	// 1,2,3,4,5 순서로...
	int nPick = -1;
	for (int i = 0; i <  4; i++) { if (gData.InfoBtm2Pick[i] > 0) nPick = i; }	// 어느 위치든 모듈을 들고 있으면 내려 놓는다.
	if (nPick > -1) { nPos = 0; return TRUE; }

	nPick = -1;
	for (int i = 4; i < 8; i++) { if (gData.InfoBtm2Pick[i] > 0) nPick = i; }
	if (nPick > -1) { nPos = 4; return TRUE; }
	return FALSE;
}

BOOL CSequenceMain::Select_SortPickGoodPos(int nNo, int &nPos, int &nCnt)
{
	nPos = -1; nCnt =  0;
	
	for (int i = 0; i < 5; i++) { 
		if (gData.InfoSortPick[nNo-1][i] == 1) { nPos = i; break; } 
	}

	for (int j = nPos; j < 5; j++) { 
		if (gData.InfoSortPick[nNo-1][j] == 1)	nCnt++;
		else									break;
	}
	if (nPos == -1 || nCnt == 0) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Select_SortPickNgPos(int nNo, int &nPos, int &nCnt, BOOL bBuffer)
{
	// NN(2), N1(4), N2(5), N3(6), BS(7), N4(8)
	nPos = -1; nCnt = 0;

	if (bBuffer) {	// Ng Buffer 전용
		for (int i = 0; i < 5; i++) { if (gData.InfoSortPick[nNo-1][i] >= 2) { nPos = i; break; } }
		for (int i = nPos; i < 5; i++) { if (gData.InfoSortPick[nNo-1][i] < 2) break; nCnt++; }

	} else {
		int *pInfo = gData.InfoSortPick[nNo-1];

		for (int i = 0; i < 5; i++) { if (pInfo[i] == 2 || pInfo[i] == 4 || pInfo[i] == 6 || pInfo[i] == 7 || pInfo[i] == 5) { nPos = i; break; } }	// Tray1,3

		if (nPos == -1) {
			for (int i = 0; i < 5; i++) { if (pInfo[i] == 8) { nPos = i; break; } }	// Tray2, 4			
		}
		if (nPos == -1) return FALSE;

		for (int i = nPos; i < 5; i++) { if (pInfo[i] != pInfo[nPos]) break; nCnt++; }
	}
	if (nPos == -1 || nCnt == 0) return FALSE;

	return TRUE;
}

int CSequenceMain::Get_NgTrayNumber(int nInfo)
{
	if (nInfo == 2 || nInfo == 4 || nInfo == 6 || nInfo == 7) return 3;	// Tray1,3
	else if (nInfo == 5) return 4;	// Tray4
	else if (nInfo == 8) return 2;	// Tray2
	else return 0;	// Error
}

BOOL CSequenceMain::Select_BufferTrayDownPos(int nNo, int &nTrayPosY, int &nRow)
{
	nTrayPosY = nRow = -1;
	if (Check_BufferFull(nNo-1)) return FALSE;

	for (int y = 0; y < gData.nTrayY; y++) {
		// 라인 전체에 모듈이 없을때 Btm2 Picker Down한다.
		BOOL bCmExist = FALSE;
		for (int x = 0; x < gData.nTrayX; x++) { if (gData.InfoBuffTray[nNo-1][y][x] > 0) bCmExist = TRUE; }
		if (!bCmExist) { nTrayPosY = y; nRow = y%2; return TRUE; }
	}	
	return FALSE;
}

BOOL CSequenceMain::Select_BufferTrayUpPos(int nNo, int &nTrayPosX, int &nTrayPosY)
{
	nTrayPosX = -1; nTrayPosY = -1;
	if (Check_BufferEmpty(nNo)) return FALSE;

	for (int y = 0; y < gData.nTrayY; y++) {
		for (int x = 0; x < gData.nTrayX; x++) {
			if (gData.InfoBuffTray[nNo-1][y][x] > 0) { nTrayPosX = x; nTrayPosY = y; return TRUE; }
		}
	}
	return FALSE;
}

BOOL CSequenceMain::Select_GoodTrayPos(int &nTrayPosX, int &nTrayPosY)
{
	nTrayPosX = nTrayPosY = -1;
	if (Check_GoodTrayFull()) return FALSE;

	for (int y = 0; y < gData.nTrayY; y++) {
		for (int x = 0; x < gData.nTrayX; x++) {
			if (gData.InfoGoodTray[y][x] == 0) { nTrayPosX = x; nTrayPosY = y; return TRUE; }
		}
	}
	return FALSE;
}

BOOL CSequenceMain::Select_NgTrayPos(int nPickNo, int &nTrayNo, int &nTrayPosX, int &nTrayPosY)
{
	// NN(2), N1(4), N2(5), N3(6), BS(7), N4(8)
	nTrayNo = nTrayPosX = nTrayPosY = -1;
	if (Check_NgTrayFull()) return FALSE;

	int *pInfo = gData.InfoSortPick[nPickNo-1];
	int nTray1 = -1, nTray2 = -1, nInfo = 0;	// Default

	for (int i = 0; i < 5; i++) { 
		if (pInfo[i] == 2 || pInfo[i] == 4 || pInfo[i] == 6 || pInfo[i] == 7 || pInfo[i] == 5) { nInfo = pInfo[i]; break; }	// Tray1,3
	}
	if (nInfo == 0) {
		for (int i = 0; i < 5; i++) {
			if (pInfo[i] == 8) { nInfo = pInfo[i]; break; }	// Tray 2,4
		}		
	}
	if (nInfo == 0) return FALSE;

	if (nInfo == 2 || nInfo == 4 || nInfo == 6 || nInfo == 7 || nInfo == 5) { nTray1 = 0; nTray2 = 2; }	// N,N1,N2,N3,B
	if (nInfo == 8) { nTray1 = 1; nTray2 = 3 ;}		// N4

	for (int t = 3; t >= 0; t--) {
		if (t != nTray1 && t != nTray2) continue;
		for (int y = 0; y < gData.nTrayY; y++) {
			for (int x = 0; x < gData.nTrayX; x++) {
				if (gData.InfoNgTray[t][y][x] == 0) { nTrayNo = t; nTrayPosX = x; nTrayPosY = y; return TRUE; }
			}
		}
	}

	return FALSE;
}

BOOL CSequenceMain::Select_NgBufferPos(int nPickNo, int &nTrayPosX)
{
	int nBNo = 0;
	nTrayPosX = -1;
	if (Check_NgBufferFull(nPickNo)) return FALSE;

	if (nPickNo == 1) nBNo = 1;
	if (nPickNo == 2) nBNo = 0;

	for (int x = 0; x < gData.nSortPickQt; x++) {
		if (gData.InfoNgBuffer[nBNo][x] == 0) { nTrayPosX = x; return TRUE; }
	}
	return FALSE;
}

void CSequenceMain::Set_InspectJigNo(int nType, int nScanCnt, int &nINo1, int &nINo2, int &nINo3, int &nINo4)
{
	if (nType == 0) {	// BTM1_Specular Vision 
		if (nScanCnt == 0) { nINo1 = 5; nINo2 = 1; nINo3 = 0; nINo4 = 0; }
		if (nScanCnt == 1) { nINo1 = 6; nINo2 = 2; nINo3 = 0; nINo4 = 0; }
		if (nScanCnt == 2) { nINo1 = 7; nINo2 = 3; nINo3 = 0; nINo4 = 0; }
		if (nScanCnt == 3) { nINo1 = 8; nINo2 = 4; nINo3 = 0; nINo4 = 0; }
	}
	if (nType == 4){   // BTM1_Angle Vision 
		if (nScanCnt == 0) { nINo1 = 5; nINo2 = 1; nINo3 = 0; nINo4 = 0; }
		if (nScanCnt == 1) { nINo1 = 6; nINo2 = 2; nINo3 = 0; nINo4 = 0; }
		if (nScanCnt == 2) { nINo1 = 7; nINo2 = 3; nINo3 = 0; nINo4 = 0; }
		if (nScanCnt == 3) { nINo1 = 8; nINo2 = 4; nINo3 = 0; nINo4 = 0; }
	}
	if (nType == 1) {	// TOP1
		if (nScanCnt == 0) { nINo1 = 4; nINo2 = 8; nINo3 = 0; nINo4 = 0; }
		if (nScanCnt == 1) { nINo1 = 3; nINo2 = 7; nINo3 = 0; nINo4 = 0; }
		if (nScanCnt == 2) { nINo1 = 2; nINo2 = 6; nINo3 = 0; nINo4 = 0; }
		if (nScanCnt == 3) { nINo1 = 1; nINo2 = 5; nINo3 = 0; nINo4 = 0; }
	}
	if (nType == 2) {	// TOP2
		if (nScanCnt == 0) { nINo1 = 8; nINo2 = 4; nINo3 = 0; nINo4 = 0; }
		if (nScanCnt == 1) { nINo1 = 7; nINo2 = 3; nINo3 = 0; nINo4 = 0; }
		if (nScanCnt == 2) { nINo1 = 6; nINo2 = 2; nINo3 = 0; nINo4 = 0; }
		if (nScanCnt == 3) { nINo1 = 5; nINo2 = 1; nINo3 = 0; nINo4 = 0; }
	}
	if (nType == 3) {	// BTM2
		if (nScanCnt == 0) { nINo1 = 8; nINo2 = 4; nINo3 = 0; nINo4 = 0; }
		if (nScanCnt == 1) { nINo1 = 7; nINo2 = 3; nINo3 = 0; nINo4 = 0; }
		if (nScanCnt == 2) { nINo1 = 6; nINo2 = 2; nINo3 = 0; nINo4 = 0; }
		if (nScanCnt == 3) { nINo1 = 5; nINo2 = 1; nINo3 = 0; nINo4 = 0; }
	}
}

void CSequenceMain::Set_3DInspectJigNo(int nType, int nScanCnt, int &nINo1, int &nINo2, int &nINo3, int &nINo4, int &nINo5, int &nINo6, int &nINo7, int &nINo8)
{
	 nINo1 = 8; nINo2 = 7; nINo3 = 6; nINo4 = 5; nINo5 = 4; nINo6 = 3; nINo7 = 2; nINo8 = 1;  
}

///////////////////////////////////////////////////////////////////////////////

BOOL CSequenceMain::Check_InspectDone(int nPortNo, int nTrayNo, int nCmNo, int &nInfo, int nSortNo)
{
	if (nPortNo == 0 || nTrayNo == 0 || nCmNo == 0 || nInfo == 0) return TRUE;
	int	nPx = nPortNo - 1;	// Port Index
	int	nTx = nTrayNo - 1;	// Tray Index
	int	nCx = nCmNo - 1;	// CM Index

	if (((gData.byInspectDone[nPx][nTx][nCx] >> 7) & 1) == 1) return TRUE;	// 판정 완료 (2번 판정하지 않기 위해)

#ifdef DRY_RUN_TEST
	int nRand = g_objCommon.Get_Random(0, 99);
	int nNg1 = m_pEquipData->nResultTestN1;
	int nNg2 = m_pEquipData->nResultTestN2 + nNg1;
	int nNg3 = m_pEquipData->nResultTestN3 + nNg2;
	int nNg4 = m_pEquipData->nResultTestN4 + nNg3;

	int nJudge = (nRand < nNg1 ? 4 : (nRand < nNg2 ? 5 : (nRand < nNg3 ? 6 : (nRand < nNg4 ? 8 : 1))));
	nInfo = gData.nInspectInfo[nPx][nTx][nCx] = nJudge;
	if (nInfo == 9) { nInfo = gData.nInspectInfo[nPx][nTx][nCx] = 1; }

#else
	CString strLog;
	BOOL bDone = TRUE;
	if (m_pEquipData->bUseInspectAngle && ((gData.byInspectDone[nPx][nTx][nCx] >> 0) & 1) == 0)
		bDone = FALSE;	// return FALSE;	// Angle
	if (m_pEquipData->bUseInspectBtm1Specular  && ((gData.byInspectDone[nPx][nTx][nCx] >> 1) & 1) == 0) 
		bDone = FALSE;	// return FALSE;	// Btm1_SP
	if (m_pEquipData->bUseInspectTop1  && ((gData.byInspectDone[nPx][nTx][nCx] >> 2) & 1) == 0) 
		bDone = FALSE;	// return FALSE;	// Top1	
	if (m_pEquipData->bUseInspectTop2  && ((gData.byInspectDone[nPx][nTx][nCx] >> 3) & 1) == 0) 
		bDone = FALSE;	// return FALSE;	// Top2
	if (m_pEquipData->bUseInspectBtm2  && ((gData.byInspectDone[nPx][nTx][nCx] >> 4) & 1) == 0)
		bDone = FALSE;	// return FALSE;	// Btm2
	if (m_pEquipData->bUseInspectBtm1Angle  && ((gData.byInspectDone[nPx][nTx][nCx] >> 5) & 1) == 0)
		bDone = FALSE;	// return FALSE;	// Btm1_AG
	if (m_pEquipData->bUseInspectBtm13D  && ((gData.byInspectDone[nPx][nTx][nCx] >> 6) & 1) == 0) 
		bDone = FALSE;	// return FALSE;	// Btm1_3D
	
	if (!bDone) {
		if (m_pEquipData->bUseInspectSkip || (GetTickCount() - gData.dwSkipTime_Sort1 > m_pEquipData->nDelayAdd[4]) ) {	// SortPicker에서 검사 완료 체크할때 검사결과가 안날라왔으면 1차로 빼준다.
			gData.nInspectInfo[nPx][nTx][nCx] = 4;
			gMes.sJudge[nPx][nTx][nCx] = "N1";
			strLog.Format("Judge Time Over Sort Picker, PortNo(%d), TrayNo(%d), CmNo(%d)", nPx+1, nTx+1, nCx+1);
			g_objLogFile.Save_HandlerLog(strLog);
		} else {
			return FALSE;
		}
	}

	int nWaitTime = 0;
	if		(nSortNo == 1)	nWaitTime = GetTickCount() - gData.nSp1Timer;
	else if (nSortNo == 2)  nWaitTime = GetTickCount() - gData.nSp2Timer;
	strLog.Format("%d,%d,%d,%s,%d", nPx+1, nTx+1, nCx+1, gData.sSortWaitStartTime[nSortNo-1], nWaitTime);
	g_objLogFile.Save_InspectWaitLog(nPx+1, strLog);


	if (m_pEquipData->bUseDispatcher && !g_objDispatcher.Is_JudgeDone(nPx+1, nTx+1, nCx+1)) return FALSE;
	
	if (!gData.bRosDone[nPx][nTx][nCx]) {
		if		(nSortNo == 1) nWaitTime = GetTickCount() - gData.nSp1Timer;
		else if (nSortNo == 2) nWaitTime = GetTickCount() - gData.nSp2Timer;

		strLog.Format("%d,%d,%d,%s,%d", nPx+1, nTx+1, nCx+1, gData.sSortWaitStartTime[nSortNo-1], nWaitTime);
		g_objLogFile.Save_RosWaitLog(nPx+1, strLog);
		gData.bRosDone[nPx][nTx][nCx] = TRUE;
	}

	if (m_pEquipData->bResultTestUse)
	{
		int nRand = g_objCommon.Get_Random(0, 99);
		int nNg1 = m_pEquipData->nResultTestN1;
		int nNg2 = m_pEquipData->nResultTestN2 + nNg1;
		int nNg3 = m_pEquipData->nResultTestN3 + nNg2;
		int nNg4 = m_pEquipData->nResultTestN4 + nNg3;

		int nJudge = (nRand < nNg1 ? 4 : (nRand < nNg2 ? 5 : (nRand < nNg3 ? 6 : (nRand < nNg4 ? 8 : 1))));
		nInfo = gData.nInspectInfo[nPx][nTx][nCx] = nJudge;
		if (nInfo == 9) { nInfo = gData.nInspectInfo[nPx][nTx][nCx] = 1; }
		strLog.Format("ResultTest_Use : %d,%d,%d",nInfo, nTx+1, nCx+1);
		g_objLogFile.Save_TestLog(strLog);

	} 
	else 
	{
		if (gData.bCycleStop && !Get_VisionInspectUse()) nInfo = gData.nInspectInfo[nPx][nTx][nCx] = 1;	//Good
		else if (Get_VisionInspectUse())  nInfo = gData.nInspectInfo[nPx][nTx][nCx];

		if (nInfo == 9) nInfo = gData.nInspectInfo[nPx][nTx][nCx] = 1;
	}

	for(int i = 0; i < gLot.nSNgCount[nPx][0]; i++ )
	{
		if(gData.nPNoMESNG[i] == nPortNo && gData.nTNoMESNG[i] == nTrayNo && gData.nCmNoMESNG[i] == nCmNo)
		{
			gData.nInspectInfo[nPx][nTx][nCx] = 4;
			gMes.sJudge[nPx][nTx][nCx] = "N1";

			nInfo = gData.nInspectInfo[nPx][nTx][nCx];		

			strLog.Format("MES NG, CM less than MES Count Info, PortNo(%d), TrayNo(%d), CmNo(%d)", nPx+1, nTx+1, nCx+1);
			g_objLogFile.Save_HandlerLog(strLog);
		}
	}

#endif

#ifdef VISION_REPEAT
	nInfo = gData.nInspectInfo[nPx][nTx][nCx] = 1;	// 반복성 검증일때 모두 양품
#endif

	gData.byInspectDone[nPx][nTx][nCx] |= (1 << 7);	// 판정 완료 (2번 판정하지 않기 위해)
	return TRUE;	// All Inspect Done
}



BOOL CSequenceMain::Check_InspectDone2(int nPortNo, int nTrayNo, int nCmNo, int &nInfo, int nSortNo)
{
	if (nPortNo == 0 || nTrayNo == 0 || nCmNo == 0 || nInfo == 0) return TRUE;
	int	nPx = nPortNo - 1;	// Port Index
	int	nTx = nTrayNo - 1;	// Tray Index
	int	nCx = nCmNo - 1;	// CM Index

	if (((gData.byInspectDone[nPx][nTx][nCx] >> 7) & 1) == 1) return TRUE;	// 판정 완료 (2번 판정하지 않기 위해)

#ifdef DRY_RUN_TEST
	int nRand = g_objCommon.Get_Random(0, 99);
	int nNg1 = m_pEquipData->nResultTestN1;
	int nNg2 = m_pEquipData->nResultTestN2 + nNg1;
	int nNg3 = m_pEquipData->nResultTestN3 + nNg2;
	int nNg4 = m_pEquipData->nResultTestN4 + nNg3;

	int nJudge = (nRand < nNg1 ? 4 : (nRand < nNg2 ? 5 : (nRand < nNg3 ? 6 : (nRand < nNg4 ? 8 : 1))));
	nInfo = gData.nInspectInfo[nPx][nTx][nCx] = nJudge;
	if (nInfo == 9) { nInfo = gData.nInspectInfo[nPx][nTx][nCx] = 1; }

#else
	CString strLog;
	BOOL bDone = TRUE;
	if (m_pEquipData->bUseInspectAngle && ((gData.byInspectDone[nPx][nTx][nCx] >> 0) & 1) == 0)
		bDone = FALSE;	// return FALSE;	// Angle
	if (m_pEquipData->bUseInspectBtm1Specular  && ((gData.byInspectDone[nPx][nTx][nCx] >> 1) & 1) == 0) 
		bDone = FALSE;	// return FALSE;	// Btm1_SP
	if (m_pEquipData->bUseInspectTop1  && ((gData.byInspectDone[nPx][nTx][nCx] >> 2) & 1) == 0) 
		bDone = FALSE;	// return FALSE;	// Top1	
	if (m_pEquipData->bUseInspectTop2  && ((gData.byInspectDone[nPx][nTx][nCx] >> 3) & 1) == 0) 
		bDone = FALSE;	// return FALSE;	// Top2
	if (m_pEquipData->bUseInspectBtm2  && ((gData.byInspectDone[nPx][nTx][nCx] >> 4) & 1) == 0)
		bDone = FALSE;	// return FALSE;	// Btm2
	if (m_pEquipData->bUseInspectBtm1Angle  && ((gData.byInspectDone[nPx][nTx][nCx] >> 5) & 1) == 0)
		bDone = FALSE;	// return FALSE;	// Btm1_AG
	//if (m_pEquipData->bUseInspectBtm13D  && ((gData.byInspectDone[nPx][nTx][nCx] >> 6) & 1) == 0) 
		//bDone = FALSE;	// return FALSE;	// Btm1_3D
	
	if (!bDone) {
		if (m_pEquipData->bUseInspectSkip || (GetTickCount() - gData.dwSkipTime_Sort2 > m_pEquipData->nDelayAdd[4]) ) {	// SortPicker에서 검사 완료 체크할때 검사결과가 안날라왔으면 1차로 빼준다.
			gData.nInspectInfo[nPx][nTx][nCx] = 4;
			gMes.sJudge[nPx][nTx][nCx] = "N1";
			strLog.Format("Judge Time Over Sort Picker, PortNo(%d), TrayNo(%d), CmNo(%d)", nPx+1, nTx+1, nCx+1);
			g_objLogFile.Save_HandlerLog(strLog);
		} else {
			return FALSE;
		}
	}

	int nWaitTime = 0;
	if		(nSortNo == 1)	nWaitTime = GetTickCount() - gData.nSp1Timer;
	else if (nSortNo == 2)  nWaitTime = GetTickCount() - gData.nSp2Timer;
	strLog.Format("%d,%d,%d,%s,%d", nPx+1, nTx+1, nCx+1, gData.sSortWaitStartTime[nSortNo-1], nWaitTime);
	g_objLogFile.Save_InspectWaitLog(nPx+1, strLog);


	if (m_pEquipData->bUseDispatcher && !g_objDispatcher.Is_JudgeDone(nPx+1, nTx+1, nCx+1)) return FALSE;
	
	if (!gData.bRosDone[nPx][nTx][nCx]) {
		if		(nSortNo == 1) nWaitTime = GetTickCount() - gData.nSp1Timer;
		else if (nSortNo == 2) nWaitTime = GetTickCount() - gData.nSp2Timer;

		strLog.Format("%d,%d,%d,%s,%d", nPx+1, nTx+1, nCx+1, gData.sSortWaitStartTime[nSortNo-1], nWaitTime);
		g_objLogFile.Save_RosWaitLog(nPx+1, strLog);
		gData.bRosDone[nPx][nTx][nCx] = TRUE;
	}

	if (m_pEquipData->bResultTestUse) {
		int nRand = g_objCommon.Get_Random(0, 99);
		int nNg1 = m_pEquipData->nResultTestN1;
		int nNg2 = m_pEquipData->nResultTestN2 + nNg1;
		int nNg3 = m_pEquipData->nResultTestN3 + nNg2;
		int nNg4 = m_pEquipData->nResultTestN4 + nNg3;

		int nJudge = (nRand < nNg1 ? 4 : (nRand < nNg2 ? 5 : (nRand < nNg3 ? 6 : (nRand < nNg4 ? 8 : 1))));
		nInfo = gData.nInspectInfo[nPx][nTx][nCx] = nJudge;
		if (nInfo == 9) { nInfo = gData.nInspectInfo[nPx][nTx][nCx] = 1; }
		strLog.Format("ResultTest_Use : %d,%d,%d",nInfo, nTx+1, nCx+1);
		g_objLogFile.Save_TestLog(strLog);

	} else {
		if (gData.bCycleStop && !Get_VisionInspectUse()) nInfo = gData.nInspectInfo[nPx][nTx][nCx] = 1;	//Good
		else if (Get_VisionInspectUse())  nInfo = gData.nInspectInfo[nPx][nTx][nCx];

		if (nInfo == 9) nInfo = gData.nInspectInfo[nPx][nTx][nCx] = 1;
	}
#endif

	for(int i = 0; i < gLot.nSNgCount[nPx][0]; i++ )
	{
		if(gData.nPNoMESNG[i] == nPortNo && gData.nTNoMESNG[i] == nTrayNo && gData.nCmNoMESNG[i] == nCmNo)
		{
			gData.nInspectInfo[nPx][nTx][nCx] = 4;
			gMes.sJudge[nPx][nTx][nCx] = "N1";

			nInfo = gData.nInspectInfo[nPx][nTx][nCx];		

			strLog.Format("MES NG, CM less than MES Count Info, PortNo(%d), TrayNo(%d), CmNo(%d)", nPx+1, nTx+1, nCx+1);
			g_objLogFile.Save_HandlerLog(strLog);
		}
	}

#ifdef VISION_REPEAT
	nInfo = gData.nInspectInfo[nPx][nTx][nCx] = 1;	// 반복성 검증일때 모두 양품
#endif

	gData.byInspectDone[nPx][nTx][nCx] |= (1 << 7);	// 판정 완료 (2번 판정하지 않기 위해)
	return TRUE;	// All Inspect Done
}

void CSequenceMain::Set_InspectError(int nPortNo, int nTrayNo, int nCmNo)
{
	int	nPx = nPortNo - 1;	// Port Index
	int	nTx = nTrayNo - 1;	// Tray Index
	int	nCx = nCmNo - 1;	// CM Index

	// byInspectDone[2][30][40] : 검사완료 (7bit 사용 ==> 0:Angle, 1:Btm1_SP, 2:top1, 3:Top2, 4:Btm2, 5:Btm1_AG, 5:Btm1_3D)
	CString strErrMsg = "";
	if		(m_pEquipData->bUseInspectAngle && ((gData.byInspectDone[nPx][nTx][nCx] >> 0) & 1) == 0) strErrMsg.Format(" #==> Tray:%d. CM:%d. Angle", nTx+1, nCx+1);	// Angle
	else if	(m_pEquipData->bUseInspectBtm1Specular  && ((gData.byInspectDone[nPx][nTx][nCx] >> 1) & 1) == 0) strErrMsg.Format(" #==> Tray:%d. CM:%d. Btm1_SP",  nTx+1, nCx+1);	// Btm1_SP
	else if (m_pEquipData->bUseInspectTop1  && ((gData.byInspectDone[nPx][nTx][nCx] >> 2) & 1) == 0) strErrMsg.Format(" #==> Tray:%d. CM:%d. Top1",  nTx+1, nCx+1);	// Top1
	else if (m_pEquipData->bUseInspectTop2  && ((gData.byInspectDone[nPx][nTx][nCx] >> 3) & 1) == 0) strErrMsg.Format(" #==> Tray:%d. CM:%d. Top2",  nTx+1, nCx+1);	// Top2
	else if (m_pEquipData->bUseInspectBtm2  && ((gData.byInspectDone[nPx][nTx][nCx] >> 4) & 1) == 0) strErrMsg.Format(" #==> Tray:%d. CM:%d. Btm2",  nTx+1, nCx+1);	// Btm2
	else if	(m_pEquipData->bUseInspectBtm1Angle  && ((gData.byInspectDone[nPx][nTx][nCx] >> 5) & 1) == 0) strErrMsg.Format(" #==> Tray:%d. CM:%d. Btm1_AG",  nTx+1, nCx+1);	// Btm1_AG
	else if	(m_pEquipData->bUseInspectBtm13D  && ((gData.byInspectDone[nPx][nTx][nCx] >> 6) & 1) == 0) strErrMsg.Format(" #==> Tray:%d. CM:%d. Btm1_3D",  nTx+1, nCx+1);	// Btm1_3D

	if (strErrMsg != "") g_objCommon.Set_ErrorSubMessage(strErrMsg);
}

void CSequenceMain::Set_ScanError(int nErrNo, int nTrayNo1, int nTrayNo2, int nTrayNo3, int nTrayNo4, int nCmNo1, int nCmNo2, int nCmNo3, int nCmNo4)
{
	CString strErrMsg = "";
	if		(nErrNo == 3315) strErrMsg.Format(" #==> Tray:%d. CM:%d. Angle", nTrayNo1, nCmNo1);	// Angle
	else if	(nErrNo == 3610) strErrMsg.Format(" #==> Tray:%d. %d. %d. %d. CM:%d. %d. %d. %d. Btm1", nTrayNo1, nTrayNo2, nTrayNo3, nTrayNo4, nCmNo1, nCmNo2, nCmNo3, nCmNo4);	// Btm1
	else if (nErrNo == 3710 || nErrNo == 3810 || nErrNo == 3910) strErrMsg.Format(" #==> Tray:%d. %d. %d. %d. CM:%d. %d. %d. %d. Top1",  nTrayNo1, nTrayNo2, nTrayNo3, nTrayNo4, nCmNo1, nCmNo2, nCmNo3, nCmNo4);	// Top1
	else if (nErrNo == 3720 || nErrNo == 3820 || nErrNo == 3920) strErrMsg.Format(" #==> Tray:%d. %d. CM:%d. %d Top2",  nTrayNo1, nTrayNo2, nCmNo1, nCmNo2);	// Top2
	else if (nErrNo == 4010) strErrMsg.Format(" #==> Tray:%d. %d. %d. %d. CM:%d. %d. %d. %d. Btm2",  nTrayNo1, nTrayNo2, nTrayNo3, nTrayNo4, nCmNo1, nCmNo2, nCmNo3, nCmNo4);	// Btm2

	if (strErrMsg != "") g_objCommon.Set_ErrorSubMessage(strErrMsg);
}

void CSequenceMain::Write_LotJudge(int nPortNo, int nTrayNo, int nCmNo, int nInfo, int nSpNo)
{
	CString strTemp;
	int nPx = nPortNo - 1;
	int nTx = nTrayNo - 1;
	int nCx = nCmNo - 1;
	if (nPx < 0 || nPx > 1 || nTx < 0 || nTx > 29 || nCx < 0 || nCx > 39 || nInfo == 0) {
		strTemp.Format("Exception Write Judge(1) : %d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo);
		g_objLogFile.Save_TestLog(strTemp);
		return;
	}

	char chCode[7];	// 판정코드 (0:AG, 1:B1SP, 2:T1, 3:T2, 4:B2 5:B1AG 6:B13D)
	if(nPx == 0){
		memcpy(chCode, gData.cJudgeCode1[nTx][nCx], sizeof(char) * 7);
	}
	if(nPx == 1){
		memcpy(chCode, gData.cJudgeCode2[nTx][nCx], sizeof(char) * 7);
	}
	for (int i = 0; i < 7; i++) {
		if(i == 0 && m_pEquipData->bUseInspectAngle && chCode[i] == 0)
		{
			chCode[i] = 0x6E; //  비전 결과가 안오면 공란으로 두지말고 E으로 기록해 달라. (From kjw request)
			strTemp.Format("Exception Write Judge(2) : %d,%d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo, i);
			g_objLogFile.Save_TestLog(strTemp);
		}
		else if(i == 1 && m_pEquipData->bUseInspectBtm1Specular && chCode[i] == 0)
		{
			chCode[i] = 0x6E; //  비전 결과가 안오면 공란으로 두지말고 E으로 기록해 달라. (From kjw request)
			strTemp.Format("Exception Write Judge(2) : %d,%d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo, i);
			g_objLogFile.Save_TestLog(strTemp);
		}
		else if(i == 2 && m_pEquipData->bUseInspectTop1 && chCode[i] == 0)
		{
			chCode[i] = 0x6E; //  비전 결과가 안오면 공란으로 두지말고 E으로 기록해 달라. (From kjw request)
			strTemp.Format("Exception Write Judge(2) : %d,%d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo, i);
			g_objLogFile.Save_TestLog(strTemp);
		}
		else if(i == 3 && m_pEquipData->bUseInspectTop2 && chCode[i] == 0)
		{
			chCode[i] = 0x6E; //  비전 결과가 안오면 공란으로 두지말고 E으로 기록해 달라. (From kjw request)
			strTemp.Format("Exception Write Judge(2) : %d,%d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo, i);
			g_objLogFile.Save_TestLog(strTemp);
		}
		else if(i == 4 && m_pEquipData->bUseInspectBtm2 && chCode[i] == 0)
		{
			chCode[i] = 0x6E; //  비전 결과가 안오면 공란으로 두지말고 E으로 기록해 달라. (From kjw request)
			strTemp.Format("Exception Write Judge(2) : %d,%d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo, i);
			g_objLogFile.Save_TestLog(strTemp);
		}
		else if(i == 5 && m_pEquipData->bUseInspectBtm1Angle && chCode[i] == 0)
		{
			chCode[i] = 0x6E; //  비전 결과가 안오면 공란으로 두지말고 E으로 기록해 달라. (From kjw request)
			strTemp.Format("Exception Write Judge(2) : %d,%d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo, i);
			g_objLogFile.Save_TestLog(strTemp);
		}
		else if(i == 6 && m_pEquipData->bUseInspectBtm13D && chCode[i] == 0)
		{
			chCode[i] = 0x6E; //  비전 결과가 안오면 공란으로 두지말고 E으로 기록해 달라. (From kjw request)
			strTemp.Format("Exception Write Judge(2) : %d,%d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo, i);
			g_objLogFile.Save_TestLog(strTemp);
		}
		else if (chCode[i] == 0) {
			chCode[i] = 0x45; //  비전 결과가 안오면 공란으로 두지말고 E으로 기록해 달라. (From kjw request)
			strTemp.Format("Exception Write Judge(2) : %d,%d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo, i);
			g_objLogFile.Save_TestLog(strTemp);
		}
		
	}
	
	CString strCode;	// Log 순서 : AG, B1_SP, T1, T2, B2, B1_AG, B1_3D, CODE
	strCode.Format("%c,%c,%c,%c,%c,%c,%c,%d", chCode[0], chCode[1], chCode[2], chCode[3], chCode[4], chCode[5], chCode[6], nInfo);	// 마지막 CODE열 추가, 불량 종류 숫자 표시
	CString strResult = (nInfo == 1 ? "Pass" : "Fail");
	for (int i = 0; i < 7; i++)
	{
		if(chCode[i] == 0x6E) strResult = "Fail";
	}

 	m_strLog.Format("%s,%s,%s,%d,%d,%s,%d,%s,%s", m_pEquipData->sEquipName, MAIN_VERSION, gData.sLotID[nPx], nTrayNo, nCmNo, gMes.sBarID[nPx][nTx][nCx], nSpNo, strCode, strResult);
	g_objLogFile.Save_LotTime(nPortNo, m_strLog);
}

// 한번에 2Lot 이상 들어가면 안된다.
BOOL CSequenceMain::Check_LoadTrayLoading(int nPNo)
{
	if (gData.nPNoAnglePort[0] == nPNo) return FALSE;
	if (gData.nPNoBtm1Pick == nPNo) return FALSE;
	if (gData.nPNoInspect[0] == nPNo) return FALSE;
	if (gData.nPNoInspect[1] == nPNo) return FALSE;
	if (gData.nPNoInspect[2] == nPNo) return FALSE;
	if (gData.nPNoBtm1Pick == nPNo) return FALSE;
	if (gData.nPNoBuffTray[0] == nPNo) return FALSE;
	if (gData.nPNoBuffTray[1] == nPNo) return FALSE;
	if (gData.nPNoGoodTray == nPNo) return FALSE;
	if (gData.nPNoNgTray == nPNo) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Check_LoadLotEnd(int nPNo, int nMode)
{
	// 로드가 다끝나고 앞공정에 모듈이 없으면 TRUE
	if (!gData.bCycleStop && (gData.nLoadTrayCount[nPNo-1] < gData.nTrayUseCount[nPNo-1])) return FALSE;
	if (gData.nPNoAnglePort[0] == nPNo) return FALSE;
	if (gData.nPNoAngleTray[0] == nPNo || gData.nPNoAngleTray[1] == nPNo) return FALSE;
// 	if (!Check_AngleTrayEmpty()) return FALSE;
	if (nMode == 0 && !Check_Btm1PickerEmpty()) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Check_LoadLotEndBtm1(int nPNo, int angletrayNo)
{
	// 로드가 다끝나고 앞공정에 모듈이 없으면 TRUE
	if (!gData.bCycleStop && (gData.nLoadTrayCount[nPNo-1] < gData.nTrayUseCount[nPNo-1])) return FALSE;
	if (gData.nPNoAnglePort[0] != 0) return FALSE;
	if (gData.nPNoAngleTray[angletrayNo] != 0) return FALSE;
// 	if (!Check_AngleTrayEmpty()) return FALSE;
	
	return TRUE;
}

BOOL CSequenceMain::Check_UnloadLotEnd(int nMode)
{
// 	if (!m_bLoadLotEnd) return FALSE;
	if (m_pDX01->iLoadPickerExist) return FALSE;
	if (m_pDX02->iAnglePortExist) return FALSE;
	if (m_nAngleTray1Case == 30 && m_pDX02->iAngleStage1Exist) return FALSE;	// 빈트레이 언로딩 해야한다.
	if (m_nAngleTray2Case == 30 && m_pDX02->iAngleStage2Exist) return FALSE;	// 빈트레이 언로딩 해야한다.
	if (!Check_AngleTrayEmpty()) return FALSE;
	if (!Check_Btm1PickerEmpty()) return FALSE;
	if (!Check_InspetionEmpty(1)) return FALSE;
	if (!Check_InspetionEmpty(2)) return FALSE;
	if (!Check_InspetionEmpty(3)) return FALSE;
	if (!Check_Btm2PickerEmpty()) return FALSE;
	if (!Check_BufferEmpty()) return FALSE;
	if (nMode == 0 && !Check_NgBufferEmpty(1)) return FALSE;
	if (nMode == 0 && !Check_NgBufferEmpty(2)) return FALSE;
	if (nMode == 0 && !Check_SortPickerEmpty(1)) return FALSE;
	if (nMode == 0 && !Check_SortPickerEmpty(2)) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Check_Btm2LoadEnd(int nPNo)
{
	if (gData.nPNoAnglePort[0] == nPNo) return FALSE;
	if (gData.nPNoAngleTray[0] == nPNo) return FALSE;
	if (gData.nPNoAngleTray[1] == nPNo) return FALSE;
	if (gData.nPNoBtm1Pick == nPNo) return FALSE;
	if (gData.nPNoInspect[0] == nPNo) return FALSE;
	if (gData.nPNoInspect[1] == nPNo) return FALSE;
	if (gData.nPNoInspect[2] == nPNo) return FALSE;
	if (gData.nPNoBtm2Pick == nPNo) return FALSE;
	return TRUE;
}

BOOL CSequenceMain::Check_InspectLotEnd(int nPNo, int nSortPickNo)
{
// 	if (nSortPickNo == 1 && (gData.nPNoSortPick[1] == nPNo)) return FALSE;
// 	if (nSortPickNo == 2 && (gData.nPNoSortPick[0] == nPNo)) return FALSE;
// 	if (gData.nPNoNgBuffer[0] == nPNo) return FALSE;
// 	if (gData.nPNoNgBuffer[1] == nPNo) return FALSE;
	if (gData.nPNoSortPick[0] == nPNo) return FALSE;
	if (gData.nPNoSortPick[1] == nPNo) return FALSE;
	if (gData.nPNoBuffTray[0] == nPNo) return FALSE;
	if (gData.nPNoBuffTray[1] == nPNo) return FALSE;
	if (gData.nPNoBtm2Pick == nPNo) return FALSE;
	if (gData.nPNoInspect[0] == nPNo) return FALSE;
	if (gData.nPNoInspect[1] == nPNo) return FALSE;
	if (gData.nPNoInspect[2] == nPNo) return FALSE;
	if (gData.nPNoBtm1Pick == nPNo) return FALSE;
	if (gData.nPNoAngleTray[0] == nPNo) return FALSE;
	if (gData.nPNoAngleTray[1] == nPNo) return FALSE;
	if (gData.nPNoAnglePort[0] == nPNo) return FALSE;
	if (gData.nPNoTrayPick == nPNo) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Check_NgBufferLotEnd(int nPNo)
{
	if (gData.nPNoNgBuffer[0] == nPNo) return FALSE;
	if (gData.nPNoNgBuffer[1] == nPNo) return FALSE;
	return TRUE;
}

BOOL CSequenceMain::Get_VisionInspectUse()
{
	BOOL bUse = (m_pEquipData->bUseInspectAngle || m_pEquipData->bUseInspectBtm1Specular || m_pEquipData->bUseInspectTop1 || m_pEquipData->bUseInspectTop2 || m_pEquipData->bUseInspectBtm2
		|| m_pEquipData->bUseInspectBtm1Angle|| m_pEquipData->bUseInspectBtm13D );
	return bUse;
}

int	CSequenceMain::Get_GoodTrayCmCnt()
{
	int Cnt = 0;
	for (int i=0; i<gData.nTrayY; i++) {
		for (int j=0; j<gData.nTrayX; j++) {
			if (gData.InfoGoodTray[i][j] > 0) Cnt++;
		}
	}
	return Cnt;
}

///////////////////////////////////////////////////////////////////////////////

void CSequenceMain::Job_LotStart(int nPortNo)
{
	int nLPNo = nPortNo - 1;
	gData.nLPNo = nPortNo;
	g_objInspector.Set_LotStart(INSPECTOR_ALL, gData.sLotID[nLPNo], nPortNo, m_pEquipData->sVisionProgVer, m_pEquipData->sVisionParaVer);
	gLot.bLotEndComplete[nLPNo] = FALSE;
	g_objDispatcher.Set_LotStart(nPortNo);
// 	g_objCapAttach.Set_LotStart(nPortNo);	// 시점 변경 (GoodTray1/2) -> (SortPicker1/2)

	SYSTEMTIME time;
	GetLocalTime(&time);

	gLot.sLotID[nLPNo] = gData.sLotID[nLPNo];
	gLot.sStartTime[nLPNo].Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	m_dwFirstLoad = gLot.dwLotStart[nLPNo] = GetTickCount();
	gLot.nTrayCount[nLPNo] = gData.nTrayUseCount[nLPNo];
	gLot.nCmCount[nLPNo] = gData.nCmUseCount[nLPNo];

	gLot.nBsNgCount[nLPNo] = 0;	// Barcode Shift 불량 수량 초기화
 	gLot.nErrorCount = 0;
 	if (gData.bFirstLotStart) {
		gData.bFirstLotStart = FALSE;
		gLot.dwRunTime = gLot.dwErrorTime = gLot.dwStopTime = 0;
	}

	// Log 표준화
	CString strModel = (gData.sLotID[nLPNo].GetLength() < 8 ? "MODEL" : gData.sLotID[nLPNo].Mid(3, 5));

	if (gData.nStdLogDay != time.wDay) {
		CIniFileCS INI(gData.sEnvPath + "\\EquipData.ini");
		gData.nStdLotNo = 0;
		gData.nStdLogDay = (int)time.wDay;
		INI.Set_Integer("STD_LOG", "STD_LOT_NO", gData.nStdLotNo);
		INI.Set_Integer("STD_LOG", "STD_LOG_DAY", gData.nStdLogDay);
	}

	//PC Information Log 파일 생성
	CString strLog, strCPU, strRAM, strHardDisk;
	g_objCommon.Get_CPUInfo(strCPU);
	g_objCommon.Get_RAMSize(strRAM);
	g_objCommon.Get_HardInfo(strHardDisk);
		
	strLog.Format("%s,%s,%s,%s,%s",gData.sLotID[nLPNo],gData.sRecipe, strCPU,strRAM,strHardDisk);
	g_objLogFile.Save_PCLog(nLPNo+1, strLog);

	// Motion 파일명 : LotID_생성년월일시_공정명_Normal_#호기번호_PC이름_모델명_순번.csv
	gData.sStdMotionFile.Format("%s_%04d%02d%02d%02d_EOL6SIDE_Normal_#%04d_%s_%s_%04d.csv",
		gData.sLotID[nLPNo], time.wYear, time.wMonth, time.wDay, time.wHour, gData.nStdEqNo, gsComputerName, strModel, gData.nStdLotNo);

	// Efficiency 파일명 : LotID_생성년월일시_공정명_Efficiency_#호기번호_PC이름_모델명_순번.csv
	gData.sEfficiencyFile.Format("%s_%04d%02d%02d%02d_EOL6SIDE_Efficiency_#%04d_%s_%s_%04d.csv",
		gData.sLotID[nLPNo], time.wYear, time.wMonth, time.wDay, time.wHour, gData.nStdEqNo, gsComputerName, strModel, gData.nStdLotNo);

	g_dlgWork.PostMessage(UM_LOT_START_END, (WPARAM)1, nPortNo);	// LotStart
}

void CSequenceMain::Job_LotEnd(int nPortNo, int nGTNo)
{
	int nPx = nPortNo - 1;
	if (gLot.nCmCount[nPx] < 1) return;	// Error
	if (gLot.bLotEndComplete[nPx] == TRUE) return;

	if(gLot.nSNgCount[nPx][0] > 0) g_dlgWork.PostMessage(UM_SHOW_MSG, 4, NULL); 
	
	gLot.bLotEndComplete[nPx] = TRUE;

	gData.nTrayUseCount[0] = gData.nTrayUseCount[1] = 0;
	gData.nCmUseCount[0] = gData.nCmUseCount[1] = 0;

	SYSTEMTIME time;
	GetLocalTime(&time);
	gLot.dwLotEnd[nPx] = GetTickCount();
	gLot.sEndTime[nPx].Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	g_objInspector.Set_LotEnd(INSPECTOR_ALL, gData.sLotID[nPx], nPortNo);
	g_objDispatcher.Set_LotEnd(nPortNo);

	g_objCapAttach.Set_TrayEnd(nPortNo);
	theApp.uSleep(30);
	g_objCapAttach.Set_LotEnd(nPortNo);
	
	if (gData.bCycleStop == FALSE) {
		if (gData.bAPDResultErr == FALSE) {	// APD 결과 항목들이 모두 OK이면 MES LotEnd 보고
			g_objMES.Set_LotEnd(gLot.sLotID[nPx], gLot.nCmCount[nPx], gData.sOperID, gLot.nGoodCount[nPx], gLot.nNgCount[nPx], nPx);
		}
	}

	//gLot.dwUphStart : lot start 이후 sort picker 1번이 처음 집는 시점 
	//gLot.dwLotEnd[nPx] : Lot End 시점 
	DWORD dwTime = gLot.dwLotEnd[nPx] - gLot.dwUphStart;//gLot.dwLotEnd[nPx] - gLot.dwLotStart[nPx];
	int iCmCount = gLot.nCmCount[nPx]-8;
	if(iCmCount <= 0) iCmCount = 1;
	gLot.dTackTime = dwTime / 1000.0 / (iCmCount);	// Floating-point inexact result
	m_dwLastUnLoad = gLot.dwLotEnd[nPx] - m_dwLastUnLoad;

	m_strLog.Format("LotID,%s,Start_Time,%s,End_Time,%s,Time,%d,Tray_Count,%02d,CM_Count,%04d,Tack,%0.7lf,%d,%d",
		gLot.sLotID[nPx], gLot.sStartTime[nPx], gLot.sEndTime[nPx], dwTime, gLot.nTrayCount[nPx], gLot.nCmCount[nPx], gLot.dTackTime, gLot.nGoodCount[nPx], gLot.nNgCount[nPx]);
	g_objLogFile.Save_HandlerLog(m_strLog);

	double dOne = (double)(dwTime - m_dwFirstLoad - m_dwLastUnLoad) / (gLot.nCmCount[nPx] - 1);
	m_strLog.Format("LotID(%s), Load(%0.3lf), Unload(%0.3lf), Takt as 600 EA Run(%0.6lf)",
		gLot.sLotID[nPx], (double)m_dwFirstLoad / 1000, (double)m_dwLastUnLoad / 1000, (dOne * 599 + m_dwFirstLoad + m_dwLastUnLoad) / 600000.0);
	g_objLogFile.Save_HandlerLog(m_strLog);

	
	m_strLog.Format("%s,%s,%s,%d,%02d,%04d,%0.7lf,%d,%d,%d,%d,%d,%d",
		gLot.sLotID[nPx], gLot.sStartTime[nPx], gLot.sEndTime[nPx], dwTime, gLot.nTrayCount[nPx], gLot.nCmCount[nPx], gLot.dTackTime, gLot.nGoodCount[nPx], gLot.nNgCount[nPx],
		gLot.nSNgCount[nPx][1], gLot.nSNgCount[nPx][2], gLot.nSNgCount[nPx][3], gLot.nSNgCount[nPx][5], gLot.nSNgCount[nPx][0]);
	g_objLogFile.Save_JobListLog(m_strLog, TRUE);

	// UPH & 생산량
	gUph.dTaktTime = gLot.dTackTime;

	if (time.wHour >= 7 && time.wHour < 19) gUph.nCmCount[0] += gLot.nCmCount[nPx];
	else gUph.nCmCount[1] += gLot.nCmCount[nPx];

	int i = (int)time.wHour;
	int j = gUph.nLotCount[i];
	if (j > 49) return;		// 1시간에 LOT 수량 MAX 50개

	gUph.dTakt[i][j] = gLot.dTackTime;
	gUph.nLotCount[i] = j + 1;

	// Log 표준화
	gData.nStdLotNo++;
	CIniFileCS INI(gData.sEnvPath + "\\EquipData.ini");
	INI.Set_Integer("STD_LOG", "STD_LOT_NO", gData.nStdEqNo);

	g_dlgWork.PostMessage(UM_LOT_START_END, (WPARAM)2, nPortNo);	// LotEnd

	int nSum = gLot.nGoodCount[nPx] + gLot.nNgCount[nPx];
	double dRate = (nSum == 0 ? 0.0 : gLot.nGoodCount[nPx] * 100.0 / nSum);
	// Lot Result [날짜,Lot Start,Lot End,Term,호기,Lot ID,투입수,양품수,불량수,불량률,HREN(H),HREP(P),Spider(S),BA(B),Chip(C),MES(M)];
	CString strMsg, strStart, strEnd, strTerm, strEquip;

	strStart.Format("%s:%s:%s", gLot.sStartTime[nPx].Mid(9, 2), gLot.sStartTime[nPx].Mid(11, 2), gLot.sStartTime[nPx].Mid(13, 2));
	strEnd.Format("%s:%s:%s", gLot.sEndTime[nPx].Mid(9, 2), gLot.sEndTime[nPx].Mid(11, 2), gLot.sEndTime[nPx].Mid(13, 2));
	int nSec = (int)(gLot.dwLotEnd[nPx] - gLot.dwLotStart[nPx]) / 1000;
	int nH = nSec / 60 / 60;
	int nM = (nSec - nH * 60 * 60) / 60;
	int nS = (nSec - nH * 60 * 60) % 60;
	strTerm.Format("%02d:%02d:%02d", nH, nM, nS);
	strEquip = m_pEquipData->sEquipName.Right(2);
	double dNgRate = (nSum == 0 ? 0.0 : gLot.nNgCount[nPx] * 100.0 / nSum);

	strMsg.Format("%s,%s,%s,%s,%s,%d,%d,%d,%0.2lf%%,%d,%d,%d",
		strStart, strEnd, strTerm, strEquip, gLot.sLotID[nPx], nSum, gLot.nGoodCount[nPx], gLot.nNgCount[nPx], dNgRate,
		gLot.nSNgCount[nPx][1], gLot.nSNgCount[nPx][2], gLot.nSNgCount[nPx][0]);

	g_objLogFile.Save_LotResult(nPx, strMsg);

	//Time, Lot_ID, Total, Good, NG, RosRequest, RosGood, RosNg, RosRepair, Filtering1, Filtering2, RateAvi, Final, Tack
	double dFiltering1 = (gLot.nRosRequest[nPx] == 0 ? 0.0 : (gLot.nRosGood[nPx] * 100.0) / gLot.nRosRequest[nPx]);
	double dFiltering2 = (nSum == 0 ? 0.0 : (gLot.nRosGood[nPx] * 100.0) / nSum);
	double dRateAvi = (nSum == 0 ? 0.0 : ((gLot.nGoodCount[nPx] - gLot.nRosGood[nPx]) * 100.0) / nSum);

	strMsg.Format("\t%s \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%0.2lf \t%0.2lf \t%0.2lf \t%0.2lf \t%0.3lf",
		gLot.sLotID[nPx], nSum, gLot.nGoodCount[nPx], gLot.nNgCount[nPx], gLot.nRosRequest[nPx], gLot.nRosGood[nPx], gLot.nRosNg[nPx], gLot.nRosRepair[nPx], gLot.nRosTimeOut[nPx],
		dFiltering1, dFiltering2, dRateAvi, dRate, gLot.dTackTime);
	g_objLogFile.Save_DailyLot(strMsg);

	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_LotStateTime();

	strStart.Format("%s-%s-%s %s:%s:%s",
		gLot.sStartTime[nPx].Mid(0, 4), gLot.sStartTime[nPx].Mid(4, 2), gLot.sStartTime[nPx].Mid(6, 2), gLot.sStartTime[nPx].Mid(9, 2), gLot.sStartTime[nPx].Mid(11, 2), gLot.sStartTime[nPx].Mid(13, 2));
	strEnd.Format("%s-%s-%s %s:%s:%s",
		gLot.sEndTime[nPx].Mid(0, 4), gLot.sEndTime[nPx].Mid(4, 2), gLot.sEndTime[nPx].Mid(6, 2), gLot.sEndTime[nPx].Mid(9, 2), gLot.sEndTime[nPx].Mid(11, 2), gLot.sEndTime[nPx].Mid(13, 2));
	strMsg.Format("%s,%s,%s,%0.3lf,%d,%0.3lf,%0.3lf,%0.3lf,%d,%0.2lf,%0.3lf,%0.3lf,%0.2lf",
		gLot.sLotID[nPx], strStart, strEnd, gLot.dTackTime, nSum, (double)(gLot.dwRunTime) / 1000, (double)(gLot.dwStopTime) / 1000, (double)(gLot.dwErrorTime) / 1000,
		gLot.nErrorCount, dRate, 3600 / gLot.dTackTime ,(double)(gLot.dwRunTime + gLot.dwErrorTime + gLot.dwStopTime) / gLot.nErrorCount);
	g_objLogFile.Save_OperatingRatio(strMsg);

	gLot.dwRunTime = gLot.dwErrorTime = gLot.dwStopTime = 0;

	g_objLogFile.Save_AverageCycle(nPx);
	g_objLogFile.Save_LotLog(nPortNo);	//gjcs

	// Jig별 불량율 계산.
	BOOL bJigErr = FALSE;
	if (m_pEquipData->nJigNgRate > 0 && m_pEquipData->nJigNgCnt > 0) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 10; j++) {
				gData.nJigNgOverCnt[i][j] = 0;
				gData.dJigNgOver[i][j] = 0.0;
				if (gData.nJigNgCnt[nPx][i][j] < m_pEquipData->nJigNgCnt) continue;	// Jig Ng 최소 수량 이상일때만 Rate 확인.
				dNgRate = (double)(gData.nJigNgCnt[nPx][i][j]) / (double)(gLot.nNgCount[nPx]) * 100;	// 각 Jig 불량수/전체 불량수
				if (dNgRate > (m_pEquipData->nJigNgRate - 0.01)) {
					gData.nJigNgOverCnt[i][j] = gData.nJigNgCnt[nPx][i][j];
					gData.dJigNgOver[i][j] = dNgRate;
					bJigErr = TRUE;
				}
			}
		}
	}

	// 해당포트 판정 완료, 결과 초기화
	g_objDispatcher.Reset_JudgeData(nPortNo);
	memset(gData.byInspectDone[nPx], 0x00, sizeof(BYTE) * 30 * 40);
	memset(gData.nInspectInfo[nPx], 0x00, sizeof(int) * 30 * 40);
	memset(gData.bRosDone[nPx], 0x00, sizeof(BOOL) * 30 * 40);
	memset(gData.nJigNgCnt[nPx], 0x00, sizeof(int) * 8 * 10 );	// Jig별 불량수량
	if(nPortNo == 1) memset(gData.cJudgeCode1, 0x00, sizeof(char) *  30 * 40 * 7);
	if(nPortNo == 2) memset(gData.cJudgeCode2, 0x00, sizeof(char) *  30 * 40 * 7);

	gData.nPNoNgTray = gData.nPNoGoodTray = 0;
	gData.nGoodTrayCount = 0;

	gLot.nGoodCount[nPx] = gLot.nNgCount[nPx] = 0;
	for (int i = 0; i < 6; i++) gLot.nSNgCount[nPx][i] = 0; 
	gLot.nRosRequest[nPx] = gLot.nRosGood[nPx] = gLot.nRosNg[nPx] = gLot.nRosRepair[nPx] = gLot.nRosTimeOut[nPx] = 0;

	gData.sLotID[nPx] = "LOT_ID";
	gData.nCmUseCount[nPx] = 0;
	gData.nTrayUseCount[nPx] = 0;	

	gData.bLoadLampOn[nPx] = TRUE;

	g_dlgWork.PostMessage(UM_LOT_INFO_CLEAR, nPx, NULL);
	g_dlgWork.PostMessage(UM_UPDATE_UPH, NULL, NULL);
	if (bJigErr) g_objCommon.Show_Error(6155);	// 특정 Jig 다량 불량
	if (gLot.nBsNgCount[nPx] > 0) {	// Barcode Shift 불량 발생시 알람
		CString strErrMsg = "";
		strErrMsg.Format("#==> %d ea", gLot.nBsNgCount[nPx]);
		g_objCommon.Set_ErrorSubMessage(strErrMsg);
		g_objCommon.Show_Error(6180);
	}
//	Beep_Post(1000);
}

void CSequenceMain::Beep_Post(int nState, int nTime)
{
	if (nTime < 500) nTime = 500;

	if (nState == 0) {	// Lot End
		m_pDY14->oBuzzerBit0 = m_pEquipData->bBuzzer[0][0];
		m_pDY14->oBuzzerBit1 = m_pEquipData->bBuzzer[0][1];
		m_pDY14->oBuzzerBit2 = m_pEquipData->bBuzzer[0][2];
		m_pDY14->oBuzzerBit3 = m_pEquipData->bBuzzer[0][3];
		m_pDY14->oBuzzerBit4 = m_pEquipData->bBuzzer[0][4];

	} else if (nState == 1) {	// NG Full
		m_pDY14->oBuzzerBit0 = TRUE;	//m_pEquipData->bBuzzer[1][0];
		m_pDY14->oBuzzerBit1 = FALSE;	//m_pEquipData->bBuzzer[1][1];
		m_pDY14->oBuzzerBit2 = FALSE;	//m_pEquipData->bBuzzer[1][2];
		m_pDY14->oBuzzerBit3 = FALSE;	//m_pEquipData->bBuzzer[1][3];
		m_pDY14->oBuzzerBit4 = FALSE;	//m_pEquipData->bBuzzer[1][4];
	}
	g_objAJinAXL.Write_Output(14);
	theApp.uSleep(nTime);
	m_pDY14->oBuzzerBit0 = FALSE;
	m_pDY14->oBuzzerBit1 = FALSE;
	m_pDY14->oBuzzerBit2 = FALSE;
	m_pDY14->oBuzzerBit3 = FALSE;
	m_pDY14->oBuzzerBit4 = FALSE;
	g_objAJinAXL.Write_Output(14);
}

///////////////////////////////////////////////////////////////////////////////
// 1. (Error : 3100)
BOOL CSequenceMain::LoadTray_Run()
{
	int nTaktZone = 1;		// Takt_Start, Takt_End
	static int nLtWorkPort = 0;			// 작업중인 Port (1 ~ 2)
	static BOOL bAlignFirst = FALSE;	// Tray Align 재확인

	// LJH 1번 투입시 2번에 Tray 있으면 가져가야한다...
	// Load Tray Lot 투입시 도어락, 버튼 확인하여 Tray Loading 작업 못하게 막아준다.
	if (((m_bLotLoadEnable[0] == TRUE && (m_pEquipData->bUseDoorLock && m_pDX15->iDoor02Unlock) && (nLtWorkPort == 1 && m_nLoadTrayCase > 3 && m_nLoadTrayCase < 10)) ||
		(m_bLotLoadEnable[1] == TRUE && (m_pEquipData->bUseDoorLock && m_pDX15->iDoor03Unlock) && (nLtWorkPort == 2 && m_nLoadTrayCase > 3 && m_nLoadTrayCase < 10)))) return TRUE;

	switch (m_nLoadTrayCase) {
 	case 0:		// Start 시 1로 진행
		nLtWorkPort = 0;
 		return TRUE;
 
	case 1:		// Load Tray, Port 확인.
		if (m_bUnloadLotEnd || gData.bCycleStop) {
			nLtWorkPort = 0;
			m_nLoadTrayCase = 0;	// 사이클스탑이면 투입된 모듈 트레이까지만 작업 후 종료.

		} else if ((m_pDX00->iLoadPort1Bottom || m_pDX00->iLoadPort2Bottom) && !gData.bCycleStop) {
			// 맨처음 시작할때 2개 모두 적재 되어있으면 1번부터 가져간다.
			// 작업할 포트에 트레이 유무와 입력수량확인.
			if ((nLtWorkPort == 0 || nLtWorkPort == 2) && m_pDX00->iLoadPort1Bottom && (gData.sLotID[0] != "LOT_ID")) nLtWorkPort = 1;
			else if ((nLtWorkPort == 0 || nLtWorkPort == 1) && m_pDX00->iLoadPort2Bottom && (gData.sLotID[1] != "LOT_ID")) nLtWorkPort = 2;

			if (!Check_LoadTrayLoading(nLtWorkPort)) return TRUE;	// 같은 Port Lot을 동시에 검사하지 않는다.

			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(5000);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 1, TRUE);						
		}
		break;
	case 2:		// Tray X Move to Load Position
		if (gData.bCycleStop) {
			nLtWorkPort = 0;
			m_nLoadTrayCase = 0;	// 사이클스탑이면 투입된 모듈 트레이까지만 작업 후 종료.
		}		
			
		if ((nLtWorkPort == 1 && !m_pDX00->iLoadPort1Bottom) || (nLtWorkPort == 2 && !m_pDX00->iLoadPort2Bottom)) break;

		if (nLtWorkPort == 1 && !g_objCommon.Check_Position(AX_LOAD_STAGE_X, 0)) g_objCommon.Move_Position(AX_LOAD_STAGE_X, 0);	// Port1
		if (nLtWorkPort == 2 && !g_objCommon.Check_Position(AX_LOAD_STAGE_X, 1)) g_objCommon.Move_Position(AX_LOAD_STAGE_X, 1);	// Port2

		if (gData.nLoadTrayCount[nLtWorkPort-1] == 0) {
			if (g_objMES.m_bMESUse) {
				if (gData.bMesFirstLot) gData.bMesFirstLot = FALSE;
				else
				{	
					// 연속 랏 관련하여 MES Lot Start 부분 수정.
					g_objMES.m_bMesStart = TRUE;
					g_dlgWork.Get_LotInfo(nLtWorkPort);	// 입력된 Lot 정보를 다시 얻는다.
					g_objMES.Set_JobReady(gData.sLotID[nLtWorkPort-1], gData.nCmUseCount[nLtWorkPort-1], gData.sOperID, nLtWorkPort);	//2020.9.16+
					m_strLog.Format("[Sequence] Set_JobReady. (LotID:%s, CmCnt:%d, Port:%d)", gData.sLotID[nLtWorkPort-1], gData.nCmUseCount[nLtWorkPort-1], nLtWorkPort);
					g_objLogFile.Save_MesAgentLog(m_strLog);					
				}
			}
		}	
		m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(30000);
		m_tLoadTrayLoop.Takt_End(nTaktZone, 1);
		m_tLoadTrayLoop.Takt_Start(nTaktZone, 2);
		break;
	case 3:		// Check Position, Lot Start
		if ((nLtWorkPort == 1 && g_objCommon.Check_Position(AX_LOAD_STAGE_X, 0)) || 
			(nLtWorkPort == 2 && g_objCommon.Check_Position(AX_LOAD_STAGE_X, 1))) 
		{
			if (gData.nLoadTrayCount[nLtWorkPort-1] < gData.nTrayUseCount[nLtWorkPort-1] && !gData.bCycleStop) 
			{ 
				//pass
			}
			else
			{
				break;
			}			
			m_dwLoadTray = GetTickCount();
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(5000);	// 30000
			m_tLoadTrayLoop.Takt_End(nTaktZone, 2);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 3);
		} 
		break;
	case 4:		// Tray Z Move to Support Up Position (Load)
		if (!g_objMES.m_bMESUse || g_objMES.m_nMESSequence == 3) {
			if (g_objCommon.Get_LoadTrayMasterSlaveOut() && !m_pDX01->iLoadStageExist) {
				
				g_dlgWork.Enable_UserInput(nLtWorkPort, FALSE);
				if (gData.nLoadTrayCount[nLtWorkPort-1] == 0) {
					Job_LotStart(nLtWorkPort);				
				}
				//MCC
				m_strLog.Format("Tray Get, pNo : %d", nLtWorkPort);
				g_objLogFile.Save_MCCLog(m_strLog);

				g_objCommon.Move_Position(AX_LOAD_STAGE_Z, 1);	// Support Up
				m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(10000);
				m_tLoadTrayLoop.Takt_End(nTaktZone, 3);
				m_tLoadTrayLoop.Takt_Start(nTaktZone, 4);
			}
		}
		break;
	case 5:		// Port Support Out
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 1)) 
		{			
			g_objCommon.Set_LoadPortSupportOut(nLtWorkPort);
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(5000);
			m_tLoadTrayLoop.Takt_End(nTaktZone, 4);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 5);			
		}
		break;
	case 6:		// Tray Z Move to Support Down Position
		if (g_objCommon.Get_LoadPortSupportOut(nLtWorkPort)) 
		{		
			g_objCommon.Move_Position(AX_LOAD_STAGE_Z, 2);	// Support Down
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(10000);
			m_tLoadTrayLoop.Takt_End(nTaktZone, 5);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 6);
		}
		break;
	case 7:		// Port Support In
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 2)) 
		{			
			g_objCommon.Set_LoadPortSupportIn(nLtWorkPort);
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(5000);
			m_tLoadTrayLoop.Takt_End(nTaktZone, 6);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 7);
		}
		break;
 	case 8:		// Tray Z Move to Ready Down Position
		if (g_objCommon.Get_LoadPortSupportIn(nLtWorkPort)) 
		{
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(10000);
			m_tLoadTrayLoop.Takt_End(nTaktZone, 7);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 8);
		}
		break;
	case 9:		// Tray Master In
		if (g_objCommon.Get_LoadPortSupportIn(nLtWorkPort)) 
		{			
			m_pDY01->oLoadStageMasterIn = TRUE;
			g_objAJinAXL.Write_Output(1);
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(5000);
			m_tLoadTrayLoop.Takt_End(nTaktZone, 8);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 9);
		}
		break;
	case 10:	// Tray Slave In
		if (m_pDX01->iLoadStageMasterIn && !m_pDX01->iLoadStageMasterOut)
		{
			m_pDY01->oLoadStageSlaveIn = TRUE;
			g_objAJinAXL.Write_Output(1);
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(5000);
			m_tLoadTrayLoop.Takt_End(nTaktZone, 9);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 10);
		}
		break;
	case 11:	// Check Tray Master/Slave In
		if (g_objCommon.Get_LoadTrayMasterSlaveIn() && m_pDX01->iLoadStageExist)
		{
			if (!m_tLoadTrayLoop.Waiting_Time(50)) break;
				
			g_objCommon.Move_Position(AX_LOAD_STAGE_Z, 0);
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(5000);
			m_tLoadTrayLoop.Takt_End(nTaktZone, 10);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 11);
		}
		break;
	case 12:	// Slave/Master Out
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 0)) 
		{
			if (!bAlignFirst) {
				m_pDY01->oLoadStageSlaveIn = FALSE;
				g_objAJinAXL.Write_Output(1);
				bAlignFirst = TRUE;
			} else {
				if (!m_tLoadTrayLoop.Waiting_Time(50)) break;
				m_pDY01->oLoadStageMasterIn = FALSE;
				g_objAJinAXL.Write_Output(1);
				bAlignFirst = FALSE;
				m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(5000);
				m_tLoadTrayLoop.Takt_End(nTaktZone, 11);
				m_tLoadTrayLoop.Takt_Start(nTaktZone, 12);
			}
		}
		break;
	case 13:	// Master/Slave In
		if ((!bAlignFirst && g_objCommon.Get_LoadTrayMasterSlaveOut()) || bAlignFirst) {
			if (!bAlignFirst) {
				if (!m_tLoadTrayLoop.Waiting_Time(50)) break;
				m_pDY01->oLoadStageMasterIn = TRUE;
				g_objAJinAXL.Write_Output(1);
				bAlignFirst = TRUE;
			} else {
				if (!m_tLoadTrayLoop.Waiting_Time(50)) break;
				m_pDY01->oLoadStageSlaveIn = TRUE;
				g_objAJinAXL.Write_Output(1);
				bAlignFirst = FALSE;
				m_nLoadTrayCase = 15; m_tLoadTrayLoop.Set_LoopTime(5000);
				m_tLoadTrayLoop.Takt_End(nTaktZone, 12);
				m_tLoadTrayLoop.Takt_Start(nTaktZone, 13);
			}
		}
		break;

	case 15:	// Tray X Move to Unload Position
		if (g_objCommon.Get_LoadTrayMasterSlaveIn() && m_pDX01->iLoadStageExist && g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 0)) 
		{			
			g_objCommon.Move_Position(AX_LOAD_STAGE_X, 2);	// Unload
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(20000);
			m_tLoadTrayLoop.Takt_End(nTaktZone, 13);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 15);
		}
		break;
	case 16:	// Tray Z Move to Unload Up Position
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_X, 2))
		{			
			g_objCommon.Move_Position(AX_LOAD_STAGE_Z, 3);	// Unload Up
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(10000);
			m_tLoadTrayLoop.Takt_End(nTaktZone, 15);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 16);
		}
		break;
	case 17:	// Load Tray Count 증가 (Start)
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 3) && m_pDX01->iLoadStageExist) 
		{			
			gData.nLoadTrayCount[nLtWorkPort-1]++;
			if (gData.nLoadTrayCount[nLtWorkPort-1] == 1) 
			{
				gData.nLPNo = nLtWorkPort;	// 첫번째 트레이 로딩할때
				if (gData.bVisionVerErr) {
					g_objInspector.Set_LotStart(INSPECTOR_ALL, gData.sLotID[nLtWorkPort-1], nLtWorkPort, m_pEquipData->sVisionProgVer, m_pEquipData->sVisionParaVer);
					gData.bVisionVerErr = FALSE;
				}
				m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(30000);	// 30초 (Wait for Vision Ready)
				m_tLoadTrayLoop.Takt_End(nTaktZone, 16);
				m_tLoadTrayLoop.Takt_Start(nTaktZone, 17);
			} 
			else
			{
				m_nLoadTrayCase = 20;
			}
		}
		break;
	case 18:	// Check Lot Ready
		if (g_objInspector.Check_LotReady()) 
		{
			m_nLoadTrayCase = 20; m_tLoadTrayLoop.Set_LoopTime(5000);
			m_tLoadTrayLoop.Takt_End(nTaktZone, 17);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 18);
		}
		break;

	case 20:	// Tray X Unload Position, Tray Pick Up Waiting
		m_tLoadTrayLoop.Set_LoopTime(5000);
		return TRUE;

	// return
	case 21:	// Tray Z Move to Ready Down Position
		if (g_objCommon.Get_LoadTrayMasterSlaveOut()) 
		{	// 작업이 끝나면 Out 상태이다.
			if (!g_objCommon.Check_Position(AX_LOAD_STAGE_X, 2)) break;	// 인터락
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 9);
			g_objCommon.Move_Position(AX_LOAD_STAGE_Z, 0);	// Ready Down
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(10000);
			m_tLoadTrayLoop.Takt_End(nTaktZone, 18);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 21);
		}
		break;
	case 22:	// Port Check
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 0)) 
		{			
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(20000);
			m_tLoadTrayLoop.Takt_End(nTaktZone, 21);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 22);
		}
		break;
	case 23:	// Tray X Move to Load Position
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 0)) 
		{			
			if (nLtWorkPort == 1) g_objCommon.Move_Position(AX_LOAD_STAGE_X, 0);	// Port1
			if (nLtWorkPort == 2) g_objCommon.Move_Position(AX_LOAD_STAGE_X, 1);	// Port2
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(10000);
			m_tLoadTrayLoop.Takt_End(nTaktZone, 22);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 23);
		}
		break;
	case 24:	// Check End or Go
		if ((nLtWorkPort == 1 && g_objCommon.Check_Position(AX_LOAD_STAGE_X, 0)) || 
			(nLtWorkPort == 2 && g_objCommon.Check_Position(AX_LOAD_STAGE_X, 1))) 
		{
			if (gData.nLoadTrayCount[nLtWorkPort-1] >= gData.nTrayUseCount[nLtWorkPort-1]) {
				m_bLotLoadEnable[nLtWorkPort-1] = TRUE;	// Port에서 트레이 로드 완료

				if (nLtWorkPort == 1) { 
					m_pDY00->oLoadPort1SlideLock = FALSE; m_pDY00->oLoadPort1SlideUnlock = TRUE; 
					nLtWorkPort = 2;
				} else if (nLtWorkPort == 2) {
					m_pDY00->oLoadPort2SlideLock = FALSE; m_pDY00->oLoadPort2SlideUnlock = TRUE; 
					nLtWorkPort = 1;
				}
				g_objAJinAXL.Write_Output(0);
				g_objAJinAXL.Write_Output(14);
				gData.nLoadTrayCount[nLtWorkPort-1] = 0;
				m_nLoadTrayCase = 30; m_tLoadTrayLoop.Set_LoopTime(10000);
			} else {
				m_nLoadTrayCase = 2; m_tLoadTrayLoop.Set_LoopTime(10000);
			}		
			m_strLog.Format("LoadTray, %d", GetTickCount() - m_dwLoadTray);
			g_objLogFile.Save_TestLog(m_strLog);

			m_tLoadTrayLoop.Takt_End(nTaktZone, 23);
			m_tLoadTrayLoop.Takt_Start(nTaktZone, 24); 
			m_tLoadTrayLoop.Takt_End(nTaktZone, 24, TRUE);
		}
		break;

	case 30:	// Wait, Port Select
		if (!Check_LoadTrayLoading(nLtWorkPort)) return TRUE;	// 같은 Port에서 연속으로 2Lot은 검사할 수 없다.
		// 원래 순서대로 트레이가 적재 되어있을때
		// 2번 차례인데 1번에만 트레이가 있을때는 다시 1번에서 작업하도록 한다.
		if ((nLtWorkPort == 1 && m_pDX00->iLoadPort1Bottom && m_bLotLoadEnable[0] == FALSE) ||
			(nLtWorkPort == 2 && m_pDX00->iLoadPort2Bottom && m_bLotLoadEnable[1] == FALSE)) 
		{
			g_dlgWork.Get_LotInfo(nLtWorkPort);
			m_nLoadTrayCase = 2; m_tLoadTrayLoop.Set_LoopTime(10000);

			m_strLog.Format("LoadTray case 30,No,%d,PNo,%d,LoadEnable1,%d,LoadEnable2,%d,1Bottom,%d,2Bottom,%d", 0, nLtWorkPort, m_bLotLoadEnable[0], m_bLotLoadEnable[1], m_pDX00->iLoadPort1Bottom, m_pDX00->iLoadPort2Bottom);
			g_objLogFile.Save_HandlerLog(m_strLog);

		} 
		else if (nLtWorkPort == 1 && !m_pDX00->iLoadPort1Bottom && m_pDX00->iLoadPort2Bottom && m_bLotLoadEnable[1] == FALSE)
		{
			g_dlgWork.Get_LotInfo(2);
			if (gData.sLotID[1].GetLength() > 0 && gData.nCmUseCount[1] > 0) 
			{
				nLtWorkPort = 2; g_objCommon.Move_Position(AX_LOAD_STAGE_X, 1);	// Port2
				m_nLoadTrayCase = 2; m_tLoadTrayLoop.Set_LoopTime(10000);

				m_strLog.Format("LoadTray case 30,No,%d,PNo,%d,LoadEnable1,%d,LoadEnable2,%d,1Bottom,%d,2Bottom,%d", 1, nLtWorkPort, m_bLotLoadEnable[0], m_bLotLoadEnable[1], m_pDX00->iLoadPort1Bottom, m_pDX00->iLoadPort2Bottom);
				g_objLogFile.Save_HandlerLog(m_strLog);
			}
		}
		else if (nLtWorkPort == 2 && !m_pDX00->iLoadPort2Bottom && m_pDX00->iLoadPort1Bottom && m_bLotLoadEnable[0] == FALSE) 
		{
			g_dlgWork.Get_LotInfo(1);
			if (gData.sLotID[0].GetLength() > 0 && gData.nCmUseCount[0] > 0)
			{
				nLtWorkPort = 1; g_objCommon.Move_Position(AX_LOAD_STAGE_X, 0);	// Port1
				m_nLoadTrayCase = 2; m_tLoadTrayLoop.Set_LoopTime(10000);

				m_strLog.Format("LoadTray case 30,No,%d,PNo,%d,LoadEnable1,%d,LoadEnable2,%d,1Bottom,%d,2Bottom,%d", 2, nLtWorkPort, m_bLotLoadEnable[0], m_bLotLoadEnable[1], m_pDX00->iLoadPort1Bottom, m_pDX00->iLoadPort2Bottom);
				g_objLogFile.Save_HandlerLog(m_strLog);
			}
		} else
		{
			if (m_bUnloadLotEnd) 
			{
				m_nLoadTrayCase = 0; m_tLoadTrayLoop.Set_LoopTime(10000);	// 작업 종료
			}
		}		
		return TRUE;
	}

	// 1. (Error : 3100)
	if (m_tLoadTrayLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(3100 + m_nLoadTrayCase);
		return FALSE;
 	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 2. (Error : 3200)
BOOL CSequenceMain::LoadPicker_Run()
{
	int nTaktZone = 2;		// Takt_Start, Takt_End

	switch (m_nLoadPickCase) 
	{
	case 0:		// Wait for Tray Ready
		if (m_nLoadTrayCase == 20) 
		{
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(5000);
		}
		return TRUE;

	case 1:		// Picker Z Cylinder Down
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0)) 
		{
			if (!m_pDX01->iLoadPickerExist)
			{
				if (m_nLoadTrayCase != 20) return TRUE;		// Wait for Tray Loading
				m_dwTrayPick = GetTickCount();
				
				g_objCommon.Set_LoadPickerDown();
				m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
				m_tLoadPickLoop.Takt_Start(nTaktZone,1,TRUE);
			}
		}
		break;
	case 2:		// Load Tray Slave Out
		if (g_objCommon.Get_LoadPickerDown()) 
		{
			m_pDY01->oLoadStageSlaveIn = FALSE;
			g_objAJinAXL.Write_Output(1);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			m_tLoadPickLoop.Takt_End(nTaktZone,1);
			m_tLoadPickLoop.Takt_Start(nTaktZone,2);
			
		}
		break;
	case 3:		// Load Tray Master Out
		if (!m_pDX01->iLoadStageSlaveIn && m_pDX01->iLoadStageSlaveOut) 
		{
			m_pDY01->oLoadStageMasterIn = FALSE;
			g_objAJinAXL.Write_Output(1);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			m_tLoadPickLoop.Takt_End(nTaktZone,2);
			m_tLoadPickLoop.Takt_Start(nTaktZone,3);
		}
		break;
	case 4:		// Picker Master In
		if (g_objCommon.Get_LoadTrayMasterSlaveOut())
		{			
			m_pDY01->oLoadPickerMasterIn = TRUE; m_pDY01->oLoadPickerMasterOut = FALSE;
			g_objAJinAXL.Write_Output(1);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			m_tLoadPickLoop.Takt_End(nTaktZone,3);
			m_tLoadPickLoop.Takt_Start(nTaktZone,4);
		}
		break;
	case 5:		// Picker Slave In
		if (m_pDX01->iLoadPickerMasterIn && !m_pDX01->iLoadPickerMasterOut)
		{
			m_pDY01->oLoadPickerSlaveIn = TRUE; m_pDY01->oLoadPickerSlaveOut = FALSE;
			g_objAJinAXL.Write_Output(1);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			m_tLoadPickLoop.Takt_End(nTaktZone,4);
			m_tLoadPickLoop.Takt_Start(nTaktZone,5);
		}
		break;
	case 6:		// 정보전달. // Picker Z Cylinder Up
		if (g_objCommon.Get_LoadPickerMasterSlaveIn()) 
		{			
			gData.nPNoTrayPick = gData.nLPNo;
			gData.nTNoTrayPick = gData.nLoadTrayCount[gData.nLPNo-1];
			g_objCommon.Set_LoadPickerUp();
			//MCC
			m_strLog.Format("Tray Pick, pNo : %d, TNo : %d", gData.nPNoTrayPick, gData.nTNoTrayPick);
			g_objLogFile.Save_MCCLog(m_strLog);


			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			m_tLoadPickLoop.Takt_End(nTaktZone,5);
			m_tLoadPickLoop.Takt_Start(nTaktZone,6);
		}
		break;
	case 7:		// Tray Check
		if (g_objCommon.Get_LoadPickerUp())
		{
			if (m_pDX01->iLoadPickerExist) 
			{				
				if (m_nLoadTrayCase == 20) m_nLoadTrayCase = 21;
				m_nLoadPickCase = 10; m_tLoadPickLoop.Set_LoopTime(10000);
				m_tLoadPickLoop.Takt_End(nTaktZone,6);
				m_tLoadPickLoop.Takt_Start(nTaktZone,7);
				m_tLoadPickLoop.Takt_End(nTaktZone,7);
			}
		}
		break;

	case 10:	// Wait for Align Port Tray Empty
		// Port에 Tray가 있는데 검사 완료가 안되었다면 대기.	// Tray 검사 끝나고 다음 Tray 미리 올려놓을때 필요. 
		if (m_nVisAngleCase == 0 && !m_pDX02->iAnglePortExist && !m_bAngleInspComplete) 
		{	// Angle Port가 작업안하고 트레이가 없다.
			if (g_objCommon.Check_Position(AX_ANGLE_UNIT_X, 0) && g_objCommon.Check_Position(AX_ANGLE_UNIT_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_UNIT_R, 0)) {	// Inter lock 간섭 있다.
				m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
				
				m_tLoadPickLoop.Takt_Start(nTaktZone,10);
				m_tLoadPickLoop.Takt_End(nTaktZone,10);

			}
		}
		return TRUE;

	case 11:	// Picker Y Move to Angle Port Unload Position
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0)) 
		{			
			g_objCommon.Move_Position(AX_LOAD_PICKER_Y, 1);	// Angle Position
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			m_tLoadPickLoop.Takt_Start(nTaktZone,11);
			
		}
		break;
	case 12:	// Angle Port 안전확인, Picker Down
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 1)) 
		{
			if (g_objCommon.Get_AnglePortSupportIn() && g_objCommon.Get_AnglePortMasterSlaveOut()) {	// Port 확인
				
				g_objCommon.Set_LoadPickerDown();
				m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
				m_tLoadPickLoop.Takt_End(nTaktZone,11);
				m_tLoadPickLoop.Takt_Start(nTaktZone,12);
			}			
		}
		break;
	case 13:	// Picker Slave Out
		if (g_objCommon.Get_LoadPickerDown()) 
		{			
			m_pDY01->oLoadPickerSlaveIn = FALSE; m_pDY01->oLoadPickerSlaveOut = TRUE;
			m_pDY01->oLoadPickerMasterIn = FALSE; m_pDY01->oLoadPickerMasterOut = TRUE;
			g_objAJinAXL.Write_Output(1);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			m_tLoadPickLoop.Takt_End(nTaktZone,12);
			m_tLoadPickLoop.Takt_Start(nTaktZone,13);
		}
		break;
	case 14:	// Picker Master Out
		if (g_objCommon.Get_LoadPickerMasterSlaveOut()) 
		{			
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			m_tLoadPickLoop.Takt_End(nTaktZone,13);
			m_tLoadPickLoop.Takt_Start(nTaktZone,14);
		}
		break;
	case 15:	// 정보전달, Picker Up
		if (g_objCommon.Get_LoadPickerMasterSlaveOut())
		{			
			gData.nPNoAnglePort[m_nAnglePortTrayCnt] = gData.nPNoTrayPick; gData.nPNoTrayPick = 0;
			gData.nTNoAnglePort[m_nAnglePortTrayCnt] = gData.nTNoTrayPick;
			m_nAnglePortTrayCnt++;
			g_objCommon.Set_LoadPickerUp();

			//MCC
			m_strLog.Format("Tray Put in angle port, pNo : %d, TNo : %d", gData.nPNoAnglePort[m_nAnglePortTrayCnt], gData.nTNoAnglePort[m_nAnglePortTrayCnt]);
			g_objLogFile.Save_MCCLog(m_strLog);

			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			m_tLoadPickLoop.Takt_End(nTaktZone,14);
			m_tLoadPickLoop.Takt_Start(nTaktZone,15);
		}
		break;
	case 16:	// Y Axis Move to Load Tray Position
		if (g_objCommon.Get_LoadPickerUp()) 
		{			
			g_objCommon.Move_Position(AX_LOAD_PICKER_Y, 0);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			m_tLoadPickLoop.Takt_End(nTaktZone,15);
			m_tLoadPickLoop.Takt_Start(nTaktZone,16);
		}
		break;
	case 17:	// Position Check
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0)) 
		{			
			m_strLog.Format("LoadTrayPick, %d", GetTickCount() - m_dwTrayPick);
			g_objLogFile.Save_TestLog(m_strLog);
			m_nLoadPickCase = 0; m_tLoadPickLoop.Set_LoopTime(10000);
			m_tLoadPickLoop.Takt_End(nTaktZone,16);
			m_tLoadPickLoop.Takt_Start(nTaktZone,17);
			m_tLoadPickLoop.Takt_End(nTaktZone,17);
		}
		break;
	}

	// 2. (Error : 3200)
	if (m_tLoadPickLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(3200 + m_nLoadPickCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 3. (Error : 3300)
BOOL CSequenceMain::VisionAngle_Run()
{
	int nTaktZone = 3;		// Takt_Start, Takt_End
	static int		nAngleTrayXPos, nAngleTrayYPos = 0;
	static double	dAngleUnitX, dAngleUnitY, dAngleUnitR = 0.0;
	static BOOL		bAlignRetry = FALSE;
 
	switch (m_nVisAngleCase)
	{
	case 0:		// Tray 수량, 센서 감지 및 검사 유무 확인 후 진행
		if (m_nAnglePortTrayCnt == 1 && !m_bAngleInspComplete)
		{
			// Inter lock 간섭이 있다. 위의 조건이 충족했는데 Port에 Tray가 감지가 안되면 알람.
			if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0) && m_pDX02->iAnglePortExist) 
			{
				if (!m_pEquipData->bUseVisionAlign && !m_pEquipData->bUseInspectAngle)
				{	// Angle Skip
					Init_AnglePortTray(gData.nPNoAnglePort[0]);
					for (int i = 0; i < 5; i++) {
						for (int j = 0; j < 8; j++) {
							if (gData.InfoAnglePortTray[i][j] == 9) gData.InfoAnglePortTray[i][j] = 1;	//Scan Done
						}
					}
					m_nVisAngleCase = 21; m_tVisAngleLoop.Set_LoopTime(5000);
				} 
				else
				{
					m_tVisAngleLoop.Takt_Start(nTaktZone, 0, TRUE);
					m_tVisAngleLoop.Takt_End(nTaktZone, 0);
					m_nVisAngleCase = 1; m_tVisAngleLoop.Set_LoopTime(5000);
				}
			}
			break;
		}
		m_tVisAngleLoop.Set_LoopTime(5000);
		return TRUE;

	case 1:		// Master In
		
		m_dwVisAngle = GetTickCount();
		m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
		m_tVisAngleLoop.Takt_Start(nTaktZone, 1);
		
		break;
	case 2:		// Slave In & Align Z Move to Down Position 
		if (!m_pDX02->iAnglePortMasterIn && m_pDX02->iAnglePortMasterOut) 
		{
			g_objCommon.Move_Position(AX_VISION_ALIGN_Z, 1);	// Align Grab Pos
			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
			m_tVisAngleLoop.Takt_End(nTaktZone, 1);
			m_tVisAngleLoop.Takt_Start(nTaktZone, 2);
		}
		break;
	case 3:		// Master/Slave Check 
		if (g_objCommon.Get_AnglePortMasterSlaveOut()) 
		{			
			Init_AnglePortTray(gData.nPNoAnglePort[0]);
			gData.bAlignReply = FALSE;
			bAlignRetry = FALSE;

			if (m_pEquipData->bUseVisionAlign)	m_nVisAngleCase++;
			else								m_nVisAngleCase = 8;
			m_tVisAngleLoop.Set_LoopTime(5000);
			m_tVisAngleLoop.Takt_End(nTaktZone, 2);
			m_tVisAngleLoop.Takt_Start(nTaktZone, 3);
		}
		break;
	case 4:		// Align Load Complete
		if (g_objCommon.Check_Position(AX_VISION_ALIGN_Z, 1)) 
		{
			g_objInspector.Set_AlignRequest(INSPECTOR_PC2, gLot.sLotID[gData.nPNoAnglePort[0]-1], gData.nPNoAnglePort[0], gData.nTNoAnglePort[0], 1);	//gData.nLoadTrayCount[gData.nPNoAnglePort[0]-1], 1);
			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(30000);
		}
		break;

	case 5:		// Wait for Vision Align Complete (30sec)
		if (!m_pEquipData->bUseVisionAlign)
		{
			m_nVisAngleCase = 8; m_tVisAngleLoop.Set_LoopTime(5000);
		}
		break;

	case 8:		// Align Z Move to Ready Up Position
		if (g_objAJinAXL.Is_Done(AX_VISION_ALIGN_Z)) 
		{
			if (!m_pEquipData->bUseVisionAlign) { if (!m_tVisAngleLoop.Waiting_Time(80)) break; }	// Dealy Time
			g_objCommon.Move_Position(AX_VISION_ALIGN_Z, 0);	// ready up
			m_nVisAngleCase = 10; m_tVisAngleLoop.Set_LoopTime(30000);
			m_tVisAngleLoop.Takt_End(nTaktZone, 3);
			m_tVisAngleLoop.Takt_Start(nTaktZone, 8);
		}
		break;

	case 10:	//Angle Z move to ready up 
		if (g_objCommon.Check_Position(AX_VISION_ALIGN_Z, 0))
		{
			if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0)) 
			{	// Inter lock
				
				g_objCommon.Move_Position(AX_VISION_ANGLE_Z, 0);
				m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
				m_tVisAngleLoop.Takt_End(nTaktZone, 8);
				m_tVisAngleLoop.Takt_Start(nTaktZone, 10);
			}
		}
		break;
	case 11:	// Angle Z position check and focus z data save 
		if (g_objCommon.Check_Position(AX_VISION_ANGLE_Z, 0))
		{			
			CString strLog;
			double dPos = m_pMoveData->dVisionAngleZ[0];
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_ANGLE_Z);
			strLog.Format("%0.3lf,%0.3lf", dPos, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_ANGLE_Z, strLog);
			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
			m_tVisAngleLoop.Takt_End(nTaktZone, 10);
			m_tVisAngleLoop.Takt_Start(nTaktZone, 11);
		}
		break;
	case 12:	// Move to Module Inspection Position
		if (Select_AngleScanPos(nAngleTrayXPos, nAngleTrayYPos)) 
		{		
			m_strLog.Format("MCC,(03) VisionAngle, Angle Scan Complete or AnglePosCalculate");
			g_objLogFile.Save_MCCLog(m_strLog);

			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, NULL);

			int nIndex =  (nAngleTrayYPos - 1) * gData.nTrayX + nAngleTrayXPos;
			dAngleUnitY = m_pMoveData->dAngleUnitY[1] + (m_pEquipData->dTrayPitchY * (nAngleTrayYPos-1) );// + g_objInspector.Get_AlignY(nIndex);
			dAngleUnitX = m_pMoveData->dAngleUnitX[1] - (m_pEquipData->dTrayPitchX * (nAngleTrayXPos-1) );// + g_objInspector.Get_AlignX(nIndex);
			dAngleUnitR = m_pMoveData->dAngleUnitR[1];// + g_objInspector.Get_AlignQ(nIndex);

			g_objAJinAXL.Move_Absolute(AX_ANGLE_UNIT_Y, dAngleUnitY);
			g_objAJinAXL.Move_Absolute(AX_ANGLE_UNIT_X, dAngleUnitX);
			g_objAJinAXL.Move_Absolute(AX_ANGLE_UNIT_R, dAngleUnitR);

		
			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(10000);
			m_tVisAngleLoop.Takt_End(nTaktZone, 11);
			m_tVisAngleLoop.Takt_Start(nTaktZone, 12);
		}
		else
		{	// Scan End
			m_strLog.Format("MCC,(03) VisionAngle, Angle Scan Complete or AnglePosCalculate");
			g_objLogFile.Save_MCCLog(m_strLog);
			m_nVisAngleCase = 20; m_tVisAngleLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Set Load Complete
		if (g_objAJinAXL.Is_MoveDone(AX_ANGLE_UNIT_Y, dAngleUnitY) &&
			g_objAJinAXL.Is_MoveDone(AX_ANGLE_UNIT_X, dAngleUnitX) &&
			g_objAJinAXL.Is_MoveDone(AX_ANGLE_UNIT_R, dAngleUnitR))
		{
		
			if (!m_pEquipData->bUseInspectAngle)
			{
				if (gData.InfoAnglePortTray[nAngleTrayYPos-1][nAngleTrayXPos-1] == 9) gData.InfoAnglePortTray[nAngleTrayYPos-1][nAngleTrayXPos-1] = 1;	//Scan Done
				m_nVisAngleCase = 19; m_tVisAngleLoop.Set_LoopTime(30000);
			} 
			else 
			{
				int nCmNo = (gData.nTrayY - nAngleTrayYPos) * gData.nTrayX + nAngleTrayXPos;	// Tray 하단부터 모듈 적재한다.
				g_objInspector.Set_LoadComplete(INSPECTOR_PC2, "AG", gLot.sLotID[gData.nPNoAnglePort[0]-1], gData.nPNoAnglePort[0], gData.nTNoAnglePort[0],0, 0, 0, nCmNo, 0, 0, 0);
				m_nVisAngleCase = 15; m_tVisAngleLoop.Set_LoopTime(30000);
				m_tVisAngleLoop.Takt_End(nTaktZone, 12 );
				m_tVisAngleLoop.Takt_Start(nTaktZone, 13);
				m_tVisAngleLoop.Takt_End(nTaktZone, 13 );
			}
		}
		break;

	case 15:	// Wait for Angle inspection(30sec)
		if (!m_pEquipData->bUseInspectAngle) 
		{
			m_nVisAngleCase = 19; m_tVisAngleLoop.Set_LoopTime(5000);
		}
		break;
	case 16:
		if (g_objAJinAXL.Is_Done(AX_VISION_ANGLE_Z))
		{			
			g_objAJinAXL.Move_Absolute(AX_VISION_ANGLE_Z, m_dAngleZ);
			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
			m_tVisAngleLoop.Takt_Start(nTaktZone, 16);
			
		}
		break;
	case 17:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_VISION_ANGLE_Z))
		{			
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_ANGLE_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dAngleZ, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_ANGLE_Z, strLog);
			g_objInspector.Set_MoveComplete(INSPECTOR_PC2, "AG");
			m_nVisAngleCase = 15; m_tVisAngleLoop.Set_LoopTime(30000);	// 30초
			m_tVisAngleLoop.Takt_End(nTaktZone, 16);
			m_tVisAngleLoop.Takt_Start(nTaktZone, 17);
			m_tVisAngleLoop.Takt_End(nTaktZone, 17);
		}
		break;

	case 19:	
		
		m_nVisAngleCase = 12; m_tVisAngleLoop.Set_LoopTime(30000);
		break;

	case 20:	// Angle Unit Move to Ready Position
		if (g_objAJinAXL.Is_Done(AX_ANGLE_UNIT_Y) && g_objAJinAXL.Is_Done(AX_ANGLE_UNIT_X) && g_objAJinAXL.Is_Done(AX_ANGLE_UNIT_R)) 
		{
			m_tVisAngleLoop.Takt_Start(nTaktZone, 20);
			
			g_objCommon.Move_Position(AX_ANGLE_UNIT_X, 0);
			g_objCommon.Move_Position(AX_ANGLE_UNIT_Y, 0);
			g_objCommon.Move_Position(AX_ANGLE_UNIT_R, 0);
			g_objCommon.Move_Position(AX_VISION_ANGLE_Z, 0);
			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
		}
		break;

	case 21:	//Angle port Slave Out
		m_pDY02->oAnglePortSlaveIn = FALSE; m_pDY02->oAnglePortSlaveOut = TRUE; 
		g_objAJinAXL.Write_Output(2);
		m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
		m_tVisAngleLoop.Takt_End(nTaktZone, 20);
		m_tVisAngleLoop.Takt_Start(nTaktZone, 21);
		break;
	case 22:	//Angle port Master Out
		if (!m_pDX02->iAnglePortSlaveIn && m_pDX02->iAnglePortSlaveOut)
		{
			m_pDY02->oAnglePortMasterIn = FALSE; m_pDY02->oAnglePortMasterOut = TRUE; 
			g_objAJinAXL.Write_Output(2);
			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
			m_tVisAngleLoop.Takt_End(nTaktZone, 21);
			m_tVisAngleLoop.Takt_Start(nTaktZone, 22);
		}
		break;
	case 23:	// Master/Slave Check 
		if (g_objCommon.Get_AnglePortMasterSlaveOut())
		{			
			m_bAngleInspComplete = TRUE;	// 검사 완료.
			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
			m_tVisAngleLoop.Takt_End(nTaktZone, 22);
			m_tVisAngleLoop.Takt_Start(nTaktZone, 23);
		}
		break;
	case 24:	// Angle Unit Position Check
		if (g_objCommon.Check_Position(AX_ANGLE_UNIT_X, 0) && g_objCommon.Check_Position(AX_ANGLE_UNIT_Y, 0) &&
			g_objCommon.Check_Position(AX_ANGLE_UNIT_R, 0) && g_objCommon.Check_Position(AX_VISION_ANGLE_Z, 0)) 
		{
			
			m_strLog.Format("VisionAngle, %d", GetTickCount() - m_dwVisAngle);
			g_objLogFile.Save_TestLog(m_strLog);
			m_nVisAngleCase = 0; m_tVisAngleLoop.Set_LoopTime(5000);
			m_tVisAngleLoop.Takt_End(nTaktZone, 23);
			m_tVisAngleLoop.Takt_Start(nTaktZone, 24);
		}
		break;
	}

	// 3. (Error : 3300)
	if (m_tVisAngleLoop.Over_LoopTime()) 
	{
		if (m_nVisAngleCase == 5)
		{
			if (!gData.bAlignReply && !bAlignRetry) { bAlignRetry = TRUE; m_nVisAngleCase = 4; m_tVisAngleLoop.Set_LoopTime(5000); return TRUE;}
		}
		if (m_nVisAngleCase == 15)
		{
			int nCmNo = (nAngleTrayYPos-1) * gData.nTrayX +  nAngleTrayXPos;
			Set_ScanError(3315, gData.nTNoAnglePort[0], 0, 0, 0, nCmNo, 0, 0, 0);
		}
		g_objCommon.Show_Error(3300 + m_nVisAngleCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 4. (Error : 3400)
BOOL CSequenceMain::AngleTray1_Run()
{
	int nTaktZone = 4;		// Takt_Start, Takt_End
	static double	dStage1Z = 0.0;

	switch (m_nAngleTray1Case)
	{
	case 0:		// Wait for Angle Vision Done
		if (m_bUnloadLotEnd) return TRUE;

		// Load Position Check
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 0) && !m_pDX02->iAngleStage1Exist && !m_pDX02->iAngleStage1AlignIn && m_pDX02->iAngleStage1AlignOut) 
		{
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 3);	// Support Ready Position
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
			
		}
		break;

	case 1:		// 안전 확인. 검사 중이면 Z Axis Move to Support Wait Position
		if (m_nVisAngleCase > 8 && m_pDX02->iAnglePortExist) 
		{	// 검사 진행 중이면
			if (m_pDX02->iAngleStage1Normal && !m_pDX02->iAngleStage1Rotate &&
				g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 3))
			{
				g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 5);	// Wait Position
				m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
				m_tAngleTray1Loop.Takt_Start(nTaktZone, 1, TRUE);
				m_tAngleTray1Loop.Takt_End(nTaktZone, 1);
			}
			break;
		} 
		else if ((m_nVisAngleCase == 0 || m_nVisAngleCase == 24) && m_pDX02->iAnglePortExist && m_bAngleInspComplete) 
		{ // 작업 끝났고 검사 완료
			if (m_pDX02->iAngleStage1Normal && !m_pDX02->iAngleStage1Rotate && g_objCommon.Get_AnglePortMasterSlaveOut() &&
				g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 3)) 
			{
				m_dwAngleTray1 = GetTickCount();
				g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 1);
				m_nAngleTray1Case = 3; m_tAngleTray1Loop.Set_LoopTime(5000);
				m_tAngleTray1Loop.Takt_Start(nTaktZone, 1, TRUE);
				m_tAngleTray1Loop.Takt_End(nTaktZone, 1);
			}
			break;
		}
		if (m_bUnloadLotEnd) 
		{
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 0);	// Ready Down Position
			m_nAngleTray1Case = 0; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 2:		// 안전 확인. Z Axis Move to Support Up Position
		if ((m_nVisAngleCase == 0 || m_nVisAngleCase == 24) && m_pDX02->iAnglePortExist && m_bAngleInspComplete) 
		{	// 작업 끝났고 검사 완료했으면 진행.
			if (m_pDX02->iAngleStage1Normal && !m_pDX02->iAngleStage1Rotate && g_objCommon.Get_AnglePortMasterSlaveOut() &&
				g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 5))
			{			
				m_dwAngleTray1 = GetTickCount();
				g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 1);
				m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
				m_tAngleTray1Loop.Takt_Start(nTaktZone, 2);
				m_tAngleTray1Loop.Takt_End(nTaktZone, 2);
			}
			break;
		}
		if (m_bUnloadLotEnd) 
		{
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 0);	// Ready Down Position
			m_nAngleTray1Case = 0; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 3:		// Angle Port Support Out, Stage Align In
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 1))
		{			
			g_objCommon.Set_AnglePortSupportOut();
			m_pDY02->oAngleStage1AlignIn = TRUE;
			g_objAJinAXL.Write_Output(2);
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 3);
			
		}
		break;
	case 4:		// 정보전달, Z Axis Move to Ready Down Position
		if (g_objCommon.Get_AnglePortSupportOut() && m_pDX02->iAngleStage1Exist 
			&&m_pDX02->iAngleStage1AlignIn && !m_pDX02->iAngleStage1AlignOut) 
		{

			m_tAngleTray1Loop.Takt_End(nTaktZone, 3);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 4);
			
			memmove(gData.InfoAngleTray[0], gData.InfoAnglePortTray, sizeof(int) * 8 * 5);
			memset(gData.InfoAnglePortTray, 0x00, sizeof(int) * 8 * 5);
			gData.nPNoAngleTray[0] = gData.nPNoAnglePort[0]; gData.nPNoAnglePort[0] = 0;
			gData.nTNoAngleTray[0] = gData.nTNoAnglePort[0]; gData.nTNoAnglePort[0] = 0;
			m_nAnglePortTrayCnt--;
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, 0);

			m_bAngleInspComplete = FALSE;
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 3);	// Support Ready Position

			//MCC
			m_strLog.Format("Angle Stage 1 Tray Get, pNo:%d, TNo:%d",gData.nPNoAngleTray[0], gData.nTNoAngleTray[0]);
			g_objLogFile.Save_MCCLog(m_strLog);

			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Angle Port Support In
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 3)) 
		{
			g_objCommon.Set_AnglePortSupportIn();
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 4);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 5);
		}
		break;
	case 6:		// Support Check
		if (g_objCommon.Get_AnglePortSupportIn())
		{			
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 5);
		}
	case 7:		// 안전 확인.
		if (m_nAngleTray2Case >= 20) 
		{	// Btm1 Picker CM Loading
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 7);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 7);
		}
		return TRUE;
	case 8:		// Y Axis Move to Ready Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 3))
		{			
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, 1); // Ready Position
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 8);
		}
		break;
	case 9:		// Z Axis Move to Pick Up Position
		if (m_pDX02->iAngleStage1Normal && !m_pDX02->iAngleStage1Rotate && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 1))
		{		
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 2);
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 8);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 9);
			
		}
		break;
	case 10:
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 2)) 
		{			
			m_nAngleTray1Case = 15; m_tAngleTray1Loop.Set_LoopTime(20000);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 9);
		}
		break;
	case 15:	// Wait for Stage2
		if (m_nAngleTray2Case > 23)	
		{	// Btm1 Picker CM Loading
			if(m_bBtm1ModulePick) g_objCommon.Set_Btm1Picker2RowUp(); // Btm1 Picker Line 1이 마지막 열을 Pick 할때 뒤에 트레이가 미리 올때 Btm1 실린더가 내려와 있으면 부딪힌다. 
			m_nAngleTray1Case++;m_tAngleTray1Loop.Set_LoopTime(20000);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 15);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 15);
			
		}
		m_tAngleTray1Loop.Set_LoopTime(5000);
		return TRUE;

	case 16:	// Y Axis Move to Btm1 Picker L1 Position
		if(!g_objCommon.Get_Btm1Picker2RowUp() && m_bBtm1ModulePick){ g_objCommon.Set_Btm1Picker2RowUp(); break;}
		
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 2) ) 
		{
			if (!m_tAngleTray1Loop.Waiting_Time(100)) break;
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 16);
			if (m_bBtm1ModulePick) 
			{
				g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, 3); // Btm1 Picker L2 Position
			} 
			else 
			{
				g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, 2); // Btm1 Picker L1 Position
			}
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 17:	// Position Check
		if ((m_bBtm1ModulePick && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 3)) || g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 2)) 
		{
			if(m_nAngleTray2Case == 20) return TRUE;
			
			Init_AngleTray(0);
			
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, 0);
			m_nAngleTray1Case = 20; m_tAngleTray1Loop.Set_LoopTime(5000);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 16);
		}
		break;

	case 20:	// Module Load Working
		return TRUE;

	case 21:	// 스테이지 20mm 다운 후 이동
		if (m_nAngleTray2Case <= 20) 
		{
			dStage1Z = m_pMoveData->dAngleStage1Z[2] - 20.0;	// CM Pick Up Position - 20mm
			g_objAJinAXL.Move_Absolute(AX_ANGLE_STAGE1_Z, dStage1Z);
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 20);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 20);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 21);

		}
		return TRUE;
	case 22:	// Down Check
		if (g_objAJinAXL.Is_MoveDone(AX_ANGLE_STAGE1_Z, dStage1Z)) 
		{
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 21);
		}
		break;

	case 23:	// Y Axis Empty Tray Unload Position
		if (m_nAngleTray2Case == 30) return TRUE;	// Empty Tray Unloading
		if (m_nAngleTray2Case > 52 || m_nAngleTray2Case < 23) 
		{	// Inter lock
			
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, 4); // Unload Position
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 22);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 22);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 23);
		}
		return TRUE;
	case 24:	// Position Check & Z Axis Move To Tray Pick Up Position
		if (m_pDX02->iAngleStage1Normal && !m_pDX02->iAngleStage1Rotate && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 4)) 
		{			
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 4); // Tray Pick Up Position
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 23);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 24);
		}
		break;
	case 25:	// Position Check
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 4)) 
		{
			m_bBtm1ModulePick = FALSE;
			m_nAngleTray1Case = 30; m_tAngleTray1Loop.Set_LoopTime(5000);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 24);
		}
		break;
	case 30:	// Empty Tray Unload Working
		return TRUE;

	case 31:	// Z Axis Move to Ready Down Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 4) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 4) && !m_pDX02->iAngleStage1Exist) 
		{
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 2); // Picker Up Position 안전한 위치.
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(30000);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 30);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 30);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 31);
		}
		break;
	case 32:	//
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 4) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 2)) 
		{			
			m_strLog.Format("AngleStage1, %d", GetTickCount() - m_dwAngleTray1);
			g_objLogFile.Save_TestLog(m_strLog);
			m_nAngleTray1Case = 50; m_tAngleTray1Loop.Set_LoopTime(30000);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 31);
		}
		break;

	case 50:	// Wait for Loading
		if (m_nAngleTray2Case > 9 && m_nAngleTray2Case < 50) m_nAngleTray1Case++; 
		return TRUE;

	case 51:	// Z Axis Move to Ready Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 4) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 2)) 
		{			
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 0);
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(30000);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 50);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 50);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 51);
		}
		break;
	case 52:	// Check Down Position, Y Move to Load pos
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 2) &&
			m_pDX02->iAngleStage1Normal && !m_pDX02->iAngleStage1Rotate) 
		{			
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, 0);
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(30000);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 51);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 52);
		}
		break;
	case 53:	// Check Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 0)) 
		{			
			m_pDY02->oAngleStage1AlignIn = FALSE;	// Tray Align Out
			g_objAJinAXL.Write_Output(2);
			m_nAngleTray1Case = 0; m_tAngleTray1Loop.Set_LoopTime(5000);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 52);
			m_tAngleTray1Loop.Takt_Start(nTaktZone, 53);
			m_tAngleTray1Loop.Takt_End(nTaktZone, 53);
		}
		break;
	}

	// 4. (Error : 3400)
	if (m_tAngleTray1Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(3400 + m_nAngleTray1Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 5. (Error : 3500)
BOOL CSequenceMain::AngleTray2_Run()
{
	int nTaktZone = 5;		// Takt_Start, Takt_End
	static double	dStage2Z = 0.0;

	switch (m_nAngleTray2Case)
	{
	case 0:		// Wait for Angle Vision Done
		if (m_bUnloadLotEnd) return TRUE;

		// Load Position Check
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 0) && !m_pDX02->iAngleStage2Exist && !m_pDX02->iAngleStage2AlignIn && m_pDX02->iAngleStage2AlignOut)
		{
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 3);	// Support Ready Position
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		break;

	case 1:		// 안전 확인. 검사 중이면 Z Axis Move to Support Wait Position
		if (m_nVisAngleCase > 8 && m_pDX02->iAnglePortExist) {	// 검사 진행 중이면
			if (m_pDX02->iAngleStage2Normal && !m_pDX02->iAngleStage2Rotate &&
				g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 3)) 
			{
				g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 5);	// Wait Position
				m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
				m_tAngleTray2Loop.Takt_Start(nTaktZone, 1, TRUE);
				m_tAngleTray2Loop.Takt_End(nTaktZone, 1);
			}
			break;
		} else if ((m_nVisAngleCase == 0 || m_nVisAngleCase == 24) && m_pDX02->iAnglePortExist && m_bAngleInspComplete) 
		{	// 작업 끝났고 검사 완료했으면 진행.
			if (m_pDX02->iAngleStage2Normal && !m_pDX02->iAngleStage2Rotate && g_objCommon.Get_AnglePortMasterSlaveOut() &&
				g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 3))
			{				
				m_dwAngleTray2 = GetTickCount();
				g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 1);
				m_nAngleTray2Case = 3; m_tAngleTray2Loop.Set_LoopTime(5000);
				m_tAngleTray2Loop.Takt_Start(nTaktZone, 1, TRUE);
				m_tAngleTray2Loop.Takt_End(nTaktZone, 1);
			}
			break;
		}
		if (m_bUnloadLotEnd)
		{
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 0);	// Ready Down Position
			m_nAngleTray2Case = 0; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 2:		// 안전 확인. Z Axis Move to Support Up Position
		if ((m_nVisAngleCase == 0 || m_nVisAngleCase == 24) && m_pDX02->iAnglePortExist && m_bAngleInspComplete)
		{	// 작업 끝났고 검사 완료했으면 진행.
			if (m_pDX02->iAngleStage2Normal && !m_pDX02->iAngleStage2Rotate && g_objCommon.Get_AnglePortMasterSlaveOut() &&
				g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 5)) 
			{
				
				m_dwAngleTray2 = GetTickCount();
				g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 1);
				m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
				m_tAngleTray2Loop.Takt_Start(nTaktZone, 2);
				m_tAngleTray2Loop.Takt_End(nTaktZone, 2);
			}
			break;
		}
		if (m_bUnloadLotEnd) 
		{
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 0);	// Ready Down Position
			m_nAngleTray2Case = 0; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 3:		// Angle Port Support Out, Stage Align In
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 1)) 
		{			
			g_objCommon.Set_AnglePortSupportOut();
			m_pDY02->oAngleStage2AlignIn = TRUE;
			g_objAJinAXL.Write_Output(2);
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 3);
		}
		break;
	case 4:		// 정보전달, Z Axis Move to Ready Down Position
		if (g_objCommon.Get_AnglePortSupportOut() && m_pDX02->iAngleStage2Exist &&
			m_pDX02->iAngleStage2AlignIn && !m_pDX02->iAngleStage2AlignOut) 
		{			
			m_tAngleTray2Loop.Takt_End(nTaktZone, 3);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 4);
			memmove(gData.InfoAngleTray[1], gData.InfoAnglePortTray, sizeof(int) * 8 * 5);
			memset(gData.InfoAnglePortTray, 0x00, sizeof(int) * 8 * 5);
			gData.nPNoAngleTray[1] = gData.nPNoAnglePort[0]; gData.nPNoAnglePort[0] = 0;
			gData.nTNoAngleTray[1] = gData.nTNoAnglePort[0]; gData.nTNoAnglePort[0] = 0;
			m_nAnglePortTrayCnt--;
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, 0);

			m_bAngleInspComplete = FALSE;
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 3);	// Support Ready Position

			//MCC
			m_strLog.Format("Angle Stage 2 Tray Get, pNo:%d, TNo:%d",gData.nPNoAngleTray[1], gData.nTNoAngleTray[1]);
			g_objLogFile.Save_MCCLog(m_strLog);
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Angle Port Support In
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 3)) 
		{			
			g_objCommon.Set_AnglePortSupportIn();
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 4);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 5);
		}
		break;
	case 6:		// Support Check
		if (g_objCommon.Get_AnglePortSupportIn()) 
		{			
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 5);
		}
	case 7:		// 안전 확인.
		if (m_nAngleTray1Case >= 20) 
		{	// Btm1 Picker CM Loading
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 7);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 7);
		}
		return TRUE;
	case 8:		// Y Axis Move to Ready Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 3))
		{			
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, 1); // Ready Position
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 8);
		}
		break;
	case 9:		// Z Axis Move to Pick Up Position
		if (m_pDX02->iAngleStage2Normal && !m_pDX02->iAngleStage2Rotate && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 1)) 
		{			
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 2);
			m_nAngleTray2Case = 10; m_tAngleTray2Loop.Set_LoopTime(5000);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 8 );
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 9);
		}
		break;
	case 10:
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 2)) 
		{			
			m_nAngleTray2Case = 15; m_tAngleTray2Loop.Set_LoopTime(20000);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 9 );
		}
		break;
	case 15:	// Wait for Stage1
		if (m_nAngleTray1Case > 23)	
		{	// Btm1 Picker CM Loading
			if(m_bBtm1ModulePick) g_objCommon.Set_Btm1Picker2RowUp();
			m_nAngleTray2Case++;m_tAngleTray2Loop.Set_LoopTime(20000);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 15);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 15);
		}
		m_tAngleTray2Loop.Set_LoopTime(5000);
		return TRUE;
		

	case 16:	// Y Axis Move to Btm1 Picker L1 Position
		if(!g_objCommon.Get_Btm1Picker2RowUp() && m_bBtm1ModulePick) { g_objCommon.Set_Btm1Picker2RowUp(); break;}
		
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 2)) 
		{ //btm1 실린더 line2 up 상태여야 충돌 안함 
			if (!m_tAngleTray2Loop.Waiting_Time(100) )break;
			if (m_bBtm1ModulePick) 
			{
				g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, 3); // Btm1 Picker L2 Position
			} 
			else
			{
				//if(!g_objCommon.Get_Btm1Picker2RowUp()) break;
				g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, 2); // Btm1 Picker L1 Position
			}
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 16);
		}
		break;
	case 17:	// Position Check
		if ((m_bBtm1ModulePick && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 3)) || g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 2)) 
		{
			if(m_nAngleTray1Case == 20) return TRUE;
			Init_AngleTray(1);
			
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, 1);
			m_nAngleTray2Case = 20; m_tAngleTray2Loop.Set_LoopTime(5000);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 16,gData.nPNoAngleTray[1]);
		}
		break;

	case 20:	// Module Load Working
		return TRUE;

	case 21:	// 스테이지 20mm 다운 후 이동
		if (m_nAngleTray1Case <=20)
		{
			dStage2Z = m_pMoveData->dAngleStage2Z[2] - 20.0;	// CM Pick Up Position - 20mm
			g_objAJinAXL.Move_Absolute(AX_ANGLE_STAGE2_Z, dStage2Z); 
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 20);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 20);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 21);
		}
		return TRUE;
	case 22:	// Down Check
		if (g_objAJinAXL.Is_MoveDone(AX_ANGLE_STAGE2_Z, dStage2Z))
		{
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 21);
		}
		break;

	case 23:	// Y Axis Empty Tray Unload Position
		if (m_nAngleTray1Case == 30) return TRUE;	// Empty Tray Unloading
		if (m_nAngleTray1Case > 52 || m_nAngleTray1Case < 23)
		{	// Inter lock			
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, 4); // Unload Position
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 22);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 22);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 23);
		}
		return TRUE;
	case 24:	// Position Check & Z Axis Move To Tray Pick Up Position
		if (m_pDX02->iAngleStage2Normal && !m_pDX02->iAngleStage2Rotate && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 4)) 
		{
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 4); // Tray Pick Up Position
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 23);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 24);
		}
		break;
	case 25:	// Position Check
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 4)) 
		{
			m_bBtm1ModulePick = FALSE;
			m_nAngleTray2Case = 30; m_tAngleTray2Loop.Set_LoopTime(5000);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 24);
		}
		break;
	case 30:	// Empty Tray Unload Working
		return TRUE;

	case 31:	// Z Axis Move to Ready Down Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 4) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 4) && !m_pDX02->iAngleStage2Exist) 
		{			
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 2);
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(30000);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 30);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 30);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 31);
		}
		break;
	case 32:	//
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 4) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 2)) 
		{			
			m_strLog.Format("AngleStage2, %d", GetTickCount() - m_dwAngleTray2);
			g_objLogFile.Save_TestLog(m_strLog);
			m_nAngleTray2Case = 50; m_tAngleTray2Loop.Set_LoopTime(30000);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 31);
		}
		break;

	case 50:	// Wait for Loading
		if (m_nAngleTray1Case > 9 && m_nAngleTray1Case < 50) m_nAngleTray2Case++; 
		return TRUE;

	case 51:	// Y Axis Move to Load Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 4) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 2))
		{			
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 0);
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(30000);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 50);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 50);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 51);
		}
		break;
	case 52:	// Check Down Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 2) &&
			m_pDX02->iAngleStage2Normal && !m_pDX02->iAngleStage2Rotate) 
		{			
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, 0);
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(30000);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 51);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 52);
		}
		break;
	case 53:	// Check Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 0)) 
		{			
			m_pDY02->oAngleStage2AlignIn = FALSE;	// Tray Align Out
			g_objAJinAXL.Write_Output(2);
			m_nAngleTray2Case = 0; m_tAngleTray2Loop.Set_LoopTime(5000);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 52);
			m_tAngleTray2Loop.Takt_Start(nTaktZone, 53);
			m_tAngleTray2Loop.Takt_End(nTaktZone, 53);
		}
		break;
	}

	// 5. (Error : 3500)
	if (m_tAngleTray2Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(3500 + m_nAngleTray2Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 6. (Error : 3600)
BOOL CSequenceMain::Btm1Picker_Run()
{
	int nTaktZone = 6;		// Takt_Start, Takt_End
	static int nB1pWorkTray = 0;				// 작업중인 Tray (1 ~ 2)
	static int nB1PStartNo = 0, nB1PickNo = 0;	// Btm1 Picker 시작 번호(0~9), 열별 순번(0~4)
	static int nB1pTrayPosY = 0;				// 작업중인 Tray 위치 Y (R04A:0 ~ 3, R04B:0 ~ 2, R04C:0 ~ 3)
	static double dB1pTrayY = 0.0, dB1pX = 0.0,	dB1ScanpX = 0.0;// Y, X, Z 이동위치
	static int	nB1pRow = 0, nB1pScanNo = 0, nB1pInspStageNo = 0;
	static int	nB1INo1 = 0, nB1INo2 = 0, nB1INo3 = 0, nB1INo4 = 0, nB1INo5 = 0, nB1INo6 = 0, nB1INo7 = 0, nB1INo8 = 0;;	// 검사할때 피커 번호
	static int	nErrRetry = 0;
	static BOOL bAngleGo1 = FALSE, bAngleGo2 = FALSE;
	static int nPNo = 0;

	switch (m_nBtm1PickCase) {
	case 0:		// Wait for Angle Tray L1 Position
		if (m_nAngleTray1Case == 20 || m_nAngleTray2Case == 20) {
			m_nBtm1PickCase++;
		}
		return TRUE;

	case 1:		// Picker Z Move to Buffer Down Position
		if(!g_objCommon.Get_InfoBtm1PickerClose())break;
		//if(m_bBtm1ModulePick && !g_objCommon.Get_InfoBtm1PickerClose())break;
		if ((g_objCommon.Check_Position(AX_BTM1_PICKER_X, 0) || g_objCommon.Check_Position(AX_BTM1_PICKER_X, 1) ||
			g_objCommon.Check_Position(AX_BTM1_PICKER_X, 2) || g_objCommon.Check_Position(AX_BTM1_PICKER_X, 3)) && 
			g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 0)) {
			if (m_nAngleTray1Case != 20 && m_nAngleTray2Case != 20) break;	// 인터락
			if(bAngleGo1){	m_nAngleTray1Case = 21;	bAngleGo1 = FALSE; }
			if(bAngleGo2){	m_nAngleTray2Case = 21;	bAngleGo2 = FALSE;}

			if(m_nAngleTray1Case >= 21){
				if (m_nAngleTray2Case != 20)break;
			}

			if(m_nAngleTray2Case >= 21){
				if (m_nAngleTray1Case != 20)break;
			}

			if (m_nAngleTray1Case == 20) nB1pWorkTray = 1;
			if (m_nAngleTray2Case == 20) nB1pWorkTray = 2;
			m_dwBtm1Pick = GetTickCount();
			m_tBtm1PickLoop.Takt_Start(nTaktZone, 1, TRUE);
			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(10000);
			
		}
		break;

	case 2:		// Angle Tray Y, Btm1 Picker X Move to Tray Down Position

		if ((nB1pWorkTray == 1 && m_nAngleTray2Case >= 30 && m_nAngleTray2Case < 53 && (nB1pRow == 0 && nB1pTrayPosY > 1)) ||
			(nB1pWorkTray == 2 && m_nAngleTray1Case >= 30 && m_nAngleTray1Case < 53 && (nB1pRow == 0 && nB1pTrayPosY > 1))) return TRUE;	// Interlock Empty Tray 작업 중에는 픽업을 하지 않는다.

		if (!g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0) ) break;
		if (!g_objCommon.Get_InfoBtm1PickerClose()) break;

		if ((nB1pWorkTray == 1 && !g_objAJinAXL.Is_Done(AX_ANGLE_STAGE1_Y)) ||
			(nB1pWorkTray == 2 && !g_objAJinAXL.Is_Done(AX_ANGLE_STAGE2_Y))) break;
		// Pick Position
		if (Select_Btm1PickPos(nB1PStartNo)) {
			
			Select_AngleTrayPos(nB1pWorkTray, nB1pTrayPosY, nB1pRow);
			if (nB1PStartNo > 3) nB1pRow = 1;
			else				 nB1pRow = 0;

			// 잔량대응 인터락. Picker 두번째 열이 집을때 Tray Y가 2번째줄 이상이면 앞의 Stage가 Unload 위치로 완전히 빠져나가고 나서 움직여준다.
			if (m_bBtm1ModulePick && nB1pTrayPosY > 0 && nB1pRow == 1) {
				if ((nB1pWorkTray == 1 && m_nAngleTray2Case >= 20 && m_nAngleTray2Case < 25) ||
					(nB1pWorkTray == 2 && m_nAngleTray1Case >= 20 && m_nAngleTray1Case < 25)) return TRUE;
			}
			
			// 0: Stage1 L1, 1:Stage1 L2, 2:Stage2 L1, 3:Stage2 L2
			if (nB1pWorkTray == 1) {
				if (nB1pRow == 0) dB1pX = m_pMoveData->dBtm1PickerX[0];	//Stage1 L1 Position
				if (nB1pRow == 1) dB1pX = m_pMoveData->dBtm1PickerX[1];	//Stage1 L2 Position
			}
			if (nB1pWorkTray == 2) {
				if (nB1pRow == 0) dB1pX = m_pMoveData->dBtm1PickerX[2];	//Stage2 L1 Position
				if (nB1pRow == 1) dB1pX = m_pMoveData->dBtm1PickerX[3];	//Stage2 L2 Position
			}
			g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_X, dB1pX);

			if (nB1pWorkTray == 1 && m_nAngleTray1Case == 20) {
				dB1pTrayY = m_pMoveData->dAngleStage1Y[2+nB1pRow] + nB1pTrayPosY * m_pEquipData->dTrayPitchY;
				g_objAJinAXL.Move_Absolute(AX_ANGLE_STAGE1_Y, dB1pTrayY);
			}
			if (nB1pWorkTray == 2 && m_nAngleTray2Case == 20) {
				dB1pTrayY = m_pMoveData->dAngleStage2Y[2+nB1pRow] + nB1pTrayPosY * m_pEquipData->dTrayPitchY;
				g_objAJinAXL.Move_Absolute(AX_ANGLE_STAGE2_Y, dB1pTrayY);
			}
			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(10000);


			nPNo = gData.nPNoBtm1Pick;
			if(nPNo < 1) nPNo = gData.nPNoAngleTray[nB1pWorkTray - 1];
			m_tBtm1PickLoop.Takt_End(nTaktZone, 1, nPNo);
			m_tBtm1PickLoop.Takt_Start(nTaktZone, 2);
		}			
		break;
	case 3:		// Btm1 Picker Down
		if (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX) ) {
			if ((nB1pWorkTray == 1 && g_objAJinAXL.Is_MoveDone(AX_ANGLE_STAGE1_Y, dB1pTrayY)) ||
				(nB1pWorkTray == 2 && g_objAJinAXL.Is_MoveDone(AX_ANGLE_STAGE2_Y, dB1pTrayY))) {

				if ((nB1pWorkTray == 1 && m_nAngleTray2Case > 15 && m_nAngleTray2Case <= 20 && !g_objAJinAXL.Is_Done(AX_ANGLE_STAGE2_Y)) ||
					(nB1pWorkTray == 2 && m_nAngleTray1Case > 15 && m_nAngleTray1Case <= 20 && !g_objAJinAXL.Is_Done(AX_ANGLE_STAGE1_Y))) break;
								
				g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 1);	// Load Down
				if (!m_tBtm1PickLoop.Waiting_Time(10)) break;
				if (nB1pRow == 0) g_objCommon.Set_Btm1Picker2RowUp();
				if (nB1pRow == 1) g_objCommon.Set_Btm1PickerRow2Down();
				m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);

				nPNo = gData.nPNoBtm1Pick;
				if(nPNo < 1) nPNo = gData.nPNoAngleTray[nB1pWorkTray - 1];
				m_tBtm1PickLoop.Takt_End(nTaktZone, 2, nPNo);
				m_tBtm1PickLoop.Takt_Start(nTaktZone, 3);
			}
		}
		break;
	case 4:		// Btm1 Picker Grip Close
		if (g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 1)) {
			if ((nB1pRow == 1 && g_objCommon.Get_Btm1PickerDown(5)) ||
				(nB1pRow == 0 && g_objCommon.Get_Btm1Picker2RowUp())) {
				g_objCommon.Set_InfoBtm1PickerClose(INFO_STAGE, nB1pRow, nB1pWorkTray, nB1pTrayPosY);
				//if (!m_tBtm1PickLoop.Waiting_Time(m_pEquipData->nDelayAdd[0])) break;	// Btm1 Delay
				
				g_objCommon.Set_InfoBtm1PickerVacOn(INFO_STAGE, nB1pRow, nB1pWorkTray, nB1pTrayPosY);
				if (!m_tBtm1PickLoop.Waiting_Time(m_pEquipData->nDelayAdd[0])) break;
				m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);

				//////////Info Exchange//////////////////////////////////////////////////////////
				int nTrayX = 0, nBtmX = 4 * nB1pRow;

				for (int i = 0; i < gData.nBtmPickQt; i++) {
					gData.InfoBtm1Pick[nBtmX+i] = gData.InfoAngleTray[nB1pWorkTray-1][nB1pTrayPosY][nTrayX+i]; 
					gData.InfoAngleTray[nB1pWorkTray-1][nB1pTrayPosY][nTrayX+i] = 0;
					gData.nTNoBtm1Pick[nBtmX+i] = gData.nTNoAngleTray[nB1pWorkTray-1];

					// Tray 하단부터 모듈 적재된다.
					gData.nCNoBtm1Pick[nBtmX+i] = (gData.InfoBtm1Pick[nBtmX+i] > 0 ? ((gData.nTrayY - 1 - nB1pTrayPosY) * gData.nTrayX + nTrayX + i + 1) : 0);

					//모듈트랙킹
					int nPNo = gData.nPNoAngleTray[nB1pWorkTray-1] - 1;
					int nTNo = gData.nTNoAngleTray[nB1pWorkTray-1] - 1;
					int nCNo = gData.nCNoBtm1Pick[nBtmX+i] - 1;
					if (nCNo > -1) {
						gData.nCmJigNo[nPNo][nTNo][nCNo][ANGLE_STAGE] = nB1pWorkTray;	// Stage No.
						gData.nCmJigNo[nPNo][nTNo][nCNo][BTM1_PICK]   = nBtmX + i + 1;	// Btm1 Jig No.
					}
				}
				gData.nPNoBtm1Pick = gData.nPNoAngleTray[nB1pWorkTray-1];
				///////////////////////////////////////////////////////////////
				

				m_tBtm1PickLoop.Takt_End(nTaktZone, 3);
				m_tBtm1PickLoop.Takt_Start(nTaktZone, 4);
			}
		}
		break;
	case 5:		// Btm1 Picker Up & Info Exchange
		
		if (1) {
				// Btm1 Delay
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 0);	// Ready up
												
			if (Check_AngleTrayEmpty(nB1pWorkTray)) {	// 트레이가 다 비었을때 초기화 해준다.
				gData.nPNoAngleTray[nB1pWorkTray-1] = 0;
				gData.nTNoAngleTray[nB1pWorkTray-1] = 0;
			}
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, nB1pWorkTray-1);
			g_objCommon.Set_InfoBtm1PickerVacOn(INFO_PICK);	// 모듈 없는 부분은 Vac 꺼준다.
			g_objCommon.Set_InfoBtm1PickerClose(INFO_PICK);	// 모듈 없는 부분은 Open 해준다.


			if (Check_Btm1PickerFull() || Check_AngleTrayEmpty(nB1pWorkTray)) {
				if (Check_AngleTrayEmpty(nB1pWorkTray) || (nB1pTrayPosY + 1 >= gData.nTrayY)) {	// Tray Change
					nB1pTrayPosY = 0;
					if (nB1pWorkTray == 1 && m_nAngleTray1Case == 20) { 
						nB1pWorkTray = 0; bAngleGo1 = TRUE; }
					if (nB1pWorkTray == 2 && m_nAngleTray2Case == 20) { 
						nB1pWorkTray = 0; bAngleGo2 = TRUE; }
					//MCC
				}

				// R04B 모델은 마지막 라인을 집고 다음 트레이에서 나머지 한줄(Picker L2) 집을 준비를 한다. Tray 5x3
				if (Check_Btm1PickerFull() || Check_LoadLotEnd(gData.nPNoBtm1Pick, 1)) { 
					nB1pTrayPosY++;
					m_nBtm1PickCase= 7; m_tBtm1PickLoop.Set_LoopTime(5000); 
				}
				else { 
					 m_nBtm1PickCase = 1; m_tBtm1PickLoop.Set_LoopTime(20000); 
				}

			} else {
				if (Select_Btm1PickPos(nB1PStartNo)) {
					if (nB1PStartNo > 3) nB1pRow = 1;
					else				 nB1pRow = 0;
				}
				nB1pTrayPosY++;
				m_nBtm1PickCase = 2; m_tBtm1PickLoop.Set_LoopTime(5000);
				//MCC
				m_strLog.Format("Btm 1 position cal to pick ");
				g_objLogFile.Save_MCCLog(m_strLog);
			}

			m_tBtm1PickLoop.Takt_End(nTaktZone, 4);
			m_tBtm1PickLoop.Takt_Start(nTaktZone, 5);
			m_tBtm1PickLoop.Takt_End(nTaktZone, 5);
		}
		break;

	case 7:		// X Axis Move To Btm1 Inspection Position
		if ( g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0) && g_objCommon.Get_InfoBtm1PickerVacOn() 
			&& g_objCommon.Get_InfoBtm1PickerClose()) {
			dB1pX = m_pMoveData->dBtm1PickerX[4];	// Btm1 Inspect
			g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_X, dB1pX);
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 2);	// Btm1 SP_DOWN
			g_objCommon.Move_Position(AX_BTM1_SHIFT_Y, 0); //Angle 1 Y Position 
			g_objCommon.Set_Btm1PickerDown();
			nB1pScanNo = 0;
			
			if(bAngleGo1){
				m_nAngleTray1Case = 21;
				bAngleGo1 = FALSE;
			}
			if(bAngleGo2){
				m_nAngleTray2Case = 21;
				bAngleGo2 = FALSE;
			}

			////////////////////////////////////////////////////////////////////////////
			int nLastT = gData.nTrayUseCount[gData.nPNoBtm1Pick-1];
			int nLastC = gData.nCmUseCount[gData.nPNoBtm1Pick-1] % gData.nCmMaxCount;
			if (nLastC == 0) nLastC = gData.nCmMaxCount;
			for (int i = 0; i < 10; i++) {
				if (gData.nTNoBtm1Pick[i] == 1 && gData.nCNoBtm1Pick[i] == 1) m_dwFirstLoad = GetTickCount() - m_dwFirstLoad;	// First Module
				if (gData.nTNoBtm1Pick[i] == nLastT && gData.nCNoBtm1Pick[i] == nLastC) m_dwLastUnLoad = GetTickCount();		// Last Module
			}
			
			m_tBtm1PickLoop.Takt_Start(nTaktZone, 7);
			m_tBtm1PickLoop.Takt_End(nTaktZone, 7 );
			m_nBtm1PickCase = 15; m_tBtm1PickLoop.Set_LoopTime(10000);			
		}
		break;

	case 9:		// Set Load Complete
		if (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX)  
			&&  g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 2)
			&& g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX) &&
			g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 0) 
			)
		{
			
			m_tBtm1PickLoop.Takt_Start(nTaktZone, 9);
			m_tBtm1PickLoop.Takt_End(nTaktZone, 9 );

			g_objLogFile.VisionTakt_Start(BTM1);	// Stiffener Scan Start
			if (m_pEquipData->bUseInspectBtm1Specular) {
				Set_InspectJigNo(0, nB1pScanNo, nB1INo1, nB1INo2, nB1INo3, nB1INo4);	// 검사하는 피커 번호를 설정해준다.
				g_objInspector.Set_LoadComplete(INSPECTOR_PC1, "B1SP", gLot.sLotID[gData.nPNoBtm1Pick-1], gData.nPNoBtm1Pick, 
					gData.nTNoBtm1Pick[nB1INo1-1], gData.nTNoBtm1Pick[nB1INo2-1], 0, 0,
					gData.nCNoBtm1Pick[nB1INo1-1], gData.nCNoBtm1Pick[nB1INo2-1], 0, 0);
					m_nBtm1PickCase = 10;m_tBtm1PickLoop.Set_LoopTime(30000);
				
			} else {
				m_nBtm1PickCase = 15; m_tBtm1PickLoop.Set_LoopTime(5000);
			}
			
			
			nB1pScanNo++;
		}
		break;
	
	case 10:	// Wait
		if (!m_pEquipData->bUseInspectBtm1Specular) {
			m_nBtm1PickCase = 15; m_tBtm1PickLoop.Set_LoopTime(5000);
		}
		break;

	

	case 15:	// X Move to Pitch & Inspection End Check
		//if (!m_pEquipData->bUseInspectBtm1Specular) { if(!m_tBtm1PickLoop.Waiting_Time(500)) break; }
		if(!g_objCommon.Get_Btm1PickerDown(0)){
			g_objCommon.Set_Btm1PickerDown(); break; 
		}
		if (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX) && g_objAJinAXL.Is_Done(AX_BTM1_PICKER_Z)
			&& g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 0)
			&& g_objCommon.Get_Btm1PickerDown(0) && g_objCommon.Check_Position(AX_BTM1_SHIFT_Y, 0) ) {
			//nB1pScanNo++;
			m_tBtm1PickLoop.Takt_Start(nTaktZone, 15);
			m_tBtm1PickLoop.Takt_End(nTaktZone, 15 );

			if (gData.bReload[1]) {
				nB1pScanNo=0; gData.bReload[1] = FALSE;
			}
			if (nB1pScanNo > 3) { //gData.nScanCnt
				nB1pScanNo = 0;
				g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 4);	// Btm1 AG Down
				m_nBtm1PickCase = 35; m_tBtm1PickLoop.Set_LoopTime(10000);
			} else {
				if (Select_BtmScanPos(AUTO_VISION_BTM1, nB1pScanNo)) {
					m_strLog.Format("Specular Scan Complete");
					g_objLogFile.Save_MCCLog(m_strLog);

					dB1pX = m_pMoveData->dBtm1PickerX[4] - nB1pScanNo * m_pEquipData->dTrayPitchX;	// Btm1 Inspect

					g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_X, dB1pX);			
					g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 2);	// Btm1 SP Down
					
										
					m_nBtm1PickCase = 9; m_tBtm1PickLoop.Set_LoopTime(10000);
				} else {
					nB1pScanNo++;
				}
			}
		}
		break;
	case 29:		// Set Load Complete
		if (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX)  && (g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0) || g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 4) )
			&& g_objCommon.Check_Position(AX_BTM1_SHIFT_Y, 0)) {
						
			m_tBtm1PickLoop.Takt_Start(nTaktZone, 29);
			m_tBtm1PickLoop.Takt_End(nTaktZone, 29 );
			
			if (m_pEquipData->bUseInspectBtm1Angle) {
				Set_InspectJigNo(4, nB1pScanNo, nB1INo1, nB1INo2, nB1INo3, nB1INo4);	// 검사하는 피커 번호를 설정해준다.
				g_objInspector.Set_LoadComplete(INSPECTOR_PC1, "B1AG", gLot.sLotID[gData.nPNoBtm1Pick-1], gData.nPNoBtm1Pick, 
					gData.nTNoBtm1Pick[nB1INo1-1], gData.nTNoBtm1Pick[nB1INo2-1], 0, 0,
					gData.nCNoBtm1Pick[nB1INo1-1], gData.nCNoBtm1Pick[nB1INo2-1], 0, 0);
					m_nBtm1PickCase = 30;m_tBtm1PickLoop.Set_LoopTime(30000);
				

			} else {
				m_nBtm1PickCase = 35; m_tBtm1PickLoop.Set_LoopTime(5000);
			}
			nB1pScanNo++;
		}
		break;

	case 30:	// Wait
		if (!m_pEquipData->bUseInspectBtm1Angle) {
			m_nBtm1PickCase = 35; m_tBtm1PickLoop.Set_LoopTime(5000);
		}
		break;

	case 33:	// Btm1 Shift Y Move
		if (g_objAJinAXL.Is_Done(AX_BTM1_SHIFT_Y) && g_objAJinAXL.Is_Done(AX_BTM1_PICKER_X) && g_objAJinAXL.Is_Done(AX_BTM1_PICKER_Z)) {
			g_objCommon.Move_Position(AX_BTM1_SHIFT_Y, 1);

			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
		}
		break;
	case 34:	// Send Shift Complete
		if (g_objCommon.Check_Position(AX_BTM1_SHIFT_Y, 1) && g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX) && g_objAJinAXL.Is_Done(AX_BTM1_PICKER_Z)) {
			g_objInspector.Set_SMoveComplete(INSPECTOR_PC1, "B1AG");
			m_nBtm1PickCase = 30; m_tBtm1PickLoop.Set_LoopTime(30000);	// 90초
		}
		break;

	case 35:	// X Move to Pitch & Inspection End Check
		//if (!m_pEquipData->bUseInspectBtm1Angle) { if(!m_tBtm1PickLoop.Waiting_Time(1)) break; }

		if (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX)  && g_objAJinAXL.Is_Done(AX_BTM1_PICKER_Z)
			&& g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 0)
			&& g_objAJinAXL.Is_Done(AX_BTM1_SHIFT_Y)) {
				//&& g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 4)
				//nB1pScanNo++;
				
				m_tBtm1PickLoop.Takt_Start(nTaktZone, 35);
				m_tBtm1PickLoop.Takt_End(nTaktZone, 35);

				if (gData.bReload[5]) {
					nB1pScanNo=0; gData.bReload[5] = FALSE;
				}
				if (nB1pScanNo > 3) { 
					nB1pScanNo = 0;
					
					
					if (m_pEquipData->bUseInspectBtm13D) {
						//MCC
						m_strLog.Format("Btm1Picker, X,Z Move to Start Position");g_objLogFile.Save_MCCLog(m_strLog);
											
						g_objCommon.Move_Position(AX_BTM1_SHIFT_Y, 0); //Angle 1 Y Position 
						m_nBtm1PickCase = 36; m_tBtm1PickLoop.Set_LoopTime(5000);				
					}
					else{
						m_nBtm1PickCase = 47; m_tBtm1PickLoop.Set_LoopTime(5000);
					}

				} else {
					if (Select_BtmScanPos(AUTO_VISION_BTM1, nB1pScanNo)) {
						dB1pX = m_pMoveData->dBtm1PickerX[8]- m_pEquipData->dTrayPitchX * nB1pScanNo;	// Btm1 Inspect

						g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_X, dB1pX);			
						g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 4);	// Btm1 AG Down 
						g_objCommon.Move_Position(AX_BTM1_SHIFT_Y, 0); //Angle 1 Y Position 
						
						m_nBtm1PickCase = 29; m_tBtm1PickLoop.Set_LoopTime(10000);
					} else {
						nB1pScanNo++;
					}
				}
		}
		break;
	case 36:
		if (g_objCommon.Check_Position(AX_BTM1_SHIFT_Y, 0))
		{
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 5);	// Btm1 3D Down 
			dB1ScanpX = m_pMoveData->dBtm1PickerX[6]; //3D Scan Ready position
			g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_X, dB1ScanpX);
			m_nBtm1PickCase = 41; m_tBtm1PickLoop.Set_LoopTime(5000);	
		}
		break;
	case 40:
		if (g_objAJinAXL.Is_Done(AX_BTM1_PICKER_X) && g_objAJinAXL.Is_Done(AX_BTM1_SHIFT_Y))
		{
			if (gData.bReload[6]) {
				gData.bReload[6] = FALSE;
			}

			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 5);	// Btm1 Ready Up
			dB1ScanpX = m_pMoveData->dBtm1PickerX[6]; //3D Scan Ready position (Start position)
			g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_X, dB1ScanpX);
			m_nBtm1PickCase = 41; m_tBtm1PickLoop.Set_LoopTime(5000);
		}
		break;
	

	case 41:
		if (g_objCommon.Check_Position(AX_BTM1_PICKER_X, 6) && g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 5)
			&& g_objCommon.Check_Position(AX_BTM1_SHIFT_Y, 0)) {
						
			m_tBtm1PickLoop.Takt_Start(nTaktZone, 41);
			m_tBtm1PickLoop.Takt_End(nTaktZone, 41);
			
			//Load Complete 			
			if (m_pEquipData->bUseInspectBtm13D) {
				
				// 검사하는 피커 번호를 설정해준다.
				Set_3DInspectJigNo(0,0,nB1INo1, nB1INo2, nB1INo3, nB1INo4,nB1INo5, nB1INo6, nB1INo7, nB1INo8);
				g_objInspector.Set_3DLoadComplete(INSPECTOR_PC1, "B13D", gLot.sLotID[gData.nPNoBtm1Pick-1], gData.nPNoBtm1Pick, 
					gData.nTNoBtm1Pick[nB1INo1-1], gData.nTNoBtm1Pick[nB1INo2-1], gData.nTNoBtm1Pick[nB1INo3-1], gData.nTNoBtm1Pick[nB1INo4-1],
					gData.nTNoBtm1Pick[nB1INo5-1], gData.nTNoBtm1Pick[nB1INo6-1], gData.nTNoBtm1Pick[nB1INo7-1], gData.nTNoBtm1Pick[nB1INo8-1],
					gData.nCNoBtm1Pick[nB1INo1-1], gData.nCNoBtm1Pick[nB1INo2-1], gData.nCNoBtm1Pick[nB1INo3-1], gData.nCNoBtm1Pick[nB1INo4-1],
					gData.nCNoBtm1Pick[nB1INo5-1], gData.nCNoBtm1Pick[nB1INo6-1], gData.nCNoBtm1Pick[nB1INo7-1], gData.nCNoBtm1Pick[nB1INo8-1]);
				m_nBtm1PickCase = 42; m_tBtm1PickLoop.Set_LoopTime(30000);
			} else {
				m_nBtm1PickCase = 47; m_tBtm1PickLoop.Set_LoopTime(5000);
			}
		}
		break;
	case 42:	// Wait
		if (!m_pEquipData->bUseInspectBtm13D) {
			m_nBtm1PickCase = 47;
			m_tBtm1PickLoop.Set_LoopTime(5000);
		}
		break;
	
	case 45:
		if ( g_objCommon.Check_Position(AX_BTM1_PICKER_X, 6)  && g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 5) &&
			  g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 0)) {
				  
			m_tBtm1PickLoop.Takt_Start(nTaktZone, 45);
			//Start Trigger with Setting Speed
			dB1ScanpX = m_pMoveData->dBtm1PickerX[7];
			if(m_pEquipData->bUsePMTrigger){
				g_objAJinAXL.SetEncoderActPos(AX_PM_TRIGGER, 0);
				g_objAJinAXL.Start_Trigger(AX_PM_TRIGGER,  m_pEquipData->dTriggerStart, m_pEquipData->dTriggerEnd, 
				m_pEquipData->dTriggerPeriod, (m_pEquipData->dTriggerPeriod/2) );
			}
			g_objAJinAXL.Move_Absolute_Vel(AX_BTM1_PICKER_X, dB1ScanpX, m_pEquipData->dTriggerVel);
						
			m_nBtm1PickCase = 47; 
			m_tBtm1PickLoop.Set_LoopTime(15000);
		}
		break;
		
	case 47:		
		if ((g_objAJinAXL.Is_Done(AX_BTM1_PICKER_Z) && g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1ScanpX))
			|| !m_pEquipData->bUseInspectBtm13D)
		{
			if(m_pEquipData->bUsePMTrigger)
			{
				g_objAJinAXL.Stop_Trigger(AX_PM_TRIGGER);
			}

			//3D 스캔 완료 전달 
			g_objInspector.Set_3DENDComplete(INSPECTOR_PC1);

			if (gData.bReload[6]) {
				gData.bReload[6] = FALSE;
				m_nBtm1PickCase = 41;
				break;
			}
			m_tBtm1PickLoop.Takt_End(nTaktZone, 45 );
			m_tBtm1PickLoop.Takt_Start(nTaktZone, 47);
			m_tBtm1PickLoop.Takt_End(nTaktZone, 47);
			
			g_objCommon.Move_Position(AX_BTM1_PICKER_P1, 1);	// Inspection Position
			g_objCommon.Move_Position(AX_BTM1_PICKER_P2, 1);	// Inspection Position
			g_objCommon.Move_Position(AX_BTM1_PICKER_X, 5);
			m_nBtm1PickCase = 21; m_tBtm1PickLoop.Set_LoopTime(5000);
		}
		break;
	case 21:
		if(g_objCommon.Check_Position(AX_BTM1_PICKER_X, 5) 
			&& g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 1)
			&& g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 1)){
			
			if (m_nInspect1Case == 0 || m_nInspect2Case == 0 || m_nInspect3Case == 0) {
				if (m_nInspect1Case == 0) nB1pInspStageNo = 1;
				if (m_nInspect2Case == 0) nB1pInspStageNo = 2;
				if (m_nInspect3Case == 0) nB1pInspStageNo = 3;
				nErrRetry = 0;	//Retry Clear
				m_nBtm1PickCase = 22; m_tBtm1PickLoop.Set_LoopTime(5000);

			} 
			else {
				m_tBtm1PickLoop.Set_LoopTime(5000); return TRUE; 
			}	// 앞공정이 밀려있어서 Stage가 없으면 대기.
		}
		break;
	
	case 22:	// 안전 확인 후 Picker Down, Inspection Stage Vac On
		if (g_objCommon.Check_Position(AX_BTM1_PICKER_X, 5) 
				&& g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 1) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 1)) {
			
			if ((nB1pInspStageNo == 1 && m_pDX05->iInspectStage1Up && !m_pDX05->iInspectStage1Down) ||
				(nB1pInspStageNo == 2 && m_pDX05->iInspectStage2Up && !m_pDX05->iInspectStage2Down && m_pDX05->iInspectStage2Fwd && !m_pDX05->iInspectStage2Bwd) ||
				(nB1pInspStageNo == 3 && m_pDX05->iInspectStage3Up && !m_pDX05->iInspectStage3Down && m_pDX05->iInspectStage3Fwd && !m_pDX05->iInspectStage3Bwd)) {
				g_objCommon.Set_Btm1PickerDown();
				g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 3);	//Unload Down
				g_objCommon.Set_InfoInspectVacOn(nB1pInspStageNo);
				/////////////////////Info Exchange///////////
				for (int i = 0; i < 10; i++) {
					gData.InfoInspect[nB1pInspStageNo-1][i] = gData.InfoBtm1Pick[i]; gData.InfoBtm1Pick[i] = 0;

					gData.nTNoInspect[nB1pInspStageNo-1][i] = gData.nTNoBtm1Pick[i];
					gData.nCNoInspect[nB1pInspStageNo-1][i] = gData.nCNoBtm1Pick[i];
					gData.nTNoBtm1Pick[i] = gData.nCNoBtm1Pick[i] = 0;

					//모듈트랙킹
					int nPNo = gData.nPNoBtm1Pick - 1;
					int nTNo = gData.nTNoInspect[nB1pInspStageNo-1][i] - 1;
					int nCNo = gData.nCNoInspect[nB1pInspStageNo-1][i] - 1;
					if (nCNo > -1) {
						gData.nCmJigNo[nPNo][nTNo][nCNo][INSP_STAGE_NO] = nB1pInspStageNo;	// Inspection Stage No.
						gData.nCmJigNo[nPNo][nTNo][nCNo][INSP_JIG_NO]	= i + 1;			// Inspection Stage Jig No.
					}
				}
				gData.nPNoInspect[nB1pInspStageNo-1] = gData.nPNoBtm1Pick; gData.nPNoBtm1Pick = 0;
				/////////////////////////////


				m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
				
				m_tBtm1PickLoop.Takt_Start(nTaktZone, 22);
			}
		}
		break;
	case 23:	// Btm1 Picker Vac Off
		if (g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 3) && g_objCommon.Get_Btm1PickerDown(0)) {
			
			g_objCommon.Set_Btm1PickerOpen(0);
			m_pThreadVacuumB1p = AfxBeginThread(Thread_Vacuum_B1p, (LPVOID)0);
			//MCC
			m_strLog.Format("Btm 1 Picker Put on inspect stage Complete, stageNo:%d", nB1pInspStageNo);
			g_objLogFile.Save_MCCLog(m_strLog);

			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
			m_tBtm1PickLoop.Takt_End(nTaktZone, 22);
			m_tBtm1PickLoop.Takt_Start(nTaktZone, 23);
		}
		break;
	case 24:	// Picker Up
		if (!m_pThreadVacuumB1p && g_objCommon.Get_Btm1PickerOpen(0)) 
		{
			g_dlgWork.ResetInfoDisplay();
			if (!m_tBtm1PickLoop.Waiting_Time(m_pEquipData->nDelayAdd[0])) break;	// Btm1 Delay
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 0);	// Ready Up	

			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(10000);
			m_tBtm1PickLoop.Takt_End(nTaktZone, 23);
			m_tBtm1PickLoop.Takt_Start(nTaktZone, 24);
		}
		break;
	case 25:	// X, P1, P2 Move to Load Position
		if (g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0) && g_objCommon.Get_Btm1PickerVacOff(0)) {
			// Inspection Stage Start
			if (nB1pInspStageNo = 1 && m_nInspect1Case == 0) m_nInspect1Case = 1;
			if (nB1pInspStageNo = 2 && m_nInspect2Case == 0) m_nInspect2Case = 1;
			if (nB1pInspStageNo = 3 && m_nInspect3Case == 0) m_nInspect3Case = 1;

			m_tBtm1PickLoop.Takt_End(nTaktZone, 24);
			m_tBtm1PickLoop.Takt_Start(nTaktZone, 25);
			if (nB1pWorkTray == 0) {
				dB1pX = m_pMoveData->dBtm1PickerX[0];
				g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_X, dB1pX);
				
				g_objCommon.Move_Position(AX_BTM1_PICKER_P1, 0);
				g_objCommon.Move_Position(AX_BTM1_PICKER_P2, 0);

				m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
			} else {
				// Pick Position
				if (Select_Btm1PickPos(nB1PStartNo)) {	// Tray
					
					Select_AngleTrayPos(nB1pWorkTray, nB1pTrayPosY, nB1pRow);
					if (nB1PStartNo > 3) { nB1pRow = 1; }
					else				 { nB1pRow = 0; }

					// 0: Stage1 L1, 1:Stage1 L2, 2:Stage2 L1, 3:Stage2 L2
					if (nB1pWorkTray == 1) {
						if (nB1pRow == 0) dB1pX = m_pMoveData->dBtm1PickerX[0];	//Stage1 L1 Position
						if (nB1pRow == 1) dB1pX = m_pMoveData->dBtm1PickerX[1];	//Stage1 L2 Position
					}
					if (nB1pWorkTray == 2) {
						if (nB1pRow == 0) dB1pX = m_pMoveData->dBtm1PickerX[2];	//Stage2 L1 Position
						if (nB1pRow == 1) dB1pX = m_pMoveData->dBtm1PickerX[3];	//Stage2 L2 Position
					}
					g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_X, dB1pX);

					if (nB1pWorkTray == 1 && m_nAngleTray1Case == 20) {
						dB1pTrayY = m_pMoveData->dAngleStage1Y[2+nB1pRow] + nB1pTrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_ANGLE_STAGE1_Y, dB1pTrayY);
					}
					if (nB1pWorkTray == 2 && m_nAngleTray2Case == 20) {
						dB1pTrayY = m_pMoveData->dAngleStage2Y[2+nB1pRow] + nB1pTrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_ANGLE_STAGE2_Y, dB1pTrayY);
					}

					g_objCommon.Move_Position(AX_BTM1_PICKER_P1, 0);
					g_objCommon.Move_Position(AX_BTM1_PICKER_P2, 0);

					m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(25000); //3626 알람 빈도 높아서 조치함
				}
			}
		}
		break;
	case 26:
		{  //앞에 트레이 btm1 Line 1이 마지막 열을 집는 위치로 가면 그 다음 트레이를 btm1 picker line2 가 1번째 열을 집는 위치로 보낸다. 
			if (nB1pWorkTray == 2 && m_nAngleTray2Case == 20 && nB1pRow == 0 && nB1pTrayPosY == 2){
				if(!Check_LoadLotEndBtm1(gData.nPNoBtm1Pick+1, 0)){ //뒤에 트레이가 안올때는 스킵
					if (m_nAngleTray1Case != 15) return TRUE;
					m_bBtm1ModulePick = TRUE;
					m_nAngleTray1Case = 16;
				}
			}
			if (nB1pWorkTray == 1 && m_nAngleTray1Case == 20 && nB1pRow == 0 && nB1pTrayPosY == 2){
				if(!Check_LoadLotEndBtm1(gData.nPNoBtm1Pick+1, 1)){//뒤에 트레이가 안올때는 스킵
					if (m_nAngleTray2Case != 15) return TRUE;
					m_bBtm1ModulePick = TRUE;
					m_nAngleTray2Case = 16;
				}				
			}
			m_nBtm1PickCase = 27; m_tBtm1PickLoop.Set_LoopTime(5000);
		}
		break;
	case 27:
		if (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX) && g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 0)) {
			m_tBtm1PickLoop.Takt_End(nTaktZone, 25);
			m_tBtm1PickLoop.Takt_Start(nTaktZone, 26);
			m_tBtm1PickLoop.Takt_End(nTaktZone, 26);
			m_strLog.Format("Btm1 Picker, %d", GetTickCount() - m_dwBtm1Pick);
			g_objLogFile.Save_TestLog(m_strLog);
			if (nB1pWorkTray == 0) { 
				m_nBtm1PickCase = 0; m_tBtm1PickLoop.Set_LoopTime(5000);
			}
			else				   {
				m_nBtm1PickCase = 2; m_tBtm1PickLoop.Set_LoopTime(5000);
			}
		}
		break;
	}

	// 6. (Error : 3600)
 	if (m_tBtm1PickLoop.Over_LoopTime()) {
 		if (m_nBtm1PickCase == 6) {
 			gData.nErrAngleStageNo = nB1pWorkTray;
 
 			int nPx = gData.nPNoAngleTray[nB1pWorkTray-1] - 1;
 			int nTx = gData.nTNoAngleTray[nB1pWorkTray-1] - 1;
 			int nCx = (gData.InfoBtm1Pick[4 * nB1pRow + gData.nErrBtm1PickIdx] > 0 ? ((gData.nTrayY - 1 - nB1pTrayPosY) * gData.nTrayX + gData.nErrBtm1PickIdx) : 0);
 			m_strLog.Format("%s,%s,Port(%d),Tray(%d),Pocket(%d),Btm1 Picker Vacuum Error", gData.sLotID[nPx], gMes.sBarID[nPx][nTx][nCx], nPx+1, nTx+1, nCx+1);
 			g_objLogFile.Save_PickerLog(m_strLog);
 
 		} else if (m_nBtm1PickCase == 10) {
 			if (nB1pScanNo > 2) {
 				Set_ScanError(3610, gData.nTNoBtm1Pick[nB1INo3-1], gData.nTNoBtm1Pick[nB1INo4-1], 0, 0, 
 									gData.nCNoBtm1Pick[nB1INo3-1], gData.nCNoBtm1Pick[nB1INo4-1], 0, 0);
 			} else {
 				Set_ScanError(3610, gData.nTNoBtm1Pick[nB1INo1-1], gData.nTNoBtm1Pick[nB1INo2-1], gData.nTNoBtm1Pick[nB1INo3-1], gData.nTNoBtm1Pick[nB1INo4-1],
 					                gData.nCNoBtm1Pick[nB1INo1-1], gData.nCNoBtm1Pick[nB1INo2-1], gData.nCNoBtm1Pick[nB1INo3-1], gData.nCNoBtm1Pick[nB1INo4-1]);
 			}
 
 		}
 		g_objCommon.Show_Error(3600 + m_nBtm1PickCase);
 		return FALSE;
 	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 7. (Error : 3700)
BOOL CSequenceMain::Inspection1_Run()
{
	int nTaktZone = 7;		// Takt_Start, Takt_End
	static int		nIs1ScanNo = 0;
	static double	dIs1PosX = 0.0;
	static int		nI1JNo1, nI1JNo2, nI1JNo3, nI1JNo4 = 0;	// 검사할때 Jig 번호
	static int		nAlignCnt = 0;
	static BOOL		bTaktSave1 = FALSE;

	switch (m_nInspect1Case) {
	case 0:		// Wait for Module Loading
		m_tInspect1Loop.Set_LoopTime(15000);
		return TRUE;

	case 1:		// 안전확인 & Move to Align Position
		{
			BOOL bInspStart = FALSE;
			BOOL bInspWaitStart = FALSE;
			if(!m_tInspect1Loop.Waiting_Time(5)) return TRUE;
			// 모두 다운
			if (!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down && !m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd &&
				!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd) { bInspStart = TRUE; }

			// R04A/B T1 마지막 검사 ~ Unloading, Inspect2번이 언로드 위치면 Inspect3번이 Top2 첫번째 검사 시작위치 진입. R04C T2 ~ Unloading
			if ((m_nInspect3Case > 7 && m_nInspect3Case <= 31) || bInspStart) {
				double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE3_X);
				double dT1_4thPos = m_pMoveData->dInspectStage3X[2] + (m_pEquipData->dInspectPitchX *3);
				double dUnPos = m_pMoveData->dInspectStage3X[4];	//Unload Pos
				if ((dCurPos >= dT1_4thPos && dCurPos <= dUnPos) || bInspStart) {	//위치 확인
					if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0) && g_objCommon.Get_InfoInspectVacOn(1) &&
						m_pDX05->iInspectStage1Up && !m_pDX05->iInspectStage1Down & g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0)) {
						m_dwInspect1 = GetTickCount();
						m_tInspect1Loop.Takt_Start(nTaktZone, 1, TRUE);
						m_tInspect1Loop.Takt_End(nTaktZone,1);
						g_objCommon.Move_Position(AX_INSPECT_STAGE1_X, 1);
						m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(9000);
					} else {
						break;
					}
				} 
				
			}
		}
		return TRUE;

	case 2:		// Align Z Axis Down
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 1) && 
			g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0) &&
			g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0) &&
			m_pDX06->iCmAlignMasterOpen && !m_pDX06->iCmAlignMasterClose &&
			m_pDX06->iCmAlignSlaveOpen  && !m_pDX06->iCmAlignSlaveClose) {
			
			g_objCommon.Move_Position(AX_MODULE_ALIGN_Z, 1);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000); 

			m_tInspect1Loop.Takt_Start(nTaktZone, 2);
			
		}
		break;
	case 3:		// CM Align Master/
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 1)) {
			m_pDY06->oCmAlignMasterOpen = FALSE; m_pDY06->oCmAlignMasterClose = TRUE;
			g_objAJinAXL.Write_Output(6);
			m_nInspect1Case = 9; m_tInspect1Loop.Set_LoopTime(5000);

			m_tInspect1Loop.Takt_End(nTaktZone,2);
			m_tInspect1Loop.Takt_Start(nTaktZone, 3);
		}
		break;
	case 9: //Slave Close
		if(!m_pDX06->iCmAlignMasterOpen && m_pDX06->iCmAlignMasterClose){

			if (!bTaktSave1) {
				bTaktSave1 = TRUE; 
				m_tInspect1Loop.Takt_End(nTaktZone,3); 
				m_tInspect1Loop.Takt_Start(nTaktZone,9); 
			}
			//g_objCommon.Set_InspectVacOff(1, 0);	// Vac Off
			//if(!m_tInspect1Loop.Waiting_Time(400)) break;

			m_pDY06->oCmAlignSlaveOpen = FALSE;  m_pDY06->oCmAlignSlaveClose = TRUE;
			g_objAJinAXL.Write_Output(6);
			m_nInspect1Case = 4; m_tInspect1Loop.Set_LoopTime(10000);
			bTaktSave1 = FALSE;
		}
		break;

	case 4:		// CM Align Vac On and Slave Open
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 1) && g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 1) &&
			!m_pDX06->iCmAlignMasterOpen && m_pDX06->iCmAlignMasterClose &&
			!m_pDX06->iCmAlignSlaveOpen  && m_pDX06->iCmAlignSlaveClose) {
				
			if (!bTaktSave1) {
				bTaktSave1 = TRUE; 
				m_tInspect1Loop.Takt_End(nTaktZone,9); 
				m_tInspect1Loop.Takt_Start(nTaktZone,4); 
			}
			g_objCommon.Set_InfoInspectVacOn(1, 1);	// Vac On

			// io WRITE를 동시에 하면 씹힐수 있어서 딜레이 추가
			if (!m_tInspect1Loop.Waiting_Time(10)) return TRUE;

			m_pDY06->oCmAlignSlaveOpen = TRUE;  m_pDY06->oCmAlignSlaveClose = FALSE;
			m_pDY06->oCmAlignMasterOpen = TRUE;  m_pDY06->oCmAlignMasterClose = FALSE;
			g_objAJinAXL.Write_Output(6);
			m_nInspect1Case=6; m_tInspect1Loop.Set_LoopTime(5000); 
			bTaktSave1 = FALSE;
		}
		break;
	case 6:		// Align Z Axis Move to Ready Up Position
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0) && g_objCommon.Get_InfoInspectVacOn(1) &&
			m_pDX06->iCmAlignMasterOpen && !m_pDX06->iCmAlignMasterClose &&
			m_pDX06->iCmAlignSlaveOpen  && !m_pDX06->iCmAlignSlaveClose) {
			if (!bTaktSave1) {
				bTaktSave1 = TRUE;
				m_tInspect1Loop.Takt_End(nTaktZone,5);
				m_tInspect1Loop.Takt_Start(nTaktZone, 6);
			}
			//if(!m_tInspect1Loop.Waiting_Time(100)) break;
			g_objCommon.Move_Position(AX_MODULE_ALIGN_Z, 0);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000); 
			bTaktSave1 = FALSE;
		}
		break;
	case 7:		// 안전확인 & Move to Top1 Position	// Mirror 실린더 확인
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0) && g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0) && g_objCommon.Get_InfoInspectVacOn(1)) {
			
			if (1) {	// Mirror 확인 안한다.
				// 다운 상태이거나 Top2, Unload 위치에 있으면 이동
				if (!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd) {	// 다운 상태
					dIs1PosX = m_pMoveData->dInspectStage1X[2];	// T1 Pos
					g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX);
					nIs1ScanNo = 0;
					m_nInspect1Case = 16; m_tInspect1Loop.Set_LoopTime(10000);
					m_tInspect1Loop.Takt_End(nTaktZone,6);
					m_tInspect1Loop.Takt_Start(nTaktZone, 7);
					m_tInspect1Loop.Takt_End(nTaktZone,7);

				} else if (m_nInspect3Case > 17 && m_nInspect3Case < 50) {	// Top2 ~ Unload 위치에 있을때
					double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE3_X);
					double dT2Pos = m_pMoveData->dInspectStage3X[2] + m_pEquipData->dInspectPitchX * 4 + 1;	// R04A/B T1 Pos + Inspection Stage Pitch * 4 + 1
					double dUnPos = m_pMoveData->dInspectStage3X[4];	//Unload Pos
					if (dCurPos > dT2Pos && dCurPos <= dUnPos) {	//위치 확인
						
						dIs1PosX = m_pMoveData->dInspectStage1X[2];	// T1 Pos
						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX);
						nIs1ScanNo = 0;
						bTaktSave1 = FALSE;
						m_nInspect1Case = 16; m_tInspect1Loop.Set_LoopTime(10000);
						m_tInspect1Loop.Takt_End(nTaktZone,6);
						m_tInspect1Loop.Takt_Start(nTaktZone, 7);
						m_tInspect1Loop.Takt_End(nTaktZone,7);
					} else { m_tInspect1Loop.Set_LoopTime(5000); return TRUE; }

				} else { m_tInspect1Loop.Set_LoopTime(5000); return TRUE; }
			}
		}
		break;
	case 8:		// Send Load Complete
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE1_X, dIs1PosX) && g_objCommon.Check_Position(AX_VISION_TOP1_Z, 0) && g_objCommon.Get_InfoInspectVacOn(1)) {
			
			g_objLogFile.VisionTakt_Start(TOP1);	// Top1 Scan Start

			if (m_pEquipData->bUseInspectTop1) {
				if (gData.bTop1MirrorErr[0]) {
					nIs1ScanNo--; gData.bTop1MirrorErr[0] = FALSE; 
					g_objCommon.Show_Error(gData.nTop1MirrorErrNo);
					return FALSE;
				}
				Set_InspectJigNo(1, nIs1ScanNo, nI1JNo1, nI1JNo2, nI1JNo3, nI1JNo4);
				g_objInspector.Set_LoadComplete(INSPECTOR_PC2, "T1", gLot.sLotID[gData.nPNoInspect[0]-1], gData.nPNoInspect[0],
					gData.nTNoInspect[0][nI1JNo1-1], gData.nTNoInspect[0][nI1JNo2-1], 0, 0,
					gData.nCNoInspect[0][nI1JNo1-1], gData.nCNoInspect[0][nI1JNo2-1], 0, 0);

				m_nInspect1Case = 10; m_tInspect1Loop.Set_LoopTime(30000);
				m_tInspect1Loop.Takt_Start(nTaktZone, 8);
				m_tInspect1Loop.Takt_End(nTaktZone,8);

			} else {
				m_nInspect1Case = 15; m_tInspect1Loop.Set_LoopTime(5000);
			}
			nIs1ScanNo++;
		}
		break;

	case 10:	// Wait for Top1 inspection(30sec)
		if (!m_pEquipData->bUseInspectTop1) {
			m_nInspect1Case = 15; m_tInspect1Loop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Top1 Z Focus Move
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			m_strLog.Format("MCC,(07) Inspection1, T1 Z Focus Move Start");
			g_objLogFile.Save_MCCLog(m_strLog);

			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, m_dTop1Z);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000);
			m_tInspect1Loop.Takt_Start(nTaktZone, 11);
			
		}
		break;
	case 12:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_TOP1_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dTop1Z, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_TOP1_Z, strLog);

			g_objInspector.Set_MoveComplete(INSPECTOR_PC2, "T1");
			m_nInspect1Case = 10; m_tInspect1Loop.Set_LoopTime(30000);	// 90초
			m_tInspect1Loop.Takt_End(nTaktZone,11);
			m_tInspect1Loop.Takt_Start(nTaktZone,12);
			m_tInspect1Loop.Takt_End(nTaktZone,12);
		}
		break;

	case 13:	// Top1 Mirror Z Move
		if (g_objAJinAXL.Is_Done(AX_TOP1_MIRROR_Z) && g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			//g_objLogFile.VisionTakt_Save(AUTO_INSPECTION1, 21, TOP1);	// Top1 Module Position Inspect Complete
			m_strLog.Format("MCC,(07) Inspection1, T1 Mirror Z Move Start");
			g_objLogFile.Save_MCCLog(m_strLog);

			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, m_dTop1Z);
			g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 1);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000);
			m_tInspect1Loop.Takt_Start(nTaktZone,13);
			
		}
		break;
	case 14:	// Send Move Complete
		if (g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 1) && g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			
			g_objLogFile.VisionTakt_Start(TOP1);	// Top1 Mirror Down 이후 Scan
			g_objInspector.Set_SMoveComplete(INSPECTOR_PC2, "T1");
			m_nInspect1Case = 10; m_tInspect1Loop.Set_LoopTime(30000);	// 90초

			m_tInspect1Loop.Takt_End(nTaktZone,13);
			m_tInspect1Loop.Takt_Start(nTaktZone,14);
			m_tInspect1Loop.Takt_End(nTaktZone,14);
		}
		break;

	case 15:	// Mirror Z축 안전 위치로 회피
		{
			g_objCommon.Move_Position(AX_VISION_TOP1_Z, 0);	// Ready Up
			g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 0);	// Ready Up
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(30000);
			m_tInspect1Loop.Takt_Start(nTaktZone, 15);
			m_tInspect1Loop.Takt_End(nTaktZone,15);
		}
		break;
	case 16:	// 다음 검사 위치 피치 이동
		if (g_objAJinAXL.Is_Done(AX_INSPECT_STAGE1_X) && g_objCommon.Check_Position(AX_VISION_TOP1_Z, 0) && g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0)) {
			if (!bTaktSave1) { bTaktSave1 = TRUE;  }	// Move to Top1 Position

			if (gData.bReload[2]) {
				nIs1ScanNo--; gData.bReload[2] = FALSE;
			}
			if (nIs1ScanNo > gData.nScanCnt) {	// 모델별로 피치이동 횟수가 다르다.
				nIs1ScanNo = 0;
				bTaktSave1 = FALSE;
				m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000);

				m_tInspect1Loop.Takt_Start(nTaktZone, 16);
				m_tInspect1Loop.Takt_End(nTaktZone,16);

			} else {
				double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE3_X);
				double dT2Pos = m_pMoveData->dInspectStage3X[3];// + m_pEquipData->dInspectPitchX * 2;
				double dUnPos = m_pMoveData->dInspectStage3X[4];	//Unload Pos
				double dT1Pos = m_pMoveData->dInspectStage3X[2];
				if(nIs1ScanNo == 3 && (dCurPos <= (dT2Pos + m_pEquipData->dInspectPitchX) && dCurPos > (dT1Pos + m_pEquipData->dInspectPitchX*3))){
					m_tInspect1Loop.Set_LoopTime(10000); return TRUE;
				}

				// 첫번째 스캔이후는 앞의 스테이지가 Unload 위치거나 Down 상태일때 진행가능.
				if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 4) || (dCurPos >= dT2Pos && dCurPos <= dUnPos) ||
					(!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd)) {
					if (Select_Top1ScanPos(AUTO_INSPECTION1, nIs1ScanNo)) {
						
						dIs1PosX = m_pMoveData->dInspectStage1X[2] + nIs1ScanNo * m_pEquipData->dInspectPitchX;
						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX);
						m_nInspect1Case = 8; m_tInspect1Loop.Set_LoopTime(10000);
						m_tInspect2Loop.Set_LoopTime(15000);//뒤에 따라오는 스테이지 알람 대기시간 초기화 

						if(nIs1ScanNo == 3 && m_nInspect2Case == 1 && g_objAJinAXL.Is_Done(AX_INSPECT_STAGE2_X)
							&& g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0) && g_objCommon.Get_InfoInspectVacOn(2)){ // Top1 마지막 피치 이동시 뒤에 있는 스테이지도 동시 어라인 진입 
							g_objCommon.Move_Position(AX_INSPECT_STAGE2_X, 1);
							m_nInspect2Case = 2; m_tInspect2Loop.Set_LoopTime(9000);
						}
						
						m_tInspect1Loop.Takt_Start(nTaktZone, 16);
						m_tInspect1Loop.Takt_End(nTaktZone,16);

					} else {
						nIs1ScanNo++;
					}
				} else { m_tInspect1Loop.Set_LoopTime(10000); return TRUE; }
			}
		}
		break;
	case 17:	// 안전확인, Move to Top2 Position
		if (g_objAJinAXL.Is_Done(AX_INSPECT_STAGE1_X)) {

			// 다운 상태, Load, Unload 위치일때 진행
			if ((!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd) || 
				g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 0) || g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 4)) {
				
				dIs1PosX = m_pMoveData->dInspectStage1X[3];
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX);
				nIs1ScanNo = 0;
								
				m_nInspect1Case = 25; m_tInspect1Loop.Set_LoopTime(10000);

				m_tInspect1Loop.Takt_Start(nTaktZone, 17);
				m_tInspect1Loop.Takt_End(nTaktZone,17);
			}
		}
		return TRUE;

	case 19:	// Send Load Complete
		if (g_objCommon.Get_InfoInspectVacOn(1) &&
			g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE1_X, dIs1PosX) &&
			g_objCommon.Check_Position(AX_VISION_TOP2_Z, 0) && g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 0))
		{
			
			g_objLogFile.VisionTakt_Start(TOP2);	// Top2 FPCB Scan Start
			if (m_pEquipData->bUseInspectTop2) {
				Set_InspectJigNo(2, nIs1ScanNo, nI1JNo1, nI1JNo2, nI1JNo3, nI1JNo4);
				g_objInspector.Set_LoadComplete(INSPECTOR_PC3, "T2", gLot.sLotID[gData.nPNoInspect[0]-1], gData.nPNoInspect[0], 
					gData.nTNoInspect[0][nI1JNo1-1], gData.nTNoInspect[0][nI1JNo2-1], 0, 0,
					gData.nCNoInspect[0][nI1JNo1-1], gData.nCNoInspect[0][nI1JNo2-1], 0, 0);

				m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(30000);
				m_tInspect1Loop.Takt_Start(nTaktZone, 19);
				m_tInspect1Loop.Takt_End(nTaktZone,19);
			} else {
				m_nInspect1Case = 25; m_tInspect1Loop.Set_LoopTime(10000); 
			}
			nIs1ScanNo++;
		}
		break;

	case 20:	// Wait for Top2 inspection(30sec)
		if (!m_pEquipData->bUseInspectTop2) {
			m_nInspect1Case = 25; m_tInspect1Loop.Set_LoopTime(10000);
		}
		break;

	case 21:	// Top2 Z Focus Move
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			m_strLog.Format("MCC,(07) Inspection1, T2 Z Focus Start");
			g_objLogFile.Save_MCCLog(m_strLog);
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, m_dTop2Z);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000);
			m_tInspect1Loop.Takt_Start(nTaktZone, 21);
			
		}
		break;
	case 22:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_TOP2_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dTop2Z, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_TOP2_Z, strLog);

			g_objInspector.Set_MoveComplete(INSPECTOR_PC3, "T2");
			m_nInspect1Case = 20; m_tInspect1Loop.Set_LoopTime(30000);	// 90초

			m_tInspect1Loop.Takt_End(nTaktZone,21);
			m_tInspect1Loop.Takt_Start(nTaktZone, 22);
			m_tInspect1Loop.Takt_End(nTaktZone,22);
		}
		break;

	case 23:	// Top2 Shift X, Y Move
		if (g_objAJinAXL.Is_Done(AX_INSPECT_STAGE1_X) && g_objAJinAXL.Is_Done(AX_TOP2_SHIFT_Y)) {
			//g_objLogFile.VisionTakt_Save(AUTO_INSPECTION1, 23, TOP2);	// Top2 FPCB Scan End
			m_strLog.Format("MCC,(07) Inspection1, Top2 Shift X, Y Move");
			g_objLogFile.Save_MCCLog(m_strLog);
			dIs1PosX = dIs1PosX + m_pEquipData->dInspectTop2ShiftX;
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, m_dTop2Z);
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX);
			g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 1);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000);
			m_tInspect1Loop.Takt_Start(nTaktZone, 23);
			
		}
		break;
	case 24:	// Send Move Complete
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE1_X, dIs1PosX) && g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 1) && g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			
			g_objLogFile.VisionTakt_Start(TOP2);	// Top2 Lens/Barrel/Sidefill Scan Start
			g_objInspector.Set_SMoveComplete(INSPECTOR_PC3, "T2");
			m_nInspect1Case = 20; m_tInspect1Loop.Set_LoopTime(30000);	// 90초
			m_tInspect1Loop.Takt_End(nTaktZone,23);
			m_tInspect1Loop.Takt_Start(nTaktZone, 24);
			m_tInspect1Loop.Takt_End(nTaktZone,24);
		}
		break;

	case 25:	// 다음 검사 위치 피치 이동
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE1_X, dIs1PosX) &&
			g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z) && g_objAJinAXL.Is_Done(AX_TOP2_SHIFT_Y)) {
			if (gData.bReload[3]) { nIs1ScanNo--; gData.bReload[3] = FALSE; }

			if (nIs1ScanNo > gData.nBtmPickQt-1) {	// 모델별로 피치이동 횟수가 다르다.
				
				nIs1ScanNo = 0;
				bTaktSave1 = FALSE;
				g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);	// Ready Up
				g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);	// Ready
				g_objCommon.Move_Position(AX_INSPECT_STAGE1_X, 4);	// Unload

				m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000);

				m_tInspect1Loop.Takt_Start(nTaktZone, 25);
				m_tInspect1Loop.Takt_End(nTaktZone,25);
			} else {	// 첫번째 스캔이후 앞의 스테이지는 다운상태 이거나 로드 위치에 있어야한다.
				if (nIs1ScanNo == 0 || (!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd) ||
					(g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 0) || g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 1, 5.0) || g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 5))) {
					
					if (Select_Top2ScanPos(AUTO_INSPECTION1, nIs1ScanNo)) {
						dIs1PosX = m_pMoveData->dInspectStage1X[3] + nIs1ScanNo * m_pEquipData->dInspectPitchX;

						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX);			
						g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);	// Ready Up
						g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);	// Ready
						m_nInspect1Case = 19; m_tInspect1Loop.Set_LoopTime(10000);

						m_tInspect1Loop.Takt_Start(nTaktZone, 25);
						m_tInspect1Loop.Takt_End(nTaktZone,25);

					} else {
						nIs1ScanNo++;
					}
				} else { m_tInspect1Loop.Set_LoopTime(10000); return TRUE; }
			}
		}
		break;
	case 26:	// Unload Position Check
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 4)) {
			m_tInspect1Loop.Takt_Start(nTaktZone, 26);
			m_tInspect1Loop.Takt_End(nTaktZone,26);

			m_nInspect1Case = 30; m_tInspect1Loop.Set_LoopTime(10000);
		}
		break;

	case 30:	// Wait for Unloading
		return TRUE;

	case 31:	// 안전확인, Stage Down
		if (!g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 4)) {	// Unload 위치에 없으면 다운
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE3_X);
			double dT2Pos = m_pMoveData->dInspectStage3X[3];	//T2 Pos
			if (dCurPos <= dT2Pos) {	//위치 확인
				
				m_pDY05->oInspectStage1Up = FALSE;	m_pDY05->oInspectStage1Down = TRUE;
				g_objAJinAXL.Write_Output(5);
				m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000);

				m_tInspect1Loop.Takt_Start(nTaktZone, 31);
				
			}
		}
		return TRUE;
	case 32:	// Stage Down Check
		if (!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down) {
			
			m_nInspect1Case = 50; m_tInspect1Loop.Set_LoopTime(10000);
			m_tInspect1Loop.Takt_End(nTaktZone,31);
			m_tInspect1Loop.Takt_Start(nTaktZone, 32);
			m_tInspect1Loop.Takt_End(nTaktZone,32);
		}
		break;

	case 50:	// Wait 회피 위치, Stage가 모두 Up 상태면 진행.
		if ((m_nInspect2Case >= 0 && m_nInspect2Case <= 31 && m_pDX05->iInspectStage2Up && !m_pDX05->iInspectStage2Down) &&
			(m_nInspect3Case >= 0 && m_nInspect3Case <= 31 && m_pDX05->iInspectStage3Up && !m_pDX05->iInspectStage3Down)) {
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000);
		}
		return TRUE;

	case 51:	// Move to Load Position
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 4)) {
			
			g_objCommon.Move_Position(AX_INSPECT_STAGE1_X, 0);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000);
			m_tInspect1Loop.Takt_Start(nTaktZone, 51);
			
		}
		break;
	case 52:	// Position Check
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0)) {
			
			m_nInspect1Case = 70; m_tInspect1Loop.Set_LoopTime(10000);

			m_tInspect1Loop.Takt_End(nTaktZone,51);
		}
		break;

	case 70:	// Wait 회피 위치, Top1 Position 부터 진행한다.
		if (m_nInspect3Case < 80 && m_nInspect3Case > 7 && !g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 0)) {
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE3_X);
			double dT1Pos = m_pMoveData->dInspectStage3X[2];	//T1 Pos
			if (dCurPos >= dT1Pos) {	//위치 확인
				m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;
	case 71:	// Stage Up
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0)) {
			
			m_pDY05->oInspectStage1Up = TRUE;	m_pDY05->oInspectStage1Down = FALSE;
			g_objAJinAXL.Write_Output(5);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000);

			m_tInspect1Loop.Takt_Start(nTaktZone, 71);
		}
		break;
	case 72:	// Stage Up Check
		if (m_pDX05->iInspectStage1Up && !m_pDX05->iInspectStage1Down) {
			if(m_nBtm1PickCase == 21 && !m_tInspect1Loop.Waiting_Time(200)) break;

			
			m_strLog.Format("Inspection Stage1, %d", GetTickCount() - m_dwInspect1);
			g_objLogFile.Save_TestLog(m_strLog);
			m_nInspect1Case = 0; m_tInspect1Loop.Set_LoopTime(10000);
			m_tInspect1Loop.Takt_End(nTaktZone,71);
		}
		break;

	case 80:	// Inspect X Axis Move to Align Offset
		if (TRUE/*g_objCommon.Get_InspectStage1VacOff(0)*/) {
			dIs1PosX = m_pMoveData->dInspectStage1X[1] + m_pEquipData->dInspectALOffset;
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX, 0.25);	// X방향 얼라인. 속도는 1/4
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000);
		}
		break;
	case 81:	// Move to Align Backward Offset
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE1_X, dIs1PosX)) {
			g_objCommon.Set_InfoInspectVacOn(1, 1);
			dIs1PosX = m_pMoveData->dInspectStage1X[1] + m_pEquipData->dInspectALOffset - m_pEquipData->dInspectALBackOffset;
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX, 0.25);	// X방향 얼라인. 속도는 1/4
			m_nInspect1Case = 4; m_tInspect1Loop.Set_LoopTime(5000);
		}
		break;

	case 85:	// Move to Module Align
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 5)) {
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE3_X);
			double dT2Pos = m_pMoveData->dInspectStage3X[3];	//T2 Pos
			double dUnPos = m_pMoveData->dInspectStage3X[4];	//Unload Pos
			if (dCurPos >= dT2Pos && dCurPos <= dUnPos) {	//위치 확인
				g_objCommon.Move_Position(AX_INSPECT_STAGE1_X, 1);
				m_nInspect1Case = 2; m_tInspect1Loop.Set_LoopTime(5000);
			} else {
				m_tInspect1Loop.Set_LoopTime(5000);
				return TRUE;
			}
		}
		break;
	}

	// 7. (Error : 3700)
	if (m_tInspect1Loop.Over_LoopTime()) {
		if (m_nInspect1Case == 10) {
			Set_ScanError(3710, gData.nTNoInspect[0][nI1JNo1-1], gData.nTNoInspect[0][nI1JNo2-1], 0, 0,
								gData.nCNoInspect[0][nI1JNo1-1], gData.nCNoInspect[0][nI1JNo2-1], 0, 0);
		} else if (m_nInspect1Case == 20) {
			Set_ScanError(3720, gData.nTNoInspect[0][nI1JNo1-1], gData.nTNoInspect[0][nI1JNo2-1], 0, 0,
								gData.nCNoInspect[0][nI1JNo1-1], gData.nCNoInspect[0][nI1JNo2-1], 0, 0);
		}
		g_objCommon.Show_Error(3700 + m_nInspect1Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 8. (Error : 3800)
BOOL CSequenceMain::Inspection2_Run()
{
	int nTaktZone = 8;		// Takt_Start, Takt_End
	static int		nIs2ScanNo = 0;
	static double	dIs2PosX = 0.0;
	static int		nI2JNo1 = 0, nI2JNo2 = 0, nI2JNo3 = 0, nI2JNo4 = 0;	// 검사할때 Jig 번호
	static int		nAlignCnt = 0;
	static BOOL		bTaktSave2 = FALSE;

	switch (m_nInspect2Case) {
	case 0:		// Wait for Module Loading
		m_tInspect2Loop.Set_LoopTime(15000);
		return TRUE;

	case 1:		// 안전확인 & Move to Align Position
		{
			BOOL bInspStart = FALSE;
			BOOL bInspWaitStart = FALSE;
			if(!m_tInspect2Loop.Waiting_Time(5)) return TRUE;
			// 모두 다운
			if (!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down &&
				!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd) { bInspStart = TRUE; }

			// R04A/B T1 마지막 검사 ~ Unloading, Inspect2번이 언로드 위치면 Inspect3번이 Top2 첫번째 검사 시작위치 진입. R04C T2 ~ Unloading
			if ((m_nInspect1Case > 7 && m_nInspect1Case <= 31)|| bInspStart) {

				double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE1_X);
				double dT1_4thPos = m_pMoveData->dInspectStage1X[2] + (m_pEquipData->dInspectPitchX *3);
				double dUnPos = m_pMoveData->dInspectStage1X[4];	//Unload Pos
				if ((dCurPos >= dT1_4thPos && dCurPos <= dUnPos)|| bInspStart) {	//위치 확인
					if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0) && g_objCommon.Get_InfoInspectVacOn(2) &&
						m_pDX05->iInspectStage2Up && !m_pDX05->iInspectStage2Down && m_pDX05->iInspectStage2Fwd && !m_pDX05->iInspectStage2Bwd && g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0)) {
						m_dwInspect2 = GetTickCount();
					
						m_tInspect2Loop.Takt_Start(nTaktZone, 1, TRUE);
						m_tInspect2Loop.Takt_End(nTaktZone,1);

						g_objCommon.Move_Position(AX_INSPECT_STAGE2_X, 1);
						m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(9000);
					} else {
						break;
					}
				} 
			}

		}
		return TRUE;

	case 2:		// Align Z Axis Down
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 1) && 
			g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0) &&
			g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0) &&
			m_pDX06->iCmAlignMasterOpen && !m_pDX06->iCmAlignMasterClose &&
			m_pDX06->iCmAlignSlaveOpen  && !m_pDX06->iCmAlignSlaveClose) {
			
			g_objCommon.Move_Position(AX_MODULE_ALIGN_Z, 1);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000); 
			m_tInspect2Loop.Takt_Start(nTaktZone, 2);

		}
		break;
	case 3:		// CM Align Master/
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 1)) {
			
			m_pDY06->oCmAlignMasterOpen = FALSE; m_pDY06->oCmAlignMasterClose = TRUE;
			g_objAJinAXL.Write_Output(6);
			m_nInspect2Case = 9; m_tInspect2Loop.Set_LoopTime(5000);

			m_tInspect2Loop.Takt_End(nTaktZone,2);
			m_tInspect2Loop.Takt_Start(nTaktZone, 3);
		}
		break;
	case 9: //Slave Close
		if(!m_pDX06->iCmAlignMasterOpen && m_pDX06->iCmAlignMasterClose){
			if (!bTaktSave2) {
				bTaktSave2 = TRUE; 
				m_tInspect2Loop.Takt_End(nTaktZone,3); 
				m_tInspect2Loop.Takt_Start(nTaktZone,9); 
			}

			//g_objCommon.Set_InspectVacOff(2, 0);	// Vac Off
			//if(!m_tInspect2Loop.Waiting_Time(400)) break;

			m_pDY06->oCmAlignSlaveOpen = FALSE;  m_pDY06->oCmAlignSlaveClose = TRUE;
			g_objAJinAXL.Write_Output(6);
			m_nInspect2Case = 4; m_tInspect2Loop.Set_LoopTime(10000);
			bTaktSave2 = FALSE;
		}
		break;
	case 4:		// CM Align Slave Open
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 1) && g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 1) &&
			!m_pDX06->iCmAlignMasterOpen && m_pDX06->iCmAlignMasterClose &&
			!m_pDX06->iCmAlignSlaveOpen  && m_pDX06->iCmAlignSlaveClose) {
				if (!bTaktSave2) {
					bTaktSave2 = TRUE; 
					m_tInspect2Loop.Takt_End(nTaktZone,9); 
					m_tInspect2Loop.Takt_Start(nTaktZone,4); 
				}

			g_objCommon.Set_InfoInspectVacOn(2, 1);	// Vac On

			// io WRITE를 동시에 하면 씹힐수 있어서 딜레이 추가
			if (!m_tInspect2Loop.Waiting_Time(10)) return TRUE;

			m_pDY06->oCmAlignSlaveOpen = TRUE;  m_pDY06->oCmAlignSlaveClose = FALSE;
			m_pDY06->oCmAlignMasterOpen = TRUE;  m_pDY06->oCmAlignMasterClose = FALSE;
			g_objAJinAXL.Write_Output(6);
			m_nInspect2Case = 6; m_tInspect2Loop.Set_LoopTime(5000); 
			bTaktSave2 = FALSE;
		}
		break;
	case 6:		// Align Z Axis Move to Ready Up Position
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0) && g_objCommon.Get_InfoInspectVacOn(2) &&
			m_pDX06->iCmAlignMasterOpen && !m_pDX06->iCmAlignMasterClose &&
			m_pDX06->iCmAlignSlaveOpen  && !m_pDX06->iCmAlignSlaveClose) {
				if (!bTaktSave2) {
					bTaktSave2 = TRUE;
					m_tInspect2Loop.Takt_End(nTaktZone,5);
					m_tInspect2Loop.Takt_Start(nTaktZone, 6);
				}
			//if(!m_tInspect2Loop.Waiting_Time(100)) break;

			g_objCommon.Move_Position(AX_MODULE_ALIGN_Z, 0);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000); 
			bTaktSave2 = FALSE;
		}
		break;
	case 7:		// 안전확인 & Move to Top1 Position	// Mirror 실린더 확인
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0) && g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0) && g_objCommon.Get_InfoInspectVacOn(2)) {
			
			if (1) {	// Mirror 확인 안한다.
				// 다운 상태이거나 Top2, Unload 위치에 있으면 이동
				if (!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down) {
				
					dIs2PosX = m_pMoveData->dInspectStage2X[2];
					g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX);
					nIs2ScanNo = 0;
					
					m_nInspect2Case = 16; m_tInspect2Loop.Set_LoopTime(10000);

					m_tInspect2Loop.Takt_End(nTaktZone,6);
					m_tInspect2Loop.Takt_Start(nTaktZone, 7);
					m_tInspect2Loop.Takt_End(nTaktZone,7);

				} else if (m_nInspect1Case > 17 && m_nInspect1Case < 50) {	// Top2 ~ Unload 위치에 있을때
					double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE1_X);
					double dT2Pos = m_pMoveData->dInspectStage1X[2] + m_pEquipData->dInspectPitchX * 4 + 1;	// R04A/B T1 Pos + Inspection Stage Pitch * 4 + 1
					double dUnPos = m_pMoveData->dInspectStage1X[4];	//Unload Pos
					if (dCurPos > dT2Pos && dCurPos <= dUnPos) {	//위치 확인
						
						dIs2PosX = m_pMoveData->dInspectStage2X[2];
						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX);
						nIs2ScanNo = 0;
						m_nInspect2Case = 16; m_tInspect2Loop.Set_LoopTime(10000);

						m_tInspect2Loop.Takt_End(nTaktZone,6);
						m_tInspect2Loop.Takt_Start(nTaktZone, 7);
						m_tInspect2Loop.Takt_End(nTaktZone,7);
					} else { m_tInspect2Loop.Set_LoopTime(10000); return TRUE; }

				} else { m_tInspect2Loop.Set_LoopTime(10000); return TRUE; }
			}
		}
		break;
	case 8:		// Send Load Complete
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE2_X, dIs2PosX) && g_objCommon.Check_Position(AX_VISION_TOP1_Z, 0) && g_objCommon.Get_InfoInspectVacOn(2)) {
			
			g_objLogFile.VisionTakt_Start(TOP1);	// Top1 Scan Start
			if (m_pEquipData->bUseInspectTop1) {
				if (gData.bTop1MirrorErr[1]) {
					nIs2ScanNo--; gData.bTop1MirrorErr[1] = FALSE; 
					g_objCommon.Show_Error(gData.nTop1MirrorErrNo);
					return FALSE;
				}
				Set_InspectJigNo(1, nIs2ScanNo, nI2JNo1, nI2JNo2, nI2JNo3, nI2JNo4);
				g_objInspector.Set_LoadComplete(INSPECTOR_PC2, "T1", gLot.sLotID[gData.nPNoInspect[1]-1], gData.nPNoInspect[1],
					gData.nTNoInspect[1][nI2JNo1-1], gData.nTNoInspect[1][nI2JNo2-1], 0, 0,
					gData.nCNoInspect[1][nI2JNo1-1], gData.nCNoInspect[1][nI2JNo2-1], 0, 0);

				m_nInspect2Case = 10; m_tInspect2Loop.Set_LoopTime(90000);
				m_tInspect2Loop.Takt_Start(nTaktZone, 8);
				m_tInspect2Loop.Takt_End(nTaktZone,8);
			} else {
				m_nInspect2Case = 15; m_tInspect2Loop.Set_LoopTime(5000);
			}
			nIs2ScanNo++;
		}
		break;

	case 10:	// Wait for Top1 inspection(30sec)
		if (!m_pEquipData->bUseInspectTop1) {
			m_nInspect2Case = 15; m_tInspect2Loop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Top1 Z Focus Move
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			m_strLog.Format("MCC,(07) Inspection2, T1 Z Focus Move Start");
			g_objLogFile.Save_MCCLog(m_strLog);
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, m_dTop1Z);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000);
			m_tInspect2Loop.Takt_Start(nTaktZone, 11);
		}
		break;
	case 12:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_TOP1_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dTop1Z, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_TOP1_Z, strLog);

			g_objInspector.Set_MoveComplete(INSPECTOR_PC2, "T1");
			m_nInspect2Case = 10; m_tInspect2Loop.Set_LoopTime(90000);	// 90초
			m_tInspect2Loop.Takt_End(nTaktZone,11);
			m_tInspect2Loop.Takt_Start(nTaktZone,12);
			m_tInspect2Loop.Takt_End(nTaktZone,12);
		}
		break;

	case 13:	// Top1 Mirror Z Move
		if (g_objAJinAXL.Is_Done(AX_TOP1_MIRROR_Z) && g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			//g_objLogFile.VisionTakt_Save(AUTO_INSPECTION2, 21, TOP1);	// Top1 Module Position Inspect Complete
			m_strLog.Format("MCC,(07) Inspection2, T1 Mirror Z Move Start");
			g_objLogFile.Save_MCCLog(m_strLog);
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, m_dTop1Z);
			g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 1);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000);
			m_tInspect2Loop.Takt_Start(nTaktZone,13);
		}
		break;
	case 14:	// Send Move Complete
		if (g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 1) && g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			
			g_objLogFile.VisionTakt_Start(TOP1);	// Top1 Mirror Down 이후 Scan
			g_objInspector.Set_SMoveComplete(INSPECTOR_PC2, "T1");
			m_nInspect2Case = 10; m_tInspect2Loop.Set_LoopTime(90000);	// 90초

			m_tInspect2Loop.Takt_End(nTaktZone,13);
			m_tInspect2Loop.Takt_Start(nTaktZone,14);
			m_tInspect2Loop.Takt_End(nTaktZone,14);
		}
		break;

	case 15:	// Mirror Z축 안전 위치로 회피
		//if (!m_pEquipData->bUseInspectTop1) { if(!m_tInspect2Loop.Waiting_Time(700)) break; }	// Delay

		g_objCommon.Move_Position(AX_VISION_TOP1_Z, 0);	// Ready Up
		g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 0);	// Ready Up
		m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(180000);
		m_tInspect2Loop.Takt_Start(nTaktZone, 15);
		m_tInspect2Loop.Takt_End(nTaktZone,15);
		break;
	case 16:	// 다음 검사 위치 피치 이동
		if (g_objAJinAXL.Is_Done(AX_INSPECT_STAGE2_X) && g_objCommon.Check_Position(AX_VISION_TOP1_Z, 0) && g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0)) {
			if (!bTaktSave2) { bTaktSave2 = TRUE;  }	// Move to Top1 Position

			if (gData.bReload[2]) {
				nIs2ScanNo--; gData.bReload[2] = FALSE;
			}
			if (nIs2ScanNo > gData.nScanCnt) {	// 모델별로 피치이동 횟수가 다르다.
				nIs2ScanNo = 0;
				bTaktSave2 = FALSE;
				m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000);

				m_tInspect2Loop.Takt_Start(nTaktZone, 16);
				m_tInspect2Loop.Takt_End(nTaktZone,16);

			} else {
				double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE1_X);
				double dT2Pos = m_pMoveData->dInspectStage1X[3];// + m_pEquipData->dInspectPitchX * 2;
				double dUnPos = m_pMoveData->dInspectStage1X[4];	//Unload Pos
				double dT1Pos = m_pMoveData->dInspectStage1X[2];
				if(nIs2ScanNo == 3 && (dCurPos <= (dT2Pos + m_pEquipData->dInspectPitchX) && dCurPos > (dT1Pos + m_pEquipData->dInspectPitchX*3))){
					m_tInspect2Loop.Set_LoopTime(10000); return TRUE;
				}
				// 첫번째 스캔이후는 앞의 스테이지가 Unload 위치거나 Down 상태일때 진행가능.
				if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 4) || (dCurPos >= dT2Pos && dCurPos <= dUnPos) ||
					(!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down)) {
					if (Select_Top1ScanPos(AUTO_INSPECTION2, nIs2ScanNo)) {
						
						dIs2PosX = m_pMoveData->dInspectStage2X[2] + nIs2ScanNo * m_pEquipData->dInspectPitchX;
						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX);
						m_nInspect2Case = 8; m_tInspect2Loop.Set_LoopTime(10000);
						m_tInspect3Loop.Set_LoopTime(15000);//뒤에 따라오는 스테이지 알람 대기시간 초기화 

						if(nIs2ScanNo == 3 && m_nInspect3Case == 1 && g_objAJinAXL.Is_Done(AX_INSPECT_STAGE3_X)
							&& g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0) && g_objCommon.Get_InfoInspectVacOn(3)){ // Top1 마지막 피치 이동시 뒤에 있는 스테이지도 동시 어라인 진입 
							g_objCommon.Move_Position(AX_INSPECT_STAGE3_X, 1);
							m_nInspect3Case = 2; m_tInspect3Loop.Set_LoopTime(9000);
						}

						m_tInspect2Loop.Takt_Start(nTaktZone, 16);
						m_tInspect2Loop.Takt_End(nTaktZone,16);
					} else {
						nIs2ScanNo++;
					}
				} else { m_tInspect2Loop.Set_LoopTime(10000); return TRUE; }
			}
		}
		break;
	case 17:	// 안전확인, Move to Top2 Position
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE2_X, dIs2PosX)) {
			
			// 다운 상태, Load, Unload 위치일때 진행
			if ((!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down) ||
				g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0) || g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 4)) {
				
				dIs2PosX = m_pMoveData->dInspectStage2X[3];
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX);
				nIs2ScanNo = 0;
						
				m_nInspect2Case = 25; m_tInspect2Loop.Set_LoopTime(10000); 

				m_tInspect2Loop.Takt_Start(nTaktZone, 17);
				m_tInspect2Loop.Takt_End(nTaktZone,17);
			}
		}
		return TRUE;

	case 19:	// Send Load Complete
		if (g_objCommon.Get_InfoInspectVacOn(2) &&
			g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE2_X, dIs2PosX) &&
			g_objCommon.Check_Position(AX_VISION_TOP2_Z, 0) && g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 0))
		{
			
			g_objLogFile.VisionTakt_Start(TOP2);	// Top2 FPCB Scan Start
			if (m_pEquipData->bUseInspectTop2) {
				Set_InspectJigNo(2, nIs2ScanNo, nI2JNo1, nI2JNo2, nI2JNo3, nI2JNo4);
				g_objInspector.Set_LoadComplete(INSPECTOR_PC3, "T2", gLot.sLotID[gData.nPNoInspect[1]-1], gData.nPNoInspect[1], 
					gData.nTNoInspect[1][nI2JNo1-1], gData.nTNoInspect[1][nI2JNo2-1], 0, 0,
					gData.nCNoInspect[1][nI2JNo1-1], gData.nCNoInspect[1][nI2JNo2-1], 0, 0);

				m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(180000); 
				m_tInspect2Loop.Takt_Start(nTaktZone, 19);
				m_tInspect2Loop.Takt_End(nTaktZone,19);
			} else {
				m_nInspect2Case = 25; m_tInspect2Loop.Set_LoopTime(10000); 
			}
			nIs2ScanNo++;
		}
		break;

	case 20:	// Wait for Top2 inspection(30sec)
		if (!m_pEquipData->bUseInspectTop2) {
			m_nInspect2Case = 25; m_tInspect2Loop.Set_LoopTime(10000);
		}
		break;

	case 21:	// Top2 Z Focus Move
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			m_strLog.Format("MCC,(07) Inspection2, T2 Z Focus Start");
			g_objLogFile.Save_MCCLog(m_strLog);
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, m_dTop2Z);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000);
			m_tInspect2Loop.Takt_Start(nTaktZone, 21);
		}
		break;
	case 22:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_TOP2_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dTop2Z, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_TOP2_Z, strLog);

			g_objInspector.Set_MoveComplete(INSPECTOR_PC3, "T2");
			m_nInspect2Case = 20; m_tInspect2Loop.Set_LoopTime(90000);	// 90초
			m_tInspect2Loop.Takt_End(nTaktZone,21);
			m_tInspect2Loop.Takt_Start(nTaktZone, 22);
			m_tInspect2Loop.Takt_End(nTaktZone,22);
		}
		break;

	case 23:	// Top2 Shift X, Y Move
		if (g_objAJinAXL.Is_Done(AX_INSPECT_STAGE2_X) && g_objAJinAXL.Is_Done(AX_TOP2_SHIFT_Y) && g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			//g_objLogFile.VisionTakt_Save(AUTO_INSPECTION2, 23, TOP2);	// Top2 FPCB Scan End
			m_strLog.Format("MCC,(07) Inspection2, Top2 Shift X, Y Move");
			g_objLogFile.Save_MCCLog(m_strLog);

			dIs2PosX = dIs2PosX + m_pEquipData->dInspectTop2ShiftX;

			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, m_dTop2Z);
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX);
			g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 1);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000);
			m_tInspect2Loop.Takt_Start(nTaktZone, 23);
		}
		break;
	case 24:	// Send Move Complete
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE2_X, dIs2PosX) && g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 1) && g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			
			g_objLogFile.VisionTakt_Start(TOP2);	// Top2 Lens/Barrel/Sidefill Scan Start
			g_objInspector.Set_SMoveComplete(INSPECTOR_PC3, "T2");
			m_nInspect2Case = 20; m_tInspect2Loop.Set_LoopTime(90000);	// 90초
			m_tInspect2Loop.Takt_End(nTaktZone,23);
			m_tInspect2Loop.Takt_Start(nTaktZone, 24);
			m_tInspect2Loop.Takt_End(nTaktZone,24);
		}
		break;

	case 25:	// 다음 검사 위치 피치 이동
		//if (!m_pEquipData->bUseInspectTop2) { if(!m_tInspect2Loop.Waiting_Time(500)) break; }	// Delay

		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE2_X, dIs2PosX) &&
			g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z) && g_objAJinAXL.Is_Done(AX_TOP2_SHIFT_Y)) {
			if (gData.bReload[3]) { nIs2ScanNo--; gData.bReload[3] = FALSE; }

			if (nIs2ScanNo > gData.nBtmPickQt-1) {	// 모델별로 피치이동 횟수가 다르다.
				
				nIs2ScanNo = 0;
				bTaktSave2 = FALSE;
				g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);	// Ready Up
				g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);	// Ready

				g_objCommon.Move_Position(AX_INSPECT_STAGE2_X, 4);	// Unload

				m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000);

				m_tInspect2Loop.Takt_Start(nTaktZone, 25);
				m_tInspect2Loop.Takt_End(nTaktZone,25);
			} else {	// 첫번째 스캔이후 앞의 스테이지는 다운상태 이거나 로드 위치에 있어야한다.
				if (nIs2ScanNo == 0 || (!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down) ||
					(g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0) || g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 1, 5.0) || g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 5))) {
					
					if (Select_Top2ScanPos(AUTO_INSPECTION2, nIs2ScanNo)) {
						dIs2PosX = m_pMoveData->dInspectStage2X[3] + nIs2ScanNo * m_pEquipData->dInspectPitchX;

						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX);			
						g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);	// Ready Up
						g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);	// Ready
						m_nInspect2Case = 19; m_tInspect2Loop.Set_LoopTime(10000);

						m_tInspect2Loop.Takt_Start(nTaktZone, 25);
						m_tInspect2Loop.Takt_End(nTaktZone,25);
					} else {
						nIs2ScanNo++;
					}
				} else { m_tInspect2Loop.Set_LoopTime(10000); return TRUE; }
			}
		}
		break;
	case 26:	// Unload Position Check
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 4)) {
			m_tInspect2Loop.Takt_Start(nTaktZone, 26);
			m_tInspect2Loop.Takt_End(nTaktZone,26);

			m_nInspect2Case = 30; m_tInspect2Loop.Set_LoopTime(10000);
		}
		break;

	case 30:	// Wait for Unloading
		return TRUE;

	case 31:	// 안전확인, Stage Down & Backward
		if (!g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 4)) {	// Unload 위치에 없으면 다운
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE1_X);
			double dT2Pos = m_pMoveData->dInspectStage1X[3];	//T2 Pos
			if (dCurPos <= dT2Pos) {	//위치 확인
				
				m_pDY05->oInspectStage2Up = FALSE;	m_pDY05->oInspectStage2Down = TRUE;
				m_pDY05->oInspectStage2Fwd = FALSE;	m_pDY05->oInspectStage2Bwd = TRUE;
				g_objAJinAXL.Write_Output(5);
				m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000);
				m_tInspect2Loop.Takt_Start(nTaktZone, 31);
			}
		}
		return TRUE;
	case 32:	// Stage Down, Backward Check
		if (!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down && !m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd) {
			
			m_nInspect2Case = 50; m_tInspect2Loop.Set_LoopTime(10000);
			m_tInspect2Loop.Takt_End(nTaktZone,31);
			m_tInspect2Loop.Takt_Start(nTaktZone, 32);
			m_tInspect2Loop.Takt_End(nTaktZone,32);
		}
		break;

	case 50:	// Wait 회피 위치, Stage가 모두 Up 상태면 진행.
		if ((m_nInspect1Case >= 0 && m_nInspect1Case <= 31 && m_pDX05->iInspectStage1Up && !m_pDX05->iInspectStage1Down) &&
			(m_nInspect3Case >= 0 && m_nInspect3Case <= 31 && m_pDX05->iInspectStage3Up && !m_pDX05->iInspectStage3Down)) {
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000);
		}
		return TRUE;

	case 51:	// Move to Load Position
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 4)) {
			
			g_objCommon.Move_Position(AX_INSPECT_STAGE2_X, 0);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000);
			m_tInspect2Loop.Takt_Start(nTaktZone, 51);
		}
		break;
	case 52:	// Position Check
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0)) {
			
			m_nInspect2Case = 70; m_tInspect2Loop.Set_LoopTime(10000);
			m_tInspect2Loop.Takt_End(nTaktZone,51);
		}
		break;

	case 70:	// Wait 회피 위치, Top1 Position 부터 진행한다.
		if (m_nInspect1Case < 80 && m_nInspect1Case > 7 && !g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0)) {
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE1_X);
			double dT1Pos = m_pMoveData->dInspectStage1X[2];	//T1 Pos
			if (dCurPos >= dT1Pos) {	//위치 확인
				m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;
	case 71:	// Stage Up
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0)) {
			
			m_pDY05->oInspectStage2Up = TRUE;	m_pDY05->oInspectStage2Down = FALSE;
			m_pDY05->oInspectStage2Fwd = TRUE;	m_pDY05->oInspectStage2Bwd = FALSE;
			g_objAJinAXL.Write_Output(5);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000);
			m_tInspect2Loop.Takt_Start(nTaktZone, 71);
		}
		break;
	case 72:	// Stage Up Check
		if (m_pDX05->iInspectStage2Up && !m_pDX05->iInspectStage2Down && m_pDX05->iInspectStage2Fwd && !m_pDX05->iInspectStage2Bwd) {
			if(m_nBtm1PickCase == 21 && !m_tInspect2Loop.Waiting_Time(200)) break;

			m_strLog.Format("Inspection Stage2, %d", GetTickCount() - m_dwInspect2);
			g_objLogFile.Save_TestLog(m_strLog);
			m_nInspect2Case = 0; m_tInspect2Loop.Set_LoopTime(10000);
			m_tInspect2Loop.Takt_End(nTaktZone,71);
		}
		break;

	case 80:	// Inspect X Axis Move to Align Offset
		if (TRUE/*g_objCommon.Get_InspectStage2VacOff(0)*/) {
			dIs2PosX = m_pMoveData->dInspectStage2X[1] + m_pEquipData->dInspectALOffset;
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX, 0.25);	// X방향 얼라인. 속도는 1/4
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000); 
		}
		break;
	case 81:	// Move to Backward Offset
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE2_X, dIs2PosX)) {
			g_objCommon.Set_InfoInspectVacOn(2, 1);
			dIs2PosX = m_pMoveData->dInspectStage2X[1] + m_pEquipData->dInspectALOffset - m_pEquipData->dInspectALBackOffset;
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX, 0.25);	// X방향 얼라인. 속도는 1/4
			m_nInspect2Case = 4; m_tInspect2Loop.Set_LoopTime(5000); 
		}
		break;

	case 85:	// Move to Module Align
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 5)) {
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE1_X);
			double dT2Pos = m_pMoveData->dInspectStage1X[3];	//T2 Pos
			double dUnPos = m_pMoveData->dInspectStage1X[4];	//Unload Pos
			if (dCurPos >= dT2Pos && dCurPos <= dUnPos) {	//위치 확인
				g_objCommon.Move_Position(AX_INSPECT_STAGE2_X, 1);
				m_nInspect2Case = 2; m_tInspect2Loop.Set_LoopTime(5000);
			} else {
				m_tInspect2Loop.Set_LoopTime(5000);
				return TRUE;
			}
		}
		break;
	}

	// 8. (Error : 3800)
	if (m_tInspect2Loop.Over_LoopTime()) {
		if (m_nInspect2Case == 10) {
			Set_ScanError(3810, gData.nTNoInspect[1][nI2JNo1-1], gData.nTNoInspect[1][nI2JNo2-1], 0, 0,
								gData.nCNoInspect[1][nI2JNo1-1], gData.nCNoInspect[1][nI2JNo2-1], 0, 0);
		} else if (m_nInspect2Case == 20) {
			Set_ScanError(3820, gData.nTNoInspect[1][nI2JNo1-1], gData.nTNoInspect[1][nI2JNo2-1], 0, 0,
								gData.nCNoInspect[1][nI2JNo1-1], gData.nCNoInspect[1][nI2JNo2-1], 0, 0);
		}
		g_objCommon.Show_Error(3800 + m_nInspect2Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 9. (Error : 3900)
BOOL CSequenceMain::Inspection3_Run()
{
	int nTaktZone = 9;		// Takt_Start, Takt_End
	static int		nIs3ScanNo = 0;
	static double	dIs3PosX = 0.0;
	static int		nI3JNo1 = 0, nI3JNo2 = 0, nI3JNo3 = 0, nI3JNo4 = 0;	// 검사할때 Jig 번호
	static int		nAlignCnt = 0;
	static BOOL		bTaktSave3 = FALSE;

	switch (m_nInspect3Case) {
	case 0:		// Wait for Module Loading
		m_tInspect3Loop.Set_LoopTime(15000);
		return TRUE;

	case 1:		// 안전확인 & Move to Align Position
		{
			BOOL bInspStart = FALSE;
			BOOL bInspWaitStart = FALSE;
			if(!m_tInspect3Loop.Waiting_Time(5)) return TRUE;
			// 모두 다운
			if (!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down &&
				!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down && !m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd) { bInspStart = TRUE; }

			// R04A/B T1 마지막 검사 ~ Unloading, Inspect2번이 언로드 위치면 Inspect3번이 Top2 첫번째 검사 시작위치 진입. R04C T2 ~ Unloading
			if ((m_nInspect2Case > 7 && m_nInspect2Case <= 31) || bInspStart) {

				double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE2_X);
				double dT1_4thPos = m_pMoveData->dInspectStage2X[2] + (m_pEquipData->dInspectPitchX *3);
				double dUnPos = m_pMoveData->dInspectStage2X[4];	//Unload Pos
				if ((dCurPos >= dT1_4thPos && dCurPos <= dUnPos) || bInspStart) {	//위치 확인
					// Interlock Alarm
					if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 0) && g_objCommon.Get_InfoInspectVacOn(3) &&
						m_pDX05->iInspectStage3Up && !m_pDX05->iInspectStage3Down && m_pDX05->iInspectStage3Fwd && !m_pDX05->iInspectStage3Bwd && g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0)) {
						m_dwInspect3 = GetTickCount();
						m_tInspect3Loop.Takt_Start(nTaktZone, 1, TRUE);
						m_tInspect3Loop.Takt_End(nTaktZone,1);
						g_objCommon.Move_Position(AX_INSPECT_STAGE3_X, 1);
						m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(9000);
					} else {
						break;
					
					}
				} 
			}
		}
		return TRUE;

	case 2:		// Align Z Axis Down
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 1) && 
			g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0) &&
			g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0) &&
			m_pDX06->iCmAlignMasterOpen && !m_pDX06->iCmAlignMasterClose &&
			m_pDX06->iCmAlignSlaveOpen  && !m_pDX06->iCmAlignSlaveClose) {
		
			g_objCommon.Move_Position(AX_MODULE_ALIGN_Z, 1);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000); 
			m_tInspect3Loop.Takt_Start(nTaktZone, 2);
		}
		break;
	case 3:		// CM Align Master
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 1)) {
			
			m_pDY06->oCmAlignMasterOpen = FALSE; m_pDY06->oCmAlignMasterClose = TRUE;
			g_objAJinAXL.Write_Output(6);
			m_nInspect3Case = 9; m_tInspect3Loop.Set_LoopTime(5000); 

			m_tInspect3Loop.Takt_End(nTaktZone,2);
			m_tInspect3Loop.Takt_Start(nTaktZone, 3);
		}
		break;
	case 9: //Slave Close
		if(!m_pDX06->iCmAlignMasterOpen && m_pDX06->iCmAlignMasterClose){
			if (!bTaktSave3) {
				bTaktSave3 = TRUE; 
				m_tInspect3Loop.Takt_End(nTaktZone,3); 
				m_tInspect3Loop.Takt_Start(nTaktZone,9); 
			}
			//g_objCommon.Set_InspectVacOff(3, 0);	// Vac Off
			//if(!m_tInspect3Loop.Waiting_Time(400)) break;

			m_pDY06->oCmAlignSlaveOpen = FALSE;  m_pDY06->oCmAlignSlaveClose = TRUE;
			g_objAJinAXL.Write_Output(6);
			m_nInspect3Case = 4; m_tInspect3Loop.Set_LoopTime(5000);
			bTaktSave3 = FALSE;
		}
		break;
	case 4:		// CM Align Slave Open
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 1) && g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 1) &&
			!m_pDX06->iCmAlignMasterOpen && m_pDX06->iCmAlignMasterClose &&
			!m_pDX06->iCmAlignSlaveOpen  && m_pDX06->iCmAlignSlaveClose) {
				if (!bTaktSave3) {
					bTaktSave3 = TRUE; 
					m_tInspect3Loop.Takt_End(nTaktZone,9); 
					m_tInspect3Loop.Takt_Start(nTaktZone,4); 
				}
			g_objCommon.Set_InfoInspectVacOn(3, 1);	// Vac On

			// io WRITE를 동시에 하면 씹힐수 있어서 딜레이 추가
			if (!m_tInspect3Loop.Waiting_Time(10)) return TRUE;

			m_pDY06->oCmAlignSlaveOpen = TRUE;  m_pDY06->oCmAlignSlaveClose = FALSE;
			m_pDY06->oCmAlignMasterOpen = TRUE;  m_pDY06->oCmAlignMasterClose = FALSE;
			g_objAJinAXL.Write_Output(6);
			m_nInspect3Case = 6; m_tInspect3Loop.Set_LoopTime(5000); 
			bTaktSave3 = FALSE;
		}
		break;
	case 6:		// Align Z Axis Move to Ready Up Position
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0) &&
			m_pDX06->iCmAlignMasterOpen && !m_pDX06->iCmAlignMasterClose &&
			m_pDX06->iCmAlignSlaveOpen  && !m_pDX06->iCmAlignSlaveClose) {
				if (!bTaktSave3) {
					bTaktSave3 = TRUE;
					m_tInspect3Loop.Takt_End(nTaktZone,5);
					m_tInspect3Loop.Takt_Start(nTaktZone, 6);
				}

			//if(!m_tInspect3Loop.Waiting_Time(100)) break;

			g_objCommon.Move_Position(AX_MODULE_ALIGN_Z, 0);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000); 
			bTaktSave3 = FALSE;
		}
		break;
	case 7:		// 안전확인 & Move to Top1 Position	// Mirror 실린더 확인
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0) && g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0) && g_objCommon.Get_InfoInspectVacOn(3)) {
			if (!bTaktSave3) { bTaktSave3 = TRUE;  }
			if (1) {	// Mirror 확인 안한다.
				// 다운 상태이거나 Unload 위치에 있으면 이동
				if (!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down && !m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd) {	// 다운 상태
					
					dIs3PosX = m_pMoveData->dInspectStage3X[2];
					g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX);
					nIs3ScanNo = 0;
					bTaktSave3 = FALSE;
					//MCC
					m_strLog.Format("Inspect stage 3 Top1 Scan Start");
					g_objLogFile.Save_MCCLog(m_strLog);
					m_nInspect3Case = 16; m_tInspect3Loop.Set_LoopTime(10000);
					m_tInspect3Loop.Takt_End(nTaktZone,6);
					m_tInspect3Loop.Takt_Start(nTaktZone, 7);
					m_tInspect3Loop.Takt_End(nTaktZone,7);

				} else if (m_nInspect2Case > 17 && m_nInspect2Case < 50) {	// Top2 ~ Unload 위치에 있을때
					double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE2_X);
					double dT2Pos = m_pMoveData->dInspectStage2X[2] + m_pEquipData->dInspectPitchX * 4 + 1;	// R04A/B T1 Pos + Inspection Stage Pitch * 4 + 1
					double dUnPos = m_pMoveData->dInspectStage2X[4];	//Unload Pos
					if (dCurPos > dT2Pos && dCurPos <= dUnPos) {	//위치 확인
						
						dIs3PosX = m_pMoveData->dInspectStage3X[2];
						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX);
						nIs3ScanNo = 0;
						bTaktSave3 = FALSE;
						//MCC
						m_strLog.Format("Inspect stage 3 Top1 Scan Start");
						g_objLogFile.Save_MCCLog(m_strLog);
						m_nInspect3Case = 16; m_tInspect3Loop.Set_LoopTime(10000);
						m_tInspect3Loop.Takt_End(nTaktZone,6);
						m_tInspect3Loop.Takt_Start(nTaktZone, 7);
						m_tInspect3Loop.Takt_End(nTaktZone,7);
					} else { m_tInspect3Loop.Set_LoopTime(10000); return TRUE; }

				} else { m_tInspect3Loop.Set_LoopTime(10000); return TRUE; }
			}
		}
		break;
	case 8:		// Send Load Complete
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE3_X, dIs3PosX) && g_objCommon.Check_Position(AX_VISION_TOP1_Z, 0) && g_objCommon.Get_InfoInspectVacOn(3)) {
			
			g_objLogFile.VisionTakt_Start(TOP1);	// Top1 Scan Start
			if (m_pEquipData->bUseInspectTop1) {
				if (gData.bTop1MirrorErr[2]) {
					nIs3ScanNo--; gData.bTop1MirrorErr[2] = FALSE; 
					g_objCommon.Show_Error(gData.nTop1MirrorErrNo);
					return FALSE;
				}
				Set_InspectJigNo(1, nIs3ScanNo, nI3JNo1, nI3JNo2, nI3JNo3, nI3JNo4);
				g_objInspector.Set_LoadComplete(INSPECTOR_PC2, "T1", gLot.sLotID[gData.nPNoInspect[2]-1], gData.nPNoInspect[2],
					gData.nTNoInspect[2][nI3JNo1-1], gData.nTNoInspect[2][nI3JNo2-1], 0, 0,
					gData.nCNoInspect[2][nI3JNo1-1], gData.nCNoInspect[2][nI3JNo2-1], 0, 0);

				m_nInspect3Case = 10; m_tInspect3Loop.Set_LoopTime(90000);
				m_tInspect3Loop.Takt_Start(nTaktZone, 8);
				m_tInspect3Loop.Takt_End(nTaktZone,8);
			} else {
				m_nInspect3Case = 15; m_tInspect3Loop.Set_LoopTime(5000);
			}
			nIs3ScanNo++;
		}
		break;

	case 10:	// Wait for Top1 inspection(30sec)
		if (!m_pEquipData->bUseInspectTop1) {
			m_nInspect3Case = 15; m_tInspect3Loop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Top1 Z Focus Move
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			m_strLog.Format("MCC,(07) Inspection3, T1 Z Focus Move Start");
			g_objLogFile.Save_MCCLog(m_strLog);
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, m_dTop1Z);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000);
			m_tInspect3Loop.Takt_Start(nTaktZone, 11);
		}
		break;
	case 12:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_TOP1_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dTop1Z, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_TOP1_Z, strLog);

			g_objInspector.Set_MoveComplete(INSPECTOR_PC2, "T1");
			m_nInspect3Case = 10; m_tInspect3Loop.Set_LoopTime(90000);	// 90초
			m_tInspect3Loop.Takt_End(nTaktZone,11);
			m_tInspect3Loop.Takt_Start(nTaktZone,12);
			m_tInspect3Loop.Takt_End(nTaktZone,12);
		}
		break;

	case 13:	// Top1 Mirror Z Move
		if (g_objAJinAXL.Is_Done(AX_TOP1_MIRROR_Z) && g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			//g_objLogFile.VisionTakt_Save(AUTO_INSPECTION3, 21, TOP1);	// Top1 Module Position Inspect Complete
			m_strLog.Format("MCC,(07) Inspection3, T1 Mirror Z Move Start");
			g_objLogFile.Save_MCCLog(m_strLog);

			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, m_dTop1Z);
			g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 1);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000);
			m_tInspect3Loop.Takt_Start(nTaktZone,13);
		}
		break;
	case 14:	// Send Move Complete
		if (g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 1) && g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			
			g_objLogFile.VisionTakt_Start(TOP1);	// Top1 Mirror Down 이후 Scan
			g_objInspector.Set_SMoveComplete(INSPECTOR_PC2, "T1");
			m_nInspect3Case = 10; m_tInspect3Loop.Set_LoopTime(90000);	// 90초

			m_tInspect3Loop.Takt_End(nTaktZone,13);
			m_tInspect3Loop.Takt_Start(nTaktZone,14);
			m_tInspect3Loop.Takt_End(nTaktZone,14);
		}
		break;

	case 15:	// Mirror Z축 안전 위치로 회피
		//if (!m_pEquipData->bUseInspectTop1) { if(!m_tInspect3Loop.Waiting_Time(700)) break; }	// Delay
		
		g_objCommon.Move_Position(AX_VISION_TOP1_Z, 0);	// Ready Up
		g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 0);	// Ready Up
		m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(30000);
		m_tInspect3Loop.Takt_Start(nTaktZone, 15);
		m_tInspect3Loop.Takt_End(nTaktZone,15);
		break;
	case 16:	// 다음 검사 위치 피치 이동
		if (g_objAJinAXL.Is_Done(AX_INSPECT_STAGE3_X) && g_objCommon.Check_Position(AX_VISION_TOP1_Z, 0) && g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0)) {
			if (!bTaktSave3) { bTaktSave3 = TRUE;  }	// Move to Top1 Position

			if (gData.bReload[2]) {
				nIs3ScanNo--; gData.bReload[2] = FALSE;
			}
			if (nIs3ScanNo > gData.nScanCnt) {	// 모델별로 피치이동 횟수가 다르다.
				nIs3ScanNo = 0;
				bTaktSave3 = FALSE;
				m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000);
				m_tInspect3Loop.Takt_Start(nTaktZone, 16);
				m_tInspect3Loop.Takt_End(nTaktZone,16);

			} else {
				double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE2_X);
				double dT2Pos = m_pMoveData->dInspectStage2X[3];// + m_pEquipData->dInspectPitchX * 2;	// R04A/B T2 Pos + Inspection Stage Pitch * 4
				double dUnPos = m_pMoveData->dInspectStage2X[4];	//Unload Pos
				double dT1Pos = m_pMoveData->dInspectStage2X[2];
				if(nIs3ScanNo == 3 && (dCurPos <= (dT2Pos + m_pEquipData->dInspectPitchX) && dCurPos > (dT1Pos + m_pEquipData->dInspectPitchX*3)))
				{
					m_tInspect3Loop.Set_LoopTime(10000); return TRUE;
				}

				// 첫번째 스캔이후는 앞의 스테이지가 Unload 위치거나 Down 상태일때 진행가능.
				if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 4) || (dCurPos >= dT2Pos && dCurPos <= dUnPos) ||
					(!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down && !m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd)) {
					if (Select_Top1ScanPos(AUTO_INSPECTION3, nIs3ScanNo)) {
						
						dIs3PosX = m_pMoveData->dInspectStage3X[2] + nIs3ScanNo * m_pEquipData->dInspectPitchX;
						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX);
						m_nInspect3Case = 8; m_tInspect3Loop.Set_LoopTime(10000);
						m_tInspect1Loop.Set_LoopTime(15000);//뒤에 따라오는 스테이지 알람 대기시간 초기화 

						if(nIs3ScanNo == 3 && m_nInspect1Case == 1 && g_objAJinAXL.Is_Done(AX_INSPECT_STAGE1_X)
							&& g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0) && g_objCommon.Get_InfoInspectVacOn(1)){ // Top1 마지막 피치 이동시 뒤에 있는 스테이지도 동시 어라인 진입 
							g_objCommon.Move_Position(AX_INSPECT_STAGE1_X, 1);
							m_nInspect1Case = 2; m_tInspect1Loop.Set_LoopTime(9000);
						}
						m_tInspect3Loop.Takt_Start(nTaktZone, 16);
						m_tInspect3Loop.Takt_End(nTaktZone,16);
					} else {
						nIs3ScanNo++;
					}
				} else { m_tInspect3Loop.Set_LoopTime(10000); return TRUE;}
			}
		}
		break;
	case 17:	// 안전확인, Move to Top2 Position
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE3_X, dIs3PosX)) {
			// 다운 상태, Load, Unload 위치일때 진행
			if ((!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down && !m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd) ||
				g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0) || g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 4)) {
				
				dIs3PosX = m_pMoveData->dInspectStage3X[3];
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX);
				nIs3ScanNo = 0;
				
				m_nInspect3Case = 25; m_tInspect3Loop.Set_LoopTime(10000); 
				m_tInspect3Loop.Takt_Start(nTaktZone, 17);
				m_tInspect3Loop.Takt_End(nTaktZone,17);
			}
		}
		return TRUE;

	case 19:	// Send Load Complete
		if (g_objCommon.Get_InfoInspectVacOn(3) &&
			g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE3_X, dIs3PosX) &&
			g_objCommon.Check_Position(AX_VISION_TOP2_Z, 0) && g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 0))
		{
			
			g_objLogFile.VisionTakt_Start(TOP2);	// Top2 FPCB Scan Start
			if (m_pEquipData->bUseInspectTop2) {
				Set_InspectJigNo(2, nIs3ScanNo, nI3JNo1, nI3JNo2, nI3JNo3, nI3JNo4);
				g_objInspector.Set_LoadComplete(INSPECTOR_PC3, "T2", gLot.sLotID[gData.nPNoInspect[2]-1], gData.nPNoInspect[2], 
					gData.nTNoInspect[2][nI3JNo1-1], gData.nTNoInspect[2][nI3JNo2-1], 0, 0,
					gData.nCNoInspect[2][nI3JNo1-1], gData.nCNoInspect[2][nI3JNo2-1], 0, 0);

				m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(180000); 
				m_tInspect3Loop.Takt_Start(nTaktZone, 19);
				m_tInspect3Loop.Takt_End(nTaktZone,19);
			} else {
				m_nInspect3Case = 25; m_tInspect3Loop.Set_LoopTime(10000); 
			}
			nIs3ScanNo++;
		}
		break;

	case 20:	// Wait for Top2 inspection(30sec)
		if (!m_pEquipData->bUseInspectTop2) {
			m_nInspect3Case = 25; m_tInspect3Loop.Set_LoopTime(10000);
		}
		break;

	case 21:	// Top2 Z Focus Move
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			m_strLog.Format("MCC,(07) Inspection3, T2 Z Focus Start");
			g_objLogFile.Save_MCCLog(m_strLog);
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, m_dTop2Z);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000);
			m_tInspect3Loop.Takt_Start(nTaktZone, 21);
		}
		break;
	case 22:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_TOP2_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dTop2Z, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_TOP2_Z, strLog);

			g_objInspector.Set_MoveComplete(INSPECTOR_PC3, "T2");
			m_nInspect3Case = 20; m_tInspect3Loop.Set_LoopTime(90000);	// 90초
			m_tInspect3Loop.Takt_End(nTaktZone,21);
			m_tInspect3Loop.Takt_Start(nTaktZone, 22);
			m_tInspect3Loop.Takt_End(nTaktZone,22);
		}
		break;

	case 23:	// Top2 Shift X, Y Move
		if (g_objAJinAXL.Is_Done(AX_INSPECT_STAGE3_X) && g_objAJinAXL.Is_Done(AX_TOP2_SHIFT_Y) && g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			//g_objLogFile.VisionTakt_Save(AUTO_INSPECTION3, 23, TOP2);	// Top2 FPCB Scan End
			m_strLog.Format("MCC,(07) Inspection3, Top2 Shift X, Y Move Start");
			g_objLogFile.Save_MCCLog(m_strLog);
			dIs3PosX = dIs3PosX + m_pEquipData->dInspectTop2ShiftX;

			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, m_dTop2Z);
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX);
			g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 1);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000);
			m_tInspect3Loop.Takt_Start(nTaktZone, 23);
		}
		break;
	case 24:	// Send Move Complete
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE3_X, dIs3PosX) && g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 1) && g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			
			g_objLogFile.VisionTakt_Start(TOP2);	// Top2 Lens/Barrel/Sidefill Scan Start
			g_objInspector.Set_SMoveComplete(INSPECTOR_PC3, "T2");
			m_nInspect3Case = 20; m_tInspect3Loop.Set_LoopTime(90000);	// 90초
			m_tInspect3Loop.Takt_End(nTaktZone,23);
			m_tInspect3Loop.Takt_Start(nTaktZone, 24);
			m_tInspect3Loop.Takt_End(nTaktZone,24);
		}
		break;

	case 25:	// 다음 검사 위치 피치 이동
		//if (!m_pEquipData->bUseInspectTop2) { if(!m_tInspect3Loop.Waiting_Time(500)) break; }	// Delay

		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE3_X, dIs3PosX) &&
			g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z) && g_objAJinAXL.Is_Done(AX_TOP2_SHIFT_Y)) {
			if (gData.bReload[3]) { nIs3ScanNo--; gData.bReload[3] = FALSE; }

			if (nIs3ScanNo > gData.nBtmPickQt-1) {	// 모델별로 피치이동 횟수가 다르다.
				
				nIs3ScanNo = 0;
				bTaktSave3 = FALSE;
				g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);	// Ready Up
				g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);	// Ready

				g_objCommon.Move_Position(AX_INSPECT_STAGE3_X, 4);	// Unload

				m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000);
				m_tInspect3Loop.Takt_Start(nTaktZone, 25);
				m_tInspect3Loop.Takt_End(nTaktZone,25);
			} else {	// 첫번째 스캔이후 앞의 스테이지는 다운상태 이거나 로드 위치에 있어야한다.
				if (nIs3ScanNo == 0 || (!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down && !m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd) ||
					(g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0) || g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 1, 5.0) || g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 5))) {
					
					if (Select_Top2ScanPos(AUTO_INSPECTION3, nIs3ScanNo)) {
						dIs3PosX = m_pMoveData->dInspectStage3X[3] + nIs3ScanNo * m_pEquipData->dInspectPitchX;

						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX);			
						g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);	// Ready Up
						g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);	// Ready
						m_nInspect3Case = 19; m_tInspect3Loop.Set_LoopTime(10000);
						m_tInspect3Loop.Takt_Start(nTaktZone, 25);
						m_tInspect3Loop.Takt_End(nTaktZone,25);
					} else {
						nIs3ScanNo++;
					}
				} else { m_tInspect3Loop.Set_LoopTime(10000); return TRUE; }
			}
		}
		break;
	case 26:	// Unload Position Check
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 4)) {
			m_tInspect3Loop.Takt_Start(nTaktZone, 26);
			m_tInspect3Loop.Takt_End(nTaktZone,26);

			m_nInspect3Case = 30; m_tInspect3Loop.Set_LoopTime(10000);
		}
		break;

	case 30:	// Wait for Unloading
		return TRUE;

	case 31:	// 안전확인, Stage Down & Backward
		if (!g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 4)) {	// Unload 위치에 없으면 다운
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE2_X);
			double dT2Pos = m_pMoveData->dInspectStage2X[3];	//T2 Pos
			if (dCurPos <= dT2Pos) {	//위치 확인
				
				m_pDY05->oInspectStage3Up = FALSE;	m_pDY05->oInspectStage3Down = TRUE;
				m_pDY05->oInspectStage3Fwd = FALSE;	m_pDY05->oInspectStage3Bwd = TRUE;
				g_objAJinAXL.Write_Output(5);
				m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000);
				m_tInspect3Loop.Takt_Start(nTaktZone, 31);
			}
		}
		return TRUE;
	case 32:	// Stage Down, Backward Check
		if (!m_pDX05->iInspectStage3Up  && m_pDX05->iInspectStage3Down && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd) {
			
			m_nInspect3Case = 50; m_tInspect3Loop.Set_LoopTime(10000);
			m_tInspect3Loop.Takt_End(nTaktZone,31);
			m_tInspect3Loop.Takt_Start(nTaktZone, 32);
			m_tInspect3Loop.Takt_End(nTaktZone,32);
		}
		break;

	case 50:	// Wait 회피 위치, Stage가 모두 Up 상태면 진행.
		if ((m_nInspect1Case >= 0 && m_nInspect1Case <= 31 && m_pDX05->iInspectStage1Up && !m_pDX05->iInspectStage1Down) &&
			(m_nInspect2Case >= 0 && m_nInspect2Case <= 31 && m_pDX05->iInspectStage2Up && !m_pDX05->iInspectStage2Down)) {
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000);
		}
		return TRUE;

	case 51:	// Move to Load Position
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 4)) {
			
			g_objCommon.Move_Position(AX_INSPECT_STAGE3_X, 0);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000);
			m_tInspect3Loop.Takt_Start(nTaktZone, 51);
		}
		break;
	case 52:	// Position Check
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 0)) {
			
			m_nInspect3Case = 70; m_tInspect3Loop.Set_LoopTime(10000);
			m_tInspect3Loop.Takt_End(nTaktZone,51);
		}
		break;

	case 70:	// Wait 회피 위치, Top1 Position 부터 진행한다.
		if (m_nInspect2Case < 80 && m_nInspect2Case > 7 && !g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0)) {
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE2_X);
			double dT1Pos = m_pMoveData->dInspectStage2X[2];	//T1 Pos
			if (dCurPos >= dT1Pos) {	//위치 확인
				m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;
	case 71:	// Stage Up
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 0)) {
			
			m_pDY05->oInspectStage3Up = TRUE;	m_pDY05->oInspectStage3Down = FALSE;
			m_pDY05->oInspectStage3Fwd = TRUE;	m_pDY05->oInspectStage3Bwd = FALSE;
			g_objAJinAXL.Write_Output(5);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000);
			m_tInspect3Loop.Takt_Start(nTaktZone, 71);
		}
		break;
	case 72:	// Stage Up Check
		if (m_pDX05->iInspectStage3Up && !m_pDX05->iInspectStage3Down && m_pDX05->iInspectStage3Fwd && !m_pDX05->iInspectStage3Bwd) {
			if(m_nBtm1PickCase == 21 && !m_tInspect3Loop.Waiting_Time(200)) break;
						
			m_strLog.Format("Inspection Stage3, %d", GetTickCount() - m_dwInspect3);
			g_objLogFile.Save_TestLog(m_strLog);
			m_nInspect3Case = 0; m_tInspect3Loop.Set_LoopTime(10000);
			m_tInspect3Loop.Takt_End(nTaktZone,71);
		}
		break;

	case 80:	//Move to Backward Offset
		if (TRUE/*g_objCommon.Get_InspectStage3VacOff(0)*/) {
			dIs3PosX = m_pMoveData->dInspectStage3X[1] + m_pEquipData->dInspectALOffset;
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX, 0.25);	// X방향 얼라인. 속도는 1/4
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000);
		}
		break;
	case 81:
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE3_X, dIs3PosX)) {
			g_objCommon.Set_InfoInspectVacOn(3, 1);
			dIs3PosX = m_pMoveData->dInspectStage3X[1] + m_pEquipData->dInspectALOffset - m_pEquipData->dInspectALBackOffset;
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX, 0.25);	// X방향 얼라인. 속도는 1/4
			m_nInspect3Case = 4; m_tInspect3Loop.Set_LoopTime(5000);
		}
		break;

	case 85:	// Move to Module Align
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 5)) {
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE2_X);
			double dT2Pos = m_pMoveData->dInspectStage2X[3];	//T2 Pos
			double dUnPos = m_pMoveData->dInspectStage2X[4];	//Unload Pos
			if (dCurPos >= dT2Pos && dCurPos <= dUnPos) {	//위치 확인
				g_objCommon.Move_Position(AX_INSPECT_STAGE3_X, 1);
				m_nInspect3Case = 2; m_tInspect3Loop.Set_LoopTime(5000);
			} else {
				m_tInspect3Loop.Set_LoopTime(5000);
				return TRUE;
			}
		}
		break;
	}

	// 9. (Error : 3900)
	if (m_tInspect3Loop.Over_LoopTime()) {
		if (m_nInspect3Case == 10) {
			Set_ScanError(3910, gData.nTNoInspect[2][nI3JNo1-1], gData.nTNoInspect[2][nI3JNo2-1], 0, 0,
								gData.nCNoInspect[2][nI3JNo1-1], gData.nCNoInspect[2][nI3JNo2-1], 0, 0);
		} else if (m_nInspect3Case == 20) {
			Set_ScanError(3920, gData.nTNoInspect[2][nI3JNo1-1], gData.nTNoInspect[2][nI3JNo2-1], 0, 0,
								gData.nCNoInspect[2][nI3JNo1-1], gData.nCNoInspect[2][nI3JNo2-1], 0, 0);
		}
		g_objCommon.Show_Error(3900 + m_nInspect3Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 10. (Error : 4000)
BOOL CSequenceMain::Btm2Picker_Run()
{
	int nTaktZone = 10;		// Takt_Start, Takt_End
	static int		nB2pWorkTray = 0;								// 작업중인 Buffer Tray (1 ~ 2)
	static int		nB2pStartNo = 0, nB2PickNo = 0;					// Btm2 Picker 시작 번호(0~9), 열별 순번(0~4)
	static int		nB2pTrayPosY = 0;								// 작업중인 Buffer Tray 위치 Y (R04A:0 ~ 4, R04B:0 ~ 2, R04C:0 ~ 3), X (R04A:0 ~ 3, R04B:0 ~ 4, R04C:0 ~ 7)
	static double	dB2pTrayY = 0.0, dB2pX = 0.0, dB2pZ = 0.0;		// Y, X, Z 이동위치
	static int		nB2pRow = 0, nB2pScanNo = 0, nB2pInspStageNo = 0;
	static int		nB2JNo1 = 0, nB2JNo2 = 0, nB2JNo3 = 0, nB2JNo4 = 0;
	static BOOL		bSave = FALSE;

	switch (m_nBtm2PickCase) {
	case 0:		// Wait for Angle Tray L1 Position
		if (m_nInspect1Case == 30 || m_nInspect2Case == 30 || m_nInspect3Case == 30) {
			m_nBtm2PickCase++;
		}
		return TRUE;

	case 1:
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_X, 0) &&
			g_objCommon.Check_Position(AX_BTM2_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM2_PICKER_P2, 0)) {
			if (m_nInspect1Case != 30 && m_nInspect2Case != 30 && m_nInspect3Case != 30) break;	// 인터락
			if (m_nInspect1Case == 30) nB2pInspStageNo = 1;
			if (m_nInspect2Case == 30) nB2pInspStageNo = 2;
			if (m_nInspect3Case == 30) nB2pInspStageNo = 3;
			m_dwBtm2Pick = GetTickCount();
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
			m_tBtm2PickLoop.Takt_Start(nTaktZone, 1, TRUE);
		}
		break;

	case 2:		// Picker Z Inspection Stage Down
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) &&
			g_objCommon.Check_Position(AX_BTM2_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM2_PICKER_P2, 0)) {
					
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 1);
			g_objCommon.Set_Btm2PickerDown();
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(10000);
			m_tBtm2PickLoop.Takt_End(nTaktZone, 1);
			m_tBtm2PickLoop.Takt_Start(nTaktZone, 2);
		}
		break;
	case 3:		// 정보전달(Stage->Picker), Btm2 Picker Grip Close
		if (g_objCommon.Get_Btm2PickerDown(0) && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 1)) {
			if (!m_tBtm2PickLoop.Waiting_Time(50)) break;
			for (int i = 0; i < 10; i++) {
				gData.InfoBtm2Pick[i] = gData.InfoInspect[nB2pInspStageNo-1][i]; gData.InfoInspect[nB2pInspStageNo-1][i] = 0;
									   
				gData.nTNoBtm2Pick[i] = gData.nTNoInspect[nB2pInspStageNo-1][i];
				gData.nCNoBtm2Pick[i] = gData.nCNoInspect[nB2pInspStageNo-1][i];
				gData.nTNoInspect[nB2pInspStageNo-1][i] = gData.nCNoInspect[nB2pInspStageNo-1][i] = 0;

				//모듈트랙킹
				int nPNo = gData.nPNoInspect[nB2pInspStageNo-1] - 1;
				int nTNo = gData.nTNoBtm2Pick[i] - 1;
				int nCNo = gData.nCNoBtm2Pick[i] - 1;
				if (nCNo > -1) gData.nCmJigNo[nPNo][nTNo][nCNo][BTM2_PICK] = i + 1;	// Btm2 Jig No.
			}
			gData.nPNoBtm2Pick = gData.nPNoInspect[nB2pInspStageNo-1]; gData.nPNoInspect[nB2pInspStageNo-1] = 0;

			m_tBtm2PickLoop.Takt_End(nTaktZone, 2);
			m_tBtm2PickLoop.Takt_Start(nTaktZone, 3);

			//MCC
			m_strLog.Format("Btm2 Picker Get Modules, Inspect Stage No:%d", nB2pInspStageNo);
			g_objLogFile.Save_MCCLog(m_strLog);

			g_objCommon.Set_InfoBtm2Close();
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(10000);
		}
		break;
	case 4:		// Picker Vaccum On & Stage Vacuum Off
		if (!m_tBtm2PickLoop.Waiting_Time(50)) break;	// Btm2 Close Delay
		if (g_objCommon.Get_InfoBtm2Close()) {
			

			g_objCommon.Set_InfoBtm2VacOn();					// Picker Vac On
			g_objCommon.Set_InspectVacOff(nB2pInspStageNo, 0);	// Stage Vac Off

			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
			m_tBtm2PickLoop.Takt_End(nTaktZone, 3);
			m_tBtm2PickLoop.Takt_Start(nTaktZone, 4);
		}
		break;
	case 5:		// Picker Up
		if (g_objCommon.Get_InfoBtm2Close() && g_objCommon.Get_InspectVacOff(nB2pInspStageNo, 0) ) {	// Btm2 Picker Delay Time만 사용한다.
			if (!m_tBtm2PickLoop.Waiting_Time(m_pEquipData->nDelayAdd[1])) break;	// Btm2 Delay
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 0);
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
			m_tBtm2PickLoop.Takt_End(nTaktZone, 4);
			m_tBtm2PickLoop.Takt_Start(nTaktZone, 5);
		}
		break;
	case 6:		// Position Check 
		if (g_objCommon.Get_InfoBtm2Check() && g_objCommon.Get_InfoBtm2VacOn() && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) &&
			g_objCommon.Check_Position(AX_BTM2_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM2_PICKER_P2, 0))
		{
			if (nB2pInspStageNo == 1 && m_nInspect1Case == 30) m_nInspect1Case = 31;
			if (nB2pInspStageNo == 2 && m_nInspect2Case == 30) m_nInspect2Case = 31;
			if (nB2pInspStageNo == 3 && m_nInspect3Case == 30) m_nInspect3Case = 31;

			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
			m_tBtm2PickLoop.Takt_End(nTaktZone, 5);
			m_tBtm2PickLoop.Takt_Start(nTaktZone, 6);
		}
		break;
	case 7:		// Wait. 안전확인 
		// TransY 0, 20~: Good Stage Position, 3~15: Empty Port Position 
		// TransX 10~20: Empty Port Position에 있으면 TransY는 대기중.
		if (((m_nEmptyTrayYCase ==  0 || m_nEmptyTrayYCase ==  2 || m_nEmptyTrayYCase >= 20) && g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0)) ||
			((m_nEmptyTrayYCase >=  3 && m_nEmptyTrayYCase <  15) && g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 1)) ||
			((m_nEmptyTrayXCase >  10 && m_nEmptyTrayXCase <  20) && g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 1)))
		{
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
		} 
		return TRUE;
	case 8:		// Btm2 Inspection 위치로 이동.
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_X, 0) && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) &&
			g_objCommon.Get_InfoBtm2VacOn() && g_objCommon.Get_InfoBtm2Check())
		{
			
			dB2pX = m_pMoveData->dBtm2PickerX[1];	// Btm2 Inspect
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 2);
			g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_X, dB2pX);
			nB2pScanNo = 0;

			
			m_nBtm2PickCase = 14; m_tBtm2PickLoop.Set_LoopTime(10000);
			m_tBtm2PickLoop.Takt_End(nTaktZone, 6);
			m_tBtm2PickLoop.Takt_Start(nTaktZone, 8);
		}
		break;

	case 9:		// Set Load Complete
		if (g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_X, dB2pX) && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 2) &&
			g_objCommon.Get_InfoBtm2VacOn() && g_objCommon.Get_InfoBtm2Check())
		{
			
			m_tBtm2PickLoop.Takt_Start(nTaktZone, 9);
			m_tBtm2PickLoop.Takt_End(nTaktZone, 9);
			if (m_pEquipData->bUseInspectBtm2) {
				
				g_objLogFile.VisionTakt_Start(BTM2);	// Scan Start

				Set_InspectJigNo(3, nB2pScanNo, nB2JNo1, nB2JNo2, nB2JNo3, nB2JNo4);
				g_objInspector.Set_LoadComplete(INSPECTOR_PC4, "B2", gLot.sLotID[gData.nPNoBtm2Pick-1], gData.nPNoBtm2Pick, 
					gData.nTNoBtm2Pick[nB2JNo1-1], gData.nTNoBtm2Pick[nB2JNo2-1], 0, 0,
					gData.nCNoBtm2Pick[nB2JNo1-1], gData.nCNoBtm2Pick[nB2JNo2-1], 0, 0);

				m_nBtm2PickCase = 10; m_tBtm2PickLoop.Set_LoopTime(30000);
				
			} else {
				if (!m_pEquipData->bUseInspectBtm2) { if(!m_tBtm2PickLoop.Waiting_Time(250)) break; }
				m_nBtm2PickCase = 15; m_tBtm2PickLoop.Set_LoopTime(5000);
			}
			nB2pScanNo++;
		}
		break;

	case 10:	// Wait
		if (!m_pEquipData->bUseInspectBtm2) {
			m_nBtm2PickCase = 15; m_tBtm2PickLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Btm2 Z Position Move
		if (g_objAJinAXL.Is_Done(AX_BTM2_PICKER_Z)) {
			m_strLog.Format("MCC,(10) Btm2Picker, Btm2 Z Focus Move Start(Scan End)");
			g_objLogFile.Save_MCCLog(m_strLog);
			g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_Z, m_dBtm2Z);
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
		
			m_tBtm2PickLoop.Takt_Start(nTaktZone,11);
		}
		break;
	case 12:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_BTM2_PICKER_Z)) {
			
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_BTM2_PICKER_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dBtm2Z, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_BTM2_PICKER_Z, strLog);

			g_objInspector.Set_MoveComplete(INSPECTOR_PC4, "B2");
			m_nBtm2PickCase = 10; m_tBtm2PickLoop.Set_LoopTime(30000);	// 30초
			m_tBtm2PickLoop.Takt_End(nTaktZone, 11);
			m_tBtm2PickLoop.Takt_Start(nTaktZone,12);
			m_tBtm2PickLoop.Takt_End(nTaktZone,12);
		}
		break;

	case 14:	// BTM2 Z, I/O Down
		if (g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_X, dB2pX)) {
			if (!bSave) { bSave = TRUE;  }
			if (!g_objCommon.Get_InfoBtm2VacOn()) { m_tBtm2PickLoop.Set_LoopTime(0); break;}	// 바로 알람 처리 해준다.

			bSave = FALSE;
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 2);
			g_objCommon.Set_Btm2PickerDown();

			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(10000);
			m_tBtm2PickLoop.Takt_End(nTaktZone, 8);
			m_tBtm2PickLoop.Takt_Start(nTaktZone,14);
			
		}
		break;
	case 15:	// X Move to Pitch & Inspection End Check
		if (g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_X, dB2pX)  && g_objCommon.Get_Btm2PickerDown(0)) {
			//nB2pScanNo++;
			m_tBtm2PickLoop.Takt_End(nTaktZone, 14);
			m_tBtm2PickLoop.Takt_Start(nTaktZone,15);
			m_tBtm2PickLoop.Takt_End(nTaktZone, 15);
			if (gData.bReload[4]) {
				nB2pScanNo--; gData.bReload[4] = FALSE;
			}
			if (nB2pScanNo > gData.nBtmPickQt-1) {	//2) { 
				
				nB2pScanNo = 0;
				g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 0);
				m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(10000);
			} else {
				if (Select_BtmScanPos(AUTO_VISION_BTM2, nB2pScanNo)) {
					
					dB2pX = m_pMoveData->dBtm2PickerX[1] + nB2pScanNo * m_pEquipData->dInspectPitchX;	// m_pEquipData->dTrayPitchX;	// Btm2 Inspect

					g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_X, dB2pX);
					g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 2);	// Btm2 Down
					m_nBtm2PickCase = 9; m_tBtm2PickLoop.Set_LoopTime(10000);
				} else {
					nB2pScanNo++;
				}
			}
		}
		break;
	case 16:	// Buffer Tray Position X, Y Move
		if (g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_X, dB2pX) && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0)) {
			
			if (m_nBuffTray1Case == 10 || m_nBuffTray2Case == 10) {
				if (m_nBuffTray1Case == 10) nB2pWorkTray = 1;
				if (m_nBuffTray2Case == 10) nB2pWorkTray = 2;
				// Buffer Tray Position
				if (Select_Btm2PickPos(nB2pStartNo)) {
					Select_BufferTrayDownPos(nB2pWorkTray, nB2pTrayPosY, nB2pRow);

					// 2:Buffer1 L1, 3:Buffer1 L2, 4:Buffer2 L1, 5:Buffer L2
					if (nB2pStartNo > 3) nB2pRow = 1;
					else				 nB2pRow = 0;

					if (nB2pWorkTray == 1) {
						if (nB2pRow == 0) dB2pX = m_pMoveData->dBtm2PickerX[2];
						if (nB2pRow == 1) dB2pX = m_pMoveData->dBtm2PickerX[3];
					}
					if (nB2pWorkTray == 2) {
						if (nB2pRow == 0) dB2pX = m_pMoveData->dBtm2PickerX[4];
						if (nB2pRow == 1) dB2pX = m_pMoveData->dBtm2PickerX[5];
					}					
					g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_X, dB2pX);
					g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 0);
					g_objCommon.Move_Position(AX_BTM2_PICKER_P1, 1);	// Tray Pitch
					g_objCommon.Move_Position(AX_BTM2_PICKER_P2, 1);	// Tray Pitch

					if (nB2pWorkTray == 1 && m_nBuffTray1Case == 10) {
									
						dB2pTrayY = m_pMoveData->dBufferStage1Y[nB2pRow] + nB2pTrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dB2pTrayY);
					}
					if (nB2pWorkTray == 2 && m_nBuffTray2Case == 10) {
						dB2pTrayY = m_pMoveData->dBufferStage2Y[nB2pRow] + nB2pTrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dB2pTrayY);
					}
					m_nBtm2PickCase = 21; m_tBtm2PickLoop.Set_LoopTime(10000);
										
					m_tBtm2PickLoop.Takt_Start(nTaktZone,16);
				}
			} else {
				// Tray 교체 이유로 Buffer가 대기중이 아니면 기본 위치로 간다.
				dB2pX = m_pMoveData->dBtm2PickerX[2];
				g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_X, dB2pX);
				g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 0);
				m_nBtm2PickCase = 20; m_tBtm2PickLoop.Set_LoopTime(10000);
				
				m_tBtm2PickLoop.Takt_Start(nTaktZone,16);
			}
		}
		break;

	case 20:	// Move to Buffer Position
		if (g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_X, dB2pX) && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) && g_objCommon.Get_Btm2PickerDown(0)) {
			if (m_nBuffTray1Case == 10 || m_nBuffTray2Case == 10) {
				if (m_nBuffTray1Case == 10) nB2pWorkTray = 1;
				if (m_nBuffTray2Case == 10) nB2pWorkTray = 2;
				// Buffer Tray Position
				if (Select_Btm2PickPos(nB2pStartNo)) {
					Select_BufferTrayDownPos(nB2pWorkTray, nB2pTrayPosY, nB2pRow);

					// 2:Buffer1 L1, 3:Buffer1 L2, 4:Buffer2 L1, 5:Buffer L2
					if (nB2pStartNo > 3) nB2pRow = 1;
					else				 nB2pRow = 0;
					
					if (nB2pWorkTray == 1) {
						if (nB2pRow == 0) dB2pX = m_pMoveData->dBtm2PickerX[2];	// Buffer1 L1 Position
						if (nB2pRow == 1) dB2pX = m_pMoveData->dBtm2PickerX[3];	// Buffer1 L2 Position
					}
					if (nB2pWorkTray == 2) {
						if (nB2pRow == 0) dB2pX = m_pMoveData->dBtm2PickerX[4];	// Buffer2 L1 Position
						if (nB2pRow == 1) dB2pX = m_pMoveData->dBtm2PickerX[5];	// Buffer2 L2 Position
					}
					g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_X, dB2pX);
					g_objCommon.Move_Position(AX_BTM2_PICKER_P1, 1);	// Tray Pitch
					g_objCommon.Move_Position(AX_BTM2_PICKER_P2, 1);	// Tray Pitch

					if (nB2pWorkTray == 1 && m_nBuffTray1Case == 10) {
						dB2pTrayY = m_pMoveData->dBufferStage1Y[nB2pRow] + nB2pTrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dB2pTrayY);
					}
					if (nB2pWorkTray == 2 && m_nBuffTray2Case == 10) {
						dB2pTrayY = m_pMoveData->dBufferStage2Y[nB2pRow] + nB2pTrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dB2pTrayY);
					}
					m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
								
					m_tBtm2PickLoop.Takt_End(nTaktZone, 16);
					m_tBtm2PickLoop.Takt_End(nTaktZone,26);
					m_tBtm2PickLoop.Takt_Start(nTaktZone,20);
				}
			}
		}
		return TRUE;
	case 21:	// Picker Up 옵션 확인 및 구동
		if (g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_X, dB2pX) && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) &&
			g_objCommon.Check_Position(AX_BTM2_PICKER_P1, 1) && g_objCommon.Check_Position(AX_BTM2_PICKER_P2, 1) &&
			((nB2pWorkTray == 1 && g_objAJinAXL.Is_MoveDone(AX_BUFFER_STAGE1_Y, dB2pTrayY)) ||
			 (nB2pWorkTray == 2 && g_objAJinAXL.Is_MoveDone(AX_BUFFER_STAGE2_Y, dB2pTrayY))) )
		{
			
			// 옵션처리하여 장비마다 다르게 쓸수있게 해준다.
			if (m_pEquipData->bUseBtm2PickUpDown) {
				if (nB2pRow == 0) {
					g_objCommon.Set_Btm2PickerUp(0);	// Picker 고정으로 사용.
				}
			}
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);

			m_tBtm2PickLoop.Takt_End(nTaktZone, 16);
			m_tBtm2PickLoop.Takt_End(nTaktZone,20);
			m_tBtm2PickLoop.Takt_Start(nTaktZone,21);
		}
		break;
	case 22:	// Picker Down	동시동작
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) &&
			((m_pEquipData->bUseBtm2PickUpDown && nB2pRow == 0 && g_objCommon.Get_Btm2PickerUp(0)) ||
			 !m_pEquipData->bUseBtm2PickUpDown || nB2pRow != 0) )
		{
			//if (!m_tBtm2PickLoop.Waiting_Time(200)) break;	// 안정시간
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 3);	// Buffer Down
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
			m_tBtm2PickLoop.Takt_End(nTaktZone,21);
			m_tBtm2PickLoop.Takt_Start(nTaktZone,22);
		}
		break;

	case 23:	// 정보전달, Btm2 Picker Vac Off
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 3))
		{
			for (int i = 0; i < gData.nBtmPickQt; i++) {
				int nBtmX = 0, nBuffX = 0;
				nBtmX = 4 * nB2pRow;

				gData.InfoBuffTray[nB2pWorkTray-1][nB2pTrayPosY][nBuffX+i] = gData.InfoBtm2Pick[nBtmX+i]; 
				gData.InfoBtm2Pick[nBtmX+i] = 0;

				gData.nTNoBuffTray[nB2pWorkTray-1][nB2pTrayPosY][nBuffX+i] = gData.nTNoBtm2Pick[nBtmX+i];
				gData.nCNoBuffTray[nB2pWorkTray-1][nB2pTrayPosY][nBuffX+i] = gData.nCNoBtm2Pick[nBtmX+i];
				gData.nTNoBtm2Pick[nBtmX+i] = gData.nCNoBtm2Pick[nBtmX+i] = 0;

				//모듈트랙킹
				int nPNo = gData.nPNoBtm2Pick - 1;
				int nTNo = gData.nTNoBuffTray[nB2pWorkTray-1][nB2pTrayPosY][nBuffX+i] - 1;
				int nCNo = gData.nCNoBuffTray[nB2pWorkTray-1][nB2pTrayPosY][nBuffX+i] - 1;
				if (nCNo > -1) {
					gData.nCmJigNo[nPNo][nTNo][nCNo][BUF_STAGE_NO] = nB2pWorkTray;	// buffer Stage No.
					gData.nCmJigNo[nPNo][nTNo][nCNo][BUF_JIG_NO]   = nBtmX + i + 1;	// buffer Stage pocket No.
				}
			}
			gData.nPNoBuffTray[nB2pWorkTray-1] = gData.nPNoBtm2Pick; 
			if (Check_Btm2PickerEmpty()) gData.nPNoBtm2Pick = 0;	// 모듈이 없을때 초기화
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 3, nB2pWorkTray-1);
			
			m_nBtm2PickMultiCnt = gData.nBtmPickQt;	m_nBtm2PickRow = nB2pRow;
			m_pThreadVacuumB2pMulti = AfxBeginThread(Thread_Vacuum_B2p_Multi, (LPVOID)(1));
			g_objCommon.Set_Btm2PickerOpen(1, nB2pRow);
						
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
			m_tBtm2PickLoop.Takt_End(nTaktZone,22);
			m_tBtm2PickLoop.Takt_Start(nTaktZone,23);
		}
		break;
	case 24:	// Picker Up
		if (!m_pThreadVacuumB2pMulti && g_objCommon.Get_Btm2PickerVacOffMulti(1, gData.nBtmPickQt, nB2pRow) && g_objCommon.Get_Btm2PickerOpen(1, nB2pRow)) {
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 0);	// Ready Up			
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
			m_tBtm2PickLoop.Takt_End(nTaktZone,23);
			m_tBtm2PickLoop.Takt_Start(nTaktZone,24);
		}
		break;
	case 25:	// Tray Check
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0)) {
			// 더이상 로드할께 없다 || 라인확인 || 현재 LOT 마지막 모듈이다 || 트레이가 꽉찼다
			if (m_bUnloadLotEnd || (nB2pTrayPosY >= gData.nTrayY) ||
				Check_Btm2LoadEnd(gData.nPNoBuffTray[nB2pWorkTray-1])|| Check_BufferFull(nB2pWorkTray-1)) {

				nB2pTrayPosY = 0;	// 초기화
				if (nB2pWorkTray == 1) { m_nBuffTray1Case = 11; m_tBuffTray1Loop.Set_LoopTime(20000); }
				if (nB2pWorkTray == 2) { m_nBuffTray2Case = 11; m_tBuffTray2Loop.Set_LoopTime(20000); }
			}
			g_objCommon.Set_Btm2PickerDown();	// 6~10 Picker Down.
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
			m_tBtm2PickLoop.Takt_End(nTaktZone,24);
			m_tBtm2PickLoop.Takt_Start(nTaktZone,25);
		}
		break;
	case 26:	// Picker Check, Unload or X, P1, P2 Move to Load Position
		if (g_objCommon.Get_Btm2PickerDown(0) && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0)) {
			
			if (Check_Btm2PickerEmpty()) {
				g_objCommon.Move_Position(AX_BTM2_PICKER_X, 0);
				g_objCommon.Move_Position(AX_BTM2_PICKER_P1, 0);
				g_objCommon.Move_Position(AX_BTM2_PICKER_P2, 0);
				m_nBtm2PickCase = 28; m_tBtm2PickLoop.Set_LoopTime(5000);

				m_tBtm2PickLoop.Takt_End(nTaktZone,25);
				m_tBtm2PickLoop.Takt_Start(nTaktZone,26);

			} else {
				m_nBtm2PickCase = 20; m_tBtm2PickLoop.Set_LoopTime(5000);
				m_tBtm2PickLoop.Takt_End(nTaktZone,25);
				m_tBtm2PickLoop.Takt_Start(nTaktZone,26);
			}
		} 
		break;
	//case 27:	// Picker Turn
	//	if (g_objCommon.Check_Position(AX_BTM2_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM2_PICKER_P2, 0)) {
	//		m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
	//	}
	//	break;
	case 28: // Position Check
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_X, 0) && g_objCommon.Check_Position(AX_BTM2_PICKER_P1, 0) &&
			g_objCommon.Check_Position(AX_BTM2_PICKER_P2, 0))
		{
			
			m_strLog.Format("Btm2 Picker, %d", GetTickCount() - m_dwBtm1Pick);
			g_objLogFile.Save_TestLog(m_strLog);
			m_nBtm2PickCase = 0; m_tBtm2PickLoop.Set_LoopTime(5000);

			m_tBtm2PickLoop.Takt_End(nTaktZone,26);
			m_tBtm2PickLoop.Takt_Start(nTaktZone,28);
			m_tBtm2PickLoop.Takt_End(nTaktZone,28,TRUE);
		}
		break;

	}

	// 10. (Error : 4000)
	if (m_tBtm2PickLoop.Over_LoopTime()) {
		if (m_nBtm2PickCase == 6) {
			int nPx = gData.nPNoBtm2Pick - 1;
			int nTx = gData.nTNoBtm2Pick[gData.nErrBtm2PickIdx] - 1;
			int nCx = gData.nCNoBtm2Pick[gData.nErrBtm2PickIdx] - 1;
			m_strLog.Format("%s,%s,Port(%d),Tray(%d),Pocket(%d),Btm2 Picker Vacuum Error", gData.sLotID[nPx], gMes.sBarID[nPx][nTx][nCx], nPx+1, nTx+1, nCx+1);
			g_objLogFile.Save_PickerLog(m_strLog);
		
		} else if (m_nBtm2PickCase == 10) {
			if (nB2pScanNo > 2) {
				Set_ScanError(4010, gData.nTNoBtm2Pick[nB2JNo1-1], gData.nTNoBtm2Pick[nB2JNo3-1], 0, 0, 
									gData.nCNoBtm2Pick[nB2JNo1-1], gData.nCNoBtm2Pick[nB2JNo3-1], 0, 0);
			} else {
				Set_ScanError(4010, gData.nTNoBtm2Pick[nB2JNo1-1], gData.nTNoBtm2Pick[nB2JNo2-1], gData.nTNoBtm2Pick[nB2JNo3-1], gData.nTNoBtm2Pick[nB2JNo4-1],
									gData.nCNoBtm2Pick[nB2JNo1-1], gData.nCNoBtm2Pick[nB2JNo2-1], gData.nCNoBtm2Pick[nB2JNo3-1], gData.nCNoBtm2Pick[nB2JNo4-1]);
			}
		}
		g_objCommon.Show_Error(4000 + m_nBtm2PickCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 11. (Error : 4100)
BOOL CSequenceMain::BufferTray1_Run()
{
	int nTaktZone = 11;		// Takt_Start, Takt_End

	switch (m_nBuffTray1Case) {
	case 0:		// Start시 1로 바뀜
		return TRUE;

	case 1:		// 안전확인.
		if (m_bUnloadLotEnd) {
			m_nBuffTray1Case = 0;

		} else if (!m_pDX09->iBufferStage1Rotate && m_pDX09->iBufferStage1Normal &&
			m_pDX09->iBufferStage1Up && g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 0)) {
			m_nBuffTray1Case++; m_tBuffTray1Loop.Set_LoopTime(5000);
			m_tBuffTray1Loop.Takt_Start(nTaktZone, 0);
			m_tBuffTray1Loop.Takt_End(nTaktZone, 0);
		}
		break;
	case 2:		// Tray Check
		if (TRUE) {
			m_dwBuffTray1 = GetTickCount();
			m_nBuffTray1Case = 10; m_tBuffTray1Loop.Set_LoopTime(5000);
		}
		break;

	case 10:	// Wait for Module Loading
		if (m_bUnloadLotEnd && Check_BufferEmpty(1)) { m_nBuffTray1Case = 0; m_tBuffTray1Loop.Set_LoopTime(5000); }
		return TRUE;

	case 11:	// Y Axis Move to Sort Picker Position
		if (!m_pDX09->iBufferStage2Up && m_pDX09->iBufferStage2Down) {
			m_tBuffTray1Loop.Takt_Start(nTaktZone, 10);
			m_tBuffTray1Loop.Takt_End(nTaktZone, 10);
			if (m_nBuffTray2Case > 21) {	// Down 이후
				
				m_tBuffTray1Loop.Takt_Start(nTaktZone, 11);
				if (m_nSortPick1Case != 0 && m_nSortPick2Case == 0) g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 3);
				else												g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 2);
				m_nBuffTray1Case = 15; m_tBuffTray1Loop.Set_LoopTime(5000);
			}

		} else {
			if (m_nBuffTray2Case == 20) {
				m_nBuffTray1Case++; m_tBuffTray1Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 12:	// Move to Sort Unload Wait Position
		if (g_objAJinAXL.Is_Done(AX_BUFFER_STAGE1_Y)) {
			g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 5);
			m_nBuffTray1Case++; m_tBuffTray1Loop.Set_LoopTime(5000);
			m_tBuffTray1Loop.Takt_Start(nTaktZone, 12);
		}
		break;
	case 13:	// Position Check & Move to Sort Picker Position
		if (!m_pDX09->iBufferStage1Rotate && m_pDX09->iBufferStage1Normal &&
			g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 5))
		{
			if (!m_pDX09->iBufferStage2Up && m_pDX09->iBufferStage2Down) {
				if (m_nBuffTray2Case > 21) {	// Down 이후
					m_tBuffTray1Loop.Takt_End(nTaktZone, 12);
					m_tBuffTray1Loop.Takt_Start(nTaktZone, 13);
					
					if (m_nSortPick1Case != 0 && m_nSortPick2Case == 0) g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 3);
					else												g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 2);
					m_nBuffTray1Case = 15; m_tBuffTray1Loop.Set_LoopTime(5000);
				} 

			} else {
				if (m_nBuffTray2Case == 20) { 
					m_tBuffTray1Loop.Set_LoopTime(5000); return TRUE;
				}
			}
		}
		break;

	case 15:	// Position Check
		if (!m_pDX09->iBufferStage1Rotate && m_pDX09->iBufferStage1Normal &&
			(g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 2) || g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 3)))
		{
			m_tBuffTray1Loop.Takt_End(nTaktZone, 11);
			m_tBuffTray1Loop.Takt_End(nTaktZone, 13);
			m_nBuffTray1Case = 20; m_tBuffTray1Loop.Set_LoopTime(30000);	// 30초
		}
		break;

	case 20:	// Wait for Module Unloading
		return TRUE;

	case 21:	// Z Cylinder Down
		if (m_pDX09->iBufferStage1Up) {
			double dB1Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE1_Y);
			double dB2Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE2_Y);
			double dDiff = fabs(dB1Y-dB2Y);
			if (dDiff > TRAY_WIDTH) {	// Buffer Stage2 Y Position Check
				m_tBuffTray1Loop.Takt_Start(nTaktZone, 2);
				m_pDY09->oBufferStage1Up = FALSE; m_pDY09->oBufferStage1Down = TRUE;
				g_objAJinAXL.Write_Output(9);
				m_nBuffTray1Case++; m_tBuffTray1Loop.Set_LoopTime(5000);
				m_tBuffTray1Loop.Takt_Start(nTaktZone, 20);
				m_tBuffTray1Loop.Takt_End(nTaktZone, 20);
				m_tBuffTray1Loop.Takt_Start(nTaktZone, 21);
			}
		}
		break;
	case 22:	// Down Check
		if (!m_pDX09->iBufferStage1Up && m_pDX09->iBufferStage1Down &&
			!m_pDX09->iBufferStage1Rotate && m_pDX09->iBufferStage1Normal)
		{
			m_tBuffTray1Loop.Takt_End(nTaktZone, 21);
			m_nBuffTray1Case = 50; m_tBuffTray1Loop.Set_LoopTime(5000);
		}
		break;

	case 50:	// Wait
		if (m_nBuffTray2Case > 2) { m_nBuffTray1Case++; m_tBuffTray1Loop.Set_LoopTime(5000); }
		return TRUE;

	case 51:	// Y Axis Move to Module Loading Position
		if ( m_pDX09->iBufferStage2Up && !m_pDX09->iBufferStage2Down  &&
			!m_pDX09->iBufferStage1Rotate && m_pDX09->iBufferStage1Normal) {	// Inter lock

			m_tBuffTray1Loop.Takt_Start(nTaktZone, 3);
			g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 0);
			m_nBuffTray1Case++; m_tBuffTray1Loop.Set_LoopTime(5000);
			m_tBuffTray1Loop.Takt_Start(nTaktZone, 50);
			m_tBuffTray1Loop.Takt_End(nTaktZone, 50);
			m_tBuffTray1Loop.Takt_Start(nTaktZone, 51);
		}
		break;
	case 52:	// Z Cylinder Up
		if (g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 0)) {
			if (m_nBuffTray2Case >= 20 && m_nBuffTray2Case < 50) {
				double dB1Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE1_Y);
				double dB2Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE2_Y);
				double dDiff = fabs(dB1Y-dB2Y);

				m_tBuffTray1Loop.Takt_End(nTaktZone, 51);
				m_tBuffTray1Loop.Takt_Start(nTaktZone, 52);
				m_tBuffTray1Loop.Takt_End(nTaktZone, 52);
				if (dDiff > TRAY_WIDTH) {	// Buffer Stage2 Y Position Check
					m_pDY09->oBufferStage1Up = TRUE; m_pDY09->oBufferStage1Down = FALSE;
					g_objAJinAXL.Write_Output(9);
					m_nBuffTray1Case = 60; m_tBuffTray1Loop.Set_LoopTime(5000);
				}
			} else if (m_nBuffTray2Case == 0 && m_bUnloadLotEnd) {
				m_nBuffTray1Case = 80; m_tBuffTray1Loop.Set_LoopTime(5000);	// 후처리
			}
		}
		return TRUE;
	case 60:	// Lot End Check
		if (m_pDX09->iBufferStage1Up && !m_pDX09->iBufferStage1Down) {
			m_tBuffTray1Loop.Takt_Start(nTaktZone, 60);
			m_tBuffTray1Loop.Takt_End(nTaktZone, 60);
			m_strLog.Format("Buffer Stage1, %d", GetTickCount() - m_dwBuffTray1);
			g_objLogFile.Save_TestLog(m_strLog);
			if (m_bUnloadLotEnd) { m_nBuffTray1Case = 0; m_tBuffTray1Loop.Set_LoopTime(5000); }
			else				 { m_nBuffTray1Case = 1; m_tBuffTray1Loop.Set_LoopTime(5000); }
		}
		break;

	case 80:	// 후처리
		if (g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 0)) {
			if (!m_pDX09->iBufferStage1Up && m_pDX09->iBufferStage1Down &&
				 m_pDX09->iBufferStage2Up && !m_pDX09->iBufferStage2Down) {

				g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 2);
			}
			m_nBuffTray1Case++; m_tBuffTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 81:
		if (g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 2)) {
			g_objAJinAXL.Write_Output(9);
			m_nBuffTray1Case = 50; m_tBuffTray1Loop.Set_LoopTime(5000);
		}
		break;
	}

	// 11. (Error : 4100)
	if (m_tBuffTray1Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(4100 + m_nBuffTray1Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 12. (Error : 4200)
BOOL CSequenceMain::BufferTray2_Run()
{
	int nTaktZone = 12;		// Takt_Start, Takt_End

	switch (m_nBuffTray2Case) {
	case 0:		// Start시 1로 바뀜
		return TRUE;

	case 1:		// 안전확인.
		if (m_bUnloadLotEnd) {
			m_nBuffTray2Case = 0;

		} else if (!m_pDX09->iBufferStage2Rotate &&  m_pDX09->iBufferStage2Normal &&
			g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 0)) {
			m_nBuffTray2Case++; m_tBuffTray2Loop.Set_LoopTime(5000);
			m_tBuffTray2Loop.Takt_Start(nTaktZone, 0);
			m_tBuffTray2Loop.Takt_End(nTaktZone, 0);
		}
		break;
	case 2:		// Tray Check
		if (1) {
			m_dwBuffTray2 = GetTickCount();
			m_nBuffTray2Case = 10; m_tBuffTray2Loop.Set_LoopTime(5000);
		}
		break;

	case 10:	// Wait for Module Loading
		if (m_bUnloadLotEnd && Check_BufferEmpty(2)) { m_nBuffTray2Case = 0; m_tBuffTray2Loop.Set_LoopTime(5000); }
		return TRUE;

	case 11:	// Y Axis Move to Sort Picker Position
		if (!m_pDX09->iBufferStage1Up && m_pDX09->iBufferStage1Down) {
			m_tBuffTray2Loop.Takt_Start(nTaktZone, 10);
			m_tBuffTray2Loop.Takt_End(nTaktZone,10);
			if (m_nBuffTray1Case > 21) {	// Down 이후
				m_tBuffTray2Loop.Takt_Start(nTaktZone, 11);
				if (m_nSortPick1Case != 0 && m_nSortPick2Case == 0) g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 3);
				else												g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 2);
				m_nBuffTray2Case = 15; m_tBuffTray2Loop.Set_LoopTime(5000);
			}

		} else {
			if (m_nBuffTray1Case == 20) {
				m_nBuffTray2Case++; m_tBuffTray2Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 12:	// Move to Sort Unload Wait Position
		if (g_objAJinAXL.Is_Done(AX_BUFFER_STAGE2_Y)) {
			g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 5);
			m_nBuffTray2Case++; m_tBuffTray2Loop.Set_LoopTime(5000);
			m_tBuffTray2Loop.Takt_Start(nTaktZone, 12);
		}
		break;
	case 13:	// Position Check & Move to Sort Picker Position
		if (!m_pDX09->iBufferStage2Rotate && m_pDX09->iBufferStage2Normal &&
			g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 5))
		{
			if (!m_pDX09->iBufferStage1Up && m_pDX09->iBufferStage1Down) {
				if (m_nBuffTray1Case > 21) {	// Down 이후
					m_tBuffTray2Loop.Takt_End(nTaktZone, 12);
					m_tBuffTray2Loop.Takt_Start(nTaktZone, 13);
					if (m_nSortPick1Case != 0 && m_nSortPick2Case == 0) g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 3);
					else												g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 2);
					m_nBuffTray2Case = 15; m_tBuffTray2Loop.Set_LoopTime(5000);
				} 

			} else {
				if (m_nBuffTray1Case == 20) { 
					m_tBuffTray2Loop.Set_LoopTime(5000); return TRUE;
				}
			}
		}
		break;

	case 15:	// Position Check
		if (!m_pDX09->iBufferStage2Rotate && m_pDX09->iBufferStage2Normal &&
			(g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 2) || g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 3)))
		{
			m_tBuffTray2Loop.Takt_End(nTaktZone, 11);
			m_tBuffTray2Loop.Takt_End(nTaktZone, 13);
			m_nBuffTray2Case = 20; m_tBuffTray2Loop.Set_LoopTime(30000);	// 30초
		}
		break;

	case 20:	// Wait for Module Unloading
		return TRUE;

	case 21:	// Z Cylinder Down
		if (m_pDX09->iBufferStage2Up) {
			double dB1Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE1_Y);
			double dB2Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE2_Y);
			double dDiff = fabs(dB1Y-dB2Y);
			if (dDiff > TRAY_WIDTH) {	// Buffer Stage Y Position Check
				m_tBuffTray2Loop.Takt_Start(nTaktZone, 2);
				m_pDY09->oBufferStage2Up = FALSE; m_pDY09->oBufferStage2Down = TRUE;
				g_objAJinAXL.Write_Output(9);
				m_nBuffTray2Case++; m_tBuffTray2Loop.Set_LoopTime(5000);
				m_tBuffTray2Loop.Takt_Start(nTaktZone, 20);
				m_tBuffTray2Loop.Takt_End(nTaktZone, 20);
				m_tBuffTray2Loop.Takt_Start(nTaktZone, 21);
			}
		}
		break;
	case 22:	// Down Check
		if (!m_pDX09->iBufferStage2Up && m_pDX09->iBufferStage2Down &&
			!m_pDX09->iBufferStage2Rotate &&  m_pDX09->iBufferStage2Normal)
		{
			m_tBuffTray2Loop.Takt_End(nTaktZone, 21);
			m_nBuffTray2Case = 50; m_tBuffTray2Loop.Set_LoopTime(5000);
		}
		break;

	case 50:	// Wait
		if (m_nBuffTray1Case > 2) { m_nBuffTray2Case++; m_tBuffTray2Loop.Set_LoopTime(5000); }
		return TRUE;

	case 51:	// Y Axis Move to Module Loading Position
		if (m_pDX09->iBufferStage1Up && !m_pDX09->iBufferStage1Down  &&
			!m_pDX09->iBufferStage2Rotate && m_pDX09->iBufferStage2Normal) {	// Inter lock

			g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 0);
			m_nBuffTray2Case++; m_tBuffTray2Loop.Set_LoopTime(5000);
			m_tBuffTray2Loop.Takt_Start(nTaktZone, 50);
			m_tBuffTray2Loop.Takt_End(nTaktZone, 50);
			m_tBuffTray2Loop.Takt_Start(nTaktZone, 51);
		}
		break;
	case 52:	// Z Cylinder Up
		if (g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 0)) {
			if (m_nBuffTray1Case >= 20 && m_nBuffTray1Case < 50) {
				double dB1Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE1_Y);
				double dB2Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE2_Y);
				double dDiff = fabs(dB1Y-dB2Y);

				m_tBuffTray2Loop.Takt_End(nTaktZone, 51);
				m_tBuffTray2Loop.Takt_Start(nTaktZone, 52);
				m_tBuffTray2Loop.Takt_End(nTaktZone, 52);
				if (dDiff > TRAY_WIDTH) {	// Buffer Stage Y Position Check
					m_pDY09->oBufferStage2Up = TRUE; m_pDY09->oBufferStage2Down = FALSE;
					g_objAJinAXL.Write_Output(9);
					m_nBuffTray2Case = 60; m_tBuffTray2Loop.Set_LoopTime(5000);
				}
			} else if (m_nBuffTray1Case == 0 && m_bUnloadLotEnd) {
				m_nBuffTray2Case = 80; m_tBuffTray2Loop.Set_LoopTime(5000);	// 후처리
			}
		}
		return TRUE;
	case 60:	// Lot End Check
		if (m_pDX09->iBufferStage2Up && !m_pDX09->iBufferStage2Down) {
			m_tBuffTray2Loop.Takt_Start(nTaktZone, 60);
			m_tBuffTray2Loop.Takt_End(nTaktZone, 60);
			m_strLog.Format("Buffer Stage2, %d", GetTickCount() - m_dwBuffTray2);
			g_objLogFile.Save_TestLog(m_strLog);
			if (m_bUnloadLotEnd) { m_nBuffTray2Case = 0; m_tBuffTray2Loop.Set_LoopTime(5000); }
			else				 { m_nBuffTray2Case = 1; m_tBuffTray2Loop.Set_LoopTime(5000); }
		}
		break;

	case 80:	// 후처리
		if (g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 0)) {
			if (m_pDX09->iBufferStage1Up && !m_pDX09->iBufferStage1Down &&
				!m_pDX09->iBufferStage2Up && m_pDX09->iBufferStage2Down) {

				g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 2);
			}
			m_nBuffTray2Case++; m_tBuffTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 81:
		if (g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 2)) {
			g_objAJinAXL.Write_Output(9);
			m_nBuffTray2Case = 50; m_tBuffTray2Loop.Set_LoopTime(5000);
		}
		break;
	}

	// 12. (Error : 4200)
	if (m_tBuffTray2Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(4200 + m_nBuffTray2Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 13. (Error : 4300)
BOOL CSequenceMain::SortPicker1_Run()
{
	int nTaktZone = 13;		// Takt_Start, Takt_End
	static int		nSp1WorkBuff = 0;
	static int		nSp1WorkGood = 0;
	static int		nSp1WorkNg = 0;
	static int		nSp1TrayPosX = 0, nSp1TrayPosY = 0;
	static double	dSp1X = 0.0, dSp1Y = 0.0;
	static int		nSp1TrayCnt = 0, nSp1PickCnt = 0, nSp1StartNo = 0, nSp1DownSu = 0, nSp1Base = 0, nSp1PNo = 0;
	static int		nSp1VacErr = 0;
	static BOOL		bLastNgBuffPick1 = FALSE;
	BOOL bS1Checking = FALSE;							// Inspect Done Check
	static DWORD	dwSortPick1Unload = 0;
	static DWORD	dwSortPick1End = 0;

	switch (m_nSortPick1Case) {
	case 0:		// Wait
		if (m_nBuffTray1Case == 20 || m_nBuffTray2Case == 20) {	// Unload 위치
			if (!Check_NgBufferEmpty(1) && Check_SortPickerEmpty(1)) {
				if (m_nBuffTray1Case == 20) nSp1WorkBuff = 1;
				if (m_nBuffTray2Case == 20) nSp1WorkBuff = 2;
				if (gData.nPNoNgTray != 0 && (gData.nPNoNgTray != gData.nPNoBuffTray[nSp1WorkBuff-1])) {
					bLastNgBuffPick1 = TRUE;
					m_nSortPick1Case = 50; m_tSortPick1Loop.Set_LoopTime(10000);
					break;
				}
			}
			if (m_nSortPick2Case == 0 || (m_nSortPick2Case >= 6 && m_nSortPick2Case < 30) || (m_nSortPick2Case >= 40 && m_nSortPick2Case < 65)) {	// Sort2가 Buffer 대기중 이거나 NG or Good Unload 작업중.
				m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(5000);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 0, TRUE);
				m_tSortPick1Loop.Takt_End(nTaktZone, 0);
			}
		} else {
			if (!gData.bNGTrayWait && !Check_NgBufferEmpty(1) && Check_SortPickerEmpty(1)) {
				m_nSortPick1Case = 50; m_tSortPick1Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;

	case 1:		// Select Pick Position
		if ((g_objCommon.Check_Position(AX_SORT_PICKER1_X, 0) || g_objCommon.Check_Position(AX_SORT_PICKER1_X, 1)) &&
			g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER1_P, 0)) {
			if (m_nBuffTray1Case == 20 || m_nBuffTray2Case == 20) {
				if (m_nBuffTray1Case == 20) nSp1WorkBuff = 1;
				if (m_nBuffTray2Case == 20) nSp1WorkBuff = 2;
				if (nSp1WorkBuff != 1 && nSp1WorkBuff != 2) break;	// Test Interlock
				if ((nSp1WorkBuff == 1 && !g_objAJinAXL.Is_Done(AX_BUFFER_STAGE1_Y)) ||
					(nSp1WorkBuff == 2 && !g_objAJinAXL.Is_Done(AX_BUFFER_STAGE2_Y))) break;

				gData.bBuffStageMove = TRUE;
				if (Check_SortPickerEmpty(1)) {
					m_dwSortPick1 = GetTickCount();
					
					if (!Select_BufferTrayUpPos(nSp1WorkBuff, nSp1TrayPosX, nSp1TrayPosY)) nSp1TrayPosX = 0;	// Good Tray 교체중...

					m_strLog.Format("MCC,(13) SortPicker1, X & buffer Y Move Start");
					g_objLogFile.Save_MCCLog(m_strLog);

					nSp1Base = 0; dSp1X = m_pMoveData->dSortPicker1X[nSp1WorkBuff-1];
					g_objAJinAXL.Move_Absolute(AX_SORT_PICKER1_X, dSp1X);

					if (nSp1WorkBuff == 1 && m_nBuffTray1Case == 20) { 
						dSp1Y = m_pMoveData->dBufferStage1Y[2] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dSp1Y);
					}
					if (nSp1WorkBuff == 2 && m_nBuffTray2Case == 20) {
						dSp1Y = m_pMoveData->dBufferStage2Y[2] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dSp1Y);
					}
					nSp1VacErr = 0;
					m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
					m_tSortPick1Loop.Takt_Start(nTaktZone, 1);
				}
			}
		}
		break;
	case 2:		// Picker Down
		if (g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER1_X, dSp1X) &&
			((nSp1WorkBuff == 1 && g_objAJinAXL.Is_MoveDone(AX_BUFFER_STAGE1_Y, dSp1Y)) ||
			 (nSp1WorkBuff == 2 && g_objAJinAXL.Is_MoveDone(AX_BUFFER_STAGE2_Y, dSp1Y))) ) {

			if (nSp1VacErr == 0)
			

			if (nSp1WorkBuff == 1) g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 1);	//Buffer 1 Down
			if (nSp1WorkBuff == 2) g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 5);	//Buffer 2 Down
			g_objCommon.Set_SortPicker1Down(0);	// All Down
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
			m_tSortPick1Loop.Takt_End(nTaktZone, 1);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 2);
		} 
		break;
	case 3:		// 정보전달, Picker Vac On
		if (((nSp1WorkBuff == 1 && g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 1)) ||
			 (nSp1WorkBuff == 2 && g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 5))) &&
			g_objCommon.Get_SortPicker1Down(0))
		{
			if (nSp1VacErr == 1) { m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000); break; }

			for (int i = 0; i < gData.nSortPickQt; i++) {
				gData.InfoSortPick[0][i] = gData.InfoBuffTray[nSp1WorkBuff-1][nSp1TrayPosY][nSp1Base+i]; 
				gData.InfoBuffTray[nSp1WorkBuff-1][nSp1TrayPosY][nSp1Base+i] = 0;

				gData.nTNoSortPick[0][i] = gData.nTNoBuffTray[nSp1WorkBuff-1][nSp1TrayPosY][nSp1Base+i];
				gData.nTNoBuffTray[nSp1WorkBuff-1][nSp1TrayPosY][nSp1Base+i] = 0;
				gData.nCNoSortPick[0][i] = gData.nCNoBuffTray[nSp1WorkBuff-1][nSp1TrayPosY][nSp1Base+i];
				gData.nCNoBuffTray[nSp1WorkBuff-1][nSp1TrayPosY][nSp1Base+i] = 0;

				//모듈트랙킹
				int nPNo = gData.nPNoBuffTray[nSp1WorkBuff-1] - 1;
				int nTNo = gData.nTNoSortPick[0][i] - 1;
				int nCNo = gData.nCNoSortPick[0][i] - 1;
				if (nCNo > -1) {
					gData.nCmJigNo[nPNo][nTNo][nCNo][SORT_PICK_NO] = 1;		// Sort Picker No.
					gData.nCmJigNo[nPNo][nTNo][nCNo][SORT_JIG_NO]  = i + 1;	// Sort Picker Jig No.
				}
			}
			gData.nPNoSortPick[0] = gData.nPNoBuffTray[nSp1WorkBuff-1];

			if (gData.nTNoSortPick[0][0] == 1 &&  gData.nCNoSortPick[0][0] == 9) {
				gLot.dwUphStart = GetTickCount();
			}

			if ((gData.nCNoSortPick[0][0] == 1 || gData.nCNoSortPick[0][0] == 5 || gData.nCNoSortPick[0][0] == 9) && nSp1TrayPosY == 0) {
				if (gData.nTNoSortPick[0][0] == 1 && gData.nCNoSortPick[0][0] == 1) {
					m_dwULCycleTime = 0;
					m_dwULPickUpTime = GetTickCount();
					
					if (m_pEquipData->bUseInlineMode) g_objCapAttach.Set_LotStart(gData.nPNoSortPick[0]);

				} else {
					m_dwULCycleTime = GetTickCount() - m_dwULPickUpTime;
					m_dwULPickUpTime = GetTickCount();
					g_dlgWork.PostMessage(UM_UPDATE_UPH, 1, NULL);
				}
			}

			if (Check_BufferEmpty(nSp1WorkBuff)) {
				gData.nPNoBuffTray[nSp1WorkBuff-1] = 0;
			}
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 3, nSp1WorkBuff-1);

			g_objCommon.Set_InfoSortPicker1Close();

			//MCC
			m_strLog.Format("Sort Picker 1 Pick Complete, BuffNo : %d", nSp1WorkBuff);
			g_objLogFile.Save_MCCLog(m_strLog);

			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
			m_tSortPick1Loop.Takt_End(nTaktZone, 2);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 3);
		} 
		break;
	case 4:		// Picker Up
		if (g_objCommon.Get_InfoSortPicker1Close()) {	// Delay Time만 사용한다.
			if (!m_tSortPick1Loop.Waiting_Time(m_pEquipData->nDelayAdd[2])) break;	// SortPicker1 Delay

			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	//Ready Up
			g_objCommon.Set_SortPicker1Up(0);	// All Up
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(6000);
			m_tSortPick1Loop.Takt_End(nTaktZone, 3);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 4);
		}
		break;
	case 5:		
		if (g_objCommon.Get_InfoSortPicker1Close() && g_objCommon.Get_InfoSortPicker1Check() &&
			g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Get_SortPicker1Up(0))
		{
			
			if (Check_BufferEmpty(nSp1WorkBuff)) {	// Tray Change
				nSp1TrayPosY = 0;
				gData.nBtWorkSortPickNo = 1;
				if (nSp1WorkBuff == 1 && m_nBuffTray1Case == 20) m_nBuffTray1Case = 21;
				if (nSp1WorkBuff == 2 && m_nBuffTray2Case == 20) m_nBuffTray2Case = 21;
			}
			gData.bBuffStageMove = FALSE;
			gData.nSp1Timer = GetTickCount();
			SYSTEMTIME time;
			GetLocalTime(&time);
			gData.sSortWaitStartTime[0].Format("%02d:%02d:%02d %03d", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(30000);	// 검사 완료
			m_tSortPick1Loop.Takt_End(nTaktZone, 4);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 5);
			gData.dwSkipTime_Sort1 = GetTickCount();
		} 
		break;
	case 6:		// NG or Good
		for (int i = 4; i >= 0; i--) { if (!Check_InspectDone(gData.nPNoSortPick[0], gData.nTNoSortPick[0][i], gData.nCNoSortPick[0][i], gData.InfoSortPick[0][i], 1)) { bS1Checking = TRUE; break; } }
		if (bS1Checking) break;	// Not Complete

		nSp1PNo = gData.nPNoSortPick[0];
		for (int i = 0; i < 5; i++) {
			if (gData.InfoSortPick[0][i] == 1 || gData.InfoSortPick[0][i] == 9) gLot.nGoodCount[nSp1PNo-1]++;
			else if (gData.InfoSortPick[0][i] != 0 ) gLot.nNgCount[nSp1PNo-1]++;
			for (int j = 0; j < 6; j++) {
				if (gData.InfoSortPick[0][i] == j + 3)	gLot.nSNgCount[nSp1PNo-1][j]++;	// S-NG (3,4,5,6,7,8)
			}
			Write_LotJudge(nSp1PNo, gData.nTNoSortPick[0][i], gData.nCNoSortPick[0][i], gData.InfoSortPick[0][i], 1);
		}
		g_dlgWork.PostMessage(UM_VISION_RESULT, nSp1PNo, NULL);

		dwSortPick1Unload = GetTickCount();	// Sort Picker1 NG, Good Unloading Start Time

		m_tSortPick1Loop.Takt_End(nTaktZone, 5);
		m_tSortPick1Loop.Takt_Start(nTaktZone, 6);
		m_tSortPick1Loop.Takt_End(nTaktZone, 6);
		//	NG가 있으면 무조건 NG 부터 간다.
		if (Select_SortPickNgPos(1, nSp1StartNo, nSp1PickCnt)) {	// NG
			m_nSortPick1Case = 7; m_tSortPick1Loop.Set_LoopTime(10000);
		} else {
			if (Select_SortPickGoodPos(1, nSp1StartNo, nSp1PickCnt)) {	// NG 없고 Good만 있을때
				m_nSortPick1Case = 17; m_tSortPick1Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 7:	// Wait Move to Ng Position
		// SortPicker2번이 Pickup 작업중, Good Unload 작업중,  NG Buffer 작업중이면 NG로 갈수있다.
		if ((m_nSortPick2Case >=  0 && m_nSortPick2Case < 10) ||
			(m_nSortPick2Case >= 19 && m_nSortPick2Case < 30) ||
			(m_nSortPick2Case >= 40 && m_nSortPick2Case < 60) ||
			//(m_nSortPick2Case >= 60 && m_nSortPick2Case < 70) ||	// APD 확인중에도 갈수있다.
			(m_nSortPick2Case == 12) && (gData.nPNoSortPick[0] != gData.nPNoSortPick[1])||
			(m_nSortPick2Case == 17)) {

			if ((gData.nPNoSortPick[0] != gData.nPNoNgTray) && (gData.nPNoSortPick[0] != 0)) {
				if (gData.nPNoNgTray != 0) return TRUE;
			}
			m_nSortPick1Case = 10; m_tSortPick1Loop.Set_LoopTime(10000);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 7);
			m_tSortPick1Loop.Takt_End(nTaktZone, 7);

		} else {
			if (Select_SortPickGoodPos(1, nSp1StartNo, nSp1PickCnt)) {		// Sort2번이 NG 작업 중이고 Good이 있다면 Good Unload 먼저 해준다.
				m_nSortPick1Case = 17; m_tSortPick1Loop.Set_LoopTime(10000);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 7);
				m_tSortPick1Loop.Takt_End(nTaktZone, 7);
			} else if (m_nSortPick2Case >= 11 && m_nSortPick2Case < 20) {	// Sort2번이 NG 작업 중이고 NG만 있다면 NG Tray 1번 위치로 가서 대기한다.
				m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 7);
				m_tSortPick1Loop.Takt_End(nTaktZone, 7);
			}
		}
		return TRUE;

	case 8:	// Move to NG Tray1 Position
		if (g_objAJinAXL.Is_Done(AX_SORT_PICKER1_X)) {
			if (!(g_objCommon.Get_InfoSortPicker1Close() && g_objCommon.Get_InfoSortPicker1Check())) break;
			g_objCommon.Move_Position(AX_SORT_PICKER1_X, 4);
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 8);
		}
		break;
	case 9:	// Check Position & Sort2
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_X, 4)) {
			// SortPicker2번이 Pickup 작업중, Good Unload 작업중,  NG Buffer 작업중이면 NG Unloading 할수있다.
			if ((m_nSortPick2Case >=  0 && m_nSortPick2Case < 10) ||
				(m_nSortPick2Case >= 19 && m_nSortPick2Case < 30) ||
				(m_nSortPick2Case >= 40 && m_nSortPick2Case < 60) ||
				//(m_nSortPick2Case >= 60 && m_nSortPick2Case < 70) ||	// APD 확인중에도 갈수있다.
				(m_nSortPick2Case == 17)) {

				if ((gData.nPNoSortPick[0] != gData.nPNoNgTray) && (gData.nPNoSortPick[0] != 0)) {
					if (gData.nPNoNgTray != 0) return TRUE; 
				}
				m_nSortPick1Case = 10; m_tSortPick1Loop.Set_LoopTime(10000);
				m_tSortPick1Loop.Takt_End(nTaktZone, 8);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 9);
			} else {
				return TRUE;
			}
		}
		break;

	// NG Unload
	case 10:	// Check NG Tray
		// NG Tray가 받을 준비 됐는지 확인한다.
		if (m_nNgTrayCase == 10) {
			m_nSortPick1Case = 12; m_tSortPick1Loop.Set_LoopTime(10000); 
				m_tSortPick1Loop.Takt_End(nTaktZone, 9);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 10);
			m_tSortPick1Loop.Takt_End(nTaktZone, 10);

		} else {
			if (g_objAJinAXL.Is_Done(AX_SORT_PICKER1_Z) && g_objAJinAXL.Is_Done(AX_SORT_PICKER1_X)) {
				g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up

				if (Select_SortPickNgPos(1, nSp1StartNo, nSp1PickCnt)) {	// startNo 0base pickCnt 1base
					if (bLastNgBuffPick1 == FALSE && !Check_UnloadLotEnd(1) && !Check_NgBufferFull(1)) { // Ng Tray 교체 작업 중에 Ng Buffer에 내려놓는다.
						m_nSortPick1Case = 40; m_tSortPick1Loop.Set_LoopTime(10000);	
							m_tSortPick1Loop.Takt_End(nTaktZone, 9);
						m_tSortPick1Loop.Takt_Start(nTaktZone, 10);
						m_tSortPick1Loop.Takt_End(nTaktZone, 10);

					} else {
						if (!(g_objCommon.Get_InfoSortPicker1Close() && g_objCommon.Get_InfoSortPicker1Check())) break;
						g_objCommon.Move_Position(AX_SORT_PICKER1_X, 5);
						m_nSortPick1Case = 11; m_tSortPick1Loop.Set_LoopTime(10000);
							m_tSortPick1Loop.Takt_End(nTaktZone, 9);
						m_tSortPick1Loop.Takt_Start(nTaktZone, 10);
						m_tSortPick1Loop.Takt_End(nTaktZone, 10);
					}
				} else {
					m_nSortPick1Case = 17; m_tSortPick1Loop.Set_LoopTime(10000);
						m_tSortPick1Loop.Takt_End(nTaktZone, 9);
						m_tSortPick1Loop.Takt_Start(nTaktZone, 10);
						m_tSortPick1Loop.Takt_End(nTaktZone, 10);
				}
			}
		}
		break;
	case 11:	// Check Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_X, 5)) {
			if (m_nNgTrayCase == 10) { 
				m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 11);
			}
		}
		return TRUE;

	case 12:	// Move to NG Unload Position
		if ((gData.nPNoSortPick[0] != gData.nPNoNgTray) && (gData.nPNoSortPick[0] != 0)) {
			if (gData.nPNoNgTray != 0) return TRUE;
		}
		if (m_nNgTrayCase != 10) { m_nSortPick1Case = 10; m_tSortPick1Loop.Set_LoopTime(10000); return TRUE; }
		if (bLastNgBuffPick1 == TRUE) bLastNgBuffPick1 = FALSE;

		if (Select_SortPickNgPos(1, nSp1StartNo, nSp1PickCnt)) {	// startNo 0base pickCnt 1base
			if (Select_NgTrayPos(1, nSp1WorkNg, nSp1TrayPosX, nSp1TrayPosY)) {
				// 한Case에서 오는게 아니라 따로 추출해서 입력해준다.
						
				if (!Select_NgTrayPos(1, nSp1WorkNg, nSp1TrayPosX, nSp1TrayPosY)) nSp1TrayPosX = 0;	// Tray 교체중...
				nSp1TrayCnt = gData.nTrayX - nSp1TrayPosX;
				nSp1DownSu = ((nSp1PickCnt < nSp1TrayCnt) ? nSp1PickCnt : nSp1TrayCnt);
				m_nSortPick1MultiCnt = nSp1DownSu;

				int nNgTrayY = nSp1WorkNg / 2;
				int nNgTrayX = nSp1WorkNg % 2;
				dSp1X = m_pMoveData->dSortPicker1X[4 + nNgTrayX] + (nSp1TrayPosX - nSp1StartNo) * m_pEquipData->dTrayPitchX;
				dSp1Y = m_pMoveData->dNgStageY[1 + nNgTrayY] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;

				g_objAJinAXL.Move_Absolute(AX_SORT_PICKER1_X, dSp1X);
				g_objAJinAXL.Move_Absolute(AX_NG_STAGE_Y, dSp1Y);
				m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
				m_tSortPick1Loop.Takt_End(nTaktZone,11);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 12);
				m_tSortPick1Loop.Takt_End(nTaktZone,12);

			}
		} else {
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
			m_nSortPick1Case = 17; m_tSortPick1Loop.Set_LoopTime(10000);
			m_tSortPick1Loop.Takt_End(nTaktZone,11);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 12);
				m_tSortPick1Loop.Takt_End(nTaktZone,12);
		}
		break;
	case 13:	// Picker Down
		if (g_objAJinAXL.Is_Done(AX_SORT_PICKER1_Z) && g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER1_X, dSp1X) && g_objAJinAXL.Is_MoveDone(AX_NG_STAGE_Y, dSp1Y) && g_objCommon.Check_Position(AX_SORT_PICKER1_P, 0)) {
			
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 3);	//NG Down
			g_objCommon.Set_SortPicker1DownMulti(nSp1StartNo+1, nSp1DownSu);
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 13);
		}
		break;
	case 14:	// 정보전달, Grip Open
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 3) && g_objCommon.Get_SortPicker1DownMulti(nSp1StartNo+1, nSp1DownSu)) {
			for (int i = 0; i < nSp1DownSu; i++) {
				gData.InfoNgTray[nSp1WorkNg][nSp1TrayPosY][nSp1TrayPosX+i] = gData.InfoSortPick[0][nSp1StartNo+i]; 
				gData.InfoSortPick[0][nSp1StartNo+i] = 0;

				CString sInfo, sNgTray; 
				sInfo.Format("%d", gData.InfoNgTray[nSp1WorkNg][nSp1TrayPosY][nSp1TrayPosX+i]);
				int nTNo = gData.nTNoSortPick[0][nSp1StartNo+i];
				int nCNo = gData.nCNoSortPick[0][nSp1StartNo+i];
				if (gMes.sJudge[nSp1PNo-1][nTNo-1][nCNo-1] == "") {
					if		(gData.InfoNgTray[nSp1WorkNg][nSp1TrayPosY][nSp1TrayPosX+i] == 8) gMes.sJudge[nSp1PNo-1][nTNo-1][nCNo-1] = "N4";
					else if (gData.InfoNgTray[nSp1WorkNg][nSp1TrayPosY][nSp1TrayPosX+i] == 7) gMes.sJudge[nSp1PNo-1][nTNo-1][nCNo-1] = "BS";
					else if	(gData.InfoNgTray[nSp1WorkNg][nSp1TrayPosY][nSp1TrayPosX+i] == 6) gMes.sJudge[nSp1PNo-1][nTNo-1][nCNo-1] = "N3";
					else if (gData.InfoNgTray[nSp1WorkNg][nSp1TrayPosY][nSp1TrayPosX+i] == 5) gMes.sJudge[nSp1PNo-1][nTNo-1][nCNo-1] = "N2";
					else																	  gMes.sJudge[nSp1PNo-1][nTNo-1][nCNo-1] = "N1";
				}
				g_objMES.Set_Result(gLot.sLotID[nSp1PNo-1], gMes.sBarID[nSp1PNo-1][nTNo-1][nCNo-1], gMes.sJudge[nSp1PNo-1][nTNo-1][nCNo-1], sInfo, gMes.sNGCode[nSp1PNo-1][nTNo-1][nCNo-1], nTNo, nCNo, 0,0,0,0);
				sNgTray.Format("NG-%d", nSp1WorkNg + 1);
				g_objLogFile.Save_OutTray(sNgTray, 1, nSp1TrayPosX+i, nSp1TrayPosY, gData.nPNoSortPick[0], gData.nTNoSortPick[0][nSp1StartNo+i], gData.nCNoSortPick[0][nSp1StartNo+i]);
				g_objLogFile.Save_CmTrackingLog("NG", nSp1WorkNg, nSp1TrayPosX+i, nSp1TrayPosY, gData.nPNoSortPick[0], gData.nTNoSortPick[0][nSp1StartNo+i], gData.nCNoSortPick[0][nSp1StartNo+i]);

				gData.nTNoSortPick[0][nSp1StartNo+i] = gData.nCNoSortPick[0][nSp1StartNo+i] = 0;
			}
			gData.nULPNo = gData.nPNoNgTray = gData.nPNoSortPick[0];
			if (Check_SortPickerEmpty(1)) gData.nPNoSortPick[0] = 0;
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 5, nSp1WorkNg);

			g_objCommon.Set_SortPicker1OpenMulti(nSp1StartNo+1, nSp1DownSu);
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
			m_tSortPick1Loop.Takt_End(nTaktZone,13);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 14);
		}
		break;
	case 15 :	// Picker Up
		if (g_objCommon.Get_SortPicker1OpenMulti(nSp1StartNo+1, nSp1DownSu)) {
			g_objCommon.Set_SortPicker1Up(0);
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	//Ready Up
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(5000);
			m_tSortPick1Loop.Takt_End(nTaktZone,14);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 15);
		}
		break;
	case 16:	// Position Check
		if (g_objCommon.Get_SortPicker1Up(0) && g_objCommon.Get_InfoSortPicker1Close() 
			&& g_objCommon.Get_InfoSortPicker1Check() && g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0)) 
		{
			
			if (Check_NgTrayFull()) {
				if (m_nNgTrayCase == 10) m_nNgTrayCase = 11;
			}
			m_nSortPick1Case = 10; m_tSortPick1Loop.Set_LoopTime(5000);
			m_tSortPick1Loop.Takt_End(nTaktZone,15);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 16);
		}
		break;
	case 17:	// Wait
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER1_P, 0)) {
			
			// Good Module이 있는지 확인 후 어디로 갈지 정한다.
			if (Select_SortPickGoodPos(1, nSp1StartNo, nSp1PickCnt)) {
				if ((m_nSortPick2Case >=  0 && m_nSortPick2Case < 10) ||
					(m_nSortPick2Case >=  9 && m_nSortPick2Case < 20) ||
					(m_nSortPick2Case >= 40 && m_nSortPick2Case < 60) ||
					//(m_nSortPick2Case >= 60 && m_nSortPick2Case < 70) ||	// APD 확인중에도 갈수있다.
					(m_nSortPick2Case == 22) && (gData.nPNoSortPick[0] != gData.nPNoSortPick[1])||
					(m_nSortPick2Case == 27)) {

					if ((gData.nPNoSortPick[0] != gData.nPNoGoodTray) && (gData.nPNoSortPick[0] != 0)) {
						if (gData.nPNoGoodTray != 0) return TRUE;
					}
					m_nSortPick1Case = 20; m_tSortPick1Loop.Set_LoopTime(10000);
					m_tSortPick1Loop.Takt_End(nTaktZone,16);
					m_tSortPick1Loop.Takt_Start(nTaktZone, 17);
					m_tSortPick1Loop.Takt_End(nTaktZone,17);

				} else if (m_nSortPick2Case >= 21 && m_nSortPick2Case < 30) {
					m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
					m_tSortPick1Loop.Takt_End(nTaktZone,16);
					m_tSortPick1Loop.Takt_Start(nTaktZone, 17);
					m_tSortPick1Loop.Takt_End(nTaktZone,17);
				}
			} else {
				if (Check_InspectLotEnd(gData.nPNoNgTray, 1)) {
					if (Check_NgBufferLotEnd(gData.nPNoNgTray)) {	// NG Buffer에 모듈이 없으면 LotEnd 모듈이 있으면 포트 넘버 확인 후 언로딩 작업.
						if (m_pEquipData->bUseApdAlarm) {	// 2023.05.11+
							m_nSortPick1Case = 60; m_tSortPick1Loop.Set_LoopTime(10000); break;	// LotEnd Case
						}

						if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
						if(!m_tSortPick1Loop.Waiting_Time(300)) break;

						if (!Check_GoodTrayEmpty() && (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20)) {
							if (m_nGoodTray1Case == 20) nSp1WorkGood = 1;
							if (m_nGoodTray2Case == 20) nSp1WorkGood = 2;

							gData.sCLotID[nSp1WorkGood-1] = gData.sLotID[gData.nPNoGoodTray-1];
							gData.nCPortNo[nSp1WorkGood-1] = gData.nPNoGoodTray;
							gData.nCGoodTrayCount[nSp1WorkGood-1] = gData.nGoodStageTrayNo[nSp1WorkGood-1];	//gData.nGoodTrayCount;
							gData.nLastTrayNo = gData.nGoodStageTrayNo[nSp1WorkGood-1];
							gData.nCTrayCmCnt[nSp1WorkGood-1] = Get_GoodTrayCmCnt();
						}

						Job_LotEnd(gData.nPNoNgTray, nSp1WorkGood);
						if (m_pThreadBeep == NULL && m_pThreadNgFullBeep == NULL) {
							m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));

						} else {
							gData.bLotEndBeep = TRUE;
						}

						gData.bContinueLotEnd = TRUE;
						if (m_nNgTrayCase == 10) m_nNgTrayCase = 11; 
						if (!Check_GoodTrayEmpty()) {
							if (m_nGoodTray1Case == 20) { gData.bGoodTrayLotEnd[0] = TRUE; m_nGoodTray1Case = 21; }
							if (m_nGoodTray2Case == 20) { gData.bGoodTrayLotEnd[1] = TRUE; m_nGoodTray2Case = 21; }
						} else {
							// 랏종료시점에 대기중인 트레이가 빈트레이면 다음랏 모듈을 받아야 하기 때문에 트레이넘버를 1로 바꿔준다.
							if (m_nGoodTray1Case == 20) { gData.nGoodStageTrayNo[0] = 1; gData.nGoodTrayCount = 1; }
							if (m_nGoodTray2Case == 20) { gData.nGoodStageTrayNo[1] = 1; gData.nGoodTrayCount = 1; }

							// Inline Mode에서는 Unload 버튼을 사용하지 않는다.
							if (!m_pEquipData->bUseInlineMode) {
								if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
								if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case >  30) { gData.bGoodTrayWait = TRUE; }
								if (m_nGoodTray1Case >  30 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
							}
						}
						if (!gData.bGoodTrayLotEnd[0] && m_nGoodTray1Case > 20 && m_nGoodTray1Case < 30) { gData.bGoodTrayLotEnd[0] = TRUE; }
						if (!gData.bGoodTrayLotEnd[1] && m_nGoodTray2Case > 20 && m_nGoodTray2Case < 30) { gData.bGoodTrayLotEnd[1] = TRUE; }

					} else {
						if (!Check_NgBufferEmpty(1) && Check_SortPickerEmpty(1)) {
							bLastNgBuffPick1 = TRUE;
							m_nSortPick1Case = 50; m_tSortPick1Loop.Set_LoopTime(10000);
							m_tSortPick1Loop.Takt_End(nTaktZone,16);
							m_tSortPick1Loop.Takt_Start(nTaktZone, 17);
							m_tSortPick1Loop.Takt_End(nTaktZone,17);
							break;
						}
					}
				}
				if (Check_UnloadLotEnd() && !m_bUnloadLotEnd) { m_bUnloadLotEnd = TRUE; }

				m_strLog.Format("Sort Picker1, %d", GetTickCount() - m_dwSortPick1);
				g_objLogFile.Save_TestLog(m_strLog);

				CString strLog;
				CString strMsg = "Sort picker1 move to NG unloading + good unloading";
				dwSortPick1End = GetTickCount() - dwSortPick1Unload;
				strLog.Format("MCC,(%02d) %s,(%02d) %s,%0.3lf", 13, "SortPicker1", 13, strMsg, dwSortPick1End / 1000.0);
				g_objLogFile.Save_MCCLog(strLog);//g_objLogFile.Save_HandlerLog(strLog);

				if (nSp1WorkBuff != 1 && nSp1WorkBuff != 2) nSp1WorkBuff = 1;	// Test
				if (nSp1WorkBuff == 1) g_objCommon.Move_Position(AX_SORT_PICKER1_X, 0);	// Buffer1 Position
				if (nSp1WorkBuff == 2) g_objCommon.Move_Position(AX_SORT_PICKER1_X, 1);	// Buffer2 Position
				//g_objCommon.Set_SortPicker1Down(0);	// 복귀할때 실린더 다운해준다.

				if (m_nBuffTray1Case == 20)		 nSp1WorkBuff = 1;
				else if (m_nBuffTray2Case == 20) nSp1WorkBuff = 2;
				else							 nSp1WorkBuff = 0;

				if (!gData.bBuffStageMove && nSp1WorkBuff != 0 && Select_BufferTrayUpPos(nSp1WorkBuff, nSp1TrayPosX, nSp1TrayPosY)){	// 복귀할때 Y축 움직일수 있으면 움직여 주자.

					if (nSp1WorkBuff == 1 && m_nBuffTray1Case == 20) { 
						dSp1Y = m_pMoveData->dBufferStage1Y[2] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dSp1Y);
					}
					if (nSp1WorkBuff == 2 && m_nBuffTray2Case == 20) {
						dSp1Y = m_pMoveData->dBufferStage2Y[2] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dSp1Y);
					}
					gData.bBuffStageMove = TRUE;
				} else {
					if (gData.bBuffStageMove) gData.bBuffStageMove = FALSE;
				}

				m_nSortPick1Case = 0; m_tSortPick1Loop.Set_LoopTime(10000);
				m_tSortPick1Loop.Takt_End(nTaktZone,16);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 17);
				m_tSortPick1Loop.Takt_End(nTaktZone,17);
			}
		}		
		return TRUE;

	case 18:	// Move to NG Tray1 Position
		if (g_objAJinAXL.Is_Done(AX_SORT_PICKER1_X)) {
			g_objCommon.Move_Position(AX_SORT_PICKER1_X, 2);	// Good1
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 18);
		}
		break;
	case 19:	// Check Position & Sort2
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_X, 2)) {	// Good1
			if ((m_nSortPick2Case >=  0 && m_nSortPick2Case < 10) ||
				(m_nSortPick2Case >=  9 && m_nSortPick2Case < 20) ||
				(m_nSortPick2Case >= 40 && m_nSortPick2Case < 60) ||
				//(m_nSortPick2Case >= 60 && m_nSortPick2Case < 70) ||	// APD 확인중에도 갈수있다.
				(m_nSortPick2Case == 27)) {

				if ((gData.nPNoSortPick[0] != gData.nPNoGoodTray) && (gData.nPNoSortPick[0] != 0)) {
					if (gData.nPNoGoodTray != 0) return TRUE;
				}
				m_nSortPick1Case = 20; m_tSortPick1Loop.Set_LoopTime(10000);
				m_tSortPick1Loop.Takt_End(nTaktZone,18);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 19);
			}
		}
		return TRUE;

	// Good Unload
	case 20:	// Check Good Tray
		if (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20) {
			m_nSortPick1Case = 22; m_tSortPick1Loop.Set_LoopTime(10000);
			m_tSortPick1Loop.Takt_End(nTaktZone,19);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 20);
				m_tSortPick1Loop.Takt_End(nTaktZone,20);
		} else {
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
			if (Check_SortPickerEmpty(1)) {				
				m_nSortPick1Case = 27; m_tSortPick1Loop.Set_LoopTime(10000);
				m_tSortPick1Loop.Takt_End(nTaktZone,19);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 20);
				m_tSortPick1Loop.Takt_End(nTaktZone,20);

			} else {
				if (Select_SortPickGoodPos(1, nSp1StartNo, nSp1PickCnt)) {
					if (!(g_objCommon.Get_InfoSortPicker1Close() && g_objCommon.Get_InfoSortPicker1Check())) break;
					// 한Case에서 오는게 아니라 따로 추출해서 입력해준다.
					g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
					g_objCommon.Move_Position(AX_SORT_PICKER1_X, 2);	// Good1
					m_nSortPick1Case = 21; m_tSortPick1Loop.Set_LoopTime(10000);
					m_tSortPick1Loop.Takt_End(nTaktZone,19);
					m_tSortPick1Loop.Takt_Start(nTaktZone, 20);
					m_tSortPick1Loop.Takt_End(nTaktZone,20);
				} else {
					m_nSortPick1Case = 27; m_tSortPick1Loop.Set_LoopTime(10000);
					m_tSortPick1Loop.Takt_End(nTaktZone,19);
					m_tSortPick1Loop.Takt_Start(nTaktZone, 20);
					m_tSortPick1Loop.Takt_End(nTaktZone,20);
				}
			}
		}
		break;
	case 21:	// Check Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_X, 2)) {	// Good1
			if (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20) { m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000); }
			else if (Check_UnloadLotEnd()) { 
				g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
				m_nSortPick1Case = 27; m_tSortPick1Loop.Set_LoopTime(10000);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 21);
				m_tSortPick1Loop.Takt_End(nTaktZone,21);
			}
		}
		return TRUE;

	case 22:	// Move to Good Position
		if ((gData.nPNoSortPick[0] != gData.nPNoGoodTray) && (gData.nPNoSortPick[0] != 0)) {
			if (gData.nPNoGoodTray != 0) return TRUE;
		}
		if (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20) {
			if (m_nGoodTray1Case == 20) nSp1WorkGood = 1;
			if (m_nGoodTray2Case == 20) nSp1WorkGood = 2;

			if ((nSp1WorkGood == 1 && g_objAJinAXL.Is_Done(AX_GOOD_STAGE1_Y)) || 
				(nSp1WorkGood == 2 && g_objAJinAXL.Is_Done(AX_GOOD_STAGE2_Y))) {
				if (Select_GoodTrayPos(nSp1TrayPosX, nSp1TrayPosY)) {
					
					if (Select_SortPickGoodPos(1, nSp1StartNo, nSp1PickCnt)) {	//startNo:0base, PickCnt:1base
						if (!Select_GoodTrayPos(nSp1TrayPosX, nSp1TrayPosY)) nSp1TrayPosX = 0;	// Tray 교체중...
						nSp1TrayCnt = gData.nTrayX - nSp1TrayPosX;
						nSp1DownSu = ((nSp1PickCnt < nSp1TrayCnt) ? nSp1PickCnt : nSp1TrayCnt);
						m_nSortPick1MultiCnt = nSp1DownSu;

						if (nSp1WorkGood == 1) dSp1X = m_pMoveData->dSortPicker1X[2] + (nSp1TrayPosX - nSp1StartNo) * m_pEquipData->dTrayPitchX;	// Good1
						if (nSp1WorkGood == 2) dSp1X = m_pMoveData->dSortPicker1X[3] + (nSp1TrayPosX - nSp1StartNo) * m_pEquipData->dTrayPitchX;	// Good2
						g_objAJinAXL.Move_Absolute(AX_SORT_PICKER1_X, dSp1X);

						if (nSp1WorkGood == 1 && m_nGoodTray1Case == 20) {
							dSp1Y = m_pMoveData->dGoodStage1Y[1] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
							g_objAJinAXL.Move_Absolute(AX_GOOD_STAGE1_Y, dSp1Y);
						}
						if (nSp1WorkGood == 2 && m_nGoodTray2Case == 20) {
							dSp1Y = m_pMoveData->dGoodStage2Y[1] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
							g_objAJinAXL.Move_Absolute(AX_GOOD_STAGE2_Y, dSp1Y);
						}
						m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
						m_tSortPick1Loop.Takt_Start(nTaktZone, 22);
						m_tSortPick1Loop.Takt_End(nTaktZone,22);

					} else {
						g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
						m_nSortPick1Case = 27; m_tSortPick1Loop.Set_LoopTime(10000);
						m_tSortPick1Loop.Takt_Start(nTaktZone, 22);
						m_tSortPick1Loop.Takt_End(nTaktZone,22);
					}
				}
			} else {
				break;
			}
		}
		return TRUE;
	case 23:	// Picker Down
		if (g_objAJinAXL.Is_Done(AX_SORT_PICKER1_Z) && g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER1_X, dSp1X) &&
			((nSp1WorkGood == 1 && g_objAJinAXL.Is_MoveDone(AX_GOOD_STAGE1_Y, dSp1Y)) ||
			 (nSp1WorkGood == 2 && g_objAJinAXL.Is_MoveDone(AX_GOOD_STAGE2_Y, dSp1Y))) )
		{
			
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 2);	//Good Down
			g_objCommon.Set_SortPicker1DownMulti(nSp1StartNo+1, nSp1DownSu);
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 23);
		}
		break;
	case 24:	// 정보전달, Vac Off
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 2) && g_objCommon.Get_SortPicker1DownMulti(nSp1StartNo+1, nSp1DownSu)) {
			for (int i = 0; i < nSp1DownSu; i++) {
				gData.InfoGoodTray[nSp1TrayPosY][nSp1TrayPosX+i] = gData.InfoSortPick[0][nSp1StartNo+i]; 
				gData.InfoSortPick[0][nSp1StartNo+i] = 0;

				CString sInfo; 
				sInfo.Format("%d", gData.InfoGoodTray[nSp1TrayPosY][nSp1TrayPosX+i]);
				int nTNo = gData.nTNoSortPick[0][nSp1StartNo+i];
				int nCNo = gData.nCNoSortPick[0][nSp1StartNo+i];
				int nGTNo = gData.nGoodStageTrayNo[nSp1WorkGood-1];	//gData.nGoodTrayCount;
				int nGCNo = (gData.nTrayX*nSp1TrayPosY) + nSp1TrayPosX + i + 1;
				g_objCapAttach.Set_BarcodeUpdate(nSp1PNo, nGTNo, nGCNo, gMes.sBarID[nSp1PNo-1][nTNo-1][nCNo-1]);
				g_objMES.Set_Result(gLot.sLotID[nSp1PNo-1], gMes.sBarID[nSp1PNo-1][nTNo-1][nCNo-1], "OK", sInfo, gMes.sNGCode[nSp1PNo-1][nTNo-1][nCNo-1], nTNo, nCNo, 0,0,0,0);
				g_objLogFile.Save_OutTray("GOOD", gData.nGoodTrayCount, nSp1TrayPosX+i, nSp1TrayPosY, gData.nPNoSortPick[0], gData.nTNoSortPick[0][nSp1StartNo+i], gData.nCNoSortPick[0][nSp1StartNo+i]);
				g_objLogFile.Save_CmTrackingLog("GOOD", gData.nGoodTrayCount, nSp1TrayPosX+i, nSp1TrayPosY, gData.nPNoSortPick[0], gData.nTNoSortPick[0][nSp1StartNo+i], gData.nCNoSortPick[0][nSp1StartNo+i]);

				gData.nTNoSortPick[0][nSp1StartNo+i] = gData.nCNoSortPick[0][nSp1StartNo+i] = 0;
			}
			gData.nULPNo = gData.nPNoGoodTray = gData.nPNoSortPick[0];
			if (Check_SortPickerEmpty(1)) gData.nPNoSortPick[0] = 0;
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 4, nSp1WorkGood-1);

			g_objCommon.Set_SortPicker1OpenMulti(nSp1StartNo+1, nSp1DownSu);
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
			m_tSortPick1Loop.Takt_End(nTaktZone, 23);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 24);
		}
		break;
	case 25 :	// Picker Up
		if (g_objCommon.Get_SortPicker1OpenMulti(nSp1StartNo+1, nSp1DownSu)) {
			g_objCommon.Set_SortPicker1Up(0);
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	//Ready Up
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(5000);
			m_tSortPick1Loop.Takt_End(nTaktZone, 24);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 25);
		}
		break;
	case 26:	// Picker Check
		if (g_objCommon.Get_SortPicker1Up(0) && g_objCommon.Get_InfoSortPicker1Close() && g_objCommon.Get_InfoSortPicker1Check()) {
			
			if (Check_GoodTrayFull()) {
				if (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20) {
					if (m_nGoodTray1Case == 20) nSp1WorkGood = 1;
					if (m_nGoodTray2Case == 20) nSp1WorkGood = 2;

					gData.sCLotID[nSp1WorkGood-1] = gData.sLotID[gData.nPNoGoodTray-1];
					gData.nCPortNo[nSp1WorkGood-1] = gData.nPNoGoodTray;
					gData.nCGoodTrayCount[nSp1WorkGood-1] = gData.nGoodStageTrayNo[nSp1WorkGood-1];	//gData.nGoodTrayCount;
					gData.nLastTrayNo = gData.nGoodStageTrayNo[nSp1WorkGood-1];
					gData.nCTrayCmCnt[nSp1WorkGood-1] = Get_GoodTrayCmCnt();
				}
				if (m_nGoodTray1Case == 20) m_nGoodTray1Case = 21;
				if (m_nGoodTray2Case == 20) m_nGoodTray2Case = 21;
			}
			if (Check_SortPickerEmpty(1)){
				g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
				m_nSortPick1Case = 27; m_tSortPick1Loop.Set_LoopTime(5000);
				m_tSortPick1Loop.Takt_End(nTaktZone, 25);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 26);
				m_tSortPick1Loop.Takt_End(nTaktZone, 26);
			} else {
				m_nSortPick1Case = 20; m_tSortPick1Loop.Set_LoopTime(5000);
				m_tSortPick1Loop.Takt_End(nTaktZone, 25);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 26);
				m_tSortPick1Loop.Takt_End(nTaktZone, 26);
			}
		}
		break;
	case 27:	// Position Check
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0)) {
			// 한Case에서 오는게 아니라 따로 추출해서 입력해준다.
			if (Check_SortPickerEmpty(1)){
				if (!gData.bNGTrayWait && Check_NgBufferFull(1)) {
					m_nSortPick1Case = 50; m_tSortPick1Loop.Set_LoopTime(10000);
					m_tSortPick1Loop.Takt_Start(nTaktZone, 27);
					m_tSortPick1Loop.Takt_End(nTaktZone, 27);

				} else {
					m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
					m_tSortPick1Loop.Takt_Start(nTaktZone, 27);
					m_tSortPick1Loop.Takt_End(nTaktZone, 27);
				}
			} else {
				if (Select_SortPickNgPos(1, nSp1StartNo, nSp1PickCnt)) {
					m_nSortPick1Case = 7; m_tSortPick1Loop.Set_LoopTime(10000);
					m_tSortPick1Loop.Takt_Start(nTaktZone, 27);
					m_tSortPick1Loop.Takt_End(nTaktZone, 27);
				}
			}
		}
		break;
	case 28:	// Buffer 복귀
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0)) {
			if (Check_InspectLotEnd(gData.nULPNo, 1)) {
				if (Check_NgBufferLotEnd(gData.nULPNo)) {	// NG Buffer에 모듈이 없으면 LotEnd 모듈이 있으면 포트 넘버 확인 후 언로딩 작업.
					if (m_pEquipData->bUseApdAlarm) {	// 2023.05.11+
						m_nSortPick1Case = 60; m_tSortPick1Loop.Set_LoopTime(10000); break;	// LotEnd Case
						m_tSortPick1Loop.Takt_Start(nTaktZone, 28);
						m_tSortPick1Loop.Takt_End(nTaktZone, 28);
					}

					if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
					if(!m_tSortPick1Loop.Waiting_Time(300)) break;
					if (!Check_GoodTrayEmpty() && (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20)) {
						if (m_nGoodTray1Case == 20) nSp1WorkGood = 1;
						if (m_nGoodTray2Case == 20) nSp1WorkGood = 2;

						gData.sCLotID[nSp1WorkGood-1] = gData.sLotID[gData.nPNoGoodTray-1];
						gData.nCPortNo[nSp1WorkGood-1] = gData.nPNoGoodTray;
						gData.nCGoodTrayCount[nSp1WorkGood-1] = gData.nGoodStageTrayNo[nSp1WorkGood-1];	//gData.nGoodTrayCount;
						gData.nLastTrayNo = gData.nGoodStageTrayNo[nSp1WorkGood-1];
						gData.nCTrayCmCnt[nSp1WorkGood-1] = Get_GoodTrayCmCnt();
					}
					Job_LotEnd(gData.nULPNo, nSp1WorkGood);
					if (m_pThreadBeep == NULL && m_pThreadNgFullBeep == NULL) {						
						m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));

					} else {
						gData.bLotEndBeep = TRUE;
					}

					gData.bContinueLotEnd = TRUE;
					if (m_nNgTrayCase == 10) m_nNgTrayCase = 11; 
					if (!Check_GoodTrayEmpty()) {
						if (m_nGoodTray1Case == 20) { gData.bGoodTrayLotEnd[0]= TRUE; m_nGoodTray1Case = 21; }
						if (m_nGoodTray2Case == 20) { gData.bGoodTrayLotEnd[1]= TRUE; m_nGoodTray2Case = 21; }
					}  else {
						// Inline Mode에서는 Unload 버튼을 사용하지 않는다.
						if (!m_pEquipData->bUseInlineMode) {
							if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
							if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case >  30) { gData.bGoodTrayWait = TRUE; }
							if (m_nGoodTray1Case >  30 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
						}
					}
					if (!gData.bGoodTrayLotEnd[0] && m_nGoodTray1Case > 20 && m_nGoodTray1Case < 30) { gData.bGoodTrayLotEnd[0] = TRUE; }
					if (!gData.bGoodTrayLotEnd[1] && m_nGoodTray2Case > 20 && m_nGoodTray2Case < 30) { gData.bGoodTrayLotEnd[1] = TRUE; }

				} else {
					if (!Check_NgBufferEmpty(1) && Check_SortPickerEmpty(1)) {
						bLastNgBuffPick1 = TRUE;
						m_nSortPick1Case = 50; m_tSortPick1Loop.Set_LoopTime(10000);
						m_tSortPick1Loop.Takt_Start(nTaktZone, 28);
						m_tSortPick1Loop.Takt_End(nTaktZone, 28);
						break;
					}
				}
			}
			if (Check_UnloadLotEnd() && !m_bUnloadLotEnd) { m_bUnloadLotEnd = TRUE; }

			m_strLog.Format("Sort Picker1, %d", GetTickCount() - m_dwSortPick1);
			g_objLogFile.Save_TestLog(m_strLog);

			CString strLog;
			CString strMsg = "Sort picker1 move to NG unloading + good unloading";
			dwSortPick1End = GetTickCount() - dwSortPick1Unload;
			strLog.Format("MCC,(%02d) %s,(%02d) %s,%0.3lf", 13, "SortPicker1", 13, strMsg, dwSortPick1End / 1000.0);
			g_objLogFile.Save_MCCLog(strLog);//g_objLogFile.Save_HandlerLog(strLog);

			if (nSp1WorkBuff != 1 && nSp1WorkBuff != 2) nSp1WorkBuff = 1;	// Test
			if (nSp1WorkBuff == 1) g_objCommon.Move_Position(AX_SORT_PICKER1_X, 0);	// Buffer1 Position
			if (nSp1WorkBuff == 2) g_objCommon.Move_Position(AX_SORT_PICKER1_X, 1);	// Buffer2 Position
			//g_objCommon.Set_SortPicker1Down(0);	// 복귀할때 실린더 다운해준다.

			if (m_nBuffTray1Case == 20)		 nSp1WorkBuff = 1;
			else if (m_nBuffTray2Case == 20) nSp1WorkBuff = 2;
			else							 nSp1WorkBuff = 0;

			if (!gData.bBuffStageMove && nSp1WorkBuff != 0 && Select_BufferTrayUpPos(nSp1WorkBuff, nSp1TrayPosX, nSp1TrayPosY)){	// 복귀할때 Y축 움직일수 있으면 움직여 주자.

				if (nSp1WorkBuff == 1 && m_nBuffTray1Case == 20) { 
					dSp1Y = m_pMoveData->dBufferStage1Y[2] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
					g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dSp1Y);
				}
				if (nSp1WorkBuff == 2 && m_nBuffTray2Case == 20) {
					dSp1Y = m_pMoveData->dBufferStage2Y[2] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
					g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dSp1Y);
				}
				gData.bBuffStageMove = TRUE;
			} else {
				if (gData.bBuffStageMove) gData.bBuffStageMove = FALSE;
			}

			m_nSortPick1Case = 0; m_tSortPick1Loop.Set_LoopTime(10000);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 28);
			m_tSortPick1Loop.Takt_End(nTaktZone, 28);
		}		
		break;

	// NG Buffer place
	case 40:	// Move to NG Buffer Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objAJinAXL.Is_Done(AX_SORT_PICKER1_X) && g_objAJinAXL.Is_Done(AX_SORT_PICKER1_P)) {
			if (Select_SortPickNgPos(1, nSp1StartNo, nSp1PickCnt, TRUE)) {	// startNo 0base pickCnt 1base
				if (Select_NgBufferPos(1, nSp1TrayPosX)) {	// Ng Buffer에 자리가 있을때
					// 한Case에서 오는게 아니라 따로 추출해서 입력해준다.
							
					nSp1TrayCnt = gData.nSortPickQt - nSp1TrayPosX;
					nSp1DownSu = ((nSp1PickCnt < nSp1TrayCnt) ? nSp1PickCnt : nSp1TrayCnt);
					m_nSortPick1MultiCnt = nSp1DownSu;

					dSp1X = m_pMoveData->dSortPicker1X[6] + (nSp1TrayPosX - nSp1StartNo) * m_pEquipData->dNgBufferPitchX;

					g_objAJinAXL.Move_Absolute(AX_SORT_PICKER1_X, dSp1X);
					g_objCommon.Move_Position(AX_SORT_PICKER1_P, 1);	// NG Buffer Pitch
					m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
					m_tSortPick1Loop.Takt_Start(nTaktZone, 40);
					m_tSortPick1Loop.Takt_End(nTaktZone, 40);

				} else {	// Ng Buffer에 자리가 없고 Picker에 Ng가 있을때
					g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
					g_objCommon.Move_Position(AX_SORT_PICKER1_P, 0);	// Tray Pitch
					m_nSortPick1Case = 10; m_tSortPick1Loop.Set_LoopTime(10000);
					m_tSortPick1Loop.Takt_Start(nTaktZone, 40);
					m_tSortPick1Loop.Takt_End(nTaktZone, 40);
				}

			} else {	// Picker에 Ng가 없을때
				g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
				g_objCommon.Move_Position(AX_SORT_PICKER1_P, 0);	// Tray Pitch
				m_nSortPick1Case = 17; m_tSortPick1Loop.Set_LoopTime(10000);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 40);
				m_tSortPick1Loop.Takt_End(nTaktZone, 40);
			}
		}
		break;
	case 41:	// Check Position
		if (g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER1_X, dSp1X) && g_objCommon.Check_Position(AX_SORT_PICKER1_P, 1)) {
			if (!(g_objCommon.Get_InfoSortPicker1Close() && g_objCommon.Get_InfoSortPicker1Check())) break;
		
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 4);	//NG Buffer Down
			g_objCommon.Set_SortPicker1DownMulti(nSp1StartNo+1, nSp1DownSu);
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 41);
		}
		break;
	case 42:	// 정보전달, Vac Off	// Sort Picker = 0, NG Buffer = 1
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 4) && g_objCommon.Get_SortPicker1DownMulti(nSp1StartNo+1, nSp1DownSu)) {
			for (int i = 0; i < nSp1DownSu; i++) {
				gData.InfoNgBuffer[1][nSp1TrayPosX+i] = gData.InfoSortPick[0][nSp1StartNo+i]; 
				gData.nTNoNgBuffer[1][nSp1TrayPosX+i] = gData.nTNoSortPick[0][nSp1StartNo+i];
				gData.nCNoNgBuffer[1][nSp1TrayPosX+i] = gData.nCNoSortPick[0][nSp1StartNo+i];

				gData.InfoSortPick[0][nSp1StartNo+i] = gData.nTNoSortPick[0][nSp1StartNo+i] = gData.nCNoSortPick[0][nSp1StartNo+i] = 0;
			}
			gData.nPNoNgBuffer[1] = gData.nPNoSortPick[0];
			if (Check_SortPickerEmpty(1)) gData.nPNoSortPick[0] = 0;

			g_objCommon.Set_InfoNgBufferVacOn(1);
			g_objCommon.Set_SortPicker1OpenMulti(nSp1StartNo+1, nSp1DownSu);
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
			m_tSortPick1Loop.Takt_End(nTaktZone, 41);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 42);
		}
		break;
	case 43 :	// Picker Up
		if (g_objCommon.Get_InfoNgBufferVacOn(1) && g_objCommon.Get_SortPicker1OpenMulti(nSp1StartNo+1, nSp1DownSu)) {
			g_objCommon.Set_SortPicker1Up(0);
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(5000);
			m_tSortPick1Loop.Takt_End(nTaktZone, 42);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 43);
		}
		break;
	case 44:	// Position Check
		if (g_objCommon.Get_SortPicker1Up(0) && g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Get_InfoSortPicker1Check()) {
			m_tSortPick1Loop.Takt_End(nTaktZone, 43);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 44);
			m_tSortPick1Loop.Takt_End(nTaktZone, 44);

			if (Select_SortPickNgPos(1, nSp1StartNo, nSp1PickCnt, TRUE)) {
				if (Check_NgBufferFull(1)) {
					g_objCommon.Move_Position(AX_SORT_PICKER1_P, 0);	// Tray Pitch
					m_nSortPick1Case = 7; m_tSortPick1Loop.Set_LoopTime(5000);

				} else {
					m_nSortPick1Case = 40; m_tSortPick1Loop.Set_LoopTime(5000);
				}
			} else {
				g_objCommon.Move_Position(AX_SORT_PICKER1_P, 0);	// Tray Pitch
				m_nSortPick1Case = 17; m_tSortPick1Loop.Set_LoopTime(10000);
			}			
		}
		break;

	// NG Buffer Pickup
	case 50:
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objAJinAXL.Is_Done(AX_SORT_PICKER1_X) && g_objAJinAXL.Is_Done(AX_SORT_PICKER1_P)) {
			if (Check_SortPickerEmpty(1)) {
				// 한Case에서 오는게 아니라 따로 추출해서 입력해준다.
				g_objCommon.Move_Position(AX_SORT_PICKER1_X, 6);	// NG Buffer Position
				g_objCommon.Move_Position(AX_SORT_PICKER1_P, 1);	// NG Buffer Pitch
				m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
				m_tSortPick1Loop.Takt_Start(nTaktZone, 50);
			}
		}
		break;
	case 51:	// Check Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_X, 6) && g_objCommon.Check_Position(AX_SORT_PICKER1_P, 1)) {
			
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 4);	//NG Buffer Down
			g_objCommon.Set_InfoSortPicker1Down(1);
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
			m_tSortPick1Loop.Takt_End(nTaktZone, 50);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 51);
		}
		break;
	case 52:	// 정보전달, Vac Off
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 4) && g_objCommon.Get_InfoSortPicker1Down(1)) {
			for (int i = 0; i < gData.nSortPickQt; i++) {
				gData.InfoSortPick[0][i] = gData.InfoNgBuffer[1][i]; 
				gData.nTNoSortPick[0][i] = gData.nTNoNgBuffer[1][i];
				gData.nCNoSortPick[0][i] = gData.nCNoNgBuffer[1][i];

				gData.InfoNgBuffer[1][i] = gData.nTNoNgBuffer[1][i] = gData.nCNoNgBuffer[1][i] = 0;
			}
			gData.nPNoSortPick[0] = gData.nPNoNgBuffer[1]; gData.nPNoNgBuffer[1] = 0;

			g_objCommon.Set_NgBufferVacOff(1);
			g_objCommon.Set_InfoSortPicker1Close();
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
			m_tSortPick1Loop.Takt_End(nTaktZone, 51);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 52);
		}
		break;
	case 53 :	// Picker Up
		if (g_objCommon.Get_InfoSortPicker1Close()) {
			if (!m_tSortPick1Loop.Waiting_Time(m_pEquipData->nDelayAdd[2])) break;	// SortPicker Delay
			g_objCommon.Set_SortPicker1Up(0);
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(5000);
			m_tSortPick1Loop.Takt_End(nTaktZone, 52);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 53);
		}
		break;
	case 54:
		if (g_objCommon.Get_InfoSortPicker1Close() && g_objCommon.Get_InfoSortPicker1Check() &&
			g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Get_SortPicker1Up(0))
		{
			
			g_objCommon.Move_Position(AX_SORT_PICKER1_P, 0);	// Tray Pitch
			m_nSortPick1Case = 7; m_tSortPick1Loop.Set_LoopTime(5000);
			m_tSortPick1Loop.Takt_End(nTaktZone, 53);
			m_tSortPick1Loop.Takt_Start(nTaktZone, 54);
			m_tSortPick1Loop.Takt_End(nTaktZone, 54);
		}
		break;

	// APD 파일 저장, MES 보고 및 APD 결과파일 확인
	case 60:	// APD 사용 유무 확인, Vision, Cap APD Data 요청
		if (m_pEquipData->bUseApdAlarm) {
			g_objInspector.Set_ApdRequest(INSPECTOR_PC1, "AG", nSp1PNo);
			g_objInspector.Set_ApdRequest(INSPECTOR_PC1, "B1", nSp1PNo);
			g_objInspector.Set_ApdRequest(INSPECTOR_PC2, "T1", nSp1PNo);
			g_objInspector.Set_ApdRequest(INSPECTOR_PC3, "T2", nSp1PNo);
			g_objInspector.Set_ApdRequest(INSPECTOR_PC4, "B2", nSp1PNo);
			g_objCapAttach.Set_ApdRequest();
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);

		} else {
			if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
			gData.bAPDResultErr = FALSE;
			m_nSortPick1Case = 63; m_tSortPick1Loop.Set_LoopTime(10000);
		}
		break;
	case 61:	// 모든 APD 값이 들어오면 Data 기록 후 MES 보고
		if (gData.bAviApdReceive[0] && gData.bAviApdReceive[1] && gData.bAviApdReceive[2] &&
			gData.bAviApdReceive[3] && gData.bAviApdReceive[4] && (gData.dAssyLoadCellAvg + 0.1 > 0))
		{
			g_objMES.Clear_APDResult();	// APD 결과 폴더에 파일이 있다면 삭제.
			g_objMES.Save_AviApdData(gData.sLotID[nSp1PNo-1], nSp1PNo, gData.sOperID);	// APD 파일 생성 및 MES 보고
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(5000);

		} else if (!m_pEquipData->bUseApdAlarm) {
			if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
			gData.bAPDResultErr = FALSE;
			m_nSortPick1Case = 63; m_tSortPick1Loop.Set_LoopTime(10000);
		}
		break;
	case 62:	// 결과 파일 읽기 확인, LotEnd 전 나머지 모듈 검사 결과 APD 파일 저장.
		if (g_objMES.Read_APDResult(gData.sLotID[nSp1PNo-1])) {			
			if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(5000);

		} else if (!m_pEquipData->bUseApdAlarm) {
			if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
			gData.bAPDResultErr = FALSE;
			m_nSortPick1Case = 63; m_tSortPick1Loop.Set_LoopTime(10000);
		}
		break;		
	case 63:	// 항목 결과에 따라 Lot End처리.
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER1_P, 0)) {
			if(!m_tSortPick1Loop.Waiting_Time(300)) break;	// 모듈 검사 결과 APD 파일 저장하고 일정시간 기다렸다가 Lot End 해줘야한다. 

			if (!Check_GoodTrayEmpty() && (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20)) {
				if (m_nGoodTray1Case == 20) nSp1WorkGood = 1;
				if (m_nGoodTray2Case == 20) nSp1WorkGood = 2;

				gData.sCLotID[nSp1WorkGood-1] = gData.sLotID[nSp1PNo-1];
				gData.nCPortNo[nSp1WorkGood-1] = nSp1PNo;
				gData.nCGoodTrayCount[nSp1WorkGood-1] = gData.nGoodStageTrayNo[nSp1WorkGood-1];	//gData.nGoodTrayCount;
				gData.nLastTrayNo = gData.nGoodStageTrayNo[nSp1WorkGood-1];
				gData.nCTrayCmCnt[nSp1WorkGood-1] = Get_GoodTrayCmCnt();
			}
			Job_LotEnd(nSp1PNo, nSp1WorkGood);
			if (m_pThreadBeep == NULL && m_pThreadNgFullBeep == NULL) {
				m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));

			} else {
				gData.bLotEndBeep = TRUE;
			}

			gData.bContinueLotEnd = TRUE;
			if (m_nNgTrayCase == 10) m_nNgTrayCase = 11; 
			if (!Check_GoodTrayEmpty()) {
				if (m_nGoodTray1Case == 20) { gData.bGoodTrayLotEnd[0] = TRUE; m_nGoodTray1Case = 21; }
				if (m_nGoodTray2Case == 20) { gData.bGoodTrayLotEnd[1] = TRUE; m_nGoodTray2Case = 21; }
			} else {
				// 랏종료시점에 대기중인 트레이가 빈트레이면 다음랏 모듈을 받아야 하기 때문에 트레이넘버를 1로 바꿔준다.
				if (m_nGoodTray1Case == 20) { gData.nGoodStageTrayNo[0] = 1; gData.nGoodTrayCount = 1; }
				if (m_nGoodTray2Case == 20) { gData.nGoodStageTrayNo[1] = 1; gData.nGoodTrayCount = 1; }

				// Inline Mode에서는 Unload 버튼을 사용하지 않는다.
				if (!m_pEquipData->bUseInlineMode) {
					if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
					if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case >  30) { gData.bGoodTrayWait = TRUE; }
					if (m_nGoodTray1Case >  30 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
				}
			}
			if (!gData.bGoodTrayLotEnd[0] && m_nGoodTray1Case > 20 && m_nGoodTray1Case < 30) { gData.bGoodTrayLotEnd[0] = TRUE; }
			if (!gData.bGoodTrayLotEnd[1] && m_nGoodTray2Case > 20 && m_nGoodTray2Case < 30) { gData.bGoodTrayLotEnd[1] = TRUE; }

			/////
			if (Check_UnloadLotEnd() && !m_bUnloadLotEnd) { m_bUnloadLotEnd = TRUE; }

			m_strLog.Format("Sort Picker1, %d", GetTickCount() - m_dwSortPick1);
			g_objLogFile.Save_TestLog(m_strLog);

			CString strLog;
			CString strMsg = "Sort picker1 move to NG unloading + good unloading";
			dwSortPick1End = GetTickCount() - dwSortPick1Unload;
			strLog.Format("MCC,(%02d) %s,(%02d) %s,%0.3lf", 13, "SortPicker1", 13, strMsg, dwSortPick1End / 1000.0);
			g_objLogFile.Save_MCCLog(strLog);//g_objLogFile.Save_HandlerLog(strLog);

			if (nSp1WorkBuff != 1 && nSp1WorkBuff != 2) nSp1WorkBuff = 1;	// Test
			if (nSp1WorkBuff == 1) g_objCommon.Move_Position(AX_SORT_PICKER1_X, 0);	// Buffer1 Position
			if (nSp1WorkBuff == 2) g_objCommon.Move_Position(AX_SORT_PICKER1_X, 1);	// Buffer2 Position

			if (gData.bAPDResultErr) m_nSortPick1Case++;	// Alarm 처리. Machine Stop
			else					 m_nSortPick1Case = 0;	// Lot End 완료.
			m_tSortPick1Loop.Set_LoopTime(5000);
		}
		break;
	case 64:	// 항목 결과중 NG 있으면 알람 처리
		m_nSortPick1Case = 0;
		g_objCommon.Show_Error(4364);
		return FALSE;
	}

	// 13. (Error : 4300)
	if (m_tSortPick1Loop.Over_LoopTime()) {
		if (m_nSortPick1Case == 5) {
			gData.nErrBufferStageNo = nSp1WorkBuff;
		} else { gData.nErrBufferStageNo = 0; }

		if (m_nSortPick1Case == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectError(gData.nPNoSortPick[0], gData.nTNoSortPick[0][i], gData.nCNoSortPick[0][i]); }
		}
		g_objCommon.Show_Error(4300 + m_nSortPick1Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 14. (Error : 4400)
BOOL CSequenceMain::SortPicker2_Run()
{
	int nTaktZone = 14;		// Takt_Start, Takt_End
	static int		nSp2WorkBuff = 0;
	static int		nSp2WorkGood = 0;
	static int		nSp2WorkNg = 0;
	static int		nSp2TrayPosX, nSp2TrayPosY = 0;
	static double	dSp2X, dSp2Y = 0.0;
	static int		nSp2TrayCnt = 0, nSp2PickCnt = 0, nSp2StartNo = 0, nSp2DownSu = 0, nSp2Base = 0, nSp2PNo = 0;
	static int		nSp2VacErr = 0;
	static BOOL		bLastNgBuffPick2 = FALSE;
	BOOL bS2Checking = FALSE;							// Inspect Done Check
	static DWORD	dwSortPick2Unload = 0;
	static DWORD	dwSortPick2End = 0;

	switch (m_nSortPick2Case) {
	case 0:		// Wait
		if (m_nBuffTray1Case == 20 || m_nBuffTray2Case == 20) {	// Unload 위치
			if (!Check_NgBufferEmpty(2) && Check_SortPickerEmpty(2)) {
				if (m_nBuffTray1Case == 20) nSp2WorkBuff = 1;
				if (m_nBuffTray2Case == 20) nSp2WorkBuff = 2;
				if (gData.nPNoNgTray != 0 && (gData.nPNoNgTray != gData.nPNoBuffTray[nSp2WorkBuff-1])) {
					bLastNgBuffPick2 = TRUE;
					m_nSortPick2Case = 50; m_tSortPick2Loop.Set_LoopTime(10000);
					
					break;
				}
			}
			if ((m_nSortPick1Case >= 6 && m_nSortPick1Case < 30) || (m_nSortPick1Case >= 40 && m_nSortPick1Case < 65)) {	// Sort1이 NG or Good Unload 작업중.
				m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(5000);
				
				m_tSortPick2Loop.Takt_Start(nTaktZone, 0, TRUE);
				m_tSortPick2Loop.Takt_End(nTaktZone, 0);
			}
		} else {
			if (!gData.bNGTrayWait && !Check_NgBufferEmpty(2) && Check_SortPickerEmpty(2)) {
				m_nSortPick2Case = 50; m_tSortPick2Loop.Set_LoopTime(10000);
				
			
			}
		}
		return TRUE;

	case 1:		// Select Pick Position
		if ((g_objCommon.Check_Position(AX_SORT_PICKER2_X, 0) || g_objCommon.Check_Position(AX_SORT_PICKER2_X, 1)) &&
			g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER2_P, 0)) {
			if (m_nBuffTray1Case == 20 || m_nBuffTray2Case == 20) {
				if (m_nBuffTray1Case == 20) nSp2WorkBuff = 1;
				if (m_nBuffTray2Case == 20) nSp2WorkBuff = 2;
				if (nSp2WorkBuff != 1 && nSp2WorkBuff != 2) break;	// Test Interlock
				if ((nSp2WorkBuff == 1 && !g_objAJinAXL.Is_Done(AX_BUFFER_STAGE1_Y)) ||
					(nSp2WorkBuff == 2 && !g_objAJinAXL.Is_Done(AX_BUFFER_STAGE2_Y))) break;

				gData.bBuffStageMove = TRUE;
				if (Check_SortPickerEmpty(2)) {
					m_dwSortPick2 = GetTickCount();
					

					if (!Select_BufferTrayUpPos(nSp2WorkBuff, nSp2TrayPosX, nSp2TrayPosY)) nSp2TrayPosX = 0;	// Good Tray 교체중...

					m_strLog.Format("MCC,(13) SortPicker2, X & buffer Y Move Start");
					g_objLogFile.Save_MCCLog(m_strLog);

					nSp2Base = 0; dSp2X = m_pMoveData->dSortPicker2X[nSp2WorkBuff-1];
					g_objAJinAXL.Move_Absolute(AX_SORT_PICKER2_X, dSp2X);

					if (nSp2WorkBuff == 1 && m_nBuffTray1Case == 20) { 
						dSp2Y = m_pMoveData->dBufferStage1Y[3] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dSp2Y);
					}
					if (nSp2WorkBuff == 2 && m_nBuffTray2Case == 20) { 
						dSp2Y = m_pMoveData->dBufferStage2Y[3] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dSp2Y);
					}
					nSp2VacErr = 0;
					m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

					m_tSortPick2Loop.Takt_Start(nTaktZone, 1);
					
				}
			}
		}
		break;
	case 2:		// Picker Down
		if (g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER2_X, dSp2X) &&
			((nSp2WorkBuff == 1 && g_objAJinAXL.Is_MoveDone(AX_BUFFER_STAGE1_Y, dSp2Y)) ||
			 (nSp2WorkBuff == 2 && g_objAJinAXL.Is_MoveDone(AX_BUFFER_STAGE2_Y, dSp2Y))) ) {

			//if (nSp2VacErr == 0) m_tSortPick2Loop.Takt_End(nTaktZone, 1);
			

			if(nSp2WorkBuff == 1) g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 1);	//Buffer 1 Down
			if(nSp2WorkBuff == 2) g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 5);	//Buffer 2 Down
			g_objCommon.Set_SortPicker2Down(0);	// All Down
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
			m_tSortPick2Loop.Takt_End(nTaktZone, 1);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 2);
		} 
		break;
	case 3:		// 정보전달, Picker Vac On
		if (((nSp2WorkBuff == 1 && g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 1)) ||
			 (nSp2WorkBuff == 2 && g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 5))) &&
			g_objCommon.Get_SortPicker2Down(0))
		{
			if (nSp2VacErr == 1) { m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000); break;}

			for (int i = 0; i < gData.nSortPickQt; i++) {
				gData.InfoSortPick[1][i] = gData.InfoBuffTray[nSp2WorkBuff-1][nSp2TrayPosY][nSp2Base+i]; 
				gData.InfoBuffTray[nSp2WorkBuff-1][nSp2TrayPosY][nSp2Base+i] = 0;

				gData.nTNoSortPick[1][i] = gData.nTNoBuffTray[nSp2WorkBuff-1][nSp2TrayPosY][nSp2Base+i];
				gData.nTNoBuffTray[nSp2WorkBuff-1][nSp2TrayPosY][nSp2Base+i] = 0;
				gData.nCNoSortPick[1][i] = gData.nCNoBuffTray[nSp2WorkBuff-1][nSp2TrayPosY][nSp2Base+i];
				gData.nCNoBuffTray[nSp2WorkBuff-1][nSp2TrayPosY][nSp2Base+i] = 0;

				//모듈트랙킹
				int nPNo = gData.nPNoBuffTray[nSp2WorkBuff-1] - 1;
				int nTNo = gData.nTNoSortPick[1][i] - 1;
				int nCNo = gData.nCNoSortPick[1][i] - 1;
				if (nCNo > -1) {
					gData.nCmJigNo[nPNo][nTNo][nCNo][SORT_PICK_NO] = 2;	// Sort Picker No.
					gData.nCmJigNo[nPNo][nTNo][nCNo][SORT_JIG_NO]  = i + 1;	// Sort Picker Jig No.
				}
			}
			gData.nPNoSortPick[1] = gData.nPNoBuffTray[nSp2WorkBuff-1];

			if ((gData.nCNoSortPick[1][0] == 1 || gData.nCNoSortPick[1][0] == 5 || gData.nCNoSortPick[1][0] == 9) && nSp2TrayPosY == 0) {
				if (gData.nTNoSortPick[1][0] == 1 && gData.nCNoSortPick[1][0] == 1) {
					m_dwULCycleTime = 0;
					m_dwULPickUpTime = GetTickCount();
					if (m_pEquipData->bUseInlineMode) g_objCapAttach.Set_LotStart(gData.nPNoSortPick[1]);

				} else {
					m_dwULCycleTime = GetTickCount() - m_dwULPickUpTime;
					m_dwULPickUpTime = GetTickCount();
					g_dlgWork.PostMessage(UM_UPDATE_UPH, 1, NULL);
				}
			}

			if (Check_BufferEmpty(nSp2WorkBuff)) {
				gData.nPNoBuffTray[nSp2WorkBuff-1] = 0;
			}
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 3, nSp2WorkBuff-1);

			g_objCommon.Set_InfoSortPicker2Close();

			
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

			m_tSortPick2Loop.Takt_End(nTaktZone, 2);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 3);
		} 
		break;
	case 4:		// Picker Up
		if (g_objCommon.Get_InfoSortPicker2Close()) {	// Delay Time만 사용한다.
			if (!m_tSortPick2Loop.Waiting_Time(m_pEquipData->nDelayAdd[2])) break;	// SortPicker Delay

			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	//Ready Up
			g_objCommon.Set_SortPicker2Up(0);	// All Up
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(6000);

			m_tSortPick2Loop.Takt_End(nTaktZone, 3);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 4);
		}
		break;
	case 5:
		if (g_objCommon.Get_InfoSortPicker2Close() && g_objCommon.Get_InfoSortPicker2Check() &&
			g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Get_SortPicker2Up(0))
		{
			
			if (Check_BufferEmpty(nSp2WorkBuff)) {	// Tray Change
				nSp2TrayPosY = 0;
				gData.nBtWorkSortPickNo = 2;
				if (nSp2WorkBuff == 1 && m_nBuffTray1Case == 20) m_nBuffTray1Case = 21;
				if (nSp2WorkBuff == 2 && m_nBuffTray2Case == 20) m_nBuffTray2Case = 21;
			}
			gData.bBuffStageMove = FALSE;
			gData.nSp2Timer = GetTickCount();
			SYSTEMTIME time;
			GetLocalTime(&time);
			gData.sSortWaitStartTime[1].Format("%02d:%02d:%02d %03d", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(30000);	// 검사 완료

			m_tSortPick2Loop.Takt_End(nTaktZone, 4);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 5);
			gData.dwSkipTime_Sort2 = GetTickCount();
		} 
		break;
	case 6:		// NG or Good
		for (int i = 4; i >= 0; i--) { if (!Check_InspectDone2(gData.nPNoSortPick[1], gData.nTNoSortPick[1][i], gData.nCNoSortPick[1][i], gData.InfoSortPick[1][i], 2)) { bS2Checking = TRUE; break; } }
		if (bS2Checking) break;	// Not Complete

		nSp2PNo = gData.nPNoSortPick[1];
		for (int i = 0; i < 5; i++) {
			if (gData.InfoSortPick[1][i] == 1 || gData.InfoSortPick[1][i] == 9) gLot.nGoodCount[nSp2PNo-1]++;
			else if (gData.InfoSortPick[1][i] != 0) gLot.nNgCount[nSp2PNo-1]++;
			for (int j = 0; j < 6; j++) {
				if (gData.InfoSortPick[1][i] == j + 3)	gLot.nSNgCount[nSp2PNo-1][j]++;	// S-NG (3,4,5,6,7,8)
			}
			Write_LotJudge(nSp2PNo, gData.nTNoSortPick[1][i], gData.nCNoSortPick[1][i], gData.InfoSortPick[1][i], 2);
		}
		g_dlgWork.PostMessage(UM_VISION_RESULT, nSp2PNo, NULL);

		dwSortPick2Unload = GetTickCount();	// Sort Picker2 NG, Good Unloading Start Time

		m_tSortPick2Loop.Takt_End(nTaktZone, 5);
		m_tSortPick2Loop.Takt_Start(nTaktZone, 6);
		m_tSortPick2Loop.Takt_End(nTaktZone, 6);

		//	NG가 있으면 무조건 NG 부터 간다.
		if (Select_SortPickNgPos(2, nSp2StartNo, nSp2PickCnt)) {	// NG
			m_nSortPick2Case = 7; m_tSortPick2Loop.Set_LoopTime(10000);
		} else {
			if (Select_SortPickGoodPos(2, nSp2StartNo, nSp2PickCnt)) {	// NG 없고 Good만 있을때
				m_nSortPick2Case = 17; m_tSortPick2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 7:	// Wait Move to Ng Position
		// SortPicker1번이 Pickup 작업중, Good Unload 작업중,  NG Buffer 작업중이면 NG로 갈수있다.이면 NG로 갈수있다.
		if ((m_nSortPick1Case >=  0 && m_nSortPick1Case < 10) ||
			(m_nSortPick1Case >= 19 && m_nSortPick1Case < 30) ||
			(m_nSortPick1Case >= 40 && m_nSortPick1Case < 60) ||
			//(m_nSortPick1Case >= 60 && m_nSortPick1Case < 70) ||	// APD 확인중에도 갈수있다.
			(m_nSortPick1Case == 12) && (gData.nPNoSortPick[0] != gData.nPNoSortPick[1])||
			(m_nSortPick1Case == 17)) {

			if ((gData.nPNoSortPick[1] != gData.nPNoNgTray) && (gData.nPNoSortPick[1] != 0)) {
				if (gData.nPNoNgTray != 0) return TRUE;
			}
			m_nSortPick2Case = 10; m_tSortPick2Loop.Set_LoopTime(10000);

			m_tSortPick2Loop.Takt_Start(nTaktZone, 7);
			m_tSortPick2Loop.Takt_End(nTaktZone, 7);

		} else {
			if (Select_SortPickGoodPos(2, nSp2StartNo, nSp2PickCnt)) {		// Sort1번이 NG 작업 중이고 Good이 있다면 Good Unload 먼저 해준다.
				m_nSortPick2Case = 17; m_tSortPick2Loop.Set_LoopTime(10000);

				m_tSortPick2Loop.Takt_Start(nTaktZone, 7);
				m_tSortPick2Loop.Takt_End(nTaktZone, 7);
			} else if (m_nSortPick1Case >= 11 && m_nSortPick1Case < 20) {	// Sort1번이 NG 작업 중이고 NG만 있다면 NG Tray 1번 위치로 가서 대기한다.
				m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

				m_tSortPick2Loop.Takt_Start(nTaktZone, 7);
				m_tSortPick2Loop.Takt_End(nTaktZone, 7);
			}
		}
		return TRUE;

	case 8:	// Move to NG Tray1 Position
		if (g_objAJinAXL.Is_Done(AX_SORT_PICKER2_X)) {
			if (!(g_objCommon.Get_InfoSortPicker2Close() && g_objCommon.Get_InfoSortPicker2Check())) break;
			g_objCommon.Move_Position(AX_SORT_PICKER2_X, 4);
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

			m_tSortPick2Loop.Takt_Start(nTaktZone, 8);
			
		}
		break;
	case 9:	// Check Position & Sort1
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_X, 4)) {
			// SortPicker2번이 Pickup 작업중, Good Unload 작업중,  NG Buffer 작업중이면 NG Unloading 할수있다.
			if ((m_nSortPick1Case >=  0 && m_nSortPick1Case < 10) ||
				(m_nSortPick1Case >= 19 && m_nSortPick1Case < 30) ||
				(m_nSortPick1Case >= 40 && m_nSortPick1Case < 60) ||
				//(m_nSortPick1Case >= 60 && m_nSortPick1Case < 70) ||	// APD 확인중에도 갈수있다.
				(m_nSortPick1Case == 17)) {

				if ((gData.nPNoSortPick[1] != gData.nPNoNgTray) && (gData.nPNoSortPick[1] != 0)) {
					if (gData.nPNoNgTray != 0) return TRUE;
				}
				m_nSortPick2Case = 10; m_tSortPick2Loop.Set_LoopTime(10000);
				
				m_tSortPick2Loop.Takt_End(nTaktZone, 8);
				m_tSortPick2Loop.Takt_Start(nTaktZone, 9);
				
			} else {
				return TRUE;
			}
		}
		break;

	// NG Unload
	case 10:	// Check NG Tray
		// NG Tray가 받을 준비 됐는지 확인한다.
		if (m_nNgTrayCase == 10) {
			m_nSortPick2Case = 12; m_tSortPick2Loop.Set_LoopTime(10000);

			m_tSortPick2Loop.Takt_End(nTaktZone, 9);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 10);
			m_tSortPick2Loop.Takt_End(nTaktZone, 10);

		} else {
			if (g_objAJinAXL.Is_Done(AX_SORT_PICKER2_Z) && g_objAJinAXL.Is_Done(AX_SORT_PICKER2_X)) {
				g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up

				if (Select_SortPickNgPos(2, nSp2StartNo, nSp2PickCnt)) {
					if (bLastNgBuffPick2 == FALSE && !Check_UnloadLotEnd(1) && !Check_NgBufferFull(2)) { // Ng Tray 교체 작업 중에 Ng Buffer에 내려놓는다.
						m_nSortPick2Case = 40; m_tSortPick2Loop.Set_LoopTime(10000);

						m_tSortPick2Loop.Takt_End(nTaktZone, 9);
						m_tSortPick2Loop.Takt_Start(nTaktZone, 10);
						m_tSortPick2Loop.Takt_End(nTaktZone, 10);

					} else {
						if (!(g_objCommon.Get_InfoSortPicker2Close() && g_objCommon.Get_InfoSortPicker2Check())) break;
						g_objCommon.Move_Position(AX_SORT_PICKER2_X, 5);
						m_nSortPick2Case = 11; m_tSortPick2Loop.Set_LoopTime(10000);

						m_tSortPick2Loop.Takt_End(nTaktZone, 9);
						m_tSortPick2Loop.Takt_Start(nTaktZone, 10);
						m_tSortPick2Loop.Takt_End(nTaktZone, 10);
					}
				} else {
					m_nSortPick2Case = 17; m_tSortPick2Loop.Set_LoopTime(10000);

					m_tSortPick2Loop.Takt_End(nTaktZone, 9);
					m_tSortPick2Loop.Takt_Start(nTaktZone, 10);
					m_tSortPick2Loop.Takt_End(nTaktZone, 10);
				}
			}
		}
		break;
	case 11:	// Check Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_X, 5)) {
			if (m_nNgTrayCase == 10) { 
				m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

				m_tSortPick2Loop.Takt_Start(nTaktZone, 11);
			}
		}
		return TRUE;

	case 12:	// Move to NG Unload Position
		if ((gData.nPNoSortPick[1] != gData.nPNoNgTray) && (gData.nPNoSortPick[1] != 0)) {
			if (gData.nPNoNgTray != 0) return TRUE;
		}
		if (m_nNgTrayCase != 10) { m_nSortPick2Case = 10; m_tSortPick2Loop.Set_LoopTime(10000); return TRUE; }
		if (bLastNgBuffPick2 == TRUE) bLastNgBuffPick2 = FALSE;

		if (Select_SortPickNgPos(2, nSp2StartNo, nSp2PickCnt)) {
			if (Select_NgTrayPos(2, nSp2WorkNg, nSp2TrayPosX, nSp2TrayPosY)) {
				// 한Case에서 오는게 아니라 따로 추출해서 입력해준다.
				
				if (!Select_NgTrayPos(2, nSp2WorkNg, nSp2TrayPosX, nSp2TrayPosY)) nSp2TrayPosX = 0;	// Tray 교체중...
				nSp2TrayCnt = gData.nTrayX - nSp2TrayPosX;
				nSp2DownSu = ((nSp2PickCnt < nSp2TrayCnt) ? nSp2PickCnt : nSp2TrayCnt);
				m_nSortPick2MultiCnt = nSp2DownSu;

				int nNgTrayY = nSp2WorkNg / 2;
				int nNgTrayX = nSp2WorkNg % 2;
				dSp2X = m_pMoveData->dSortPicker2X[4 + nNgTrayX] + (nSp2TrayPosX - nSp2StartNo) * m_pEquipData->dTrayPitchX;
				dSp2Y = m_pMoveData->dNgStageY[3 + nNgTrayY] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;

				g_objAJinAXL.Move_Absolute(AX_SORT_PICKER2_X, dSp2X);
				g_objAJinAXL.Move_Absolute(AX_NG_STAGE_Y, dSp2Y);
				m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

				m_tSortPick2Loop.Takt_End(nTaktZone,11);
				m_tSortPick2Loop.Takt_Start(nTaktZone, 12);
				m_tSortPick2Loop.Takt_End(nTaktZone,12);

			}
		} else {
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
			m_nSortPick2Case = 17; m_tSortPick2Loop.Set_LoopTime(10000);

				m_tSortPick2Loop.Takt_End(nTaktZone,11);
				m_tSortPick2Loop.Takt_Start(nTaktZone, 12);
				m_tSortPick2Loop.Takt_End(nTaktZone,12);
		}
		break;
	case 13:	// Picker Down
		if (g_objAJinAXL.Is_Done(AX_SORT_PICKER2_Z) && g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER2_X, dSp2X) && g_objAJinAXL.Is_MoveDone(AX_NG_STAGE_Y, dSp2Y) && g_objCommon.Check_Position(AX_SORT_PICKER2_P, 0)) {
			
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 3);	//NG Down
			g_objCommon.Set_SortPicker2DownMulti(nSp2StartNo+1, nSp2DownSu);
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

			m_tSortPick2Loop.Takt_Start(nTaktZone, 13);
		}
		break;
	case 14:	// 정보전달, Vac Off
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 3) && g_objCommon.Get_SortPicker2DownMulti(nSp2StartNo+1, nSp2DownSu)) {
			for (int i = 0; i < nSp2DownSu; i++) {
				gData.InfoNgTray[nSp2WorkNg][nSp2TrayPosY][nSp2TrayPosX+i] = gData.InfoSortPick[1][nSp2StartNo+i]; 
				gData.InfoSortPick[1][nSp2StartNo+i] = 0;

				CString sInfo, sNgTray; 
				sInfo.Format("%d", gData.InfoNgTray[nSp2WorkNg][nSp2TrayPosY][nSp2TrayPosX+i]);
				int nTNo = gData.nTNoSortPick[1][nSp2StartNo+i];
				int nCNo = gData.nCNoSortPick[1][nSp2StartNo+i];
				if (gMes.sJudge[nSp2PNo-1][nTNo-1][nCNo-1] == "") {
					if		(gData.InfoNgTray[nSp2WorkNg][nSp2TrayPosY][nSp2TrayPosX+i] == 8) gMes.sJudge[nSp2PNo-1][nTNo-1][nCNo-1] = "N4";
					else if	(gData.InfoNgTray[nSp2WorkNg][nSp2TrayPosY][nSp2TrayPosX+i] == 7) gMes.sJudge[nSp2PNo-1][nTNo-1][nCNo-1] = "BS";
					else if	(gData.InfoNgTray[nSp2WorkNg][nSp2TrayPosY][nSp2TrayPosX+i] == 6) gMes.sJudge[nSp2PNo-1][nTNo-1][nCNo-1] = "N3";
					else if (gData.InfoNgTray[nSp2WorkNg][nSp2TrayPosY][nSp2TrayPosX+i] == 5) gMes.sJudge[nSp2PNo-1][nTNo-1][nCNo-1] = "N2";
					else																	  gMes.sJudge[nSp2PNo-1][nTNo-1][nCNo-1] = "N1";
				}
				g_objMES.Set_Result(gLot.sLotID[nSp2PNo-1], gMes.sBarID[nSp2PNo-1][nTNo-1][nCNo-1], gMes.sJudge[nSp2PNo-1][nTNo-1][nCNo-1], sInfo, gMes.sNGCode[nSp2PNo-1][nTNo-1][nCNo-1], nTNo, nCNo, 0,0,0,0);
				sNgTray.Format("NG-%d", nSp2WorkNg + 1);
				g_objLogFile.Save_OutTray(sNgTray, 1, nSp2TrayPosX+i, nSp2TrayPosY, gData.nPNoSortPick[1], gData.nTNoSortPick[1][nSp2StartNo+i], gData.nCNoSortPick[1][nSp2StartNo+i]);
				g_objLogFile.Save_CmTrackingLog("NG", nSp2WorkNg, nSp2TrayPosX+i, nSp2TrayPosY, gData.nPNoSortPick[1], gData.nTNoSortPick[1][nSp2StartNo+i], gData.nCNoSortPick[1][nSp2StartNo+i]);

				gData.nTNoSortPick[1][nSp2StartNo+i] = gData.nCNoSortPick[1][nSp2StartNo+i] = 0;
			}
			gData.nULPNo = gData.nPNoNgTray = gData.nPNoSortPick[1];
			if (Check_SortPickerEmpty(2)) gData.nPNoSortPick[1] = 0;
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 5, nSp2WorkNg);

			g_objCommon.Set_SortPicker2OpenMulti(nSp2StartNo+1, nSp2DownSu);
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

			m_tSortPick2Loop.Takt_End(nTaktZone,13);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 14);
		}
		break;
	case 15 :	// Picker Up
		if (g_objCommon.Get_SortPicker2OpenMulti(nSp2StartNo+1, nSp2DownSu)) {
			g_objCommon.Set_SortPicker2Up(0);
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	//Ready Up
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(5000);

			m_tSortPick2Loop.Takt_End(nTaktZone,14);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 15);
		}
		break;
	case 16:	// Position Check
		if (g_objCommon.Get_SortPicker2Up(0) && g_objCommon.Get_InfoSortPicker2Close() 
			&& g_objCommon.Get_InfoSortPicker2Check() && g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0)) {
			
			if (Check_NgTrayFull()) {
				if (m_nNgTrayCase == 10) m_nNgTrayCase = 11;
			}
			m_nSortPick2Case = 10; m_tSortPick2Loop.Set_LoopTime(5000);

			m_tSortPick2Loop.Takt_End(nTaktZone,15);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 16);
		}
		break;
	case 17:	// Wait
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER2_P, 0)) {

			// Good Module이 있는지 확인 후 어디로 갈지 정한다.
			if (Select_SortPickGoodPos(2, nSp2StartNo, nSp2PickCnt)) {
				if ((m_nSortPick1Case >=  0 && m_nSortPick1Case < 10) ||
					(m_nSortPick1Case >=  9 && m_nSortPick1Case < 20) ||
					(m_nSortPick1Case >= 40 && m_nSortPick1Case < 60) ||
					//(m_nSortPick1Case >= 60 && m_nSortPick1Case < 70) ||	// APD 확인중에도 갈수있다.
					(m_nSortPick1Case == 22) && (gData.nPNoSortPick[0] != gData.nPNoSortPick[1])||
					(m_nSortPick1Case == 27)) {

					if ((gData.nPNoSortPick[1] != gData.nPNoGoodTray) && (gData.nPNoSortPick[1] != 0)) {
						if (gData.nPNoGoodTray != 0) return TRUE;
					}
					m_nSortPick2Case = 20; m_tSortPick2Loop.Set_LoopTime(10000);

					m_tSortPick2Loop.Takt_End(nTaktZone,16);
					m_tSortPick2Loop.Takt_Start(nTaktZone, 17);
					m_tSortPick2Loop.Takt_End(nTaktZone,17);

				} else if (m_nSortPick1Case >= 21 && m_nSortPick1Case < 30) {
					m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

					m_tSortPick2Loop.Takt_End(nTaktZone,16);
					m_tSortPick2Loop.Takt_Start(nTaktZone, 17);
					m_tSortPick2Loop.Takt_End(nTaktZone,17);
				}
			} else {
				if (Check_InspectLotEnd(gData.nPNoNgTray, 2)) {
					if (Check_NgBufferLotEnd(gData.nPNoNgTray)) {	// NG Buffer에 모듈이 없으면 LotEnd 모듈이 있으면 포트 넘버 확인 후 언로딩 작업.
						if (m_pEquipData->bUseApdAlarm) {	// 2023.05.11+
							m_nSortPick2Case = 60; m_tSortPick2Loop.Set_LoopTime(10000); break;	// LotEnd Case
						}

						if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
						if(!m_tSortPick2Loop.Waiting_Time(300)) break;

						if (!Check_GoodTrayEmpty() && (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20)) {
							if (m_nGoodTray1Case == 20) nSp2WorkGood = 1;
							if (m_nGoodTray2Case == 20) nSp2WorkGood = 2;

							gData.sCLotID[nSp2WorkGood-1] = gData.sLotID[gData.nPNoGoodTray-1];
							gData.nCPortNo[nSp2WorkGood-1]= gData.nPNoGoodTray;
							gData.nCGoodTrayCount[nSp2WorkGood-1] = gData.nGoodStageTrayNo[nSp2WorkGood-1];	//gData.nGoodTrayCount;
							gData.nLastTrayNo = gData.nGoodStageTrayNo[nSp2WorkGood-1];
							gData.nCTrayCmCnt[nSp2WorkGood-1] = Get_GoodTrayCmCnt();
						}

						Job_LotEnd(gData.nPNoNgTray, nSp2WorkGood);
						if (m_pThreadBeep == NULL && m_pThreadNgFullBeep == NULL) {
							m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));

						} else {
							gData.bLotEndBeep = TRUE;
						}

						gData.bContinueLotEnd = TRUE;
						if (m_nNgTrayCase == 10) m_nNgTrayCase = 11; 
						if (!Check_GoodTrayEmpty()) {
							if (m_nGoodTray1Case == 20) { gData.bGoodTrayLotEnd[0] = TRUE; m_nGoodTray1Case = 21; }
							if (m_nGoodTray2Case == 20) { gData.bGoodTrayLotEnd[1] = TRUE; m_nGoodTray2Case = 21; }
						} else {
							// 랏종료시점에 대기중인 트레이가 빈트레이면 다음랏 모듈을 받아야 하기 때문에 트레이넘버를 1로 바꿔준다.
							if (m_nGoodTray1Case == 20) { gData.nGoodStageTrayNo[0] = 1; gData.nGoodTrayCount = 1; }
							if (m_nGoodTray2Case == 20) { gData.nGoodStageTrayNo[1] = 1; gData.nGoodTrayCount = 1; }

							// Inline Mode에서는 Unload 버튼을 사용하지 않는다.
							if (!m_pEquipData->bUseInlineMode) {
								if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
								if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case >  30) { gData.bGoodTrayWait = TRUE; }
								if (m_nGoodTray1Case >  30 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
							}
						}
						if (!gData.bGoodTrayLotEnd[0] && m_nGoodTray1Case > 20 && m_nGoodTray1Case < 30) { gData.bGoodTrayLotEnd[0] = TRUE; }
						if (!gData.bGoodTrayLotEnd[1] && m_nGoodTray2Case > 20 && m_nGoodTray2Case < 30) { gData.bGoodTrayLotEnd[1] = TRUE; }

					} else {
						if (!Check_NgBufferEmpty(2) && Check_SortPickerEmpty(2)) {
							bLastNgBuffPick2 = TRUE;
							m_nSortPick2Case = 50; m_tSortPick2Loop.Set_LoopTime(10000);

							m_tSortPick2Loop.Takt_End(nTaktZone,16);
							m_tSortPick2Loop.Takt_Start(nTaktZone, 17);
							m_tSortPick2Loop.Takt_End(nTaktZone,17);
							break;
						}
					}
				}
				if (Check_UnloadLotEnd() && !m_bUnloadLotEnd) { m_bUnloadLotEnd = TRUE; }

				m_strLog.Format("Sort Picker2, %d", GetTickCount() - m_dwSortPick2);
				g_objLogFile.Save_TestLog(m_strLog);

				CString strLog;
				CString strMsg = "Sort picker2 move to NG unloading + good unloading";
				dwSortPick2End = GetTickCount() - dwSortPick2Unload;
				strLog.Format("MCC,(%02d) %s,(%02d) %s,%0.3lf", 14, "SortPicker2", 13, strMsg, dwSortPick2End / 1000.0);
				g_objLogFile.Save_MCCLog(strLog);//g_objLogFile.Save_HandlerLog(strLog);

				if (nSp2WorkBuff != 1 && nSp2WorkBuff != 2) nSp2WorkBuff = 1;	// Test
				if (nSp2WorkBuff == 1) g_objCommon.Move_Position(AX_SORT_PICKER2_X, 0);	// Buffer1 Position
				if (nSp2WorkBuff == 2) g_objCommon.Move_Position(AX_SORT_PICKER2_X, 1);	// Buffer2 Position
				//g_objCommon.Set_SortPicker2Down(0);	// 복귀할때 실린더 다운해준다.

				if (m_nBuffTray1Case == 20)		 nSp2WorkBuff = 1;
				else if (m_nBuffTray2Case == 20) nSp2WorkBuff = 2;
				else							 nSp2WorkBuff = 0;

				if (!gData.bBuffStageMove && nSp2WorkBuff != 0 && Select_BufferTrayUpPos(nSp2WorkBuff, nSp2TrayPosX, nSp2TrayPosY)){	// 복귀할때 Y축 움직일수 있으면 움직여 주자.

					if (nSp2WorkBuff == 1 && m_nBuffTray1Case == 20) { 
						dSp2Y = m_pMoveData->dBufferStage1Y[3] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dSp2Y);
					}
					if (nSp2WorkBuff == 2 && m_nBuffTray2Case == 20) {
						dSp2Y = m_pMoveData->dBufferStage2Y[3] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dSp2Y);
					}
					gData.bBuffStageMove = TRUE;
				} else {
					if (gData.bBuffStageMove) gData.bBuffStageMove = FALSE;
				}

				m_nSortPick2Case = 0; m_tSortPick2Loop.Set_LoopTime(10000);

				m_tSortPick2Loop.Takt_End(nTaktZone,16);
				m_tSortPick2Loop.Takt_Start(nTaktZone, 17);
				m_tSortPick2Loop.Takt_End(nTaktZone,17);

			}
		}		
		return TRUE;

	case 18:	// Move to Good Tray1 Position
		if (g_objAJinAXL.Is_Done(AX_SORT_PICKER2_X)) {
			g_objCommon.Move_Position(AX_SORT_PICKER2_X, 2);	// Good1
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

			m_tSortPick2Loop.Takt_Start(nTaktZone, 18);
		}
		break;
	case 19:	// Check Position & Sort2
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_X, 2)) {	// Good1
			if ((m_nSortPick1Case >=  0 && m_nSortPick1Case < 10) ||
				(m_nSortPick1Case >=  9 && m_nSortPick1Case < 20) ||
				(m_nSortPick1Case >= 40 && m_nSortPick1Case < 60) ||
				//(m_nSortPick1Case >= 60 && m_nSortPick1Case < 70) ||	// APD 확인중에도 갈수있다.
				(m_nSortPick1Case == 27)) {

				if ((gData.nPNoSortPick[1] != gData.nPNoGoodTray) && (gData.nPNoSortPick[1] != 0)) {
					if (gData.nPNoGoodTray != 0) return TRUE;
				}
				m_nSortPick2Case = 20; m_tSortPick2Loop.Set_LoopTime(10000);

				m_tSortPick2Loop.Takt_End(nTaktZone,18);
				m_tSortPick2Loop.Takt_Start(nTaktZone, 19);
			}
		}
		return TRUE;

	// Good Unload
	case 20:	// Check Good Tray
		if (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20) {
			m_nSortPick2Case = 22; m_tSortPick2Loop.Set_LoopTime(10000); 
		} else {
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
			if (Check_SortPickerEmpty(2)) {				
				m_nSortPick2Case = 27; m_tSortPick2Loop.Set_LoopTime(10000);

				m_tSortPick2Loop.Takt_End(nTaktZone,19);
				m_tSortPick2Loop.Takt_Start(nTaktZone, 20);
				m_tSortPick2Loop.Takt_End(nTaktZone,20);

			} else {
				if (Select_SortPickGoodPos(2, nSp2StartNo, nSp2PickCnt)) {
					if (!(g_objCommon.Get_InfoSortPicker2Close() && g_objCommon.Get_InfoSortPicker2Check())) break;
					// 한Case에서 오는게 아니라 따로 추출해서 입력해준다.
					g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
					g_objCommon.Move_Position(AX_SORT_PICKER2_X, 2);	// Good1
					m_nSortPick2Case = 21; m_tSortPick2Loop.Set_LoopTime(10000);

					m_tSortPick2Loop.Takt_End(nTaktZone,19);
					m_tSortPick2Loop.Takt_Start(nTaktZone, 20);
					m_tSortPick2Loop.Takt_End(nTaktZone,20);

				} else {
					m_nSortPick2Case = 27; m_tSortPick2Loop.Set_LoopTime(10000);

					m_tSortPick2Loop.Takt_End(nTaktZone,19);
					m_tSortPick2Loop.Takt_Start(nTaktZone, 20);
					m_tSortPick2Loop.Takt_End(nTaktZone,20);
				}
			}
		}
		break;
	case 21:	// Check Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_X, 2)) {	// Good1
			if (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20) { m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000); }
			else if (Check_UnloadLotEnd()) { 
				g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
				m_nSortPick2Case = 27; m_tSortPick2Loop.Set_LoopTime(10000);

				m_tSortPick2Loop.Takt_Start(nTaktZone, 21);
				m_tSortPick2Loop.Takt_End(nTaktZone,21);
			}
		}
		return TRUE;

	case 22:	// Move to Good Unload Position
		if ((gData.nPNoSortPick[1] != gData.nPNoGoodTray) && (gData.nPNoSortPick[1] != 0)) {
			if (gData.nPNoGoodTray != 0) return TRUE;
		}
		if (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20) {
			if (m_nGoodTray1Case == 20) nSp2WorkGood = 1;
			if (m_nGoodTray2Case == 20) nSp2WorkGood = 2;

			if ((nSp2WorkGood == 1 && g_objAJinAXL.Is_Done(AX_GOOD_STAGE1_Y)) || 
				(nSp2WorkGood == 2 && g_objAJinAXL.Is_Done(AX_GOOD_STAGE2_Y))) {
				if (Select_GoodTrayPos(nSp2TrayPosX, nSp2TrayPosY)) {
					
					if (Select_SortPickGoodPos(2, nSp2StartNo, nSp2PickCnt)) {
						if (!Select_GoodTrayPos(nSp2TrayPosX, nSp2TrayPosY)) nSp2TrayPosX = 0;	// Tray 교체중...
						nSp2TrayCnt = gData.nTrayX - nSp2TrayPosX;
						nSp2DownSu = ((nSp2PickCnt < nSp2TrayCnt) ? nSp2PickCnt : nSp2TrayCnt);
						m_nSortPick2MultiCnt = nSp2DownSu;

						if (nSp2WorkGood == 1) dSp2X = m_pMoveData->dSortPicker2X[2] + (nSp2TrayPosX - nSp2StartNo) * m_pEquipData->dTrayPitchX;
						if (nSp2WorkGood == 2) dSp2X = m_pMoveData->dSortPicker2X[3] + (nSp2TrayPosX - nSp2StartNo) * m_pEquipData->dTrayPitchX;
						g_objAJinAXL.Move_Absolute(AX_SORT_PICKER2_X, dSp2X);

						if (nSp2WorkGood == 1 && m_nGoodTray1Case == 20) {
							dSp2Y = m_pMoveData->dGoodStage1Y[2] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
							g_objAJinAXL.Move_Absolute(AX_GOOD_STAGE1_Y, dSp2Y);
						}
						if (nSp2WorkGood == 2 && m_nGoodTray2Case == 20) {
							dSp2Y = m_pMoveData->dGoodStage2Y[2] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
							g_objAJinAXL.Move_Absolute(AX_GOOD_STAGE2_Y, dSp2Y);
						}
						m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

						
						m_tSortPick2Loop.Takt_Start(nTaktZone, 22);
						m_tSortPick2Loop.Takt_End(nTaktZone,22);


					} else {
						g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
						m_nSortPick2Case = 27; m_tSortPick2Loop.Set_LoopTime(10000);

						m_tSortPick2Loop.Takt_Start(nTaktZone, 22);
						m_tSortPick2Loop.Takt_End(nTaktZone,22);
					}
				}
			} else {
				break;
			}
		}
		return TRUE;
	case 23:	// Picker Down
		if (g_objAJinAXL.Is_Done(AX_SORT_PICKER2_Z) && g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER2_X, dSp2X) &&
			((nSp2WorkGood == 1 && g_objAJinAXL.Is_MoveDone(AX_GOOD_STAGE1_Y, dSp2Y)) ||
			 (nSp2WorkGood == 2 && g_objAJinAXL.Is_MoveDone(AX_GOOD_STAGE2_Y, dSp2Y))) )
		{
			
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 2);	//Good Down
			g_objCommon.Set_SortPicker2DownMulti(nSp2StartNo+1, nSp2DownSu);
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
			
			m_tSortPick2Loop.Takt_Start(nTaktZone, 23);
		}
		break;
	case 24:	// 정보전달, Vac Off
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 2) && g_objCommon.Get_SortPicker2DownMulti(nSp2StartNo+1, nSp2DownSu)) {
			for (int i = 0; i < nSp2DownSu; i++) {
				gData.InfoGoodTray[nSp2TrayPosY][nSp2TrayPosX+i] = gData.InfoSortPick[1][nSp2StartNo+i]; 
				gData.InfoSortPick[1][nSp2StartNo+i] = 0;

				CString sInfo; 
				sInfo.Format("%d", gData.InfoGoodTray[nSp2TrayPosY][nSp2TrayPosX+i]);
				int nTNo = gData.nTNoSortPick[1][nSp2StartNo+i];
				int nCNo = gData.nCNoSortPick[1][nSp2StartNo+i];
				int nGTNo = gData.nGoodStageTrayNo[nSp2WorkGood-1];	//gData.nGoodTrayCount;
				int nGCNo = (gData.nTrayX*nSp2TrayPosY) + nSp2TrayPosX + i + 1;
				g_objCapAttach.Set_BarcodeUpdate(nSp2PNo, nGTNo, nGCNo, gMes.sBarID[nSp2PNo-1][nTNo-1][nCNo-1]);
				g_objMES.Set_Result(gLot.sLotID[nSp2PNo-1], gMes.sBarID[nSp2PNo-1][nTNo-1][nCNo-1], "OK", sInfo, gMes.sNGCode[nSp2PNo-1][nTNo-1][nCNo-1], nTNo, nCNo, 0,0,0,0);
				g_objLogFile.Save_OutTray("GOOD", gData.nGoodTrayCount, nSp2TrayPosX+i, nSp2TrayPosY, gData.nPNoSortPick[1], gData.nTNoSortPick[1][nSp2StartNo+i], gData.nCNoSortPick[1][nSp2StartNo+i]);
				g_objLogFile.Save_CmTrackingLog("GOOD", gData.nGoodTrayCount, nSp2TrayPosX+i, nSp2TrayPosY, gData.nPNoSortPick[1], gData.nTNoSortPick[1][nSp2StartNo+i], gData.nCNoSortPick[1][nSp2StartNo+i]);

				gData.nTNoSortPick[1][nSp2StartNo+i] = gData.nCNoSortPick[1][nSp2StartNo+i] = 0;
			}
			gData.nULPNo = gData.nPNoGoodTray = gData.nPNoSortPick[1]; 
			if (Check_SortPickerEmpty(2)) gData.nPNoSortPick[1] = 0;
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 4, nSp2WorkGood-1);

			g_objCommon.Set_SortPicker2OpenMulti(nSp2StartNo+1, nSp2DownSu);
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

			m_tSortPick2Loop.Takt_End(nTaktZone, 23);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 24);
		}
		break;
	case 25 :	// Picker Up
		if (g_objCommon.Get_SortPicker2OpenMulti(nSp2StartNo+1, nSp2DownSu)) {
			g_objCommon.Set_SortPicker2Up(0);
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	//Ready Up
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(5000);

			m_tSortPick2Loop.Takt_End(nTaktZone, 24);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 25);
		}
		break;
	case 26:	// Picker Check
		if (g_objCommon.Get_SortPicker2Up(0) && g_objCommon.Get_InfoSortPicker2Close() && g_objCommon.Get_InfoSortPicker2Check()) {
			
			if (Check_GoodTrayFull()) {
				if (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20) {
					if (m_nGoodTray1Case == 20) nSp2WorkGood = 1;
					if (m_nGoodTray2Case == 20) nSp2WorkGood = 2;

					gData.sCLotID[nSp2WorkGood-1] = gData.sLotID[gData.nPNoGoodTray-1];
					gData.nCPortNo[nSp2WorkGood-1] = gData.nPNoGoodTray;
					gData.nCGoodTrayCount[nSp2WorkGood-1] = gData.nGoodStageTrayNo[nSp2WorkGood-1];	//gData.nGoodTrayCount;
					gData.nLastTrayNo = gData.nGoodStageTrayNo[nSp2WorkGood-1];
					gData.nCTrayCmCnt[nSp2WorkGood-1] = Get_GoodTrayCmCnt();
				}
				if (m_nGoodTray1Case == 20) m_nGoodTray1Case = 21;
				if (m_nGoodTray2Case == 20) m_nGoodTray2Case = 21;
			}
			if (Check_SortPickerEmpty(2)){
				g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
				m_nSortPick2Case = 27; m_tSortPick2Loop.Set_LoopTime(5000);

				m_tSortPick2Loop.Takt_End(nTaktZone, 25);
				m_tSortPick2Loop.Takt_Start(nTaktZone, 26);
				m_tSortPick2Loop.Takt_End(nTaktZone, 26);
			} else {
				m_nSortPick2Case = 20; m_tSortPick2Loop.Set_LoopTime(5000);
				m_tSortPick2Loop.Takt_End(nTaktZone, 25);
				m_tSortPick2Loop.Takt_Start(nTaktZone, 26);
				m_tSortPick2Loop.Takt_End(nTaktZone, 26);
			}
		}
		break;
	case 27:	// Position Check
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0)) {
			// 한Case에서 오는게 아니라 따로 추출해서 입력해준다.
			if (Check_SortPickerEmpty(2)){
				if (!gData.bNGTrayWait && Check_NgBufferFull(2)) {
					m_nSortPick2Case = 50; m_tSortPick2Loop.Set_LoopTime(10000);

					m_tSortPick2Loop.Takt_Start(nTaktZone, 27);
					m_tSortPick2Loop.Takt_End(nTaktZone, 27);

				} else {
					m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
					m_tSortPick2Loop.Takt_Start(nTaktZone, 27);
					m_tSortPick2Loop.Takt_End(nTaktZone, 27);
				}
			} else {
				if (Select_SortPickNgPos(2, nSp2StartNo, nSp2PickCnt)) {	// startNo 0base pickCnt 1base
					m_nSortPick2Case = 7; m_tSortPick2Loop.Set_LoopTime(10000);
					m_tSortPick2Loop.Takt_Start(nTaktZone, 27);
					m_tSortPick2Loop.Takt_End(nTaktZone, 27);
				}
			}
		}
		break;
	case 28:	// Buffer 복귀
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0)) {
			if (Check_InspectLotEnd(gData.nULPNo, 2)) {
				if (Check_NgBufferLotEnd(gData.nULPNo)) {	// NG Buffer에 모듈이 없으면 LotEnd 모듈이 있으면 포트 넘버 확인 후 언로딩 작업.
					if (m_pEquipData->bUseApdAlarm) {	// 2023.05.11+
						m_nSortPick2Case = 60; m_tSortPick2Loop.Set_LoopTime(10000); break;	// LotEnd Case

						m_tSortPick2Loop.Takt_Start(nTaktZone, 28);
						m_tSortPick2Loop.Takt_End(nTaktZone, 28);
					}

					if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
					if(!m_tSortPick2Loop.Waiting_Time(300)) break;
					if (!Check_GoodTrayEmpty() && (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20)) {
						if (m_nGoodTray1Case == 20) nSp2WorkGood = 1;
						if (m_nGoodTray2Case == 20) nSp2WorkGood = 2;

						gData.sCLotID[nSp2WorkGood-1] = gData.sLotID[gData.nPNoGoodTray-1];
						gData.nCPortNo[nSp2WorkGood-1] = gData.nPNoGoodTray;
						gData.nCGoodTrayCount[nSp2WorkGood-1] = gData.nGoodStageTrayNo[nSp2WorkGood-1];	//gData.nGoodTrayCount;
						gData.nLastTrayNo = gData.nGoodStageTrayNo[nSp2WorkGood-1];
						gData.nCTrayCmCnt[nSp2WorkGood-1] = Get_GoodTrayCmCnt();
					}
					Job_LotEnd(gData.nULPNo, nSp2WorkGood);
					if (m_pThreadBeep == NULL && m_pThreadNgFullBeep == NULL) {						
						m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));

					} else {
						gData.bLotEndBeep = TRUE;
					}

					gData.bContinueLotEnd = TRUE;
					if (m_nNgTrayCase == 10) m_nNgTrayCase = 11; 
					if (!Check_GoodTrayEmpty()) {
						if (m_nGoodTray1Case == 20) { gData.bGoodTrayLotEnd[0]= TRUE; m_nGoodTray1Case = 21; }
						if (m_nGoodTray2Case == 20) { gData.bGoodTrayLotEnd[1]= TRUE; m_nGoodTray2Case = 21; }
					}  else {
						// Inline Mode에서는 Unload 버튼을 사용하지 않는다.
						if (!m_pEquipData->bUseInlineMode) {
							if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
							if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case >  30) { gData.bGoodTrayWait = TRUE; }
							if (m_nGoodTray1Case >  30 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
						}
					}
					if (!gData.bGoodTrayLotEnd[0] && m_nGoodTray1Case > 20 && m_nGoodTray1Case < 30) { gData.bGoodTrayLotEnd[0] = TRUE; }
					if (!gData.bGoodTrayLotEnd[1] && m_nGoodTray2Case > 20 && m_nGoodTray2Case < 30) { gData.bGoodTrayLotEnd[1] = TRUE; }

				} else {
					if (!Check_NgBufferEmpty(2) && Check_SortPickerEmpty(2)) {
						bLastNgBuffPick2 = TRUE;
						m_nSortPick2Case = 50; m_tSortPick2Loop.Set_LoopTime(10000);

						m_tSortPick2Loop.Takt_Start(nTaktZone, 28);
						m_tSortPick2Loop.Takt_End(nTaktZone, 28);
						break;
					}
				}
			}
			if (Check_UnloadLotEnd() && !m_bUnloadLotEnd) { m_bUnloadLotEnd = TRUE; }

			m_strLog.Format("Sort Picker2, %d", GetTickCount() - m_dwSortPick2);
			g_objLogFile.Save_TestLog(m_strLog);

			CString strLog;
			CString strMsg = "Sort picker2 move to NG unloading + good unloading";
			dwSortPick2End = GetTickCount() - dwSortPick2Unload;
			strLog.Format("MCC,(%02d) %s,(%02d) %s,%0.3lf", 14, "SortPicker2", 13, strMsg, dwSortPick2End / 1000.0);
			g_objLogFile.Save_MCCLog(strLog);//g_objLogFile.Save_HandlerLog(strLog);

			if (nSp2WorkBuff != 1 && nSp2WorkBuff != 2) nSp2WorkBuff = 1;	// Test
			if (nSp2WorkBuff == 1) g_objCommon.Move_Position(AX_SORT_PICKER2_X, 0);	// Buffer1 Position
			if (nSp2WorkBuff == 2) g_objCommon.Move_Position(AX_SORT_PICKER2_X, 1);	// Buffer2 Position
			//g_objCommon.Set_SortPicker2Down(0);	// 복귀할때 실린더 다운해준다.

			if (m_nBuffTray1Case == 20)		 nSp2WorkBuff = 1;
			else if (m_nBuffTray2Case == 20) nSp2WorkBuff = 2;
			else							 nSp2WorkBuff = 0;

			if (!gData.bBuffStageMove && nSp2WorkBuff != 0 && Select_BufferTrayUpPos(nSp2WorkBuff, nSp2TrayPosX, nSp2TrayPosY)){	// 복귀할때 Y축 움직일수 있으면 움직여 주자.

				if (nSp2WorkBuff == 1 && m_nBuffTray1Case == 20) { 
					dSp2Y = m_pMoveData->dBufferStage1Y[3] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
					g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dSp2Y);
				}
				if (nSp2WorkBuff == 2 && m_nBuffTray2Case == 20) {
					dSp2Y = m_pMoveData->dBufferStage2Y[3] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
					g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dSp2Y);
				}
				gData.bBuffStageMove = TRUE;
			} else {
				if (gData.bBuffStageMove) gData.bBuffStageMove = FALSE;
			}

			m_nSortPick2Case = 0; m_tSortPick2Loop.Set_LoopTime(10000);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 28);
			m_tSortPick2Loop.Takt_End(nTaktZone, 28);
		}		
		break;

	// NG Buffer place
	case 40:	// Move to NG Buffer Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objAJinAXL.Is_Done(AX_SORT_PICKER2_X) && g_objAJinAXL.Is_Done(AX_SORT_PICKER2_P)) {
			if (Select_SortPickNgPos(2, nSp2StartNo, nSp2PickCnt, TRUE)) {	// startNo 0base pickCnt 1base
				if (Select_NgBufferPos(2, nSp2TrayPosX)) {	// Ng Buffer에 자리가 있을때
					// 한Case에서 오는게 아니라 따로 추출해서 입력해준다.
							
					nSp2TrayCnt = gData.nSortPickQt - nSp2TrayPosX;
					nSp2DownSu = ((nSp2PickCnt < nSp2TrayCnt) ? nSp2PickCnt : nSp2TrayCnt);
					m_nSortPick2MultiCnt = nSp2DownSu;

					dSp2X = m_pMoveData->dSortPicker2X[6] + (nSp2TrayPosX - nSp2StartNo) * m_pEquipData->dNgBufferPitchX;

					g_objAJinAXL.Move_Absolute(AX_SORT_PICKER2_X, dSp2X);
					g_objCommon.Move_Position(AX_SORT_PICKER2_P, 1);	// NG Buffer Pitch
					m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

					m_tSortPick2Loop.Takt_Start(nTaktZone, 40);
					m_tSortPick2Loop.Takt_End(nTaktZone, 40);

				} else {	// Ng Buffer에 자리가 없고 Picker에 Ng가 있을때
					g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
					g_objCommon.Move_Position(AX_SORT_PICKER2_P, 0);	// Tray Pitch
					m_nSortPick2Case = 10; m_tSortPick2Loop.Set_LoopTime(10000);

					m_tSortPick2Loop.Takt_Start(nTaktZone, 40);
					m_tSortPick2Loop.Takt_End(nTaktZone, 40);
				}

			} else {	// Picker에 Ng가 없을때
				g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
				g_objCommon.Move_Position(AX_SORT_PICKER2_P, 0);	// Tray Pitch
				m_nSortPick2Case = 17; m_tSortPick2Loop.Set_LoopTime(10000);

				m_tSortPick2Loop.Takt_Start(nTaktZone, 40);
				m_tSortPick2Loop.Takt_End(nTaktZone, 40);
			}
		}
		break;
	case 41:	// Check Position
		if (g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER2_X, dSp2X) && g_objCommon.Check_Position(AX_SORT_PICKER2_P, 1)) {
			if (!(g_objCommon.Get_InfoSortPicker2Close() && g_objCommon.Get_InfoSortPicker2Check())) break;
			
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 4);	//NG Buffer Down
			g_objCommon.Set_SortPicker2DownMulti(nSp2StartNo+1, nSp2DownSu);
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

			m_tSortPick2Loop.Takt_Start(nTaktZone, 41);
		}
		break;
	case 42:	// 정보전달, Vac Off	// Sort Picker = 0, NG Buffer = 1
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 4) && g_objCommon.Get_SortPicker2DownMulti(nSp2StartNo+1, nSp2DownSu)) {
			for (int i = 0; i < nSp2DownSu; i++) {
				gData.InfoNgBuffer[0][nSp2TrayPosX+i] = gData.InfoSortPick[1][nSp2StartNo+i]; 
				gData.nTNoNgBuffer[0][nSp2TrayPosX+i] = gData.nTNoSortPick[1][nSp2StartNo+i];
				gData.nCNoNgBuffer[0][nSp2TrayPosX+i] = gData.nCNoSortPick[1][nSp2StartNo+i];

				gData.InfoSortPick[1][nSp2StartNo+i] = gData.nTNoSortPick[1][nSp2StartNo+i] = gData.nCNoSortPick[1][nSp2StartNo+i] = 0;
			}
			gData.nPNoNgBuffer[0] = gData.nPNoSortPick[1];
			if (Check_SortPickerEmpty(2)) gData.nPNoSortPick[1] = 0;

			g_objCommon.Set_InfoNgBufferVacOn(2);
			g_objCommon.Set_SortPicker2OpenMulti(nSp2StartNo+1, nSp2DownSu);
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

			m_tSortPick2Loop.Takt_End(nTaktZone, 41);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 42);
			

		}
		break;
	case 43 :	// Picker Up
		if (g_objCommon.Get_InfoNgBufferVacOn(2) && g_objCommon.Get_SortPicker2OpenMulti(nSp2StartNo+1, nSp2DownSu)) {
			g_objCommon.Set_SortPicker2Up(0);
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(5000);

			m_tSortPick2Loop.Takt_End(nTaktZone, 42);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 43);

		}
		break;
	case 44:	// Positon Check
		if (g_objCommon.Get_SortPicker2Up(0) && g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Get_InfoSortPicker2Check()) {
			
			m_tSortPick2Loop.Takt_End(nTaktZone, 43);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 44);
			m_tSortPick2Loop.Takt_End(nTaktZone, 44);

			if (Select_SortPickNgPos(2, nSp2StartNo, nSp2PickCnt, TRUE)) {
				if (Check_NgBufferFull(2)) {
					g_objCommon.Move_Position(AX_SORT_PICKER2_P, 0);	// Tray Pitch
					m_nSortPick2Case = 7; m_tSortPick2Loop.Set_LoopTime(5000);

				} else {
					m_nSortPick2Case = 40; m_tSortPick2Loop.Set_LoopTime(5000);
				}
			} else {
				g_objCommon.Move_Position(AX_SORT_PICKER2_P, 0);	// Tray Pitch
				m_nSortPick2Case = 17; m_tSortPick2Loop.Set_LoopTime(10000);
			}			
		}
		break;

	// NG Buffer Pickup
	case 50:
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objAJinAXL.Is_Done(AX_SORT_PICKER2_X) && g_objAJinAXL.Is_Done(AX_SORT_PICKER2_P)) {
			if (Check_SortPickerEmpty(2)) {
				// 한Case에서 오는게 아니라 따로 추출해서 입력해준다.
				g_objCommon.Move_Position(AX_SORT_PICKER2_X, 6);	// NG Buffer Position
				g_objCommon.Move_Position(AX_SORT_PICKER2_P, 1);	// NG Buffer Pitch
				m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

				m_tSortPick2Loop.Takt_Start(nTaktZone, 50);
				
			}
		}
		break;
	case 51:	// Check Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_X, 6) && g_objCommon.Check_Position(AX_SORT_PICKER2_P, 1)) {
			
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 4);	//NG Buffer Down
			g_objCommon.Set_InfoSortPicker2Down(1);
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

			m_tSortPick2Loop.Takt_End(nTaktZone, 50);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 51);
				
		}
		break;
	case 52:	// 정보전달, Vac Off
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 4) && g_objCommon.Get_InfoSortPicker2Down(1)) {
			for (int i = 0; i < gData.nSortPickQt; i++) {
				gData.InfoSortPick[1][i] = gData.InfoNgBuffer[0][i]; 
				gData.nTNoSortPick[1][i] = gData.nTNoNgBuffer[0][i];
				gData.nCNoSortPick[1][i] = gData.nCNoNgBuffer[0][i];

				gData.InfoNgBuffer[0][i] = gData.nTNoNgBuffer[0][i] = gData.nCNoNgBuffer[0][i] = 0;
			}
			gData.nPNoSortPick[1] = gData.nPNoNgBuffer[0]; gData.nPNoNgBuffer[0] = 0;

			g_objCommon.Set_NgBufferVacOff(2);
			g_objCommon.Set_InfoSortPicker2Close();
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

			m_tSortPick2Loop.Takt_End(nTaktZone, 51);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 52);

		}
		break;
	case 53 :	// Picker Up
		if (g_objCommon.Get_InfoSortPicker2Close()) {
			if (!m_tSortPick2Loop.Waiting_Time(m_pEquipData->nDelayAdd[2])) break;	// SortPicker Delay
			g_objCommon.Set_SortPicker2Up(0);
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(5000);

			m_tSortPick2Loop.Takt_End(nTaktZone, 52);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 53);
		}
		break;
	case 54:
		if (g_objCommon.Get_InfoSortPicker2Close() && g_objCommon.Get_InfoSortPicker2Check() &&
			g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Get_SortPicker2Up(0))
		{
			
			g_objCommon.Move_Position(AX_SORT_PICKER2_P, 0);	// Tray Pitch
			m_nSortPick2Case = 7; m_tSortPick2Loop.Set_LoopTime(5000);

			m_tSortPick2Loop.Takt_End(nTaktZone, 53);
			m_tSortPick2Loop.Takt_Start(nTaktZone, 54);
			m_tSortPick2Loop.Takt_End(nTaktZone, 54);
		}
		break;

	// APD 파일 저장, MES 보고 및 APD 결과파일 확인
	case 60:	// APD 사용 유무 확인, Vision, Cap APD Data 요청
		if (m_pEquipData->bUseApdAlarm) {
			g_objInspector.Set_ApdRequest(INSPECTOR_PC1, "AG", nSp2PNo);
			g_objInspector.Set_ApdRequest(INSPECTOR_PC1, "B1", nSp2PNo);
			g_objInspector.Set_ApdRequest(INSPECTOR_PC2, "T1", nSp2PNo);
			g_objInspector.Set_ApdRequest(INSPECTOR_PC3, "T2", nSp2PNo);
			g_objInspector.Set_ApdRequest(INSPECTOR_PC4, "B2", nSp2PNo);
			g_objCapAttach.Set_ApdRequest();
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

		} else {
			if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
			gData.bAPDResultErr = FALSE;
			m_nSortPick2Case = 63; m_tSortPick2Loop.Set_LoopTime(10000);
		}
		break;
	case 61:	// 모든 APD 값이 들어오면 Data 기록 후 MES 보고
		if (gData.bAviApdReceive[0] && gData.bAviApdReceive[1] && gData.bAviApdReceive[2] &&
			gData.bAviApdReceive[3] && gData.bAviApdReceive[4] && (gData.dAssyLoadCellAvg + 0.1 > 0))
		{
			g_objMES.Clear_APDResult();	// APD 결과 폴더에 파일이 있다면 삭제.
			g_objMES.Save_AviApdData(gData.sLotID[nSp2PNo-1], nSp2PNo, gData.sOperID);	// APD 파일 생성 및 MES 보고
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(5000);

		} else if (!m_pEquipData->bUseApdAlarm) {
			if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
			gData.bAPDResultErr = FALSE;
			m_nSortPick2Case = 63; m_tSortPick2Loop.Set_LoopTime(10000);
		}
		break;
	case 62:	// 결과 파일 읽기 확인, LotEnd 전 나머지 모듈 검사 결과 APD 파일 저장.
		if (g_objMES.Read_APDResult(gData.sLotID[nSp2PNo-1])) {			
			if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(5000);

		} else if (!m_pEquipData->bUseApdAlarm) {
			if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
			gData.bAPDResultErr = FALSE;
			m_nSortPick2Case = 63; m_tSortPick2Loop.Set_LoopTime(10000);
		}
		break;		
	case 63:	// 항목 결과에 따라 Lot End처리.
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER2_P, 0)) {
			if(!m_tSortPick2Loop.Waiting_Time(300)) break;	// 모듈 검사 결과 APD 파일 저장하고 일정시간 기다렸다가 Lot End 해줘야한다. 

			if (!Check_GoodTrayEmpty() && (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20)) {
				if (m_nGoodTray1Case == 20) nSp2WorkGood = 1;
				if (m_nGoodTray2Case == 20) nSp2WorkGood = 2;

				gData.sCLotID[nSp2WorkGood-1] = gData.sLotID[nSp2PNo-1];
				gData.nCPortNo[nSp2WorkGood-1] = nSp2PNo;
				gData.nCGoodTrayCount[nSp2WorkGood-1] = gData.nGoodStageTrayNo[nSp2WorkGood-1];	//gData.nGoodTrayCount;
				gData.nLastTrayNo = gData.nGoodStageTrayNo[nSp2WorkGood-1];
				gData.nCTrayCmCnt[nSp2WorkGood-1] = Get_GoodTrayCmCnt();
			}
			Job_LotEnd(nSp2PNo, nSp2WorkGood);
			if (m_pThreadBeep == NULL && m_pThreadNgFullBeep == NULL) {
				m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));

			} else {
				gData.bLotEndBeep = TRUE;
			}

			gData.bContinueLotEnd = TRUE;
			if (m_nNgTrayCase == 10) m_nNgTrayCase = 11; 
			if (!Check_GoodTrayEmpty()) {
				if (m_nGoodTray1Case == 20) { gData.bGoodTrayLotEnd[0] = TRUE; m_nGoodTray1Case = 21; }
				if (m_nGoodTray2Case == 20) { gData.bGoodTrayLotEnd[1] = TRUE; m_nGoodTray2Case = 21; }
			} else {
				// 랏종료시점에 대기중인 트레이가 빈트레이면 다음랏 모듈을 받아야 하기 때문에 트레이넘버를 1로 바꿔준다.
				if (m_nGoodTray1Case == 20) { gData.nGoodStageTrayNo[0] = 1; gData.nGoodTrayCount = 1; }
				if (m_nGoodTray2Case == 20) { gData.nGoodStageTrayNo[1] = 1; gData.nGoodTrayCount = 1; }

				// Inline Mode에서는 Unload 버튼을 사용하지 않는다.
				if (!m_pEquipData->bUseInlineMode) {
					if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
					if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case >  30) { gData.bGoodTrayWait = TRUE; }
					if (m_nGoodTray1Case >  30 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
				}
			}
			if (!gData.bGoodTrayLotEnd[0] && m_nGoodTray1Case > 20 && m_nGoodTray1Case < 30) { gData.bGoodTrayLotEnd[0] = TRUE; }
			if (!gData.bGoodTrayLotEnd[1] && m_nGoodTray2Case > 20 && m_nGoodTray2Case < 30) { gData.bGoodTrayLotEnd[1] = TRUE; }

			/////
			if (Check_UnloadLotEnd() && !m_bUnloadLotEnd) { m_bUnloadLotEnd = TRUE; }

			m_strLog.Format("Sort Picker2, %d", GetTickCount() - m_dwSortPick2);
			g_objLogFile.Save_TestLog(m_strLog);

			CString strLog;
			CString strMsg = "Sort picker2 move to NG unloading + good unloading";
			dwSortPick2End = GetTickCount() - dwSortPick2Unload;
			strLog.Format("MCC,(%02d) %s,(%02d) %s,%0.3lf", 14, "SortPicker2", 13, strMsg, dwSortPick2End / 1000.0);
			g_objLogFile.Save_HandlerLog(strLog);

			if (nSp2WorkBuff != 1 && nSp2WorkBuff != 2) nSp2WorkBuff = 1;	// Test
			if (nSp2WorkBuff == 1) g_objCommon.Move_Position(AX_SORT_PICKER2_X, 0);	// Buffer1 Position
			if (nSp2WorkBuff == 2) g_objCommon.Move_Position(AX_SORT_PICKER2_X, 1);	// Buffer2 Position

			if (gData.bAPDResultErr) m_nSortPick2Case++;	// Alarm 처리. Machine Stop
			else					 m_nSortPick2Case = 0;	// Lot End 완료.
			m_tSortPick2Loop.Set_LoopTime(5000);
		}
		break;
	case 64:	// 항목 결과중 NG 있으면 알람 처리
		m_nSortPick2Case = 0;
		g_objCommon.Show_Error(4464);
		return FALSE;
	}

	// 14. (Error : 4400)
	if (m_tSortPick2Loop.Over_LoopTime()) {
		if (m_nSortPick2Case == 5) {
			gData.nErrBufferStageNo = nSp2WorkBuff;
		} else { gData.nErrBufferStageNo = 0; }

		if (m_nSortPick2Case == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectError(gData.nPNoSortPick[1], gData.nTNoSortPick[1][i], gData.nCNoSortPick[1][i]); }
		}
		g_objCommon.Show_Error(4400 + m_nSortPick2Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 15. (Error : 4500)
BOOL CSequenceMain::GoodTray1_Run()
{
	int nTaktZone = 15;		// Takt_Start, Takt_End
	static BOOL	bgs1EmptyTray = FALSE;

	// 양품 Tray 배출 시 도어락 및 버튼 확인 필요
	if (gData.bGoodTrayWait && m_nGoodTray1Case > 24 && m_nGoodTray1Case < 50) return TRUE;

	switch (m_nGoodTray1Case) {
	case 0:		// Start 하면 1로 바꿔준다.
		return TRUE;

	case 1:		// Tray Check
		if (m_bUnloadLotEnd) {
			m_nGoodTray1Case = 0;
		} else if (!m_pDX12->iGoodStage1Exist && g_objCommon.Get_GoodTray1MasterSlaveOut()) {
			if (m_pDX12->iGoodTrayBufferBottom && m_nEmptyTrayYCase < 21) {
				m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
			} else {
				m_tGoodTray1Loop.Set_LoopTime(5000); return TRUE;
			}
		} 
		break;
	case 2:		// Position Check & Z Axis Move to Load Support Up Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1)) {	// Good Tray Z축 기본위치 확인 필요.
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 2);	// Support Up
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 3:	// Good Tray Buffer Support Out
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 2)) {
			m_pDY12->oGoodTrayBuffSupport1In = FALSE; m_pDY12->oGoodTrayBuffSupport2In = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 4:	// Support Down
		if (m_pDX12->iGoodTrayBuffSupport1Out && m_pDX12->iGoodTrayBuffSupport2Out) {
			if (m_tGoodTray1Loop.Waiting_Time(100)) break;

			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 4);	// Support Down
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 5:	// Support In
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 4)) {
			m_pDY12->oGoodTrayBuffSupport1In = TRUE; m_pDY12->oGoodTrayBuffSupport2In = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 6:	// Master In
		if (m_pDX12->iGoodTrayBuffSupport1In && m_pDX12->iGoodTrayBuffSupport2In) {
			m_pDY12->oGoodStage1MasterIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 7:	// Slave In
		if (m_pDX12->iGoodStage1MasterIn && !m_pDX12->iGoodStage1MasterOut) {
			m_pDY12->oGoodStage1SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 8:	// Moving Up
		if (g_objCommon.Get_GoodTray1MasterSlaveIn()) {
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 1);
			m_nGoodTray1Case = 11; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;

// 	case 10:	// Wait for Empty Tray Loading
// 		if (m_bUnloadLotEnd && !m_pDX12->iGoodStage1Exist) { m_nGoodTray1Case = 80; }
// 		return TRUE;
	case 11:	// Master In
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1) && m_pDX12->iGoodStage1Exist) {
			m_dwGoodTray1 = GetTickCount();
			m_tGoodTray1Loop.Takt_Start(nTaktZone, 1, TRUE);
			m_pDY12->oGoodStage1MasterIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 12:	// Slave In
		if (m_pDX12->iGoodStage1MasterIn && !m_pDX12->iGoodStage1MasterOut) {
			m_pDY12->oGoodStage1SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 13:	// Z Axis Move to Moving Up Position
		if (g_objCommon.Get_GoodTray1MasterSlaveIn()) {
			m_tGoodTray1Loop.Takt_End(nTaktZone, 1,0);
			m_tGoodTray1Loop.Takt_Start(nTaktZone, 2);
//			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 1);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 14:	// Position Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1)) {
			m_tGoodTray1Loop.Takt_End(nTaktZone, 2,0);
			//Unload_Time Log
			SYSTEMTIME time;
			GetLocalTime(&time);
			gData.sGoodTray1LoadTime.Format("%04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
			gData.dwGoodTray1LoadingTime = GetTickCount();	// Tray Loading 완료한 시간

			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 15:	// 안전 확인.
		if (m_nGoodTray2Case > 24) { 
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 16:	// Y Axis Move to Sort Picker1 Position 
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1) && m_pDX12->iGoodStage1Exist) {
			m_tGoodTray1Loop.Takt_Start(nTaktZone, 3);

			int nIdx = (m_nSortPick2Case == 21 || m_nSortPick2Case == 22 ? 2 : 1);
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Y, nIdx);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 17:	// Init Tray
		if ((g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 1) || g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 2)) && m_pDX12->iGoodStage1Exist) {
			m_tGoodTray1Loop.Takt_End(nTaktZone, 3,0);
			Init_GoodTray();
			bgs1EmptyTray = FALSE;
			gData.nGoodTrayCount++;
			gData.nGoodStageTrayNo[0] = gData.nGoodTrayCount;
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 18:	// Position Check
		if ((g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 1) || g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 2)) && g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1)) {
			m_nGoodTray1Case = 20; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;

	case 20:	// Working
		if (m_bUnloadLotEnd && Check_GoodTrayEmpty()) {
			bgs1EmptyTray = TRUE; 
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 21:	// 안전 확인.
		if (m_nGoodTray2Case > 53 || (m_nGoodTray2Case >= 0 && m_nGoodTray2Case <= 15)) {	//로드 위치로 갈때까지 대기
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 22:	// Z Axis Move to Moving Down Position
		if (g_objAJinAXL.Is_Done(AX_GOOD_STAGE1_Y) && g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1)) {
			m_tGoodTray1Loop.Takt_Start(nTaktZone, 5);
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 0);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 23:	// Position Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 0)) {
			m_tGoodTray1Loop.Takt_End(nTaktZone, 5,0);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(30000);
		}
		break;
	case 24:	// Y Axis Move to Unload Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 0)) {
			m_tGoodTray1Loop.Takt_Start(nTaktZone, 6);
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Y, 3);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 25:	// Z Axis Move to Unload Up Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 3) && m_pDX12->iGoodPortSlideClose) {
			if (m_pEquipData->bUseInlineMode && gData.bCapTrayLoad) return TRUE;

			m_tGoodTray1Loop.Takt_End(nTaktZone, 6,0);
			m_tGoodTray1Loop.Takt_Start(nTaktZone, 7);
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 3);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 26:	// Slave Out
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 3)) {
			m_tGoodTray1Loop.Takt_End(nTaktZone, 7,0);
			m_tGoodTray1Loop.Takt_Start(nTaktZone, 8);
			m_pDY12->oGoodStage1SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 27:	// Master Out
		if (!m_pDX12->iGoodStage1SlaveIn && m_pDX12->iGoodStage1SlaveOut) {
			m_pDY12->oGoodStage1MasterIn = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 28:	// Master/Slave Out Check
		if (g_objCommon.Get_GoodTray1MasterSlaveOut()) {
			m_tGoodTray1Loop.Takt_End(nTaktZone, 8,0);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 29:	// Z Axis Move to Moving Down Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 3) && !g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 3)) {	// Inter lock
			m_tGoodTray1Loop.Takt_Start(nTaktZone, 9);
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 0);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 30:	// Lot End Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 0)) {
			// Inline Mode에서는 Unload 버튼을 사용하지 않는다.
			if (!m_pEquipData->bUseInlineMode) {
				// 해당 Lot 종료이면 문을 열어 Tray 제거 할수 있게 해준다.
				if (gData.bGoodTrayLotEnd[0] && m_bUnloadLotEnd && m_nGoodTray2Case < 11) {	// 도어락 오픈 후처리 확인.
					m_pDY12->oGoodPortSlideLock = FALSE; m_pDY12->oGoodPortSlideUnlock = TRUE;
					//m_pDY15->oDoor05Unlock = TRUE;
					g_objAJinAXL.Write_Output(12); g_objAJinAXL.Write_Output(15);
					gData.bGoodTrayWait = TRUE;
				} else if (gData.bGoodTrayLotEnd[0] && !m_bUnloadLotEnd) {	// 연속랏
					m_pDY12->oGoodPortSlideLock = FALSE; m_pDY12->oGoodPortSlideUnlock = TRUE;
					//m_pDY15->oDoor05Unlock = TRUE;
					g_objAJinAXL.Write_Output(12);
					g_objAJinAXL.Write_Output(15);
					gData.bGoodTrayWait = TRUE;
				}
			}
			if (m_pEquipData->bUseInlineMode && !bgs1EmptyTray) {
				gData.bCapTrayLoad = TRUE;
				g_objCapAttach.Set_TrayLoad(gData.nCPortNo[0], 1);
			}
			if (bgs1EmptyTray) {
				gData.nCTrayCmCnt[0] = 0;
				bgs1EmptyTray = FALSE;
			}
			m_nGoodTray1Case = 50; m_tGoodTray1Loop.Set_LoopTime(5000);

			// Unloading_Time Log add
			SYSTEMTIME time;
			GetLocalTime(&time);
			gData.sGoodTray1UnloadTime.Format("%04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
			gData.dwGoodTray1UnloadingTime = GetTickCount();	// Tray Unloading 완료한 시각
			g_objLogFile.Save_UnloadingTime(1, gData.nULPNo);

			m_tGoodTray1Loop.Takt_End(nTaktZone, 9,0);
			m_strLog.Format("Good Stage1, %d", GetTickCount() - m_dwGoodTray1);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;

	case 50:	// Wait
		if (m_nGoodTray2Case > 10 && m_nGoodTray2Case < 50) m_nGoodTray1Case++;
		return TRUE;

	case 51:	// Tray Check
		if (!m_pDX12->iGoodStage1Exist && g_objCommon.Get_GoodTray1MasterSlaveOut()) {
			m_pDY12->oGoodStage1MasterIn = TRUE; m_pDY12->oGoodStage1SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 52:	// 안전 확인 및 Y Axis Move to Tray Load Position
		// Stage2 Up Check
		if ((g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1) ||
			 g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 2) ||
			 g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 3)) &&
			 g_objCommon.Get_GoodTray1MasterSlaveIn()) {
			m_tGoodTray1Loop.Takt_Start(nTaktZone, 10);
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Y, 0);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 53:	// Position Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 0)) {
			m_tGoodTray1Loop.Takt_End(nTaktZone, 10,0);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(30000);
		}
		break;
	case 54:	// 안전 확인
		if (m_nGoodTray2Case > 15 && m_nGoodTray2Case < 50) {
			m_pDY12->oGoodStage1MasterIn = FALSE; m_pDY12->oGoodStage1SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 55:	// Z Axis Move to Moving Up Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 0)) {
			double dS1Y = g_objAJinAXL.Get_Position(AX_GOOD_STAGE1_Y);
			double dS2Y = g_objAJinAXL.Get_Position(AX_GOOD_STAGE2_Y);
			double dDiff = fabs(dS1Y-dS2Y);
			if (dDiff > TRAY_WIDTH) {	// Good Stage Y Position Check
				m_tGoodTray1Loop.Takt_Start(nTaktZone, 11);
				g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 1);
				m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 56:	// Lot End면 case 0, 계속 진행하면 case 10
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1) && g_objCommon.Get_GoodTray1MasterSlaveOut()) {
			m_tGoodTray1Loop.Takt_End(nTaktZone, 11,TRUE);
			m_nGoodTray1Case = (m_bUnloadLotEnd ? 0 : 1);
			m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;

	case 80:	// 후처리
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 2)) {
			if (m_nGoodTray2Case > 20) {	// 안전 확인.
				g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 1);
				m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 81:
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1)) {
			m_nGoodTray1Case = 0; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	}

	// 15. (Error : 4500)
	if (m_tGoodTray1Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(4500 + m_nGoodTray1Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 16. (Error : 4600)
BOOL CSequenceMain::GoodTray2_Run()
{
	int nTaktZone = 16;		// Takt_Start, Takt_End
	static BOOL	bgs2EmptyTray = FALSE;

	// 양품 Tray 배출 시 도어락 및 버튼 확인 필요
	if (gData.bGoodTrayWait && m_nGoodTray2Case > 24 && m_nGoodTray2Case < 50) return TRUE;

	switch (m_nGoodTray2Case) {
	case 0:		// Start 하면 1로 바꿔준다.
		return TRUE;

	case 1:		// Tray Check
		if (m_bUnloadLotEnd) {
			m_nGoodTray2Case = 0;
		} else if (!m_pDX12->iGoodStage2Exist && g_objCommon.Get_GoodTray2MasterSlaveOut()) {
			if (m_pDX12->iGoodTrayBufferBottom && m_nEmptyTrayYCase < 21) {
				m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
			} else {
				m_tGoodTray2Loop.Set_LoopTime(5000); return TRUE;
			}
		} 
		break;
	case 2:		// Position Check & Z Axis Move to Load Support Up Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 0) && g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1)) {	// Good Tray Z축 기본위치 확인 필요.
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 2);	// Support Up
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 3:	// Good Tray Buffer Support Out
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 2)) {
			m_pDY12->oGoodTrayBuffSupport1In = FALSE; m_pDY12->oGoodTrayBuffSupport2In = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 4:	// Stage Support Down
		if (m_pDX12->iGoodTrayBuffSupport1Out && m_pDX12->iGoodTrayBuffSupport2Out) {
			if (m_tGoodTray2Loop.Waiting_Time(100)) break;

			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 4);	// Support Down
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 5:	// Good Tray Buffer Support In
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 4)) {
			m_pDY12->oGoodTrayBuffSupport1In = TRUE; m_pDY12->oGoodTrayBuffSupport2In = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 6:	// Master In
		if (m_pDX12->iGoodTrayBuffSupport1In && m_pDX12->iGoodTrayBuffSupport2In) {
			m_pDY12->oGoodStage2MasterIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 7:	// Slave In
		if (m_pDX12->iGoodStage2MasterIn && !m_pDX12->iGoodStage2MasterOut) {
			m_pDY12->oGoodStage2SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 8:	// Good Stage Moving Down
		if (g_objCommon.Get_GoodTray2MasterSlaveIn()) {
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 1);	// Moving Down
			m_nGoodTray2Case = 11; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;

// 	case 10:	// Wait for Empty Tray Loading
// 		if (m_bUnloadLotEnd && !m_pDX12->iGoodStage2Exist) m_nGoodTray2Case = 80;
// 		return TRUE;
	case 11:	// Master In
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1) && m_pDX12->iGoodStage2Exist) {
			m_dwGoodTray2 = GetTickCount();
			m_tGoodTray2Loop.Takt_Start(nTaktZone, 1, TRUE);
			m_pDY12->oGoodStage2MasterIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 12:	// Slave In
		if (m_pDX12->iGoodStage2MasterIn && !m_pDX12->iGoodStage2MasterOut) {
			m_pDY12->oGoodStage2SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 13:	// Z Axis Move to Moving Up Position
		if (g_objCommon.Get_GoodTray2MasterSlaveIn()) {
			m_tGoodTray2Loop.Takt_End(nTaktZone, 1,0);
			m_tGoodTray2Loop.Takt_Start(nTaktZone, 2);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 14:	// Position Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1)) {
			m_tGoodTray2Loop.Takt_End(nTaktZone, 2,0);
			//Unload_Time Log
			SYSTEMTIME time;
			GetLocalTime(&time);
			gData.sGoodTray2LoadTime.Format("%04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
			gData.dwGoodTray2LoadingTime = GetTickCount();	// Tray Loading 완료한 시간

			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 15:	// 안전 확인.
		if (m_nGoodTray1Case > 24) {
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 16:	// Y Axis Move to Sort Picker1 Position 
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 0) && g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1) && m_pDX12->iGoodStage2Exist) {
			m_tGoodTray2Loop.Takt_Start(nTaktZone, 3);

			int nIdx = (m_nSortPick2Case == 21 || m_nSortPick2Case == 22 ? 2: 1);
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Y, nIdx);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 17:	// Z Axis Move to Load Up Position
		if ((g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 1) || g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 2)) && m_pDX12->iGoodStage2Exist) {
			m_tGoodTray2Loop.Takt_End(nTaktZone, 3,0);
			Init_GoodTray();
			bgs2EmptyTray = FALSE;
			gData.nGoodTrayCount++;
			gData.nGoodStageTrayNo[1] = gData.nGoodTrayCount;
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 18:	// Position Check
		if ((g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 1) || g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 2)) && g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1)) {
			m_nGoodTray2Case = 20; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;

	case 20:	// Working
		if (m_bUnloadLotEnd && Check_GoodTrayEmpty()) {
			bgs2EmptyTray = TRUE; 
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 21:	// 안전 확인.
		if (m_nGoodTray1Case > 53 || (m_nGoodTray1Case >= 0 && m_nGoodTray1Case <= 15) ) {	//로드 위치로 갈때까지 대기
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 22:	// Z Axis Move to Moving Up Position
		if (g_objAJinAXL.Is_Done(AX_GOOD_STAGE2_Y) && g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1)) {
			m_tGoodTray2Loop.Takt_Start(nTaktZone, 5);
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 0);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 23:	// Position Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 0)) {
			m_tGoodTray2Loop.Takt_End(nTaktZone, 5,0);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(30000);
		}
		break;
	case 24:	// Y Axis Move to Unload Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 0)) {
			m_tGoodTray2Loop.Takt_Start(nTaktZone, 6);
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Y, 3);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 25:	// Z Axis Move to Unload Up Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 3) && m_pDX12->iGoodPortSlideClose) {
			if (m_pEquipData->bUseInlineMode && gData.bCapTrayLoad) return TRUE;

			m_tGoodTray2Loop.Takt_End(nTaktZone, 6,0);
			m_tGoodTray2Loop.Takt_Start(nTaktZone, 7);
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 3);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 26:	// Slave Out
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 3)) {
			m_tGoodTray2Loop.Takt_End(nTaktZone, 7,0);
			m_tGoodTray2Loop.Takt_Start(nTaktZone, 8);
			m_pDY12->oGoodStage2SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 27:	// Master Out
		if (!m_pDX12->iGoodStage2SlaveIn && m_pDX12->iGoodStage2SlaveOut) {
			m_pDY12->oGoodStage2MasterIn = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 28:	// Master/Slave Out Check
		if (g_objCommon.Get_GoodTray2MasterSlaveOut()) {
			m_tGoodTray2Loop.Takt_End(nTaktZone, 8,0);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 29:	// Z Axis Move to Moving Down Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 3) && !g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 3)) {	// Inter lock
			m_tGoodTray2Loop.Takt_Start(nTaktZone, 9);
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 0);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 30:	// Lot End Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 0)) {
			// Inline Mode에서는 Unload 버튼을 사용하지 않는다.
			if (!m_pEquipData->bUseInlineMode) {
				// 해당 Lot 종료이면 문을 열어 Tray 제거 할수 있게 해준다.
				if (gData.bGoodTrayLotEnd[1] && m_bUnloadLotEnd && m_nGoodTray1Case < 11) {	// 도어락 오픈 후처리 해야할 상황인지 확인.
					m_pDY12->oGoodPortSlideLock = FALSE; m_pDY12->oGoodPortSlideUnlock = TRUE;
					//m_pDY15->oDoor05Unlock = TRUE;
					g_objAJinAXL.Write_Output(12); g_objAJinAXL.Write_Output(15);
					gData.bGoodTrayWait = TRUE;
				} else if (gData.bGoodTrayLotEnd[1] && !m_bUnloadLotEnd) {	// 연속랏
					m_pDY12->oGoodPortSlideLock = FALSE; m_pDY12->oGoodPortSlideUnlock = TRUE;
					//m_pDY15->oDoor05Unlock = TRUE;
					g_objAJinAXL.Write_Output(12);
					g_objAJinAXL.Write_Output(15);
					gData.bGoodTrayWait = TRUE;
				}
			}
			if (m_pEquipData->bUseInlineMode && !bgs2EmptyTray) {
				//if (gData.nGoodStageTrayNo[1] == 1) g_objCapAttach.Set_LotStart(gData.nCPortNo[1]);	// 15개 이하 소량 Lot일때 Cap Attach에서 문제 발생한다.
				gData.bCapTrayLoad = TRUE;
				g_objCapAttach.Set_TrayLoad(gData.nCPortNo[1], 2);
			}
			if (bgs2EmptyTray) {
				gData.nCTrayCmCnt[1] = 0;
				bgs2EmptyTray = FALSE;
			}
			m_nGoodTray2Case = 50; m_tGoodTray2Loop.Set_LoopTime(5000);

			// Unloading_Time Log add
			SYSTEMTIME time;
			GetLocalTime(&time);
			gData.sGoodTray2UnloadTime.Format("%04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
			gData.dwGoodTray2UnloadingTime = GetTickCount();	// Tray Unloading 완료한 시각
			g_objLogFile.Save_UnloadingTime(2, gData.nULPNo);

			m_tGoodTray2Loop.Takt_End(nTaktZone, 9,0);
			m_strLog.Format("Good Stage2, %d", GetTickCount() - m_dwGoodTray2);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;

	case 50:	// Wait
		if (m_nGoodTray1Case >= 10 && m_nGoodTray1Case < 50) m_nGoodTray2Case++;
		return TRUE;

	case 51:	// Tray Check
		if (!m_pDX12->iGoodStage2Exist && g_objCommon.Get_GoodTray2MasterSlaveOut()) {
			m_pDY12->oGoodStage2MasterIn = TRUE; m_pDY12->oGoodStage2SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 52:	// 안전 확인 및 Y Axis Move to Tray Load Position
		// Stage1 Up Check
		if ((g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1) ||
			 g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 2) ||
			 g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 3)) &&
			 g_objCommon.Get_GoodTray2MasterSlaveIn()) {
			m_tGoodTray2Loop.Takt_Start(nTaktZone, 10);
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Y, 0);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 53:	// Position Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 0)) {
			m_tGoodTray2Loop.Takt_End(nTaktZone, 10,0);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(30000);
		}
		break;
	case 54:	// 안전 확인
		if (m_nGoodTray1Case > 15 && m_nGoodTray1Case < 50) {
			m_pDY12->oGoodStage2MasterIn = FALSE; m_pDY12->oGoodStage2SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 55:	// Z Axis Move to Moving Up Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 0)) {
			double dS1Y = g_objAJinAXL.Get_Position(AX_GOOD_STAGE1_Y);
			double dS2Y = g_objAJinAXL.Get_Position(AX_GOOD_STAGE2_Y);
			double dDiff = fabs(dS1Y-dS2Y);
			if (dDiff > TRAY_WIDTH) {	// Good Stage Y Position Check
				m_tGoodTray2Loop.Takt_Start(nTaktZone, 11);
				g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 1);
				m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 56:	// Lot End면 case 0, 계속 진행하면 case 10
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1) && g_objCommon.Get_GoodTray2MasterSlaveOut()) {
			m_tGoodTray2Loop.Takt_End(nTaktZone, 11, TRUE);
			m_nGoodTray2Case = (m_bUnloadLotEnd ? 0 : 1);
			m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;

	case 80:	// 후처리
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 2)) {
			if (m_nGoodTray1Case > 20) {	// 안전 확인.
				g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 1);
				m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 81:
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1)) {
			m_nGoodTray2Case = 0; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	}

	// 16. (Error : 4600)
	if (m_tGoodTray2Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(4600 + m_nGoodTray2Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 17. (Error : 4700)
BOOL CSequenceMain::NgTray_Run()
{
	int nTaktZone = 17;		// Takt_Start, Takt_End

	// LotEnd되어 NG Tray 배출과정에서도 장비가 멈추면 안되는 예외 처리
	if (gData.bNGTrayWait) return TRUE;

	switch (m_nNgTrayCase) {
	case 0:		// Start 또는 버튼을 눌렀을때 1로 바뀐다.
		return TRUE;

	case 1:		// Slide Close Check
		if (m_bUnloadLotEnd) {
			m_nNgTrayCase = 0;
		} else if (m_pDX09->iNgPortSlideClose) {
			m_dwNgTray = GetTickCount();
			if (Check_NgTrayFull()) { m_nNgTrayCase = 0; g_objCommon.Show_Error(4720); return FALSE; }	// NG Full Alarm
			m_nNgTrayCase++; m_tNgTrayLoop.Set_LoopTime(10000);
		}
		break;
	case 2:		// Ready Position Check	// B접
		if (!m_pDX09->iNgStageExist1 && !m_pDX09->iNgStageExist2 && !m_pDX09->iNgStageExist3 && !m_pDX09->iNgStageExist4) {
			m_pDY09->oNgPortSlideLock = TRUE; m_pDY09->oNgPortSlideUnlock = FALSE;
			g_objAJinAXL.Write_Output(9);
			m_nNgTrayCase++; m_tNgTrayLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Tray Exist Check & Move to Sort Picker2 Tray1 Position
		if (m_pDX09->iNgPortSlideClose && m_pDX09->iNgPortSlideLock && g_objCommon.Check_Position(AX_NG_STAGE_Y, 0)) {
			m_tNgTrayLoop.Takt_Start(nTaktZone, 1, TRUE);
			g_objCommon.Move_Position(AX_NG_STAGE_Y, 3);
			m_nNgTrayCase++; m_tNgTrayLoop.Set_LoopTime(10000);
		}
		break;
	case 4:		// Position Check 
		if (g_objCommon.Check_Position(AX_NG_STAGE_Y, 3)) {
			m_tNgTrayLoop.Takt_End(nTaktZone, 1,0);
			m_nNgTrayCase = 10; m_tNgTrayLoop.Set_LoopTime(5000);
		}

	case 10:	// Working
		return TRUE;

	case 11:	// Move to Ready Position
		if (g_objAJinAXL.Is_Done(AX_NG_STAGE_Y)) {
			m_tNgTrayLoop.Takt_Start(nTaktZone, 2);
			g_objCommon.Move_Position(AX_NG_STAGE_Y, 0);
			m_nNgTrayCase++; m_tNgTrayLoop.Set_LoopTime(10000);
		}
		break;
	case 12:	// Position Check
		if (g_objCommon.Check_Position(AX_NG_STAGE_Y, 0)) {
			m_tNgTrayLoop.Takt_End(nTaktZone, 2, TRUE);
			m_pDY09->oNgPortSlideLock = FALSE;	m_pDY09->oNgPortSlideUnlock = TRUE;
			g_objAJinAXL.Write_Output(9);
			m_nNgTrayCase++; m_tNgTrayLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Unlock Check
		if (m_pDX09->iNgPortSlideUnlock) {
			m_strLog.Format("NG Stage, %d", GetTickCount() - m_dwNgTray);
			g_objLogFile.Save_TestLog(m_strLog);

			if (!m_bUnloadLotEnd && Check_NgTrayFull()) {
				gData.bNGTrayWait = TRUE; 
				g_dlgWork.PostMessage(UM_SHOW_MSG, 1, NULL);
				if (m_pThreadNgFullBeep == NULL) m_pThreadNgFullBeep = AfxBeginThread(Thread_NgFullBeep, (LPVOID)(2000));
				//g_objCommon.Show_Error(4720);	// NG Full Alarm
			} 
			else if (!m_bUnloadLotEnd)
			{
				// 장비 종료만 아니면 대기상태로 만들어 준다.
				gData.bNGTrayWait = TRUE;
				gData.bContinueLotEnd = TRUE;
				g_dlgWork.PostMessage(UM_SHOW_MSG, 2, NULL);
				
			} 
			else if (m_bUnloadLotEnd) 
			{
				gData.bNgTrayEnd = TRUE;
			}
			m_nNgTrayCase = 0; m_tNgTrayLoop.Set_LoopTime(5000);
		}
		break;
	}

	// 17. (Error : 4700)
	if (m_tNgTrayLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(4700 + m_nNgTrayCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 18. (Error : 4800)
BOOL CSequenceMain::EmptyTrayX_Run()
{
	int nTaktZone = 18;		// Takt_Start, Takt_End

	static int nEtWorkNo = 0;

	switch (m_nEmptyTrayXCase) {
	case 0:		// Start시 1로 바꿔준다.
		return TRUE;

	case 1:		// 안전 확인. 및 Down 상태에서 대기한다.
		if (m_bUnloadLotEnd) {
			g_objCommon.Set_EmptyTrayXUp();
			m_nEmptyTrayXCase = 0;
		} else if (!m_pDX13->iEmptyTrans1Exist && g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 0) &&
			g_objCommon.Get_EmptyTrayXUp() && g_objCommon.Get_EmptyTrayXMasterSlaveOut()) {
			//g_objCommon.Set_EmptyTrayXDown();
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 2:		// Angle Tray Wait
		if (m_nAngleTray1Case == 30 || m_nAngleTray2Case == 30) {
			m_dwEmptyTrayX = GetTickCount();
			if (m_nAngleTray1Case == 30) nEtWorkNo = 1;
			if (m_nAngleTray2Case == 30) nEtWorkNo = 2;
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		} else if (m_bUnloadLotEnd && Check_UnloadLotEnd()) {	// Load 할께 없으면 종료.
			g_objCommon.Set_EmptyTrayXUp();
			m_nEmptyTrayXCase = 0;
		}
		return TRUE;

	case 3:		// Empty Trans Down
		if ((nEtWorkNo == 1 && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 4)) ||
			(nEtWorkNo == 2 && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 4))) {
			m_tEmptyTrayXLoop.Takt_Start(nTaktZone, 1, TRUE);
			g_objCommon.Set_EmptyTrayXDown();
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Angle Tray Vacuum Off
		if (g_objCommon.Get_EmptyTrayXDown()) {
			m_tEmptyTrayXLoop.Takt_End(nTaktZone, 1,0);
			m_tEmptyTrayXLoop.Takt_Start(nTaktZone, 2);
			if (nEtWorkNo == 1) m_pDY02->oAngleStage1AlignIn = FALSE;
			if (nEtWorkNo == 2) m_pDY02->oAngleStage2AlignIn = FALSE;
			g_objAJinAXL.Write_Output(2);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Empty Tray Master In
		if ((nEtWorkNo == 1 && !m_pDX02->iAngleStage1AlignIn && m_pDX02->iAngleStage1AlignOut) ||
			(nEtWorkNo == 2 && !m_pDX02->iAngleStage2AlignIn && m_pDX02->iAngleStage2AlignOut)) {
			m_tEmptyTrayXLoop.Takt_End(nTaktZone, 2,0);
			m_tEmptyTrayXLoop.Takt_Start(nTaktZone, 3);
			m_pDY13->oEmptyTrans1MasterIn = TRUE; m_pDY13->oEmptyTrans1MasterOut = FALSE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 6:		// Empty Tray Slave In
		if (m_pDX13->iEmptyTrans1MasterIn && !m_pDX13->iEmptyTrans1MasterOut) {
		
			m_pDY13->oEmptyTrans1SlaveIn = TRUE; m_pDY13->oEmptyTrans1SlaveOut = FALSE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 7:		// Empty Tray Up & Angle Tray 진행.
		if (g_objCommon.Get_EmptyTrayXMasterSlaveIn()) {
			m_tEmptyTrayXLoop.Takt_End(nTaktZone, 3,0);
			m_tEmptyTrayXLoop.Takt_Start(nTaktZone, 4);
			g_objCommon.Set_EmptyTrayXUp();
			if (nEtWorkNo == 1 || m_nAngleTray1Case == 30) { m_nAngleTray1Case = 31; m_tAngleTray1Loop.Set_LoopTime(10000); }
			if (nEtWorkNo == 2 || m_nAngleTray2Case == 30) { m_nAngleTray2Case = 31; m_tAngleTray2Loop.Set_LoopTime(10000); }
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 8:		// Move to Wait Position
		if (g_objCommon.Get_EmptyTrayXUp() && m_pDX13->iEmptyTrans1Exist) {
			m_tEmptyTrayXLoop.Takt_End(nTaktZone, 4,0);
			g_objCommon.Move_Position(AX_EMPTY_TRANS1_X, 3);	// Wait Position
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 9:	// Wait Position Check
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 3)) {
			m_nEmptyTrayXCase = 10; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;

	case 10:	// Wait
		if (m_nEmptyTrayYCase == 0 || m_nEmptyTrayYCase == 2 || m_nEmptyTrayYCase >= 20) {
			// EmptyPort에 Tray가 있고 Good Stage에 Tray가 없으면 먼저 작업할수있게 기다려 준다.
			if (m_pDX01->iEmptyPortExist && m_pDX01->iEmptyPortTopCheck && (!m_pDX12->iGoodTrayBufferBottom || !m_pDX12->iGoodStage1Exist || !m_pDX12->iGoodStage2Exist))
			{
				return TRUE;
			} 
			else 
			{
				m_strLog.Format("Empty Picker X, 10");g_objLogFile.Save_MCCLog(m_strLog);
				m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
			}
		}
		return TRUE;

	case 11:	// X Move to Empty Tray Elevator Position
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 3) && g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0)) {
			m_tEmptyTrayXLoop.Takt_Start(nTaktZone, 5);
			g_objCommon.Move_Position(AX_EMPTY_TRANS1_X, 1);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(300000);	// 속도 100일때 50초
		}
		break;
	case 12:	// Position Check
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 1)) {
			m_tEmptyTrayXLoop.Takt_End(nTaktZone, 5,0);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(60000);
		}
		break;
	case 13:	// Elevator 작업 완료 확인. (30sec)
		if (m_nEmptyTrayElCase == 10) {
			double dCurPos = g_objAJinAXL.Get_Position(AX_EMPTY_PORT_Z) - m_pMoveData->dEmptyTrans1X[2];	// 현재위치 - Down Offset 하한 리미트 알람 뜨기전에 Full Alarm
			gData.dEmptyPortZDownOffset = dCurPos;
			double dReadyPos = m_pMoveData->dEmptyPortZ[1];
			if (dReadyPos < dCurPos) {
				m_strLog.Format("Empty Picker X, 13-1");g_objLogFile.Save_MCCLog(m_strLog);
				g_objAJinAXL.Move_Absolute(AX_EMPTY_PORT_Z, gData.dEmptyPortZDownOffset, 0.5);	// Down Offset Move
				m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(10000);
			} 
			else 
			{
				m_strLog.Format("Empty Picker X, 13-2");g_objLogFile.Save_MCCLog(m_strLog);
				m_nEmptyTrayElCase = 30;	// Full Alarm 처리
				m_tEmptyTrayXLoop.Set_LoopTime(10000);
			}
		}
		break;
	case 14:	// Empty Tray Down
		if (g_objAJinAXL.Is_MoveDone(AX_EMPTY_PORT_Z, gData.dEmptyPortZDownOffset) && g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 1)) 
		{
			m_strLog.Format("Empty Picker X, 14-1");g_objLogFile.Save_MCCLog(m_strLog);
			m_tEmptyTrayXLoop.Takt_Start(nTaktZone, 6);
			gData.dEmptyPortZDownOffset = 0.0;
			g_objCommon.Set_EmptyTrayXDown();
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		} 
		else if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && !g_objAJinAXL.Is_MoveDone(AX_EMPTY_PORT_Z, gData.dEmptyPortZDownOffset)) 
		{
			if (!m_tEmptyTrayXLoop.Waiting_Time(100)) break;
			m_strLog.Format("Empty Picker X, 14-2");g_objLogFile.Save_MCCLog(m_strLog);
			g_objAJinAXL.Move_Absolute(AX_EMPTY_PORT_Z, gData.dEmptyPortZDownOffset, 0.5);	// Down Offset Move
		}
		break;
	case 15:	// Empty Tray Master Out
		if (g_objCommon.Get_EmptyTrayXDown()) {
			m_tEmptyTrayXLoop.Takt_End(nTaktZone, 6,0);
			m_tEmptyTrayXLoop.Takt_Start(nTaktZone, 7);
			m_pDY13->oEmptyTrans1MasterIn = FALSE; m_pDY13->oEmptyTrans1MasterOut = TRUE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 16:	// Empty Tray Slave Out
		if (!m_pDX13->iEmptyTrans1MasterIn && m_pDX13->iEmptyTrans1MasterOut) {
			m_pDY13->oEmptyTrans1SlaveIn = FALSE; m_pDY13->oEmptyTrans1SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 17:	// Empty Tray Up
		if (g_objCommon.Get_EmptyTrayXMasterSlaveOut()) {
			m_tEmptyTrayXLoop.Takt_End(nTaktZone, 7,0);
			m_tEmptyTrayXLoop.Takt_Start(nTaktZone, 8);
			g_objCommon.Set_EmptyTrayXUp();
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 18:	// Elevator 진행.
		if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && g_objCommon.Get_EmptyTrayXUp() && !m_pDX13->iEmptyTrans1Exist) {
			if (m_nEmptyTrayElCase == 10) {
				m_tEmptyTrayXLoop.Takt_End(nTaktZone, 8,0);
				m_strLog.Format("Empty Picker X, 18");g_objLogFile.Save_MCCLog(m_strLog);

				if (m_pDX01->iEmptyPortTopCheck)
				{
					m_strLog.Format("Empty Picker X, 18-1");g_objLogFile.Save_MCCLog(m_strLog);
					m_nEmptyTrayElCase = 5;	// Slow Down
				}
				else
				{
					m_strLog.Format("Empty Picker X, 18-2");g_objLogFile.Save_MCCLog(m_strLog);
					m_nEmptyTrayElCase = 2;	// Slow Up
				}

				m_nEmptyTrayXCase = 20; m_tEmptyTrayXLoop.Set_LoopTime(5000);
			}			
		}
		break;
		
	case 20:	// X Move to Angle Tray Position
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 1)) {
			m_tEmptyTrayXLoop.Takt_Start(nTaktZone, 9);
			g_objCommon.Move_Position(AX_EMPTY_TRANS1_X, 0);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(90000);	// 속도 100일때 50초
		}
		break;
	case 21:	//Done
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 0)) {
			m_tEmptyTrayXLoop.Takt_End(nTaktZone, 9, TRUE);
			m_strLog.Format("Empty Trans X, %d", GetTickCount() - m_dwEmptyTrayX);
			g_objLogFile.Save_TestLog(m_strLog);
			m_nEmptyTrayXCase = 1; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	}

	// 18. (Error : 4800)
	if (m_tEmptyTrayXLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(4800 + m_nEmptyTrayXCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 19. (Error : 4900)
BOOL CSequenceMain::EmptyTrayElevator_Run()
{
	int nTaktZone = 19;		// Takt_Start, Takt_End
	static double dEmpty_Z;
	

	// Top check Sensor 켜져있으면 준비 완료.
	switch (m_nEmptyTrayElCase) {
	case 0:		// Start시 1로 바뀜.
		return TRUE;

	case 1:		// Tray 확인.
		if (m_bUnloadLotEnd) m_nEmptyTrayElCase = 0;
		
		else if (m_pDX01->iEmptyPortSlideClose)
		{

			gData.bEmptyZTopCheckRepeat = FALSE;
			gData.bEmptyFull = FALSE;
			if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 0))
			{
				m_strLog.Format("Empty Port Z, 1-1");g_objLogFile.Save_MCCLog(m_strLog);
				m_nEmptyTrayElCase = 20;
			}
			else if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 1))
			{
				m_strLog.Format("Empty Port Z, 1-2");g_objLogFile.Save_MCCLog(m_strLog);
				m_nEmptyTrayElCase = 21;
			}
			else if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 2, 1.5) && !m_pDX01->iEmptyPortTopCheck && !m_pDX01->iEmptyPortExist)
			{
				m_strLog.Format("Empty Port Z, 1-3");g_objLogFile.Save_MCCLog(m_strLog);
				m_nEmptyTrayElCase++;
			}
			else 
			{ 
				m_strLog.Format("Empty Port Z, 1-4");g_objLogFile.Save_MCCLog(m_strLog);
				g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 0);
				m_nEmptyTrayElCase = 20;
			}
			m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		break;

	// 1. Slow Up
	case 2:		// Elevator Z Slow Up
		dEmpty_Z = g_objAJinAXL.Get_Position(AX_EMPTY_PORT_Z);	
		if (!m_pDX01->iEmptyPortTopCheck)
		{
			m_strLog.Format("Empty Port Z, 2-1");g_objLogFile.Save_MCCLog(m_strLog);

			if (m_dwEmptyTrayEl == 0) m_dwEmptyTrayEl = GetTickCount();
			m_tEmptyTrayElLoop.Takt_Start(nTaktZone, 1, TRUE);
			g_objAJinAXL.Move_Relative(AX_EMPTY_PORT_Z, 2.0, 0.25);	// 2mm씩 올려준다.
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(20000);	// 20초
		} 
		else if(dEmpty_Z > gData.dEmptyPort_Z_Limit && g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z)) // if over limit ---> ready down 
		{
			m_strLog.Format("Empty Port Z, 2-2");g_objLogFile.Save_MCCLog(m_strLog);
			//g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z);
			g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 0);
			m_nEmptyTrayElCase = 20; m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		else if (m_pDX01->iEmptyPortTopCheck || g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 2, 1.5)) 
		{
			m_strLog.Format("Empty Port Z, 2-3");g_objLogFile.Save_MCCLog(m_strLog);
			g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z);
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Buffer Z Up Stop
		dEmpty_Z = g_objAJinAXL.Get_Position(AX_EMPTY_PORT_Z);		
		if (m_pDX01->iEmptyPortTopCheck || g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 2, 1.5))
		{
			m_strLog.Format("Empty Port Z, 3-1");g_objLogFile.Save_MCCLog(m_strLog);
			g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z);
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		else if(dEmpty_Z > gData.dEmptyPort_Z_Limit && g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z)) // if over limit ---> ready down 
		{
			m_strLog.Format("Empty Port Z, 3-2");g_objLogFile.Save_MCCLog(m_strLog);
			g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z);
			g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 0);
			m_nEmptyTrayElCase = 20; m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		else if (!m_pDX01->iEmptyPortTopCheck && g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && !g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 2, 1.0)) 
		{
			m_strLog.Format("Empty Port Z, 3-3");g_objLogFile.Save_MCCLog(m_strLog);
			m_nEmptyTrayElCase = 2; m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		break;
	case 4:
		if ((g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && m_pDX01->iEmptyPortTopCheck) || (!m_pDX01->iEmptyPortTopCheck && g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 2, 1.0))) 
		{
			gData.dEmptyPort_Z_Limit = g_objAJinAXL.Get_Position(AX_EMPTY_PORT_Z) + (2.5 * m_pMoveData->dEmptyTrans1X[2]);
			m_tEmptyTrayElLoop.Takt_End(nTaktZone, 1,0);

			gData.bEmptyZTopCheckRepeat = FALSE;
			m_strLog.Format("Empty Elevator, %d", GetTickCount() - m_dwEmptyTrayEl);
			g_objLogFile.Save_TestLog(m_strLog);
			m_dwEmptyTrayEl = 0;
			m_nEmptyTrayElCase = 10; m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		break;

	// 2. Slow Down
	case 5:		// Buffer Z Slow Down
		if (m_pDX01->iEmptyPortTopCheck)
		{
			m_strLog.Format("Empty Port Z, 5-1");g_objLogFile.Save_MCCLog(m_strLog);
			if (m_dwEmptyTrayEl == 0) m_dwEmptyTrayEl = GetTickCount();
			m_tEmptyTrayElLoop.Takt_Start(nTaktZone, 2);
			g_objAJinAXL.Move_Absolute(AX_EMPTY_PORT_Z, m_pMoveData->dEmptyPortZ[1], 0.25);	//Ready Position
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(10000);	// 10초
		} 
		else
		{
			m_strLog.Format("Empty Port Z, 5-2");g_objLogFile.Save_MCCLog(m_strLog);
			gData.bEmptyZTopCheckRepeat = FALSE;
			m_nEmptyTrayElCase = 10; m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		break;
	case 6:		// Buffer Z Down Stop
		if (!m_pDX01->iEmptyPortTopCheck) 
		{
			g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z);
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		break;
	case 7:		// Check Buffer Z Done
		if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && !m_pDX01->iEmptyPortTopCheck) 
		{
			if (!m_tEmptyTrayElLoop.Waiting_Time(300)) break;	// 센서가 정말로 꺼졌나 확인하는 시간.

			m_tEmptyTrayElLoop.Takt_End(nTaktZone, 2,0);
			m_nEmptyTrayElCase = 2; m_tEmptyTrayElLoop.Set_LoopTime(5000);
			m_strLog.Format("Empty Port Z, 7-1");g_objLogFile.Save_MCCLog(m_strLog);
		} 

		else if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && m_pDX01->iEmptyPortTopCheck) 
		{
			m_strLog.Format("Empty Port Z, 7-2");g_objLogFile.Save_MCCLog(m_strLog);
			m_nEmptyTrayElCase = 5; m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		break;

	case 10:	// Wait (EmptyTray X 또는 Y가 Start시킴)
		return TRUE;

	case 20:	// Move to Ready Position
		if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 0)) 
		{
			m_strLog.Format("Empty Port Z, 20");g_objLogFile.Save_MCCLog(m_strLog);
			g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 1);
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(10000);
		}
		break;
	case 21:	// Position Check
		if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 1)) 
		{
			m_strLog.Format("Empty Port Z, 21");g_objLogFile.Save_MCCLog(m_strLog);
			m_dwEmptyTrayEl = GetTickCount();
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(10000);
		}
		break;
	case 22:	// 트레이가 있어야하고 탑체크 감지 안되어 있어야한다.
		if (!m_pDX01->iEmptyPortTopCheck && m_pDX01->iEmptyPortSlideClose) 
		{
			m_strLog.Format("Empty Port Z, 22-1");g_objLogFile.Save_MCCLog(m_strLog);
			m_nEmptyTrayElCase = 2; m_tEmptyTrayElLoop.Set_LoopTime(20000);	// 20초
		} 
		else if (m_pDX01->iEmptyPortTopCheck) 
		{
			m_strLog.Format("Empty Port Z, 22-2");g_objLogFile.Save_MCCLog(m_strLog);
			m_nEmptyTrayElCase = 25; m_tEmptyTrayElLoop.Set_LoopTime(10000);
		}
		break;
	case 25:	// Move to Base Position
		if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 1)) 
		{
			m_tEmptyTrayElLoop.Takt_Start(nTaktZone, 3);
			g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 0);
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(10000);
		}
		break;
	case 26:	// 알람 처리
		if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 0)) {
			m_tEmptyTrayElLoop.Takt_End(nTaktZone, 3, TRUE);
			m_nEmptyTrayElCase = 0;
			gData.bEmptyFull = TRUE;
			gData.dEmptyPort_Z_Limit = m_pMoveData->dEmptyPortZ[2];
			g_objCommon.Show_Error(4926);	// Empty Tray Full Alarm
			return FALSE;
		}
		break;

	case 30:	//
		if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z)) {
			m_tEmptyTrayElLoop.Takt_Start(nTaktZone, 3);
			g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 0);
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(10000);
		}
		break;
	case 31:	// 알람 처리
		if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 0)) {
			m_tEmptyTrayElLoop.Takt_End(nTaktZone, 3, TRUE);
			m_nEmptyTrayElCase = 0; 
			gData.bEmptyFull = TRUE;
			gData.dEmptyPort_Z_Limit = m_pMoveData->dEmptyPortZ[2];
			g_objCommon.Show_Error(4932);	// Empty Tray Full Alarm
			return FALSE;
		}
		break;

	// 6. Unload (Lot End)	// Empty Tray Y에서 종료 시켜준다.
	case 51:	// Buffer Z Move to Base Down Position
		if (m_bUnloadLotEnd && m_nEmptyTrayXCase == 0) {
			m_tEmptyTrayElLoop.Takt_Start(nTaktZone, 3);
			g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 0);
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(10000);
		}
		break;
	case 52:	// Check Ready Down Position
		if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 0)) {
			m_tEmptyTrayElLoop.Takt_End(nTaktZone, 3, TRUE);
			m_nEmptyTrayElCase = 0; m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		break;
	}

	// 19. (Error : 4900)
	if (m_tEmptyTrayElLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(4900 + m_nEmptyTrayElCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 20. (Error : 5000)
BOOL CSequenceMain::EmptyTrayY_Run()
{
	int nTaktZone = 20;		// Takt_Start, Takt_End

	static int nEtWorkNo = 0;

	switch (m_nEmptyTrayYCase) {
	case 0:		// Start 시 1로 바뀐다.
		return TRUE;

	case 1:		// 안전 확인.
		if (m_bUnloadLotEnd) {
			if (m_nEmptyTrayElCase == 10 && m_nEmptyTrayXCase == 0) { m_nEmptyTrayElCase = 51; m_tEmptyTrayElLoop.Set_LoopTime(60000); }
			if (m_nEmptyTrayElCase == 0) m_nEmptyTrayYCase = 0;	//Elevator 종료 확인.
		} else if (!m_pDX13->iEmptyTrans2Exist && g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0) && g_objCommon.Get_EmptyTrayYUp() && g_objCommon.Get_EmptyTrayYMasterSlaveOut()) {
			m_dwEmptyTrayY = GetTickCount();
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(95000);	// Tray 2장 가져가는걸로 셋팅 되어있으면 무언정지 일때가 발생하여 1분35초 후 알람 나도록 설정.
		}
		break;
	case 2:		// 대기 (Btm2, Empty Tray X 확인)
		// Btm2 검사 위치 아닐때 && Empty Tray X Port 위치 아닐때 && Port에 Tray가 있을때 && Good Tray Buffer에 트레이가 없을때
		if (m_bUnloadLotEnd) {	// Load 할께 없으면 종료.
			if (m_nEmptyTrayElCase == 10 && m_nEmptyTrayXCase == 0) { m_nEmptyTrayElCase = 51; m_tEmptyTrayElLoop.Set_LoopTime(60000); }
			if (m_nEmptyTrayElCase == 0)  { m_nEmptyTrayYCase = 0; }	//Elevator 종료 확인.
		}
#ifdef EDITION_2ND
		else if (m_pDX01->iEmptyPortExist && m_pDX01->iEmptyPortTopCheck && m_nEmptyTrayXCase < 11 &&
#else
		else if (m_pDX07->iEmptyPortExist && m_pDX07->iEmptyPortTopCheck && m_nEmptyTrayXCase < 11 &&
#endif
			(m_nBtm2PickCase < 8 || m_nBtm2PickCase > 21 ||
			g_objCommon.Check_Position(AX_BTM2_PICKER_X, 2) || g_objCommon.Check_Position(AX_BTM2_PICKER_X, 3)||
			g_objCommon.Check_Position(AX_BTM2_PICKER_X, 4) || g_objCommon.Check_Position(AX_BTM2_PICKER_X, 5)) &&
			!m_pDX13->iEmptyTrans2Exist) {
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;

	case 3:		// Y Axis Move to Load Position
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0) && (g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 0) || g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 3)) &&
			(m_nBtm2PickCase < 8 || m_nBtm2PickCase > 21 ||
			 g_objCommon.Check_Position(AX_BTM2_PICKER_X, 2)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 3)||
			 g_objCommon.Check_Position(AX_BTM2_PICKER_X, 4)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 5))) {
			m_tEmptyTrayYLoop.Takt_Start(nTaktZone, 1, TRUE);
			g_objCommon.Move_Position(AX_EMPTY_TRANS2_Y, 1);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Position Check
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 1)) {
			m_tEmptyTrayYLoop.Takt_End(nTaktZone, 1,0);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(60000);
		}
		break;
	case 5:		// Angle Port 확인 (30sec)
		// Offset Up 하기전에 다시 Port 확인 해준다.
		if (m_nEmptyTrayElCase == 10)
		{
			if (!m_tEmptyTrayYLoop.Waiting_Time(100)) break;

			if (m_pDX01->iEmptyPortTopCheck)
			{
				m_strLog.Format("Empty Picker Y, 5-1");g_objLogFile.Save_MCCLog(m_strLog);
				m_nEmptyTrayElCase = 5;	// Slow Down
			}
			else
			{
				m_strLog.Format("Empty Picker Y, 5-2");g_objLogFile.Save_MCCLog(m_strLog);
				m_nEmptyTrayElCase = 2;	// Slow Up
			}

			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(30000);
		}
		break;
	case 6:		// Offset Up
		if (m_nEmptyTrayElCase == 10) {

			if (m_pDX01->iEmptyPortTopCheck) 
			{		// 23.06.25 테스트, AHN
				double dCurPos = g_objAJinAXL.Get_Position(AX_EMPTY_PORT_Z);
				double dLimitPos = m_pMoveData->dEmptyPortZ[2] + m_pMoveData->dEmptyTrans2Y[2];	// Limit Up + Up Offset 보다 위로 올라가지 못하게 해준다.

				if (dCurPos > m_pMoveData->dEmptyPortZ[2] + 0.5) 
				{
					m_strLog.Format("Empty Picker Y, 6-1");g_objLogFile.Save_MCCLog(m_strLog);
					g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 0);
					m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(10000);
				} 
				else 
				{
					m_strLog.Format("Empty Picker Y, 6-2");g_objLogFile.Save_MCCLog(m_strLog);
					gData.dEmptyPortZUpOffset = dCurPos + m_pMoveData->dEmptyTrans2Y[2];	// 현재 위치 + Up Offset
					if (gData.dEmptyPortZUpOffset > dLimitPos) gData.dEmptyPortZUpOffset = dLimitPos;
					g_objAJinAXL.Move_Absolute(AX_EMPTY_PORT_Z, gData.dEmptyPortZUpOffset, 0.5);	// Offset Move
					m_nEmptyTrayYCase = 10; m_tEmptyTrayYLoop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 7:
		if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 0)) {
			g_objCommon.Show_Error(5007);
			return FALSE;
		}
		break;

	case 10:	// Empty Tray Down
		if (g_objAJinAXL.Is_MoveDone(AX_EMPTY_PORT_Z, gData.dEmptyPortZUpOffset) && g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 1)) 
		{
			m_tEmptyTrayYLoop.Takt_Start(nTaktZone, 2);
			m_strLog.Format("Empty Picker Y, 10-1-1");g_objLogFile.Save_MCCLog(m_strLog);
			gData.dEmptyPortZUpOffset = 0.0;
			g_objCommon.Set_EmptyTrayYDown();
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);

		}
		else if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && !g_objAJinAXL.Is_MoveDone(AX_EMPTY_PORT_Z, gData.dEmptyPortZUpOffset)) 
		{
			if (!m_tEmptyTrayYLoop.Waiting_Time(100)) break;
			// 매뉴얼로 조치했을수 있기 때문에 무조건 옵셋으로 보내주지 않고 Z축 Slow Up/Down을 다시 해준다.
			// case는 6번으로 보내줘 옵셋을 다시 설정해준다.

			if (m_nEmptyTrayElCase == 10)
			{
				if (m_pDX01->iEmptyPortTopCheck)
				{
					m_strLog.Format("Empty Picker Y, 10-2-1");g_objLogFile.Save_MCCLog(m_strLog);
					m_nEmptyTrayElCase = 5;	// Slow Down
				}
				else
				{
					m_strLog.Format("Empty Picker Y, 10-2-2");g_objLogFile.Save_MCCLog(m_strLog);
					m_nEmptyTrayElCase = 2;	// Slow Up
				}

			}			
			m_nEmptyTrayYCase = 6; m_tEmptyTrayYLoop.Set_LoopTime(30000);
		}
		break;
	case 11:	// Empty Tray Master In
		if (g_objCommon.Get_EmptyTrayYDown()) {
			m_tEmptyTrayYLoop.Takt_End(nTaktZone, 2,0);
			m_tEmptyTrayYLoop.Takt_Start(nTaktZone, 3);
			m_pDY13->oEmptyTrans2MasterIn = TRUE; m_pDY13->oEmptyTrans2MasterOut = FALSE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 12:	// Empty Tray Slave In
		if (m_pDX13->iEmptyTrans2MasterIn && !m_pDX13->iEmptyTrans2MasterOut) {
			m_pDY13->oEmptyTrans2SlaveIn = TRUE; m_pDY13->oEmptyTrans2SlaveOut = FALSE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 13:	// Empty Tray Up
		if (g_objCommon.Get_EmptyTrayYMasterSlaveIn()) {
			m_tEmptyTrayYLoop.Takt_End(nTaktZone, 3,0);
			m_tEmptyTrayYLoop.Takt_Start(nTaktZone, 4);
			g_objCommon.Set_EmptyTrayYUp();
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 14:	// Angle Port 진행.
		if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && g_objCommon.Get_EmptyTrayYUp() && m_pDX13->iEmptyTrans2Exist) {
			if (m_nEmptyTrayElCase == 10) 
			{
				m_tEmptyTrayYLoop.Takt_End(nTaktZone, 4,0);

				if (m_pDX01->iEmptyPortTopCheck)
				{
					m_strLog.Format("Empty Picker Y, 14-1");g_objLogFile.Save_MCCLog(m_strLog);
					m_nEmptyTrayElCase = 5;	// Slow Down
				}
				else
				{
					m_strLog.Format("Empty Picker Y, 14-2");g_objLogFile.Save_MCCLog(m_strLog);
					m_nEmptyTrayElCase = 2;	// Slow Up
				}
				m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
			}			
		}
		break;
	case 15:	//  안전 확인
		if (m_nBtm2PickCase < 8 || m_nBtm2PickCase > 21 ||
			g_objCommon.Check_Position(AX_BTM2_PICKER_X, 2)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 3)||
			g_objCommon.Check_Position(AX_BTM2_PICKER_X, 4)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 5)) {	// Btm2 검사 위치 아닐때
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		return TRUE;
	case 16:	// Move to Good Tray Position
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 1) &&
			(m_nBtm2PickCase < 8 || m_nBtm2PickCase > 21 ||
			 g_objCommon.Check_Position(AX_BTM2_PICKER_X, 2)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 3)||
			 g_objCommon.Check_Position(AX_BTM2_PICKER_X, 4)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 5))) {
			m_tEmptyTrayYLoop.Takt_Start(nTaktZone, 5);
			g_objCommon.Move_Position(AX_EMPTY_TRANS2_Y, 0);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 17:	// Position Check
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0)) {
			m_tEmptyTrayYLoop.Takt_End(nTaktZone, 5,0);
			m_nEmptyTrayYCase = 20; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;

		// Tray Unloading
	case 20:	// Good Tray 확인
		// Buffer에 Tray가 없고 Good Stage가 Tray를 집는 동작이 아니어야한다. 
		if (!m_pDX12->iGoodTrayBufferBottom &&
			((m_nGoodTray1Case <= 1 || m_nGoodTray1Case > 10) || (m_nGoodTray2Case <= 1 || m_nGoodTray2Case > 10))) {
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}

		if (m_bUnloadLotEnd && m_pDX12->iGoodTrayBufferBottom &&
			((m_nGoodTray1Case ==  0 && m_nGoodTray2Case == 50) || (m_nGoodTray1Case == 50 && m_nGoodTray2Case ==  0))) {
			m_nEmptyTrayYCase = 40; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		return TRUE;
	case 21:	// Transfer Down
		if (m_pDX13->iEmptyTrans2Exist && g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0) &&
			m_pDX12->iGoodTrayBuffSupport1In && m_pDX12->iGoodTrayBuffSupport2In) {
			m_tEmptyTrayYLoop.Takt_Start(nTaktZone, 6);
			g_objCommon.Set_EmptyTrayYDown();
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 22:	// Slave Out
		if (g_objCommon.Get_EmptyTrayYDown()) {
			m_tEmptyTrayYLoop.Takt_End(nTaktZone, 6,0);
			m_tEmptyTrayYLoop.Takt_Start(nTaktZone, 7);
			m_pDY13->oEmptyTrans2SlaveIn = FALSE; m_pDY13->oEmptyTrans2SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 23:	// Empty Tray Master Out
		if (!m_pDX13->iEmptyTrans2SlaveIn && m_pDX13->iEmptyTrans2SlaveOut) {
			m_pDY13->oEmptyTrans2MasterIn = FALSE; m_pDY13->oEmptyTrans2MasterOut = TRUE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 24:	// Transfer Up 
		if (g_objCommon.Get_EmptyTrayYMasterSlaveOut()){
			m_tEmptyTrayYLoop.Takt_End(nTaktZone, 7,0);
			m_tEmptyTrayYLoop.Takt_Start(nTaktZone, 8);
			g_objCommon.Set_EmptyTrayYUp();
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 25:
		if (!m_pDX13->iEmptyTrans2Exist && g_objCommon.Get_EmptyTrayYUp()) {
			m_tEmptyTrayYLoop.Takt_End(nTaktZone, 8, TRUE);
			m_strLog.Format("Empty Trans Y, %d", GetTickCount() - m_dwEmptyTrayY);
			g_objLogFile.Save_TestLog(m_strLog);
			m_nEmptyTrayYCase = 1; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;

	// 후처리
	case 40:	// 안전 확인 & Move to Port Position
		if (m_nEmptyTrayXCase < 11 && (m_nBtm2PickCase < 8 || m_nBtm2PickCase > 21)) {
			m_tEmptyTrayYLoop.Takt_Start(nTaktZone, 1, TRUE);
			g_objCommon.Move_Position(AX_EMPTY_TRANS2_Y, 1);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(10000);
		}
		break;
	case 41:	// Offset Move
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 1) && m_nEmptyTrayElCase == 10) {
			m_tEmptyTrayYLoop.Takt_End(nTaktZone, 1,0);
			g_objAJinAXL.Move_Relative(AX_EMPTY_PORT_Z, m_pMoveData->dEmptyTrans2Y[2] / 2.0, 0.5);	// Offset Move
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(10000);
		}
		break;
	case 42:
		if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 1)) {
			m_tEmptyTrayYLoop.Takt_Start(nTaktZone, 2);
			g_objCommon.Set_EmptyTrayYDown();
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 43:	// Empty Tray Master Out
		if (g_objCommon.Get_EmptyTrayYDown()) {
			m_tEmptyTrayYLoop.Takt_End(nTaktZone, 2,0);
			m_tEmptyTrayYLoop.Takt_Start(nTaktZone, 7);
			m_pDY13->oEmptyTrans2MasterIn = FALSE; m_pDY13->oEmptyTrans2MasterOut = TRUE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 44:	// Empty Tray Slave Out
		if (!m_pDX13->iEmptyTrans2MasterIn && m_pDX13->iEmptyTrans2MasterOut) {
			m_pDY13->oEmptyTrans2SlaveIn = FALSE; m_pDY13->oEmptyTrans2SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 45:	// Empty Tray Up
		if (g_objCommon.Get_EmptyTrayYMasterSlaveOut()){
			m_tEmptyTrayYLoop.Takt_End(nTaktZone, 7,0);
			g_objCommon.Set_EmptyTrayYUp();
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 46:	// Angle Port 진행.
		if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && g_objCommon.Get_EmptyTrayYUp()) {
			if (m_nEmptyTrayElCase == 10) { m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000); }
		}
		break;
	case 47:	//  안전 확인
		if (m_nBtm2PickCase < 8 || m_nBtm2PickCase > 21) {	// Btm2 검사 위치 아닐때
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		return TRUE;
	case 48:	// Move to Good Tray Position
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 1)) {
			m_tEmptyTrayYLoop.Takt_Start(nTaktZone, 5);
			g_objCommon.Move_Position(AX_EMPTY_TRANS2_Y, 0);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 49:	// Position Check
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0)) {
			m_tEmptyTrayYLoop.Takt_End(nTaktZone, 5, TRUE);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 50:
		if (m_bUnloadLotEnd) {
			if (m_nEmptyTrayElCase == 10 && m_nEmptyTrayXCase == 0) { m_nEmptyTrayElCase = 51; m_tEmptyTrayElLoop.Set_LoopTime(60000); }
			if (m_nEmptyTrayElCase == 0)  { m_nEmptyTrayYCase = 0; }	//Elevator 종료 확인.
		}
		break;
	}
 
	// 20. (Error : 5000)
	if (m_tEmptyTrayYLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(5000 + m_nEmptyTrayYCase);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////





BOOL CSequenceMain::Run_Simulation()
{
#ifdef AJIN_BOARD_USE
	return TRUE;
#endif
	m_pDX01->iEmptyPortExist =TRUE;
	//m_pDX01->iEmptyPortTopCheck = TRUE;

	if(m_nEmptyTrayElCase == 2){
		m_pDX01->iEmptyPortTopCheck = FALSE;
	}


	if (m_nLoadTrayCase == 10) { // 로드트레이가 포트에서 get
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadStageExist = TRUE; 
	}
	else if(m_nLoadTrayCase == 2){ 

		g_dlgWork.FakeLoadPortSensor();
	}
		
	if (m_nLoadPickCase == 7) { //로드 피커가 로드트레이에서 get
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadPickerExist = TRUE; 
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadStageExist = FALSE; 
	}
	else if(m_nLoadPickCase == 15 ){ // 로드 피커가 angle port에 put
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadPickerExist = FALSE;
		Sleep(SIM_WAITTIMES); m_pDX02->iAnglePortExist = TRUE;
	}

	if (m_nAngleTray1Case == 4 ) { // 
		Sleep(SIM_WAITTIMES); m_pDX02->iAnglePortExist = FALSE;
		Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage1Exist = TRUE;
	}

	 // angle tray 1 대기중 btm1 피커가 픽
	if(m_nAngleTray1Case == 20 && m_nBtm1PickCase == 5){
		//Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage1Exist = FALSE;
	}
	// angle tray 2 대기중 btm1 피커가 픽
	if(m_nAngleTray2Case == 20 && m_nBtm1PickCase == 5){
		//Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage2Exist = FALSE;
	}

	if (m_nAngleTray2Case == 4 ) { 
		Sleep(SIM_WAITTIMES); m_pDX02->iAnglePortExist = FALSE;
		Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage2Exist = TRUE;
	}

	if(m_nEmptyTrayXCase == 6 ){
		Sleep(SIM_WAITTIMES); m_pDX13->iEmptyTrans1Exist = TRUE;
		if(m_nAngleTray1Case == 30 ) m_pDX02->iAngleStage1Exist = FALSE;
		if(m_nAngleTray2Case == 30 ) m_pDX02->iAngleStage2Exist = FALSE;

	}
	if(m_nEmptyTrayXCase == 17 ){
		Sleep(SIM_WAITTIMES); m_pDX13->iEmptyTrans1Exist = FALSE;
		m_pDX01->iEmptyPortExist = TRUE;
		m_pDX01->iEmptyPortTopCheck = TRUE;
	}	
	
	
	if(m_nEmptyTrayYCase == 2){//4
		g_objCommon.Move_Position(AX_EMPTY_TRANS2_Y, 1);
		g_objCommon.Set_EmptyTrayYDown();
		m_nEmptyTrayYCase = 11;
	}
	if(m_nEmptyTrayYCase == 14){
		m_nEmptyTrayYCase = 15;
		m_pDX01->iEmptyPortExist = FALSE;
		m_pDX01->iEmptyPortTopCheck = FALSE;
		m_pDX13->iEmptyTrans2Exist = TRUE;
	}
	if(m_nEmptyTrayYCase == 22){
		m_pDX13->iEmptyTrans2Exist = FALSE;
		m_pDX12->iGoodTrayBufferBottom = TRUE;
		iGoodTrayBufferCount++;
		iGoodTrayBufferCount++;
	}

	if(m_nGoodTray1Case == 11){
		m_pDX12->iGoodStage1Exist = TRUE;
		iGoodTrayBufferCount--;
		if(iGoodTrayBufferCount <= 0){
			m_pDX12->iGoodTrayBufferBottom = FALSE;
		}
	}

	if(m_nGoodTray2Case == 11){
		m_pDX12->iGoodStage2Exist = TRUE;
		iGoodTrayBufferCount--;
		if(iGoodTrayBufferCount <= 0){
			m_pDX12->iGoodTrayBufferBottom = FALSE;
		}
	}

	if(m_nGoodTray1Case == 27){
		m_pDX12->iGoodStage1Exist = FALSE;
		m_pDX12->iGoodPortBottom = TRUE;
	}
	if(m_nGoodTray2Case == 27){
		m_pDX12->iGoodStage2Exist = FALSE;
		m_pDX12->iGoodPortBottom = TRUE;
	}



	if(m_nInspect1Case == 0 || m_nInspect1Case == 30){
		m_pDX05->iInspectStage1Up = TRUE; // 맨처음 시작시 Up 상태로 시작 가정 
		m_pDX05->iInspectStage1Down = FALSE;
	}

	if(m_nEmptyTrayXCase == 13)
	{
		m_nEmptyTrayXCase++;
	}
	
	if(m_nEmptyTrayXCase == 18){
		m_nEmptyTrayXCase = 20;
	}
	
	
	return TRUE;
}