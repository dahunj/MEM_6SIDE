// DataManager.cpp: 구현 파일
//
#include "stdafx.h"
#include "CMI8000.h"
#include "DataManager.h"

#include "Common.h"
#include "MESInterface.h"
#include "CMI8000Dlg.h"

// CDataManager
CDataManager g_objDataManager;

CDataManager::CDataManager()
{
	Reset_EquipData();
	Reset_MoveData();
	Reset_IdleData();
}

CDataManager::~CDataManager()
{
}

void CDataManager::Reset_EquipData()
{
	m_EquipData.sEquipName = "";
	m_EquipData.nLotBarcodePort = 0;
	m_EquipData.nScreenOff = 0;
	m_EquipData.nNoWorkTime = 0;
	m_EquipData.bManualTaktTest = FALSE;
	m_EquipData.bUseDoorLock = FALSE;

	m_EquipData.bUseVisionAlign = FALSE;
	m_EquipData.bUseInspectAngle = FALSE;
	m_EquipData.bUseInspectBtm1Specular = FALSE;
	m_EquipData.bUseInspectTop1 = FALSE;
	m_EquipData.bUseInspectTop2 = FALSE;
	m_EquipData.bUseInspectBtm2 = FALSE;
	m_EquipData.bUseDispatcher = FALSE;
	m_EquipData.bUseMES = FALSE;
	m_EquipData.bUseApdAlarm = FALSE;
	m_EquipData.bUsePMTrigger = FALSE;

	m_EquipData.bUseBtm2PickUpDown = FALSE;
	m_EquipData.bUseRosSkip = FALSE;
	m_EquipData.bUseInspectSkip = FALSE;

	m_EquipData.dTrayPitchX = 0.0;
	m_EquipData.dTrayPitchY = 0.0;
	m_EquipData.dInspectPitchX = 0.0;
	m_EquipData.dInspectPitchY = 0.0;

	for (int i = 0; i < 4; i++) m_EquipData.nVacOffDelay[i] = 0;
	for (int i = 0; i < 4; i++) m_EquipData.nVacOffRepeat[i] = 0;
	for (int i = 0; i < 5; i++) m_EquipData.nDelayAdd[i] = 0;

	for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) m_EquipData.bTower[i][j] = FALSE;
	for (int i = 0; i < 2; i++) for (int j = 0; j < 6; j++) m_EquipData.bBuzzer[i][j] = FALSE;

	m_EquipData.sPasswordMt = "";
	m_EquipData.sPasswordSi = "";

	m_EquipData.bResultTestUse = FALSE;
	m_EquipData.nResultTestN1 = 0;
	m_EquipData.nResultTestN2 = 0;
	m_EquipData.nResultTestN3 = 0;
	m_EquipData.nResultTestN4 = 0;

	m_EquipData.sVisionProgVer = "0000";
	m_EquipData.sVisionParaVer = "0000";
	m_EquipData.bUseFocusLog = FALSE;
	m_EquipData.nTmrFocusLog = 100;

	m_EquipData.dTriggerStart = 0;
	m_EquipData.dTriggerEnd = 0;
	m_EquipData.dTriggerPeriod = 0;
	m_EquipData.dTriggerVel = 0;
}

void CDataManager::Reset_MoveData()
{
	for (int i = 0; i < 3; i++) m_MoveData.dLoadStageX[i];		// AXIS 00
	for (int i = 0; i < 4; i++) m_MoveData.dLoadStageZ[i];		// AXIS 01
	for (int i = 0; i < 2; i++) m_MoveData.dLoadPickerY[i];		// AXIS 02
	for (int i = 0; i < 2; i++) m_MoveData.dAngleUnitY[i];		// AXIS 03
	for (int i = 0; i < 2; i++) m_MoveData.dAngleUnitX[i];		// AXIS 04
	for (int i = 0; i < 2; i++) m_MoveData.dAngleUnitR[i];		// AXIS 05
	for (int i = 0; i < 1; i++) m_MoveData.dVisionAngleZ[i];	// AXIS 06
	for (int i = 0; i < 2; i++) m_MoveData.dVisonAlignZ[i];		// AXIS 07
	for (int i = 0; i < 5; i++) m_MoveData.dAngleStage1Y[i];	// AXIS 08
	for (int i = 0; i < 6; i++) m_MoveData.dAngleStage1Z[i];	// AXIS 09
	for (int i = 0; i < 5; i++) m_MoveData.dAngleStage2Y[i];	// AXIS 10
	for (int i = 0; i < 6; i++) m_MoveData.dAngleStage2Z[i];	// AXIS 11
	for (int i = 0; i < 9; i++) m_MoveData.dBtm1PickerX[i];		// AXIS 12
	for (int i = 0; i < 6; i++) m_MoveData.dBtm1PickerZ[i];		// AXIS 13
	for (int i = 0; i < 2; i++) m_MoveData.dBtm1PickerP1[i];	// AXIS 14
	for (int i = 0; i < 2; i++) m_MoveData.dBtm1PickerP2[i];	// AXIS 15
	for (int i = 0; i < 2; i++) m_MoveData.dBtm1ShiftY[i];		// AXIS 16
	for (int i = 0; i < 6; i++) m_MoveData.dInspectStage1X[i];	// AXIS 17
	for (int i = 0; i < 6; i++) m_MoveData.dInspectStage2X[i];	// AXIS 18
	for (int i = 0; i < 6; i++) m_MoveData.dInspectStage3X[i];	// AXIS 19
	for (int i = 0; i < 3; i++) m_MoveData.dModuleAlignY[i];	// AXIS 20
	for (int i = 0; i < 2; i++) m_MoveData.dModuleAlignZ[i];	// AXIS 21
	for (int i = 0; i < 1; i++) m_MoveData.dVisionTop1Z[i];		// AXIS 22
	for (int i = 0; i < 2; i++) m_MoveData.dTop1MirrorZ[i];		// AXIS 23
	for (int i = 0; i < 2; i++) m_MoveData.dTop2ShiftY[i];		// AXIS 24
	for (int i = 0; i < 1; i++) m_MoveData.dVisionTop2Z[i];		// AXIS 25
	for (int i = 0; i < 6; i++) m_MoveData.dBtm2PickerX[i];		// AXIS 26
	for (int i = 0; i < 4; i++) m_MoveData.dBtm2PickerZ[i];		// AXIS 27
	for (int i = 0; i < 2; i++) m_MoveData.dBtm2PickerP1[i];	// AXIS 28
	for (int i = 0; i < 2; i++) m_MoveData.dBtm2PickerP2[i];	// AXIS 29
	for (int i = 0; i < 6; i++) m_MoveData.dBufferStage1Y[i];	// AXIS 30
	for (int i = 0; i < 6; i++) m_MoveData.dBufferStage2Y[i];	// AXIS 31
	for (int i = 0; i < 7; i++) m_MoveData.dSortPicker1X[i];	// AXIS 32
	for (int i = 0; i < 6; i++) m_MoveData.dSortPicker1Z[i];	// AXIS 33
	for (int i = 0; i < 2; i++) m_MoveData.dSortPicker1P[i];	// AXIS 34
	for (int i = 0; i < 7; i++) m_MoveData.dSortPicker2X[i];	// AXIS 35
	for (int i = 0; i < 6; i++) m_MoveData.dSortPicker2Z[i];	// AXIS 36
	for (int i = 0; i < 2; i++) m_MoveData.dSortPicker2P[i];	// AXIS 37
	for (int i = 0; i < 4; i++) m_MoveData.dGoodStage1Y[i];		// AXIS 38
	for (int i = 0; i < 5; i++) m_MoveData.dGoodStage1Z[i];		// AXIS 39
	for (int i = 0; i < 4; i++) m_MoveData.dGoodStage2Y[i];		// AXIS 40
	for (int i = 0; i < 5; i++) m_MoveData.dGoodStage2Z[i];		// AXIS 41
	for (int i = 0; i < 5; i++) m_MoveData.dNgStageY[i];		// AXIS 42
	for (int i = 0; i < 4; i++) m_MoveData.dEmptyTrans1X[i];	// AXIS 43
	for (int i = 0; i < 3; i++) m_MoveData.dEmptyPortZ[i];		// AXIS 44
	for (int i = 0; i < 3; i++) m_MoveData.dEmptyTrans2Y[i];	// AXIS 45
}

void CDataManager::Reset_IdleData()
{
	m_IdleData.nCount = 0;
	for (int i = 0; i < 100; i++) m_IdleData.sCode[i] = "";
	for (int i = 0; i < 100; i++) m_IdleData.sText[i] = "";
}

void CDataManager::Reset_RosData()
{
	m_RosData.sDispatcherIp = "0.0.0.0";
	m_RosData.sHandlerIp = "0.0.0.0";
	m_RosData.nJudgeTimeOver = 2000;
	m_RosData.bJudgeSpecialNg = FALSE;
}

BOOL CDataManager::Read_EquipData()
{
	CIniFileCS INI(gData.sEnvPath + "\\EquipData.ini");
	if (!INI.Check_File()) { AfxMessageBox("EquipData.ini File Not Found!!!"); return FALSE; }
	CString strKey;

	m_EquipData.sEquipName = INI.Get_String("EQUIPMENT", "NAME", "");
	m_EquipData.nLotBarcodePort = INI.Get_Integer("EQUIPMENT", "LOT_BARCODE", 1);
	m_EquipData.nScreenOff = INI.Get_Integer("EQUIPMENT", "SCREEN_OFF", 0);
	m_EquipData.nNoWorkTime = INI.Get_Integer("EQUIPMENT", "NOWORK_TIME", 500000);
	m_EquipData.nLogDate = INI.Get_Integer("EQUIPMENT", "LOG_SAVE_DATE", 30);
	m_EquipData.bManualTaktTest = INI.Get_Bool("EQUIPMENT", "MANUAL_TAKT_TEST", FALSE);
	m_EquipData.bUseDoorLock = INI.Get_Bool("EQUIPMENT", "DOOR_LOCK", FALSE);
	gAlm.dMotionChkPos		= INI.Get_Double("EQUIPMENT", "MOTION_CHECK", 0.0);
	gData.nDoorLockTime = INI.Get_Integer("EQUIPMENT", "DOOR_LOCK_TIME", 10000);


#ifdef DRY_RUN_TEST
	m_EquipData.bUseVisionAlign = FALSE;
	m_EquipData.bUseInspectAngle = FALSE;
	m_EquipData.bUseInspectBtm1Specular = FALSE;
	m_EquipData.bUseInspectBtm1Angle = FALSE;
	m_EquipData.bUseInspectBtm13D = FALSE;
	m_EquipData.bUseInspectTop1 = FALSE;
	m_EquipData.bUseInspectTop2 = FALSE;
	m_EquipData.bUseInspectBtm2 = FALSE;
	m_EquipData.bUseDispatcher = FALSE;
	m_EquipData.bUseApdAlarm = FALSE;
	m_EquipData.bUseMES = FALSE;
#else
	m_EquipData.bUseVisionAlign = INI.Get_Bool("OPTION", "VISION_ALIGN", FALSE);
	m_EquipData.bUseInspectAngle = INI.Get_Bool("OPTION", "INSPECT_ANGLE", FALSE);
	m_EquipData.bUseInspectBtm1Specular = INI.Get_Bool("OPTION", "INSPECT_BTM_1_SP", FALSE);
	m_EquipData.bUseInspectBtm1Angle = INI.Get_Bool("OPTION", "INSPECT_BTM_1_AG", FALSE);
	m_EquipData.bUseInspectBtm13D = INI.Get_Bool("OPTION", "INSPECT_BTM_1_3D", FALSE);
	m_EquipData.bUseInspectTop1 = INI.Get_Bool("OPTION", "INSPECT_TOP_1", FALSE);
	m_EquipData.bUseInspectTop2 = INI.Get_Bool("OPTION", "INSPECT_TOP_2", FALSE);
	m_EquipData.bUseInspectBtm2 = INI.Get_Bool("OPTION", "INSPECT_BTM_2", FALSE);
	m_EquipData.bUseDispatcher = INI.Get_Bool("OPTION", "DISPATCHER", FALSE);
	m_EquipData.bUseInlineMode = INI.Get_Bool("OPTION", "INLINE_MODE", FALSE);
	m_EquipData.bUseApdAlarm = INI.Get_Bool("OPTION", "MES_APD", FALSE);
	m_EquipData.bUseMES = INI.Get_Bool("OPTION", "MES_USE", FALSE);
	m_EquipData.bUsePMTrigger = INI.Get_Bool("OPTION", "PM_TRIGGER", FALSE);
#endif
	m_EquipData.bUseBtm2PickUpDown = INI.Get_Bool("OPTION", "BTM2_PICK_UPDOWN", FALSE);

	m_EquipData.bUseRosSkip = INI.Get_Bool("OPTION", "ROS_SKIP", FALSE);
	m_EquipData.bUseInspectSkip = INI.Get_Bool("OPTION", "INSPECT_SKIP", FALSE);

	m_EquipData.sVisionProgVer = INI.Get_String("OPTION", "VIS_PROG_VER", "0000");
	m_EquipData.sVisionParaVer = INI.Get_String("OPTION", "VIS_PARA_VER", "0000");
	m_EquipData.bUseFocusLog = INI.Get_Bool("OPTION", "USE_FOCUS_LOG", FALSE);
	m_EquipData.nTmrFocusLog = INI.Get_Integer("OPTION", "TMR_FOCUS_LOG", 100);
	m_EquipData.nJigNgRate = INI.Get_Integer("OPTION", "JIG_NG_RATE", 0);
	m_EquipData.nJigNgCnt = INI.Get_Integer("OPTION", "JIG_NG_CNT", 5);

	m_EquipData.dTrayPitchX = INI.Get_Double("TRAY", "PITCH_X", 0.0);
	m_EquipData.dTrayPitchY = INI.Get_Double("TRAY", "PITCH_Y", 0.0);
	m_EquipData.dInspectPitchX = INI.Get_Double("INSPECT", "PITCH_X", 0.0);
	m_EquipData.dInspectPitchY = INI.Get_Double("INSPECT", "PITCH_Y", 0.0);
	m_EquipData.dInspectALOffset = INI.Get_Double("INSPECT", "AL_OFFSET", 0.0);
	m_EquipData.dInspectALBackOffset = INI.Get_Double("INSPECT", "AL_BACK_OFFSET", 0.0);
	m_EquipData.dInspectTop2ShiftX = INI.Get_Double("INSPECT", "TOP2_SHIFT_X", 0.0);
	m_EquipData.dInspectBTM1ShiftX = INI.Get_Double("INSPECT", "BTM1_SHIFT_X", 0.0);

	m_EquipData.dNgBufferPitchX = INI.Get_Double ("NG_BUFFER", "PITCH_X", 0.0);

	for (int i = 0; i < 4; i++) { strKey.Format("%d", i); m_EquipData.nVacOffDelay[i] = INI.Get_Integer("VAC_OFF_DELAY", strKey, 30); }
	for (int i = 0; i < 4; i++) { strKey.Format("%d", i); m_EquipData.nVacOffRepeat[i] = INI.Get_Integer("VAC_OFF_REPEAT", strKey, 3); }
	for (int i = 0; i < 4; i++) { strKey.Format("%d", i); m_EquipData.nDelayAdd[i] = INI.Get_Integer("DELAY_ADD", strKey, 100); }
	for (int i = 4; i < 5; i++) { strKey.Format("%d", i); m_EquipData.nDelayAdd[i] = INI.Get_Integer("DELAY_ADD", strKey, 30000); }	// Sort Picker 검사 대기 기본 30초
	
	for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) { strKey.Format("%d%d", i, j); m_EquipData.bTower[i][j] = INI.Get_Bool("TOWER", strKey, FALSE); }
	for (int i = 0; i < 2; i++) for (int j = 0; j < 6; j++) { strKey.Format("%d%d", i, j); m_EquipData.bBuzzer[i][j] = INI.Get_Bool("BUZZER", strKey, FALSE); }

	m_EquipData.sPasswordMt = INI.Get_String("HIDDEN", "PASSWORD_MT", "");
	m_EquipData.sPasswordSi = INI.Get_String("HIDDEN", "PASSWORD_SI", "");

	m_EquipData.nResultTestN1 = INI.Get_Integer("RESULT_TEST", "N1", 0);
	m_EquipData.nResultTestN2 = INI.Get_Integer("RESULT_TEST", "N2", 0);
	m_EquipData.nResultTestN3 = INI.Get_Integer("RESULT_TEST", "N3", 0);
	m_EquipData.nResultTestN4 = INI.Get_Integer("RESULT_TEST", "N4", 0);

	m_EquipData.sCapAttachIp = INI.Get_String("CAPATTACH", "CAP_IP", "");

	m_EquipData.dTriggerStart = INI.Get_Double("TRIGGER", "START", 0.0);
	m_EquipData.dTriggerEnd = INI.Get_Double("TRIGGER", "END", 0.0);
	m_EquipData.dTriggerPeriod = INI.Get_Double("TRIGGER", "PERIOD", 0.0);
	m_EquipData.dTriggerVel = INI.Get_Double("TRIGGER", "WIDTH", 0.0);

	gData.nCmMaxCount = gData.nTrayX * gData.nTrayY;		// Tray 배열 (3X15)

	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Display_EquipName();

	// Log 표준화
	gData.nStdEqNo = INI.Get_Integer("STD_LOG", "STD_EQ_NO", 0);
	gData.nStdLotNo = INI.Get_Integer("STD_LOG", "STD_LOT_NO", 0);
	gData.nStdLogDay = INI.Get_Integer("STD_LOG", "STD_LOG_DAY", 0);

	return TRUE;
}

BOOL CDataManager::Read_MoveData()
{
	CIniFileCS INI(gData.sEnvPath + "\\MoveData.ini");
	if (!INI.Check_File()) { AfxMessageBox("MoveData.ini File Not Found!!!"); return FALSE; }

	CString strKey;

	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i); m_MoveData.dLoadStageX[i]     = INI.Get_Double("00_LOAD_STAGE_X", strKey, 0.0); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i); m_MoveData.dLoadStageZ[i]     = INI.Get_Double("01_LOAD_STAGE_Z", strKey, 0.0); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i); m_MoveData.dLoadPickerY[i]    = INI.Get_Double("02_LOAD_PICKER_Y", strKey, 0.0); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i); m_MoveData.dAngleUnitY[i]     = INI.Get_Double("03_ANGLE_UNIT_Y", strKey, 0.0); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i); m_MoveData.dAngleUnitX[i]     = INI.Get_Double("04_ANGLE_UNIT_X", strKey, 0.0); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i); m_MoveData.dAngleUnitR[i]     = INI.Get_Double("05_ANGLE_UNIT_R", strKey, 0.0); }
	for (int i = 0; i < 1; i++) { strKey.Format("%02d", i); m_MoveData.dVisionAngleZ[i]   = INI.Get_Double("06_VISION_ANGLE_Z", strKey, 0.0); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i); m_MoveData.dVisonAlignZ[i]    = INI.Get_Double("07_VISION_ALIGN_Z", strKey, 0.0); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i); m_MoveData.dAngleStage1Y[i]   = INI.Get_Double("08_ANGLE_STAGE1_Y", strKey, 0.0); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i); m_MoveData.dAngleStage1Z[i]   = INI.Get_Double("09_ANGLE_STAGE1_Z", strKey, 0.0); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i); m_MoveData.dAngleStage2Y[i]   = INI.Get_Double("10_ANGLE_STAGE2_Y", strKey, 0.0); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i); m_MoveData.dAngleStage2Z[i]   = INI.Get_Double("11_ANGLE_STAGE2_Z", strKey, 0.0); }
	for (int i = 0; i < 9; i++) { strKey.Format("%02d", i); m_MoveData.dBtm1PickerX[i]    = INI.Get_Double("12_BTM1_PICKER_X", strKey, 0.0); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i); m_MoveData.dBtm1PickerZ[i]    = INI.Get_Double("13_BTM1_PICKER_Z", strKey, 0.0); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i); m_MoveData.dBtm1PickerP1[i]   = INI.Get_Double("14_BTM1_PICKER_P1", strKey, 0.0); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i); m_MoveData.dBtm1PickerP2[i]   = INI.Get_Double("15_BTM1_PICKER_P2", strKey, 0.0); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i); m_MoveData.dBtm1ShiftY[i]     = INI.Get_Double("16_BTM1_SHIFT_Y", strKey, 0.0); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i); m_MoveData.dInspectStage1X[i] = INI.Get_Double("17_INSPECT_STAGE1_X", strKey, 0.0); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i); m_MoveData.dInspectStage2X[i] = INI.Get_Double("18_INSPECT_STAGE2_X", strKey, 0.0); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i); m_MoveData.dInspectStage3X[i] = INI.Get_Double("19_INSPECT_STAGE3_X", strKey, 0.0); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i); m_MoveData.dModuleAlignY[i]   = INI.Get_Double("20_MODULE_ALIGN_Y", strKey, 0.0); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i); m_MoveData.dModuleAlignZ[i]   = INI.Get_Double("21_MODULE_ALIGN_Z", strKey, 0.0); }
	for (int i = 0; i < 1; i++) { strKey.Format("%02d", i); m_MoveData.dVisionTop1Z[i]    = INI.Get_Double("22_VISION_TOP1_Z", strKey, 0.0); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i); m_MoveData.dTop1MirrorZ[i]    = INI.Get_Double("23_TOP1_MIRROR_Z", strKey, 0.0); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i); m_MoveData.dTop2ShiftY[i]     = INI.Get_Double("24_TOP2_SHIFT_Y", strKey, 0.0); }
	for (int i = 0; i < 1; i++) { strKey.Format("%02d", i); m_MoveData.dVisionTop2Z[i]    = INI.Get_Double("25_VISION_TOP2_Z", strKey, 0.0); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i); m_MoveData.dBtm2PickerX[i]    = INI.Get_Double("26_BTM2_PICKER_X", strKey, 0.0); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i); m_MoveData.dBtm2PickerZ[i]    = INI.Get_Double("27_BTM2_PICKER_Z", strKey, 0.0); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i); m_MoveData.dBtm2PickerP1[i]   = INI.Get_Double("28_BTM2_PICKER_P1", strKey, 0.0); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i); m_MoveData.dBtm2PickerP2[i]   = INI.Get_Double("29_BTM2_PICKER_P2", strKey, 0.0); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i); m_MoveData.dBufferStage1Y[i]  = INI.Get_Double("30_BUFFER_STAGE1_Y", strKey, 0.0); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i); m_MoveData.dBufferStage2Y[i]  = INI.Get_Double("31_BUFFER_STAGE2_Y", strKey, 0.0); }
	for (int i = 0; i < 7; i++) { strKey.Format("%02d", i); m_MoveData.dSortPicker1X[i]   = INI.Get_Double("32_SORT_PICKER1_X", strKey, 0.0); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i); m_MoveData.dSortPicker1Z[i]   = INI.Get_Double("33_SORT_PICKER1_Z", strKey, 0.0); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i); m_MoveData.dSortPicker1P[i]   = INI.Get_Double("34_SORT_PICKER1_P", strKey, 0.0); }
	for (int i = 0; i < 7; i++) { strKey.Format("%02d", i); m_MoveData.dSortPicker2X[i]   = INI.Get_Double("35_SORT_PICKER2_X", strKey, 0.0); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i); m_MoveData.dSortPicker2Z[i]   = INI.Get_Double("36_SORT_PICKER2_Z", strKey, 0.0); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i); m_MoveData.dSortPicker2P[i]   = INI.Get_Double("37_SORT_PICKER2_P", strKey, 0.0); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i); m_MoveData.dGoodStage1Y[i]    = INI.Get_Double("38_GOOD_STAGE1_Y", strKey, 0.0); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i); m_MoveData.dGoodStage1Z[i]    = INI.Get_Double("39_GOOD_STAGE1_Z", strKey, 0.0); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i); m_MoveData.dGoodStage2Y[i]    = INI.Get_Double("40_GOOD_STAGE2_Y", strKey, 0.0); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i); m_MoveData.dGoodStage2Z[i]    = INI.Get_Double("41_GOOD_STAGE2_Z", strKey, 0.0); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i); m_MoveData.dNgStageY[i]       = INI.Get_Double("42_NG_STAGE_Y", strKey, 0.0); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i); m_MoveData.dEmptyTrans1X[i]   = INI.Get_Double("43_EMPTY_TRANS1_X", strKey, 0.0); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i); m_MoveData.dEmptyPortZ[i]     = INI.Get_Double("44_EMPTY_PORT_Z", strKey, 0.0); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i); m_MoveData.dEmptyTrans2Y[i]   = INI.Get_Double("45_EMPTY_TRANS2_Y", strKey, 0.0); }

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

BOOL CDataManager::Read_IdleData()
{
	CIniFileCS INI(gData.sEnvPath + "\\IdleCodeList.ini");
	if (!INI.Check_File()) { AfxMessageBox("IdleCodeList.ini File Not Found!!!"); return FALSE; }

	char chSep = ',';
	CString strKey, sData, sCode, sText;

	m_IdleData.nCount = INI.Get_Integer("IDELCODE", "COUNT", 0);
	if (m_IdleData.nCount < 1 || m_IdleData.nCount > 99) return FALSE;

	for (int i = 0; i < m_IdleData.nCount; i++) {
		strKey.Format("%02d", i + 1);
		sData = INI.Get_String("IDELCODE", strKey, "");

		AfxExtractSubString(sCode, sData, 0, chSep);
		AfxExtractSubString(sText, sData, 1, chSep);

		m_IdleData.sCode[i] = sCode;
		m_IdleData.sText[i] = sText;
	}

	return TRUE;
}

BOOL CDataManager::Read_RosData()
{
	CIniFileCS INI(gData.sEnvPath + "\\RosData.ini");
	if (!INI.Check_File()) { AfxMessageBox("RosData.ini File Not Found!!!"); return FALSE; }

	m_RosData.sDispatcherIp = INI.Get_String("DISPATCHER", "DISPATCHER_IP", "0.0.0.0");
	m_RosData.sHandlerIp = INI.Get_String("DISPATCHER", "HANDLER_IP", "0.0.0.0");

	m_RosData.nJudgeTimeOver = INI.Get_Integer("DISPATCHER", "TIME_OVER", 2000);
	m_RosData.bJudgeSpecialNg = INI.Get_Bool("DISPATCHER", "SPECIAL_NG", FALSE);

	return TRUE;
}

BOOL CDataManager::Read_ModelData()
{
	CIniFileCS INI(gData.sEnvPath + "\\ModelData.ini");
	if (!INI.Check_File()) { AfxMessageBox("ModelData.ini File Not Found!!!"); return FALSE; }
	
	CString strModel = INI.Get_String("MODEL", "NAME", "");
	gData.sRecipe = (strModel == "" ? "R53B" : strModel);	// Default(R63B)

	gData.nTrayX = INI.Get_Integer("DATA", "TRAY_X", 4);
	gData.nTrayY = INI.Get_Integer("DATA", "TRAY_Y", 3);
	gData.nScanCnt = INI.Get_Integer("DATA", "SCAN_CNT", 3);
	gData.nBtmPickQt = INI.Get_Integer("DATA", "BTM_PICK_QT", 4);
	gData.nSortPickQt = INI.Get_Integer("DATA", "SORT_PICK_QT", 4);

	// RMS 항목 저장
	CString strData;
	strData.Format("%d", gData.nTrayX);			g_objMES.Save_AviRmsData("TRAY_X", strData);
	strData.Format("%d", gData.nTrayY);			g_objMES.Save_AviRmsData("TRAY_Y", strData);
	strData.Format("%d", gData.nBtmPickQt * 2);	g_objMES.Save_AviRmsData("BTM_PICK_QT", strData);
	strData.Format("%d", gData.nSortPickQt);	g_objMES.Save_AviRmsData("SORT_PICK_QT", strData);

	return TRUE;
}

void CDataManager::Save_ModelData()
{
	CIniFileCS INI(gData.sEnvPath + "\\ModelData.ini");
	if (!INI.Check_File()) { AfxMessageBox("ModelData.ini File Not Found!!!"); return; }

	INI.Set_String("MODEL", "NAME", gData.sRecipe);
}

///////////////////////////////////////////////////////////////////////////////
